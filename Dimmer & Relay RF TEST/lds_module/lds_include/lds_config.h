#ifndef _LDS_CONFIG_H_
#define _LDS_CONFIG_H_

// information
#define SVN_VERSION				"NA"
#define FIRMWARE_VERSION		"V1.0"
#define HARDWARE_VERSION		"V1.0"
#define CUSTOMER_CODE			"LDS"
#define FIRMWARE_ID				"NA"
#ifdef M619_BEACON
#define DATE_CODE				"20201105"
#define IC_CODE					"NRF52810-BLE"	// оƬܰЭө
#define MODEL_ID				"NRF52810"
#else
#define DATE_CODE				"20210524"
#define IC_CODE					"NRF52840-BLE"	// оƬܰЭө
#define MODEL_ID				"NRF52840"
#endif

// ble tx power



#endif
