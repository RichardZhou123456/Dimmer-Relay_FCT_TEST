#ifndef _LDS_BSP_CONFIG_H_
#define _LDS_BSP_CONFIG_H_


// 模组脚位对于的gpio口

#define MODULE_PIN_2		    25 		// ic 37
#define MODULE_PIN_3			26 		// 38
#define MODULE_PIN_4			2  		// 4
#define MODULE_PIN_7			6 		// 8
#define MODULE_PIN_8			7 		// 9
#define MODULE_PIN_9			8 		// 10
#define MODULE_PIN_10			9 		// 11
#define MODULE_PIN_11			10 		// 12
#define MODULE_PIN_12			19 		// 22  uart TX
#define MODULE_PIN_13			20 		// 23  uart RX


#define MODULE_PIN_2_LOW()		nrf_gpio_pin_clear(MODULE_PIN_2)
#define MODULE_PIN_2_HIGH()		nrf_gpio_pin_set(MODULE_PIN_2)

#define MODULE_PIN_3_LOW()		nrf_gpio_pin_clear(MODULE_PIN_3)
#define MODULE_PIN_3_HIGH()		nrf_gpio_pin_set(MODULE_PIN_3)

#define MODULE_PIN_4_LOW()		nrf_gpio_pin_clear(MODULE_PIN_4)
#define MODULE_PIN_4_HIGH()		nrf_gpio_pin_set(MODULE_PIN_4)

#define MODULE_PIN_7_LOW()		nrf_gpio_pin_clear(MODULE_PIN_7)
#define MODULE_PIN_7_HIGH()		nrf_gpio_pin_set(MODULE_PIN_7)

#define MODULE_PIN_8_LOW()		nrf_gpio_pin_clear(MODULE_PIN_8)
#define MODULE_PIN_8_HIGH()		nrf_gpio_pin_set(MODULE_PIN_8)

#define MODULE_PIN_9_LOW()		nrf_gpio_pin_clear(MODULE_PIN_9)
#define MODULE_PIN_9_HIGH()		nrf_gpio_pin_set(MODULE_PIN_9)

#define MODULE_PIN_10_LOW()		nrf_gpio_pin_clear(MODULE_PIN_10)
#define MODULE_PIN_10_HIGH()	nrf_gpio_pin_set(MODULE_PIN_10)

#define MODULE_PIN_11_LOW()		nrf_gpio_pin_clear(MODULE_PIN_11)
#define MODULE_PIN_11_HIGH()	nrf_gpio_pin_set(MODULE_PIN_11)

#endif
