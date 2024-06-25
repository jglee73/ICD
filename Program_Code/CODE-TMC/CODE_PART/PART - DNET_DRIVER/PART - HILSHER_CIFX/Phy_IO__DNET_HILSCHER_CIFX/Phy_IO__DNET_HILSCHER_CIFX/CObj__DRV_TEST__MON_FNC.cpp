#include "StdAfx.h"
#include "CObj__DRV_TEST.h"
#include "CObj__DRV_TEST__DEF.h"

#include "Macro_Function.h"


//------------------------------------------------------------------------------------
void CObj__DRV_TEST
::Mon__MONITOR(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	CString ch_data;

	// ...
	SCX__ASYNC_TIMER_CTRL x_heartbeat_timer;

	if(bActive__DO_DNET_HEARTBEAT)
	{
		x_heartbeat_timer->REGISTER__COUNT_CHANNEL_NAME(sCH__DNET_HEARTBEAT_TIME_COUNT->Get__CHANNEL_NAME());
		x_heartbeat_timer->START__COUNT_UP(9999);
	}

	// ...
	int active__link_io = -1;
	double cfg__wait_sec = aCH__CFG_LINK_IO_WAITING_SEC->Get__VALUE();

	SCX__ASYNC_TIMER_CTRL x_link_io_timer;

	if(iSIZE__LINK_IO > 0)
	{
		x_link_io_timer->START__COUNT_UP(9999);
	}

	// ...
	int loop_count = 0;

	if((bActive__DNET_INIT)
	|| (iActive__SIM_MODE > 0))
	{
		if(dCH__CFG_DRV_INT_ERROR_CHECK->Check__DATA(STR__YES) > 0)
		{
			if(Check__DEV_ERROR() > 0)
			{
				// ...
				{
					double cfg_sec = aCH__CFG_DRV_INT_STABLE_SEC->Get__VALUE();
					double cfg_msec = cfg_sec * 1000.0;
		
					Sleep(cfg_msec);   // NDet ¾ÈÁ¤È­
				}
	
				Call__DEV_INFO(p_variable, p_alarm);
			}
		}
	}

	if(iActive__SIM_MODE > 0)
	{
		bActive__DNET_INIT = true;
	}

	if(bActive__DNET_INIT)			bActive__COMM_STATE = true;
	else							bActive__COMM_STATE = false;

	// ...
	{
		bActive__DNET_START = true;

		dCH__MON_ACTIVE_DNET_START->Set__DATA(STR__ON);
	}


	while(1)
	{
		bActive__POLL_LOOP = false;

		p_variable->Wait__SINGLE_OBJECT(0.02);

		if(bActive__EXT_MSG)		continue;
		bActive__POLL_LOOP = true;

		loop_count++;
		if(loop_count > 20)			loop_count = 1;


		// LINK : IO ...
		if(iSIZE__LINK_IO > 0)
		{
			if(active__link_io < 0)
			{
				if(x_link_io_timer->Get__CURRENT_TIME() > cfg__wait_sec)
				{
					active__link_io = 1;
					x_link_io_timer->STOP_ZERO();

					int i_limit = iSIZE__LINK_IO;
					for(int i=0; i<i_limit; i++)
					{
						dEXT_CH__DNET_LINK_IO[i]->Set__DATA("ON");
					}
				}
			}
		}

		if(bActive__DO_DNET_HEARTBEAT)
		{
			double cfg_sec = aCH__CFG_DNET_HEARTBEAT_PERIOD->Get__VALUE();

			if(cfg_sec < 1)
			{
				x_heartbeat_timer->STOP_ZERO();
			}
			else
			{
				double cur_sec = x_heartbeat_timer->Get__CURRENT_TIME();

				if(cur_sec >= cfg_sec)
				{
					x_heartbeat_timer->STOP_ZERO();
					x_heartbeat_timer->START__COUNT_UP(9999);

					if(dEXT_CH__DO_DNET_HEARTBEAT->Check__DATA("ON") < 0)
					{
						dEXT_CH__DO_DNET_HEARTBEAT->Set__DATA("ON");
					}
					else
					{
						dEXT_CH__DO_DNET_HEARTBEAT->Set__DATA("OFF");
					}
				}
				else if(cur_sec < 0.001)
				{
					x_heartbeat_timer->START__COUNT_UP(9999);
				}
			}
		}

		if(bActive__DNET_INIT)
		{
			EnterCriticalSection(&mLOCK_DNET);

			if(iActive__SIM_MODE > 0)
			{

			}
			else
			{
				_DNet__UPDATE_NODE_ALL(p_variable, p_alarm);
			}

			LeaveCriticalSection(&mLOCK_DNET);
		}	
		else
		{
			// ...
			{
				int alm_id = ALID__DNET_INIT_ERROR;
				CString r_act;

				p_alarm->Check__ALARM(alm_id, r_act);
				p_alarm->Post__ALARM(alm_id);
			}
		}

		if(loop_count == 1)
		{
			if(bActive__COMM_STATE)
			{
				dCH__MON_COMM_STATE->Set__DATA(STR__ONLINE);
			}
			else
			{
				dCH__MON_COMM_STATE->Set__DATA(STR__OFFLINE);

				// ...
				{
					int alm_id = ALID__OFFLINE;
					CString alm_msg;
					CString	r_act;

					alm_msg = "";

					p_alarm->Check__ALARM(alm_id, r_act);
					p_alarm->Post__ALARM_With_MESSAGE(alm_id, alm_msg);
				}
			}

			// ...
			uint32_t err_code;
			CString  err_msg;

			if(mDNet_Mng.Check__LAST_ERROR_CODE(err_code, err_msg))
			{
				int alm_id = ALID__LAST_ERROR_CODE;
				CString r_act;

				p_alarm->Check__ALARM(alm_id, r_act);
				p_alarm->Post__ALARM_With_MESSAGE(alm_id, err_msg);
			}
		}
	}

	// ...
}

void CObj__DRV_TEST
::_DNet__UPDATE_NODE_ALL(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	if(iDNet_BoardNumber < 0)
	{
		printf("iDNet_BoardNumber < -%1d \n", iDNet_BoardNumber);
		return;
	}

	// ...
	HANDLE h_channel_res = mDNet_Mng.Get__DRV_CHANNEL_HANDLE();
	if(h_channel_res == NULL)		return;

	// ...
	bool active__input_err  = false;
	bool active__output_err = false;

	CString msg__err_input;
	CString msg__err_output;
	CString err_bff;

	// ...
	bool active__in_byte_check  = false;
	bool active__out_byte_check = false;

	if(dCH__DNET_INFO__IN_BYTE_CHECK_ACTIVE->Check__DATA(STR__YES) > 0)
	{
		active__in_byte_check = true;

		dCH__DNET_INFO__IN_BYTE_CHECK_ACTIVE->Set__DATA(STR__NO);
	}
	if(dCH__DNET_INFO__OUT_BYTE_CHECK_ACTIVE->Check__DATA(STR__YES) > 0)
	{
		active__out_byte_check = true;

		dCH__DNET_INFO__OUT_BYTE_CHECK_ACTIVE->Set__DATA(STR__NO);
	}

	// Output.Download ...
	{
		mCtrl__DNet_Node.Update__ALL_OUT_BYTE(cDNet_OutputData, uDNet_OutputSize);

		if(active__out_byte_check)
		{
			CString log_msg;
			CString log_bff;

			log_msg.Format("Output_Node.All_Info ... \n");

			log_bff.Format(" * uDNet_OutputSize <- %1d \n", uDNet_OutputSize);
			log_msg += log_bff;					

			log_bff.Format("HEXA Info ... \n");
			log_msg += log_bff;

			for(int i=0; i<uDNet_OutputSize; i++)
			{
				if((i > 0) && ((i % 10) == 0))			log_msg += "\n";		

				log_bff.Format("%02X ", 0x0ff & cDNet_OutputData[i]);
				log_msg += log_bff;
			}
			log_msg += "\n";

			Write__APP_LOG(log_msg);
		}

		// ...
		unsigned int area_index    = 0;
		unsigned int output_offset = (unsigned int) iDNet_Board_Out_Offset;

		long r_ret = xChannelIOWrite(h_channel_res, area_index, output_offset, uDNet_OutputSize, cDNet_OutputData, IO_WAIT_TIMEOUT);
		if(r_ret != CIFX_NO_ERROR)
		{
			active__output_err = true;

			msg__err_output  = "xChannelIOWrite() - Error \n";
			msg__err_output += "Error Message : \n";
			msg__err_output += "  ";
			msg__err_output += Macro__Get_CIFX_Error(r_ret);
			msg__err_output += "\n";
		}
	}

	// Input.Upload ...
	{
		unsigned int area_index   = 0;
		unsigned int input_offset = (unsigned int) iDNet_Board_In_Offset;

		long r_ret = xChannelIORead(h_channel_res, area_index, input_offset, uDNet_InputSize, cDNet_InputData, IO_WAIT_TIMEOUT);
		if(r_ret != CIFX_NO_ERROR)
		{
			active__input_err = true;

			msg__err_input  = "xChannelIORead() - Error \n";
			msg__err_input += "Error Message : \n";
			msg__err_input += "  ";
			msg__err_input += Macro__Get_CIFX_Error(r_ret);
			msg__err_input += "\n";
		}
		else
		{
			mCtrl__DNet_Node.Update__ALL_IN_BYTE(cDNet_InputData, uDNet_InputSize);
		}

		if(active__in_byte_check)
		{
			CString log_msg;
			CString log_bff;

			log_msg.Format("Input_Node.All_Info ... \n");

			log_bff.Format(" * uDNet_InputSize <- %1d \n", uDNet_InputSize);
			log_msg += log_bff;					

			log_bff.Format("HEXA Info ... \n");
			log_msg += log_bff;

			for(int i=0; i<uDNet_InputSize; i++)
			{
				if((i > 0) && ((i % 10) == 0))			log_msg += "\n";		

				log_bff.Format("%02X ", 0x0ff & cDNet_InputData[i]);
				log_msg += log_bff;
			}
			log_msg += "\n";

			Write__APP_LOG(log_msg);
		}
	}

	// Error.Report ...
	if(active__output_err)
	{
		bActive__COMM_STATE = false;

		// ...
		{
			int alm_id = ALID__DNET_NODE_OUT_BYTE_UPDATE_ERROR;
			CString r_act;

			p_alarm->Check__ALARM(alm_id, r_act);
			p_alarm->Post__ALARM_With_MESSAGE(alm_id, msg__err_output);
		}
	}
	if(active__input_err)
	{
		bActive__COMM_STATE = false;

		// ...
		{
			int alm_id = ALID__DNET_NODE_IN_BYTE_UPDATE_ERROR;
			CString r_act;

			p_alarm->Check__ALARM(alm_id, r_act);
			p_alarm->Post__ALARM_With_MESSAGE(alm_id, msg__err_input);
		}
	}

	if((!active__output_err) && (!active__input_err))
	{
		bActive__COMM_STATE = true;
	}
}
void CObj__DRV_TEST
::_DNet__UPDATE_NODE_X(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	if(iDNet_BoardNumber < 0)
	{
		printf("iDNet_BoardNumber < -%1d \n", iDNet_BoardNumber);
		return;
	}

	// ...
	HANDLE h_channel_res = NULL;

	// ...
	bool active__input_err  = false;
	bool active__output_err = false;

	CString msg__err_input;
	CString msg__err_output;
	CString err_bff;

	// ...
	bool active__in_byte_check  = false;
	bool active__out_byte_check = false;

	if(dCH__DNET_INFO__IN_BYTE_CHECK_ACTIVE->Check__DATA(STR__YES) > 0)
	{
		active__in_byte_check = true;

		dCH__DNET_INFO__IN_BYTE_CHECK_ACTIVE->Set__DATA(STR__NO);
	}
	if(dCH__DNET_INFO__OUT_BYTE_CHECK_ACTIVE->Check__DATA(STR__YES) > 0)
	{
		active__out_byte_check = true;

		dCH__DNET_INFO__OUT_BYTE_CHECK_ACTIVE->Set__DATA(STR__NO);
	}

	// ...
	unsigned int count__area_output = 0;
	unsigned int count__area_input  = 0;

	// ...
	int node_size = mCtrl__DNet_Node.Get__Node_Size();
	int node_index = 0;

	for(node_index = 0; node_index < node_size; node_index++)
	{
		CInfo__DNet_Node* p_node = mCtrl__DNet_Node.Get__Node_Address(node_index);
		if(p_node != NULL)			continue;
		
		// Update.OUTPUT ...
		{
			unsigned int area_index    = (unsigned int) node_index;
			unsigned int node_offset   = (unsigned int) p_node->iOUT_OFFSET;
			unsigned int node_byte_len = (unsigned int) p_node->iOUT_SIZE;

			if(node_byte_len > 0)
			{
				area_index = count__area_output;

				long r_ret = xChannelIOWrite(h_channel_res, area_index, node_offset, node_byte_len, p_node->usDATA__OUT_BYTE, IO_WAIT_TIMEOUT);
				if(r_ret != CIFX_NO_ERROR)
				{
					if(!active__output_err)
					{
						active__output_err = true;

						msg__err_output  = "xChannelIOWrite() - Error \n";
						msg__err_output += "Error Message : \n";
						msg__err_output += "  ";
						msg__err_output += Macro__Get_CIFX_Error(r_ret);
						msg__err_output += "\n";

						err_bff.Format(" * Node.ID <- %1d \n", p_node->iNODE_ID);
						msg__err_output += err_bff;
						err_bff.Format(" * Mac.ID <- %1d \n", p_node->iMAC_ID);
						msg__err_output += err_bff;
						err_bff.Format(" * Name <- %s \n", p_node->sNAME);
						msg__err_output += err_bff;
						err_bff.Format(" * Out_Byte Size <- %1d \n", p_node->iOUT_SIZE);
						msg__err_output += err_bff;
						err_bff.Format(" * Out_Byte.Offset <- %1d \n", p_node->iOUT_OFFSET);
						msg__err_output += err_bff;
					}
				}

				if(active__out_byte_check)
				{
					CString log_msg;
					CString log_bff;

					log_msg.Format("Output_Node(%1d) ... \n", node_index);

					//
					log_bff.Format("Node_Index.%1d - Info ... \n", node_index);
					log_msg += log_bff;					
					log_bff.Format(" * Node.ID <- %1d \n", p_node->iNODE_ID);
					log_msg += log_bff;
					log_bff.Format(" * Mac.ID <- %1d \n",  p_node->iMAC_ID);
					log_msg += log_bff;
					log_bff.Format(" * Name <- %s \n", p_node->sNAME);
					log_msg += log_bff;
					log_bff.Format(" * Out_Byte Size <- %1d \n", p_node->iOUT_SIZE);
					log_msg += log_bff;
					log_bff.Format(" * Out_Byte.Offset <- %1d \n", p_node->iOUT_OFFSET);
					log_msg += log_bff;

					//
					log_msg += " * xChannelIOWrite() - Parameter ... \n";
					log_bff.Format("   * ulAreaNumber <- %1d \n", area_index);
					log_msg += log_bff;
					log_bff.Format("   * ulOffset <- %1d \n", node_offset);
					log_msg += log_bff;
					log_bff.Format("   * ulDataLen <- %1d \n", node_byte_len);
					log_msg += log_bff;

					log_bff.Format("HEXA Info ... \n");
					log_msg += log_bff;

					for(int i=0; i<node_byte_len; i++)
					{
						if((i > 0) && ((i % 10) == 0))			log_msg += "\n";		

						log_bff.Format("%02X ", 0x0ff & p_node->usDATA__OUT_BYTE[i]);
						log_msg += log_bff;
					}
					log_msg += "\n";

					Write__APP_LOG(log_msg);
				}

				count__area_output++;
			}
		}

		// Update.INPUT ...
		{
			unsigned int area_index    = (unsigned int) node_index;
			unsigned int node_offset   = (unsigned int) p_node->iIN_OFFSET;
			unsigned int node_byte_len = (unsigned int) p_node->iIN_SIZE;
			
			if(node_byte_len > 0)
			{
				area_index = count__area_input;

				long r_ret = xChannelIORead(h_channel_res, area_index, node_offset, node_byte_len, p_node->usDATA__IN_BYTE, IO_WAIT_TIMEOUT);
				if(r_ret != CIFX_NO_ERROR)
				{
					if(!active__input_err)
					{
						active__input_err = true;

						msg__err_input  = "xChannelIORead() - Error \n";
						msg__err_input += "Error Message : \n";
						msg__err_input += "  ";
						msg__err_input += Macro__Get_CIFX_Error(r_ret);
						msg__err_input += "\n";
						
						err_bff.Format(" * Node.ID <- %1d \n", p_node->iNODE_ID);
						msg__err_input += err_bff;
						err_bff.Format(" * Mac.ID <- %1d \n", p_node->iMAC_ID);
						msg__err_input += err_bff;
						err_bff.Format(" * Name <- %s \n", p_node->sNAME);
						msg__err_input += err_bff;
						err_bff.Format(" * In_Byte Size <- %1d \n", p_node->iIN_SIZE);
						msg__err_input += err_bff;
						err_bff.Format(" * In_Byte.Offset <- %1d \n", p_node->iIN_OFFSET);
						msg__err_input += err_bff;
					}
				}
				else
				{
					if(active__in_byte_check)
					{
						CString log_msg;
						CString log_bff;

						log_msg.Format("Input_Node(%1d) ... \n", node_index);

						//
						log_bff.Format("Node_Index.%1d - Info ... \n", node_index);
						log_msg += log_bff;					
						log_bff.Format(" * Node.ID <- %1d \n", p_node->iNODE_ID);
						log_msg += log_bff;
						log_bff.Format(" * Mac.ID <- %1d \n",  p_node->iMAC_ID);
						log_msg += log_bff;
						log_bff.Format(" * Name <- %s \n", p_node->sNAME);
						log_msg += log_bff;
						log_bff.Format(" * In_Byte Size <- %1d \n", p_node->iIN_SIZE);
						log_msg += log_bff;
						log_bff.Format(" * In_Byte.Offset <- %1d \n", p_node->iIN_OFFSET);
						log_msg += log_bff;

						//
						log_msg += " * xChannelIORead() - Parameter ... \n";
						log_bff.Format("   * ulAreaNumber <- %1d \n", area_index);
						log_msg += log_bff;
						log_bff.Format("   * ulOffset <- %1d \n", node_offset);
						log_msg += log_bff;
						log_bff.Format("   * ulDataLen <- %1d \n", node_byte_len);
						log_msg += log_bff;

						log_bff.Format("HEXA Info ... \n");
						log_msg += log_bff;

						for(int i=0; i<node_byte_len; i++)
						{
							if((i > 0) && ((i % 10) == 0))			log_msg += "\n";		

							log_bff.Format("%02X ", 0x0ff & p_node->usDATA__IN_BYTE[i]);
							log_msg += log_bff;
						}
						log_msg += "\n";

						Write__APP_LOG(log_msg);
					}
				}

				count__area_input++;
			}
		}
	}

	// Error.Report ...
	if(active__output_err)
	{
		bActive__COMM_STATE = false;

		// ...
		{
			int alm_id = ALID__DNET_NODE_OUT_BYTE_UPDATE_ERROR;
			CString r_act;

			p_alarm->Check__ALARM(alm_id, r_act);
			p_alarm->Post__ALARM_With_MESSAGE(alm_id, msg__err_output);
		}
	}
	if(active__input_err)
	{
		bActive__COMM_STATE = false;

		// ...
		{
			int alm_id = ALID__DNET_NODE_IN_BYTE_UPDATE_ERROR;
			CString r_act;

			p_alarm->Check__ALARM(alm_id, r_act);
			p_alarm->Post__ALARM_With_MESSAGE(alm_id, msg__err_input);
		}
	}

	if((!active__output_err) && (!active__input_err))
	{
		bActive__COMM_STATE = true;
	}
}

