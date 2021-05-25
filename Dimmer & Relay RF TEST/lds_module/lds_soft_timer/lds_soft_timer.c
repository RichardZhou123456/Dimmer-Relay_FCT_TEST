/**
 * @file    lds_soft_timer.c
 * @brief   软件定时接口
 * @author  linyulan, linyulan@leedarson.com
 * @version v1.0.0
 * @date    W49 Tue, 2018-12-27 9:30:00
 * @par     Copyright
 * Copyright (c) Leedarson Lighting 2000-2018. All rights reserved.
 *
 * @par     History
 * 1.Date        :W52 Thurs, 2018-12-27 9:30:00
 *   Author      :linyulan
 *   Version     :v1.0.0
 *   Modification:Create file
 */


#include "lds_includes.h"


// If you need to view serial debugging information, then "#define LOG_LOCAL_OPEN"
#define LOG_LOCAL_OPEN
#include "lds_log.h"

extern xt_u32 LdsGetSysClockMs(void);

/*
 * Defines the wiced timer structure.
 */
typedef struct
{
    xt_u8                   flag;           // 建立时标志  0xa5 为建立OK  0为未建立
    p_timer_callback        cback;
    xt_u32                  cback_param;
    xt_timer_type_t         type;
    xt_u32                  timeout;
    xt_u32                  end_time;
    xt_process_timer_t      *S;
}xt_soft_timer_t;

static xt_soft_timer_t soft_timer_tb[SOFT_TIMER_NAME_MAX]={{0}};

static xt_u32  s_clock_time_1ms = 0;

/**
 * @brief 定时器处理 放入主循环
 *
 * @param [in] void
 * @param [out] None
 *
 * @return
 *
 */
void LdsSoftTimerhandle(xt_u32 tick_ms)
{
    static xt_u32 bac_clock_time_1ms;
    xt_u32 i;
    xt_soft_timer_t *p_timer;

    s_clock_time_1ms = tick_ms;

    /**< 定时检索*/
    if(bac_clock_time_1ms != s_clock_time_1ms)
    {
        for (i = 0; i < SOFT_TIMER_NAME_MAX; i++)
        {
            p_timer = &soft_timer_tb[i];

            if ((p_timer->timeout==0) ||
                ((s_clock_time_1ms - p_timer->end_time) < p_timer->timeout))
            {
                continue;
            }

            if (p_timer->cback)
            {
                //LDSLogPrintf("1%u\r\n",i);
                p_timer->cback();
                //LDSLogPrintf("o\r\n");
            }

            // update end_time
            switch (p_timer->type)
            {
                case SECONDS_TIMER:
                    p_timer->timeout = 0;
                    break;
                case MILLI_SECONDS_TIMER:
                    p_timer->timeout = 0;
                    break;

                case SECONDS_PERIODIC_TIMER:
                    p_timer->end_time = s_clock_time_1ms;
                    break;
                case MILLI_SECONDS_PERIODIC_TIMER:
                    p_timer->end_time = s_clock_time_1ms;
                    break;
                case MILLI_SECONDS_PERIODIC_TIMER_PROCESS:
                    {
                    	xt_process_timer_t *proccess_timer;
                        proccess_timer = p_timer->S;
                        //LDSLogPrintf("next_timeout:%u\r\n",proccess_timer->next_timeout);
                        if (proccess_timer->next_cback != NULL)
                        {
                            p_timer->end_time = s_clock_time_1ms;
                            p_timer->timeout  = proccess_timer->next_timeout;
                            p_timer->S ++;
                            proccess_timer = p_timer->S;
                            p_timer->cback    = proccess_timer->next_cback;
                        }else
                        {
                            p_timer->timeout = 0;
                        }
                    }
                    break;
                default:
                    p_timer->timeout = 0;
                    break;
            }

        }
        //LDS_LOG_DBG("s_clock_time_1ms: %d\r\n", s_clock_time_1ms);
    }
    bac_clock_time_1ms = s_clock_time_1ms;

}


/**
 * @brief 建立软件定时器
 *
 * @param [in] xt_soft_timer_t *p_timer
 * @param [in] p_timer_callback TimerCb
 * @param [in] xt_u32 cBackparam
 * @param [in] timer_type_t type
 * @param [out] None
 *
 * @return
 *
 */
xt_bool LdsSoftTimerCreate(xt_timer_port_e timer,
                        p_timer_callback TimerCb,
                        xt_u32 cBackparam,
                        xt_timer_type_t type)
{
	xt_soft_timer_t *p_timer;
		
    if((timer >= SOFT_TIMER_NAME_MAX) || (TimerCb==NULL))
    {
        return LDS_FALSE;
    }

    p_timer = &soft_timer_tb[timer];

    if(p_timer->flag == 0xa5)
    {
        // 防止重新建立定时器
        return LDS_TRUE;
    }

    p_timer->flag = 0xa5;
    p_timer->cback = TimerCb;
    p_timer->cback_param = cBackparam;
    p_timer->type = type;
    p_timer->end_time = 0;
    p_timer->timeout = 0;

	return LDS_TRUE;
}

/**
 * @brief 开启软件定时器
 *
 * @param [in] xt_soft_timer_t *p_timer
 * @param [in] xt_u32 timeout
 * @param [out] None
 *
 * @return
 *
 */
void LdsSoftTimerStart(xt_timer_port_e timer, xt_u32 timeout)
{
    xt_soft_timer_t *p_timer = &soft_timer_tb[timer];
    p_timer->timeout = timeout;
    if(p_timer->type == SECONDS_TIMER || p_timer->type == SECONDS_PERIODIC_TIMER)
    {
        p_timer->timeout = 1000*timeout;
    }
    p_timer->end_time = s_clock_time_1ms;
}

/**
 * @brief 关闭软件定时器
 *
 * @param [in] xt_soft_timer_t *p_timer
 * @param [out] None
 *
 * @return
 *
 */
void LdsSoftTimerStop(xt_timer_port_e timer)
{
    xt_soft_timer_t *p_timer = &soft_timer_tb[timer];
    p_timer->timeout = 0;
}

/**
 * @brief 多任务定时器
 *
 * @param [in] xt_soft_timer_t *p_timer
 * @param [in] process_timer_t *S_tb
 * @param [out] None
 *
 * @return
 *
 */
void LdsSoftTimerFlowStart(xt_timer_port_e timer, xt_process_timer_t *S_tb)
{
    xt_soft_timer_t *p_timer = &soft_timer_tb[timer];
    p_timer->S = S_tb;
    p_timer->end_time = s_clock_time_1ms;
    p_timer->timeout  = 1; // 定时
    p_timer->cback    = S_tb->next_cback;
}

/**
 * @brief 软定时初始化
 *
 * @param [in] void
 * @param [out] None
 *
 * @return
 *
 */
void LdsSoftTimerInit(void)
{
    return;
}


