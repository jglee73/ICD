#include "StdAfx.h"
#include "CObj__LPx_SYS.h"
#include "CObj__LPx_SYS__DEF.h"
#include "CObj__LPx_SYS__ALID.h"


//----------------------------------------------------------------------------------------------------
// INIT ----- 
int  CObj__LPx_SYS
::Call__INIT(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	int flag = 1;

	if(pLPx__OBJ_CTRL->Call__OBJECT(CMMD__INIT) < 0)
	{
		flag = -1;
	}

	if(dCH__CFG_INIT_SLOT_STATE_UNKNOWN->Check__DATA(STR__YES) > 0)
	{
		for(int i=0; i<CFG_LPx__SLOT_MAX; i++)
		{
			dCH__MON_SLOT_STATUS[i]->Set__DATA(STR__UNKNOWN);
		}
	}

	if(dCH__CFG_RF_EXIST_FLAG->Check__DATA(STR__YES) > 0)
	{
		if(bActive__RFx_OBJ)
		{
			if(pRFx__OBJ_CTRL->Call__OBJECT(CMMD__INIT) < 0)
			{
				flag = -1;
			}
		}
	}
	return flag;
}

int  CObj__LPx_SYS
::Call__HOME(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	return pLPx__OBJ_CTRL->Call__OBJECT(CMMD__HOME);
}

int  CObj__LPx_SYS
::Call__ALARM_RESET(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	return pLPx__OBJ_CTRL->Call__OBJECT(CMMD__ALARM_RESET);
}

// DOOR OPEN -----	
int  CObj__LPx_SYS
::Call__DOOR_OPEN(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	if(dCH__MON_DOOR_STS->Check__DATA(STR__OPEN) > 0) 
	{
		return 1;
	}
	
	return pLPx__OBJ_CTRL->Call__OBJECT(CMMD__DOOR_OPEN);
}

// CHECK DOOR OPEN -----
int  CObj__LPx_SYS
::Call__CHECK_DOOR_OPEN(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
LOOP_CHECK:

	int loop_count = 10;

	while(loop_count > 0)
	{
		if(dCH__MON_DOOR_STS->Check__DATA(STR__OPEN) > 0) 
		{
			return 1;
		}

		Sleep(100);

		loop_count--;
	}

	// ...
	{
		int alarm_id = ALID__DOOR__NOT_OPEN;
		CString r_act;

		p_alarm->Popup__ALARM(alarm_id,r_act);

		if(r_act.CompareNoCase(ACT__RETRY) == 0)
		{
			goto LOOP_CHECK;
		}
	}
	return -1;
}

// LOAD ...
int  CObj__LPx_SYS
::Call__LOAD(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	int r_flag = Alarm_Check__ARM_NOT_RETRACTED(p_alarm);
	if(r_flag < 0)		return r_flag;

	r_flag = Fnc__LOAD(p_variable,p_alarm);

	if(r_flag > 0)
	{
		if(dCH__CFG_LOAD_MAP_MODE->Check__DATA(STR__ENABLE) > 0)
		{
			r_flag = Fnc__DOOR_OPEN(p_variable,p_alarm);
			if(r_flag < 0)		return -11;

			r_flag = Fnc__MAP(p_variable, p_alarm);
			if(r_flag < 0)		return -21;

			if(dCH__CFG_LOAD_DOOR_OPEN->Check__DATA(STR__NO) > 0)
			{
				r_flag = Fnc__DOOR_CLOSE(p_variable,p_alarm);
				if(r_flag < 0)		return -31;
			}
		}
		else
		{
			if(dCH__CFG_LOAD_ACT_AUTO_MAP_MODE->Check__DATA(STR__ENABLE) > 0)
			{
				Update__MAP_INFO(p_variable,p_alarm);
			}
		}
	}
	return r_flag;
}
int  CObj__LPx_SYS
::Fnc__LOAD(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	int r_flag;

	if(dCH__CFG_BEFORE_LOAD_CST_CHECK->Check__DATA(STR__TRUE) > 0)
	{
		r_flag = Is__LOAD_CONDITION(p_variable, p_alarm);
		if(r_flag < 0)		return -11;
	}

	if(dCH__CFG_LOAD_DOOR_OPEN->Check__DATA(STR__NO) > 0)
	{
		r_flag = Seq__LOAD(p_variable,p_alarm);
		if(r_flag < 0)		return -21;
	}
	else
	{
		r_flag = Seq__LOAD(p_variable,p_alarm);
		if(r_flag < 0)		return -31;

		r_flag = Fnc__DOOR_OPEN(p_variable,p_alarm);
		if(r_flag < 0)		return -32;
	}

	// ...
	{
		SCX__TIMER_CTRL x_timer;
		x_timer->REGISTER__ABORT_OBJECT(sObject_Name);

		double cfg__delay_sec = aCH__CFG_LOAD_COMPT_DELAY_TIME->Get__VALUE();

		if(x_timer->WAIT(cfg__delay_sec) < 0)		return -41;
	}

	return 1;	
}
int  CObj__LPx_SYS
::Seq__LOAD(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	int r_flag;

	if(dCH__CFG_MACRO_LOAD_USE->Check__DATA(STR__YES) > 0)
	{
		// CLAMP : ACTIVE CHECK ...
		{
			sCH__ACTIVE_CHECK_CLAMP_DO->Set__DATA(STR__YES);
			sCH__ACTIVE_CHECK_UNCLAMP_DO->Set__DATA(STR__YES);

			sCH__ACTIVE_CHECK_CLAMP_DI->Set__DATA(STR__YES);
			sCH__ACTIVE_CHECK_UNCLAMP_DI->Set__DATA(STR__YES);
		}
		// SHUTTLE_IN : ACTIVE CHECK ...
		{
			sCH__ACTIVE_CHECK_SHUTTLE_IN_DO->Set__DATA(STR__YES);
			sCH__ACTIVE_CHECK_SHUTTLE_OUT_DO->Set__DATA(STR__YES);

			sCH__ACTIVE_CHECK_SHUTTLE_IN_DI->Set__DATA(STR__YES);
			sCH__ACTIVE_CHECK_SHUTTLE_OUT_DI->Set__DATA(STR__YES);
		}

		r_flag = pLPx__OBJ_CTRL->Call__OBJECT(CMMD__LOAD);
		if(r_flag < 0)
		{
			sCH__ACTIVE_CHECK_CLAMP_DI->Set__DATA(STR__ERROR);
			sCH__ACTIVE_CHECK_UNCLAMP_DI->Set__DATA(STR__ERROR);

			sCH__ACTIVE_CHECK_SHUTTLE_IN_DI->Set__DATA(STR__ERROR);
			sCH__ACTIVE_CHECK_SHUTTLE_OUT_DI->Set__DATA(STR__ERROR);

			return -11;
		}
		else
		{
			sCH__ACTIVE_CHECK_CLAMP_DI->Set__DATA(STR__OK);
			sCH__ACTIVE_CHECK_UNCLAMP_DI->Set__DATA(STR__OK);

			sCH__ACTIVE_CHECK_SHUTTLE_IN_DI->Set__DATA(STR__OK);
			sCH__ACTIVE_CHECK_SHUTTLE_OUT_DI->Set__DATA(STR__OK);
		}
	}
	else
	{
		r_flag = Fnc__CLAMP(p_variable,p_alarm);
		if(r_flag < 0)		return -11;

		r_flag = Fnc__SHUTTLE_IN(p_variable,p_alarm);
		if(r_flag < 0)		return -21;
	}

	Update__LOAD_POS_STATE();
	return 1;	
}

// UNLOAD ...
int  CObj__LPx_SYS
::Call__UNLOAD(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	/*
	int r_flag = Alarm_Check__ARM_NOT_RETRACTED(p_alarm);
	if(r_flag < 0)		return r_flag;
	*/

	return Fnc__UNLOAD(p_variable,p_alarm);
}
int  CObj__LPx_SYS
::Fnc__UNLOAD(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	// ACTIVE.CHECK ...
	{
		sCH__ACTIVE_CHECK_FOUP_PRESENT_DI->Set__DATA(STR__YES);
		sCH__ACTIVE_CHECK_FOUP_PLACED_DI->Set__DATA(STR__YES);
	}

	int r_flag = Fnc__RLSUNLOAD(p_variable, p_alarm);
	if(r_flag < 0)
	{
		sCH__ACTIVE_CHECK_FOUP_PRESENT_DI->Set__DATA(STR__ERROR);
		sCH__ACTIVE_CHECK_FOUP_PLACED_DI->Set__DATA(STR__ERROR);

		return r_flag;
	}

	while(1)
	{
		Sleep(20);

		if(p_variable->Check__CTRL_ABORT() > 0)	
		{
			return -11;
		}

		// Manual Mode
		if(dCH__LINK_PIO_ACTIVE_FA_AUTO->Check__DATA(STR__ON) < 0)
		{
			if(iActive__SIM_MODE > 0) 
			{
				CString cfg_option = dCH__CFG_UNLD_CLAMP_OPT->Get__STRING();

				if(cfg_option.CompareNoCase(STR__BUTTON) == 0)
				{
					CString box_msg = "Click ""UNLOAD"" button if you want to unclamp !";

					int r_box = ::MessageBox(NULL, box_msg, sObject_Name, MB_TOPMOST | MB_SETFOREGROUND | MB_OK);
					if(r_flag == IDOK)
					{
						pLPx__OBJ_CTRL->Call__OBJECT(CMMD__UNCLAMP);
						
						Sleep(500);
					}
				}

				if(dCH__MON_CLAMP_STS->Check__DATA(STR__CLAMP) < 0)
				{
					CString box_msg = "Click ""OK"" button if you want to remove foup !";

					int r_box = ::MessageBox(NULL, box_msg, sObject_Name, MB_TOPMOST | MB_SETFOREGROUND | MB_OK);
					if(r_box == IDOK)
					{
						pLPx__OBJ_CTRL->Call__OBJECT("SIM.FOUP_NONE");

						Sleep(500);
					}
				}
			}

			if(dCH__MON_FOUP_STS->Check__DATA(STR__NONE) > 0) 
			{
				break;
			}
		}
		else   // Auto Mode
		{
			bool active__lp_check = true;

			if(dCH__MON_FOUP_STS->Check__DATA(STR__NONE) < 0) 
			{
				active__lp_check = false;
			}

			if(bActive__LPx_OHT)
			{
				if(dEXT_CH__REPORT_PIO_E84_RUN_STATE->Check__DATA(STR__DONE) < 0)		
					active__lp_check = false;
			}

			if(active__lp_check)
			{
				break;
			}
		}
	} 

	// ACTIVE.OK ...
	{
		sCH__ACTIVE_CHECK_FOUP_PRESENT_DI->Set__DATA(STR__OK);
		sCH__ACTIVE_CHECK_FOUP_PLACED_DI->Set__DATA(STR__OK);
	}

	// Slot Init ...
	for(int i=0;i<CFG_LPx__SLOT_MAX;i++)
	{
		dCH__MON_SLOT_STATUS[i]->Set__DATA(STR__UNKNOWN);
	}

	Update__LOAD_POS_STATE();
	return 1;
}

// PREPLOAD ...
int  CObj__LPx_SYS
::Call__PREPLOAD(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	for(int i=0;i<CFG_LPx__SLOT_MAX;i++)
	{
		dCH__MON_SLOT_STATUS[i]->Set__DATA(STR__UNKNOWN);		
	}

	int r_flag = Fnc__PREPLOAD(p_variable,p_alarm);
	if(r_flag < 0)		return r_flag;

	if(dCH__CFG_PREPLOAD_CID_MODE->Check__DATA(STR__ENABLE) > 0)
	{
		Fnc__CID_READ(p_variable, p_alarm);
	}
	return 1;
}

int  CObj__LPx_SYS
::Fnc__PREPLOAD(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	// ACTIVE.CHECK ...
	{
		sCH__ACTIVE_CHECK_FOUP_PRESENT_DI->Set__DATA(STR__YES);
		sCH__ACTIVE_CHECK_FOUP_PLACED_DI->Set__DATA(STR__YES);
	}

	int r_flag = _Fnc__PREPLOAD(p_variable, p_alarm);
	if(r_flag < 0)
	{
		sCH__ACTIVE_CHECK_FOUP_PRESENT_DI->Set__DATA(STR__ERROR);
		sCH__ACTIVE_CHECK_FOUP_PLACED_DI->Set__DATA(STR__ERROR);

		return r_flag;
	}
	else
	{
		sCH__ACTIVE_CHECK_FOUP_PRESENT_DI->Set__DATA(STR__OK);
		sCH__ACTIVE_CHECK_FOUP_PLACED_DI->Set__DATA(STR__OK);
	}

	return 1;
}
int  CObj__LPx_SYS
::_Fnc__PREPLOAD(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	if(iActive__SIM_MODE > 0) 
	{
		if(dCH__LINK_PIO_ACTIVE_FA_AUTO->Check__DATA(STR__ON) < 0)
		{
			pLPx__OBJ_CTRL->Call__OBJECT("SIM.FOUP_EXIST");

			if(dCH__MON_CLAMP_STS->Check__DATA(STR__CLAMP) < 0)
			{
				if(dCH__CFG_DYNAMIC_USE->Check__DATA(STR__NO) > 0)
				{
					CString box_msg = "Click \"LOAD\" button if you want to clamp";

					int r_box = ::MessageBox(NULL, box_msg, sObject_Name, MB_TOPMOST | MB_SETFOREGROUND | MB_OK);
					if(r_box == IDOK)
					{
						pLPx__OBJ_CTRL->Call__OBJECT(CMMD__CLAMP);

						Sleep(500);
					}
				}
				else
				{
					pLPx__OBJ_CTRL->Call__OBJECT(CMMD__CLAMP);
				}
			}
		}
	}

	do 
	{
		Sleep(200);

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -11;
		}

		// Manual Mode
		if(dCH__LINK_PIO_ACTIVE_FA_AUTO->Check__DATA(STR__ON) < 0)
		{
			if(dCH__MON_FOUP_STS->Check__DATA(STR__EXIST) > 0)
			{
				if(dCH__CFG_DYNAMIC_USE->Check__DATA(STR__YES) > 0)
				{
					return Fnc__CLAMP(p_variable, p_alarm);
				}

				if(dCH__MON_CLAMP_STS->Check__DATA(STR__CLAMP) > 0)
				{
					return 1;
				}
			}
		}
		// Auto Mode
		else
		{
			if(dCH__MON_FOUP_STS->Check__DATA(STR__EXIST) > 0) 
			{
				if(bActive__LPx_OHT)
				{
					if(dEXT_CH__REPORT_PIO_E84_RUN_STATE->Check__DATA(STR__DONE) > 0)			return 1;
				}
				else
				{
					return 1;
				}
			}
		}
	} 
	while(1);

	return 1;
}

// ...
int  CObj__LPx_SYS
::Call__RLSUNLOAD(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	int r_flag = Alarm_Check__ARM_NOT_RETRACTED(p_alarm);
	if(r_flag < 0)		return r_flag;

	return Fnc__RLSUNLOAD(p_variable,p_alarm);
}
int  CObj__LPx_SYS
::Fnc__RLSUNLOAD(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	if(dCH__CFG_MACRO_UNLOAD_USE->Check__DATA(STR__YES) > 0)
	{
		// DOOR : ACTIVE CHECK ...
		{
			sCH__ACTIVE_CHECK_DOOR_UP_DO->Set__DATA(STR__YES);
			sCH__ACTIVE_CHECK_DOOR_DN_DO->Set__DATA(STR__YES);
			sCH__ACTIVE_CHECK_DOOR_RET_DO->Set__DATA(STR__YES);
			sCH__ACTIVE_CHECK_DOOR_EXT_DO->Set__DATA(STR__YES);
			sCH__ACTIVE_CHECK_LATCH_OPEN_DO->Set__DATA(STR__YES);
			sCH__ACTIVE_CHECK_LATCH_CLOSE_DO->Set__DATA(STR__YES);

			sCH__ACTIVE_CHECK_DOOR_UP_DI->Set__DATA(STR__YES);
			sCH__ACTIVE_CHECK_DOOR_DN_DI->Set__DATA(STR__YES);
			sCH__ACTIVE_CHECK_DOOR_RET_DI->Set__DATA(STR__YES);
			sCH__ACTIVE_CHECK_DOOR_EXT_DI->Set__DATA(STR__YES);
			sCH__ACTIVE_CHECK_LATCH_OPEN_DI->Set__DATA(STR__YES);
			sCH__ACTIVE_CHECK_LATCH_CLOSE_DI->Set__DATA(STR__YES);
		}
		// SHUTTLE_IN : ACTIVE CHECK ...
		{
			sCH__ACTIVE_CHECK_SHUTTLE_IN_DO->Set__DATA(STR__YES);
			sCH__ACTIVE_CHECK_SHUTTLE_OUT_DO->Set__DATA(STR__YES);

			sCH__ACTIVE_CHECK_SHUTTLE_IN_DI->Set__DATA(STR__YES);
			sCH__ACTIVE_CHECK_SHUTTLE_OUT_DI->Set__DATA(STR__YES);
		}
		// CLAMP : ACTIVE CHECK ...
		{
			sCH__ACTIVE_CHECK_CLAMP_DO->Set__DATA(STR__YES);
			sCH__ACTIVE_CHECK_UNCLAMP_DO->Set__DATA(STR__YES);

			sCH__ACTIVE_CHECK_CLAMP_DI->Set__DATA(STR__YES);
			sCH__ACTIVE_CHECK_UNCLAMP_DI->Set__DATA(STR__YES);
		}

		int r_flag = pLPx__OBJ_CTRL->Call__OBJECT(CMMD__UNLOAD);
		if(r_flag < 0)
		{
			sCH__ACTIVE_CHECK_DOOR_UP_DI->Set__DATA(STR__ERROR);
			sCH__ACTIVE_CHECK_DOOR_DN_DI->Set__DATA(STR__ERROR);
			sCH__ACTIVE_CHECK_DOOR_RET_DI->Set__DATA(STR__ERROR);
			sCH__ACTIVE_CHECK_DOOR_EXT_DI->Set__DATA(STR__ERROR);
			sCH__ACTIVE_CHECK_LATCH_OPEN_DI->Set__DATA(STR__ERROR);
			sCH__ACTIVE_CHECK_LATCH_CLOSE_DI->Set__DATA(STR__ERROR);

			sCH__ACTIVE_CHECK_SHUTTLE_IN_DI->Set__DATA(STR__ERROR);
			sCH__ACTIVE_CHECK_SHUTTLE_OUT_DI->Set__DATA(STR__ERROR);

			return -11;
		}
		else
		{
			sCH__ACTIVE_CHECK_DOOR_UP_DI->Set__DATA(STR__OK);
			sCH__ACTIVE_CHECK_DOOR_DN_DI->Set__DATA(STR__OK);
			sCH__ACTIVE_CHECK_DOOR_RET_DI->Set__DATA(STR__OK);
			sCH__ACTIVE_CHECK_DOOR_EXT_DI->Set__DATA(STR__OK);
			sCH__ACTIVE_CHECK_LATCH_OPEN_DI->Set__DATA(STR__OK);
			sCH__ACTIVE_CHECK_LATCH_CLOSE_DI->Set__DATA(STR__OK);

			sCH__ACTIVE_CHECK_SHUTTLE_IN_DI->Set__DATA(STR__OK);
			sCH__ACTIVE_CHECK_SHUTTLE_OUT_DI->Set__DATA(STR__OK);
		}

		if(dCH__CFG_UNLD_CLAMP_OPT->Check__DATA(STR__AUTO) > 0)	
		{
			r_flag = Fnc__UNCLAMP(p_variable,p_alarm);
			if(r_flag < 0)
			{
				sCH__ACTIVE_CHECK_CLAMP_DI->Set__DATA(STR__ERROR);
				sCH__ACTIVE_CHECK_UNCLAMP_DI->Set__DATA(STR__ERROR);

				return -21;
			}
			else
			{
				sCH__ACTIVE_CHECK_CLAMP_DI->Set__DATA(STR__OK);
				sCH__ACTIVE_CHECK_UNCLAMP_DI->Set__DATA(STR__OK);
			}
		}
	}
	else
	{
		int r_flag = Fnc__DOOR_CLOSE(p_variable,p_alarm);
		if(r_flag < 0)				return -11;

		r_flag = Fnc__SHUTTLE_OUT(p_variable,p_alarm);
		if(r_flag < 0)				return -21;

		if(dCH__CFG_UNLD_CLAMP_OPT->Check__DATA(STR__AUTO) > 0)	
		{
			r_flag = Fnc__UNCLAMP(p_variable,p_alarm);
			if(r_flag < 0)			return -31;
		}
	}

	return 1;
}

// ...
int  CObj__LPx_SYS
::Call__CYCLE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	CString str_log;

	double cfg_count = aCH__PARA_CYCLE_COUNT->Get__VALUE();
	aCH__CUR_CYCLE_COUNT->Set__VALUE(cfg_count);

	while(1)
	{
		Sleep(10);

		if(p_variable->Check__CTRL_ABORT() > 0)				return -1;

		str_log = "CYCLE - LOAD";
		sCH__OBJ_MSG->Set__DATA(str_log);

		if(Fnc__LOAD(p_variable, p_alarm) < 0)				return -11;

		str_log = "CYCLE - UNLOAD";
		sCH__OBJ_MSG->Set__DATA(str_log);

		if(Fnc__RLSUNLOAD(p_variable, p_alarm) < 0)			return -12;

		cfg_count--;
		aCH__CUR_CYCLE_COUNT->Set__VALUE(cfg_count);
		if(cfg_count < 1)		break;
	}

	return 1;
}

// CID_READ -----
int  CObj__LPx_SYS
::Call__CID_READ(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	return Fnc__CID_READ(p_variable, p_alarm);
}
int  CObj__LPx_SYS
::Fnc__CID_READ(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	if(dCH__CFG_RF_EXIST_FLAG->Check__DATA(STR__YES) > 0)
	{
		if(bActive__RFx_OBJ)
		{
			int r_flag = pRFx__OBJ_CTRL->Call__OBJECT(CMMD__CID_READ);

			// ...
			{
				CString ch_data = sEXT_CH__RFx__RSP_CID_DATA->Get__STRING();
				sCH__FOUP_CID_STRING->Set__DATA(ch_data);
			}
			return r_flag;
		}
	}

	sCH__FOUP_CID_STRING->Set__DATA("");
	return 1;
}
int  CObj__LPx_SYS
::Call__PAGE_READ(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	if(dCH__CFG_RF_EXIST_FLAG->Check__DATA(STR__YES) > 0)
	{
		if(bActive__RFx_OBJ)
		{
			CString ch_data = aCH__PARA_PAGE_ID->Get__STRING();
			aEXT_CH__RFx__PARA_PAGE_ID->Set__DATA(ch_data);

			int r_flag = pRFx__OBJ_CTRL->Call__OBJECT(CMMD__PAGE_READ);

			// ...
			{
				ch_data = sEXT_CH__RFx__RSP_PAGE_DATA->Get__STRING();
				sCH__FOUP_PAGE_STRING->Set__DATA(ch_data);
			}
			return r_flag;
		}
	}
	return 1;
}

int  CObj__LPx_SYS
::Call__CID_WRITE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	if(dCH__CFG_RF_EXIST_FLAG->Check__DATA(STR__YES) > 0)
	{
		if(bActive__RFx_OBJ)
		{
			CString ch_data = sCH__PARA_PAGE_CID->Get__STRING();
			sEXT_CH__RFx__PARA_CID_DATA->Set__DATA(ch_data);

			return pRFx__OBJ_CTRL->Call__OBJECT(CMMD__CID_WRITE);
		}
	}
	return 1;
}
int  CObj__LPx_SYS
::Call__PAGE_WRITE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	if(dCH__CFG_RF_EXIST_FLAG->Check__DATA(STR__YES) > 0)
	{
		if(bActive__RFx_OBJ)
		{
			CString ch_data = sCH__PARA_PAGE_DATA->Get__STRING();
			sEXT_CH__RFx__PARA_PAGE_DATA->Set__DATA(ch_data);

			return pRFx__OBJ_CTRL->Call__OBJECT(CMMD__PAGE_WRITE);
		}
	}
	return 1;
}

// MAP -----
int  CObj__LPx_SYS
::Call__MAP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{

	return Fnc__MAP(p_variable, p_alarm);
}
int  CObj__LPx_SYS
::Fnc__MAP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
LOOP_RETRY:

	Update__LOAD_POS_STATE();

	// ...
	CString alm_msg;
	CString r_act;

	if(dCH__MON_DOOR_STS->Check__DATA(STR__OPEN) < 0)
	{
		int alm_id = ALID__MAP_ERROR__DOOR_NOT_OPEN;

		alm_msg.Format(" * %s <- %s \n", 
						dCH__MON_DOOR_STS->Get__CHANNEL_NAME(),
						dCH__MON_DOOR_STS->Get__STRING());

		p_alarm->Popup__ALARM_With_MESSAGE(alm_id, alm_msg, r_act);

		if(r_act.CompareNoCase(ACT__RETRY) == 0)		goto LOOP_RETRY;
		return -101;
	}
	if(dCH__MON_FOUP_STS->Check__DATA(STR__EXIST) < 0)
	{
		int alm_id = ALID__MAP_ERROR__FOUP_NOT_EXIST;

		alm_msg.Format(" * %s <- %s \n", 
						dCH__MON_FOUP_STS->Get__CHANNEL_NAME(),
						dCH__MON_FOUP_STS->Get__STRING());

		p_alarm->Popup__ALARM_With_MESSAGE(alm_id, alm_msg, r_act);

		if(r_act.CompareNoCase(ACT__RETRY) == 0)		goto LOOP_RETRY;
		return -102;
	}
	if(dCH__MON_FOUP_POS->Check__DATA(STR__LOAD) < 0)
	{
		int alm_id = ALID__MAP_ERROR__FOUP_POS_NOT_LOAD;

		alm_msg.Format(" * %s <- %s \n", 
						dCH__MON_FOUP_POS->Get__CHANNEL_NAME(),
						dCH__MON_FOUP_POS->Get__STRING());

		p_alarm->Popup__ALARM_With_MESSAGE(alm_id, alm_msg, r_act);

		if(r_act.CompareNoCase(ACT__RETRY) == 0)		goto LOOP_RETRY;
		return -103;
	}

	if(dCH__CFG_MAPPING_SENSOR_USE->Check__DATA(STR__YES) > 0)
	{
		if(dCH__CFG_MAPPING_SENSOR_TYPE->Check__DATA(STR__ROBOT) > 0)
		{
			CString para__arm_type;
			CString para__lp_name;
			CString para__lp_slot;

			para__arm_type = "A";
			para__lp_name.Format("LP%1d", iLPx_ID);
			para__lp_slot = "1";

			// ...
			CStringArray l_para;

			l_para.Add(para__arm_type);
			l_para.Add(para__lp_name);
			l_para.Add(para__lp_slot);

			int r_flag = pROBOT__OBJ_CTRL->Call__OBJ_MODE(CMMD__LP_MAP, l_para);
			if(r_flag < 0)		return -11;
		}
		else
		{
			int r_flag = pLPx__OBJ_CTRL->Call__OBJECT(CMMD__MAP);
			if(r_flag < 0)		return -21;

			Update__MAP_INFO(p_variable,p_alarm);
		}
	}
	else
	{
		int alm_id = ALID__LP_MAPPING_SENSOR_NOT_USE;

		CString alm_msg;
		CString r_act;

		alm_msg.Format(" * Config Channel(%s) <- %s \n", 
						dCH__CFG_MAPPING_SENSOR_USE->Get__CHANNEL_NAME(),
						dCH__CFG_MAPPING_SENSOR_USE->Get__STRING());

		p_alarm->Popup__ALARM_With_MESSAGE(alm_id, alm_msg, r_act);

		if(r_act.CompareNoCase(ACT__RETRY) == 0)		goto LOOP_RETRY;
		return -11;
	}

	return 1;
}
int  CObj__LPx_SYS
::Update__MAP_INFO(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	CString str__slot_sts;

	for(int i=0; i<CFG_LPx__SLOT_MAX; i++)
	{
		CString var_data = sEXT_CH__LPx__RES_MAP_SLOT_X[i]->Get__STRING();

		if(var_data.CompareNoCase(STR__ABSENT) == 0)
		{
			str__slot_sts = STR__NONE;
		}
		else if(var_data.CompareNoCase(STR__PRESENT) == 0)
		{
			str__slot_sts = STR__EXIST;
		}
		else
		{
			str__slot_sts = STR__UNKNOWN;
		}

		//
		CString cur__slot_sts = dCH__MON_SLOT_STATUS[i]->Get__STRING();

		if((cur__slot_sts.CompareNoCase(STR__NONE)    == 0)
		|| (cur__slot_sts.CompareNoCase(STR__UNKNOWN) == 0))
		{
			dCH__MON_SLOT_STATUS[i]->Set__DATA(str__slot_sts);
		}
	}
	return 1;
}

// CLAMP -----
int  CObj__LPx_SYS
::Call__CLAMP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	return Fnc__CLAMP(p_variable,p_alarm);
}
int  CObj__LPx_SYS
::Fnc__CLAMP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	// ACTIVE CHECK ...
	{
		sCH__ACTIVE_CHECK_CLAMP_DO->Set__DATA(STR__YES);
		sCH__ACTIVE_CHECK_UNCLAMP_DO->Set__DATA(STR__YES);

		sCH__ACTIVE_CHECK_CLAMP_DI->Set__DATA(STR__YES);
		sCH__ACTIVE_CHECK_UNCLAMP_DI->Set__DATA(STR__YES);
	}

	int r_flag = pLPx__OBJ_CTRL->Call__OBJECT(CMMD__CLAMP);
	if(r_flag < 0)
	{
		sCH__ACTIVE_CHECK_CLAMP_DI->Set__DATA(STR__ERROR);
		sCH__ACTIVE_CHECK_UNCLAMP_DI->Set__DATA(STR__ERROR);

		return r_flag;
	}
	else
	{
		sCH__ACTIVE_CHECK_CLAMP_DI->Set__DATA(STR__OK);
		sCH__ACTIVE_CHECK_UNCLAMP_DI->Set__DATA(STR__OK);
	}

	return 1;
}

// UNCLAMP -----
int  CObj__LPx_SYS
::Call__UNCLAMP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	return Fnc__UNCLAMP(p_variable,p_alarm);
}
int  CObj__LPx_SYS
::Fnc__UNCLAMP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	// ACTIVE CHECK ...
	{
		sCH__ACTIVE_CHECK_CLAMP_DO->Set__DATA(STR__YES);
		sCH__ACTIVE_CHECK_UNCLAMP_DO->Set__DATA(STR__YES);

		sCH__ACTIVE_CHECK_CLAMP_DI->Set__DATA(STR__YES);
		sCH__ACTIVE_CHECK_UNCLAMP_DI->Set__DATA(STR__YES);
	}

	int r_flag = pLPx__OBJ_CTRL->Call__OBJECT(CMMD__UNCLAMP);
	if(r_flag < 0)
	{
		sCH__ACTIVE_CHECK_CLAMP_DI->Set__DATA(STR__ERROR);
		sCH__ACTIVE_CHECK_UNCLAMP_DI->Set__DATA(STR__ERROR);

		return r_flag;
	}
	else
	{
		sCH__ACTIVE_CHECK_CLAMP_DI->Set__DATA(STR__OK);
		sCH__ACTIVE_CHECK_UNCLAMP_DI->Set__DATA(STR__OK);
	}

	return 1;
}

// SHUTTLE_IN ...
int  CObj__LPx_SYS
::Call__SHUTTLE_IN(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	return Fnc__SHUTTLE_IN(p_variable,p_alarm);
}
int  CObj__LPx_SYS
::Fnc__SHUTTLE_IN(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	// ACTIVE CHECK ...
	{
		sCH__ACTIVE_CHECK_SHUTTLE_IN_DO->Set__DATA(STR__YES);
		sCH__ACTIVE_CHECK_SHUTTLE_OUT_DO->Set__DATA(STR__YES);

		sCH__ACTIVE_CHECK_SHUTTLE_IN_DI->Set__DATA(STR__YES);
		sCH__ACTIVE_CHECK_SHUTTLE_OUT_DI->Set__DATA(STR__YES);
	}

	int r_flag = pLPx__OBJ_CTRL->Call__OBJECT(CMMD__SHUTTLE_IN);
	if(r_flag < 0)
	{
		sCH__ACTIVE_CHECK_SHUTTLE_IN_DI->Set__DATA(STR__ERROR);
		sCH__ACTIVE_CHECK_SHUTTLE_OUT_DI->Set__DATA(STR__ERROR);

		return r_flag;
	}
	else
	{
		sCH__ACTIVE_CHECK_SHUTTLE_IN_DI->Set__DATA(STR__OK);
		sCH__ACTIVE_CHECK_SHUTTLE_OUT_DI->Set__DATA(STR__OK);
	}

	return 1;
}

// SHUTTLE_OUT ...
int  CObj__LPx_SYS
::Call__SHUTTLE_OUT(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	return Fnc__SHUTTLE_OUT(p_variable,p_alarm);
}
int  CObj__LPx_SYS
::Fnc__SHUTTLE_OUT(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	// ACTIVE CHECK ...
	{
		sCH__ACTIVE_CHECK_SHUTTLE_IN_DO->Set__DATA(STR__YES);
		sCH__ACTIVE_CHECK_SHUTTLE_OUT_DO->Set__DATA(STR__YES);

		sCH__ACTIVE_CHECK_SHUTTLE_IN_DI->Set__DATA(STR__YES);
		sCH__ACTIVE_CHECK_SHUTTLE_OUT_DI->Set__DATA(STR__YES);
	}

	int r_flag = pLPx__OBJ_CTRL->Call__OBJECT(CMMD__SHUTTLE_OUT);
	if(r_flag < 0)
	{
		sCH__ACTIVE_CHECK_SHUTTLE_IN_DI->Set__DATA(STR__ERROR);
		sCH__ACTIVE_CHECK_SHUTTLE_OUT_DI->Set__DATA(STR__ERROR);

		return r_flag;
	}
	else
	{
		sCH__ACTIVE_CHECK_SHUTTLE_IN_DI->Set__DATA(STR__OK);
		sCH__ACTIVE_CHECK_SHUTTLE_OUT_DI->Set__DATA(STR__OK);
	}

	return 1;
}

// EXTENDPADDLE -----
int  CObj__LPx_SYS
::Call__EXTENDPADDLE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	// ACTIVE CHECK ...
	{
		sCH__ACTIVE_CHECK_DOOR_EXT_DO->Set__DATA(STR__YES);
		sCH__ACTIVE_CHECK_DOOR_RET_DO->Set__DATA(STR__YES);

		sCH__ACTIVE_CHECK_DOOR_EXT_DI->Set__DATA(STR__YES);
		sCH__ACTIVE_CHECK_DOOR_RET_DI->Set__DATA(STR__YES);
	}

	int r_flag = pLPx__OBJ_CTRL->Call__OBJECT(CMMD__EXTENDPADDLE);
	if(r_flag < 0)
	{
		sCH__ACTIVE_CHECK_DOOR_EXT_DI->Set__DATA(STR__ERROR);
		sCH__ACTIVE_CHECK_DOOR_RET_DI->Set__DATA(STR__ERROR);

		return r_flag;
	}
	else
	{
		sCH__ACTIVE_CHECK_DOOR_EXT_DI->Set__DATA(STR__OK);
		sCH__ACTIVE_CHECK_DOOR_RET_DI->Set__DATA(STR__OK);
	}

	return 1;
}

// RETRACTPADDLE -----
int  CObj__LPx_SYS
::Call__RETRACTPADDLE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	// ACTIVE CHECK ...
	{
		sCH__ACTIVE_CHECK_DOOR_EXT_DO->Set__DATA(STR__YES);
		sCH__ACTIVE_CHECK_DOOR_RET_DO->Set__DATA(STR__YES);

		sCH__ACTIVE_CHECK_DOOR_EXT_DI->Set__DATA(STR__YES);
		sCH__ACTIVE_CHECK_DOOR_RET_DI->Set__DATA(STR__YES);
	}

	int r_flag = pLPx__OBJ_CTRL->Call__OBJECT(CMMD__RETRACTPADDLE);
	if(r_flag < 0)
	{
		sCH__ACTIVE_CHECK_DOOR_EXT_DI->Set__DATA(STR__ERROR);
		sCH__ACTIVE_CHECK_DOOR_RET_DI->Set__DATA(STR__ERROR);

		return r_flag;
	}
	else
	{
		sCH__ACTIVE_CHECK_DOOR_EXT_DI->Set__DATA(STR__OK);
		sCH__ACTIVE_CHECK_DOOR_RET_DI->Set__DATA(STR__OK);
	}

	return 1;
}

// LATCH -----
int  CObj__LPx_SYS
::Call__LATCH(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	return Fnc__LATCH(p_variable,p_alarm);
}
int  CObj__LPx_SYS
::Fnc__LATCH(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	// ACTIVE CHECK
	{
		sCH__ACTIVE_CHECK_LATCH_CLOSE_DO->Set__DATA(STR__YES);
		sCH__ACTIVE_CHECK_LATCH_OPEN_DO->Set__DATA(STR__YES);

		sCH__ACTIVE_CHECK_LATCH_CLOSE_DI->Set__DATA(STR__YES);
		sCH__ACTIVE_CHECK_LATCH_OPEN_DI->Set__DATA(STR__YES);
	}

	int r_flag = pLPx__OBJ_CTRL->Call__OBJECT(CMMD__LATCH);
	if(r_flag < 0)
	{
		sCH__ACTIVE_CHECK_LATCH_CLOSE_DI->Set__DATA(STR__ERROR);
		sCH__ACTIVE_CHECK_LATCH_OPEN_DI->Set__DATA(STR__ERROR);

		return r_flag;
	}
	else
	{
		sCH__ACTIVE_CHECK_LATCH_CLOSE_DI->Set__DATA(STR__OK);
		sCH__ACTIVE_CHECK_LATCH_OPEN_DI->Set__DATA(STR__OK);
	}

	return 1;
}

// UNLATCH -----
int  CObj__LPx_SYS
::Call__UNLATCH(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	return Fnc__UNLATCH(p_variable,p_alarm);
}
int  CObj__LPx_SYS
::Fnc__UNLATCH(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	// ACTIVE CHECK
	{
		sCH__ACTIVE_CHECK_LATCH_CLOSE_DO->Set__DATA(STR__YES);
		sCH__ACTIVE_CHECK_LATCH_OPEN_DO->Set__DATA(STR__YES);

		sCH__ACTIVE_CHECK_LATCH_CLOSE_DI->Set__DATA(STR__YES);
		sCH__ACTIVE_CHECK_LATCH_OPEN_DI->Set__DATA(STR__YES);
	}

	int r_flag = pLPx__OBJ_CTRL->Call__OBJECT(CMMD__UNLATCH);
	if(r_flag < 0)
	{
		sCH__ACTIVE_CHECK_LATCH_CLOSE_DI->Set__DATA(STR__ERROR);
		sCH__ACTIVE_CHECK_LATCH_OPEN_DI->Set__DATA(STR__ERROR);

		return r_flag;
	}
	else
	{
		sCH__ACTIVE_CHECK_LATCH_CLOSE_DI->Set__DATA(STR__OK);
		sCH__ACTIVE_CHECK_LATCH_OPEN_DI->Set__DATA(STR__OK);
	}

	return 1;
}

// ...
int  CObj__LPx_SYS
::Call__OPENDOOR(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	return Fnc__DOOR_OPEN(p_variable,p_alarm);
}
int  CObj__LPx_SYS
::Fnc__DOOR_OPEN(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	// ACTIVE CHECK ...
	{
		sCH__ACTIVE_CHECK_DOOR_EXT_DO->Set__DATA(STR__YES);
		sCH__ACTIVE_CHECK_DOOR_RET_DO->Set__DATA(STR__YES);

		sCH__ACTIVE_CHECK_DOOR_UP_DO->Set__DATA(STR__YES);
		sCH__ACTIVE_CHECK_DOOR_DN_DO->Set__DATA(STR__YES);

		//
		sCH__ACTIVE_CHECK_DOOR_EXT_DI->Set__DATA(STR__YES);
		sCH__ACTIVE_CHECK_DOOR_RET_DI->Set__DATA(STR__YES);

		sCH__ACTIVE_CHECK_DOOR_UP_DI->Set__DATA(STR__YES);
		sCH__ACTIVE_CHECK_DOOR_DN_DI->Set__DATA(STR__YES);

		//
		dCH__ACTIVE_CHECK_DOOR_VAC_DO->Set__DATA(STR__YES);
		dCH__ACTIVE_CHECK_DOOR_VAC_DI->Set__DATA(STR__YES);
	}

	int r_flag = pLPx__OBJ_CTRL->Call__OBJECT(CMMD__DOOR_OPEN);
	if(r_flag < 0)
	{
		sCH__ACTIVE_CHECK_DOOR_EXT_DI->Set__DATA(STR__ERROR);
		sCH__ACTIVE_CHECK_DOOR_RET_DI->Set__DATA(STR__ERROR);

		sCH__ACTIVE_CHECK_DOOR_UP_DI->Set__DATA(STR__ERROR);
		sCH__ACTIVE_CHECK_DOOR_DN_DI->Set__DATA(STR__ERROR);

		dCH__ACTIVE_CHECK_DOOR_VAC_DI->Set__DATA(STR__ERROR);

		return r_flag;
	}
	else
	{
		sCH__ACTIVE_CHECK_DOOR_EXT_DI->Set__DATA(STR__OK);
		sCH__ACTIVE_CHECK_DOOR_RET_DI->Set__DATA(STR__OK);

		sCH__ACTIVE_CHECK_DOOR_UP_DI->Set__DATA(STR__OK);
		sCH__ACTIVE_CHECK_DOOR_DN_DI->Set__DATA(STR__OK);

		dCH__ACTIVE_CHECK_DOOR_VAC_DI->Set__DATA(STR__OK);

		// ...
		{
			SCX__TIMER_CTRL x_timer;
			x_timer->REGISTER__ABORT_OBJECT(sObject_Name);

			double cfg__delay_sec = aCH__CFG_DOOR_OPEN_COMPLETE_DELAY_TIME->Get__VALUE();
	
			if(x_timer->WAIT(cfg__delay_sec) < 0)		return -11;
		}
	}

	return r_flag;
}

int  CObj__LPx_SYS
::Call__OPENDOOR_WITH_NO_UNLATCH(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	return pLPx__OBJ_CTRL->Call__OBJECT(CMMD__OPENDOOR_WITH_NO_UNLATCH);
}

int  CObj__LPx_SYS
::Call__CLOSEDOOR(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	return Fnc__DOOR_CLOSE(p_variable,p_alarm);
}
int  CObj__LPx_SYS
::Fnc__DOOR_CLOSE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	// ACTIVE CHECK ...
	{
		sCH__ACTIVE_CHECK_DOOR_EXT_DO->Set__DATA(STR__YES);
		sCH__ACTIVE_CHECK_DOOR_RET_DO->Set__DATA(STR__YES);

		sCH__ACTIVE_CHECK_DOOR_UP_DO->Set__DATA(STR__YES);
		sCH__ACTIVE_CHECK_DOOR_DN_DO->Set__DATA(STR__YES);

		//
		sCH__ACTIVE_CHECK_DOOR_EXT_DI->Set__DATA(STR__YES);
		sCH__ACTIVE_CHECK_DOOR_RET_DI->Set__DATA(STR__YES);

		sCH__ACTIVE_CHECK_DOOR_UP_DI->Set__DATA(STR__YES);
		sCH__ACTIVE_CHECK_DOOR_DN_DI->Set__DATA(STR__YES);

		//
		dCH__ACTIVE_CHECK_DOOR_VAC_DO->Set__DATA(STR__YES);
		dCH__ACTIVE_CHECK_DOOR_VAC_DI->Set__DATA(STR__YES);
	}

	int r_flag = Alarm_Check__ARM_NOT_RETRACTED(p_alarm);
	if(r_flag < 0)		return -11;

	r_flag = Is__LP_CLOSE_CND(p_variable, p_alarm);
	if(r_flag < 0)		return -21;

	r_flag = pLPx__OBJ_CTRL->Call__OBJECT(CMMD__DOOR_CLOSE);
	if(r_flag < 0)
	{
		sCH__ACTIVE_CHECK_DOOR_EXT_DI->Set__DATA(STR__ERROR);
		sCH__ACTIVE_CHECK_DOOR_RET_DI->Set__DATA(STR__ERROR);

		sCH__ACTIVE_CHECK_DOOR_UP_DI->Set__DATA(STR__ERROR);
		sCH__ACTIVE_CHECK_DOOR_DN_DI->Set__DATA(STR__ERROR);

		dCH__ACTIVE_CHECK_DOOR_VAC_DI->Set__DATA(STR__ERROR);

		return -31;
	}
	else
	{
		sCH__ACTIVE_CHECK_DOOR_EXT_DI->Set__DATA(STR__OK);
		sCH__ACTIVE_CHECK_DOOR_RET_DI->Set__DATA(STR__OK);

		sCH__ACTIVE_CHECK_DOOR_UP_DI->Set__DATA(STR__OK);
		sCH__ACTIVE_CHECK_DOOR_DN_DI->Set__DATA(STR__OK);

		dCH__ACTIVE_CHECK_DOOR_VAC_DI->Set__DATA(STR__OK);
	}

	return 1;
}

// ...
int  CObj__LPx_SYS
::Call__VACON(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	// ACTIVE.CHECK ...
	{
		dCH__ACTIVE_CHECK_DOOR_VAC_DO->Set__DATA(STR__YES);
		dCH__ACTIVE_CHECK_DOOR_VAC_DI->Set__DATA(STR__YES);
	}

	int r_flag = pLPx__OBJ_CTRL->Call__OBJECT(CMMD__VACON);

	// ACTIVE.OK ...
	{
		if(r_flag > 0)			dCH__ACTIVE_CHECK_DOOR_VAC_DI->Set__DATA(STR__OK);
		else					dCH__ACTIVE_CHECK_DOOR_VAC_DI->Set__DATA(STR__ERROR);
	}
	return r_flag;
}
int  CObj__LPx_SYS::
Call__VACOFF(CII_OBJECT__VARIABLE* p_variable,
			 CII_OBJECT__ALARM* p_alarm)
{
	// ACTIVE.CHECK ...
	{
		dCH__ACTIVE_CHECK_DOOR_VAC_DO->Set__DATA(STR__YES);
		dCH__ACTIVE_CHECK_DOOR_VAC_DI->Set__DATA(STR__YES);
	}

	int r_flag = pLPx__OBJ_CTRL->Call__OBJECT(CMMD__VACOFF);

	// ACTIVE.OK ...
	{
		if(r_flag > 0)			dCH__ACTIVE_CHECK_DOOR_VAC_DI->Set__DATA(STR__OK);
		else					dCH__ACTIVE_CHECK_DOOR_VAC_DI->Set__DATA(STR__ERROR);
	}
	return r_flag;
}

// EXTENDSHUTTLE -----
int  CObj__LPx_SYS::
Call__EXTENDSHUTTLE(CII_OBJECT__VARIABLE* p_variable,
					CII_OBJECT__ALARM* p_alarm)
{
	//
	return pLPx__OBJ_CTRL->Call__OBJECT(CMMD__EXTENDSHUTTLE);
}

// RETRACTSHUTTLE -----
int  CObj__LPx_SYS::
Call__RETRACTSHUTTLE(CII_OBJECT__VARIABLE* p_variable,
					 CII_OBJECT__ALARM* p_alarm)
{
	//
	return pLPx__OBJ_CTRL->Call__OBJECT(CMMD__RETRACTSHUTTLE);
}

int  CObj__LPx_SYS::
Call__PADDLEUP(CII_OBJECT__VARIABLE* p_variable,
			   CII_OBJECT__ALARM* p_alarm)
{
	//
	return pLPx__OBJ_CTRL->Call__OBJECT(CMMD__PADDLEUP);
}

int  CObj__LPx_SYS::
Call__PADDLEDOWN(CII_OBJECT__VARIABLE* p_variable,
				 CII_OBJECT__ALARM* p_alarm)
{
	//
	return pLPx__OBJ_CTRL->Call__OBJECT(CMMD__PADDLEDOWN);
}

// HALT -----
int  CObj__LPx_SYS::
Call__HALT(CII_OBJECT__VARIABLE* p_variable,
		   CII_OBJECT__ALARM* p_alarm)
{
	//
	return pLPx__OBJ_CTRL->Call__OBJECT(CMMD__HALT);
}