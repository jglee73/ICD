#include "StdAfx.h"
#include "CObj__DNET_STD.h"
#include "CObj__DNET_STD__DEF.h"

#include "cifuser.h"
#include "rcs_user.h"
#include "dnm_user.h"


//------------------------------------------------------------------------------------
void CObj__DNET_STD
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
		if(dCH__CFG_SLAVE_INFO_CHECK->Check__DATA(STR__YES) > 0)
		{
			if(_Check__SLAVE_ERROR() > 0)
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

	if(bActive__DNET_INIT)			diCH__COMM_STS->Set__DATA(STR__ONLINE);
	else							diCH__COMM_STS->Set__DATA(STR__OFFLINE);


	while(1)
	{
		p_variable->Wait__SINGLE_OBJECT(0.05);

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

		if(loop_count == 1)
		{
			if(diCH__COMM_STS->Check__DATA(STR__OFFLINE) > 0)
			{
				int alm_id = ALID__OFFLINE;
				CString alm_msg;
				CString	r_act;

				alm_msg = "";

				p_alarm->Check__ALARM(alm_id, r_act);
				p_alarm->Post__ALARM_With_MESSAGE(alm_id, alm_msg);
			}

			// ...
			unsigned short err_code;
			CString err_msg;

			if(mDNet_Mng.Check__LAST_ERROR_CODE(err_code, err_msg))
			{
				int alm_id = ALID__LAST_ERROR_CODE;
				CString r_act;

				p_alarm->Check__ALARM(alm_id, r_act);
				p_alarm->Post__ALARM_With_MESSAGE(alm_id, err_msg);
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
				_DNet__UPDATE_INFO();
			}

			LeaveCriticalSection(&mLOCK_DNET);
		}	
		else
		{
			diCH__COMM_STS->Set__DATA(STR__OFFLINE);

			// ...
			{
				int alm_id = ALID__DNET_INIT_ERROR;
				CString r_act;

				p_alarm->Check__ALARM(alm_id, r_act);
				p_alarm->Post__ALARM(alm_id);
			}
		}
	}

	// ...
}

void CObj__DNET_STD::_DNet__UPDATE_INFO()
{
	if(iDNet_BoardNumber < 0)
	{
		printf("iDNet_BoardNumber < -%1d \n", iDNet_BoardNumber);
		return;
	}

	// ...
	CString ch_data;

	int dev_id = iDNet_BoardNumber;
	int i;

	// Output Download ...
	{
		mCtrl__DNet_Node.Update__ALL_OUT_BYTE(abOutputData,usOutputOffset);

		if(dCH__DNET_INFO__OUT_BYTE_CHECK_ACTIVE->Check__DATA(STR__YES) > 0)
		{
			for(i=0; i<usOutputOffset; i++)
			{
				if(i < CFG__DNET_OUT_BYTE_LIMIT)
				{
					ch_data.Format("%02X", abOutputData[i]);
					sCH__DNET_INFO__OUT_BYTE_DATA_X[i]->Set__DATA(ch_data);
				}
			}
		}
	}

	// ...
	COMSTATE tComState;
	short s_ret;

	/*
	s_ret = DevExchangeIO(dev_id,
						0,
						usOutputOffset,
						&abOutputData,
						0,
						usInputOffset,
						&abInputData,
						500L);
	*/
	s_ret = DevExchangeIOErr(dev_id,
							iDNet_Board_Out_Offset,
							usOutputOffset,
							&abOutputData,
							iDNet_Board_In_Offset,
							usInputOffset,
							&abInputData,
							&tComState,
							100L);

	// slave state ...
	if(s_ret == DRV_NO_ERROR)
	{
		if(dCH__DNET_INFO__COMM_STATE_CHECK_ACTIVE->Check__DATA(STR__YES) > 0)
		{
			CString log_msg;
			CString log_bff;

			bool active__state_check = false;

			switch(tComState.usMode) 
			{
				case STATE_MODE_3:
					active__state_check = true;

					log_bff  = "STATE_MODE_3: \n";
					log_msg += log_bff;
					
					log_bff = "  Check state field usStateFlag signals entrys \n";
					log_msg += log_bff;
					break;
				
				case STATE_MODE_4:
					active__state_check = true;

					log_bff  = "STATE_MODE_4: \n";
					log_msg += log_bff;
					
					log_bff  = "  Check state field usStateFlag signals new entrys \n";
					log_msg += log_bff;
					break;

				default:
					log_bff  = "State mode unknown or not implemented \n";
					log_msg += log_bff;

					log_bff.Format("  usStateFlag <- %d \n", tComState.usStateFlag);
					log_msg += log_bff;
					break;
			} 

			if(active__state_check)
			{
				if(tComState.usStateFlag != 0) 
				{
					int k_limit = sizeof(tComState.abState);

					log_msg += "\n";
					log_bff.Format("Comm.abState (%1d) ... \n", k_limit);
					log_msg += log_bff;
			
					for(int k=0; k<k_limit; k++)
					{
						log_bff.Format("%02X ", 0x0ff & tComState.abState[k]);
						log_msg += log_bff;
	
						if((k % 10) == 0)		log_msg += "\n  ";
					}
				}
			}

			printf(log_msg);
		}
	}

	// Input Upload ...
	if(s_ret == DRV_NO_ERROR)
	{
		mCtrl__DNet_Node.Update__ALL_IN_BYTE(abInputData,usInputOffset);

		if(dCH__DNET_INFO__IN_BYTE_CHECK_ACTIVE->Check__DATA(STR__YES) > 0)
		{
			for(i=0; i<usInputOffset; i++)
			{
				if(i < CFG__DNET_IN_BYTE_LIMIT)
				{
					ch_data.Format("%02X", abInputData[i]);
					sCH__DNET_INFO__IN_BYTE_DATA_X[i]->Set__DATA(ch_data);
				}
			}
		}
	}
	else
	{
		diCH__COMM_STS->Set__DATA(STR__ONLINE);

		printf("DevExchangeIO( %1d, ... ) : Error (%1d) \n", dev_id, s_ret);
		printf(" * Error Message : \"%s\" \n", mDNet_Mng.Get__ERR_CODE(s_ret));

		Sleep(1000);
	}
}
