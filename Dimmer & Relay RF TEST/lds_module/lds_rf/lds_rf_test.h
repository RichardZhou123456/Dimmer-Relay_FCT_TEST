#ifndef _LDS_RF_TEST_H_
#define _LDS_RF_TEST_H_


#define RF_TX_POWER_MAX        10
#define RF_TX_POWER_MIN       -10

#define RF_TX_PACKET_LEN		37

#define RF_PKT_TYPE_CW			 	254
#define RF_PKT_TYPE_10101010  		2
#define RF_PKT_TYPE_11110000		1
#define RF_PKT_TYPE_PRBS9			0

#define RF_STATE_IDLE				0
#define RF_STATE_BUSY				1

#define RF_CMD_RESET				0
#define RF_CMD_RX					1
#define RF_CMD_TX					2
#define RF_CMD_END					3

typedef struct
{
    xt_u8  rf_mode;             // 1: �������ģʽ�� 0: ����ϵͳģʽ
    xt_u8  rf_cmd;
    xt_u8  rf_pkttype;             // ����rf����
    xt_u16 rf_frequency;        // ����rfƵ������
    xt_u16 rf_chanle;           // �ŵ�
    xt_u8  rf_state;            // �Ƿ����ù�
    xt_u8  rf_phy;              // ����
    xt_s16 rf_txpower;          // ���书��
    xt_u16 rf_num;              // ��Ҫ���Ͱ���
    xt_u8  rf_len;              // ���ݰ�����
} xt_rf_txtest_t;


void LdsRfTestModeEnter(void);

void LdsRfTestUartRxHandle(xt_u8 *buf, xt_u8 len);

void LdsRfTestHandle(void);


#endif
