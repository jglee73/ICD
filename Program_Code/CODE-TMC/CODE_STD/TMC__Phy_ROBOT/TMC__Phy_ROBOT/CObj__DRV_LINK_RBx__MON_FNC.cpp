#include "StdAfx.h"
#include "CObj__DRV_LINK_RBx.h"
#include "CObj__DRV_LINK_RBx__DEF.h"
#include "CObj__DRV_LINK_RBx__ALID.h"


//------------------------------------------------------------------------------------
void CObj__DRV_LINK_RBx
::Mon__IO_MONITOR(CII_OBJECT__VARIABLE* p_variable,
				  CII_OBJECT__ALARM* p_alarm)
{
	SCX__ASYNC_TIMER_CTRL x_timer_ctrl;
	double sec_wait = 0.0;

	while(1)
	{
		p_variable->Wait__SINGLE_OBJECT(0.01);

		if(iActive__SIM_MODE > 0)
		{
			if(dCH__SIM_CFG__REAL_TEST->Check__DATA(STR__YES) > 0)			iCFG__SIM_MODE = -1;
			else															iCFG__SIM_MODE =  1;
		}
		else
		{
			iCFG__SIM_MODE = -1;
		}

		if(iCFG__SIM_MODE > 0)
		{
			if(sEXT_CH__ROBOT_CMMD_STS->Check__DATA(STR__RUN) > 0)
			{
				sEXT_CH__ROBOT_CMMD_STS->Set__DATA(STR__WAIT);

				CString cur_mode = sCH__CUR_CTRL_MODE->Get__STRING();
				CString para_stn = dCH__PARA_STN_NAME->Get__STRING();

				if(cur_mode.CompareNoCase(sMODE__ROTATE) == 0)
				{
					sec_wait = aCH__SIM_CFG__ROTATE_TIME->Get__VALUE();
				}
				else if(cur_mode.CompareNoCase(sMODE__PICK) == 0)
				{
						 if(Fnc__Check_LPx(para_stn) > 0)			sec_wait = aCH__SIM_CFG__LPx_PICK_TIME->Get__VALUE();
					else if(Fnc__Check_ALx(para_stn) > 0)			sec_wait = aCH__SIM_CFG__ALx_PICK_TIME->Get__VALUE();
					else if(Fnc__Check_LBx(para_stn) > 0)			sec_wait = aCH__SIM_CFG__LBx_PICK_TIME->Get__VALUE();
					else if(Fnc__Check_BUFFx(para_stn) > 0)			sec_wait = aCH__SIM_CFG__BUFFx_PICK_TIME->Get__VALUE();
				}
				else if(cur_mode.CompareNoCase(sMODE__PLACE) == 0)
				{
						 if(Fnc__Check_LPx(para_stn) > 0)			sec_wait = aCH__SIM_CFG__LPx_PLACE_TIME->Get__VALUE();
					else if(Fnc__Check_ALx(para_stn) > 0)			sec_wait = aCH__SIM_CFG__ALx_PLACE_TIME->Get__VALUE();
					else if(Fnc__Check_LBx(para_stn) > 0)			sec_wait = aCH__SIM_CFG__LBx_PLACE_TIME->Get__VALUE();
					else if(Fnc__Check_BUFFx(para_stn) > 0)			sec_wait = aCH__SIM_CFG__BUFFx_PLACE_TIME->Get__VALUE();
				}

				x_timer_ctrl->START__COUNT_UP(sec_wait + 100);

				// ...
				{
					CString log_msg;
					CString log_bff;

					log_msg.Format(" * Mon__IO_MONITOR() - Started ... \n");

					log_bff.Format(" * cur_mode <- [%s] \n", cur_mode);
					log_msg += log_bff;
					
					log_bff.Format(" * para_stn <- [%s] \n", para_stn);
					log_msg += log_bff;

					log_bff.Format(" * sec_wait <- [%.3f] \n", sec_wait); 
					log_msg += log_bff;

					log_bff.Format(" * cur_sec  <- [%.3f] \n", x_timer_ctrl->Get__CURRENT_TIME());
					log_msg += log_bff;

					Fnc__APP_LOG(log_msg);
				}
			}

			if(sEXT_CH__ROBOT_CMMD_STS->Check__DATA(STR__WAIT) > 0)
			{
				if(x_timer_ctrl->Get__CURRENT_TIME() >= sec_wait)
				{
					x_timer_ctrl->STOP();

					sEXT_CH__ROBOT_CMMD_STS->Set__DATA(STR__IDLE);
				}
			}
		}

		// ...
	}	
}


// ...
int  CObj__DRV_LINK_RBx::Fnc__Check_LPx(const CString& para_stn)
{
	CString str_stn;
	
	int i_limit = 5;
	int i;

	for(i=0; i<i_limit; i++)
	{
		str_stn.Format("LP%1d", i+1);

		if(para_stn.CompareNoCase(str_stn) == 0)		
			return (1 + i);
	}
	return -1;
}
int  CObj__DRV_LINK_RBx::Fnc__Check_LBx(const CString& para_stn)
{
	if(para_stn.CompareNoCase("LBA") == 0)		return 1;
	if(para_stn.CompareNoCase("LBB") == 0)		return 2;

	return -1;
}
int  CObj__DRV_LINK_RBx::Fnc__Check_ALx(const CString& para_stn)
{
	if(para_stn.CompareNoCase("AL1") == 0)		return 1;

	return -1;
}
int  CObj__DRV_LINK_RBx::Fnc__Check_BUFFx(const CString& para_stn)
{
	if(para_stn.CompareNoCase("BUFF1") == 0)		return 1;
	if(para_stn.CompareNoCase("BUFF2") == 0)		return 2;

	return -1;
}
