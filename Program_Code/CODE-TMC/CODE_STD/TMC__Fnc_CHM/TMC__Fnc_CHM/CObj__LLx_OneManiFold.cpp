#include "StdAfx.h"
#include "CObj__LLx_OneManiFold.h"
#include "CObj__LLx_OneManiFold__DEF.h"
#include "CObj__LLx_OneManiFold__ALID.h"


//-------------------------------------------------------------------------
CObj__LLx_OneManiFold::CObj__LLx_OneManiFold()
{

}
CObj__LLx_OneManiFold::~CObj__LLx_OneManiFold()
{

}

//-------------------------------------------------------------------------
int CObj__LLx_OneManiFold::__DEFINE__CONTROL_MODE(obj,l_mode)
{
	sObject_Name = obj;

	// ...
	{
		ADD__CTRL_VAR(sMODE__LLx_LEAK_CHECK, "LLx_LEAK_CHECK");
	}
	return 1;
}
int CObj__LLx_OneManiFold::__DEFINE__VERSION_HISTORY(version)
{

	return 1;
}


// ...
#define APP_DSP__CLOSE_OPEN				"UNKNOWN  CLOSE  OPEN"
#define APP_DSP__DISABLE_ENABLE			"DISABLE  ENABLE"


int CObj__LLx_OneManiFold::__DEFINE__VARIABLE_STD(p_variable)
{
	DECLARE__STD_VARIABLE;

	CCommon_Error__DEF_VARIABLE m_err_def_variable;
	p_variable->Link__DEF_VARIABLE__ERROR_FEEDBACK(&m_err_def_variable);

	// ...
	CString str_name;
	int i;

	// ...
	{
		str_name = "OTR.OUT.MON.OBJ.STATUS";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(sCH__OBJ_STATUS,str_name);

		str_name = "OTR.OUT.MON.OBJ.MSG";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__OBJ_MSG,str_name);
	}

	// LLx ...
	{
		str_name = "OTR.IN.MON.ROBOT.TRANSFER.FLAG";
		STD__ADD_STRING_WITH_COMMENT(str_name, "");
		LINK__VAR_STRING_CTRL(sCH__LLx_ROBOT_TRANSFER_FLAG, str_name);

		str_name = "OTR.IN.CFG.LEAK.CHECK";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, APP_DSP__DISABLE_ENABLE,"");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_LLx_LEAK_CHECK, str_name);

		//
		str_name = "OTR.OUT.MON.LEAK_CHECK.TIME_COUNT";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",0,0.0,9999,"");
		LINK__VAR_ANALOG_CTRL(aCH__LLx_LEAK_CHECK__TIME_COUNT, str_name);

		str_name = "OTR.IN.CFG.LEAK_CHECK.OVER.PUMPING.TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",0,0.0,9999,"");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_LLx_LEAK_CHECK__OVER_PUMPING_TIME, str_name);

		str_name = "OTR.IN.CFG.LEAK_CHECK.STABLE.TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",0,0.0,9999,"");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_LLx_LEAK_CHECK__STABLE_TIME, str_name);

		str_name = "OTR.IN.CFG.LEAK_CHECK.CHECK_TIME.MIN";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "min" ,0,1,7,"");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_LLx_LEAK_CHECK__CHECK_TIME_MIN, str_name);

		//
		str_name = "OTR.OUT.MON.LEAK_CHECK.START.TIME";
		STD__ADD_STRING_WITH_X_OPTION(str_name,"");
		LINK__VAR_STRING_CTRL(sCH__LLx_LEAK_CHECK__START_TIME, str_name);

		str_name = "OTR.OUT.MON.LEAK_CHECK.END.TIME";
		STD__ADD_STRING_WITH_X_OPTION(str_name,"");
		LINK__VAR_STRING_CTRL(sCH__LLx_LEAK_CHECK__END_TIME, str_name);

		str_name = "OTR.OUT.MON.LEAK_CHECK.START.PRESSURE.TORR";
		STD__ADD_STRING_WITH_X_OPTION(str_name,"");
		LINK__VAR_STRING_CTRL(sCH__LLx_LEAK_CHECK__START_PRESSURE_TORR, str_name);

		str_name = "OTR.OUT.MON.LEAK_CHECK.END.PRESSURE.TORR";
		STD__ADD_STRING_WITH_X_OPTION(str_name, "");
		LINK__VAR_STRING_CTRL(sCH__LLx_LEAK_CHECK__END_PRESSURE_TORR, str_name);

		str_name = "OTR.OUT.MON.LEAK_CHECK.TOTAL.LEAK.RATE";
		STD__ADD_STRING_WITH_X_OPTION(str_name, "");
		LINK__VAR_STRING_CTRL(sCH__LLx_LEAK_CHECK__TOTAL_LEAK_RATE, str_name);

		str_name = "CFG.LEAK_CHECK.ALARM.RATE";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "mtorr_min", 0, 0, 1000, "rec:10");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_LLx_LEAK_CHECK__ALARM_RATE, str_name);

		for(i=0; i<CFG_LEAK_RATE__STEP_SIZE; i++)
		{
			str_name.Format("OTR.OUT.MON.LEAK_CHECK.STEP%1d.PRESSURE.TORR", i+1);
			STD__ADD_STRING_WITH_X_OPTION(str_name, "");
			LINK__VAR_STRING_CTRL(sCH__LLx_LEAK_CHECK__STEP_PRESSURE_TORR[i], str_name);

			str_name.Format("OTR.OUT.MON.LEAK_CHECK.STEP%1d.LEAK.RATE", i+1);
			STD__ADD_STRING_WITH_X_OPTION(str_name,"");
			LINK__VAR_STRING_CTRL(sCH__LLx_LEAK_CHECK__STEP_LEAK_RATE[i], str_name);
		}
	}

	// ...
	{
		str_name = "dOTR.OUT.PUMPING.STS.FLAG";
		STD__ADD_DIGITAL_WITH_COMMENT(str_name, "END PUMPING", "");
		LINK__VAR_DIGITAL_CTRL(dCH__PUMPING_STS_FLAG,str_name);
	}

	return 1;
}


// ...
#define  ACT__RETRY_ABORT						\
l_act.RemoveAll();								\
l_act.Add("RETRY");								\
l_act.Add("ABORT");

#define  ACT__CHK_RETRY_ABORT					\
l_act.RemoveAll();								\
l_act.Add("CHECK_RETRY");						\
l_act.Add("ABORT");

#define  ACT__CHECK								\
l_act.RemoveAll();								\
l_act.Add("CHECK");								


int CObj__LLx_OneManiFold::__DEFINE__ALARM(p_alarm)
{
	DECLARE__ALARM;

	// ...
	CString title;
	title.Format("%s - ",sObject_Name);

	CString alarm_title;
	CString alarm_msg;
	CStringArray l_act;
	int alarm_id;

	// ...
	{
		alarm_id = ALID__LLx__VAC_PRESSURE_ERROR;

		alarm_title  = title;
		alarm_title += "LLx's Vacuum Pressure Error.";

		alarm_msg = "Please, Check LLx's Pressure !\n";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__SLIT_VALVE__NOT_CLOSE;

		alarm_title  = title;
		alarm_title += "Slot-Valve is not Close !";

		alarm_msg = "";

		ACT__CHK_RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__LLx_LEAK_CHECK_BAD_RESULT_ALARM;

		alarm_title  = title;
		alarm_title += "LLx's LeakCheck is higher than config error value !";

		alarm_msg = "";

		ACT__CHECK;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	return 1;
}

//-------------------------------------------------------------------------
int CObj__LLx_OneManiFold::__INITIALIZE__OBJECT(p_variable,p_ext_obj_create)
{
	CString def_name;
	CString def_data;
	CString str_name;
	int i;

	// ...
	{
		CString file_name;
		CString log_msg;

		file_name.Format("%s_App.log", sObject_Name);

		log_msg  = "\n\n";
		log_msg += "//------------------------------------------------------------------------";

		xLOG_CTRL->CREATE__SUB_DIRECTORY(sObject_Name);
		xLOG_CTRL->SET__PROPERTY(file_name,24*5,60);

		xLOG_CTRL->DISABLE__TIME_LOG();
		xLOG_CTRL->WRITE__LOG(log_msg);

		xLOG_CTRL->ENABLE__TIME_LOG();
		xLOG_CTRL->WRITE__LOG("   START   \n");
	}


	// LLx ID ...
	{
		def_name = "LLx__ID";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name,def_data);

		iLLx_ID = atoi(def_data);
	}

	// DB OBJ
	{
		def_name = "OBJ__DB";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name,def_data);

		//
		str_name = "CFG.TRANSFER.MODE";
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__CFG_TRANSFER_MODE, def_data,str_name);

		str_name = "CFG.REF.ATM.PRESSURE";
		LINK__EXT_VAR_ANALOG_CTRL(aEXT_CH__CFG_REF_ATM_PRESSURE, def_data,str_name);

		str_name = "CFG.REF.VAC.PRESSURE";
		LINK__EXT_VAR_ANALOG_CTRL(aEXT_CH__CFG_REF_VAC_PRESSURE, def_data,str_name);
	}

	// LLx OBJ ...
	{
		def_name = "OBJ__LLx";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name,def_data);

		pLLx__OBJ_CTRL = p_ext_obj_create->Create__OBJECT_CTRL(def_data);

		//
		str_name = "OTR.OUT.MON.OBJ.STATUS";
		LINK__EXT_VAR_STRING_CTRL(sEXT_CH__LLx__OBJ_STATUS, def_data,str_name);

		str_name = "OTR.OUT.MON.PUMP.VLV.OPEN.FLAG";
		LINK__EXT_VAR_STRING_CTRL(sEXT_CH__LLx__PUMP_VLV_OPEN_FLAG, def_data,str_name);

		str_name = "OTR.OUT.MON.PRESSURE.CTRL.FLAG";
		LINK__EXT_VAR_STRING_CTRL(sEXT_CH__LLx__PRESSURE_CTRL_FLAG, def_data,str_name);

		str_name = "OTR.OUT.MON.aPRESSURE.TORR";
		LINK__EXT_VAR_ANALOG_CTRL(aEXT_CH__LLx__PRESSURE_TORR, def_data,str_name);

		str_name = "OTR.OUT.MON.dPRESSURE.STATUS";
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__LLx__PRESSURE_STATUS, def_data,str_name);

		str_name = "OTR.OUT.MON.dSLIT.VALVE.STATUS";
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__LLx__SLIT_VALVE_STATUS, def_data,str_name);
	}

	return 1;
}

//-------------------------------------------------------------------------
int CObj__LLx_OneManiFold::__CALL__CONTROL_MODE(mode,p_debug,p_variable,p_alarm)
{
	// ...
	{
		CString log_msg;
		log_msg.Format("Start ... :  [%s]",mode);

		xCH__OBJ_MSG->Set__DATA(log_msg);
		xLOG_CTRL->WRITE__LOG(log_msg);
	}

	int flag = -1;

	// ...
	{
		IF__CTRL_MODE(sMODE__LLx_LEAK_CHECK)			flag = Call__LLx_LEAK_CHECK(p_variable,p_alarm);
	}

	if((flag < 0)||(p_variable->Check__CTRL_ABORT() > 0))
	{
		CString log_msg;
		log_msg.Format("Aborted ... :  [%s] [Flag : %1d]",mode,flag);

		xCH__OBJ_MSG->Set__DATA(log_msg);
		xLOG_CTRL->WRITE__LOG(log_msg);
	}
	else
	{
		CString log_msg;
		log_msg.Format("Completed ... :  [%s]",mode);

		xCH__OBJ_MSG->Set__DATA(log_msg);
		xLOG_CTRL->WRITE__LOG(log_msg);
	}

	dCH__PUMPING_STS_FLAG->Set__DATA("END");
	return flag;
}
int CObj__LLx_OneManiFold::__CALL__MONITORING(id,p_variable,p_alarm)
{

	return 1;
}
