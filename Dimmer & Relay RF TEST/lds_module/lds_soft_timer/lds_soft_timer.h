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
    p_timer_callback        next_cback;   /**< ������ʱ  ��NULLֹͣ */
    xt_u32                  next_timeout;
}xt_process_timer_t;

/*
 * Defines the wiced timer types
 */
typedef enum
{
    SECONDS_TIMER = 1,            /**< �붨ʱ һ��  */
    MILLI_SECONDS_TIMER,          /**< ���붨ʱ һ�� */
    SECONDS_PERIODIC_TIMER,       /**< �붨ʱ ����  */
    MILLI_SECONDS_PERIODIC_TIMER, /**< ���붨ʱ ����  */
    MILLI_SECONDS_PERIODIC_TIMER_PROCESS /**< ���������ʱ  ��NULLֹͣ */
}xt_timer_type_t;


/**
 * @brief ��ʱ����������ע��
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
    SOFT_TIMER_NAME_MAX         // ������
}xt_timer_port_e;


/**
 * @brief ��ʱ����ʼ�� ��ʱ���ó�ʼ��
 *
 * @param [in] void
 * @param [out] None
 *
 * @return
 *
 */
void LdsSoftTimerInit(void);


/**
 * @brief ��ʱ������ ������ѭ��
 *
 * @param [in] void
 * @param [out] None
 *
 * @return
 *
 */
void LdsSoftTimerhandle(xt_u32 tick_ms);


/**
 * @brief ���������ʱ��
 *
 * @param [in] soft_timer_t *p_timer    : ��ʱ������
 * @param [in] p_timer_callback TimerCb : ��ʱ���ص�
 * @param [in] xt_u32 cBackparam        : ���� д0
 * @param [in] xt_timer_type_t type        : ��ʱ������
 * @param [out] None
 *
 * @return
 *
 */
 xt_bool LdsSoftTimerCreate(xt_timer_port_e timer, p_timer_callback TimerCb, xt_u32 cBackparam, xt_timer_type_t type);
/**
 * @brief ���������ʱ��
 *
 * @param [in] soft_timer_t *p_timer  : ��ʱ������
 * @param [in] xt_u32 timeout         : ��ʱʱ��ֵ
 * @param [out] None
 *
 * @return
 *
 */
void LdsSoftTimerStart(xt_timer_port_e timer, xt_u32 timeout);
/**
 * @brief �ر������ʱ��
 *
 * @param [in] soft_timer_t *p_timer : ��ʱ������
 * @param [out] None
 *
 * @return
 *
 */
void LdsSoftTimerStop(xt_timer_port_e timer);
/**
 * @brief ������ʱ��
 *
 * @param [in] soft_timer_t *p_timer : ��ʱ������
 * @param [in] xt_process_timer_t *S_tb : �����
 * @param [out] None
 *
 * @return
 *
 */
void LdsSoftTimerFlowStart(xt_timer_port_e timer,  xt_process_timer_t *S_tb);

#endif

