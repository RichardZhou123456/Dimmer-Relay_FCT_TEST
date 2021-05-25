/**
 * @brief
 * @file lds_dtm_test.c
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



const int32_t dtm_txpowerTable[DTM_TXPOWER_LEVEL_NUM] = { 
#if defined(NRF52840_XXAA) || defined(NRF52833_XXAA)
          RADIO_TXPOWER_TXPOWER_Pos8dBm,
          RADIO_TXPOWER_TXPOWER_Pos7dBm,
          RADIO_TXPOWER_TXPOWER_Pos6dBm,
          RADIO_TXPOWER_TXPOWER_Pos5dBm,
#endif
          RADIO_TXPOWER_TXPOWER_Pos4dBm,
          RADIO_TXPOWER_TXPOWER_Pos3dBm,
#if defined(NRF52840_XXAA) || defined(NRF52833_XXAA)
          RADIO_TXPOWER_TXPOWER_Pos2dBm,
#endif
          RADIO_TXPOWER_TXPOWER_0dBm, 
          RADIO_TXPOWER_TXPOWER_Neg4dBm,
          RADIO_TXPOWER_TXPOWER_Neg8dBm,
          RADIO_TXPOWER_TXPOWER_Neg12dBm,
          RADIO_TXPOWER_TXPOWER_Neg16dBm,
          RADIO_TXPOWER_TXPOWER_Neg20dBm,
          // -30 - -40不推荐使用，-30实际测试出来为-40
          RADIO_TXPOWER_TXPOWER_Neg40dBm
};

const xt_u8 dtm_txpowerRegisterValue[DTM_TXPOWER_LEVEL_NUM] = {
#if defined(NRF52840_XXAA) || defined(NRF52833_XXAA)
        0x88,  //+8
        0x87,  //+7
        0x86,  //+6
        0x85,  //+5
#endif
	0x84, // +4
	0x83, // +3
#if defined(NRF52840_XXAA) || defined(NRF52833_XXAA)
        0x82,//+2
#endif
	0x80, // 0
	0xbc, // -4
	0xb8, // -8
	0xb4, // -12
	0xb0, // -16
	0xac, // -20
	0x98  // -40
};


xt_bool LdsDtmTestInit(void)
{
	uint32_t    dtm_error_code;
	dtm_error_code = dtm_init();
    if (dtm_error_code != DTM_SUCCESS)
    {
		LOG_ERROR("LdsDtmTestInit err\r\n");
        // If DTM cannot be correctly initialized, then we just return.
        return LDS_FALSE;
    }
    return LDS_TRUE;
}

void LdsDtmTestWait(void)
{
	uint32_t    current_time;
	current_time = dtm_wait();
}


void LdsDtmTestTxPowerSet(xt_u8 level)
{
	if(level >= DTM_TXPOWER_LEVEL_NUM) {
		return;
	}
	dtm_set_txpower(dtm_txpowerTable[level]);
}

xt_s16 LdsDtmTxPowerdBmGet(xt_u8 level) 
{
	xt_s16 result = -255;
	if(level >= DTM_TXPOWER_LEVEL_NUM) {
		return result;
	}
	switch(dtm_txpowerTable[level])
	{
#if defined(NRF52840_XXAA) || defined(NRF52833_XXAA)
                case RADIO_TXPOWER_TXPOWER_Pos8dBm:
			result = 8;
			break;
		case RADIO_TXPOWER_TXPOWER_Pos7dBm:
			result = 7;
			break;
                case RADIO_TXPOWER_TXPOWER_Pos6dBm:
			result = 6;
			break;
		case RADIO_TXPOWER_TXPOWER_Pos5dBm:
			result = 5;
			break;
#endif
		case RADIO_TXPOWER_TXPOWER_Pos4dBm:
			result = 4;
			break;
		case RADIO_TXPOWER_TXPOWER_Pos3dBm:
			result = 3;
			break;
                        
#if defined(NRF52840_XXAA) || defined(NRF52833_XXAA)
               case RADIO_TXPOWER_TXPOWER_Pos2dBm:
			result = 2;
			break;
#endif
		case RADIO_TXPOWER_TXPOWER_0dBm:
			result = 0;
			break;
		case RADIO_TXPOWER_TXPOWER_Neg4dBm:
			result = -4;
			break;
		case RADIO_TXPOWER_TXPOWER_Neg8dBm:
			result = -8;
			break;
		case RADIO_TXPOWER_TXPOWER_Neg12dBm:
			result = -12;
			break;
		case RADIO_TXPOWER_TXPOWER_Neg16dBm:
			result = -16;
			break;
		case RADIO_TXPOWER_TXPOWER_Neg20dBm:
			result = -20;
			break;
		case RADIO_TXPOWER_TXPOWER_Neg30dBm:
			result = -30;
			break;
		case RADIO_TXPOWER_TXPOWER_Neg40dBm:
			result = -40;
			break;
		default:
			result = -255;
			break;
	}

	return result;
}
void LdsDtmCommandSet(dtm_cmd_t       command_code, dtm_freq_t freq, uint32_t length, dtm_pkt_type_t payload)
{
    //dtm_cmd_t      command_code = (command >> 14) & 0x03;
    //dtm_freq_t     freq         = (command >> 8) & 0x3F;
    //uint32_t       length       = (command >> 2) & 0x3F;
    //dtm_pkt_type_t payload      = command & 0x03;
    static dtm_cmd_t last_command_code = 0;
	xt_u16 receive_packet = 0;

	LOG_DEBUG("dtm cmd = %d, freq = %d, len = %d, pkt = %d\r\n", command_code, freq, length, payload);

	uint32_t err_code = dtm_cmd(command_code, freq, length, payload);

    if(err_code != DTM_SUCCESS) {
		LOG_ERROR("dtm_cmd err: %d\r\n", err_code);
    }

	dtm_event_t result; 

	if (dtm_event_get(&result))
    {
        // Report command status on the UART.
        // Transmit MSB of the result.
        //while (app_uart_put((result >> 8) & 0xFF));
        // Transmit LSB of the result.
        //while (app_uart_put(result & 0xFF));
    }

	LOG_DEBUG("dtm_event: 0x%04x\r\n", result);

	if(command_code == LE_TEST_END && last_command_code == LE_RECEIVER_TEST) {

		receive_packet = (xt_u16)(result & 0x7fff);
		LOG_RF("Result) %d StopNumCount\r\n\r\n", receive_packet);
	}
	last_command_code = command_code;
}

void LdsDtmTestReset(void)
{
	LdsDtmCommandSet(LE_TEST_SETUP, 0x00, 0x00, 0x00);

	LOG_DEBUG("test reset: 0x%x 0x%x\r\n", 0x00, 0x00);
}

void LdsDtmTestEnd(void)
{
	LdsDtmCommandSet(LE_TEST_END, 0x00, 0x00, 0x00);

	LOG_DEBUG("test end: 0x%x 0x%x\r\n", 0xc0, 0x00);
}

void LdsDtmTestTxSetPower(xt_u8 level)
{
	xt_u8 reg_value;
	if(level >= DTM_TXPOWER_LEVEL_NUM) {
			return;
	}
	reg_value = dtm_txpowerRegisterValue[level];

	dtm_cmd_t      command_code = (reg_value >> 6) & 0x03;
    dtm_freq_t     freq         = (reg_value) & 0x3F;
    uint32_t       length       = 0x02;
    dtm_pkt_type_t payload      = 0x03;

	LdsDtmCommandSet(command_code, freq, length, payload);

	dtm_cmd_t testCmd;
	testCmd = (command_code << 14);
	testCmd |= (freq << 8);
	testCmd |= (length << 2);
	testCmd |= payload;

	LOG_DEBUG("set tx power: 0x%04x\r\n", testCmd);
}

void LdsDtmTestTxSetPhy(xt_u8 phy)
{
	if((phy != PHY_LE_1M) && (phy != PHY_LE_2M)) {
		return;
	}

	dtm_cmd_t      command_code = LE_TEST_SETUP;
    dtm_freq_t     freq         = CTL_PHY;
    uint32_t       length       = phy;
    dtm_pkt_type_t payload      = 0x00;
	
	LdsDtmCommandSet(command_code, freq, length, payload);

	dtm_cmd_t testCmd;
	testCmd = (command_code << 14);
	testCmd |= (freq << 8);
	testCmd |= (length << 2);
	testCmd |= payload;

	LOG_DEBUG("set phy: 0x%04x\r\n", testCmd);
}

void LdsDtmTestTxRxSetPacket(xt_u8 rf_cmd, xt_u8 rf_pktType, xt_u8 rf_pktLen, xt_u16 rf_freq)
{
	dtm_cmd_t      command_code;
    dtm_freq_t     freq;
    uint32_t       length;
    dtm_pkt_type_t payload;

	if(rf_cmd == RF_CMD_RX) {
		command_code = LE_RECEIVER_TEST;
	}
	else if(rf_cmd == RF_CMD_TX) {
		command_code = LE_TRANSMITTER_TEST;
	}
	else {
		return;
	}
	freq = (rf_freq - 2402) / 2;
	length = rf_pktLen;
	if(rf_pktType == RF_PKT_TYPE_CW) {
		payload = DTM_PKT_0XFF;
	}
	else if(rf_pktType == RF_PKT_TYPE_PRBS9) {
		payload = DTM_PKT_PRBS9;
	}
	else if(rf_pktType == RF_PKT_TYPE_10101010) {
		payload = DTM_PKT_0X55;
	}
	else if(rf_pktType == RF_PKT_TYPE_11110000) {
		payload = DTM_PKT_0X0F;
	}
	
	LdsDtmCommandSet(command_code, freq, length, payload);

	dtm_cmd_t testCmd;
	testCmd = (command_code << 14);
	testCmd |= (freq << 8);
	testCmd |= (length << 2);
	testCmd |= payload;

	LOG_DEBUG("set txrx packet: 0x%04x\r\n", testCmd);
}

void LdsDtmTestTxRx(xt_u8 rf_cmd, xt_u8 rf_pktType, xt_u8 rf_pktLen, xt_u16 rf_freq, xt_u8 rf_phy)
{
	dtm_cmd_t      command_code = 0;
    dtm_freq_t     freq_ch = 0;
    uint32_t       length = 0;
    dtm_pkt_type_t payload = 0;

	xt_bool test_flag = LDS_FALSE;
	
	switch(rf_cmd)
	{
		case RF_CMD_RX: {
			command_code = LE_RECEIVER_TEST;
			freq_ch = (rf_freq - 2402) / 2;
			length = rf_pktLen;
			if(rf_pktType == RF_PKT_TYPE_CW) {
				payload = DTM_PKT_0XFF;
			}
			else if(rf_pktType == RF_PKT_TYPE_PRBS9) {
				payload = DTM_PKT_PRBS9;
			}
			else if(rf_pktType == RF_PKT_TYPE_10101010) {
				payload = DTM_PKT_0X55;
			}
			else if(rf_pktType == RF_PKT_TYPE_11110000) {
				payload = DTM_PKT_0X0F;
			}

			test_flag = LDS_TRUE;
		} break;
		case RF_CMD_TX: {
			command_code = LE_TRANSMITTER_TEST;
			freq_ch = (rf_freq - 2402) / 2;
			length = rf_pktLen;
			if(rf_pktType == RF_PKT_TYPE_CW) {
				payload = DTM_PKT_0XFF;
			}
			else if(rf_pktType == RF_PKT_TYPE_PRBS9) {
				payload = DTM_PKT_PRBS9;
			}
			else if(rf_pktType == RF_PKT_TYPE_10101010) {
				payload = DTM_PKT_0X55;
			}
			else if(rf_pktType == RF_PKT_TYPE_11110000) {
				payload = DTM_PKT_0X0F;
			}

			test_flag = LDS_TRUE;
		} break;
		case RF_CMD_END: {
			command_code = LE_TEST_END;
		} break;
		default: {
			return;
		} break;
	}

	if(test_flag == LDS_TRUE) {
		LdsDtmCommandSet(command_code, freq_ch, length, payload);
	}
}

