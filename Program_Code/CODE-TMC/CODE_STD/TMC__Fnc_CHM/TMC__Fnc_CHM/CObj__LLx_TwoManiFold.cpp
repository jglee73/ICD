#include "StdAfx.h"
#include "CObj__LLx_TwoManiFold.h"
#include "CObj__LLx_TwoManiFold__DEF.h"
#include "CObj__LLx_TwoManiFold__ALID.h"


//-------------------------------------------------------------------------
CObj__LLx_TwoManiFold::CObj__LLx_TwoManiFold()
{

}
CObj__LLx_TwoManiFold::~CObj__LLx_TwoManiFold()
{

}

//-------------------------------------------------------------------------
int CObj__LLx_TwoManiFold::__DEFINE__CONTROL_MODE(obj,l_mode)
{
	sObject_Name = obj;

	// LBA & LEFT ...
	{
		ADD__CTRL_VAR(sMODE__LBA_PUMP,  "LBA_PUMP");
		ADD__CTRL_VAR(sMODE__LEFT_PUMP, "LEFT_PUMP");

		ADD__CTRL_VAR(sMODE__LBA_LEAK_CHECK,  "LBA_LEAK_CHECK");
		ADD__CTRL_VAR(sMODE__LEFT_LEAK_CHECK, "LEFT_LEAK_CHECK");
	}
	// LBB & RIGHT ...
	{
		ADD__CTRL_VAR(sMODE__LBB_PUMP,    "LBB_PUMP");
		ADD__CTRL_VAR(sMODE__RIGHT_PUMP,  "RIGHT_PUMP");

		ADD__CTRL_VAR(sMODE__LBB_LEAK_CHECK,   "LBB_LEAK_CHECK");
		ADD__CTRL_VAR(sMODE__RIGHT_LEAK_CHECK, "RIGHT_LEAK_CHECK");
	}
	return 1;
}
int CObj__LLx_TwoManiFold::__DEFINE__VERSION_HISTORY(version)
{

	return 1;
}


// ...
#define APP_DSP__CLOSE_OPEN				"UNKNOWN  CLOSE  OPEN"
#define APP_DSP__DISABLE_ENABLE			"DISABLE  ENABLE"


int CObj__LLx_TwoManiFold::__DEFINE__VARIABLE_STD(p_variable)
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

		str_name = "OTR.IN.MON.sLBA.ROBOT.TRANSFER.FLAG";
		STD__ADD_STRING_WITH_COMMENT(str_name, "");
		LINK__VAR_STRING_CTRL(sCH__LBA_ROBOT_TRANSFER_FLAG, str_name);

		str_name = "OTR.IN.MON.sLBB.ROBOT.TRANSFER.FLAG";
		STD__ADD_STRING_WITH_COMMENT(str_name, "");
		LINK__VAR_STRING_CTRL(sCH__LBB_ROBOT_TRANSFER_FLAG, str_name);
	}

	// ...
	{
		str_name = "OTR.IN.CFG.dLBA.LEAK.CHECK";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, APP_DSP__DISABLE_ENABLE,"");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_LBA_LEAK_CHECK, str_name);

		str_name = "OTR.IN.CFG.dLBB.LEAK.CHECK";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, APP_DSP__DISABLE_ENABLE,"");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_LBB_LEAK_CHECK, str_name);
	}

	// LBA : LEAK_CHECK PARA -----
	{
		str_name = "OTR.OUT.MON.aLBA.LEAK_CHECK.TIME_COUNT";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",0,0.0,9999,"");
		LINK__VAR_ANALOG_CTRL(aCH__LBA_LEAK_CHECK__TIME_COUNT, str_name);

		str_name = "OTR.IN.CFG.aLBA.LEAK_CHECK.OVER.PUMPING.TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",0,0.0,9999,"");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_LBA_LEAK_CHECK__OVER_PUMPING_TIME, str_name);

		str_name = "OTR.IN.CFG.aLBA.LEAK_CHECK.STABLE.TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",0,0.0,9999,"");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_LBA_LEAK_CHECK__STABLE_TIME, str_name);

		str_name = "OTR.IN.CFG.aLBA.LEAK_CHECK.CHECK_TIME.MIN";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",0,1,30,"");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_LBA_LEAK_CHECK__CHECK_TIME_MIN, str_name);

		//
		str_name = "OTR.OUT.MON.sLBA.LEAK_CHECK.START.TIME";
		STD__ADD_STRING_WITH_X_OPTION(str_name,"");
		LINK__VAR_STRING_CTRL(sCH__LBA_LEAK_CHECK__START_TIME, str_name);

		str_name = "OTR.OUT.MON.sLBA.LEAK_CHECK.END.TIME";
		STD__ADD_STRING_WITH_X_OPTION(str_name,"");
		LINK__VAR_STRING_CTRL(sCH__LBA_LEAK_CHECK__END_TIME, str_name);

		str_name = "OTR.OUT.MON.aLBA.LEAK_CHECK.START.PRESSURE.TORR";
		STD__ADD_STRING_WITH_X_OPTION(str_name,"");
		LINK__VAR_STRING_CTRL(sCH__LBA_LEAK_CHECK__START_PRESSURE_TORR, str_name);

		str_name = "OTR.OUT.MON.aLBA.LEAK_CHECK.END.PRESSURE.TORR";
		STD__ADD_STRING_WITH_X_OPTION(str_name,"");
		LINK__VAR_STRING_CTRL(sCH__LBA_LEAK_CHECK__END_PRESSURE_TORR, str_name);

		str_name = "OTR.OUT.MON.sLBA.LEAK_CHECK.TOTAL.LEAK.RATE";
		STD__ADD_STRING_WITH_X_OPTION(str_name,"");
		LINK__VAR_STRING_CTRL(sCH__LBA_LEAK_CHECK__TOTAL_LEAK_RATE, str_name);

		// LBALeakBackAlarmRate
		str_name = "CFG__aLBA.LEAK_CHECK.ALARM.RATE";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "mtorr_min", 0, 0, 1000, "rec:10");
		LINK__VAR_ANALOG_CTRL(CFG_aCH__LBA_LEAK_CHECK__ALARM_RATE, str_name);

		for(i=0;i<CFG_LEAK_RATE__STEP_SIZE;i++)
		{
			str_name.Format("OTR.OUT.MON.sLBA.LEAK_CHECK.STEP%1d.PRESSURE.TORR",i+1);
			STD__ADD_STRING_WITH_X_OPTION(str_name,"");
			LINK__VAR_STRING_CTRL(sCH__LBA_LEAK_CHECK__STEP_PRESSURE_TORR[i], str_name);

			str_name.Format("OTR.OUT.MON.sLBA.LEAK_CHECK.STEP%1d.LEAK.RATE",i+1);
			STD__ADD_STRING_WITH_X_OPTION(str_name,"");
			LINK__VAR_STRING_CTRL(sCH__LBA_LEAK_CHECK__STEP_LEAK_RATE[i], str_name);
		}
	}

	// LBB : LEAK_CHECK PARA -----
	{
		str_name = "OTR.OUT.MON.aLBB.LEAK_CHECK.TIME_COUNT";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",0,0.0,9999,"");
		LINK__VAR_ANALOG_CTRL(aCH__LBB_LEAK_CHECK__TIME_COUNT, str_name);

		str_name = "OTR.IN.CFG.aLBB.LEAK_CHECK.OVER.PUMPING.TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",0,0.0,9999,"");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_LBB_LEAK_CHECK__OVER_PUMPING_TIME, str_name);

		str_name = "OTR.IN.CFG.aLBB.LEAK_CHECK.STABLE.TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",0,0.0,9999,"");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_LBB_LEAK_CHECK__STABLE_TIME, str_name);

		str_name = "OTR.IN.CFG.aLBB.LEAK_CHECK.CHECK_TIME.MIN";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",0,1,30,"");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_LBB_LEAK_CHECK__CHECK_TIME_MIN, str_name);

		//
		str_name = "OTR.OUT.MON.sLBB.LEAK_CHECK.START.TIME";
		STD__ADD_STRING_WITH_X_OPTION(str_name,"");
		LINK__VAR_STRING_CTRL(sCH__LBB_LEAK_CHECK__START_TIME, str_name);

		str_name = "OTR.OUT.MON.sLBB.LEAK_CHECK.END.TIME";
		STD__ADD_STRING_WITH_X_OPTION(str_name,"");
		LINK__VAR_STRING_CTRL(sCH__LBB_LEAK_CHECK__END_TIME, str_name);

		str_name = "OTR.OUT.MON.aLBB.LEAK_CHECK.START.PRESSURE.TORR";
		STD__ADD_STRING_WITH_X_OPTION(str_name,"");
		LINK__VAR_STRING_CTRL(sCH__LBB_LEAK_CHECK__START_PRESSURE_TORR, str_name);

		str_name = "OTR.OUT.MON.aLBB.LEAK_CHECK.END.PRESSURE.TORR";
		STD__ADD_STRING_WITH_X_OPTION(str_name,"");
		LINK__VAR_STRING_CTRL(sCH__LBB_LEAK_CHECK__END_PRESSURE_TORR, str_name);

		str_name = "OTR.OUT.MON.sLBB.LEAK_CHECK.TOTAL.LEAK.RATE";
		STD__ADD_STRING_WITH_X_OPTION(str_name,"");
		LINK__VAR_STRING_CTRL(sCH__LBB_LEAK_CHECK__TOTAL_LEAK_RATE, str_name);

		// LBBLeakBackAlarmRate
		str_name = "CFG__aLBB.LEAK_CHECK.ALARM.RATE";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "mtorr_min", 0, 0, 1000, "rec:10");
		LINK__VAR_ANALOG_CTRL(CFG_aCH__LBB_LEAK_CHECK__ALARM_RATE, str_name);

		for(i=0;i<CFG_LEAK_RATE__STEP_SIZE;i++)
		{
			str_name.Format("OTR.OUT.MON.sLBB.LEAK_CHECK.STEP%1d.PRESSURE.TORR",i+1);
			STD__ADD_STRING_WITH_X_OPTION(str_name,"");
			LINK__VAR_STRING_CTRL(sCH__LBB_LEAK_CHECK__STEP_PRESSURE_TORR[i], str_name);

			str_name.Format("OTR.OUT.MON.sLBB.LEAK_CHECK.STEP%1d.LEAK.RATE",i+1);
			STD__ADD_STRING_WITH_X_OPTION(str_name,"");
			LINK__VAR_STRING_CTRL(sCH__LBB_LEAK_CHECK__STEP_LEAK_RATE[i], str_name);
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


int CObj__LLx_TwoManiFold::__DEFINE__ALARM(p_alarm)
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
		alarm_id = ALID__LEFT_LLx__VAC_PRESSURE_ERROR;

		alarm_title  = title;
		alarm_title += "Left_LLx's Vacuum Pressure Error.";

		alarm_msg = "Please, Check Left_LLx's Pressure !\n";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}
	// ...
	{
		alarm_id = ALID__RIGHT_LLx__VAC_PRESSURE_ERROR;

		alarm_title  = title;
		alarm_title += "Right_LLx's Vacuum Pressure Error.";

		alarm_msg = "Please, Check Right_LLx's Pressure !\n";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__LEFT_LLx__LEAK_CHECK_BAD_RESULT_ALARM;

		alarm_title  = title;
		alarm_title += "Left_LLx's Leak Check is higher than config value !";

		alarm_msg = "";

		ACT__CHECK;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}
	// ...
	{
		alarm_id = ALID__RIGHT_LLx__LEAK_CHECK_BAD_RESULT_ALARM;

		alarm_title  = title;
		alarm_title += "Right_LLx's Leak Check is higher than config value !";

		alarm_msg = "";

		ACT__CHECK;
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

	return 1;
}

//-------------------------------------------------------------------------
int CObj__LLx_TwoManiFold::__INITIALIZE__OBJECT(p_variable,p_ext_obj_create)
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


	// DB OBJ
	{
		def_name = "OBJ__DB";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name,def_data);

		str_name = "CFG.TRANSFER.MODE";
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__CFG_TRANSFER_MODE, def_data,str_name);

		str_name = "CFG.REF.ATM.PRESSURE";
		LINK__EXT_VAR_ANALOG_CTRL(aEXT_CH__CFG_REF_ATM_PRESSURE, def_data,str_name);

		str_name = "CFG.REF.VAC.PRESSURE";
		LINK__EXT_VAR_ANALOG_CTRL(aEXT_CH__CFG_REF_VAC_PRESSURE, def_data,str_name);
	}

	// OBJ__LLx_LEFT ...
	{
		def_name = "OBJ__LLx_LEFT";
		p_variable->Get__DEF_CONST_DATA(def_name, def_data);

		pLBA_PRESS__OBJ_CTRL = p_ext_obj_create->Create__OBJECT_CTRL(def_data);

		//
		str_name = "OTR.OUT.MON.OBJ.STATUS";
		LINK__EXT_VAR_STRING_CTRL(sEXT_CH__LBA__OBJ_STATUS, def_data,str_name);

		str_name = "OTR.OUT.MON.PUMP.VLV.OPEN.FLAG";
		LINK__EXT_VAR_STRING_CTRL(sEXT_CH__LBA__PUMP_VLV_OPEN_FLAG, def_data,str_name);

		str_name = "OTR.OUT.MON.PRESSURE.CTRL.FLAG";
		LINK__EXT_VAR_STRING_CTRL(sEXT_CH__LBA__PRESSURE_CTRL_FLAG, def_data,str_name);

		str_name = "OTR.OUT.MON.aPRESSURE.TORR";
		LINK__EXT_VAR_ANALOG_CTRL(aEXT_CH__LBA__PRESSURE_TORR, def_data,str_name);

		str_name = "OTR.OUT.MON.dPRESSURE.STATUS";
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__LBA__PRESSURE_STATUS, def_data,str_name);

		str_name = "OTR.OUT.MON.dSLIT.VALVE.STATUS";
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__LBA__SLIT_VALVE_STATUS, def_data,str_name);
	}
	// LOG__LLx_LEFT ...
	{
		def_name = "LOG__LLx_LEFT";
		p_variable->Get__DEF_CONST_DATA(def_name, def_data);

		sLOG_NAME__LLx_LEFT = def_data;
	}

	// OBJ__LLx_RIGHT ...
	{
		def_name = "OBJ__LLx_RIGHT";
		p_variable->Get__DEF_CONST_DATA(def_name,def_data);

		pLBB_PRESS__OBJ_CTRL = p_ext_obj_create->Create__OBJECT_CTRL(def_data);

		//
		str_name = "OTR.OUT.MON.OBJ.STATUS";
		LINK__EXT_VAR_STRING_CTRL(sEXT_CH__LBB__OBJ_STATUS, def_data,str_name);

		str_name = "OTR.OUT.MON.PUMP.VLV.OPEN.FLAG";
		LINK__EXT_VAR_STRING_CTRL(sEXT_CH__LBB__PUMP_VLV_OPEN_FLAG, def_data,str_name);

		str_name = "OTR.OUT.MON.PRESSURE.CTRL.FLAG";
		LINK__EXT_VAR_STRING_CTRL(sEXT_CH__LBB__PRESSURE_CTRL_FLAG, def_data,str_name);

		str_name = "OTR.OUT.MON.aPRESSURE.TORR";
		LINK__EXT_VAR_ANALOG_CTRL(aEXT_CH__LBB__PRESSURE_TORR, def_data,str_name);

		str_name = "OTR.OUT.MON.dPRESSURE.STATUS";
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__LBB__PRESSURE_STATUS, def_data,str_name);

		str_name = "OTR.OUT.MON.dSLIT.VALVE.STATUS";
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__LBB__SLIT_VALVE_STATUS, def_data,str_name);
	}
	// OBJ__LLx_RIGHT ...
	{
		def_name = "LOG__LLx_RIGHT";
		p_variable->Get__DEF_CONST_DATA(def_name, def_data);

		sLOG_NAME__LLx_RIGHT = def_data;
	}

	// PRESS VLV OBJECT ...
	{
		def_name = "OBJ__PRESS_VLV";
		p_variable->Get__DEF_CONST_DATA(def_name,def_data);

		pPRESS_VLV__OBJ_CTRL = p_ext_obj_create->Create__OBJECT_CTRL(def_data);
	}

	return 1;
}

//-------------------------------------------------------------------------
int CObj__LLx_TwoManiFold::__CALL__CONTROL_MODE(mode,p_debug,p_variable,p_alarm)
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
		// LBA & LEFT ...
		     IF__CTRL_MODE(sMODE__LBA_PUMP)				flag = Call__LEFT_PUMP(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__LEFT_PUMP)			flag = Call__LEFT_PUMP(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__LBA_LEAK_CHECK)		flag = Call__LEFT_LEAK_CHECK(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__LEFT_LEAK_CHECK)		flag = Call__LEFT_LEAK_CHECK(p_variable,p_alarm);

		// LBB & RIGHT ...
		ELSE_IF__CTRL_MODE(sMODE__LBB_PUMP)				flag = Call__RIGHT_PUMP(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__RIGHT_PUMP)			flag = Call__RIGHT_PUMP(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__LBB_LEAK_CHECK)		flag = Call__RIGHT_LEAK_CHECK(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__RIGHT_LEAK_CHECK)		flag = Call__RIGHT_LEAK_CHECK(p_variable,p_alarm);
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
int CObj__LLx_TwoManiFold::__CALL__MONITORING(id,p_variable,p_alarm)
{

	return 1;
}
