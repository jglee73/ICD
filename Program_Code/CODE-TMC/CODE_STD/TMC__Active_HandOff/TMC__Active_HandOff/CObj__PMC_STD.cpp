#include "StdAfx.h"
#include "CObj__PMC_STD.h"


//-------------------------------------------------------------------------
CObj__PMC_STD::CObj__PMC_STD()
{

}
CObj__PMC_STD::~CObj__PMC_STD()
{

}

//-------------------------------------------------------------------------
int CObj__PMC_STD::__DEFINE__CONTROL_MODE(obj,l_mode)
{
	sObject_Name = obj;

	// ...
	{
		ADD__CTRL_VAR(sMODE__INIT, "INIT");

		ADD__CTRL_VAR(sMODE__S1, "S1");
		ADD__CTRL_VAR(sMODE__S2, "S2");
		ADD__CTRL_VAR(sMODE__S3, "S3");

		ADD__CTRL_VAR(sMODE__R1, "R1");
		ADD__CTRL_VAR(sMODE__R2, "R2");
		ADD__CTRL_VAR(sMODE__R3, "R3");
	}
	return 1;
}
int CObj__PMC_STD::__DEFINE__VERSION_HISTORY(version)
{

	return 1;
}


// ...
#define  MON_ID__MODULE_STATUS							1

// ...
#define  DSP__OBJ_MODE									\
"INIT                                                   \
S1 S2 S3												\
R1 R2 R3"


int CObj__PMC_STD::__DEFINE__VARIABLE_STD(p_variable)
{
	DECLARE__STD_VARIABLE;

	// ...
	CCommon_Error__DEF_VARIABLE m_err_def_variable;
	p_variable->Link__DEF_VARIABLE__ERROR_FEEDBACK(&m_err_def_variable);

	// ...
	CString str_name;
	CString dsc_item_list;

	// Remote Control ...
	{
		p_variable->Get__STD_DESCRIPTION("STD_OBJ_CTRL", dsc_item_list);

		// ...
		{
			str_name = "OBJ.CTRL";
			STD__ADD_DIGITAL(str_name,dsc_item_list);
			LINK__VAR_DIGITAL_CTRL(xCH__OBJ_CTRL,str_name);

			str_name = "OBJ.MODE";
			STD__ADD_DIGITAL(str_name,DSP__OBJ_MODE);
			LINK__VAR_DIGITAL_CTRL(xCH__OBJ_MODE,str_name);
		}
	}

	// Status ...
	{
		p_variable->Get__STD_DESCRIPTION("STD_OBJ_STATUS", dsc_item_list);

		// ...
		{
			str_name = "OBJ.STATUS";
			STD__ADD_DIGITAL(str_name,dsc_item_list);
			LINK__VAR_STRING_CTRL(xCH__OBJ_STATUS,str_name);
		}
	}

	// Link Test ...
	{
		str_name = "LINK.COUNT.REPORT.SEND";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__LINK_COUNT_REPORT_SEND, str_name);

		str_name = "LINK.COUNT.REPORT.RECV";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__LINK_COUNT_REPORT_RECV, str_name);
	}

	// ...
	{
		p_variable->Add__MONITORING_PROC(3.0, MON_ID__MODULE_STATUS);
	}
	return 1;
}
int CObj__PMC_STD::__DEFINE__ALARM(p_alarm)
{
	DECLARE__ALARM;

	// ...
	CString title;
	title.Format("%s - ",sObject_Name);

	int alarm_id;
	CString alarm_title;
	CString alarm_msg;
	CStringArray l_act;


	// ...
	{
		ALARM_DEFINE__MODULE_OBJECT;
	}

	// ...
	{
		alarm_id = ALID__OBJECT_MODE_UNKNOWN;

		alarm_title  = title;
		alarm_title += "OBJECT MODE : UNKNOWN";

		alarm_msg = "";

		l_act.RemoveAll();
		l_act.Add(STR__ABORT);

		ADD__ALARM_EX(alarm_id, alarm_title,alarm_msg,l_act);
	}

	return 1;
}

//-------------------------------------------------------------------------
int CObj__PMC_STD::__INITIALIZE__OBJECT(p_variable,p_ext_obj_create)
{
	// ...
	{
		CString file_name;
		CString log_msg;

		file_name.Format("%s-APPs.log", sObject_Name);

		log_msg  = "\n\n";
		log_msg += "//------------------------------------------------------------------------";

		xLOG_CTRL->CREATE__SUB_DIRECTORY(sObject_Name);
		xLOG_CTRL->SET__PROPERTY(file_name,24*5,60);

		xLOG_CTRL->DISABLE__TIME_LOG();
		xLOG_CTRL->WRITE__LOG(log_msg);

		xLOG_CTRL->ENABLE__TIME_LOG();
		xLOG_CTRL->WRITE__LOG("   START   \n");
	}

	// ...
	{
		xI_Module_Obj->Link__ERROR_FEEDBACK(&mERROR__MODULE_OBJ);

		//
		xI_Module_Obj->Link__Object_Name(sObject_Name);

		xI_Module_Obj->Register__Module_Status_Channel(xCH__OBJ_STATUS->Get__VARIABLE_NAME());
		xI_Module_Obj->Register__Module_Mode_Channel(xCH__OBJ_MODE->Get__VARIABLE_NAME());
		xI_Module_Obj->Register__Module_Ctrl_Channel(xCH__OBJ_CTRL->Get__VARIABLE_NAME());

		xI_Module_Obj->Disable__CTCINUSE_TO_STANDBY();
	}

	return 1;
}

int CObj__PMC_STD::__CALL__CONTROL_MODE(mode,p_debug,p_variable,p_alarm)
{
	int flag = -1;

	// ...
	{
		xI_Module_Obj->Enable__ERROR_CHECK__NOT_STANDBY();
	}

	// ...
	{
		CString log_msg;

		log_msg.Format("Start ... :  [%s]", mode);
		xLOG_CTRL->WRITE__LOG(log_msg);
	}

	// ...
	{
		     IF__CTRL_MODE(sMODE__INIT)			flag = Call__INIT(p_variable);

		ELSE_IF__CTRL_MODE(sMODE__S1)			flag = Call__S1(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__S2)			flag = Call__S2(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__S3)			flag = Call__S3(p_variable);

		ELSE_IF__CTRL_MODE(sMODE__R1)			flag = Call__R1(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__R2)			flag = Call__R2(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__R3)			flag = Call__R3(p_variable);

		else
		{
			CString bff;
			CString alarm_msg;
			CString r_act;

			bff.Format("Object Name : %s\n",sObject_Name);
			alarm_msg  = bff;
			bff.Format("Unknown Object Mode : \"%s\"\n",mode);
			alarm_msg += bff;

			p_alarm->Popup__ALARM_With_MESSAGE(ALID__OBJECT_MODE_UNKNOWN,alarm_msg,r_act);		
		}
	}


	if((flag < 0)||(p_variable->Check__CTRL_ABORT() > 0))
	{
		CString log_msg;

		log_msg.Format("Aborted ... :  [%s]",mode);
		xLOG_CTRL->WRITE__LOG(log_msg);
	}
	else
	{
		CString log_msg;

		log_msg.Format("Completed ... :  [%s]",mode);
		xLOG_CTRL->WRITE__LOG(log_msg);
	}

	return flag;
}

int CObj__PMC_STD::__CALL__MONITORING(id,p_variable,p_alarm)
{
	switch(id)
	{
		case MON_ID__MODULE_STATUS:
			Mon__MODULE_STATUS(p_variable,p_alarm);
			break;
	}

	return 1;
}
