#ifndef _LDS_DTM_TEST_H_
#define _LDS_DTM_TEST_H_

#define DTM_TXPOWER_LEVEL_NUM		14

#define CTL_PHY			0x02

#define PHY_LE_1M		1
#define PHY_LE_2M		2


void LdsDtmTestTxRx(xt_u8 rf_cmd, xt_u8 rf_pktType, xt_u8 rf_pktLen, xt_u16 rf_freq, xt_u8 rf_phy);

xt_s16 LdsDtmTxPowerdBmGet(xt_u8 level) ;

xt_bool LdsDtmTestInit(void);

void LdsDtmTestWait(void);

void LdsDtmTestTxSetPower(xt_u8 level);

void LdsDtmTestTxSetPhy(xt_u8 phy);

void LdsDtmTestTxRxSetPacket(xt_u8 rf_cmd, xt_u8 rf_pktType, xt_u8 rf_pktLen, xt_u16 rf_freq);


#endif
