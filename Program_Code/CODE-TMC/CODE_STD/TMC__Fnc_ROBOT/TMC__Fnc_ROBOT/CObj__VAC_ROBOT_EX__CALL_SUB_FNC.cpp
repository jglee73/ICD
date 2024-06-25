#include "StdAfx.h"
#include "CObj__VAC_ROBOT_EX.h"
#include "CObj__VAC_ROBOT_EX__DEF.h"
#include "CObj__VAC_ROBOT_EX__ALID.h"

#include "Macro_Function.h"


// ...
void  CObj__VAC_ROBOT_EX::
Fnc__APP_LOG(const CString& log_msg)
{
	xLOG_CTRL->WRITE__LOG(log_msg);
}


// VAC_ROBOT -----
int  CObj__VAC_ROBOT_EX::
Fnc__CALL_ROTATE(CII_OBJECT__VARIABLE* p_variable,
				 CII_OBJECT__ALARM* p_alarm,
				 const CString& para_arm,
				 const CString& para_module,
				 const CString& para_slot)
{
	CStringArray l_para;

	l_para.Add(para_arm);
	l_para.Add(para_module);
	l_para.Add(para_slot);

	return pROBOT__OBJ_CTRL->Call__OBJ_MODE(CMMD__ROTATE,l_para);
}
int  CObj__VAC_ROBOT_EX::
Fnc__RUN_ROTATE(CII_OBJECT__VARIABLE* p_variable,
				CII_OBJECT__ALARM* p_alarm,
				const CString& para_arm,
				const CString& para_module,
				const CString& para_slot)
{
	CStringArray l_para;

	l_para.Add(para_arm);
	l_para.Add(para_module);
	l_para.Add(para_slot);

	return pROBOT__OBJ_CTRL->Run__OBJ_MODE(CMMD__ROTATE,l_para);
}


// ...
int  CObj__VAC_ROBOT_EX::
Fnc__PASSIVE_TRANSFER_READY(const int pm_index)
{
	if(!bActive__PMx_Passive_Use)
	{
		return 1;
	}
	if(dEXT_CH__CFG_PMx_CHM_TYPE[pm_index]->Check__DATA(STR__DUMMY) > 0)
	{
		return 1;
	}

	if(bActive__Single_Handoff)
	{
		CString ch_data;

		ch_data.Format("%1d", pm_index + 1);
		aEXT_CH__PARA_PMC_ID->Set__DATA(ch_data);

		return pOPR_PMx_HANDOFF_CTRL->Call__OBJECT(CMMD__TRANSFER_READY);
	}

	return pOPR_PMC_HANDOFF_X[pm_index]->Call__OBJECT(CMMD__TRANSFER_READY);
}
int  CObj__VAC_ROBOT_EX::
Fnc__PASSIVE_TRANSFER_END(const int pm_index)
{
	if(!bActive__PMx_Passive_Use)
	{
		return 1;
	}
	if(dEXT_CH__CFG_PMx_CHM_TYPE[pm_index]->Check__DATA(STR__DUMMY) > 0)
	{
		return 1;
	}

	if(bActive__Single_Handoff)
	{
		CString ch_data;

		ch_data.Format("%1d", pm_index + 1);
		aEXT_CH__PARA_PMC_ID->Set__DATA(ch_data);

		return pOPR_PMx_HANDOFF_CTRL->Call__OBJECT(CMMD__TRANSFER_END);
	}

	return pOPR_PMC_HANDOFF_X[pm_index]->Call__OBJECT(CMMD__TRANSFER_END);
}

int  CObj__VAC_ROBOT_EX::
Fnc__CALL_PICK(CII_OBJECT__VARIABLE* p_variable,
			   CII_OBJECT__ALARM* p_alarm,
			   const CString& para_arm,
			   const CString& para_module,
			   const CString& para_slot)
{
	int pm_index = Macro__CHECK_PMx_INDEX(para_module);
	if(pm_index >= 0)
	{
		if(dEXT_CH__PMx_CFG_HANDSHAKE_MODE[pm_index]->Check__DATA(STR__PASSIVE) > 0)
		{	
			if(Is__TRANSFER_READY_TO_PICK(p_alarm, "TRANSFER.READY", "PICK", pm_index) < 0)
			{
				return -101;
			}
		}
	}

	// ...
	CStringArray l_para;

	l_para.Add(para_arm);
	l_para.Add(para_module);
	l_para.Add(para_slot);

	return pROBOT__OBJ_CTRL->Call__OBJ_MODE(CMMD__PICK,l_para);
}

int  CObj__VAC_ROBOT_EX::
Fnc__CALL_PLACE(CII_OBJECT__VARIABLE* p_variable,
				CII_OBJECT__ALARM* p_alarm,
				const CString& para_arm,
				const CString& para_module,
				const CString& para_slot)
{
	int pm_index = Macro__CHECK_PMx_INDEX(para_module);
	if(pm_index >= 0)
	{
		if(dEXT_CH__PMx_CFG_HANDSHAKE_MODE[pm_index]->Check__DATA(STR__PASSIVE) > 0)
		{	
			if(Is__TRANSFER_READY_TO_PLACE(p_alarm, false, "TRANSFER.READY", "PLACE", pm_index) < 0)
			{
				return -101;
			}
		}
	}

	// ...
	CStringArray l_para;

	l_para.Add(para_arm);
	l_para.Add(para_module);
	l_para.Add(para_slot);

	return pROBOT__OBJ_CTRL->Call__OBJ_MODE(CMMD__PLACE,l_para);
}


// PMx CONTROL -----
int  CObj__VAC_ROBOT_EX::
Fnc__PMx_SV_CLOSE(const int pmc_id)
{
	if((dEXT_CH__CFG_SETUP_TEST_MODE->Check__DATA(STR__ENABLE) > 0)
	&& (dEXT_CH__CFG_SETUP_LLx_DOOR_SLOT_VLV_CONTROL->Check__DATA(STR__DISABLE) > 0))
	{
		return 1;
	}

	// ...
	CString para__pm_data;
	para__pm_data.Format("%1d", pmc_id);
	
	CStringArray l__pmc_id;
	l__pmc_id.Add(para__pm_data);

	return pTMC_VLV__OBJ_CTRL->Call__OBJ_MODE("PMC_SV_CLOSE", l__pmc_id);
}
int  CObj__VAC_ROBOT_EX::
Run__PMx_SV_CLOSE(const int pmc_id)
{
	if((dEXT_CH__CFG_SETUP_TEST_MODE->Check__DATA(STR__ENABLE) > 0)
	&& (dEXT_CH__CFG_SETUP_LLx_DOOR_SLOT_VLV_CONTROL->Check__DATA(STR__DISABLE) > 0))
	{
		return 1;
	}

	// ...
	CString para__pm_data;
	para__pm_data.Format("%1d", pmc_id);

	CStringArray l__pmc_id;
	l__pmc_id.Add(para__pm_data);

	return pTMC_VLV__OBJ_CTRL->Run__OBJ_MODE("PMC_SV_CLOSE", l__pmc_id);
}

int  CObj__VAC_ROBOT_EX::
Fnc__PMx_SV_TRANSFER_OPEN(const int pmc_id)
{
	if((dEXT_CH__CFG_SETUP_TEST_MODE->Check__DATA(STR__ENABLE) > 0)
	&& (dEXT_CH__CFG_SETUP_LLx_DOOR_SLOT_VLV_CONTROL->Check__DATA(STR__DISABLE) > 0))
	{
		return 1;
	}

	// ...
	CString para__pm_id;
	para__pm_id.Format("%1d", pmc_id);

	CStringArray l__pmc_id;
	l__pmc_id.Add(para__pm_id);

	return pTMC_VLV__OBJ_CTRL->Call__OBJ_MODE("PMC_SV_TRANSFER_OPEN", l__pmc_id);
}
int  CObj__VAC_ROBOT_EX::
Fnc__Run_PMx_SV_TRANSFER_OPEN(const int pmc_id)
{
	if((dEXT_CH__CFG_SETUP_TEST_MODE->Check__DATA(STR__ENABLE) > 0)
	&& (dEXT_CH__CFG_SETUP_LLx_DOOR_SLOT_VLV_CONTROL->Check__DATA(STR__DISABLE) > 0))
	{
		return 1;
	}

	// ...
	CString para__pm_id;
	para__pm_id.Format("%1d", pmc_id);

	CStringArray l__pmc_id;
	l__pmc_id.Add(para__pm_id);

	return pTMC_VLV__OBJ_CTRL->Run__OBJ_MODE("PMC_SV_TRANSFER_OPEN", l__pmc_id);
}
int  CObj__VAC_ROBOT_EX::
Fnc__Wait_PMx_SV_TRANSFER_OPEN(const int pmc_id)
{	

	return pTMC_VLV__OBJ_CTRL->When__OBJECT();
}

int  CObj__VAC_ROBOT_EX::
Fnc__PMx_SV_CLOSE_CHECK(CII_OBJECT__ALARM* p_alarm)
{
LOOP_RETRY:

	// ...
	pTMC_VLV__OBJ_CTRL->Call__OBJECT("CHECK_PMC_SV_ALL_CLOSE");

	// ...
	CStringArray l_err_msg;
	pTMC_VLV__OBJ_CTRL->Get__RETURN_FNC_PARAMETER(l_err_msg);

	int limit = l_err_msg.GetSize();
	int i;

	if(limit < 1)		return 1;

	// ...
	{
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

		p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,alarm_msg,r_act);	

		if(r_act.CompareNoCase("RETRY") == 0)
		{
			pTMC_VLV__OBJ_CTRL->Call__OBJECT("PMC_ALL_SV_CLOSE");
			goto LOOP_RETRY;
		}
	}
	return -1;
}

int  CObj__VAC_ROBOT_EX::
Sub_OBJCALL_PMC_ACT_HANDOFF(const int pmc_index, const CString obj_mode)
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
		if(bActive__Single_Handoff)
		{
			double pmc_id = pmc_index + 1.0;
			aEXT_CH__PARA_PMC_ID->Set__VALUE(pmc_id);

			int retVal = pOPR_PMx_HANDOFF_CTRL->Call__OBJECT(obj_mode);
			if(retVal != OBJ_AVAILABLE)
			{
				str_log.Format("[Call__ACT_HOFF] TMC->PM%1d Call Mode: %s Failed, retVal : %d", pmc_index+1, obj_mode, retVal);
				Fnc__APP_LOG(str_log);

				szMessage.Format("%s Call Failed.....", obj_mode);
				sCH__OBJ_MESSAGE->Set__DATA(szMessage);
				return retVal;
			}
		}
		else
		{
			int retVal = pOPR_PMC_HANDOFF_X[pmc_index]->Call__OBJECT(obj_mode);
			if(retVal != OBJ_AVAILABLE)
			{
				str_log.Format("[Call__ACT_HOFF] TMC->PM%1d Call Mode: %s Failed, retVal : %d", pmc_index+1, obj_mode, retVal);
				Fnc__APP_LOG(str_log);

				szMessage.Format("%s Call Failed.....", obj_mode);
				sCH__OBJ_MESSAGE->Set__DATA(szMessage);
				return retVal;
			}
		}
	}

	szMessage.Format("%s Call End.....", obj_mode);
	sCH__OBJ_MESSAGE->Set__DATA(szMessage);
	return 1;
}

int  CObj__VAC_ROBOT_EX::
Sub_OBJRUN_PMC_ACT_HANDOFF(const int pmc_index, const CString obj_mode)
{
	CString szMessage;
	CString str_log;

	str_log.Format("[Run__ACT_HOFF] TMC -> PMC Call Mode: %s", obj_mode);
	Fnc__APP_LOG(str_log);

	szMessage.Format("%s Call Start.....", obj_mode);
	sCH__OBJ_MESSAGE->Set__DATA(szMessage);

	if(iSIM__PMC_DUMMY_MODE < 0)
	{
		if(bActive__Single_Handoff)
		{
			double pmc_id = pmc_index + 1.0;
			aEXT_CH__PARA_PMC_ID->Set__VALUE(pmc_id);

			int retVal = pOPR_PMx_HANDOFF_CTRL->Run__OBJECT(obj_mode);
			if(retVal != OBJ_AVAILABLE)
			{
				str_log.Format("[Run__ACT_HOFF] TMC->PM[%1d] Call Mode: %s Failed, retVal : %d", pmc_index+1, obj_mode, retVal);
				Fnc__APP_LOG(str_log);

				szMessage.Format("%s Call Failed.....", obj_mode);
				sCH__OBJ_MESSAGE->Set__DATA(szMessage);
				return retVal;
			}
		}
		else
		{
			int retVal = pOPR_PMC_HANDOFF_X[pmc_index]->Run__OBJECT(obj_mode);
			if(retVal != OBJ_AVAILABLE)
			{
				str_log.Format("[Run__ACT_HOFF] TMC->PMC Call Mode: %s Failed, retVal : %d", obj_mode, retVal);
				Fnc__APP_LOG(str_log);

				szMessage.Format("%s Call Failed.....", obj_mode);
				sCH__OBJ_MESSAGE->Set__DATA(szMessage);
				return retVal;
			}
		}
	}

	szMessage.Format("%s Call End.....", obj_mode);
	sCH__OBJ_MESSAGE->Set__DATA(szMessage);
	return 1l;
}

int  CObj__VAC_ROBOT_EX::
Fnc__ACT_HOFF_PICK(CII_OBJECT__VARIABLE* p_variable,
				   CII_OBJECT__ALARM* p_alarm,
				   const CString& arm_type,
				   const CString& stn_name,
				   const CString& stn_slot)
{
	Fnc__APP_LOG("Pick Active HandOff Start...");

	// ...
	CStringArray l__arm_type;
	CStringArray l__stn_name;
	CStringArray l__stn_slot;

	_Get__ARM_INFO(arm_type,stn_name,stn_slot, l__arm_type,l__stn_name,l__stn_slot);

	// ...
	CString str_log;

	if(iSIM__PMC_DUMMY_MODE < 0)
	{
		Fnc__APP_LOG("[Fnc__ACT_HOFF_PICK] : ACTIVE HANDOFF PMx CTRL is AVAILABLE");

		int k_limit = l__stn_name.GetSize();

		for(int k=0; k<k_limit; k++)
		{
			CString cur__stn_name = l__stn_name[k];

			int p_index = Macro__CHECK_PMx_INDEX(cur__stn_name);
			if(p_index <  0)				return -101;
			if(p_index >= iSIZE__PMx)		return -102;

			if(bActive__Single_Handoff)
			{
				CString pm_status;
				pOPR_PMx_HANDOFF_CTRL->Get__OBJECT_STATUS(pm_status);

				str_log.Format("[Fnc__ACT_HOFF_PICK] : PMC_HANDOFF Status : %s", pm_status);
				Fnc__APP_LOG(str_log);

				if(pm_status.CompareNoCase("AVAILABLE") != 0)
				{
					int pm_id = p_index + 1;
	
					CString para_id;
					para_id.Format("%1d", pm_id);

					CStringArray l__para_id;
					l__para_id.Add(para_id);

					pOPR_PMx_HANDOFF_CTRL->Call__OBJ_MODE("INIT", l__para_id);
				};
	
				// ...
				int r_flag = pOPR_PMx_HANDOFF_CTRL->When__OBJECT();

				str_log.Format("[Fnc__ACT_HOFF_PICK] TMC->PMC Call Mode: When retVal : %d", r_flag);
				Fnc__APP_LOG(str_log);

				if(r_flag < 0)
				{
					str_log.Format("[Fnc__ACT_HOFF_PICK] TMC->PMC Call Mode: When retVal : -1");
					Fnc__APP_LOG(str_log);
					return -1;	
				}
			}
			else
			{
				CString pm_status;
				pOPR_PMC_HANDOFF_X[p_index]->Get__OBJECT_STATUS(pm_status);
	
				str_log.Format("[Fnc__ACT_HOFF_PICK] : PMC_HANDOFF Status : %s", pm_status);
				Fnc__APP_LOG(str_log);

				if(pm_status.CompareNoCase("AVAILABLE") != 0)
				{
					pOPR_PMC_HANDOFF_X[p_index]->Call__OBJECT("INIT");
				}

				// ...
				int r_flag = pOPR_PMC_HANDOFF_X[p_index]->When__OBJECT();
	
				str_log.Format("[Fnc__ACT_HOFF_PICK] TMC->PMC Call Mode: When retVal : %d", r_flag);
				Fnc__APP_LOG(str_log);

				if(r_flag < 0)
				{
					str_log.Format("[Fnc__ACT_HOFF_PICK] TMC->PMC Call Mode: When retVal : -1");
					Fnc__APP_LOG(str_log);
					return -1;	
				}
			}
		}
	}

	// S1 : PIN UP....
	{
		int k_limit = l__stn_name.GetSize();

		for(int k=0; k<k_limit; k++)
		{
			CString cur__stn_name = l__stn_name[k];
			int p_index = Macro__CHECK_PMx_INDEX(cur__stn_name);

			if(p_index <  0)				return -101;
			if(p_index >= iSIZE__PMx)		return -102;

			if(Sub_OBJCALL_PMC_ACT_HANDOFF(p_index, "S1") < 0)	
			{
				return -11;
			}
		}
	}

	// PM : CHECK ...
	{
		int k_limit = l__stn_name.GetSize();

		for(int k=0; k<k_limit; k++)
		{
			CString cur__stn_name = l__stn_name[k];
			int p_index = Macro__CHECK_PMx_INDEX(cur__stn_name);

			if(p_index <  0)				return -101;
			if(p_index >= iSIZE__PMx)		return -102;

			if(Is__TRANSFER_READY_TO_PICK(p_alarm, "PICK", "EXTEND", p_index) < 0)
			{
				return -12;
			}
		}
	}

	/*
	// ROBOT: EXTEND
	{
		int k_limit = l__arm_type.GetSize();

		for(int k=0; k<k_limit; k++)
		{
			CString cur__arm_type = l__arm_type[k];
			CString cur__stn_name = l__stn_name[k];
			CString cur__stn_slot = l__stn_slot[k];

			// ...
			CStringArray l_para;

			l_para.Add(cur__arm_type);
			l_para.Add(cur__stn_name);
			l_para.Add(cur__stn_slot);

			if(pROBOT__OBJ_CTRL->Call__OBJ_MODE(CMMD__EXTEND, l_para) < 0)
			{
				return -14;
			}
		}
	}
	*/
	// ROBOT: EXTEND
	{
		CStringArray l_para;

		l_para.Add(arm_type);
		l_para.Add(stn_name);
		l_para.Add(stn_slot);

		if(pROBOT__OBJ_CTRL->Call__OBJ_MODE(CMMD__EXTEND, l_para) < 0)
		{
			return -14;
		}
	}

	// S2 : PIN DOWN ...
	{
		int k_limit = l__stn_name.GetSize();

		for(int k=0; k<k_limit; k++)
		{
			CString cur__stn_name = l__stn_name[k];
			int p_index = Macro__CHECK_PMx_INDEX(cur__stn_name);

			if(p_index <  0)				return -101;
			if(p_index >= iSIZE__PMx)		return -102;

			if(Sub_OBJCALL_PMC_ACT_HANDOFF(p_index, "S2") < 0)			
			{
				return -15;
			}
		}
	}

	// CHECK : PIN DOWN ...
	{
		int k_limit = l__stn_name.GetSize();

		for(int k=0; k<k_limit; k++)
		{
			CString cur__stn_name = l__stn_name[k];
			int p_index = Macro__CHECK_PMx_INDEX(cur__stn_name);

			if(p_index <  0)				return -101;
			if(p_index >= iSIZE__PMx)		return -102;

			if(Is__PIN_DOWN_OR_MIDDLE_STS(p_alarm, "PICK", "RETRACT", p_index) < 0)
			{
				return -16;
			}	
		}
	}

	Fnc__CHANGE_MATERIAL_INFO(-1,arm_type,stn_name,stn_slot);

	// ROBOT: RETRACT
	{
		CStringArray l_para;

		l_para.Add(arm_type);
		l_para.Add(stn_name);
		l_para.Add(stn_slot);

		dEXT_CH__PHY_ROBOT__PARA_ACTIVE_HANDOFF_REQ->Set__DATA(STR__ON);
		dEXT_CH__PHY_ROBOT__PARA_ACTIVE_HANDOFF_ACT->Set__DATA(STR__PICK);

		if(pROBOT__OBJ_CTRL->Call__OBJ_MODE(CMMD__RETRACT, l_para) < 0)
		{
			return -22;
		}
	}

	return 1;
}

int  CObj__VAC_ROBOT_EX::
Fnc__ACT_HOFF_PLACE(CII_OBJECT__VARIABLE* p_variable,
					CII_OBJECT__ALARM* p_alarm,
					const CString& arm_type,
					const CString& stn_name,
					const CString& stn_slot)
{
	Fnc__APP_LOG("Place Active HandOff Start...");

	// ...
	CStringArray l__arm_type;
	CStringArray l__stn_name;
	CStringArray l__stn_slot;

	_Get__ARM_INFO(arm_type,stn_name,stn_slot, l__arm_type,l__stn_name,l__stn_slot);

	// ...
	CString str_log;

	// PMx : HANDOFF INIT ...
	if(iSIM__PMC_DUMMY_MODE < 0)
	{
		Fnc__APP_LOG("[Fnc__ACT_HOFF_PLACE] : ACTIVE HANDOFF PMx CTRL is AVAILABLE");

		int k_limit = l__stn_name.GetSize();

		for(int k=0; k<k_limit; k++)
		{
			CString cur__stn_name = l__stn_name[k];

			int p_index = Macro__CHECK_PMx_INDEX(cur__stn_name);
			if(p_index < 0)			return -11;

			if(bActive__Single_Handoff)
			{
				CString pm_status;
				pOPR_PMx_HANDOFF_CTRL->Get__OBJECT_STATUS(pm_status);

				str_log.Format("[Fnc__ACT_HOFF_PLACE] : PMC_HANDOFF Status : %s", pm_status);
				Fnc__APP_LOG(str_log);

				if(pm_status.CompareNoCase("AVAILABLE") != 0)
				{
					int pmc_id = p_index + 1;
					aEXT_CH__PARA_PMC_ID->Set__VALUE(pmc_id);

					pOPR_PMx_HANDOFF_CTRL->Call__OBJECT("INIT");
				}	

				// ...
				int r_flag = pOPR_PMx_HANDOFF_CTRL->When__OBJECT();

				str_log.Format("[Fnc__ACT_HOFF_PLACE] TMC->PMC Call Mode: When retVal : %d", r_flag);
				Fnc__APP_LOG(str_log);

				if(r_flag < 0)
				{
					str_log.Format("[Fnc__ACT_HOFF_PLACE] TMC->PMC Call Mode: When retVal : -1");
					Fnc__APP_LOG(str_log);
					return -12;	
				}
			}
			else
			{
				CString pm_status;
				pOPR_PMC_HANDOFF_X[p_index]->Get__OBJECT_STATUS(pm_status);

				str_log.Format("[Fnc__ACT_HOFF_PLACE] : PMC_HANDOFF Status : %s", pm_status);
				Fnc__APP_LOG(str_log);

				if(pm_status.CompareNoCase("AVAILABLE") != 0)
				{
					pOPR_PMC_HANDOFF_X[p_index]->Call__OBJECT("INIT");
				}	

				// ...
				int r_flag = pOPR_PMC_HANDOFF_X[p_index]->When__OBJECT();

				str_log.Format("[Fnc__ACT_HOFF_PLACE] TMC->PMC Call Mode: When retVal : %d", r_flag);
				Fnc__APP_LOG(str_log);

				if(r_flag < 0)
				{
					str_log.Format("[Fnc__ACT_HOFF_PLACE] TMC->PMC Call Mode: When retVal : -1");
					Fnc__APP_LOG(str_log);
					return -13;	
				}
			}
		}
	}

	// PMx : CHECK 
	{
		int k_limit = l__stn_name.GetSize();

		for(int k=0; k<k_limit; k++)
		{
			CString cur__stn_name = l__stn_name[k];

			int p_index = Macro__CHECK_PMx_INDEX(cur__stn_name);
			if(p_index < 0)			return -11;

			if(Is__TRANSFER_READY_TO_PLACE(p_alarm, true, "PLACE", "EXTEND", p_index) < 0)
			{
				return -101;
			}
		}
	}

	/*
	// ROBOT : EXTEND
	{
		int k_limit = l__arm_type.GetSize();

		for(int k=0; k<k_limit; k++)
		{
			CString cur__arm_type = l__arm_type[k];
			CString cur__stn_name = l__stn_name[k];
			CString cur__stn_slot = l__stn_slot[k];

			int p_index = Macro__CHECK_PMx_INDEX(cur__stn_name);
			if(p_index < 0)			return -111;

			// ...
			CStringArray l__cmmd_para;

			l__cmmd_para.Add(cur__arm_type);
			l__cmmd_para.Add(cur__stn_name);
			l__cmmd_para.Add(cur__stn_slot);

			if(pROBOT__OBJ_CTRL->Call__OBJ_MODE(CMMD__EXTEND, l__cmmd_para) < 0)
			{
				return -112;
			}
		}
	}
	*/
	// ROBOT : EXTEND
	{
		CStringArray l__cmmd_para;

		l__cmmd_para.Add(arm_type);
		l__cmmd_para.Add(stn_name);
		l__cmmd_para.Add(stn_slot);

		if(pROBOT__OBJ_CTRL->Call__OBJ_MODE(CMMD__EXTEND, l__cmmd_para) < 0)
		{
			return -112;
		}
	}

	// R2 : PIN UP
	{
		int k_limit = l__stn_name.GetSize();

		for(int k=0; k<k_limit; k++)
		{
			CString cur__stn_name = l__stn_name[k];

			int p_index = Macro__CHECK_PMx_INDEX(cur__stn_name);
			if(p_index < 0)			return -121;

			if(Sub_OBJCALL_PMC_ACT_HANDOFF(p_index, "R2") < 0)
			{
				return -122;
			}
		}
	}

	// PMx : CHECK 
	{
		int k_limit = l__stn_name.GetSize();

		for(int k=0; k<k_limit; k++)
		{
			CString cur__stn_name = l__stn_name[k];

			int p_index = Macro__CHECK_PMx_INDEX(cur__stn_name);
			if(p_index < 0)			return -131;

			if(Is__PIN_UP_STS(p_alarm, "PLACE", "RETRACT", p_index) < 0)
			{
				return -132;
			}
		}
	}

	Fnc__CHANGE_MATERIAL_INFO(1,arm_type,stn_name,stn_slot);

	// ROBOT : RETRACT
	{
		CStringArray l__cmmd_para;

		l__cmmd_para.Add(arm_type);
		l__cmmd_para.Add(stn_name);
		l__cmmd_para.Add(stn_slot);

		dEXT_CH__PHY_ROBOT__PARA_ACTIVE_HANDOFF_REQ->Set__DATA(STR__ON);
		dEXT_CH__PHY_ROBOT__PARA_ACTIVE_HANDOFF_ACT->Set__DATA(STR__PLACE);

		if(pROBOT__OBJ_CTRL->Call__OBJ_MODE(CMMD__RETRACT, l__cmmd_para) < 0)
		{
			return -142;
		}
	}

	return 1;
}

int  CObj__VAC_ROBOT_EX::
Is__TRANSFER_READY_TO_PICK(CII_OBJECT__ALARM* p_alarm, 
						   const CString str_pick_place,
						   const CString str_ext_ret,
						   const int pm_index)
{
	CString str_shutter;
	CString str_pin;
	CString str_cr_pos;

LOOP_RETRY:

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

	// ...
	int check_count = 100;

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
			return 1;
		}

		check_count--;
		if(check_count < 0)
		{
			break;
		}

		Sleep(20);
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

int  CObj__VAC_ROBOT_EX::
Is__PIN_UP_STS(CII_OBJECT__ALARM* p_alarm, 
			   const CString str_pick_place,
			   const CString str_ext_ret,
			   const int pm_index)
{
	CString str_pin;

LOOP_RETRY:

	if(iSIM__PMC_DUMMY_MODE < 0)
	{
		sEXT_CH__PMx_LIFT_PIN_POS_STS[pm_index]->Get__DATA(str_pin);
	}
	else
	{
		str_pin = STR__UP;
		sEXT_CH__PMx_LIFT_PIN_POS_STS[pm_index]->Set__DATA(str_pin);
	}

	// ...
	int check_count = 100;

	while(1)
	{
		sEXT_CH__PMx_LIFT_PIN_POS_STS[pm_index]->Get__DATA(str_pin);

		if(str_pin.Find(STR__UP) == 0)
		{
			return 1;
		}

		check_count--;
		if(check_count < 0)
		{
			break;
		}

		Sleep(20);
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

int  CObj__VAC_ROBOT_EX
::Is__TRANSFER_READY_TO_PLACE(CII_OBJECT__ALARM* p_alarm, 
							  const bool active__active_handoff,
							  const CString str_pick_place,
							  const CString str_ext_ret,
							  const int pm_index)
{
	CString str_shutter;
	CString str_pin;
	CString str_cr_pos;

LOOP_RETRY:

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
			if(active__active_handoff)		str_pin = STR__DOWN;
			else							str_pin = STR__UP;
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

	// ...
	int check_count = 100;

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

			if(active__active_handoff)
			{
				if((str_pin.Find(STR__DOWN)   != 0)
				&& (str_pin.Find(STR__MIDDLE) != 0))
				{
					ok_flag = -1;
				}
			}
			else
			{
				if(str_pin.Find(STR__UP) != 0)
				{
					ok_flag = -1;
				}
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
			return 1;
		}

		check_count--;
		if(check_count < 0)
		{
			break;
		}

		Sleep(20);
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

int CObj__VAC_ROBOT_EX::
Is__PIN_DOWN_OR_MIDDLE_STS(CII_OBJECT__ALARM* p_alarm, 
						   const CString str_pick_place,
						   const CString str_ext_ret,
						   const int pm_index)
{
	CString  str_pin;

LOOP_RETRY:

	if(iSIM__PMC_DUMMY_MODE < 0)
	{
		sEXT_CH__PMx_LIFT_PIN_POS_STS[pm_index]->Get__DATA(str_pin);
	}
	else
	{
		str_pin = STR__DOWN;
		sEXT_CH__PMx_LIFT_PIN_POS_STS[pm_index]->Set__DATA(str_pin);
	}

	// ...
	int check_count = 100;

	while(1)
	{
		sEXT_CH__PMx_LIFT_PIN_POS_STS[pm_index]->Get__DATA(str_pin);

		if((str_pin.Find(STR__DOWN)   == 0)
		|| (str_pin.Find(STR__MIDDLE) == 0))
		{
			return 1;
		}

		check_count--;
		if(check_count < 0)
		{
			break;
		}

		Sleep(20);
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

int  CObj__VAC_ROBOT_EX
::Fnc__TRANSFER_AUTO_PUMPING(const CString& arm_type, const CString& stn_name, const CString& stn_slot)
{
	if(dEXT_CH__CFG_TRANSFER_MODE->Check__DATA(STR__VAC) > 0)
	{
		// VAC_CHM - PUMP
		if(sEXT_CH__VAC_CHM__PRESSURE_STATUS->Check__DATA(STR__VAC) < 0)
		{
			dEXT_CH__PARA_BALLAST_CTRL_ACTIVE->Set__DATA(STR__ON);

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
	else if(dEXT_CH__CFG_TRANSFER_MODE->Check__DATA(STR__ATM) > 0)
	{
		// LLx - VENT
		int ll_i = Macro__CHECK_LLx_INDEX(stn_name);
		if(ll_i >= 0)
		{
			if(sEXT_CH__LLx__PRESSURE_STATUS[ll_i]->Check__DATA(STR__ATM) < 0)
			{
				pLLx__OBJ_CTRL[ll_i]->Call__OBJECT(CMMD__VENT);
			}
		}

		// VAC_CHM - VENT
		if(sEXT_CH__VAC_CHM__PRESSURE_STATUS->Check__DATA(STR__ATM) < 0)
		{
			pVAC_CHM__OBJ_CTRL->Call__OBJECT(CMMD__VENT);
		}
	}
	return 1;
}

int  CObj__VAC_ROBOT_EX::
Fnc__RUN_SV_OPEN(const CString& arm_type, const CString& stn_name, const CString& stn_slot)
{
	CString str_log;
	CString str_bff;

	// ...
	CStringArray l__arm_type;
	CStringArray l__stn_name;
	CStringArray l__stn_slot;

	_Get__ARM_INFO(arm_type,stn_name,stn_slot, l__arm_type,l__stn_name,l__stn_slot);

	// ...
	{
		str_log.Format("Arm(%s) & %s(%s) Station, SV OPEN Run Start...", arm_type,stn_name,stn_slot);
		Fnc__APP_LOG(str_log);
	}

	// LLx ...
	int ll_i = Macro__CHECK_LLx_INDEX(stn_name);
	if(ll_i >= 0)
	{
		if(bActive__LLx_MANIFOLD_X)
		{
			if(ll_i < iSIZE__LLx)
			{
				int k_limit = l__stn_slot.GetSize();

				for(int k=0; k<k_limit; k++)
				{
					CString cmmd__sv_open = "SV_TRANSFER.OPEN";
					CString cur__stn_slot = l__stn_slot[k];

					CStringArray l__cmmd_para;
					l__cmmd_para.Add(cur__stn_slot);

					// ...
					{
						str_log = "\n";

						str_bff.Format("%s : [RUN] Start ... \n", cmmd__sv_open);
						str_log += str_bff;

						str_bff.Format(" * ll_id <- [%1d] \n", ll_i+1);
						str_log += str_bff;

						str_bff.Format(" * cur__stn_slot <- [%s] \n", cur__stn_slot);
						str_log += str_bff;

						Fnc__APP_LOG(str_log);
					}

					if(pLLx__OBJ_CTRL[ll_i]->Run__OBJ_MODE(cmmd__sv_open, l__cmmd_para) < 0)
					{
						str_log = "\n";

						str_bff.Format("%s : [RUN] Aborted ... \n", cmmd__sv_open);
						str_log += str_bff;

						str_bff.Format(" * ll_id <- [%1d] \n", ll_i+1);
						str_log += str_bff;
						
						str_bff.Format(" * cur__stn_slot <- [%s] \n", cur__stn_slot);
						str_log += str_bff;

						Fnc__APP_LOG(str_log);
						return -301;
					}
				}
				return 1;
			}
			return -303;
		}
		else
		{
			CString ll_name = Macro__GET_LLx_NAME(ll_i);

			CString cmmd__sv_open;
			cmmd__sv_open.Format("%s_SV_OPEN", ll_name);

			if(pTMC_VLV__OBJ_CTRL->Run__OBJECT(cmmd__sv_open) < 0)
			{
				str_log = "\n";

				str_bff.Format("%s : [RUN] Aborted ... \n", cmmd__sv_open);
				str_log += str_bff;

				str_bff.Format(" * ll_name <- [%s] \n", ll_name);
				str_log += str_bff;

				Fnc__APP_LOG(str_log);
				return -101;
			}
			return 1;
		}
	}

	// PMX ...
	if(Macro__CHECK_PMx_INDEX(stn_name) >= 0)
	{
		int k_limit = l__stn_name.GetSize();

		for(int k=0; k<k_limit; k++)
		{
			CString cur__stn_name = l__stn_name[k];

			int pm_i = Macro__CHECK_PMx_INDEX(cur__stn_name);
			if(pm_i >= 0)
			{
				int pm_id = pm_i + 1;

				if(Fnc__Run_PMx_SV_TRANSFER_OPEN(pm_id) < 0)
				{
					str_log = "\n";

					str_bff.Format("PM%d : OPEN [RUN] Aborted ... \n", pm_id);
					str_log += str_bff;

					Fnc__APP_LOG(str_log);
					return -111;
				}	
			}
		}
		return 2;
	}

	return -1;
}
int  CObj__VAC_ROBOT_EX::
Fnc__WAIT_SV_OPEN(const CString& arm_type, const CString& stn_name, const CString& stn_slot)
{
	CString str_log;
	CString str_bff;

	// ...
	CStringArray l__arm_type;
	CStringArray l__stn_name;
	CStringArray l__stn_slot;

	_Get__ARM_INFO(arm_type,stn_name,stn_slot, l__arm_type,l__stn_name,l__stn_slot);

	// ...
	{
		str_log.Format("%s(%s) : SV OPEN Wait Start ...", stn_name,stn_slot);
		Fnc__APP_LOG(str_log);
	}

	int ll_i = Macro__CHECK_LLx_INDEX(stn_name);
	if(ll_i >= 0)
	{
		if(bActive__LLx_MANIFOLD_X)
		{
			if(ll_i < iSIZE__LLx)
			{
				if(pLLx__OBJ_CTRL[ll_i]->When__OBJECT() < 0)
				{
					str_log.Format("LL%1d_SV_OPEN Wait..[CALL] Aborted...", ll_i+1);
					Fnc__APP_LOG(str_log);
					return -101;
				}
				return 1;
			}
			return -1001;
		}
		else
		{
			if(pTMC_VLV__OBJ_CTRL->When__OBJECT() < 0)
			{
				str_log.Format("LL%1d_SV_OPEN Wait..[CALL] Aborted...", ll_i+1);
				Fnc__APP_LOG(str_log);
				return -102;
			}
		}
	}
	else
	{
		int k_limit = l__stn_name.GetSize();
		int k;

		for(k=0; k<k_limit; k++)
		{
			CString cur__stn_name = l__stn_name[k];

			int pm_index = Macro__CHECK_PMx_INDEX(cur__stn_name);
			if(pm_index < 0)		return -201;

			// ...
			int pm_id = pm_index + 1;

			if(Fnc__Wait_PMx_SV_TRANSFER_OPEN(pm_id) < 0)
			{
				str_log.Format("PM%d_OPEN : Wait Aborted...", pm_id);
				Fnc__APP_LOG(str_log);
				return -103;
			}
		}
	}

	// ...
	{
		str_log.Format("%s Station : SV OPEN Wait Completed ...", stn_name);
		Fnc__APP_LOG(str_log);
	}
	return 1;
}
int  CObj__VAC_ROBOT_EX::
Fnc__CALL_SV_OPEN(const CString& arm_type, const CString& stn_name, const CString& stn_slot)
{
	CString str_log;
	CString str_bff;

	// ...
	CStringArray l__arm_type;
	CStringArray l__stn_name;
	CStringArray l__stn_slot;

	_Get__ARM_INFO(arm_type,stn_name,stn_slot, l__arm_type,l__stn_name,l__stn_slot);

	// ...
	{
		str_log.Format("%s(%s) : SV OPEN Call Start...", stn_name,stn_slot);
		Fnc__APP_LOG(str_log);
	}

	int ll_i = Macro__CHECK_LLx_INDEX(stn_name);
	if(ll_i >= 0)
	{
		if(bActive__LLx_MANIFOLD_X)
		{
			if(ll_i < iSIZE__LLx)
			{
				int k_limit = l__stn_slot.GetSize();
				int k;

				for(k=0; k<k_limit; k++)
				{
					CString cmmd__sv_open = "SV_TRANSFER.OPEN";
					CString cur__stn_slot = l__stn_slot[k];

					aEXT_CH__LLx__PARA_SLOT_ID[ll_i]->Set__DATA(cur__stn_slot);

					if(pLLx__OBJ_CTRL[ll_i]->Call__OBJECT(cmmd__sv_open) < 0)
					{
						str_bff.Format("%s : [CALL] Aborted ... \n", cmmd__sv_open);
						str_log += str_bff;

						str_bff.Format(" * ll_id <- [%1d] \n", ll_i+1);
						str_log += str_bff;

						str_bff.Format(" * cur__stn_slot <- [%s] \n", cur__stn_slot);
						str_log += str_bff;

						Fnc__APP_LOG(str_log);
						return -301;
					}
					return 1;
				}
			}
			return -303;
		}
		else
		{
			CString ll_name = Macro__GET_LLx_NAME(ll_i);

			CString cmmd__sv_open;
			cmmd__sv_open.Format("%s_SV_OPEN", ll_name);

			if(pTMC_VLV__OBJ_CTRL->Call__OBJECT(cmmd__sv_open) < 0)
			{
				str_log.Format("%s Set..[CALL] Aborted ...", cmmd__sv_open);
				Fnc__APP_LOG(str_log);
				return -102;
			}
		}
	}
	else
	{
		int k_limit = l__stn_name.GetSize();
		int k;

		for(k=0; k<k_limit; k++)
		{
			CString cur__stn_name = l__stn_name[k];

			int pm_index = Macro__CHECK_PMx_INDEX(cur__stn_name);
			if(pm_index < 0)		return -201;

			int pm_id = pm_index + 1;

			if(Fnc__PMx_SV_TRANSFER_OPEN(pm_id) < 0)
			{
				str_log.Format("PM%d : OPEN [CALL] Aborted...", pm_id);
				Fnc__APP_LOG(str_log);
				return -202;
			}
		}
	}

	// ...
	{
		str_log.Format("%s(%s) : SV OPEN - Call Completed ...", stn_name,stn_slot);
		Fnc__APP_LOG(str_log);
	}
	return 1;
}

// ...
void CObj__VAC_ROBOT_EX
::_Get__ARM_INFO(const CString& arm_type,
				 const CString& stn_name,
				 const CString& stn_slot,
				 CStringArray& l__arm_type,
				 CStringArray& l__stn_name,
				 CStringArray& l__stn_slot)
{
	// ...
	{
		l__arm_type.RemoveAll();
		l__stn_name.RemoveAll();
		l__stn_slot.RemoveAll();
	}

	if(arm_type.CompareNoCase(_ARM_AB) == 0)
	{
		CString ch_data;

		int ll_i = Macro__CHECK_LLx_INDEX(stn_name);
		int pm_i = Macro__CHECK_PMx_INDEX(stn_name);

		if(ll_i >= 0)
		{
			l__arm_type.Add(_ARM_A);
			l__stn_name.Add(stn_name);
			l__stn_slot.Add("1");

			l__arm_type.Add(_ARM_B);
			l__stn_name.Add(stn_name);
			l__stn_slot.Add("2");
		}
		else if(pm_i >= 0)
		{
			int pm_id = ((pm_i / 2) * 2) + 1;
			int id_left  = pm_id;
			int id_right = pm_id + 1;

			l__arm_type.Add(_ARM_A);
			ch_data.Format("PM%1d", id_left);
			l__stn_name.Add(ch_data);
			l__stn_slot.Add("1");

			l__arm_type.Add(_ARM_B);
			ch_data.Format("PM%1d", id_right);
			l__stn_name.Add(ch_data);
			l__stn_slot.Add("1");
		}
		else
		{
			l__arm_type.Add(arm_type);
			l__stn_name.Add(stn_name);
			l__stn_slot.Add(stn_slot);
		}
	}
	else
	{
		l__arm_type.Add(arm_type);
		l__stn_name.Add(stn_name);
		l__stn_slot.Add(stn_slot);
	}
}
