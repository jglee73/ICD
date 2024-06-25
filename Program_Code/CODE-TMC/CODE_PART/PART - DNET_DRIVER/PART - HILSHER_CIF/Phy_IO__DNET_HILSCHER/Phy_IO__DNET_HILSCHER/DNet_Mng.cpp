#include "StdAfx.h"
#include "DNet_Mng.h"

#include "cifuser.h"
#include "rcs_user.h"
#include "dnm_user.h"


//--------------------------------------------------------------------------------
int CDNet_Mng::Set__BOARD_ID(const int bd_id)
{
	iBD_ID = bd_id;
	return 1;
}
int CDNet_Mng::Get__BOARD_ID()
{
	return iBD_ID;
}

int CDNet_Mng::Call__BOARD_OPEN()
{
	int dev_id = iBD_ID;

	// ...
	BOARD_INFO   tBoardInfo;
	FIRMWAREINFO tFirmwareInfo;
	DRIVERINFO	 tDriverInfo;
	RCS_MESSAGE  tMsg;
	RCS_MESSAGE  tMsgDownload;

	// ...
	CString log_add;
	CString log_msg;

	CString ch_data;
	short s_ret;

	// open driver first
	s_ret = DevOpenDriver(0);
	if((s_ret = DevOpenDriver(0)) != DRV_NO_ERROR) 
	{
		printf("DevOpenDriver(%1d): %d\n", dev_id,s_ret);
		printf("  * Error <- \"%s\" \n", Get__ERR_CODE(s_ret));
		return -11;
	} 
	printf("DevOpenDriver(%1d): OK\n", dev_id);

	// Board Info ...
	tBoardInfo.abDriverVersion[0] = 0;

	s_ret = DevGetBoardInfo(0, sizeof(tBoardInfo), &tBoardInfo);
	if(s_ret != DRV_NO_ERROR) 
	{
		log_msg.Format("DevGetBoardInfo(%1d) : Error(%d) \n", dev_id, s_ret);
		log_add += log_msg;
		
		printf(log_msg);
		printf("  * Error <- \"%s\" \n", Get__ERR_CODE(s_ret));

		sDRIVER_VERSION = "ERROR";
		DevCloseDriver(0);
		return -12;
	} 
	else
	{
		tBoardInfo.abDriverVersion[15] = 0;

		sDRIVER_VERSION.Format("%s", tBoardInfo.abDriverVersion);
		printf("DevGetBoardInfo(%s): OK\n", sDRIVER_VERSION);

		//
		log_msg = "DevGetBoardInfo() ... \n";

		for(int t=0; t<MAX_DEV_BOARDS; t++)
		{
			int board_number  = tBoardInfo.tBoard[t].usBoardNumber;
			int irq_number    = tBoardInfo.tBoard[t].usIrqNumber;
			int physical_addr = tBoardInfo.tBoard[t].ulPhysicalAddress;
			int available_sts = tBoardInfo.tBoard[t].usAvailable;

			log_add.Format("  * %1d)  Board (%1d) \n", t, board_number);
			log_msg += log_add;
			log_add.Format("    * irq_number <- [%1d] \n", irq_number);
			log_msg += log_add;
			log_add.Format("    * physical_address <- [%1d] \n", physical_addr);
			log_msg += log_add;

			if(available_sts == 0)
				log_add.Format("    * not available ! (%1d) \n", available_sts);
			else
				log_add.Format("    * available ! (%1d) \n", available_sts);
			log_msg += log_add;
		}
		printf(log_msg);

		//
		s_ret = DevInitBoard(dev_id, NULL);
		if(s_ret != DRV_NO_ERROR)
		{
			printf("DevInitBoard(%1d): %d\n", dev_id,s_ret);
			printf("  * Error <- \"%s\" \n", Get__ERR_CODE(s_ret));

			DevCloseDriver(0);
			return -13;
		} 
		printf("DevInitBoard(%1d) : OK \n", dev_id);
	}

	// Firmware Info ...
	s_ret = DevGetInfo(dev_id, GET_FIRMWARE_INFO, sizeof(tFirmwareInfo), &tFirmwareInfo);
	if(s_ret != DRV_NO_ERROR) 
	{
		printf("DevGetInfo(%1d) : Error ! \n", dev_id);
		printf("  * Error <- \"%s\" \n", Get__ERR_CODE(s_ret));

		sFIRMWARE_INFO = "ERROR";
	}
	else
	{
		CString str_info;
		int i;

		for(i = 0; i < 16; i++)
		{
			char ch = tFirmwareInfo.abFirmwareName[i];
			str_info += ch;
		}
		ch_data += ' ';

		for(i = 0; i < 16; i++)
		{
			char ch = tFirmwareInfo.abFirmwareVersion[i];
			str_info += ch;
		}

		sFIRMWARE_INFO = str_info;
	}

	//
	s_ret = DevReset(dev_id, WARMSTART, 5000);
	if(s_ret != DRV_NO_ERROR)
	{
		printf("DevReset(%1d): %d\n", dev_id,s_ret);
		printf("  * Error <- \"%s\" \n", Get__ERR_CODE(s_ret));
		return -21;
	}

	return 1;
}
int CDNet_Mng::Call__BOARD_CLOSE()
{
	int dev_id = iBD_ID;
	short s_ret;

	// ...
	{
		s_ret = DevExitBoard(dev_id);
		printf("DevExitBoard RetWert = %5d \n", s_ret);

		s_ret = DevCloseDriver(0);
		printf("DevCloseDriver RetWert = %5d \n", s_ret);
		return -1;
	}
	return 1;
}

int CDNet_Mng::Call__SCAN_START()
{
	int dev_id = iBD_ID;
	short s_ret;

	// Signal board, application is running
	s_ret = DevSetHostState(dev_id, HOST_READY, 0L);
	if(s_ret != DRV_NO_ERROR)
	{
		printf("ERROR ! - DevSetHostState (HOST_READY) RetWert = %5d \n", s_ret);
		printf("  * Error <- \"%s\" \n", Get__ERR_CODE(s_ret));
		return -11;
	}

	// ...
	IOINFO io_info;

	s_ret = DevGetInfo(dev_id, GET_IO_INFO, sizeof(IOINFO), &io_info);
	if(s_ret != DRV_NO_ERROR)
	{
		printf("ERROR ! - DevGetInfo(%1d, GET_IO_INFO) RetWert = %5d \n", dev_id,s_ret);
		printf("  * Error <- \"%s\" \n", Get__ERR_CODE(s_ret));
		return -12;
	}
	else
	{
		CString log_msg;
		CString log_bff;

		//
		log_bff.Format("io_info.bComBit <- %1d \n", io_info.bComBit);						// Actual state of the COM bit (0,1)
		log_msg += log_bff;

		if(io_info.bComBit == 0)				log_bff = "  * IO-Communication is not running \n";
		else									log_bff = "  * IO-Communication is running \n";
		log_msg += log_bff;

		//
		log_bff.Format("io_info.bIOExchangeMode <- %1d \n", io_info.bIOExchangeMode);		// Actual data exchange mode (0..5)
		log_msg += log_bff;

		if(io_info.bIOExchangeMode == 0)		log_bff = "  * IO-Communication NOT AVAILABLE \n";
		else									log_bff = "  * IO-Communication AVAILABLE \n";
		log_msg += log_bff;

		//
		log_bff.Format("io_info.ulIOExchangeCnt <- %1d \n", io_info.ulIOExchangeCnt);		// Exchange IO counter
		log_msg += log_bff;

		//
		printf(log_msg);
	}
	return 1;
}
int CDNet_Mng::Call__SCAN_STOP()
{
	int dev_id = iBD_ID;
	short s_ret;

	// Signal board, application is not running
	s_ret = DevSetHostState(dev_id, HOST_NOT_READY, 0L);
	printf("DevSetHostState (HOST_NOT_READY) RetWert = %5d \n", s_ret);

	return 1;
}

int CDNet_Mng::Call__DEV_INFO()
{
	int dev_id = iBD_ID;
	short s_ret;

	// ...
	int usReadOffset = 0;
	int usReadSize = 100;
	unsigned char abIOReadData[101];

	memset(abIOReadData, 0, 100);

	COMSTATE  tComState;
	TASKSTATE tTaskState;

	if((s_ret = DevExchangeIOErr(dev_id,
		0,0,NULL,
		usReadOffset,usReadSize,&abIOReadData[0],
		&tComState,
		100L)) == DRV_NO_ERROR) 
	{
		// Check state field transfer mode
		switch(tComState.usMode) 
		{
			case STATE_MODE_3:
				// Check state field usStateFlag signals entrys
				if(tComState.usStateFlag != 0) 
				{
					// Show COM errors
					printf("STATE_3 : Show COM errors \n");
				}
				break;
			case STATE_MODE_4:
				// Check state field usStateFlag signals new entrys
				if(tComState.usStateFlag != 0) 
				{
					// Show COM errors
					printf("STATE_4 : Show COM errors \n");
				}
				break;

			default:
				// State mode unknown or not implemented
				// Read the task state field by yourself
				if(s_ret = DevGetTaskState(dev_id,0,16, &tTaskState) != DRV_NO_ERROR) 
				{
					// Error by reading the task state
					printf("Error by reading the task state \n");
				}
				break;
		} 
		/* end switch */

		// ...
		{
			CString str_data;
			CString str_hexa;
			int i_count = 0;
			int i;

			printf("=========================================================\n");

			for(i=0; i<usReadSize; i++)
			{
				i_count++;
				str_hexa.Format("%02X ", 0x0ff & abIOReadData[i]);
				str_data += str_hexa;

				if(i_count >= 10)
				{
					i_count = 0;
					printf(str_data);
					printf("\n");

					str_data = "";
				}
			}

			if(i_count >= 10)
			{
				printf(str_data);
				printf("\n");
			}
		}
	}
	return 1;
}


// ...
bool CDNet_Mng::Check__LAST_ERROR_CODE(unsigned short& err_code, CString& err_msg)
{
	if(usLAST__ERR_CODE != DRV_NO_ERROR)
	{
		err_code = usLAST__ERR_CODE;
		err_msg  = Get__ERR_CODE(err_code);

		usLAST__ERR_CODE = DRV_NO_ERROR;
		return true;
	}

	return false;
}

CString CDNet_Mng::Get__ERR_CODE(const unsigned short err_code)
{	
	if(err_code == DRV_NO_ERROR)
	{
		return "no error";
	}

	usLAST__ERR_CODE = err_code;

	switch(err_code)
	{
		case -1:		return "DRIVER Board not initialized";
		case -2:		return "DRIVER Error in internal init state";
		case -3:		return "DRIVER Error in internal read state";
		case -4:		return "DRIVER Command on this channel is activ";
		case -5:		return "DRIVER Unknown parameter in function occured";
		case -6:		return "DRIVER Version is incompatible with DLL";
		case -7:		return "DRIVER Error during PCI set run mode";
		case -8:		return "DRIVER Could not read PCI dual port memory length";
		case -9:		return "DRIVER Error during PCI set run mode";

		case -10:		return "DEVICE Dual port ram not accessable(board not found)";
		case -11:		return "DEVICE Not ready (ready flag failed)";
		case -12:		return "DEVICE Not running (running flag failed)";
		case -13:		return "DEVICE Watchdog test failed";
		case -14:		return "DEVICE Signals wrong OS version";
		case -15:		return "DEVICE Error in dual port flags";
		case -16:		return "DEVICE Send mailbox is full";
		case -17:		return "DEVICE PutMessage timeout";
		case -18:		return "DEVICE GetMessage timeout";
		case -19:		return "DEVICE No message available";

		case -20:		return "DEVICE RESET command timeout";
		case -21:		return "DEVICE COM-flag not set";
		case -22:		return "DEVICE IO data exchange failed";
		case -23:		return "DEVICE IO data exchange timeout";
		case -24:		return "DEVICE IO data mode unknown";
		case -25:		return "DEVICE Function call failed";
		case -26:		return "DEVICE DPM size differs from configuration";
		case -27:		return "DEVICE State mode unknown";
		case -28:		return "DEVICE Output port already in use";
		
		case -30:		return "USER Driver not opened (device driver not loaded)";
		case -31:		return "USER Can't connect with device";
		case -32:		return "USER Board not initialized (DevInitBoard not called)";
		case -33:		return "USER IOCTRL function failed";
		case -34:		return "USER Parameter DeviceNumber invalid";
		case -35:		return "USER Parameter InfoArea unknown";
		case -36:		return "USER Parameter Number invalid";
		case -37:		return "USER Parameter Mode invalid";
		case -38:		return "USER NULL pointer assignment";
		case -39:		return "USER Message buffer too short";

		case -40:		return "USER Parameter Size invalid";
		case -42:		return "USER Parameter Size with zero length";
		case -43:		return "USER Parameter Size too long";
		case -44:		return "USER Device address null pointer";
		case -45:		return "USER Pointer to buffer is a null pointer";
		case -46:		return "USER Parameter SendSize too long";
		case -47:		return "USER Parameter ReceiveSize too long";
		case -48:		return "USER Pointer to send buffer is a null pointer";
		case -49:		return "USER Pointer to receive buffer is a null pointer";

		case -50:		return "DMA Read IO timeout";
		case -51:		return "DMA Wrrite IO timeout";
		case -52:		return "DMA PCI transfer timeout";
		case -53:		return "DMA Download timeout";
		case -54:		return "DMA Memory allocation error";
		case -55:		return "DMA Database download failed";
		case -56:		return "DMA Firmware download failed";
		case -57:		return "DMA Clear database on the device failed";
		
		case -60:		return "DMA USER Virtual memory not available";
		case -61:		return "DMA USER Unmap virtual memory failed";

		case -70:		return "DRIVER General error";
		case -71:		return "DRIVER General DMA error";
		case -74:		return "DRIVER I/O WatchDog failed";
		case -75:		return "DRIVER Device WatchDog failed";

		case -80:		return "USER driver unknown";
		case -81:		return "USER device name invalid";
		case -82:		return "USER device name unknown";
		case -83:		return "USER device function not implemented";

		case -100:		return "USER File not opened";
		case -101:		return "USER File size zero";
		case -102:		return "USER Not enough memory to load file";
		case -103:		return "USER File read failed";
		case -104:		return "USER File type invalid";
		case -105:		return "USER File name not valid";

		case -110:		return "USER Firmware file not opened";
		case -111:		return "USER Firmware file size zero";
		case -112:		return "USER Not enough memory to load firmware file";
		case -113:		return "USER Firmware file read failed";
		case -114:		return "USER Firmware file type invalid";
		case -115:		return "USER Firmware file name not valid";
		case -116:		return "USER Firmware file download error";
		case -117:		return "USER Firmware file not found in the internal table";
		case -118:		return "USER Firmware file BOOTLOADER active";
		case -119:		return "USER Firmware file no file path";

		case -120:		return "USER Configuration file not open";
		case -121:		return "USER Configuration file size zero";
		case -122:		return "USER Not enough memory to load configuration file";
		case -123:		return "USER Configuration file read failed";
		case -124:		return "USER Configuration file type invalid";
		case -125:		return "USER Configuration file name not valid";
		case -126:		return "USER Configuration file download error";
		case -127:		return "USER No flash segment in the configuration file";
		case -128:		return "USER Configuration file differs from database";

		case -131:		return "USER Database size zero";
		case -132:		return "USER Not enough memory to upload database";
		case -133:		return "USER Database read failed";
		case -136:		return "USER Database segment unknown";
		
		case -150:		return "CONFIG Version of the descript table invalid";
		case -151:		return "CONFIG Input offset is invalid";
		case -152:		return "CONFIG Input size is 0";
		case -153:		return "CONFIG Input size does not match configuration";
		case -154:		return "CONFIG Invalid output offset";
		case -155:		return "CONFIG Output size is 0";
		case -156:		return "CONFIG Output size does not match configuration";
		case -157:		return "CONFIG Station not configured";
		case -158:		return "CONFIG Cannot get the Station configuration";
		case -159:		return "CONFIG Module definition is missing";

		case -160:		return "CONFIG Empty slot mismatch";
		case -161:		return "CONFIG Input offset mismatch";
		case -162:		return "CONFIG Output offset mismatch";
		case -163:		return "CONFIG Data type mismatch";
		case -164:		return "CONFIG Module definition is missing,(no Slot/Idx)";
	}

	if(err_code >= 1000)
	{
		return "RCS error number start";
	}

	// ...
	CString str_err;
	
	str_err.Format("Unknown Error Code (%1d)", err_code);
	return str_err;
}
