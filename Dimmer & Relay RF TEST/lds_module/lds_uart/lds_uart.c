/**
 * @brief
 * @file lds_uart.c
 * @version 1.0
 * @author zwb
 * @date 2019-10-21
 * @par Copyright:
 * Copyright (c) Leedarson 2018-2028. All rights reserved.
 *
 * @history
 * 1.Date        : 2019-10-21
 *   Author      : zwb
 *   Modification: Created file
 *
 *
 */

#include "lds_includes.h"

// If you need to view serial debugging information, then "#define LOG_LOCAL_OPEN"
#define LOG_LOCAL_OPEN
#include "lds_log.h"

static xt_uart_ctrl_t uart_dev;


#define UART_TX_BUF_SIZE 1024                         /**< UART TX buffer size. */
#define UART_RX_BUF_SIZE 128                         /**< UART RX buffer size. */

#define LOG_UART_RX_TIMEOUT			10


static xt_u8 uart_rx_buffer[UART_RX_BUF_SIZE + 1];
static xt_u8 uart_rx_counter = 0;
static xt_u8 uart_rx_len = 1;


static xt_bool uart_rx_flag = UART_RX_STATUS_IDLE;


// Error handler for UART
static void ldsUartEventHandle(app_uart_evt_t * p_event)
{
    if (p_event->evt_type == APP_UART_COMMUNICATION_ERROR)
    {
        APP_ERROR_HANDLER(p_event->data.error_communication);
    }
    else if (p_event->evt_type == APP_UART_FIFO_ERROR)
    {
        APP_ERROR_HANDLER(p_event->data.error_code);
    }
	else if(p_event->evt_type == APP_UART_DATA_READY)
	{
		xt_u8 read_char;
		if(app_uart_get(&read_char) == NRF_SUCCESS)
		{
			if(uart_rx_flag == UART_RX_STATUS_IDLE) {
				uart_rx_flag = UART_RX_STATUS_START;
				uart_rx_counter = 0;
			}

			if(uart_rx_counter < UART_RX_BUF_SIZE)
			{
				uart_rx_buffer[uart_rx_counter] = read_char;

				uart_rx_counter ++;

				// rx timeout
				LdsSoftTimerStart(SOFT_TIMER_LOG_UART_RX_TIMEOUT, LOG_UART_RX_TIMEOUT);
			}
			
			if(uart_rx_counter >= UART_RX_BUF_SIZE)
			{
				uart_rx_flag = UART_RX_STATUS_FINISH;
			}

			if(uart_rx_flag == UART_RX_STATUS_FINISH)
			{
				//uart_cmd_handler(uart_rx_buffer, uart_rx_counter);
				if(uart_dev.rxCb != NULL)
				{
					uart_dev.rxCb(uart_rx_buffer, uart_rx_counter);
				}
				// clear rx flag
				uart_rx_flag = UART_RX_STATUS_IDLE;
				uart_rx_counter = 0;
				memset(uart_rx_buffer, 0, sizeof(uart_rx_buffer));
			}
		}
	}
}

/**
 * @brief function for Log Uart receive timeout handle
 *
 * @param [in]  None
 * @param [out] None
 *
 * @return 
 */
static void ldsLogUartRxTimeout(void)
{
	uart_rx_flag = UART_RX_STATUS_FINISH;
	// do command
	//LOG_DEBUG("Log rx(%d): %s\r\n",m_uart_rx_index, m_uart_rx_buffer);

	//uart_cmd_handler(uart_rx_buffer, uart_rx_counter);
	if(uart_dev.rxCb != NULL)
	{
		uart_dev.rxCb(uart_rx_buffer, uart_rx_counter);
	}
	// clear rx flag
	uart_rx_flag = UART_RX_STATUS_IDLE;
	uart_rx_counter = 0;
	memset(uart_rx_buffer, 0, sizeof(uart_rx_buffer));
}


/**@brief Function for UART initialization.
 */
void LdsUartInit(uartRxCbPtr rxCb)
{   
    uint32_t err_code;
    const app_uart_comm_params_t comm_params =
      {
          RX_PIN_NUMBER,
          TX_PIN_NUMBER,
          RTS_PIN_NUMBER,
          CTS_PIN_NUMBER,
          APP_UART_FLOW_CONTROL_DISABLED,
          false,
          DTM_BITRATE
      };
     
    APP_UART_FIFO_INIT(&comm_params,
                       UART_RX_BUF_SIZE,
                       UART_TX_BUF_SIZE,
                       ldsUartEventHandle,
                       APP_IRQ_PRIORITY_LOWEST,
                       err_code);

    APP_ERROR_CHECK(err_code);

    nrf_gpio_cfg_input(RX_PIN_NUMBER,NRF_GPIO_PIN_PULLUP);
    
	uart_dev.rxCb = rxCb;

	LdsSoftTimerCreate(SOFT_TIMER_LOG_UART_RX_TIMEOUT, ldsLogUartRxTimeout, 0, MILLI_SECONDS_TIMER);
}


