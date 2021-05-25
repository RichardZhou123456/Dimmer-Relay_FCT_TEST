/**
 * @brief
 * @file lds_main.c
 * @version 1.0
 * @author zwb
 * @date 2019-7-22
 * @par Copyright:
 * Copyright (c) Leedarson 2018-2028. All rights reserved.
 *
 * @history
 * 1.Date        : 2019-7-22
 *   Author      : zwb
 *   Modification: Created file
 * 
 * release notes：
 * 1、关闭串口调试信息： #define LOG_LEVEL              LOG_LEVEL_DISABLE
 * 2. 版本号确认
 *
 *
 */

#include "lds_includes.h"

// If you need to view serial debugging information, then "#define LOG_LOCAL_OPEN"
#define LOG_LOCAL_OPEN
#include "lds_log.h"


xt_u32 start_tick = 0;  
xt_u32 tick = 0;
xt_u32 diff_tick;
xt_u32 tick_count_ms = 0;
xt_u32 test_count_ms = 0;

/**
 * @brief function for main init
 *
 * @param [in]  None
 * @param [out] None
 *
 * @return 
 */
void ldsMainInit(void)
{
	// uart
	LdsUartInit(LdsRfTestUartRxHandle);
	
	// bsp gpio init
	LdsBspGpioInit();

	// timer4
	LdsTimer4Init(1);
	
	/* Init systick driver */
    //nrf_drv_systick_init();

	LdsRfTestModeEnter();
}


/**
 * @brief function for user rf loop test 
 *
 * @param [in]  None
 * @param [out] None
 *
 * @return 
 */
void ldsMainLoopHandle(void)
{
	start_tick = LdsTimer4CountMsGet();
    nrfx_systick_state_t sys_tick = {0};

	while(1)
	{       
		tick = LdsTimer4CountMsGet();
		if(tick != start_tick)
		{
			diff_tick = tick - start_tick;
			start_tick = LdsTimer4CountMsGet();

			LdsSoftTimerhandle(tick);

			tick_count_ms += diff_tick;
			if (tick_count_ms > 1000)
			{
				tick_count_ms = 0;

				//LOG_RF("tick time %d,%d\r\n",SystemCoreClock);
			}
		}

		LdsDtmTestWait();

		LdsRfTestHandle();
	}
}


void LdsRfTask(void)
{
	ldsMainInit();

	ldsMainLoopHandle();
}

