/**
 * @brief
 * @file lds_soft_timer.h
 * @version 1.0
 * @author hejinwu
 * @date 2018-4-11 10:16:47
 * @par Copyright:
 * Copyright (c) Leedarson 2000-2018. All rights reserved.
 *
 * @history
 * 1.Date        : 2018-4-11 10:16:47
 *   Author      : hejinwu
 *   Modification: Created file
 */

#ifndef __LDS_SOFT_TIMER_H__
#define __LDS_SOFT_TIMER_H__


typedef void(*p_timer_callback)();

typedef struct
{
    p_timer_callback        next_cback;   /**< 多任务定时  遇NULL停止 */
    xt_u32                  next_timeout;
}xt_process_timer_t;

/*
 * Defines the wiced timer types
 */
typedef enum
{
    SECONDS_TIMER = 1,            /**< 秒定时 一次  */
    MILLI_SECONDS_TIMER,          /**< 毫秒定时 一次 */
    SECONDS_PERIODIC_TIMER,       /**< 秒定时 周期  */
    MILLI_SECONDS_PERIODIC_TIMER, /**< 毫秒定时 周期  */
    MILLI_SECONDS_PERIODIC_TIMER_PROCESS /**< 毫秒多任务定时  遇NULL停止 */
}xt_timer_type_t;


/**
 * @brief 定时器类型名称注册
 *
 *
 * @return
 *
 * @history
 * 1.Date         : 2018-4-17 18:36:3
 *   Author       : hejinwu
 *   Modification : Created function
 */
typedef enum timer_port{
    SOFT_TIMER_LOG_UART_RX_TIMEOUT = 0, 
	SOFT_TIMER_RF_TEST_TIMEOUT,
	SOFT_TIMER_BT_UART_RX_TIMEOUT,
	SOFT_TIMER_LIGHT_MEMORY_SAVE_TIMEOUT,
    SOFT_TIMER_NAME_MAX         // 最大个数
}xt_timer_port_e;


/**
 * @brief 软定时器初始化 暂时不用初始化
 *
 * @param [in] void
 * @param [out] None
 *
 * @return
 *
 */
void LdsSoftTimerInit(void);


/**
 * @brief 定时器处理 放入主循环
 *
 * @param [in] void
 * @param [out] None
 *
 * @return
 *
 */
void LdsSoftTimerhandle(xt_u32 tick_ms);


/**
 * @brief 建立软件定时器
 *
 * @param [in] soft_timer_t *p_timer    : 定时器变量
 * @param [in] p_timer_callback TimerCb : 定时器回调
 * @param [in] xt_u32 cBackparam        : 保留 写0
 * @param [in] xt_timer_type_t type        : 定时器类型
 * @param [out] None
 *
 * @return
 *
 */
 xt_bool LdsSoftTimerCreate(xt_timer_port_e timer, p_timer_callback TimerCb, xt_u32 cBackparam, xt_timer_type_t type);
/**
 * @brief 开启软件定时器
 *
 * @param [in] soft_timer_t *p_timer  : 定时器变量
 * @param [in] xt_u32 timeout         : 定时时间值
 * @param [out] None
 *
 * @return
 *
 */
void LdsSoftTimerStart(xt_timer_port_e timer, xt_u32 timeout);
/**
 * @brief 关闭软件定时器
 *
 * @param [in] soft_timer_t *p_timer : 定时器变量
 * @param [out] None
 *
 * @return
 *
 */
void LdsSoftTimerStop(xt_timer_port_e timer);
/**
 * @brief 多任务定时器
 *
 * @param [in] soft_timer_t *p_timer : 定时器变量
 * @param [in] xt_process_timer_t *S_tb : 任务表
 * @param [out] None
 *
 * @return
 *
 */
void LdsSoftTimerFlowStart(xt_timer_port_e timer,  xt_process_timer_t *S_tb);

#endif

