#include "StdAfx.h"
#include "CObj__PMC_EX.h"


//-------------------------------------------------------------------------
CObj__PMC_EX::CObj__PMC_EX()
{

}
CObj__PMC_EX::~CObj__PMC_EX()
{

}

//-------------------------------------------------------------------------
int CObj__PMC_EX::__DEFINE__CONTROL_MODE(obj,l_mode)
{
	sObject_Name = obj;

	// ...
	{
		ADD__CTRL_VAR(sMODE__INIT, "INIT");

		ADD__CTRL_VAR(sMODE__TRANSFER_READY, "TRANSFER.READY");
		ADD__CTRL_VAR(sMODE__TRANSFER_END,   "TRANSFER.END");

		ADD__CTRL_VAR(sMODE__S1, "S1");
		ADD__CTRL_VAR(sMODE__S2, "S2");
		ADD__CTRL_VAR(sMODE__S3, "S3");

		ADD__CTRL_VAR(sMODE__R1, "R1");
		ADD__CTRL_VAR(sMODE__R2, "R2");
		ADD__CTRL_VAR(sMODE__R3, "R3");
	}
	return 1;
}
int CObj__PMC_EX::__DEFINE__VERSION_HISTORY(version)
{

	return 1;
}


// ...
#define  DSP__OBJ_MODE									\
"INIT                                                   \
TRANSFER.READY  TRANSFER.END							\
S1 S2 S3												\
R1 R2 R3"


int CObj__PMC_EX::__DEFINE__VARIABLE_STD(p_variable)
{
	DECLARE__STD_VARIABLE;

	// ...
	CCommon_Error__DEF_VARIABLE m_err_def_variable;
	p_variable->Link__DEF_VARIABLE__ERROR_FEEDBACK(&m_err_def_variable);

	// ...
	CString pmc_name;
	CString str_name;
	CString dsc_item_list;
	int i;

	// Parameter ...
	{
		str_name = "PARA.PMx.ID";
		STD__ADD_ANALOG(str_name, "ID", 0, 1, 10);
		LINK__VAR_ANALOG_CTRL(aCH__PARA_PMx_ID, str_name);
	}

	for(i=0; i<_CFG__PMx_SIZE; i++)
	{
		pmc_name.Format("PM%1d", i+1);

		// Remote Control ...
		{
			p_variable->Get__STD_DESCRIPTION("STD_OBJ_CTRL", dsc_item_list);

			str_name.Format("%s.OBJ.CTRL", pmc_name);
			STD__ADD_DIGITAL(str_name,dsc_item_list);
			LINK__VAR_DIGITAL_CTRL(xCH__PMx_OBJ_CTRL[i], str_name);

			str_name.Format("%s.OBJ.MODE", pmc_name);
			STD__ADD_DIGITAL(str_name,DSP__OBJ_MODE);
			LINK__VAR_DIGITAL_CTRL(xCH__PMx_OBJ_MODE[i], str_name);
		}

		// Status ...
		{
			p_variable->Get__STD_DESCRIPTION("STD_OBJ_STATUS", dsc_item_list);

			str_name.Format("%s.OBJ.STATUS", pmc_name);
			STD__ADD_DIGITAL(str_name, dsc_item_list);
			LINK__VAR_STRING_CTRL(xCH__PMx_OBJ_STATUS[i], str_name);
		}

		// Link Test ...
		{
			/*
			str_name.Format("%s.LINK.COUNT.REPORT.SEND", pmc_name);
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__PMx_LINK_COUNT_REPORT_SEND[i], str_name);

			str_name.Format("%s.LINK.COUNT.REPORT.RECV", pmc_name);
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__PMx_LINK_COUNT_REPORT_RECV[i], str_name);
			*/
		}
	}

	return 1;
}
int CObj__PMC_EX::__DEFINE__ALARM(p_alarm)
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
	// ...
	{
		alarm_id = ALID__PMC_ID_ERROR;

		alarm_title  = title;
		alarm_title += "PMC ID : UNKNOWN";

		alarm_msg = "";

		l_act.RemoveAll();
		l_act.Add(STR__ABORT);

		ADD__ALARM_EX(alarm_id, alarm_title,alarm_msg,l_act);
	}

	return 1;
}

//-------------------------------------------------------------------------
int CObj__PMC_EX::__INITIALIZE__OBJECT(p_variable,p_ext_obj_create)
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
	CString def_name;
	CString def_data;

	// PMx_SIZE ...
	{
		def_name = "PM_SIZE";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name,def_data);

		iPMx_SIZE = atoi(def_data);
		if(iPMx_SIZE > _CFG__PMx_SIZE)		iPMx_SIZE = _CFG__PMx_SIZE;
	}

	for(int i=0; i<iPMx_SIZE; i++)
	{
		SCI__CTC__MODULE_OBJ *p_obj = xI_PMx_Obj[i].Get__PTR();

		// ...
		p_obj->Link__ERROR_FEEDBACK(&mERROR__MODULE_OBJ);

		p_obj->Link__Object_Name(sObject_Name);

		p_obj->Register__Module_Status_Channel(xCH__PMx_OBJ_STATUS[i]->Get__VARIABLE_NAME());
		p_obj->Register__Module_Mode_Channel(xCH__PMx_OBJ_MODE[i]->Get__VARIABLE_NAME());
		p_obj->Register__Module_Ctrl_Channel(xCH__PMx_OBJ_CTRL[i]->Get__VARIABLE_NAME());
	
		p_obj->Disable__CTCINUSE_TO_STANDBY();
	}

	return 1;
}

int CObj__PMC_EX::__CALL__CONTROL_MODE(mode,p_debug,p_variable,p_alarm)
{
	int flag = -1;

	// ...
	CStringArray l_para;
	p_variable->Get__FNC_PARAMETER(l_para);

	if(l_para.GetSize() > 0)
	{
		int limit = l_para.GetSize();

		for(int i=0; i<limit; i++)
		{
			CString para_data = l_para[i];

			switch(i)
			{
				case 0:		
					aCH__PARA_PMx_ID->Set__DATA(para_data);
					break;
			}	
		}
	}

	// ...
	int pm_i = (int) (aCH__PARA_PMx_ID->Get__VALUE() - 1);

	// ...
	{
		CString log_msg;
		log_msg.Format("Start_PM(%1d) ... :  [%s]", pm_i+1, mode);

		xLOG_CTRL->WRITE__LOG(log_msg);
	}

	if((pm_i >= 0) && (pm_i < iPMx_SIZE))
	{
		xI_PMx_Obj[pm_i]->Enable__ERROR_CHECK__NOT_STANDBY();

		// ...
	    IF__CTRL_MODE(sMODE__INIT)				flag = Call__INIT(p_variable, pm_i);

		ELSE_IF__CTRL_MODE(sMODE__TRANSFER_READY)		flag = Call__TRANSFER_READY(p_variable, pm_i);
		ELSE_IF__CTRL_MODE(sMODE__TRANSFER_END)			flag = Call__TRANSFER_END(p_variable, pm_i);

		ELSE_IF__CTRL_MODE(sMODE__S1)			flag = Call__S1(p_variable, pm_i);
		ELSE_IF__CTRL_MODE(sMODE__S2)			flag = Call__S2(p_variable, pm_i);
		ELSE_IF__CTRL_MODE(sMODE__S3)			flag = Call__S3(p_variable, pm_i);

		ELSE_IF__CTRL_MODE(sMODE__R1)			flag = Call__R1(p_variable, pm_i);
		ELSE_IF__CTRL_MODE(sMODE__R2)			flag = Call__R2(p_variable, pm_i);
		ELSE_IF__CTRL_MODE(sMODE__R3)			flag = Call__R3(p_variable, pm_i);
	}
	else
	{
		int alm_id = ALID__PMC_ID_ERROR;
		CString alm_msg;
		CString r_act;

		alm_msg.Format("PM ID <- %1d \n", pm_i+1);
		alm_msg += "Range Error ! \n";

		p_alarm->Popup__ALARM_With_MESSAGE(alm_id,alm_msg,r_act);		
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

int CObj__PMC_EX::__CALL__MONITORING(id,p_variable,p_alarm)
{

	return 1;
}
