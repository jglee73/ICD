#include "StdAfx.h"
#include "DNet_Mng.h"

#include "cifuser.h"
#include "rcs_user.h"
#include "dnm_user.h"

#include "Macro_Function.h"


//--------------------------------------------------------------------------------
int CDNet_Mng::Set__BOARD_ID(const int bd_id)
{
	iBOARD_ID = bd_id;
	return 1;
}
int CDNet_Mng::Get__BOARD_ID()
{
	return iBOARD_ID;
}

// ...
int CDNet_Mng::Set__CHANNEL_ID(const int ch_id)
{
	iCHANNEL_ID = ch_id;
	return 1;
}
int CDNet_Mng::Get__CHANNEL_ID()
{
	return iCHANNEL_ID;
}

// ...
int CDNet_Mng::Call__CHANNEL_OPEN()
{
	Call__CHANNEL_CLOSE();
	_Clear__AREA_INFO();

	// ...
	CString log_msg;
	CString log_bff;

	log_msg  = "\n";
	log_msg += "CDNet_Mng::Call__CHANNEL_OPEN() ... \n";

	// ...
	uint32_t r_ret = xDriverOpen(&hRes__DRV_BOARD);
	if(r_ret != CIFX_NO_ERROR)
	{
		log_bff.Format(" * xDriverOpen() - Error ! \n");
		log_msg += log_bff;		
		log_msg += Macro__Get_CIFX_Error(r_ret);
		log_msg += "\n";

		pI__LOG_CTRL->WRITE__LOG(log_msg);
		return -11;
	}

	// ...
	uint32_t bd_id = (uint32_t) iBOARD_ID;
	uint32_t ch_id = (uint32_t) iCHANNEL_ID;

	// ...
	{
		log_bff.Format(" * Board_ID   <- %1d \n", bd_id);
		log_msg += log_bff;		

		log_bff.Format(" * Channel_ID <- %1d \n", ch_id);
		log_msg += log_bff;		
	}

	// Board.Info ...
	{
		BOARD_INFORMATION t_board_info = {0};

		long r_ret = xDriverEnumBoards(hRes__DRV_BOARD, bd_id, sizeof(t_board_info), &t_board_info);
		if(r_ret != CIFX_NO_ERROR)
		{
			log_bff.Format(" * xDriverEnumBoards(() - Error ! \n");
			log_msg += log_bff;		
			log_msg += Macro__Get_CIFX_Error(r_ret);
			log_msg += "\n";

			pI__LOG_CTRL->WRITE__LOG(log_msg);
			return -21;
		}

		// ...
		CString str__board_name = t_board_info.abBoardName;
		sRes__BOARD_NAME = str__board_name;

		if(strlen((char*) t_board_info.abBoardAlias) != 0)
		{
			sRes__ALIAS_NAME = t_board_info.abBoardAlias;
		}

		sRes__DEVICE_NUMBER.Format("%u", t_board_info.tSystemInfo.ulDeviceNumber);
		sRes__SERIAL_NUMBER.Format("%u", t_board_info.tSystemInfo.ulSerialNumber);
		sRes__BOARD_ID.Format("%u", t_board_info.ulBoardID);
		sRes__SYSTEM_ERROR.Format("0x%08X", t_board_info.ulSystemError);
		sRes__CHANNEL_COUNT.Format("%u", t_board_info.ulChannelCnt);
		sRes__DMP_SIZE.Format("%u", t_board_info.ulDpmTotalSize);
	}

	// Channel.Open ...
	{
		HANDLE h_channel = NULL;

		char str__board_name[256];
		strcpy(str__board_name, sRes__BOARD_NAME);

		r_ret = xChannelOpen(hRes__DRV_BOARD, str__board_name, ch_id, &h_channel);
		if(r_ret != CIFX_NO_ERROR)
		{
			log_bff.Format(" * xChannelOpen() - Error ! \n");
			log_msg += log_bff;		
			log_msg += Macro__Get_CIFX_Error(r_ret);
			log_msg += "\n";

			pI__LOG_CTRL->WRITE__LOG(log_msg);
			return -101;
		}

		hRes__DRV_CHANNEL = h_channel;
	}

	// Channel.Info ...
	{
		HANDLE h_channel = hRes__DRV_CHANNEL;

		CHANNEL_INFORMATION t_channel_info = {0};
		CIFX_PACKET  t_send_pkt = {0};
		CIFX_PACKET  t_recv_pkt = {0};
		uint32_t ul_state = 0;

		// Channel successfully opened, so query basic information
		r_ret = xChannelInfo(h_channel, sizeof(CHANNEL_INFORMATION), &t_channel_info);
		if(r_ret != CIFX_NO_ERROR)
		{
			log_bff.Format(" * xChannelInfo() - Error ! \n");
			log_msg += log_bff;		
			log_msg += Macro__Get_CIFX_Error(r_ret);
			log_msg += "\n";

			pI__LOG_CTRL->WRITE__LOG(log_msg);
			return -201;
		} 

		// ...
		{
			sRes__CHANNEL_FIRMWARE.Format("%s", t_channel_info.abFWName);

			sRes__CHANNEL_VERSION.Format("%u.%u.%u build %u",
										 t_channel_info.usFWMajor,
										 t_channel_info.usFWMinor,
										 t_channel_info.usFWBuild,
										 t_channel_info.usFWRevision);

			sRes__CHANNEL_DATE.Format("%02u/%02u/%04u",
									  t_channel_info.bFWMonth,
									  t_channel_info.bFWDay,
									  t_channel_info.usFWYear);

			//
			sRes__CHANNEL_Error.Format("0x%08X", t_channel_info.ulChannelError);
			sRes__CHANNEL_OpenCount.Format("%u", t_channel_info.ulOpenCnt);
			sRes__CHANNEL_PutPacketCount.Format("%u", t_channel_info.ulPutPacketCnt);
			sRes__CHANNEL_GetPacketCount.Format("%u", t_channel_info.ulGetPacketCnt);
			sRes__CHANNEL_MailboxSize.Format("%u", t_channel_info.ulMailboxSize);

			sRes__CHANNEL_IOInAreaCount.Format("%u",  t_channel_info.ulIOInAreaCnt);
			sRes__CHANNEL_IOOutAreaCount.Format("%u", t_channel_info.ulIOInAreaCnt);

			sRes__CHANNEL_HandShakeSize.Format("%u", t_channel_info.ulHskSize);
			sRes__CHANNEL_NetxFlag.Format("0x%08X",  t_channel_info.ulNetxFlags);
			sRes__CHANNEL_HostFlag.Format("0x%08X",  t_channel_info.ulHostFlags);
			sRes__CHANNEL_HostCOSFlag.Format("0x%08X", t_channel_info.ulHostCOSFlags);
			sRes__CHANNEL_DeviceCOSFlag.Format("0x%08X", t_channel_info.ulDeviceCOSFlags);
		}

		// ...
		{
			log_bff.Format(" * BOARD_NAME <- %s \n", sRes__BOARD_NAME);
			log_msg += log_bff;

			log_bff.Format(" * ALIAS_NAME <- %s \n", sRes__ALIAS_NAME);
			log_msg += log_bff;

			log_bff.Format(" * DEVICE_NUMBER <- %s \n", sRes__DEVICE_NUMBER);
			log_msg += log_bff;

			log_bff.Format(" * SERIAL_NUMBER <- %s \n", sRes__SERIAL_NUMBER);
			log_msg += log_bff;

			log_bff.Format(" * BOARD_ID <- %s \n", sRes__BOARD_ID);
			log_msg += log_bff;

			log_bff.Format(" * SYSTEM_ERROR <- %s \n", sRes__SYSTEM_ERROR);
			log_msg += log_bff;

			log_bff.Format(" * CHANNEL_COUNT <- %s \n", sRes__CHANNEL_COUNT);
			log_msg += log_bff;

			log_bff.Format(" * DMP_SIZE <- %s \n", sRes__DMP_SIZE);
			log_msg += log_bff;

			//
			log_bff.Format(" * CHANNEL_FIRMWARE <- %s \n", sRes__CHANNEL_FIRMWARE);
			log_msg += log_bff;

			log_bff.Format(" * CHANNEL_VERSION <- %s \n", sRes__CHANNEL_VERSION);
			log_msg += log_bff;

			log_bff.Format(" * CHANNEL_DATE <- %s \n", sRes__CHANNEL_DATE);
			log_msg += log_bff;

			//
			log_bff.Format(" * CHANNEL_Error <- %s \n", sRes__CHANNEL_Error);
			log_msg += log_bff;

			log_bff.Format(" * CHANNEL_OpenCount <- %s \n", sRes__CHANNEL_OpenCount);
			log_msg += log_bff;

			log_bff.Format(" * CHANNEL_PutPacketCount <- %s \n", sRes__CHANNEL_PutPacketCount);
			log_msg += log_bff;

			log_bff.Format(" * CHANNEL_GetPacketCount <- %s \n", sRes__CHANNEL_GetPacketCount);
			log_msg += log_bff;

			log_bff.Format(" * CHANNEL_MailboxSize < -%s \n", sRes__CHANNEL_MailboxSize);
			log_msg += log_bff;

			log_bff.Format(" * CHANNEL_IOInAreaCount <- %s \n", sRes__CHANNEL_IOInAreaCount);
			log_msg += log_bff;

			log_bff.Format(" * CHANNEL_IOOutAreaCount <- %s \n", sRes__CHANNEL_IOOutAreaCount);
			log_msg += log_bff;

			log_bff.Format(" * CHANNEL_HandShakeSize <- %s \n", sRes__CHANNEL_HandShakeSize);
			log_msg += log_bff;

			log_bff.Format(" * CHANNEL_NetxFlag <- %s \n", sRes__CHANNEL_NetxFlag);
			log_msg += log_bff;

			log_bff.Format(" * CHANNEL_HostFlag <- %s \n", sRes__CHANNEL_HostFlag);
			log_msg += log_bff;

			log_bff.Format(" * CHANNEL_HostCOSFlag < -%s \n", sRes__CHANNEL_HostCOSFlag);
			log_msg += log_bff;

			log_bff.Format(" * CHANNEL_DeviceCOSFlag <- %s \n", sRes__CHANNEL_DeviceCOSFlag);
			log_msg += log_bff;
		}

		// Do a basic Packet Transfer
		r_ret = xChannelPutPacket(h_channel, &t_send_pkt, PACKET_WAIT_TIMEOUT);
		if(r_ret != CIFX_NO_ERROR)
		{
			log_bff.Format(" * xChannelPutPacket() - Error ! \n");
			log_msg += log_bff;		
			log_msg += Macro__Get_CIFX_Error(r_ret);
			log_msg += "\n";

			pI__LOG_CTRL->WRITE__LOG(log_msg);
			return -202;
		} 
		else
		{
			r_ret = xChannelGetPacket(h_channel, sizeof(t_recv_pkt), &t_recv_pkt, PACKET_WAIT_TIMEOUT);
			if(r_ret != CIFX_NO_ERROR)
			{
				log_bff.Format(" * xChannelGetPacket() - Error ! \n");
				log_msg += log_bff;		
				log_msg += Macro__Get_CIFX_Error(r_ret);
				log_msg += "\n";

				pI__LOG_CTRL->WRITE__LOG(log_msg);
				return -203;
			} 
		}

		// CIFX_IO_INPUT_AREA ...
		{
			uint32_t size__area_in = t_channel_info.ulIOInAreaCnt;

			log_bff.Format(" * IO.InAreaCount <- [%1d] \n", size__area_in);
			log_msg += log_bff;		

			for(uint32_t idx = 0; idx < size__area_in; idx++)
			{
				log_bff.Format("   * %02d) IO.In_Byte - Info ... \n", idx);
				log_msg += log_bff;		

				// ...
				CHANNEL_IO_INFORMATION t_io_info = {0};

				r_ret = xChannelIOInfo(h_channel, CIFX_IO_INPUT_AREA, idx, sizeof(t_io_info), &t_io_info);
				if(r_ret != CIFX_NO_ERROR)
				{
					log_bff.Format(" * xChannelIOInfo() - Error ! \n");
					log_msg += log_bff;		
					log_msg += Macro__Get_CIFX_Error(r_ret);
					log_msg += "\n";

					pI__LOG_CTRL->WRITE__LOG(log_msg);
					return -211;
				}

				// ...
				{
					iList__AREA_IN__Total_Size.Add(	t_io_info.ulTotalSize );
					iList__AREA_IN__Reserved.Add( t_io_info.ulReserved );
					iList__AREA_IN__IO_Mode.Add( t_io_info.ulIOMode );
				}

				// ...
				{
					log_bff.Format("     * Total.Size <- [%1d] \n", t_io_info.ulTotalSize);
					log_msg += log_bff;		

					log_bff.Format("     * Reserved <- [%1d] \n", t_io_info.ulReserved);
					log_msg += log_bff;		

					log_bff.Format("     * IO.Mode  <- [%1d] \n", t_io_info.ulIOMode);
					log_msg += log_bff;		
				}
			}
		}

		// CIFX_IO_OUTPUT_AREA ...
		{
			uint32_t size__area_out = t_channel_info.ulIOOutAreaCnt;

			log_bff.Format(" * IO.OutAreaCount <- [%1d] \n", size__area_out);
			log_msg += log_bff;		

			for(uint32_t idx = 0; idx < size__area_out; idx++)
			{
				log_bff.Format("   * %02d) IO.Out_Byte - Info ... \n", idx);
				log_msg += log_bff;		

				// ...
				CHANNEL_IO_INFORMATION t_io_info = {0};

				r_ret = xChannelIOInfo(h_channel, CIFX_IO_OUTPUT_AREA, idx, sizeof(t_io_info), &t_io_info);
				if(r_ret != CIFX_NO_ERROR)
				{
					log_bff.Format(" * xChannelIOInfo() - Error ! \n");
					log_msg += log_bff;		
					log_msg += Macro__Get_CIFX_Error(r_ret);
					log_msg += "\n";

					pI__LOG_CTRL->WRITE__LOG(log_msg);
					return -212;
				}

				// ...
				{
					iList__AREA_OUT__Total_Size.Add( t_io_info.ulTotalSize );
					iList__AREA_OUT__Reserved.Add( t_io_info.ulReserved );
					iList__AREA_OUT__IO_Mode.Add( t_io_info.ulIOMode );
				}

				// ...
				{
					log_bff.Format("     * Total.Size <- [%1d] \n", t_io_info.ulTotalSize);
					log_msg += log_bff;		

					log_bff.Format("     * Reserved <- [%1d] \n", t_io_info.ulReserved);
					log_msg += log_bff;		

					log_bff.Format("     * IO.Mode  <- [%1d] \n", t_io_info.ulIOMode);
					log_msg += log_bff;		
				}
			}
		}

		// ...
	}

	// ...
	{
		log_msg += "\n";		

		log_bff.Format(" * hRes__DRV_BOARD   <- %08X \n", hRes__DRV_BOARD);
		log_msg += log_bff;		

		log_bff.Format(" * hRes__DRV_CHANNEL <- %08X \n", hRes__DRV_CHANNEL);
		log_msg += log_bff;		
	}

	pI__LOG_CTRL->WRITE__LOG(log_msg);
	return 1;
}
int CDNet_Mng::Call__CHANNEL_CLOSE()
{
	CString log_msg;
	CString log_bff;

	// ...
	{
		log_msg  = "\n";
		log_msg += "Call__CHANNEL_CLOSE() ... \n";
	}

	if(hRes__DRV_CHANNEL != NULL)
	{
		log_msg += "Channel.Driver Close ... \n";	

		// ...
		uint32_t ul_state;

		/*
		xChannelBusState(hRes__DRV_CHANNEL, CIFX_BUS_STATE_OFF, &ul_state, 0L);
		xChannelHostState(hRes__DRV_CHANNEL, CIFX_HOST_STATE_NOT_READY, &ul_state, HOSTSTATE_TIMEOUT);
		*/
		xChannelClose(hRes__DRV_CHANNEL);

		hRes__DRV_CHANNEL = NULL;
	}

	if(hRes__DRV_BOARD != NULL)
	{
		log_msg += "Board.Driver Close ... \n";	

		// ...
		xDriverClose(hRes__DRV_BOARD);

		hRes__DRV_BOARD = NULL;
	}

	pI__LOG_CTRL->WRITE__LOG(log_msg);
	return 1;
}

int CDNet_Mng::Call__SCAN_START()
{
	if(hRes__DRV_BOARD   == NULL)				return -11;
	if(hRes__DRV_CHANNEL == NULL)				return -12;

	// ...
	CString log_msg;
	CString log_bff;

	log_msg  = "\n";
	log_msg += "CDNet_Mng::Call__SCAN_START() ... \n";

	// ...
	HANDLE h_channel = hRes__DRV_CHANNEL;

	uint32_t ul_state = 0;
	uint32_t r_ret;

	// Set Host Ready to signal the filed bus an application is ready
	{
		r_ret = xChannelHostState(h_channel, CIFX_HOST_STATE_READY, &ul_state, HOSTSTATE_TIMEOUT);
		if(r_ret != CIFX_NO_ERROR)
		{
			log_bff.Format(" * xChannelHostState() - Error( Host_State_Ready ) ! \n");
			log_msg += log_bff;		
			log_msg += Macro__Get_CIFX_Error(r_ret);
			log_msg += "\n";

			pI__LOG_CTRL->WRITE__LOG(log_msg);
			return -101;
		}
		else
		{
			log_bff.Format(" * xChannelHostState() <- Host_State_Ready \n");
			log_msg += log_bff;		
		}

		// ...
		CString str_state;

			 if(ul_state == CIFX_HOST_STATE_NOT_READY)			str_state = "Clears the application ready flag";
		else if(ul_state == CIFX_HOST_STATE_READY)				str_state = "Sets the application ready flag";
		else if(ul_state == CIFX_HOST_STATE_READ)				str_state = "Read the current state of the flag";
		else													str_state.Format("Unknown State (%1d)", ul_state);

		// ...
		{
			log_msg += str_state;
			log_msg += "\n";
		}
	}

	// Switch on the bus if it is not automatically running (see configuration options)
	{
		r_ret = xChannelBusState( h_channel, CIFX_BUS_STATE_ON, &ul_state, 0L);		
		if(r_ret != CIFX_NO_ERROR)
		{
			log_bff.Format(" * xChannelBusState() - Error( Bus_State_On ) ! \n");
			log_msg += log_bff;		
			log_msg += Macro__Get_CIFX_Error(r_ret);
			log_msg += "\n";

			pI__LOG_CTRL->WRITE__LOG(log_msg);
			return -102;
		}

		// ...
		CString str_state;

			 if(ul_state == CIFX_BUS_STATE_OFF)					str_state = "Clears the BUS state flag";
		else if(ul_state == CIFX_BUS_STATE_ON)					str_state = "Sets the bus state flag";
		else if(ul_state == CIFX_BUS_STATE_GETSTATE)			str_state = "Read the actual state of the bus state flag";
		else													str_state.Format("Unknown State (%1d)", ul_state);

		// ...
		{
			log_msg += str_state;
			log_msg += "\n";
		}
	}

	pI__LOG_CTRL->WRITE__LOG(log_msg);
	return 1;
}
int CDNet_Mng::Call__SCAN_STOP()
{
	CString log_msg;
	CString log_bff;

	log_msg  = "\n";
	log_msg += "CDNet_Mng::Call__SCAN_STOP() ... \n";

	if(hRes__DRV_CHANNEL != NULL)
	{
		log_msg += "Channel.Driver - Scan_Stop ! \n";

		// ...
		uint32_t ul_state;

		/*
		xChannelBusState(hRes__DRV_CHANNEL, CIFX_BUS_STATE_OFF, &ul_state, 0L);
		xChannelHostState(hRes__DRV_CHANNEL, CIFX_HOST_STATE_NOT_READY, &ul_state, HOSTSTATE_TIMEOUT);
		*/

		xChannelClose(hRes__DRV_CHANNEL);

		hRes__DRV_CHANNEL = NULL;
	}

	pI__LOG_CTRL->WRITE__LOG(log_msg);
	return 1;
}


// ...
bool CDNet_Mng::Check__LAST_ERROR_CODE(uint32_t& err_code, CString& err_msg)
{
	if(usLAST__ERR_CODE != CIFX_NO_ERROR)
	{
		char sz_error[1024] = {0};
		xDriverGetErrorDescription(err_code, sz_error, sizeof(sz_error));

		err_code = usLAST__ERR_CODE;
		err_msg.Format("%s", sz_error);

		usLAST__ERR_CODE = DRV_NO_ERROR;
		return true;
	}

	return false;
}
