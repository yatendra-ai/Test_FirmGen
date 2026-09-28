/**
  ******************************************************************************
  * file           : mx_lan8742.c
  * brief          : Code generation for the LAN8742 part driver.
  ******************************************************************************
  * attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the mx_lan8742_license.md file
  * in the same directory as the generated code.
  * If no mx_lan8742_license.md file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "mx_lan8742.h"
#include "app_logger.h"

/* Private defines -----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static lan8742_obj_t  lan8742_0_obj;
/* Private function prototypes -----------------------------------------------*/
/* Functions Definition ------------------------------------------------------*/

/**
  * @brief initialize the IO layer.
  * @param  pio      pointer to the part IO structure.
  * @retval error status.
  */
int32_t lan8742_io_init(lan8742_io_t *pio)
{
  /* Part Status */
  int32_t ret = 0;

  switch (pio->id)
  {
    case LAN8742_0:
      pio->heth    = LAN8742_0_ETH_GETHANDLE();
      pio->addr    = LAN8742_0_ETH_ADDRESS;
      /* If the nRST pin is not enabled, set it to an invalid value */
      pio->nrst_pin      = LAN8742_INVALID_PIN;
      pio->nrst_active_state = 0;
      pio->it_pin      = LAN8742_INVALID_PIN;
      pio->hexti       = NULL;
      pio->irq_src     = LAN8742_INVALID_IRQ;
      break;
 
 
    default:
    /* Error -- Unknown ID */
      ret = -1;
    break;
  }

  return ret;
}

/**
  * @brief Function to retrieve the Part object LAN8742_0.
  * @retval part object.
  */
lan8742_obj_t *mx_lan8742_0_getobject(void)
{
  return &lan8742_0_obj;
}
 
 

/**
  * @brief Initialize the LAN8742 PHY device by MX_PHY_INTERFACE.
  * @param p_phy   LAN8742 PHY object pointer.
  * @param dev_id  Device ID.
  * @retval MX_PHY_STATUS_OK or MX_PHY_STATUS_ERROR.
  */
mx_phy_status_t mx_lan8742_init(lan8742_obj_t* p_phy, uint32_t dev_id)
{
  mx_phy_status_t phy_status = MX_PHY_STATUS_ERROR;
  lan8742_status_t status;
  status = lan8742_init(p_phy, dev_id);
  if(status == LAN8742_STATUS_OK)
  {
    phy_status = MX_PHY_STATUS_OK;
  }
  return phy_status;
}

/**
  * @brief Deinitialize the LAN8742 PHY device by MX_PHY_INTERFACE.
  * @param p_phy   LAN8742 PHY object pointer.
  * @retval MX_PHY_STATUS_OK or MX_PHY_STATUS_ERROR.
  */
mx_phy_status_t mx_lan8742_deinit(lan8742_obj_t* p_phy)
{
  mx_phy_status_t phy_status = MX_PHY_STATUS_ERROR;
  lan8742_status_t status;
  status = lan8742_deinit(p_phy);
  if(status == LAN8742_STATUS_OK)
  {
    phy_status = MX_PHY_STATUS_OK;
  }
  return phy_status;
}

/**
  * @brief Get the current link mode of the LAN8742 PHY by MX_PHY_INTERFACE.
  * @param p_phy   LAN8742 PHY object pointer.
  * @param p_link  Pointer to link mode structure to fill.
  */
void mx_lan8742_get_link_mode(lan8742_obj_t *p_phy, mx_phy_link_mode_t *p_link)
{
  uint16_t bsr_first = 0U;
  uint16_t bsr_second = 0U;
  uint16_t physcsr = 0U;
  lan8742_status_t bsr_first_status;
  lan8742_status_t bsr_second_status;
  lan8742_status_t physcsr_status;

  bsr_first_status = lan8742_read_c22_reg(p_phy, LAN8742_BSR, &bsr_first);
  bsr_second_status = lan8742_read_c22_reg(p_phy, LAN8742_BSR, &bsr_second);
  APP_LOG_INFO("LAN8742", "BSR first=0x%04x status=%u; second=0x%04x status=%u; link_bit=0x%04x\n",
               (unsigned int)bsr_first, (unsigned int)bsr_first_status,
               (unsigned int)bsr_second, (unsigned int)bsr_second_status,
               (unsigned int)LAN8742_BSR_LINK_STATUS);

  if ((bsr_first_status != LAN8742_STATUS_OK) ||
      (bsr_second_status != LAN8742_STATUS_OK))
  {
    MX_PHY_LINK_MODE_CLEAR(p_link);
  }
  else if ((bsr_second & LAN8742_BSR_LINK_STATUS) == 0U)
  {
    /* The vendor driver sets LAN8742_LINK_UP unconditionally after its down branch. */
    MX_PHY_LINK_MODE_CLEAR(p_link);
  }
  else if ((physcsr_status = lan8742_read_c22_reg(p_phy, LAN8742_PHYSCSR, &physcsr)) != LAN8742_STATUS_OK)
  {
    APP_LOG_INFO("LAN8742", "PHYSCSR read failed: status=%u raw=0x%04x\n",
                 (unsigned int)physcsr_status, (unsigned int)physcsr);
    MX_PHY_LINK_MODE_CLEAR(p_link);
  }
  else
  {
    APP_LOG_INFO("LAN8742", "PHYSCSR read OK: raw=0x%04x masked=0x%04x\n",
                 (unsigned int)physcsr,
                 (unsigned int)(physcsr & LAN8742_PHYSCSR_HCDSPEEDMASK));
    p_link->status = MX_PHY_LINK_UP;

    switch (physcsr & LAN8742_PHYSCSR_HCDSPEEDMASK)
    {
      case LAN8742_PHYSCSR_100BTX_FD:
        p_link->speed = MX_PHY_LINK_SPEED_100;
        p_link->duplex = MX_PHY_LINK_FULL_DUPLEX;
        break;
      case LAN8742_PHYSCSR_100BTX_HD:
        p_link->speed = MX_PHY_LINK_SPEED_100;
        p_link->duplex = MX_PHY_LINK_HALF_DUPLEX;
        break;
      case LAN8742_PHYSCSR_10BT_FD:
        p_link->speed = MX_PHY_LINK_SPEED_10;
        p_link->duplex = MX_PHY_LINK_FULL_DUPLEX;
        break;
      case LAN8742_PHYSCSR_10BT_HD:
        p_link->speed = MX_PHY_LINK_SPEED_10;
        p_link->duplex = MX_PHY_LINK_HALF_DUPLEX;
        break;
      default:
        APP_LOG_INFO("LAN8742", "PHYSCSR decode miss: raw=0x%04x masked=0x%04x mask=0x%04x; 100FD=0x%04x 100HD=0x%04x 10FD=0x%04x 10HD=0x%04x\n",
                     (unsigned int)physcsr,
                     (unsigned int)(physcsr & LAN8742_PHYSCSR_HCDSPEEDMASK),
                     (unsigned int)LAN8742_PHYSCSR_HCDSPEEDMASK,
                     (unsigned int)LAN8742_PHYSCSR_100BTX_FD,
                     (unsigned int)LAN8742_PHYSCSR_100BTX_HD,
                     (unsigned int)LAN8742_PHYSCSR_10BT_FD,
                     (unsigned int)LAN8742_PHYSCSR_10BT_HD);
        MX_PHY_LINK_MODE_CLEAR(p_link);
        break;
    }
  }
}
