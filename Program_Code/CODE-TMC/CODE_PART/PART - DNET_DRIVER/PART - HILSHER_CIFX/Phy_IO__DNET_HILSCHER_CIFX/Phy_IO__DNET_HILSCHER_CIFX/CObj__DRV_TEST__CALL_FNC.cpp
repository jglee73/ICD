#include "StdAfx.h"
#include "CObj__DRV_TEST.h"
#include "CObj__DRV_TEST__DEF.h"

#include "CCommon_Utility.h"
#include "Macro_Function.h"


//-------------------------------------------------------------------------	
int  CObj__DRV_TEST::Call__DRIVER_OPEN(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	CString log_msg;
	CString log_bff;

	log_msg = "\n";

	if(hRes__DRV_BOARD != NULL)
	{
		log_msg += " * xDriverClose(() !!! \n";

		// ...
		xDriverClose(hRes__DRV_BOARD);
		hRes__DRV_BOARD = NULL;
	}

	// ...
	uint32_t r_ret = xDriverOpen(&hRes__DRV_BOARD);

	if(r_ret != CIFX_NO_ERROR)
	{
		log_bff.Format(" * xDriverOpen() - Error ! \n");
		log_msg += log_bff;

		log_bff.Format(" * Error Message <- %s \n", Macro__Get_CIFX_Error(r_ret));
		log_msg += log_bff;

		Write__APP_LOG(log_msg);
		return -11;
	}

	log_msg += " * xDriverOpen(() - OK \n";

	Write__APP_LOG(log_msg);
	return 1;
}

int  CObj__DRV_TEST::Call__ENUM_BOARD(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	if(hRes__DRV_BOARD == NULL)			return -1;

	return Fnc__ENUM_BOARD(hRes__DRV_BOARD);
}
int  CObj__DRV_TEST::Fnc__ENUM_BOARD(const HANDLE h_drv_board)
{
	if(h_drv_board == NULL)			return -11;

	// ...
	CString log_msg;
	CString log_bff;

	// ...
	CStringArray l__board_name;

	// ...
	uint32_t board_id = 0;
	BOARD_INFORMATION t_board_info = {0};

	log_msg = "---------- Board/Channel enumeration demo ---------- \n";

	// Iterate over all boards
	while(1)
	{
		uint32_t r_ret = xDriverEnumBoards(hRes__DRV_BOARD, board_id, sizeof(t_board_info), &t_board_info);
		if(r_ret != CIFX_NO_ERROR)			break;

		// ...
		uint32_t channel_id = 0;
		CHANNEL_INFORMATION t_channel_info = {0};

		CString str__board_name = t_board_info.abBoardName;
		l__board_name.Add(str__board_name);

		log_bff.Format("Found Board %.10s \n", str__board_name);
		log_msg += log_bff;
		
		if(strlen((char*) t_board_info.abBoardAlias) != 0)
		{
			log_bff.Format(" Alias        : %.10s \n", t_board_info.abBoardAlias);
			log_msg += log_bff;
		}

		log_bff.Format(" DeviceNumber : %u \n", t_board_info.tSystemInfo.ulDeviceNumber);
		log_msg += log_bff;

		log_bff.Format(" SerialNumber : %u \n", t_board_info.tSystemInfo.ulSerialNumber);
		log_msg += log_bff;

		log_bff.Format(" Board ID     : %u \n", t_board_info.ulBoardID);
		log_msg += log_bff;

		log_bff.Format(" System Error : 0x%08X \n", t_board_info.ulSystemError);
		log_msg += log_bff;

		log_bff.Format(" Channels     : %u \n", t_board_info.ulChannelCnt);
		log_msg += log_bff;

		log_bff.Format(" DPM Size     : %u \n", t_board_info.ulDpmTotalSize);
		log_msg += log_bff;

		// iterate over all channels on the current board 
		bool active__first_loop = true;
		HANDLE h_channel = NULL;

		while(1)
		{
			if(active__first_loop)				active__first_loop = false;
			else								++channel_id;

			if(h_channel != NULL)
			{
				xChannelClose(h_channel);
				h_channel = NULL;
			}

			r_ret = xDriverEnumChannels(hRes__DRV_BOARD, board_id, channel_id, sizeof(t_channel_info), &t_channel_info);
			if(r_ret != CIFX_NO_ERROR)			break;

			//
			log_bff.Format(" - Channel.%u \n", channel_id);
			log_msg += log_bff;

			//
			log_bff.Format("    Board Name : %s \n", t_channel_info.abBoardName);
			log_msg += log_bff;

			log_bff.Format("    Device Number : %u \n", t_channel_info.ulDeviceNumber);
			log_msg += log_bff;

			log_bff.Format("    Serial Number : %u \n", t_channel_info.ulSerialNumber);
			log_msg += log_bff;

			//
			log_bff.Format("    Firmware : %s \n", t_channel_info.abFWName);
			log_msg += log_bff;

			log_bff.Format("    Version  : %u.%u.%u build %u \n",
							t_channel_info.usFWMajor,
							t_channel_info.usFWMinor,
							t_channel_info.usFWBuild,
							t_channel_info.usFWRevision);
			log_msg += log_bff;

			log_bff.Format("    Date     : %02u/%02u/%04u \n",
							t_channel_info.bFWMonth,
							t_channel_info.bFWDay,
							t_channel_info.usFWYear);
			log_msg += log_bff;

			//
			log_bff.Format("    Channel Error : 0x%08X \n", t_channel_info.ulChannelError);
			log_msg += log_bff;

			log_bff.Format("    Open Count : %u \n", t_channel_info.ulOpenCnt);
			log_msg += log_bff;

			log_bff.Format("    Put Packet Count : %u \n", t_channel_info.ulPutPacketCnt);
			log_msg += log_bff;

			log_bff.Format("    Get Packet Count : %u \n", t_channel_info.ulGetPacketCnt);
			log_msg += log_bff;

			log_bff.Format("    Mailbox Size : %u \n", t_channel_info.ulMailboxSize);
			log_msg += log_bff;

			log_bff.Format("    I/O Input Area Count : %u \n", t_channel_info.ulIOInAreaCnt);
			log_msg += log_bff;

			log_bff.Format("    I/O Output Area Count : %u \n", t_channel_info.ulIOOutAreaCnt);
			log_msg += log_bff;

			log_bff.Format("    Handshake Size : %u \n", t_channel_info.ulHskSize);
			log_msg += log_bff;

			log_bff.Format("    netX Status Flags : 0x%08X \n", t_channel_info.ulNetxFlags);
			log_msg += log_bff;

			log_bff.Format("    Host Status Flags : 0x%08X \n", t_channel_info.ulHostFlags);
			log_msg += log_bff;

			log_bff.Format("    Host COS Flags : 0x%08X \n", t_channel_info.ulHostCOSFlags);
			log_msg += log_bff;

			log_bff.Format("    Device COS Flags : 0x%08X \n", t_channel_info.ulDeviceCOSFlags);
			log_msg += log_bff;

			// ...
			uint32_t ulState = 0;

			r_ret = xChannelOpen(hRes__DRV_BOARD, t_channel_info.abBoardName, channel_id, &h_channel);
			if(r_ret != CIFX_NO_ERROR)
			{
				log_bff.Format("    * xChannelOpen() - Error !!! \n");
				log_msg += log_bff;
				continue;
			}
		
			/*
			// Set Host Ready to signal the filed bus an application is ready
			r_ret = xChannelHostState(h_channel, CIFX_HOST_STATE_READY, &ulState, HOSTSTATE_TIMEOUT);
			if(r_ret != CIFX_NO_ERROR)
			{
				log_bff.Format("    * xChannelHostState() - Error ( HOST_STATE_READY ) !!! \n");
				log_msg += log_bff;
				
				// Close the communication channel
				xChannelClose(h_channel);
				continue;
			}
		
			// Switch on the bus if it is not automatically running (see configuration options)
			r_ret = xChannelBusState( h_channel, CIFX_BUS_STATE_ON, &ulState, 0L);		
			if(r_ret != CIFX_NO_ERROR)
			{
				log_bff.Format("    * xChannelBusState(() - Error ( BUS_STATE_ON ) !!! \n");
				log_msg += log_bff;

				// Set Host not ready to stop bus communication
				xChannelHostState(h_channel, CIFX_HOST_STATE_NOT_READY, &ulState, HOSTSTATE_TIMEOUT);

				// Close the communication channel
				xChannelClose(h_channel);
				continue;
			}
			*/

			// CIFX_IO_INPUT_AREA ...
			{
				uint32_t size__area_in = t_channel_info.ulIOInAreaCnt;

				log_bff.Format("    * CIFX_IO_INPUT_AREA - SIZE (%1d) \n", size__area_in);
				log_msg += log_bff;

				for(uint32_t ulIdx = 0; ulIdx < size__area_in; ulIdx++)
				{
					log_bff.Format("    * %1d) Input Area ... \n", ulIdx);
					log_msg += log_bff;

					//
					CHANNEL_IO_INFORMATION tIOInfo = {0};

					r_ret = xChannelIOInfo(h_channel, CIFX_IO_INPUT_AREA, ulIdx, sizeof(tIOInfo), &tIOInfo);
					if(r_ret == CIFX_NO_ERROR)
					{
						log_bff.Format("       * Size     : %u \n", tIOInfo.ulTotalSize);
						log_msg += log_bff;

						log_bff.Format("       * Reserved : %u \n", tIOInfo.ulReserved);
						log_msg += log_bff;

						log_bff.Format("       * I/O Mode : %u \n", tIOInfo.ulIOMode);
						log_msg += log_bff;				
					}
				}
			}

			// CIFX_IO_OUTPUT_AREA ...
			{
				uint32_t size__area_out = t_channel_info.ulIOOutAreaCnt;

				log_bff.Format("    * CIFX_IO_OUTPUT_AREA - SIZE (%1d) \n", size__area_out);
				log_msg += log_bff;

				for(uint32_t ulIdx = 0; ulIdx < size__area_out; ulIdx++)
				{
					log_bff.Format("    * %1d) Output Area ... \n", ulIdx);
					log_msg += log_bff;

					//
					CHANNEL_IO_INFORMATION tIOInfo = {0};

					r_ret = xChannelIOInfo(h_channel, CIFX_IO_OUTPUT_AREA, ulIdx, sizeof(tIOInfo), &tIOInfo);
					if(r_ret == CIFX_NO_ERROR)
					{
						log_bff.Format("       * Size     : %u \n", tIOInfo.ulTotalSize);
						log_msg += log_bff;

						log_bff.Format("       * Reserved : %u \n", tIOInfo.ulReserved);
						log_msg += log_bff;

						log_bff.Format("       * I/O Mode : %u \n", tIOInfo.ulIOMode);
						log_msg += log_bff;				
					}
				}
			}

			// Channel.Close ...
			{
				log_bff.Format(" * Channel.Close ... \n");
				log_msg += log_bff;

				/*
				// Switch off the bus
				xChannelBusState( h_channel, CIFX_BUS_STATE_OFF, &ulState, 0L);

				// Set Host not ready to stop bus communication
				xChannelHostState(h_channel, CIFX_HOST_STATE_NOT_READY, &ulState, HOSTSTATE_TIMEOUT);
				*/

				// Close the communication channel
				xChannelClose(h_channel);
			}		
		}

		++board_id;
	}

	// ...
	{
		log_msg += "---------------------------------------------------- \n";
		log_msg += "\n";

		Write__APP_LOG(log_msg);
	}

	if(l__board_name.GetSize() > 0)
	{
		CString str__board_name = l__board_name[0];

		sCH__TEST_PARA_BOARD_NAME->Set__DATA(str__board_name);
	}
	else
	{
		sCH__TEST_PARA_BOARD_NAME->Set__DATA("???");
	}
	return 1;
}

int  CObj__DRV_TEST::Call__SYS_DEVICE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	if(hRes__DRV_BOARD == NULL)			return -1;

	// ...
	CString test__board_name = sCH__TEST_PARA_BOARD_NAME->Get__STRING();

	char str__board_name[256];
	strcpy(str__board_name, test__board_name);

	// ...
	CString log_msg;
	CString log_bff;

	log_msg = "\n";
	log_bff.Format("---------- System Device handling demo ---------- \n");
	log_msg += log_bff;
	
	log_bff.Format(" * test__board_name <- %s \n", str__board_name);
	log_msg += log_bff;

	// ...
	HANDLE h_sys_dev = NULL;
	uint32_t r_ret;

	// Driver/Toolkit successfully opened
	r_ret = xSysdeviceOpen(hRes__DRV_BOARD, str__board_name, &h_sys_dev);
	if(r_ret != CIFX_NO_ERROR)
	{
		log_bff.Format(" * xSysdeviceOpen() - Error !!! \n");
		log_msg += log_bff;

		log_bff.Format("   * Error Message <- %s \n", Macro__Get_CIFX_Error(r_ret));
		log_msg += log_bff;

		Write__APP_LOG(log_msg);
		return -11;
	} 
	else
	{
		SYSTEM_CHANNEL_SYSTEM_INFO_BLOCK t_sys_info = {0};
		uint32_t    ul_send_pkt_count = 0;
		uint32_t    ul_recv_pkt_count = 0;
		CIFX_PACKET t_send_pkt = {0};
		CIFX_PACKET t_recv_pkt = {0};

		// System channel successfully opened, try to read the System Info Block
		r_ret = xSysdeviceInfo(h_sys_dev, CIFX_INFO_CMD_SYSTEM_INFO_BLOCK, sizeof(t_sys_info), &t_sys_info);		
		if(r_ret != CIFX_NO_ERROR)
		{
			log_bff.Format(" * xSysdeviceInfo(() - Error !!! \n");
			log_msg += log_bff;

			log_bff.Format("   * Error Message <- %s \n", Macro__Get_CIFX_Error(r_ret));
			log_msg += log_bff;
		} 
		else
		{
			log_bff.Format(" * System Channel - Info Block:\n");
			log_msg += log_bff;

			log_bff.Format("   * DPM Size         : %u \n", t_sys_info.ulDpmTotalSize);
			log_msg += log_bff;

			log_bff.Format("   * Device Number    : %u \n", t_sys_info.ulDeviceNumber);
			log_msg += log_bff;

			log_bff.Format("   * Serial Number    : %u \n", t_sys_info.ulSerialNumber);
			log_msg += log_bff;

			log_bff.Format("   * Manufacturer     : %u \n", t_sys_info.usManufacturer);
			log_msg += log_bff;

			log_bff.Format("   * Production Date  : %u \n", t_sys_info.usProductionDate);
			log_msg += log_bff;

			log_bff.Format("   * Device Class     : %u \n", t_sys_info.usDeviceClass);
			log_msg += log_bff;

			log_bff.Format("   * Revision         : %u \n", t_sys_info.bHwRevision);
			log_msg += log_bff;

			log_bff.Format("   * Compatibility    : %u \n", t_sys_info.bHwCompatibility);
			log_msg += log_bff;
		}

		// Do a simple Packet exchange via system channel
		r_ret = xSysdeviceGetMBXState(h_sys_dev, &ul_recv_pkt_count, &ul_send_pkt_count);
		if(r_ret != CIFX_NO_ERROR)
		{
			log_msg += " * xSysdeviceGetMBXState() - Error ! \n";
			log_bff.Format("   * Error Message <- %s \n", Macro__Get_CIFX_Error(r_ret));
			log_msg += log_bff;
		}
		else
		{
			log_bff.Format(" * System Mailbox State: MaxSend = %u, Pending Receive = %u \n", ul_send_pkt_count, ul_recv_pkt_count);
			log_msg += log_bff;
		}

		//
		r_ret = xSysdevicePutPacket(h_sys_dev, &t_send_pkt, PACKET_WAIT_TIMEOUT);
		if(r_ret != CIFX_NO_ERROR)
		{
			log_msg += " * Error sending packet to device! \n";
			log_bff.Format("   * Error Message <- %s \n", Macro__Get_CIFX_Error(r_ret));
			log_msg += log_bff;
		} 
		else
		{
			log_msg += Get__Dump_Packet(&t_send_pkt);

			//
			r_ret = xSysdeviceGetMBXState(h_sys_dev, &ul_recv_pkt_count, &ul_send_pkt_count);
			if(r_ret != CIFX_NO_ERROR)
			{
				log_bff.Format(" * xSysdeviceGetMBXState() - Error !!! \n");
				log_msg += log_bff;
				log_bff.Format("   * Error Message <- %s \n", Macro__Get_CIFX_Error(r_ret));
				log_msg += log_bff;
			}
			else
			{
				log_bff.Format(" * System Mailbox State: MaxSend = %u, Pending Receive = %u \n", ul_send_pkt_count, ul_recv_pkt_count);
				log_msg += log_bff;
			}

			//
			r_ret = xSysdeviceGetPacket(h_sys_dev, sizeof(t_recv_pkt), &t_recv_pkt, PACKET_WAIT_TIMEOUT);
			if(r_ret != CIFX_NO_ERROR)
			{
				log_bff.Format(" * xSysdeviceGetPacket() - Error !!! \n");
				log_msg += log_bff;
				log_bff.Format("   * Error Message <- %s \n", Macro__Get_CIFX_Error(r_ret));
				log_msg += log_bff;
			} 
			else
			{
				log_msg += Get__Dump_Packet(&t_recv_pkt);

				//
				r_ret = xSysdeviceGetMBXState(h_sys_dev, &ul_recv_pkt_count, &ul_send_pkt_count);
				if(r_ret != CIFX_NO_ERROR)
				{
					log_bff.Format(" * xSysdeviceGetMBXState() - Error !!! \n");
					log_msg += log_bff;
					log_bff.Format("   * Error Message <- %s \n", Macro__Get_CIFX_Error(r_ret));
					log_msg += log_bff;
				}
				else
				{
					log_bff.Format(" * System Mailbox State: MaxSend = %u, Pending Receive = %u \n", ul_send_pkt_count, ul_recv_pkt_count);
					log_msg += log_bff;
				}
			}
		}

		// ...
		{
			log_bff.Format(" * xSysdeviceClose() \n");
			log_msg += log_bff;

			xSysdeviceClose(h_sys_dev);
		}
	}

	log_msg += "---------------------------------------------------- \n";

	Write__APP_LOG(log_msg);
	return 1;
}

int  CObj__DRV_TEST::Call__CHANNEL_INFO(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	if(hRes__DRV_BOARD == NULL)			return -1;

	// ...
	CString log_msg;
	CString log_bff;

	log_msg  = "\n";
	log_bff.Format("---------- Communication Channel demo ---------- \n");
	log_msg += log_bff;

	// ...
	CString test__board_name = sCH__TEST_PARA_BOARD_NAME->Get__STRING();
	uint32_t ul_channel_id = (uint32_t) aCH__TEST_PARA_CHANNEL_ID->Get__VALUE();

	char str__board_name[256];
	strcpy(str__board_name, test__board_name);

	// ...
	{
		log_bff.Format(" * Test.Board_Name <- %s \n", str__board_name);
		log_msg += log_bff;

		log_bff.Format(" * Channel_ID <- %1d \n", ul_channel_id);
		log_msg += log_bff;
	}

	// ...
	HANDLE h_channel = NULL;

	uint32_t r_ret = xChannelOpen(hRes__DRV_BOARD, str__board_name, ul_channel_id, &h_channel);
	if(r_ret != CIFX_NO_ERROR)
	{
		log_bff.Format(" * xChannelOpen() - Error !!! \n");
		log_msg += log_bff;
	
		log_bff.Format("   * Error Message <- %s \n", Macro__Get_CIFX_Error(r_ret));
		log_msg += log_bff;
	} 
	else
	{
		CHANNEL_INFORMATION t_channel_info = {0};
		CIFX_PACKET  t_send_pkt = {0};
		CIFX_PACKET  t_recv_pkt = {0};
		
		unsigned char mem_send_data[32] = {0};
		unsigned char mem_recv_data[32] = {0};
		uint32_t ul_state = 0;

		// Channel successfully opened, so query basic information
		r_ret = xChannelInfo(h_channel, sizeof(CHANNEL_INFORMATION), &t_channel_info);
		if(r_ret != CIFX_NO_ERROR)
		{
			log_bff.Format(" * xChannelInfo() - Error !!! \n");
			log_msg += log_bff;

			log_bff.Format("   * Error Message <- %s \n", Macro__Get_CIFX_Error(r_ret));
			log_msg += log_bff;
		} 
		else
		{
			log_bff.Format(" * Communication Channel Info ... \n");
			log_msg += log_bff;

			log_bff.Format("   * Device Number    : %u \n", t_channel_info.ulDeviceNumber);
			log_msg += log_bff;
	
			log_bff.Format("   * Serial Number    : %u \n", t_channel_info.ulSerialNumber);
			log_msg += log_bff;

			log_bff.Format("   * Firmware         : %s \n", t_channel_info.abFWName);
			log_msg += log_bff;
			
			log_bff.Format("   * FW Version       : %u.%u.%u build %u \n",
							t_channel_info.usFWMajor,
							t_channel_info.usFWMinor,
							t_channel_info.usFWBuild,
							t_channel_info.usFWRevision);
			log_msg += log_bff;
			
			log_bff.Format("   * FW Date          : %02u/%02u/%04u \n",
							t_channel_info.bFWMonth,
							t_channel_info.bFWDay,
							t_channel_info.usFWYear);
			log_msg += log_bff;

			log_bff.Format("   * Mailbox Size     : %u \n", t_channel_info.ulMailboxSize);
			log_msg += log_bff;
		}

		// Do a basic Packet Transfer
		r_ret = xChannelPutPacket(h_channel, &t_send_pkt, PACKET_WAIT_TIMEOUT);
		if(r_ret != CIFX_NO_ERROR)
		{
			log_bff.Format(" * xChannelPutPacket() - Error !!! \n");
			log_msg += log_bff;

			log_bff.Format("   * Error Message <- %s \n", Macro__Get_CIFX_Error(r_ret));
			log_msg += log_bff;
		} 
		else
		{
			log_msg += " * xChannelPutPacket() - Info ... \n";
			log_msg += Get__Dump_Packet(&t_send_pkt);

			//
			r_ret = xChannelGetPacket(h_channel, sizeof(t_recv_pkt), &t_recv_pkt, PACKET_WAIT_TIMEOUT);
			if(r_ret != CIFX_NO_ERROR)
			{
				printf("Error getting packet from device!\r\n");
			} 
			else
			{
				log_msg += " * xChannelGetPacket() - Info ... \n";
				log_msg += Get__Dump_Packet(&t_recv_pkt);
			}
		}

		r_ret = xChannelHostState(h_channel, CIFX_HOST_STATE_READ, &ul_state, HOSTSTATE_TIMEOUT);
		if(r_ret != CIFX_NO_ERROR)
		{
			log_bff.Format(" * xChannelHostState( Host_State_Read ) - Error ??? \n");
			log_msg += log_bff;

			log_bff.Format("   * Error Message <- %s \n", Macro__Get_CIFX_Error(r_ret));
			log_msg += log_bff;
		}
		else
		{
			log_bff.Format(" * Current Host State <- %1d \n", ul_state);
			log_msg += log_bff;
			log_msg += "\n";
		}

		/*
		// Set Host Ready to signal the filed bus an application is ready
		// r_ret = xChannelHostState(h_channel, CIFX_HOST_STATE_READY, &ul_state, HOSTSTATE_TIMEOUT);
		r_ret = xChannelHostState(h_channel, CIFX_HOST_STATE_READY, &ul_state, 0);
		if(r_ret != CIFX_NO_ERROR)
		{
			log_bff.Format(" * xChannelHostState( Host_State_Ready ) - Error !!! \n");
			log_msg += log_bff;

			log_bff.Format("   * Error Message <- %s \n", Macro__Get_CIFX_Error(r_ret));
			log_msg += log_bff;
		} 
		else
		*/
		{
			/*
			log_msg += " * Set Host Ready to signal the filed bus an application is ready \n";
			log_msg += "\n";

			// Switch on the bus if it is not automatically running (see configuration options)
			r_ret = xChannelBusState(h_channel, CIFX_BUS_STATE_ON, &ul_state, 0L);			
			if(r_ret != CIFX_NO_ERROR)
			{
				log_bff.Format(" * xChannelHostState() - Error !!! \n");
				log_msg += log_bff;

				log_bff.Format("   * Error Message <- %s \n", Macro__Get_CIFX_Error(r_ret));
				log_msg += log_bff;
			} 
			else
			*/
			{
				uint32_t count__area_number = 0;

				while(1)
				{
					uint32_t data_len = sizeof(mem_recv_data);

					r_ret = xChannelIORead(h_channel, count__area_number, 0, data_len, mem_recv_data, IO_WAIT_TIMEOUT);
					if(r_ret != CIFX_NO_ERROR)
					{
						log_bff.Format(" * count__area_number <- %1d \n", count__area_number);
						log_msg += log_bff;

						log_bff.Format(" * xChannelIORead(() - Error !!! \n");
						log_msg += log_bff;

						log_bff.Format("   * Error Message <- %s \n", Macro__Get_CIFX_Error(r_ret));
						log_msg += log_bff;
						break;
					} 

					// ...
					{
						log_msg += " * xChannelIORead() - Info ... \n";
						log_msg += log_bff;

						log_bff.Format(" * Area_Number.%1d  \n", count__area_number);
						log_msg += log_bff;

						//
						log_bff.Format(" * HEXA(%1d) \n", data_len);
						log_msg += log_bff;

						for(uint32_t i=0; i<data_len; i++)
						{
							if((i > 0) && ((i % 16) == 0))			log_msg += "\n";

							log_bff.Format("%02X ", mem_recv_data[i]);
							log_msg += log_bff;
						}
						log_msg += "\n";
					}

					count__area_number++;
				}
			}
		}

		// ...
		{
			log_msg += " * Channel.Driver - Close ! \n";
			log_msg += "\n";

			/*
			// Switch off the bus
			xChannelBusState( h_channel, CIFX_BUS_STATE_OFF, &ul_state, 0L);

			// Set Host not ready to stop bus communication
			xChannelHostState(h_channel, CIFX_HOST_STATE_NOT_READY, &ul_state, HOSTSTATE_TIMEOUT);
			*/

			// Close the communication channel
			xChannelClose(h_channel);
		}
	}

	if(r_ret != CIFX_NO_ERROR)
	{
		char sz_error[1024] = {0};
		xDriverGetErrorDescription(r_ret, sz_error, sizeof(sz_error));

		log_bff.Format(" * State = 0x%08X <%s> \n", r_ret, sz_error);
		log_msg += log_bff;
	} 
	else
	{
		log_bff.Format(" * State = 0x%08X \n", r_ret);
		log_msg += log_bff;
	}

	log_msg += "---------------------------------------------------- \n";

	Write__APP_LOG(log_msg);
	return 1;
}

int  CObj__DRV_TEST::Call__DEVICE_RESET(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	if(hRes__DRV_BOARD == NULL)			return -1;

	// ...
	CString log_msg;
	CString log_bff;

	log_msg  = "\n";
	log_msg += "Channel.Reset ... \n";

	// ...
	CString test__board_name = sCH__TEST_PARA_BOARD_NAME->Get__STRING();

	char str__board_name[256];
	strcpy(str__board_name, test__board_name);

	// ...
	{
		log_bff.Format(" * Test.Board_Name <- %s \n", str__board_name);
		log_msg += log_bff;
	}

	// ...
	HANDLE h_channel = NULL;
	uint32_t ul_channel_id = 0;

	uint32_t r_ret = xChannelOpen(hRes__DRV_BOARD, str__board_name, ul_channel_id, &h_channel);
	if(r_ret != CIFX_NO_ERROR)
	{
		log_bff.Format(" * xChannelOpen() - Error !!! \n");
		log_msg += log_bff;

		log_bff.Format("   * Error Message <- %s \n", Macro__Get_CIFX_Error(r_ret));
		log_msg += log_bff;
	} 
	else
	{
		/*
		uint32_t r_ret = xDriverRestartDevice(hRes__DRV_BOARD, str__board_name, NULL);
		if(r_ret != CIFX_NO_ERROR)
		{
			log_bff.Format(" * xDriverRestartDevice() - Error !!! \n");
			log_msg += log_bff;

			log_bff.Format("   * Error Message <- %s \n", Macro__Get_CIFX_Error(r_ret));
			log_msg += log_bff;
		} 
		else
		{
			log_bff.Format(" * xDriverRestartDevice() - OK !!! \n");
			log_msg += log_bff;
		}
		*/

		// uint32_t reset_mode = CIFX_CHANNELINIT;		
		uint32_t reset_mode = CIFX_SYSTEMSTART;
		uint32_t time_out = 3000;

		uint32_t r_ret = xChannelReset(h_channel, reset_mode, time_out);
		if(r_ret != CIFX_NO_ERROR)
		{
			log_bff.Format(" * xChannelReset() - Error !!! \n");
			log_msg += log_bff;

			log_bff.Format("   * Error Message <- %s \n", Macro__Get_CIFX_Error(r_ret));
			log_msg += log_bff;
		} 
		else
		{
			log_bff.Format(" * xChannelReset() - OK !!! \n");
			log_msg += log_bff;
		}
	}

	Write__APP_LOG(log_msg);
	return 1;
}

int  CObj__DRV_TEST::Call__DRIVER_CLOSE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	if(hRes__DRV_BOARD == NULL)			return -1;

	xDriverClose(hRes__DRV_BOARD);
	hRes__DRV_BOARD = NULL;
	return 1;
}

// ...
int  CObj__DRV_TEST::Call__EXPLICIT_MESSAGE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	HANDLE h_drv_board   = mDNet_Mng.Get__DRV_BOARD_HANDLE();
	HANDLE h_drv_channel = mDNet_Mng.Get__DRV_CHANNEL_HANDLE();

	if(h_drv_board   == NULL)		return -1;
	if(h_drv_channel == NULL)		return -2;

	// ...
	bActive__EXT_MSG = true;

	CString ch_data = dCH__PARA_SLAVE_REQ_INFO->Get__STRING();
	int r_flag = Fnc__DNET_CMD__GET_ATT_REQ(h_drv_channel,ch_data);

	bActive__EXT_MSG = false;
	return r_flag;
}
int  CObj__DRV_TEST::Fnc__DNET_CMD__GET_ATT_REQ(const HANDLE h_drv_channel, 
												const CString& para__slave_info)
{
	while(bActive__POLL_LOOP)
	{
		Sleep(10);
	}

	// ...
	uint8_t  para__mac_id  = (uint8_t) aCH__PARA_SLAVE_REQ_MAC_ID->Get__VALUE();
	uint16_t para__cls_id  = 0;
	uint16_t para__inst_id = 0;
	uint16_t para__attr_id = 0;

	if(para__slave_info.CompareNoCase(STR__PRODUCT_NAME) == 0)
	{
		para__cls_id  = 0x01;
		para__inst_id = 1;
		para__attr_id = 7;
	}
	else if(para__slave_info.CompareNoCase(STR__IN_BYTE) == 0)
	{
		para__cls_id  = 0x05;
		para__inst_id = 2;
		para__attr_id = 7;
	}
	else if(para__slave_info.CompareNoCase(STR__OUT_BYTE) == 0)
	{
		para__cls_id  = 0x05;
		para__inst_id = 2;
		para__attr_id = 8;
	}

	// ...
	CString log_msg;
	CString log_bff;

	log_msg  = "\n";
	log_bff.Format("Explicit Message ...\n");
	log_msg += log_bff;

	log_bff.Format(" * para__slave_info <- %s \n", para__slave_info);
	log_msg += log_bff;

	log_bff.Format(" * para__mac_id <- %1d \n", para__mac_id);
	log_msg += log_bff;

	log_bff.Format(" * para__class_id <- %1d \n", para__cls_id);
	log_msg += log_bff;

	log_bff.Format(" * para__instance_id <- %1d \n", para__inst_id);
	log_msg += log_bff;

	log_bff.Format(" * para__attribute_id <- %1d \n", para__attr_id);
	log_msg += log_bff;

	// ...
	CIFX_PACKET t_send_pkt = { 0 };
	CIFX_PACKET t_recv_pkt = { 0 };

	CIFX_PACKET__GET_ATT_REQ* p_attr_req = (CIFX_PACKET__GET_ATT_REQ*) &t_send_pkt;

	// Send.PAcket ...
	{
		// Message header
		p_attr_req->tHeader.ulDest   = 0x20;		// 0x20 : Packet is passed to communication channel of the mailbox 
		p_attr_req->tHeader.ulSrc    = 0x10;
		p_attr_req->tHeader.ulDestId = 0x00;
		p_attr_req->tHeader.ulSrcId  = 0x00;
		p_attr_req->tHeader.ulLen    = 12;
		p_attr_req->tHeader.ulId     = 0x00;
		p_attr_req->tHeader.ulState  = 0x00;
		p_attr_req->tHeader.ulCmd    = DEVNET_FAL_CMD_GET_ATT_REQ;
		p_attr_req->tHeader.ulExt    = 0x00;
		p_attr_req->tHeader.ulRout   = 0x00;

		// Message data
		p_attr_req->bDevMacId   = para__mac_id;
		p_attr_req->usClass     = para__cls_id;
		p_attr_req->usInstance  = para__inst_id;
		p_attr_req->usAttribute = para__attr_id;

		p_attr_req->usReserved  = 0x0000;
		p_attr_req->abReserved[0] = 0x00;
		p_attr_req->abReserved[1] = 0x00;
		p_attr_req->abReserved[2] = 0x00;
	}

	// RSP.Init ...
	{
		dCH__PARA_SLAVE_RSP_STATE->Set__DATA(STR__UNKNOWN);
	
		sCH__PARA_SLAVE_RSP_DATA_HEXA->Set__DATA("");
		sCH__PARA_SLAVE_RSP_DATA_1->Set__DATA("");
		sCH__PARA_SLAVE_RSP_DATA_2->Set__DATA("");				
	}

	//
	long r_ret = xChannelPutPacket(h_drv_channel, (CIFX_PACKET*) p_attr_req, 10000L);
	if(r_ret != CIFX_NO_ERROR)
	{
		log_bff.Format(" * xChannelPutPacket() - Error !!! \n");
		log_msg += log_bff;

		log_msg += Get__Dump_Packet(&t_send_pkt);
		log_msg += "\n";

		dCH__PARA_SLAVE_RSP_STATE->Set__DATA(STR__ERROR);
	}
	else
	{
		r_ret = xChannelGetPacket(h_drv_channel, sizeof(t_recv_pkt), &t_recv_pkt, 10000L);
		if(r_ret == CIFX_NO_ERROR)
		{
			CIFX_PACKET__GET_ATT_CNF* p_attr_cnf = (CIFX_PACKET__GET_ATT_CNF*) &t_recv_pkt;
			
			bool active__recv_ok = true;

			// message command check
			if(p_attr_cnf->tHeader.ulCmd != (p_attr_req->tHeader.ulCmd + 0x01))
			{
				active__recv_ok = false;

				log_bff.Format(" * Message Command not matched ! \n");
				log_msg += log_bff;

				log_bff.Format("  ** p_attr_req->tHeader.ulCmd <- %08X \n", p_attr_req->tHeader.ulCmd);
				log_msg += log_bff;
				
				log_bff.Format("  ** p_attr_cnf->tHeader.ulCmd <- %08X \n", p_attr_cnf->tHeader.ulCmd);
				log_msg += log_bff;
			}

			// message id check
			if(p_attr_cnf->tHeader.ulId != p_attr_req->tHeader.ulId)
			{
				active__recv_ok = false;

				log_bff.Format(" * Message Id not matched ! \n");
				log_msg += log_bff;
			}

			// Error code DeviceNet FAL - Task
			if(p_attr_cnf->tHeader.ulState != 0)
			{
				active__recv_ok = false;

				log_bff.Format(" * Message Error: 0x%08X \n", p_attr_cnf->tHeader.ulState);
				log_msg += log_bff;
			}

			// General Error
			if(p_attr_cnf->bGenErr != 0)
			{
				active__recv_ok = false;

				uint8_t err_id = p_attr_cnf->bGenErr;

				log_bff.Format(" * General Error: 0x%08X \n", err_id);
				log_msg += log_bff;

				log_bff.Format(" * Error Message <- %s \n", Macro__Get_Message_Of_GenErr(err_id));
				log_msg += log_bff;
			}

			// The additional error is network specific, refer to the manual of the slave ...        
			if(p_attr_cnf->bAddErr != 0)
			{
				active__recv_ok = false;

				log_bff.Format(" * General Error: 0x%08X \n", p_attr_cnf->bAddErr);
				log_msg += log_bff;
			}

			// Attribute.Data ...
			if(active__recv_ok)
			{
				uint32_t data_ken = p_attr_cnf->tHeader.ulLen - 12;

				log_bff.Format("HEXA(%1d) \n", data_ken);
				log_msg += log_bff;

				if(data_ken > 0)
				{	
					CString str_hexa;

					for(uint32_t i=0; i<data_ken; i++)
					{
						if((i > 0) && ((i % 16) == 0))			log_msg += "\n";
	
						log_bff.Format("%02X ", p_attr_cnf->abAttData[i]);
	
						str_hexa += log_bff;
						log_msg += log_bff;
					}
	
					sCH__PARA_SLAVE_RSP_DATA_HEXA->Set__DATA(str_hexa);
	
					log_msg += "\n";
				}

				if((para__slave_info.CompareNoCase(STR__IN_BYTE)  == 0)
				|| (para__slave_info.CompareNoCase(STR__OUT_BYTE) == 0))
				{
					UNION_2_BYTE__UINT m_uint;
					m_uint.cBYTE[0] = 0x0ff & p_attr_cnf->abAttData[0];
					m_uint.cBYTE[1] = 0x0ff & p_attr_cnf->abAttData[1];

					CString str__byte_size;
					str__byte_size.Format("%1d", m_uint.uiDATA);

					//
					dCH__PARA_SLAVE_RSP_STATE->Set__DATA(STR__OK);

					sCH__PARA_SLAVE_RSP_DATA_1->Set__DATA(str__byte_size);
					sCH__PARA_SLAVE_RSP_DATA_2->Set__DATA("");				
				}
				else if(para__slave_info.CompareNoCase(STR__PRODUCT_NAME) == 0)
				{
					CString str_gas_name;

					int ch_len = p_attr_cnf->abAttData[0];
					if(ch_len > 256)		ch_len = 256;

					for(int i=0; i<ch_len; i++)
					{
						str_gas_name += (char) p_attr_cnf->abAttData[i + 1];
					}

					dCH__PARA_SLAVE_RSP_STATE->Set__DATA(STR__OK);

					sCH__PARA_SLAVE_RSP_DATA_1->Set__DATA(str_gas_name);
					sCH__PARA_SLAVE_RSP_DATA_2->Set__DATA("");
				}
			}
			else
			{
				dCH__PARA_SLAVE_RSP_STATE->Set__DATA(STR__ERROR);
			}
		}
		else
		{
			log_bff.Format(" * Error: 0x%08X \n", r_ret);
			log_msg += log_bff;

			dCH__PARA_SLAVE_RSP_STATE->Set__DATA(STR__ERROR);
		}
	}

	Write__APP_LOG(log_msg);
	return 1;
}

CString CObj__DRV_TEST::Get__Dump_Packet(CIFX_PACKET* ptPacket)
{
	CString log_msg;
	CString log_bff;

	log_msg = "Dump_Packet ... \n";

	log_bff.Format(" Dest   : 0x%08X      ID   : 0x%08X \n", ptPacket->tHeader.ulDest,   ptPacket->tHeader.ulId);
	log_msg += log_bff;

	log_bff.Format(" Src    : 0x%08X      Sta  : 0x%08X \n", ptPacket->tHeader.ulSrc,    ptPacket->tHeader.ulState);

	log_bff.Format(" DestID : 0x%08X      Cmd  : 0x%08X \n", ptPacket->tHeader.ulDestId, ptPacket->tHeader.ulCmd);

	log_bff.Format(" SrcID  : 0x%08X      Ext  : 0x%08X \n", ptPacket->tHeader.ulSrcId,  ptPacket->tHeader.ulExt);

	log_bff.Format(" Len    : 0x%08X      Rout : 0x%08X \n", ptPacket->tHeader.ulLen,    ptPacket->tHeader.ulRout);

	log_msg += Get__Dump_Data(ptPacket->abData, ptPacket->tHeader.ulLen);
	log_msg += "\n";

	return log_msg;
}
CString CObj__DRV_TEST::Get__Dump_Data(unsigned char* p_data, unsigned long ch_len)
{
	CString log_msg;
	CString log_bff;

	log_msg += "Dump_Data:... \n";

	for(unsigned long ch_i = 0; ch_i < ch_len; ++ch_i)
	{
		if((ch_i > 0) && ((ch_i % 16) == 0))			log_msg += "\n";

		log_bff.Format("%02X ", p_data[ch_i]);
		log_msg += log_bff;
	}
	return log_msg;
}


//-------------------------------------------------------------------------	
int  CObj__DRV_TEST
::Call__LINK_IO_SET_OFF(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	int i_limit = iSIZE__LINK_IO;
	int i;

	for(i=0; i<i_limit; i++)
	{
		dEXT_CH__DNET_LINK_IO[i]->Set__DATA(STR__OFF);
	}
	return 1;
}
int  CObj__DRV_TEST
::Call__LINK_IO_SET_ON(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	int i_limit = iSIZE__LINK_IO;
	int i;

	for(i=0; i<i_limit; i++)
	{
		dEXT_CH__DNET_LINK_IO[i]->Set__DATA(STR__ON);
	}
	return 1;
}
