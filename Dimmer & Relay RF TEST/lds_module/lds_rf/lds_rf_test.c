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

//#define HIDDEN_ITEMS_ENABLE

#define DEFAULT_TX_POWER_LEVEL		0

static xt_rf_txtest_t g_rfcmd;
static xt_u32 tx_timeout = 0;
static xt_u8 rf_mac[6];
static xt_u8 rf_txpower_level = 0; // +4dbm
static xt_u8 rf_phy = PHY_LE_1M;

#define RF_UART_CMD_LEN_MAX			8
static xt_bool rf_uart_cmd_flag = LDS_FALSE;
static xt_u8 rf_uart_cmd[RF_UART_CMD_LEN_MAX] = 0;
static xt_u8 rf_uart_cmd_len = 0;
static const xt_u8 RF_TEST_CMD[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x1C};

/**
 * @brief function for  initialization of variable
 *
 * @param [in]  None
 * @param [out] None
 *
 * @return 
 */
void ldsRfTestParamInit(void)
{
	rf_txpower_level = DEFAULT_TX_POWER_LEVEL;
	
	g_rfcmd.rf_txpower = LdsDtmTxPowerdBmGet(rf_txpower_level);
	g_rfcmd.rf_len = RF_TX_PACKET_LEN;
	g_rfcmd.rf_frequency = 2402;
	
	g_rfcmd.rf_state = RF_STATE_IDLE;
}

/**
 * @brief function for rf test get mac address
 *
 * @param [in]  None
 * @param [out] None
 *
 * @return 
 */
void ldsRfGetMacAddr(void)
{
	xt_u32 deviceAddr[2];
	deviceAddr[0] = NRF_FICR->DEVICEADDR[0];
	deviceAddr[1] = NRF_FICR->DEVICEADDR[1];
	rf_mac[5] = (xt_u8)(deviceAddr[1] >> 8);
    rf_mac[5] |= 0xc0;		//蓝牙地址最高2位是类型
	rf_mac[4] = (xt_u8)(deviceAddr[1]);
	rf_mac[3] = (xt_u8)(deviceAddr[0] >> 24);
	rf_mac[2] = (xt_u8)(deviceAddr[0] >> 16);
	rf_mac[1] = (xt_u8)(deviceAddr[0] >> 8);
	rf_mac[0] = (xt_u8)(deviceAddr[0]);
	//LOG_DEBUG("MacAddr          : 0x%02x%02x%02x%02x%02x%02x\r\n", rf_mac[5], rf_mac[4], rf_mac[3], 
	//															rf_mac[2], rf_mac[1], rf_mac[0]);
}

/**
 * @brief function for rf test timeout set
 *
 * @param [in]  None
 * @param [out] None
 *
 * @return 
 */
void ldsRfTestTimeoutSet(xt_u32 ms)
{
	if(ms != 0) {
		LdsSoftTimerStart(SOFT_TIMER_RF_TEST_TIMEOUT, ms);
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
static void ldsRfTestTimeout(void)
{
	LOG_DEBUG("RF Test timeout: %d ms\r\n", tx_timeout);
	LOG_DEBUG("DTM STOP \r\n");
	LdsDtmTestEnd();

	g_rfcmd.rf_state = RF_STATE_IDLE;
	g_rfcmd.rf_cmd = RF_CMD_END;
}

/**
  * @brief  function for print device information
  * @param  None
  * @retval None
  */
static void lds_DeviceInfoPrint(void)
{
	LOG_RF("\r\n******************************\r\n");
	LOG_RF("* LDS Module Evaluation Tool * \r\n");
	LOG_RF("Software Version : SVN=%s, APP=%s\r\n", SVN_VERSION, FIRMWARE_VERSION);
	LOG_RF("OTA Image Info   : File Ver=NA,Image Type=NA\r\n");
	LOG_RF("Customer         : %s\r\n", CUSTOMER_CODE);
	LOG_RF("Model ID         : %s\r\n", MODEL_ID); 
	LOG_RF("Date Code        : %s\r\n", DATE_CODE); 
	LOG_RF("FW ID            : %s\r\n", FIRMWARE_ID); 
	LOG_RF("MacAddr          : 0x%02x%02x%02x%02x%02x%02x\r\n", rf_mac[5], rf_mac[4], rf_mac[3], 
																rf_mac[2], rf_mac[1], rf_mac[0]);
	LOG_RF("IC Code          : %s\r\n", IC_CODE);
	LOG_RF("Alarm Temp       : NA\r\n");
	LOG_RF("NTC Temp         : NA\r\n");
}

/**
  * @brief  function for printf rf menu information
  * @param  None
  * @retval None
  */
static void lds_RfInfoPrint(void)
{
    LOG_RF("a) Tx 2402 CW\r\n");        //
    LOG_RF("b) Tx 2442 CW\r\n");        //
    LOG_RF("c) Tx 2480 CW\r\n");        //
    LOG_RF("d) Tx 2402 10101010\r\n");  //
    LOG_RF("e) Tx 2442 10101010\r\n");  //
    LOG_RF("f) Tx 2480 10101010\r\n");  //
    LOG_RF("g) Tx 2402 PRBS9\r\n");     //
    LOG_RF("h) Tx 2442 PRBS9\r\n");     //
    LOG_RF("i) Tx 2480 PRBS9\r\n");     //
    LOG_RF("j) Tx 2402 11110000\r\n");  //
    LOG_RF("k) Tx 2442 11110000\r\n");  //
    LOG_RF("l) Tx 2480 11110000\r\n");  //
    LOG_RF("m) Rx 2402\r\n");           //
    LOG_RF("n) Rx 2442\r\n");           //
    LOG_RF("o) Rx 2480\r\n");           //
    LOG_RF("p) Info\r\n");              // 
    LOG_RF("q) Menu\r\n");              //
    
    LOG_RF("r) EEPROM Test\r\n");       //
    LOG_RF("x) Stop tx/rx\r\n");        //
    LOG_RF("t) Exit\r\n");              //

	LOG_RF("1)odd high and even low\r\n");  
	LOG_RF("2)odd low and even high\r\n");  
	LOG_RF("3)PHY 1M\r\n"); 
	LOG_RF("4)PHY 2M\r\n");
    
#ifdef HIDDEN_ITEMS_ENABLE
	LOG_RF("5)PHY 125K\r\n"); 
	LOG_RF("6)PHY 500K\r\n"); 
	LOG_RF("7)Get Ctune Value\r\n");
	LOG_RF("8)Enter Ctune Calibration Mode\r\n"); 
	
    LOG_RF("s) Resume Tx\r\n");       	//
    LOG_RF("+) + 1db\r\n");             //
    LOG_RF("-) - 1db\r\n");             //
    LOG_RF(">) + 2Mhz\r\n");        	//
    LOG_RF("<) - 2Mhz\r\n");          	//
    LOG_RF("u) tx continue\r\n");       //
    LOG_RF("v) Tx 5s\r\n");           	//
    LOG_RF("w) Tx 10s\r\n");            //
    LOG_RF("y) Tx 15s\r\n");            //
    LOG_RF("z) Tx 25s\r\n");            //
#endif
}

/**
  * @brief  function for printf rf test information
  * @param  None
  * @retval None
  */
static void ldsInformationPrint(void)
{
	LOG_RF("MacAddr  : 0x%02x%02x%02x%02x%02x%02x \r\n", rf_mac[5], rf_mac[4], rf_mac[3], rf_mac[2], rf_mac[1], rf_mac[0]);
	LOG_RF("tx time  : %d ms    \r\n", tx_timeout);
	g_rfcmd.rf_txpower = LdsDtmTxPowerdBmGet(rf_txpower_level);
	LOG_RF("tx power : %d dB    \r\n", g_rfcmd.rf_txpower);
	LOG_RF("pkt len  : %d Bytes \r\n", g_rfcmd.rf_len);
	LOG_RF("channel  : %d MHz   \r\n", g_rfcmd.rf_frequency);
	switch(g_rfcmd.rf_pkttype) {
		case 0 :
			LOG_RF("pkt type : PRBS9 \r\n");
		break;

		case 1 :
			LOG_RF("pkt type : 11110000 \r\n");
		break;

		case 2 :
			LOG_RF("pkt type : 10101010 \r\n");
		break;

		case 254 :
			LOG_RF("pkt type : Unmodulated carrier \r\n");
		break;

		default:
		break;
	}
	LOG_RF("\r\n\r\n");
}

/**@brief Function for RF Test UART RECEIVE Handle
 *
 * @param[in]  buf: data pointer
 * @param[in]  len: data length
 * @return none
 */
void LdsRfTestUartRxHandle(xt_u8 *buf, xt_u8 len)
{
#if 0
	xt_u8 i;
	LOG_DEBUG("Uart rx(%d): ", len);
	for(i = 0; i < len; i ++) {
		LOG_DEBUG("%c ", buf[i]);
	}
	LOG_DEBUG("\r\n");
#endif
	if((len == 1) || ((len == 3) && (buf[1] == 0x0d) && (buf[2] == 0x0a)) ||
		((len == 8) || ((len == 10) && (buf[1] == 0x0d) && (buf[2] == 0x0a)))) {
		
	}
	else {
		return;
	}

	if(rf_uart_cmd_flag == LDS_TRUE) {
		return;
	}

	rf_uart_cmd_flag = LDS_TRUE;
	rf_uart_cmd_len = len;
	memcpy(rf_uart_cmd, buf, len);
}

/**@brief Function for FTM Test Handle
 *
 * @param[in]  buf: data pointer
 * @param[in]  len: data length
 * @return none
 */
void lds_UartRfTestCmdHandle(xt_u8 *buf, xt_u8 len)
{
	xt_bool dtm_tx_flag = LDS_FALSE;
	xt_bool dtm_rx_flag = LDS_FALSE;
	xt_bool dtm_stop_flag = LDS_FALSE;
	xt_bool dtm_reset_flag = LDS_FALSE;
	
	xt_bool tx_power_set_flag = LDS_FALSE;
	
	g_rfcmd.rf_phy = 1;
	
	
	
	switch(buf[0])
	{
		case 'a':				// Tx 2402 CW
			if (g_rfcmd.rf_state == RF_STATE_IDLE)
			{
				g_rfcmd.rf_state = RF_STATE_BUSY;
				LOG_RF("a) Tx 2402 CW\r\n"); 	   //
				g_rfcmd.rf_pkttype = RF_PKT_TYPE_CW;
				g_rfcmd.rf_frequency = 2402;
				g_rfcmd.rf_len = 1;		// 1byte
				g_rfcmd.rf_cmd = RF_CMD_TX;
				
				dtm_tx_flag = LDS_TRUE;
			}
			break;
		case 'b':				// Tx 2442 CW
			if (g_rfcmd.rf_state == RF_STATE_IDLE)
			{
				g_rfcmd.rf_state = RF_STATE_BUSY;
				LOG_RF("b) Tx 2442 CW\r\n"); 	   //
				g_rfcmd.rf_pkttype = RF_PKT_TYPE_CW;
				g_rfcmd.rf_frequency = 2442;
				g_rfcmd.rf_len = 1;
				g_rfcmd.rf_cmd = RF_CMD_TX;

				dtm_tx_flag = LDS_TRUE;
			}
			break;
		case 'c':				// Tx 2480 CW
			if (g_rfcmd.rf_state == RF_STATE_IDLE)
			{
				g_rfcmd.rf_state = RF_STATE_BUSY;
				LOG_RF("c) Tx 2480 CW\r\n"); 	   //
				g_rfcmd.rf_pkttype = RF_PKT_TYPE_CW;
				g_rfcmd.rf_frequency = 2480;
				g_rfcmd.rf_len = 1;
				g_rfcmd.rf_cmd = RF_CMD_TX;

				dtm_tx_flag = LDS_TRUE;
			}
			break;
		case 'd':				// Tx 2402 10101010
			if (g_rfcmd.rf_state == RF_STATE_IDLE)
			{
				g_rfcmd.rf_state = RF_STATE_BUSY;
				LOG_RF("d) Tx 2402 10101010\r\n");  //
				g_rfcmd.rf_pkttype = RF_PKT_TYPE_10101010;
				g_rfcmd.rf_frequency = 2402;
				g_rfcmd.rf_len = RF_TX_PACKET_LEN;
				g_rfcmd.rf_cmd = RF_CMD_TX;

				dtm_tx_flag = LDS_TRUE;
			}
			break;
		case 'e':				// Tx 2402 10101010
			if (g_rfcmd.rf_state == RF_STATE_IDLE)
			{
				g_rfcmd.rf_state = RF_STATE_BUSY;
				LOG_RF("e) Tx 2442 10101010\r\n");  //
				g_rfcmd.rf_pkttype = RF_PKT_TYPE_10101010;
				g_rfcmd.rf_frequency = 2442;
				g_rfcmd.rf_len = RF_TX_PACKET_LEN;
				g_rfcmd.rf_cmd = RF_CMD_TX;

				dtm_tx_flag = LDS_TRUE;
			}
			break;
		case 'f':				// Tx 2402 10101010
			if (g_rfcmd.rf_state == RF_STATE_IDLE)
			{
				g_rfcmd.rf_state = RF_STATE_BUSY;
				LOG_RF("f) Tx 2480 10101010\r\n");  //
				g_rfcmd.rf_pkttype = RF_PKT_TYPE_10101010;
				g_rfcmd.rf_frequency = 2480;
				g_rfcmd.rf_len = RF_TX_PACKET_LEN;
				g_rfcmd.rf_cmd = RF_CMD_TX;

				dtm_tx_flag = LDS_TRUE;
			}
			break;
		case 'g':				// Tx 2402 PRBS9
			if (g_rfcmd.rf_state == RF_STATE_IDLE)
			{
				g_rfcmd.rf_state = RF_STATE_BUSY;
				LOG_RF("g) Tx 2402 PRBS9\r\n");
				g_rfcmd.rf_pkttype = RF_PKT_TYPE_PRBS9;
				g_rfcmd.rf_frequency = 2402;
				g_rfcmd.rf_len = RF_TX_PACKET_LEN;
				g_rfcmd.rf_cmd = RF_CMD_TX;

				dtm_tx_flag = LDS_TRUE;
			}
			break;
		case 'h':				// Tx 2402 PRBS9
			if (g_rfcmd.rf_state == RF_STATE_IDLE)
			{
				g_rfcmd.rf_state = RF_STATE_BUSY;
				LOG_RF("h) Tx 2442 PRBS9\r\n");
				g_rfcmd.rf_pkttype = RF_PKT_TYPE_PRBS9;
				g_rfcmd.rf_frequency = 2442;
				g_rfcmd.rf_len = RF_TX_PACKET_LEN;
				g_rfcmd.rf_cmd = RF_CMD_TX;

				dtm_tx_flag = LDS_TRUE;
			}
			break;
		case 'i':				// Tx 2402 PRBS9
			if (g_rfcmd.rf_state == RF_STATE_IDLE)
			{
				g_rfcmd.rf_state = RF_STATE_BUSY;
				LOG_RF("i) Tx 2480 PRBS9\r\n");
				g_rfcmd.rf_pkttype = RF_PKT_TYPE_PRBS9;
				g_rfcmd.rf_frequency = 2480;
				g_rfcmd.rf_len = RF_TX_PACKET_LEN;
				g_rfcmd.rf_cmd = RF_CMD_TX;

				dtm_tx_flag = LDS_TRUE;
			}
			break;
		case 'j':				// Tx 2402 11110000
			if (g_rfcmd.rf_state == RF_STATE_IDLE)
			{
				g_rfcmd.rf_state = RF_STATE_BUSY;
				LOG_RF("j) Tx 2402 11110000\r\n");
				g_rfcmd.rf_pkttype = RF_PKT_TYPE_11110000;
				g_rfcmd.rf_frequency = 2402;
				g_rfcmd.rf_len = RF_TX_PACKET_LEN;
				g_rfcmd.rf_cmd = RF_CMD_TX;

				dtm_tx_flag = LDS_TRUE;
			}
			break;
		case 'k':				// Tx 2402 11110000
			if (g_rfcmd.rf_state == RF_STATE_IDLE)
			{
				g_rfcmd.rf_state = RF_STATE_BUSY;
				LOG_RF("k) Tx 2442 11110000\r\n");
				g_rfcmd.rf_pkttype = RF_PKT_TYPE_PRBS9;
				g_rfcmd.rf_frequency = 2442;
				g_rfcmd.rf_len = RF_TX_PACKET_LEN;
				g_rfcmd.rf_cmd = RF_CMD_TX;

				dtm_tx_flag = LDS_TRUE;
			}
			break;
		case 'l':				// Tx 2402 11110000
			if (g_rfcmd.rf_state == RF_STATE_IDLE)
			{
				g_rfcmd.rf_state = RF_STATE_BUSY;
				LOG_RF("l) Tx 2480 11110000\r\n");
				g_rfcmd.rf_pkttype = RF_PKT_TYPE_PRBS9;
				g_rfcmd.rf_frequency = 2480;
				g_rfcmd.rf_len = RF_TX_PACKET_LEN;
				g_rfcmd.rf_cmd = RF_CMD_TX;

				dtm_tx_flag = LDS_TRUE;
			}
			break;
		case 'm':				// Rx 2402（每秒上报收包数，时间待测试，带RSSI）
			if (g_rfcmd.rf_state == RF_STATE_IDLE)
			{
				g_rfcmd.rf_state = RF_STATE_BUSY;
				LOG_RF("m) Rx 2402\r\n");
				g_rfcmd.rf_frequency = 2402;
				g_rfcmd.rf_cmd = RF_CMD_RX;

				dtm_rx_flag = LDS_TRUE;
			}
			break;
		case 'n':				// Rx 2442（每秒上报收包数，时间待测试，带RSSI）
			if (g_rfcmd.rf_state == RF_STATE_IDLE)
			{
				g_rfcmd.rf_state = RF_STATE_BUSY;
				LOG_RF("n) Rx 2442\r\n");
				g_rfcmd.rf_frequency = 2442;
				g_rfcmd.rf_cmd = RF_CMD_RX;

				dtm_rx_flag = LDS_TRUE;
			}
			break;
		case 'o':				// Rx 2480（每秒上报收包数，时间待测试，带RSSI）
			if (g_rfcmd.rf_state == RF_STATE_IDLE)
			{
				g_rfcmd.rf_state = RF_STATE_BUSY;
				LOG_RF("o) Rx 2480\r\n");
				g_rfcmd.rf_frequency = 2480;
				g_rfcmd.rf_cmd = RF_CMD_RX;

				dtm_rx_flag = LDS_TRUE;
			}
			break;
		case 'p':				// Information（MAC+数据长度（固定37）+发射功率、信道和数据格式等）
			
			ldsInformationPrint();
		   break;

		case 'q':				// 保持在RF测试模式；记录已设定发射功率、信道和数据格式；刷新打印菜单；停止收发；
			if(g_rfcmd.rf_state == RF_STATE_BUSY) {
				g_rfcmd.rf_state = RF_STATE_IDLE;
				g_rfcmd.rf_cmd = RF_CMD_END;
				dtm_stop_flag = LDS_TRUE;
			}
			lds_DeviceInfoPrint();
			lds_RfInfoPrint();
			break;
			
		case 'r' :

			break;

		case 'x':				
			LOG_RF("x) Rf test stop\r\n");
			g_rfcmd.rf_state = RF_STATE_IDLE;
			g_rfcmd.rf_cmd = RF_CMD_END;
			dtm_stop_flag = LDS_TRUE;
			break;

		case 't' : {
			LOG_DEBUG("t)Exit \r\n\r\n");
			g_rfcmd.rf_state = RF_STATE_IDLE;
			g_rfcmd.rf_cmd = RF_CMD_END;
			dtm_stop_flag = LDS_TRUE;
			break;
		}

		case '1':
			LdsBspOddNumberPinHigh();
			LdsBspEvenNumberPinLow();
		
			LOG_RF("1)odd high and even low\r\n");  
			break;
		case '2':
			LdsBspOddNumberPinLow();
			LdsBspEvenNumberPinHigh();
		
			LOG_RF("2)odd low and even high\r\n");  
			break;
		case '3':
			LOG_RF("3)PHY 1M\r\n"); 

			rf_phy = PHY_LE_1M;
			//dtm_set_radioMode(RADIO_MODE_MODE_Ble_1Mbit);
			//LdsDtmTestInit();
			break;
		case '4':
			LOG_RF("4)PHY 2M\r\n");

			rf_phy = PHY_LE_2M;
			//dtm_set_radioMode(RADIO_MODE_MODE_Ble_2Mbit);
			//LdsDtmTestInit();
			break;
		case '5':
			LOG_RF("5)PHY 125K, NO Support\r\n"); 
			break;
		case '6':
			LOG_RF("6)PHY 500K, No Support\r\n"); 
			break;
		case '7':
			LOG_RF("7)Get Ctune Value, No Support\r\n");
			break;
		case '8':
			LOG_RF("8)Enter Ctune Calibration Mode, No Support\r\n"); 
			break;
		case 'u':				//
			tx_timeout = 0;
			LOG_RF("u) Tx 0s\r\n");
			break;
		case 'v':				// tx 5s
			tx_timeout = 5000;
			LOG_RF("u) Tx 5s\r\n");
			break;
		case 'w':				// tx 10s
			tx_timeout = 10000;
			LOG_RF("u) Tx 10s\r\n");
			break;
		case 'y':				// tx 15s
			tx_timeout = 15000;
			LOG_RF("u) Tx 15s\r\n");
			break;
		case 'z':				// tx 25s
			tx_timeout = 25000;
			LOG_RF("u) Tx 25s\r\n");
			break;
		case '+':	
			if(rf_txpower_level > 0) {
				rf_txpower_level --;

				if(g_rfcmd.rf_state == RF_STATE_BUSY) {
					
					dtm_stop_flag = LDS_TRUE;
					dtm_tx_flag = LDS_TRUE;
				}
				tx_power_set_flag = LDS_TRUE;
			}
			
			g_rfcmd.rf_txpower = LdsDtmTxPowerdBmGet(rf_txpower_level);

			LOG_RF("+)+ 1dB = %d \r\n\r\n", g_rfcmd.rf_txpower);
			break;
		case '-':				
			if(rf_txpower_level < (DTM_TXPOWER_LEVEL_NUM - 1)) {
				rf_txpower_level ++;

				if(g_rfcmd.rf_state == RF_STATE_BUSY) {
					
					dtm_stop_flag = LDS_TRUE;
					dtm_tx_flag = LDS_TRUE;
				}
				tx_power_set_flag = LDS_TRUE;
			}

			g_rfcmd.rf_txpower = LdsDtmTxPowerdBmGet(rf_txpower_level);
			
			LOG_RF("-)- 1dB = %d \r\n\r\n", g_rfcmd.rf_txpower);
			break;
		case '<':				
			if(g_rfcmd.rf_frequency > 2402) {
				g_rfcmd.rf_frequency -= 2;

				//重新启动tx
				dtm_stop_flag = LDS_TRUE;
				dtm_tx_flag = LDS_TRUE;
			}
			
			LOG_RF("<) - 2MHz = %d \r\n\r\n", g_rfcmd.rf_frequency);

			ldsInformationPrint();
			break;
		case '>':				// D?μà-
			if(g_rfcmd.rf_frequency < 2480) {
				g_rfcmd.rf_frequency += 2;

				//重新启动tx
				dtm_stop_flag = LDS_TRUE;
				dtm_tx_flag = LDS_TRUE;
			}
			
			LOG_RF(">) + 2MHz = %d \r\n\r\n", g_rfcmd.rf_frequency);
			
			ldsInformationPrint();
			break;
		default:
			break;
	}

	if(dtm_stop_flag == LDS_TRUE) {
		LOG_DEBUG("DTM STOP \r\n");
		LdsDtmTestEnd();
	}

	if(dtm_tx_flag == LDS_TRUE || 
		dtm_rx_flag == LDS_TRUE || 
		tx_power_set_flag == LDS_TRUE) {

		// RESET
		LdsDtmTestReset();

		LdsDtmTestWait();
		// set tx power
		LdsDtmTestTxSetPower(rf_txpower_level);
		
		LdsDtmTestWait();
		// set phy
		LdsDtmTestTxSetPhy(rf_phy);

		LdsDtmTestWait();
		// set tx/rx packet
		LdsDtmTestTxRxSetPacket(g_rfcmd.rf_cmd, g_rfcmd.rf_pkttype, g_rfcmd.rf_len, g_rfcmd.rf_frequency);

		LdsDtmTestWait();
		// set timeout
		if(dtm_stop_flag != LDS_TRUE) {
			ldsRfTestTimeoutSet(tx_timeout);
		}
	}
}

void lds_UartRfModeCmd(xt_u8 *data, xt_u8 len)
{
	if((len == sizeof(RF_TEST_CMD)) && 
	    (memcmp(RF_TEST_CMD, data, sizeof(RF_TEST_CMD)) == 0)) 
    {
		
		NVIC_SystemReset();
		LOG_DEBUG("RF TEST CMD\r\n");
    }
}

/**
  * @brief  function for rf test loop handle 
  * @param  None
  * @retval None
  */
void LdsRfTestHandle(void)
{
	if(rf_uart_cmd_flag == LDS_FALSE) {
		return;
	}

	lds_UartRfModeCmd(&rf_uart_cmd[0], rf_uart_cmd_len);
	
	lds_UartRfTestCmdHandle(&rf_uart_cmd[0], 1);

	rf_uart_cmd_flag = LDS_FALSE;
}

/**
  * @brief  function for test mode 
  * @param  None
  * @retval None
  */
void LdsRfTestModeEnter(void)
{
	ldsRfTestParamInit();
	
	ldsRfGetMacAddr();
	// prinf device information
	lds_DeviceInfoPrint();
	// printf test menu
	lds_RfInfoPrint();

	//rf test time register
	LdsSoftTimerCreate(SOFT_TIMER_RF_TEST_TIMEOUT, ldsRfTestTimeout, 0, MILLI_SECONDS_TIMER);

	LdsDtmTestInit();
}



