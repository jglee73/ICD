#include "StdAfx.h"
#include "CObj__DRV_LINK_ALx.h"
#include "CObj__DRV_LINK_ALx__DEF.h"
#include "CObj__DRV_LINK_ALx__ALID.h"


//--------------------------------------------------------------------------------
CObj__DRV_LINK_ALx::CObj__DRV_LINK_ALx()
{

}
CObj__DRV_LINK_ALx::~CObj__DRV_LINK_ALx()
{

}

//--------------------------------------------------------------------------------
int CObj__DRV_LINK_ALx::__DEFINE__CONTROL_MODE(obj,l_mode)
{
	// ...
	{
		sObject_Name = obj;
	}

	// ...
	{
		ADD__CTRL_VAR(sMODE__INIT,    "INIT");	
		ADD__CTRL_VAR(sMODE__HOME,	  "HOME");

		ADD__CTRL_VAR(sMODE__ALIGN,	  "ALGN");
		
		ADD__CTRL_VAR(sMODE__ROTATE,  "ROTATE");
		ADD__CTRL_VAR(sMODE__SCAN,	  "SCAN");
	}
	return 1;
}
int CObj__DRV_LINK_ALx::__DEFINE__VERSION_HISTORY(version)
{
	version = "JG.Lee \n";

	return 1;
}


// ...
#define  MON_ID__IO_MONITOR									1

// ...
#define APP_DSP__DSlot										\
"1  2"


int CObj__DRV_LINK_ALx::__DEFINE__VARIABLE_STD(p_variable)
{
	DECLARE__STD_VARIABLE

	CCommon_Error__DEF_VARIABLE m_err_def_variable;
	p_variable->Link__DEF_VARIABLE__ERROR_FEEDBACK(&m_err_def_variable);

	// ...
	CString str_name;

	// PARA ...
	{
		str_name = "PARA.STN.SLOT";
		STD__ADD_DIGITAL(str_name, APP_DSP__DSlot);
		LINK__VAR_DIGITAL_CTRL(dCH__PARA_SLOT, str_name);

		str_name = "PARA.CCD.POS";
		STD__ADD_ANALOG(str_name, "deg", 1, 0.0, 360.0);
		LINK__VAR_ANALOG_CTRL(aCH__PARA_CCD_POS, str_name);
	}

	// CFG ...
	{
		str_name = "CFG.ACTION.CONFIRM.FLAG";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"DISABLE ENABLE","");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_ACTION_CONFIRM_FLAG,str_name);
	}

	// Simulation ...
	{
		str_name = "SIM.CFG.REAL.TEST";
		STD__ADD_DIGITAL(str_name, "NO YES");
		LINK__VAR_DIGITAL_CTRL(dCH__SIM_CFG__REAL_TEST, str_name);

		str_name = "SIM.CFG.ALIGN.TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "sec", 1, 0.0, 10.0, "");
		LINK__VAR_ANALOG_CTRL(aCH__SIM_CFG__ALIGN_TIME, str_name);
	}

	// ...
	{
		p_variable->Add__MONITORING_PROC(1.0, MON_ID__IO_MONITOR);
	}
	return 1;
}


// ...
#define  ACT__RETRY_ABORT						\
l_act.RemoveAll();								\
l_act.Add("RETRY");								\
l_act.Add("ABORT");

#define  ACT__ABORT_ALARM						\
l_act.RemoveAll();								\
l_act.Add("ABORT");

#define  ACT__NO_YES_ALARM						\
l_act.RemoveAll();								\
l_act.Add("NO");								\
l_act.Add("YES");


int CObj__DRV_LINK_ALx::__DEFINE__ALARM(p_alarm)
{
	DECLARE__ALARM;

	// ...
	CString title;
	title.Format("%s - ",sObject_Name);

	CString alarm_title;
	CString alarm_msg;
	CString alarm_bff;
	CStringArray l_act;
	int alarm_id;

	// ...
	{
		alarm_id = ALID__OFFLINE_ALARM;

		alarm_title  = title;
		alarm_title += "Offline.";

		alarm_msg  = "Controller is Offline.\n";
		alarm_msg += "Please, Check Communication Status !\n";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	return 1;
}


//--------------------------------------------------------------------------------
int CObj__DRV_LINK_ALx::__INITIALIZE__OBJECT(p_variable,p_ext_obj_create)
{
	CString def_name;
	CString ch_name;
	CString obj_name;
	CString var_name;

	// OBJ : AL1_DRV ...
	{
		def_name = "OBJ__ALx_DRV";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, obj_name);

		pOBJ_CTRL__ALx_DRV = p_ext_obj_create->Create__OBJECT_CTRL(obj_name);

		//
		var_name = "PARA.ALx.CMMD";
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__ALx_PARA_CMMD, obj_name,var_name);

		var_name = "ALx.CMMD.STATE";
		LINK__EXT_VAR_STRING_CTRL(sEXT_CH__ALx_CMMD_STS, obj_name,var_name);
	}

	// ...
	{
		CString file_name;
		CString log_msg;

		file_name.Format("%s_App.log", sObject_Name);

		log_msg  = "\n\n";
		log_msg += "//------------------------------------------------------------------------";

		xAPP_LOG_CTRL->CREATE__SUB_DIRECTORY(sObject_Name);
		xAPP_LOG_CTRL->SET__PROPERTY(file_name,24*5,60);

		xAPP_LOG_CTRL->DISABLE__TIME_LOG();
		xAPP_LOG_CTRL->WRITE__LOG(log_msg);

		xAPP_LOG_CTRL->ENABLE__TIME_LOG();
		xAPP_LOG_CTRL->WRITE__LOG("   START   \n");
	}

	// ...
	{
		SCX__SEQ_INFO seq_info;

		iActive__SIM_MODE = seq_info->Is__SIMULATION_MODE();
	}
	return 1;
}

//--------------------------------------------------------------------------------
int CObj__DRV_LINK_ALx::__CALL__CONTROL_MODE(mode, p_debug, p_variable, p_alarm)
{
LOOP_RETRY:

	// ...
	CString para_slot;
	dCH__PARA_SLOT->Get__DATA(para_slot);

	// ...
	{
		CString log_msg;

		log_msg.Format("Start ... :  [%s] (%s)",
						mode,
						para_slot);

		Fnc__APP_LOG(log_msg);
	}

	// ...
	int seq_flag = 1;
	int flag = -1;

	if(dCH__CFG_ACTION_CONFIRM_FLAG->Check__DATA(STR__ENABLE) > 0)
	{
		CString log_msg;

		CString str_title;
		CString str_msg;
		CString str_bff;
		CStringArray l_act;
		CString r_act;

		str_title.Format("%s - Action Confirm Flag", sObject_Name);

		str_bff.Format("Object Mode : [%s] \n", mode);
		str_msg += str_bff;
		str_bff.Format("Para Slot : [%s] \n", para_slot);
		str_msg += str_bff;

		l_act.RemoveAll();
		l_act.Add(STR__YES);
		l_act.Add(STR__NO);

		p_alarm->Popup__MESSAGE_BOX(str_title,str_msg,l_act,r_act);

		if(r_act.CompareNoCase(STR__NO) == 0)
		{
			seq_flag = -1;

			CString log_msg = "Confirm : Action No Clicked !";

			Fnc__APP_LOG(log_msg);
		}
	}

	if(seq_flag > 0)
	{
			 IF__CTRL_MODE(sMODE__INIT)			flag = Call__INIT(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__HOME)			flag = Call__HOME(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__ALIGN)		flag = Call__ALIGN(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__ROTATE)		flag = Call__ROTATE(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__SCAN)			flag = Call__SCAN(p_variable,p_alarm);
	}

	if((flag < 0)||(p_variable->Check__CTRL_ABORT() > 0))
	{
		CString log_msg;	
		log_msg.Format("Aborted(%1d) ... :  [%s]\n", flag,mode);

		Fnc__APP_LOG(log_msg);
	}
	else
	{
		CString log_msg;
		log_msg.Format("Completed ... :  [%s]\n", mode);

		Fnc__APP_LOG(log_msg);
	}

	return flag;
}

int CObj__DRV_LINK_ALx::__CALL__MONITORING(id, p_variable, p_alarm)
{
	switch(id)
	{
		case MON_ID__IO_MONITOR:
			Mon__IO_MONITOR(p_variable,p_alarm);
			break;
	}

	return 1;
}
