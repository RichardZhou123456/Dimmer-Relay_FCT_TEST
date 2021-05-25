#ifndef _LDS_UART_H_
#define _LDS_UART_H_


enum xt_uart_rx_status_e
{
	UART_RX_STATUS_IDLE	= 0,
	UART_RX_STATUS_START = 1,
	UART_RX_STATUS_FINISH = 2
};

typedef void (*uartRxCbPtr)(xt_u8 *p_data, xt_u8 len);

typedef struct{ 

	uartRxCbPtr rxCb;

} xt_uart_ctrl_t;


#endif
