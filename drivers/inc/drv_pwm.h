/**
  ******************************************************************************
  * @file    drv_pwm.h
  * @brief   Custom PWM driver for the Arduino D7 output on NUCLEO-C5A3ZG.
  *
  *          This is project-owned code, not Cube-generated. It wraps the
  *          generated TIM1 initialisation (generated/hal/mx_tim1.c) and owns
  *          the D7 output for the whole firmware:
  *
  *            Arduino D7 -> CN9 pin 8 -> PA8 -> TIM1_CH1 (AF1), 3.3 V logic
  *
  *          Duty cycle is expressed in per-mille (0..1000) rather than percent
  *          so that callers can ramp in fractions of a percent without
  *          floating point. The carrier frequency is fixed at init time and is
  *          never touched by a duty-cycle change.
  *
  *          Anything in this firmware that needs to drive D7 must go through
  *          this driver. Do not call HAL_TIM_* on TIM1 directly and do not
  *          reconfigure PA8 elsewhere - the driver assumes it is the only
  *          owner of both.
  ******************************************************************************
  */
#ifndef DRV_PWM_H
#define DRV_PWM_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------
 * Duty-cycle scale
 * ---------------------------------------------------------------------- */

/** Fully off. */
#define DRV_PWM_DUTY_MIN        0U

/** Fully on. Duty is per-mille, so 1000 = 100.0%, 25 = 2.5%. */
#define DRV_PWM_DUTY_MAX        1000U

/** Convenience: whole percent -> per-mille. */
#define DRV_PWM_PERCENT(p)      ((uint16_t)((p) * 10U))

/* -------------------------------------------------------------------------
 * Status
 * ---------------------------------------------------------------------- */
typedef enum
{
  DRV_PWM_OK = 0,        /*!< Success                                        */
  DRV_PWM_ERR_PARAM,     /*!< Argument out of range                          */
  DRV_PWM_ERR_STATE,     /*!< Called in the wrong order (e.g. before init)   */
  DRV_PWM_ERR_HW         /*!< Underlying HAL call failed                     */
} drv_pwm_status_t;

/* -------------------------------------------------------------------------
 * Lifecycle
 * ---------------------------------------------------------------------- */

/**
  * @brief  Brings up TIM1_CH1 and PA8 for PWM output on D7.
  *
  *         Calls the Cube-generated mx_tim1_init() for the timer and pin
  *         configuration, then latches the carrier frequency and forces the
  *         duty cycle to 0% so the output starts low. The timer is configured
  *         but not yet running - call drv_pwm_start().
  *
  *         Safe to call twice; the second call is a no-op returning DRV_PWM_OK.
  *
  * @retval DRV_PWM_OK   Output is configured and idle at 0% duty.
  * @retval DRV_PWM_ERR_HW  Timer or GPIO configuration failed.
  */
drv_pwm_status_t drv_pwm_init(void);

/**
  * @brief  Releases TIM1 and returns PA8 to its reset state.
  * @retval DRV_PWM_OK    Always, even if the driver was never initialised.
  */
drv_pwm_status_t drv_pwm_deinit(void);

/* -------------------------------------------------------------------------
 * Run control
 * ---------------------------------------------------------------------- */

/**
  * @brief  Starts the carrier. The pin begins toggling at the configured
  *         frequency with whatever duty cycle is currently set.
  * @retval DRV_PWM_OK
  * @retval DRV_PWM_ERR_STATE  drv_pwm_init() has not been called.
  * @retval DRV_PWM_ERR_HW     The HAL refused to start the timer.
  */
drv_pwm_status_t drv_pwm_start(void);

/**
  * @brief  Stops the carrier and leaves the pin in its inactive (low) state.
  *         The duty-cycle setting is retained across a stop/start pair.
  * @retval DRV_PWM_OK
  * @retval DRV_PWM_ERR_STATE  drv_pwm_init() has not been called.
  * @retval DRV_PWM_ERR_HW     The HAL refused to stop the timer.
  */
drv_pwm_status_t drv_pwm_stop(void);

/* -------------------------------------------------------------------------
 * Duty cycle
 * ---------------------------------------------------------------------- */

/**
  * @brief  Sets the duty cycle without disturbing the carrier frequency.
  *
  *         Cheap enough to call from a periodic tick: it resolves to a single
  *         compare-register write. Both endpoints are exact - 0 holds the pin
  *         low for the whole period, DRV_PWM_DUTY_MAX holds it high.
  *
  * @param  permille  Duty in per-mille, DRV_PWM_DUTY_MIN..DRV_PWM_DUTY_MAX.
  * @retval DRV_PWM_OK
  * @retval DRV_PWM_ERR_PARAM  permille exceeded DRV_PWM_DUTY_MAX.
  * @retval DRV_PWM_ERR_STATE  drv_pwm_init() has not been called.
  * @retval DRV_PWM_ERR_HW     The HAL refused the compare update.
  */
drv_pwm_status_t drv_pwm_set_duty_permille(uint16_t permille);

/**
  * @brief  Requests a new PWM carrier frequency before the timer is started.
  * @param  carrier_hz  Requested carrier frequency in hertz.
  * @retval DRV_PWM_OK  Frequency accepted and timer geometry updated.
  * @retval DRV_PWM_ERR_PARAM  Frequency is zero, out of range, or not realizable.
  * @retval DRV_PWM_ERR_STATE  The timer is already running or not initialized.
  */
drv_pwm_status_t drv_pwm_set_carrier_hz(uint32_t carrier_hz);

/**
  * @brief  Returns the duty cycle last accepted by
  *         drv_pwm_set_duty_permille(), in per-mille.
  */
uint16_t drv_pwm_get_duty_permille(void);

/* -------------------------------------------------------------------------
 * Introspection
 * ---------------------------------------------------------------------- */

/**
  * @brief  Carrier frequency in Hz, as latched at init time. Constant for the
  *         lifetime of the driver.
  * @retval 0 if drv_pwm_init() has not been called.
  */
uint32_t drv_pwm_get_carrier_hz(void);

/**
  * @brief  Number of distinct duty steps the hardware can resolve, i.e. the
  *         timer period in counts. Useful for deciding a ramp step size.
  * @retval 0 if drv_pwm_init() has not been called.
  */
uint32_t drv_pwm_get_resolution(void);

/**
  * @brief  True between a successful drv_pwm_start() and the next
  *         drv_pwm_stop() or drv_pwm_deinit().
  */
bool drv_pwm_is_running(void);

#ifdef __cplusplus
}
#endif

#endif /* DRV_PWM_H */
