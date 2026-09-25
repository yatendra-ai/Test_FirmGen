/**
  ******************************************************************************
  * @file    drv_pwm.c
  * @brief   Custom PWM driver for the Arduino D7 output on NUCLEO-C5A3ZG.
  *
  *          Owns TIM1_CH1 / PA8. See drv_pwm.h for the contract.
  *
  *          The timer and pin configuration itself is not duplicated here -
  *          it comes from the Cube-generated mx_tim1_init() (aliased as
  *          mx_pwm_tim_init in generated/hal/mx_hal_def.h). This driver adds
  *          the run-time behaviour the generated code deliberately leaves out:
  *          a duty-cycle scale that is independent of the timer period, exact
  *          0% and 100% endpoints, and explicit start/stop state tracking.
  ******************************************************************************
  */

#include "drv_pwm.h"

#include "board.h"
#include "mx_hal_def.h"
#include "app_config.h"

/* -------------------------------------------------------------------------
 * Driver state
 * ---------------------------------------------------------------------- */

/** Handle owned by the generated TIM1 code; NULL until drv_pwm_init(). */
static hal_tim_handle_t *s_htim;

/** Timer period in counts (ARR + 1), latched at init. */
static uint32_t s_resolution;

/** Carrier frequency in Hz, derived from the hardware at init. */
static uint32_t s_carrier_hz;

/** Last duty accepted by drv_pwm_set_duty_permille(), in per-mille. */
static uint16_t s_duty_permille;

/** True between a successful start and the next stop. */
static bool s_running;

/* -------------------------------------------------------------------------
 * Helpers
 * ---------------------------------------------------------------------- */

/**
  * @brief  Converts a per-mille duty into a compare value.
  *
  *         Rounds to nearest so that a linear sweep through the per-mille
  *         range produces a monotonic sweep of compare values. Both endpoints
  *         are exact: 0 yields a compare of 0 (channel never active in PWM1
  *         mode), and DRV_PWM_DUTY_MAX yields a compare equal to the period,
  *         which is above every counter value so the channel stays active.
  *
  * @note   The multiply is done in 64-bit-free arithmetic: s_resolution is at
  *         most 65536 and permille at most 1000, so the product fits in 32
  *         bits with room to spare.
  */
static uint32_t duty_to_pulse(uint16_t permille)
{
  uint32_t pulse;

  if (permille == DRV_PWM_DUTY_MIN)
  {
    pulse = 0U;
  }
  else if (permille >= DRV_PWM_DUTY_MAX)
  {
    pulse = s_resolution;
  }
  else
  {
    pulse = ((s_resolution * (uint32_t)permille)
             + (DRV_PWM_DUTY_MAX / 2U))
            / DRV_PWM_DUTY_MAX;
  }

  return pulse;
}

/* -------------------------------------------------------------------------
 * Lifecycle
 * ---------------------------------------------------------------------- */

drv_pwm_status_t drv_pwm_init(void)
{
  if (s_htim != NULL)
  {
    /* Already initialised - idempotent by contract. */
    return DRV_PWM_OK;
  }

  /* Timer, channel and PA8 alternate-function setup all live in the
   * Cube-generated code. Anything that needs to change about the carrier
   * frequency or the pin belongs there, not here. */
  s_htim = mx_pwm_tim_init();
  if (s_htim == NULL)
  {
    return DRV_PWM_ERR_HW;
  }

  /* Latch the geometry of the carrier so duty maths never has to touch the
   * timer registers again. */
  s_resolution = HAL_TIM_GetPeriod(s_htim) + 1U;

  {
    const uint32_t tim_clk_hz = HAL_TIM_GetClockFreq(s_htim);
    const uint32_t divider    = (HAL_TIM_GetPrescaler(s_htim) + 1U) * s_resolution;

    s_carrier_hz = (divider != 0U) ? (tim_clk_hz / divider) : 0U;
  }

  /* Start from a defined state: output low, carrier stopped. */
  s_duty_permille = DRV_PWM_DUTY_MIN;

  /* The generated timer starts at the 1 kHz default.  The application may
   * override that geometry before drv_pwm_start(), so initialization must not
   * reject a later, valid configured carrier value. */
  s_running       = false;

  if (HAL_TIM_OC_SetCompareUnitPulse(s_htim,
                                     BOARD_ARD_D7_CMP_UNIT,
                                     duty_to_pulse(DRV_PWM_DUTY_MIN)) != HAL_OK)
  {
    s_htim = NULL;
    return DRV_PWM_ERR_HW;
  }

  return DRV_PWM_OK;
}

drv_pwm_status_t drv_pwm_set_carrier_hz(uint32_t carrier_hz)
{
  if (s_htim == NULL)
  {
    return DRV_PWM_ERR_STATE;
  }

  if (s_running)
  {
    return DRV_PWM_ERR_STATE;
  }

  if ((carrier_hz < APP_PWM_MIN_CARRIER_HZ)
      || (carrier_hz > APP_PWM_MAX_CARRIER_HZ))
  {
    return DRV_PWM_ERR_PARAM;
  }

  {
    const uint32_t timer_clock_hz = HAL_TIM_GetClockFreq(s_htim);
    const uint32_t prescaler = HAL_TIM_GetPrescaler(s_htim) + 1U;
    const uint32_t period = timer_clock_hz / (prescaler * carrier_hz);

    if ((period < 2U) || (period > 65536U)
        || ((timer_clock_hz % (prescaler * carrier_hz)) != 0U))
    {
      return DRV_PWM_ERR_PARAM;
    }

    if (HAL_TIM_SetPeriod(s_htim, period - 1U) != HAL_OK)
    {
      return DRV_PWM_ERR_HW;
    }

    s_resolution = period;
    s_carrier_hz = timer_clock_hz / (prescaler * period);
  }

  return DRV_PWM_OK;
}

drv_pwm_status_t drv_pwm_deinit(void)
{
  if (s_htim == NULL)
  {
    return DRV_PWM_OK;
  }

  if (s_running)
  {
    (void)drv_pwm_stop();
  }

  mx_pwm_tim_deinit();

  s_htim          = NULL;
  s_resolution    = 0U;
  s_carrier_hz    = 0U;
  s_duty_permille = DRV_PWM_DUTY_MIN;
  s_running       = false;

  return DRV_PWM_OK;
}

/* -------------------------------------------------------------------------
 * Run control
 * ---------------------------------------------------------------------- */

drv_pwm_status_t drv_pwm_start(void)
{
  if (s_htim == NULL)
  {
    return DRV_PWM_ERR_STATE;
  }

  if (s_running)
  {
    return DRV_PWM_OK;
  }

  /* Enabling the channel also asserts TIM1's main output (MOE), which an
   * advanced-control timer needs before anything reaches the pin. */
  if (HAL_TIM_OC_StartChannel(s_htim, BOARD_ARD_D7_CHANNEL) != HAL_OK)
  {
    return DRV_PWM_ERR_HW;
  }

  if (HAL_TIM_Start(s_htim) != HAL_OK)
  {
    (void)HAL_TIM_OC_StopChannel(s_htim, BOARD_ARD_D7_CHANNEL);
    return DRV_PWM_ERR_HW;
  }

  s_running = true;
  return DRV_PWM_OK;
}

drv_pwm_status_t drv_pwm_stop(void)
{
  if (s_htim == NULL)
  {
    return DRV_PWM_ERR_STATE;
  }

  if (!s_running)
  {
    return DRV_PWM_OK;
  }

  if (HAL_TIM_Stop(s_htim) != HAL_OK)
  {
    return DRV_PWM_ERR_HW;
  }

  if (HAL_TIM_OC_StopChannel(s_htim, BOARD_ARD_D7_CHANNEL) != HAL_OK)
  {
    return DRV_PWM_ERR_HW;
  }

  s_running = false;
  return DRV_PWM_OK;
}

/* -------------------------------------------------------------------------
 * Duty cycle
 * ---------------------------------------------------------------------- */

drv_pwm_status_t drv_pwm_set_duty_permille(uint16_t permille)
{
  if (s_htim == NULL)
  {
    return DRV_PWM_ERR_STATE;
  }

  if (permille > DRV_PWM_DUTY_MAX)
  {
    return DRV_PWM_ERR_PARAM;
  }

  /* Single compare-register write. The carrier keeps running and the period
   * is untouched, so the output frequency does not move. The generated TIM1
   * PWM configuration uses the timer's compare shadowing/update mechanism, so
   * this value is transferred at an update boundary rather than altering an
   * active pulse mid-period. */
  if (HAL_TIM_OC_SetCompareUnitPulse(s_htim,
                                     BOARD_ARD_D7_CMP_UNIT,
                                     duty_to_pulse(permille)) != HAL_OK)
  {
    return DRV_PWM_ERR_HW;
  }

  s_duty_permille = permille;
  return DRV_PWM_OK;
}

uint16_t drv_pwm_get_duty_permille(void)
{
  return s_duty_permille;
}

/* -------------------------------------------------------------------------
 * Introspection
 * ---------------------------------------------------------------------- */

uint32_t drv_pwm_get_carrier_hz(void)
{
  return s_carrier_hz;
}

uint32_t drv_pwm_get_resolution(void)
{
  return s_resolution;
}

bool drv_pwm_is_running(void)
{
  return s_running;
}
