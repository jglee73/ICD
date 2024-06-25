#pragma once

#include "Interface_Code.h"


// ...
#include "stdint.h"
#include "cifXUser.h"
#include "cifXErrors.h"

// ...
#define PACKET_WAIT_TIMEOUT				20
#define IO_WAIT_TIMEOUT					10
#define HOSTSTATE_TIMEOUT				5000


// ...
class CDNet_Mng
{
private:
	// ...
	HANDLE hRes__DRV_BOARD;
	HANDLE hRes__DRV_CHANNEL;

	SCI__USER_LOG_CTRL* pI__LOG_CTRL;

	// ...
	int iBOARD_ID;
	int iCHANNEL_ID;

	uint32_t usLAST__ERR_CODE;

	// ...
	CUIntArray iList__AREA_IN__Total_Size;
	CUIntArray iList__AREA_IN__Reserved;
	CUIntArray iList__AREA_IN__IO_Mode;

	CUIntArray iList__AREA_OUT__Total_Size;
	CUIntArray iList__AREA_OUT__Reserved;
	CUIntArray iList__AREA_OUT__IO_Mode;

	// ...
	CString sRes__BOARD_NAME;
	CString sRes__ALIAS_NAME;
	CString sRes__DEVICE_NUMBER;
	CString sRes__SERIAL_NUMBER;
	CString sRes__BOARD_ID;
	CString sRes__SYSTEM_ERROR;
	CString sRes__CHANNEL_COUNT;
	CString sRes__DMP_SIZE;

	CString sRes__CHANNEL_FIRMWARE;
	CString sRes__CHANNEL_VERSION;
	CString sRes__CHANNEL_DATE;

	CString sRes__CHANNEL_Error;
	CString sRes__CHANNEL_OpenCount;
	CString sRes__CHANNEL_PutPacketCount;
	CString sRes__CHANNEL_GetPacketCount;
	CString sRes__CHANNEL_MailboxSize;
	CString sRes__CHANNEL_IOInAreaCount;
	CString sRes__CHANNEL_IOOutAreaCount;
	CString sRes__CHANNEL_HandShakeSize;
	CString sRes__CHANNEL_NetxFlag;
	CString sRes__CHANNEL_HostFlag;
	CString sRes__CHANNEL_HostCOSFlag;
	CString sRes__CHANNEL_DeviceCOSFlag;
	//

	void _Clear__AREA_INFO()
	{
		iList__AREA_IN__Total_Size.RemoveAll();
		iList__AREA_IN__Reserved.RemoveAll();
		iList__AREA_IN__IO_Mode.RemoveAll();

		iList__AREA_OUT__Total_Size.RemoveAll();
		iList__AREA_OUT__Reserved.RemoveAll();
		iList__AREA_OUT__IO_Mode.RemoveAll();
	}


public:	
	CDNet_Mng()
	{
		hRes__DRV_BOARD   = NULL;
		hRes__DRV_CHANNEL = NULL;

		// ...
		iBOARD_ID = 0;
		iCHANNEL_ID = 0;

		usLAST__ERR_CODE = 0;

		// ...
		pI__LOG_CTRL = NULL;
	}

	// ...
	void Link__LOG_CTRL(SCI__USER_LOG_CTRL* p_log_ctrl)
	{
		pI__LOG_CTRL = p_log_ctrl;
	}

	// ...
	int Set__BOARD_ID(const int bd_id);
	int Get__BOARD_ID();

	// ...
	int Set__CHANNEL_ID(const int ch_id);
	int Get__CHANNEL_ID();
	
	// ...
	CString Get__CHANNEL_FIRMWARE()
	{
		return sRes__CHANNEL_FIRMWARE;
	}
	CString Get__CHANNEL_VERSION()
	{
		return sRes__CHANNEL_VERSION;
	}

	// ...
	int Call__CHANNEL_OPEN();
	int Call__CHANNEL_CLOSE();

	int Call__SCAN_START();
	int Call__SCAN_STOP();

	// ...
	bool Check__LAST_ERROR_CODE(uint32_t& err_code, CString& err_msg);

	HANDLE Get__DRV_BOARD_HANDLE()
	{
		return hRes__DRV_BOARD;
	}
	HANDLE Get__DRV_CHANNEL_HANDLE()
	{
		return hRes__DRV_CHANNEL;
	}
};
