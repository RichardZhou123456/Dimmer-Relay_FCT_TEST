/**
 * @brief
 * @file lds_bsp.c
 * @version 1.0
 * @author zwb
 * @date 2019-10-22
 * @par Copyright:
 * Copyright (c) Leedarson 2018-2028. All rights reserved.
 *
 * @history
 * 1.Date        : 2019-10-22
 *   Author      : zwb
 *   Modification: Created file
 * 
 *
 *
 */

#include "lds_includes.h"

/**
  * @brief  function for GPIO Even number foot output low level
  * @param  None
  * @retval None
  */
void LdsBspEvenNumberPinLow(void)
{
	MODULE_PIN_3_LOW();
	MODULE_PIN_4_LOW();
	MODULE_PIN_7_LOW();
	MODULE_PIN_9_LOW();
	MODULE_PIN_10_LOW();
	MODULE_PIN_11_LOW();
}

/**
  * @brief  function for GPIO Even number foot output high level
  * @param  None
  * @retval None
  */
void LdsBspEvenNumberPinHigh(void)
{
	MODULE_PIN_3_HIGH();
	MODULE_PIN_4_HIGH();
	MODULE_PIN_7_HIGH();
	MODULE_PIN_9_HIGH();
	MODULE_PIN_10_HIGH();
	MODULE_PIN_11_HIGH();
}

/**
  * @brief  function for GPIO Even number foot output low level
  * @param  None
  * @retval None
  */
void LdsBspOddNumberPinLow(void)
{
	MODULE_PIN_2_LOW();
	MODULE_PIN_8_LOW();
}

/**
  * @brief  function for GPIO Even number foot output high level
  * @param  None
  * @retval None
  */
void LdsBspOddNumberPinHigh(void)
{
	MODULE_PIN_2_HIGH();
	MODULE_PIN_8_HIGH();
}

/**
  * @brief  function for GPIO initialize
  * @param  None
  * @retval None
  */
void LdsBspGpioInit(void)
{
    nrf_gpio_cfg_input(2,NRF_GPIO_PIN_NOPULL);
    nrf_gpio_cfg_input(3,NRF_GPIO_PIN_NOPULL);
    nrf_gpio_cfg_input(4,NRF_GPIO_PIN_NOPULL);
    nrf_gpio_cfg_input(5,NRF_GPIO_PIN_NOPULL);
    nrf_gpio_cfg_input(6,NRF_GPIO_PIN_NOPULL);
    
    nrf_gpio_cfg_output(25);
    nrf_gpio_pin_clear(25);
#if 0
    nrf_gpio_cfg_output(MODULE_PIN_2);
    nrf_gpio_cfg_output(MODULE_PIN_3);	
    nrf_gpio_cfg_output(MODULE_PIN_4);	
    nrf_gpio_cfg_output(MODULE_PIN_7);
    nrf_gpio_cfg_output(MODULE_PIN_8);
    nrf_gpio_cfg_output(MODULE_PIN_9);
    nrf_gpio_cfg_output(MODULE_PIN_10);
    nrf_gpio_cfg_output(MODULE_PIN_11);	

    LdsBspEvenNumberPinLow();
    LdsBspOddNumberPinLow();
#endif
}

