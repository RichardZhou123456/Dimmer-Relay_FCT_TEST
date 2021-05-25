/**
 * @brief
 * @file lds_rf_test.c
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

#ifdef M619_BEACON
const nrf_drv_timer_t m_timer4 = NRF_DRV_TIMER_INSTANCE(2);
#else
const nrf_drv_timer_t m_timer4 = NRF_DRV_TIMER_INSTANCE(4);
#endif

xt_u32 timer4_ms_count = 0;

/**
 * @brief Handler for timer events.
 */
void ldstimer4eventHandler(nrf_timer_event_t event_type, void* p_context)
{

    switch (event_type)
    {
        case NRF_TIMER_EVENT_COMPARE0:
            //LOG_DEBUG("timer4\r\n");
            timer4_ms_count ++;
            break;

        default:
            //Do nothing.
            break;
    }
}

void LdsTimer4Init(xt_u32 time_ms)
{
	uint32_t err_code = NRF_SUCCESS;
	
	//Configure TIMER_LED for generating simple light effect - leds on board will invert his state one after the other.
	nrf_drv_timer_config_t timer_cfg = NRF_DRV_TIMER_DEFAULT_CONFIG;
	err_code = nrf_drv_timer_init(&m_timer4, &timer_cfg, ldstimer4eventHandler);
	APP_ERROR_CHECK(err_code);

	xt_u32 time_ticks = nrf_drv_timer_ms_to_ticks(&m_timer4, time_ms);

	nrf_drv_timer_extended_compare(
		 &m_timer4, NRF_TIMER_CC_CHANNEL0, time_ticks, NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, true);

	nrf_drv_timer_enable(&m_timer4);
}

xt_u32 LdsTimer4CountMsGet(void)
{
	return timer4_ms_count;
}

