#include "StdAfx.h"
#include "CObj__VAC_ROBOT_STD.h"
#include "CObj__VAC_ROBOT_STD__DEF.h"
#include "CObj__VAC_ROBOT_STD__ALID.h"

#include "Macro_Function.h"


// ...
void  CObj__VAC_ROBOT_STD::
Fnc__APP_LOG(const CString& log_msg)
{
	xLOG_CTRL->WRITE__LOG(log_msg);
}


// VAC_ROBOT -----
int  CObj__VAC_ROBOT_STD::
Fnc__CALL_ROTATE(CII_OBJECT__VARIABLE* p_variable,
				 CII_OBJECT__ALARM* p_alarm,
				 const CString& para_arm,
				 const CString& para_module,
				 const CString& para_slot)
{
	//.....
	CStringArray l_para;

	l_para.Add(para_arm);
	l_para.Add(para_module);
	l_para.Add(para_slot);

	//
	return pROBOT__OBJ_CTRL->Call__OBJ_MODE(CMMD__ROTATE,l_para);
}
int  CObj__VAC_ROBOT_STD::
Fnc__RUN_ROTATE(CII_OBJECT__VARIABLE* p_variable,
				CII_OBJECT__ALARM* p_alarm,
				const CString& para_arm,
				const CString& para_module,
				const CString& para_slot)
{
	//.....
	CStringArray l_para;

	l_para.Add(para_arm);
	l_para.Add(para_module);
	l_para.Add(para_slot);

	//
	return pROBOT__OBJ_CTRL->Run__OBJ_MODE(CMMD__ROTATE,l_para);
}


int  CObj__VAC_ROBOT_STD::
Fnc__CALL_PICK(CII_OBJECT__VARIABLE* p_variable,
			   CII_OBJECT__ALARM* p_alarm,
			   const CString& para_arm,
			   const CString& para_module,
			   const CString& para_slot)
{
	//.....
	CStringArray l_para;

	l_para.Add(para_arm);
	l_para.Add(para_module);
	l_para.Add(para_slot);

	//
	return pROBOT__OBJ_CTRL->Call__OBJ_MODE(CMMD__PICK,l_para);
}

int  CObj__VAC_ROBOT_STD::
Fnc__CALL_PLACE(CII_OBJECT__VARIABLE* p_variable,
				CII_OBJECT__ALARM* p_alarm,
				const CString& para_arm,
				const CString& para_module,
				const CString& para_slot)
{
	//.....
	CStringArray l_para;

	l_para.Add(para_arm);
	l_para.Add(para_module);
	l_para.Add(para_slot);

	//
	return pROBOT__OBJ_CTRL->Call__OBJ_MODE(CMMD__PLACE,l_para);
}


// PMx CONTROL -----
int  CObj__VAC_ROBOT_STD::
Fnc__PMx_SV_CLOSE(const int pmc_id)
{
	if((dEXT_CH__CFG_SETUP_TEST_MODE->Check__DATA(STR__ENABLE) > 0)
	&& (dEXT_CH__CFG_SETUP_LLx_DOOR_SLOT_VLV_CONTROL->Check__DATA(STR__DISABLE) > 0))
	{
		return 1;
	}

	// ...
	CString var_data;
	var_data.Format("%1d",pmc_id);
	aEXT_CH__TMC_VLV__PMC_ID->Set__DATA(var_data);

	return pTMC_VLV__OBJ_CTRL->Call__OBJECT("PMC_SV_CLOSE");
}
int  CObj__VAC_ROBOT_STD::
Run__PMx_SV_CLOSE(const int pmc_id)
{
	if((dEXT_CH__CFG_SETUP_TEST_MODE->Check__DATA(STR__ENABLE) > 0)
	&& (dEXT_CH__CFG_SETUP_LLx_DOOR_SLOT_VLV_CONTROL->Check__DATA(STR__DISABLE) > 0))
	{
		return 1;
	}

	// ...
	CString var_data;
	var_data.Format("%1d",pmc_id);
	aEXT_CH__TMC_VLV__PMC_ID->Set__DATA(var_data);

	return pTMC_VLV__OBJ_CTRL->Run__OBJECT("PMC_SV_CLOSE");
}

int  CObj__VAC_ROBOT_STD::
Fnc__PMx_SV_TRANSFER_OPEN(const int pmc_id)
{
	if((dEXT_CH__CFG_SETUP_TEST_MODE->Check__DATA(STR__ENABLE) > 0)
	&& (dEXT_CH__CFG_SETUP_LLx_DOOR_SLOT_VLV_CONTROL->Check__DATA(STR__DISABLE) > 0))
	{
		return 1;
	}

	// ...
	CString var_data;
	var_data.Format("%1d",pmc_id);
	aEXT_CH__TMC_VLV__PMC_ID->Set__DATA(var_data);

	return pTMC_VLV__OBJ_CTRL->Call__OBJECT("PMC_SV_TRANSFER_OPEN");
}
int  CObj__VAC_ROBOT_STD::
Fnc__Run_PMx_SV_TRANSFER_OPEN(const int pmc_id)
{
	if((dEXT_CH__CFG_SETUP_TEST_MODE->Check__DATA(STR__ENABLE) > 0)
	&& (dEXT_CH__CFG_SETUP_LLx_DOOR_SLOT_VLV_CONTROL->Check__DATA(STR__DISABLE) > 0))
	{
		return 1;
	}

	// ...
	CString var_data;
	var_data.Format("%1d",pmc_id);
	aEXT_CH__TMC_VLV__PMC_ID->Set__DATA(var_data);

	return pTMC_VLV__OBJ_CTRL->Run__OBJECT("PMC_SV_TRANSFER_OPEN");
}
int  CObj__VAC_ROBOT_STD::
Fnc__Wait_PMx_SV_TRANSFER_OPEN(const int pmc_id)
{
	CString var_data;
	var_data.Format("%1d",pmc_id);
	aEXT_CH__TMC_VLV__PMC_ID->Set__DATA(var_data);

	//
	return pTMC_VLV__OBJ_CTRL->When__OBJECT();
}

int  CObj__VAC_ROBOT_STD::
Fnc__PMx_SV_CLOSE_CHECK(CII_OBJECT__ALARM* p_alarm)
{
LOOP_RETRY:

	//.....
	pTMC_VLV__OBJ_CTRL->Call__OBJECT("CHECK_PMC_SV_ALL_CLOSE");

	//.....
	CStringArray l_err_msg;
	pTMC_VLV__OBJ_CTRL->Get__RETURN_FNC_PARAMETER(l_err_msg);

	int limit = l_err_msg.GetSize();
	int i;

	if(limit < 1)		return 1;

	//.....
	int alarm_id = ALID__PMx_SLIT_VALVE__NOT_CLOSE;
	CString alarm_msg;
	CString alarm_bff;
	CString r_act;

	alarm_msg = "Please, check PMC's slit-valve.\n";

	limit = l_err_msg.GetSize();
	for(i=0;i<limit;i++)
	{
		alarm_bff.Format("   %s\n",l_err_msg[i]);
		alarm_msg += alarm_bff;
	}

	alarm_msg += "\n";
	alarm_msg += "If you click [RETRY], pmc's slit-valve will be closed !\n";

	//.....
	p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,alarm_msg,r_act);	

	if(r_act.CompareNoCase("RETRY") == 0)
	{
		pTMC_VLV__OBJ_CTRL->Call__OBJECT("PMC_ALL_SV_CLOSE");
		goto LOOP_RETRY;
	}
	return -1;
}

int  CObj__VAC_ROBOT_STD::
Sub_OBJCALL_PMC_ACT_HANDOFF(const int pmc_id, const CString obj_mode)
{
	CString szMessage;
	CString str_log;

	// ...
	{
		str_log.Format("[Call__ACT_HOFF] TMC -> PMC Call Mode: %s", obj_mode);
		Fnc__APP_LOG(str_log);
	}

	szMessage.Format("%s Call Start.....", obj_mode);
	sCH__OBJ_MESSAGE->Set__DATA(szMessage);

	if(iSIM__PMC_DUMMY_MODE < 0)
	{
		int retVal = pOPR_PMC_HANDOFF[pmc_id]->Call__OBJECT(obj_mode);
		if(retVal != OBJ_AVAILABLE)
		{
			str_log.Format("[Call__ACT_HOFF] TMC->PMC Call Mode: %s Failed, retVal : %d", obj_mode, retVal);
			Fnc__APP_LOG(str_log);

			szMessage.Format("%s Call Failed.....", obj_mode);
			sCH__OBJ_MESSAGE->Set__DATA(szMessage);
			return retVal;
		}
	}
	else
	{
		
	}

	szMessage.Format("%s Call End.....", obj_mode);
	sCH__OBJ_MESSAGE->Set__DATA(szMessage);
	return 1;
}

int  CObj__VAC_ROBOT_STD::
Sub_OBJRUN_PMC_ACT_HANDOFF(const int pmc_id, const CString obj_mode)
{
	CString szMessage;
	CString str_log;

	str_log.Format("[Run__ACT_HOFF] TMC -> PMC Call Mode: %s", obj_mode);
	Fnc__APP_LOG(str_log);

	szMessage.Format("%s Call Start.....", obj_mode);
	sCH__OBJ_MESSAGE->Set__DATA(szMessage);

	if(iSIM__PMC_DUMMY_MODE < 0)
	{
		int retVal = pOPR_PMC_HANDOFF[pmc_id]->Run__OBJECT(obj_mode);
		if(retVal != OBJ_AVAILABLE)
		{
			str_log.Format("[Run__ACT_HOFF] TMC->PMC Call Mode: %s Failed, retVal : %d", obj_mode, retVal);
			Fnc__APP_LOG(str_log);
			
			szMessage.Format("%s Call Failed.....", obj_mode);
			sCH__OBJ_MESSAGE->Set__DATA(szMessage);
			return retVal;
		}
	}
	else
	{

	}

	szMessage.Format("%s Call End.....", obj_mode);
	sCH__OBJ_MESSAGE->Set__DATA(szMessage);
	return 1l;
}

int  CObj__VAC_ROBOT_STD::
Fnc__ACT_HOFF_PICK(CII_OBJECT__VARIABLE* p_variable,
				   CII_OBJECT__ALARM* p_alarm,
				   const CString& arm_type,
				   const CString& stn_name,
				   const CString& stn_slot,
				   const int pm_index,
				   const int handoff_mode)
{
	int retVal;
	CString str_log;
	CString szMessage;
	CStringArray l_para;

	Fnc__APP_LOG("Pick Active HandOff Start...");

	l_para.Add(arm_type);
	l_para.Add(stn_name);
	l_para.Add(stn_slot);

	if(iSIM__PMC_DUMMY_MODE < 0)
	{
		CString pm1_status;
		
		Fnc__APP_LOG("[Fnc__ACT_HOFF_PICK] : ACTIVE HANDOFF PMx CTRL is AVAILABLE");
		
		pOPR_PMC_HANDOFF[pm_index]->Get__OBJECT_STATUS(pm1_status);
		str_log.Format("[Fnc__ACT_HOFF_PICK] : PMC_HANDOFF Status : %s", pm1_status);
		Fnc__APP_LOG(str_log);
		
		if(pm1_status.CompareNoCase("AVAILABLE") != 0)
		{
			pOPR_PMC_HANDOFF[pm_index]->Call__OBJECT("INIT");
		}	

		//
		retVal = pOPR_PMC_HANDOFF[pm_index]->When__OBJECT();
		
		str_log.Format("[Fnc__ACT_HOFF_PICK] TMC->PMC Call Mode: When retVal : %d", retVal);
		Fnc__APP_LOG(str_log);

		if(retVal != OBJ_AVAILABLE)
		{
			str_log.Format("[Fnc__ACT_HOFF_PICK] TMC->PMC Call Mode: When retVal : -1");
			Fnc__APP_LOG(str_log);
			return OBJ_ABORT;	
		}
	}

	// S1 : PIN UP....
	if(handoff_mode == ACTIVE_HANDOFF)
	{
		if(Sub_OBJCALL_PMC_ACT_HANDOFF(pm_index, "S1") < 0)	
		{
			return OBJ_ABORT;
		}
	}

	if(Is__TRANSFER_READY_TO_PICK(p_alarm, "PICK", "EXTEND", pm_index) < 0)
	{
		return OBJ_ABORT;
	}

	// EXTEND
	retVal = pROBOT__OBJ_CTRL->Call__OBJ_MODE(CMMD__EXTEND,l_para);
	if(retVal != OBJ_AVAILABLE)				return retVal;

	// S2 : PIN DOWN....
	{
		if(handoff_mode == ACTIVE_HANDOFF)
		{
			if(Sub_OBJCALL_PMC_ACT_HANDOFF(pm_index, "S2") < 0)	return OBJ_ABORT;
		}
	}

	//2010.08.12 KJH Extend 전에 C-Ring Check를 했기 때문에 C-Ring Position을 체크할 필요가 없음.
	// 	if(Is__CR_UP_and_PIN_DOWN_STS(p_alarm, "PICK", "RETRACT", pm_index) < 0)
	// 	{
	// 		return OBJ_ABORT;
	// 	}

	//2010.08.12 KJH
	//PIN UP CHECK...
	if(Is__PIN_DOWN_STS(p_alarm, "PICK", "RETRACT", pm_index) < 0)
	{
		return OBJ_ABORT;
	}

	// Wafer 정보 Update. (Wafer는 ARM에 있다..)
	Fnc__CHANGE_MATERIAL_INFO(-1,arm_type,stn_name,stn_slot);

	// RETRACT
	retVal = pROBOT__OBJ_CTRL->Call__OBJ_MODE(CMMD__RETRACT,l_para);
	if(retVal != OBJ_AVAILABLE)				return retVal;

	return OBJ_AVAILABLE;
}

int  CObj__VAC_ROBOT_STD::
Fnc__ACT_HOFF_PLACE(CII_OBJECT__VARIABLE* p_variable,
					CII_OBJECT__ALARM* p_alarm,
					const CString& arm_type,
					const CString& stn_name,
					const CString& stn_slot,
					const int pm_index,
					const int handoff_mode)
{
	// ...
	int retVal;
	CString str_log;
	CString szMessage;
	CStringArray l_para;

	Fnc__APP_LOG("Place Active HandOff Start...");

	l_para.Add(arm_type);
	l_para.Add(stn_name);
	l_para.Add(stn_slot);

	if(iSIM__PMC_DUMMY_MODE < 0)
	{
		CString pm1_status;

		Fnc__APP_LOG("[Fnc__ACT_HOFF_PLACE] : ACTIVE HANDOFF PMx CTRL is AVAILABLE");
		pOPR_PMC_HANDOFF[pm_index]->Get__OBJECT_STATUS(pm1_status);

		str_log.Format("[Fnc__ACT_HOFF_PLACE] : PMC_HANDOFF Status : %s", pm1_status);
		Fnc__APP_LOG(str_log);

		if(pm1_status.CompareNoCase("AVAILABLE") != 0)
		{
			pOPR_PMC_HANDOFF[pm_index]->Call__OBJECT("INIT");
		}	

		//
		retVal = pOPR_PMC_HANDOFF[pm_index]->When__OBJECT();
		
		str_log.Format("[Fnc__ACT_HOFF_PLACE] TMC->PMC Call Mode: When retVal : %d", retVal);
		Fnc__APP_LOG(str_log);
	
		if(retVal != OBJ_AVAILABLE)
		{
			str_log.Format("[Fnc__ACT_HOFF_PLACE] TMC->PMC Call Mode: When retVal : -1");
			Fnc__APP_LOG(str_log);
			return OBJ_ABORT;	
		}
	}

	if(Is__TRANSFER_READY_TO_PLACE(p_alarm, "PLACE", "EXTEND", pm_index) < 0)
	{
		return OBJ_ABORT;
	}

	// 2. EXTEND
	retVal = pROBOT__OBJ_CTRL->Call__OBJ_MODE(CMMD__EXTEND,l_para);
	if(retVal != OBJ_AVAILABLE)				return retVal;

	// R2 : PIN UP....
	if(handoff_mode == ACTIVE_HANDOFF)
	{
		if(Sub_OBJCALL_PMC_ACT_HANDOFF(pm_index, "R2") < 0)
		{
			return OBJ_ABORT;
		}
	}

	if(Is__PIN_UP_STS(p_alarm, "PLACE", "RETRACT", pm_index) < 0)
	{
		return OBJ_ABORT;
	}

	// Wafer 정보 Update. (Wafer는 ARM에 없다.. PIN에 있다.)
	Fnc__CHANGE_MATERIAL_INFO(1,arm_type,stn_name,stn_slot);

	// RETRACT
	retVal = pROBOT__OBJ_CTRL->Call__OBJ_MODE(CMMD__RETRACT,l_para);
	if(retVal != OBJ_AVAILABLE)				return retVal;

	return OBJ_AVAILABLE;
}

int  CObj__VAC_ROBOT_STD::
Is__TRANSFER_READY_TO_PICK(CII_OBJECT__ALARM* p_alarm, 
						   const CString str_pick_place,
						   const CString str_ext_ret,
						   const int pm_index)
{
	CString str_shutter;
	CString str_pin;
	CString str_cr_pos;
	int r_cnt = 0;

	SCX__TIMER_CTRL xTimer;
	xTimer->REGISTER__ABORT_OBJECT(sObject_Name);


LOOP_RETRY:

	// ...
	int check_count = 10;

	while(1)
	{
		int ok_flag = 1;

		if(dEXT_CH__PMx_CFG_USE_SHUTTER[pm_index]->Check__DATA(STR__YES) > 0)
		{
			sEXT_CH__PMx_SHUTTER_STS[pm_index]->Get__DATA(str_shutter);

			if(str_shutter.Find(STR__OPEN) != 0)
			{
				ok_flag = -1;
			}
		}

		// Pin Up ...
		if(dEXT_CH__PMx_CFG_USE_LIFT_PIN[pm_index]->Check__DATA(STR__YES) > 0)
		{
			sEXT_CH__PMx_LIFT_PIN_POS_STS[pm_index]->Get__DATA(str_pin);

			if(str_pin.Find(STR__UP) != 0)
			{
				ok_flag = -1;
			}
		}

		// CR Position ...
		if(dEXT_CH__PMx_CFG_USE_CR_POSITION[pm_index]->Check__DATA(STR__YES) > 0)
		{
			sEXT_CH__PMx_CR_POSITION_STS[pm_index]->Get__DATA(str_cr_pos);

			if(str_cr_pos.Find(STR__UP) != 0)
			{
				ok_flag = -1;
			}
		}

		if(ok_flag > 0)
		{
			break;
		}

		// ...
		{
			check_count--;
			if(check_count < 0)
			{
				break;
			}
		}

		Sleep(100);
	}


	if(iSIM__PMC_DUMMY_MODE < 0)
	{
		// Shutter ...
		if(dEXT_CH__PMx_CFG_USE_SHUTTER[pm_index]->Check__DATA(STR__YES) > 0)
		{
			sEXT_CH__PMx_SHUTTER_STS[pm_index]->Get__DATA(str_shutter);
		}
		else
		{
			str_shutter = STR__OPEN;
		}

		// Pin Up ...
		if(dEXT_CH__PMx_CFG_USE_LIFT_PIN[pm_index]->Check__DATA(STR__YES) > 0)
		{
			sEXT_CH__PMx_LIFT_PIN_POS_STS[pm_index]->Get__DATA(str_pin);
		}
		else
		{
			str_pin = STR__UP;
		}

		// CR Position ...
		if(dEXT_CH__PMx_CFG_USE_CR_POSITION[pm_index]->Check__DATA(STR__YES) > 0)
		{
			sEXT_CH__PMx_CR_POSITION_STS[pm_index]->Get__DATA(str_cr_pos);
		}
		else
		{
			str_cr_pos = STR__UP;
		}
	}
	else
	{
		// Shutter ...
		{
			str_shutter = STR__OPEN;
			sEXT_CH__PMx_SHUTTER_STS[pm_index]->Set__DATA(str_shutter);
		}

		// Pin Up ...
		{
			str_pin = STR__UP;
			sEXT_CH__PMx_LIFT_PIN_POS_STS[pm_index]->Set__DATA(str_pin);
		}

		// CR Position ...
		{
			str_cr_pos = STR__UP;
			sEXT_CH__PMx_CR_POSITION_STS[pm_index]->Set__DATA(str_cr_pos);
		}
	}

	if((str_shutter.Find(STR__OPEN) != 0)
	|| (str_pin.Find(STR__UP) != 0)
	|| (str_cr_pos.Find(STR__UP) != 0))
	{
		if(xTimer->WAIT(2.0) < 0)
		{
			return OBJ_ABORT;
		}

		// ...
		{
			r_cnt++;
			if(r_cnt <= 1)
			{
				Fnc__APP_LOG("Goto Retry...");
				goto LOOP_RETRY;
			}
		}

		// ...
		{
			int alarm_id = ALID__PMx__NOT_TRANSFER_READY_TO_PICK;
			CString err_msg;
			CString err_bff;
			CString r_act;

			// ...
			{
				err_msg.Format("PM%1d(으)로 [%s : %s] Action 수행 중입니다. \n", 
							   pm_index+1,
							   str_pick_place,
							   str_ext_ret); 

				if(str_shutter.Find(STR__OPEN) != 0)
				{
					err_msg += " Shutter State ... \n";

					err_bff.Format("  *  %s <- %s \n",
								   sEXT_CH__PMx_SHUTTER_STS[pm_index]->Get__CHANNEL_NAME(),
								   sEXT_CH__PMx_SHUTTER_STS[pm_index]->Get__STRING());
					err_msg += err_bff;
				}
				if(str_pin.Find(STR__UP) != 0)
				{
					err_msg += " Lift_Pin State ... \n";
					
					err_bff.Format("  *  %s <- %s \n",
						           sEXT_CH__PMx_LIFT_PIN_POS_STS[pm_index]->Get__CHANNEL_NAME(),
								   sEXT_CH__PMx_LIFT_PIN_POS_STS[pm_index]->Get__STRING());
					err_msg += err_bff;
				}
				if(str_cr_pos.Find(STR__UP) != 0)
				{
					err_msg += " CRing Position State ... \n";

					err_bff.Format("  *  %s <- %s \n",
								   sEXT_CH__PMx_CR_POSITION_STS[pm_index]->Get__CHANNEL_NAME(),
								   sEXT_CH__PMx_CR_POSITION_STS[pm_index]->Get__STRING());
					err_msg += err_bff;
				}

				Fnc__APP_LOG(err_msg);
			}

			p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,err_msg,r_act);

			if(r_act.CompareNoCase(CHECK__RETRY) == 0)
			{
				goto LOOP_RETRY;
			}
		}

		return -1;
	}

	return OBJ_AVAILABLE;
}

int  CObj__VAC_ROBOT_STD::
Is__PIN_UP_STS(CII_OBJECT__ALARM* p_alarm, 
			   const CString str_pick_place,
			   const CString str_ext_ret,
			   const int pm_index)
{
	CString str_pin;
	int nRtyCnt = 0;

	SCX__TIMER_CTRL xTimer;
	xTimer->REGISTER__ABORT_OBJECT(sObject_Name);


LOOP_RETRY:

	// ...
	int check_count = 10;

	while(1)
	{
		sEXT_CH__PMx_LIFT_PIN_POS_STS[pm_index]->Get__DATA(str_pin);

		if(str_pin.Find(STR__UP) == 0)
		{
			break;
		}

		// ...
		{
			check_count--;
			if(check_count < 0)
			{
				break;
			}
		}

		Sleep(100);
	}

	if(iSIM__PMC_DUMMY_MODE < 0)
	{
		sEXT_CH__PMx_LIFT_PIN_POS_STS[pm_index]->Get__DATA(str_pin);
	}
	else
	{
		str_pin = STR__UP;
		sEXT_CH__PMx_LIFT_PIN_POS_STS[pm_index]->Set__DATA(str_pin);
	}

	if(str_pin.Find(STR__UP) != 0)
	{
		if(xTimer->WAIT(2.0) < 0)
		{
			return OBJ_ABORT;
		}

		// ...
		{
			nRtyCnt++;
			if(nRtyCnt <= 1)
			{
				Fnc__APP_LOG("Goto Retry...");
				goto LOOP_RETRY;
			}
		}		

		// ...
		{
			int alarm_id = ALID__PMx__NOT_PIN_UP_ERROR;
			CString err_msg;
			CString r_act;

			// ...
			{
				err_msg.Format("During [%s] Action, Before VAC Robot %s, PM%d's PIN[%s].\n", 
								str_pick_place, 
								str_ext_ret, 
								pm_index+1,
								str_pin);

				Fnc__APP_LOG(err_msg);
			}

			p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,err_msg,r_act);

			if(r_act.CompareNoCase(CHECK__RETRY) == 0)
			{
				goto LOOP_RETRY;
			}
		}

		return -1;
	}

	return OBJ_AVAILABLE;
}

int  CObj__VAC_ROBOT_STD
::Is__TRANSFER_READY_TO_PLACE(CII_OBJECT__ALARM* p_alarm, 
							  const CString str_pick_place,
							  const CString str_ext_ret,
							  const int pm_index)
{
	CString str_shutter;
	CString str_pin;
	CString str_cr_pos;
	int nRtyCnt = 0;

	SCX__TIMER_CTRL xTimer;
	xTimer->REGISTER__ABORT_OBJECT(sObject_Name);

LOOP_RETRY:

	// ...
	int check_count = 10;

	while(1)
	{
		int ok_flag = 1;

		// Shutter Open ...
		if(dEXT_CH__PMx_CFG_USE_SHUTTER[pm_index]->Check__DATA(STR__YES) > 0)
		{
			sEXT_CH__PMx_SHUTTER_STS[pm_index]->Get__DATA(str_shutter);

			if(str_shutter.Find(STR__OPEN) != 0)
			{
				ok_flag = -1;
			}
		}

		// Pin Down ...
		if(dEXT_CH__PMx_CFG_USE_LIFT_PIN[pm_index]->Check__DATA(STR__YES) > 0)
		{
			sEXT_CH__PMx_LIFT_PIN_POS_STS[pm_index]->Get__DATA(str_pin);

			if(str_pin.Find(STR__DOWN) != 0)
			{
				ok_flag = -1;
			}
		}

		// CR Position ...
		if(dEXT_CH__PMx_CFG_USE_CR_POSITION[pm_index]->Check__DATA(STR__YES) > 0)
		{
			sEXT_CH__PMx_CR_POSITION_STS[pm_index]->Get__DATA(str_cr_pos);

			if(str_cr_pos.Find(STR__UP) != 0)
			{
				ok_flag = -1;
			}
		}

		if(ok_flag > 0)
		{
			break;
		}

		check_count--;
		if(check_count < 0)
		{
			break;
		}

		Sleep(100);
	}

	
	if(iSIM__PMC_DUMMY_MODE < 0)
	{
		// Shutter Open ...
		if(dEXT_CH__PMx_CFG_USE_SHUTTER[pm_index]->Check__DATA(STR__YES) > 0)
		{
			sEXT_CH__PMx_SHUTTER_STS[pm_index]->Get__DATA(str_shutter);
		}
		else
		{
			str_shutter = STR__OPEN;
		}
		
		// Pin Down ...
		if(dEXT_CH__PMx_CFG_USE_LIFT_PIN[pm_index]->Check__DATA(STR__YES) > 0)
		{
			sEXT_CH__PMx_LIFT_PIN_POS_STS[pm_index]->Get__DATA(str_pin);
		}
		else
		{
			str_pin = STR__DOWN;
		}

		// CR Position ...
		if(dEXT_CH__PMx_CFG_USE_CR_POSITION[pm_index]->Check__DATA(STR__YES) > 0)
		{
			sEXT_CH__PMx_CR_POSITION_STS[pm_index]->Get__DATA(str_cr_pos);
		}
		else
		{
			str_cr_pos = STR__UP;
		}
	}
	else
	{
		// Shutter Open ...
		{
			str_shutter = STR__OPEN;
			sEXT_CH__PMx_SHUTTER_STS[pm_index]->Set__DATA(str_shutter);
		}

		// Pin Down ...
		{
			str_pin = STR__DOWN;
			sEXT_CH__PMx_LIFT_PIN_POS_STS[pm_index]->Set__DATA(str_pin);
		}

		// CR Position ...
		{
			str_cr_pos = STR__UP;
			sEXT_CH__PMx_CR_POSITION_STS[pm_index]->Set__DATA(str_cr_pos);
		}
	}
	
	if((str_shutter.Find(STR__OPEN) != 0)
	|| (str_pin.Find(STR__DOWN) != 0)
	|| (str_cr_pos.Find(STR__UP) != 0))
	{
		if(xTimer->WAIT(2.0) < 0)	
		{
			return OBJ_ABORT;
		}

		// ...
		{
			nRtyCnt++;
			if(nRtyCnt <= 1)
			{
				Fnc__APP_LOG("Goto Retry...");
				goto LOOP_RETRY;
			}
		}

		// ...
		{
			int alarm_id = ALID__PMx__NOT_TRANSFER_READY_TO_PLACE;
			CString err_msg;
			CString err_bff;
			CString r_act;

			// ...
			{
				err_msg.Format("PM%1d(으)로 [%s : %s] Action 수행 중입니다. \n", 
					           pm_index+1,
							   str_pick_place,
							   str_ext_ret); 

				if(str_shutter.Find(STR__OPEN) != 0)
				{
					err_msg += " * Shutter State ... \n";

					err_bff.Format("  %s <- %s \n", 
									sEXT_CH__PMx_SHUTTER_STS[pm_index]->Get__CHANNEL_NAME(),
									str_shutter);
					err_msg += err_bff;
				}
				if(str_pin.Find(STR__DOWN) != 0)
				{
					err_msg += " * Lift_Pin State ... \n";

					err_bff.Format("  %s <- %s \n", 
									sEXT_CH__PMx_LIFT_PIN_POS_STS[pm_index]->Get__CHANNEL_NAME(),
									str_pin);
					err_msg += err_bff;
				}
				if(str_cr_pos.Find(STR__UP) != 0)
				{
					err_msg += " * CRing State ... \n";

					err_bff.Format("  %s <- %s \n", 
						sEXT_CH__PMx_CR_POSITION_STS[pm_index]->Get__CHANNEL_NAME(),
						str_pin);
					err_msg += err_bff;
				}

				Fnc__APP_LOG(err_msg);
			}

			p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,err_msg,r_act);

			if(r_act.CompareNoCase(CHECK__RETRY) == 0)
			{
				goto LOOP_RETRY;
			}
		}

		return -1;
	}

	return OBJ_AVAILABLE;
}

int CObj__VAC_ROBOT_STD::
Is__PIN_DOWN_STS(CII_OBJECT__ALARM* p_alarm, 
				 const CString str_pick_place,
				 const CString str_ext_ret,
				 const int pm_index)
{
	CString  str_pin;
	int nRtyCnt = 0;

	SCX__TIMER_CTRL xTimer;
	xTimer->REGISTER__ABORT_OBJECT(sObject_Name);


LOOP_RETRY:

	// ...
	int check_count = 10;

	while(1)
	{
		sEXT_CH__PMx_LIFT_PIN_POS_STS[pm_index]->Get__DATA(str_pin);

		if(str_pin.Find(STR__DOWN) == 0)
		{
			break;
		}

		// ...
		{
			check_count--;
			if(check_count < 0)
			{
				break;
			}
		}

		Sleep(100);
	}

	if(iSIM__PMC_DUMMY_MODE < 0)
	{
		sEXT_CH__PMx_LIFT_PIN_POS_STS[pm_index]->Get__DATA(str_pin);
	}
	else
	{
		str_pin = STR__DOWN;
		sEXT_CH__PMx_LIFT_PIN_POS_STS[pm_index]->Set__DATA(str_pin);
	}

	if(str_pin.Find(STR__DOWN) != 0)
	{
		if(xTimer->WAIT(2.0) < 0)
		{
			return OBJ_ABORT;
		}

		// ...
		{
			nRtyCnt++;
			if(nRtyCnt <= 1)
			{
				Fnc__APP_LOG("Goto Retry...");
				goto LOOP_RETRY;
			}
		}

		// ...
		{
			int alarm_id = ALID__PMx__NOT_PIN_DOWN_ERROR;
			CString err_msg;
			CString r_act;

			// ...
			{
				err_msg.Format("During [%s] Action, Before VAC Robot %s, PM%d's PIN[%s] Sts .\n", 
								str_pick_place, 
								str_ext_ret, 
								pm_index+1, 
								str_pin);

				Fnc__APP_LOG(err_msg);
			}

			p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,err_msg,r_act);

			if(r_act.CompareNoCase(CHECK__RETRY) == 0)
			{
				goto LOOP_RETRY;
			}
		}

		return -1;
	}

	return OBJ_AVAILABLE;
}

int  CObj__VAC_ROBOT_STD::
Fnc__TRANSFER_AUTO_PUMPING(const CString& arm_type, const CString& stn_name, const CString& stn_slot)
{
	if(dEXT_CH__CFG_TRANSFER_MODE->Check__DATA(STR__VAC) > 0)
	{
		// VAC_CHM - PUMP
		if(sEXT_CH__VAC_CHM__PRESSURE_STATUS->Check__DATA(STR__VAC) < 0)
		{
			pVAC_CHM__OBJ_CTRL->Call__OBJECT(CMMD__PUMP);
		}

		// LLx - PUMP
		int ll_i = Macro__CHECK_LLx_INDEX(stn_name);
		if(ll_i >= 0)
		{
			if(sEXT_CH__LLx__PRESSURE_STATUS[ll_i]->Check__DATA(STR__VAC) < 0)
			{
				pLLx__OBJ_CTRL[ll_i]->Call__OBJECT(CMMD__TRANSFER_PUMP);
			}
		}
	}
	return 1;
}

int  CObj__VAC_ROBOT_STD::
Fnc__RUN_SV_OPEN(const CString& stn_name)
{
	CString str_log;
	CString para_module;

	para_module = stn_name;

	// ...
	{
		str_log.Format("%s Station, SV OPEN Run Start...", para_module);
		Fnc__APP_LOG(str_log);
	}

	int ll_i = Macro__CHECK_LLx_INDEX(stn_name);
	if(ll_i >= 0)
	{
		CString ll_name = Macro__GET_LLx_NAME(ll_i);

		CString cmmd_ll;
		cmmd_ll.Format("%s_SV_OPEN", ll_name);

		if(pTMC_VLV__OBJ_CTRL->Run__OBJECT(cmmd_ll) < 0)
		{
			str_log.Format("LL%1d_SV_OPEN Set..[RUN] Aborted...", ll_i+1);

			Fnc__APP_LOG(str_log);
			return -102;
		}
	}
	else
	{
		int pm_index = Macro__CHECK_PMx_INDEX(para_module);

		if(pm_index < 0)
		{
			return OBJ_ABORT;
		}

		if(Fnc__Run_PMx_SV_TRANSFER_OPEN(pm_index+1) < 0)
		{
			str_log.Format("PM%d_OPEN Set..[RUN] Aborted...", pm_index+1);
			Fnc__APP_LOG(str_log);
			return -103;
		}	
	}

	return OBJ_AVAILABLE;
}
int  CObj__VAC_ROBOT_STD::
Fnc__WAIT_SV_OPEN(const CString& stn_name)
{
	CString str_log;
	CString para_module;

	para_module = stn_name;

	// ...
	{
		str_log.Format("%s Station, SV OPEN Wait Start...", para_module);
		Fnc__APP_LOG(str_log);
	}

	int ll_i = Macro__CHECK_LLx_INDEX(stn_name);
	if(ll_i >= 0)
	{
		if(pTMC_VLV__OBJ_CTRL->When__OBJECT() < 0)
		{
			str_log.Format("LL%1d_SV_OPEN Wait..[CALL] Aborted...", ll_i+1);

			Fnc__APP_LOG(str_log);
			return -102;
		}
	}
	else
	{
		int pm_index = Macro__CHECK_PMx_INDEX(para_module);

		if(pm_index < 0)
		{
			return OBJ_ABORT;
		}

		if(Fnc__Wait_PMx_SV_TRANSFER_OPEN(pm_index+1) < 0)
		{
			str_log.Format("PM%d_OPEN Wait..[RUN] Aborted...", pm_index+1);
			Fnc__APP_LOG(str_log);
			return -103;
		}		
	}

	// ...
	{
		str_log.Format("%s Station, SV OPEN Wait Completed ...", para_module);
		Fnc__APP_LOG(str_log);
	}
	return OBJ_AVAILABLE;
}
int  CObj__VAC_ROBOT_STD::
Fnc__CALL_SV_OPEN(const CString& stn_name)
{
	CString str_log;
	CString para_module;

	para_module = stn_name;

	// ...
	{
		str_log.Format("%s Station, SV OPEN Call Start...", para_module);
		Fnc__APP_LOG(str_log);
	}

	int ll_i = Macro__CHECK_LLx_INDEX(stn_name);
	if(ll_i >= 0)
	{
		CString ll_name = Macro__GET_LLx_NAME(ll_i);

		CString cmmd_ll;
		cmmd_ll.Format("%s_SV_OPEN", ll_name);

		if(pTMC_VLV__OBJ_CTRL->Call__OBJECT(cmmd_ll) < 0)
		{
			str_log.Format("LL%1d_SV_OPEN Set..[CALL] Aborted...", ll_i+1);
			
			Fnc__APP_LOG(str_log);
			return -102;
		}
	}
	else
	{
		int pm_index = Macro__CHECK_PMx_INDEX(para_module);
		if(pm_index < 0)
		{
			return OBJ_ABORT;
		}

		if(Fnc__PMx_SV_TRANSFER_OPEN(pm_index+1) < 0)
		{
			str_log.Format("PM%d_OPEN Set..[CALL] Aborted...", pm_index+1);
			Fnc__APP_LOG(str_log);
			return -103;
		}

	}

	// ...
	{
		str_log.Format("%s Station, SV OPEN Call Completed ...", para_module);
		Fnc__APP_LOG(str_log);
	}

	return OBJ_AVAILABLE;
}
