#include "StdAfx.h"
#include "CObj__PMC_STD.h"
#include "CObj__PMC_STD__ALID.h"


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

		//
		ADD__CTRL_VAR(sMODE__CHECK_SV_PM_ALL_CLOSE, "CHECK_SV_ALL_CLOSE");
		ADD__CTRL_VAR(sMODE__SV_PM_ALL_CLOSE, "SV_ALL_CLOSE");

		// 
		ADD__CTRL_VAR(sMODE__SV_OPEN,  "SV_OPEN");
		ADD__CTRL_VAR(sMODE__SV_CLOSE, "SV_CLOSE");

		ADD__CTRL_VAR(sMODE__INIT_SV_CLOSE, "INIT_SV_CLOSE");

		// 
		ADD__CTRL_VAR(sMODE__SV_PM_CLOSE, "SV_PM_CLOSE");		// up, pm_close..
		ADD__CTRL_VAR(sMODE__SV_TM_CLOSE, "SV_TM_CLOSE");		// up, tm_close..

		//
		ADD__CTRL_VAR(sMODE__CYCLE_TEST, "CYCLE_TEST");
	}

	return 1;
}

int CObj__PMC_STD::__DEFINE__VERSION_HISTORY(version)
{

	return 1;
}


// ...
#define  MON_ID__IO_MONITOR				2

// ...
#define  APP_DSP__CLOSE_OPEN			\
"UNKNOWN  CLOSE  OPEN"


int CObj__PMC_STD::__DEFINE__VARIABLE_STD(p_variable)
{
	DECLARE__STD_VARIABLE;

	//-----------------------------------------------------------------------------------
	 CCommon_Error__DEF_VARIABLE m_err_def_variable;
	p_variable->Link__DEF_VARIABLE__ERROR_FEEDBACK(&m_err_def_variable);

	// ...
	CString str_name;
	CString def_name, def_data;
	int i;

	// ...
	{
		def_name = "PM_SIZE";
		p_variable->Get__DEF_CONST_DATA(def_name, def_data);

		m_nPM_LIMIT = atoi(def_data);
		if(m_nPM_LIMIT <= 0)
		{
			m_nPM_LIMIT = CFG_PMx__SIZE;
		}
	}

	// ...
	{
		str_name = "OBJ.ACTIVE.MODE";

		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(sCH__OBJ_ACTIVE_MODE, str_name);
	}

	// ...
	{
		str_name = "OTR.IN.PARA.aPMC.ID";

		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"id",0,1,m_nPM_LIMIT,"");
		LINK__VAR_ANALOG_CTRL(aCH__PARA_PMC_ID, str_name);
	}

	// ...
	CString dsc_item_list;
	p_variable->Get__STD_DESCRIPTION(DSP__SLOT_STATUS,dsc_item_list);

	for(i=0; i<m_nPM_LIMIT; i++)
	{
		str_name.Format("CFG.PM%1d.HANDSHAKE.MODE", i+1);
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "ACTIVE PASSIVE", "");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_HANDSHAKE_MODE[i], str_name);

		str_name.Format("CFG.USE.SHUTTER.%1d", i+1);
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "YES NO", "");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_USE_SHUTTER[i], str_name);

		str_name.Format("CFG.USE.LIFT.PIN.%1d", i+1);
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "YES NO", "");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_USE_LIFT_PIN[i], str_name);

		str_name.Format("CFG.USE.CR.POSITION.%1d", i+1);
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "YES NO", "");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_USE_CR_POSITION[i], str_name);

		//
		str_name.Format("OTR.IN.CFG.aPM%1d.VALVE.OPEN.TIMEOUT",i+1);
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",1,0.5,100,"");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_VALVE_OPEN_TIMEOUT[i], str_name);

		str_name.Format("OTR.IN.CFG.aPM%1d.VALVE.CLOSE.TIMEOUT",i+1);
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",1,0.5,100,"");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_VALVE_CLOSE_TIMEOUT[i], str_name);

		// ...
		str_name.Format("OTR.OUT.MON.sPM%1d.OBJ.STATUS",i+1);
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(sCH__OBJ_STATUS[i], str_name);

		str_name.Format("OTR.OUT.MON.aPM%1d.PRESSURE.TORR",i+1);
		STD__ADD_ANALOG_WITH_COMMENT(str_name,"torr",3,0,1000,"");
		LINK__VAR_ANALOG_CTRL(aCH__PRESSURE_TORR[i], str_name);

		str_name.Format("OTR.OUT.MON.sPM%1d.PRESSURE.STATUS",i+1);
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(sCH__PRESSURE_STATUS[i], str_name);

		str_name.Format("OTR.OUT.MON.dPM%1d.SLIT.VALVE.STATUS",i+1);
		STD__ADD_DIGITAL_WITH_COMMENT(str_name,APP_DSP__CLOSE_OPEN,"");
		LINK__VAR_DIGITAL_CTRL(dCH__SLIT_VALVE_STATUS[i], str_name);

		str_name.Format("OTR.IN.MON.sPM%1d.CHUCK.STATUS",i+1);
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(sCH__CHUCK_STATUS[i], str_name);

		str_name.Format("OTR.IN.MON.sPM%1d.LIFT.PIN.STATUS",i+1);
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(sCH__LIFT_PIN_STATUS[i], str_name);

		// Slot Status & Title
		str_name.Format("OTR.OUT.MON.dPM%1d.SLOT01.STATUS",i+1);
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,dsc_item_list,"");
		LINK__VAR_DIGITAL_CTRL(dCH__SLOT01_STATUS[i],str_name);

		str_name.Format("OTR.OUT.MON.sPM%1d.SLOT01.TITLE",i+1);
		STD__ADD_STRING_WITH_X_OPTION(str_name,"");
		LINK__VAR_STRING_CTRL(sCH__SLOT01_TITLE[i],str_name);

		// PMx Process Status 
		str_name.Format("OTR.OUT.MON.dPM%1d.PRC.STS",i+1);
		STD__ADD_DIGITAL(str_name,"IDLE PROCESSING ALARM");
		LINK__VAR_DIGITAL_CTRL(dCH__PRC_STATUS[i],str_name);

		// PMx Pressure Status 
		str_name.Format("INR.dPM%1d.PRESS.STS",i+1);
		STD__ADD_DIGITAL(str_name,"UNKNOWN VAC BTW ATM");
		LINK__VAR_DIGITAL_CTRL(dCH__PRESS_STATUS[i],str_name);

		// TM and PM.. SV OPEN
		str_name.Format("aPM%1d.SV.OPEN.aTIME.COUNT",i+1);
		STD__ADD_ANALOG_WITH_COMMENT(str_name,"sec",0,0,9999,"");
		LINK__VAR_ANALOG_CTRL(aCH_SV_OPEN__TIME_COUNT[i],str_name);

		// TM and PM.. SV CLOSE
		str_name.Format("aPM%1d.SV.CLOSE.aTIME.COUNT",i+1);
		STD__ADD_ANALOG_WITH_COMMENT(str_name,"sec",0,0,9999,"");
		LINK__VAR_ANALOG_CTRL(aCH_SV_CLOSE__TIME_COUNT[i],str_name);

		// SV UP
		str_name.Format("aPM%1d.SV.UP.aTIME.COUNT",i+1);
		STD__ADD_ANALOG_WITH_COMMENT(str_name,"sec",0,0,9999,"");
		LINK__VAR_ANALOG_CTRL(aCH_SV_UP__TIME_COUNT[i],str_name);

		// SV DOWN
		str_name.Format("aPM%1d.SV.DOWN.aTIME.COUNT",i+1);
		STD__ADD_ANALOG_WITH_COMMENT(str_name,"sec",0,0,9999,"");
		LINK__VAR_ANALOG_CTRL(aCH_SV_DOWN__TIME_COUNT[i],str_name);

		// Shutter Status
		str_name.Format("MODULE.LINK.sPM%1d.SHUTTER.STS",i+1);
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH_SHUTTER_STS[i],str_name);

		// Confinement Ring Position
		str_name.Format("MODULE.LINK.sPM%1d.CR.POSITION.STS",i+1);
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH_CR_POSITION_STS[i],str_name);

		// Lift Pin Position
		str_name.Format("MODULE.LINK.sPM%1d.LIFT.PIN.STS",i+1);
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH_LIFT_PIN_POS_STS[i],str_name);

		//
		str_name.Format("CFG.aMAX.PM%1d.DIFFERENTIAL.PRESS", i+1);
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"mtorr",0,10,1000,"200mtorr");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_MAX_DIFF_PRESS[i],str_name);
	}

	// ...
	{
		str_name = "aTIME.COUNT";
		STD__ADD_ANALOG_WITH_COMMENT(str_name,"sec",0,0,9999,"");
		LINK__VAR_ANALOG_CTRL(aCH__TIME_COUNT,str_name);
	}

	// ...
	{
		//
		str_name = "CYCLE_TEST.CFG.PMC_ID";
		STD__ADD_STRING_WITH_X_OPTION(str_name,"");
		LINK__VAR_STRING_CTRL(sCH__CYCLE_TEST_CFG_PMC_ID,str_name);

		str_name = "CYCLE_TEST.CFG.SV_CLOSE_MODE";
		STD__ADD_STRING_WITH_X_OPTION(str_name,"");
		LINK__VAR_STRING_CTRL(sCH__CYCLE_TEST_CFG_SV_CLOSE_MODE,str_name);

		str_name = "CYCLE_TEST.CFG.CYCLE_COUNT";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"count",0,0,9999,"");
		LINK__VAR_ANALOG_CTRL(aCH__CYCLE_TEST_CFG_CYCLE_COUNT,str_name);

		str_name = "CYCLE_TEST.CUR.CYCLE_COUNT";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__CYCLE_TEST_CUR_CYCLE_COUNT,str_name);

		str_name = "CYCLE_TEST.CFG.DELAY_SEC";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",0,1,60,"");
		LINK__VAR_ANALOG_CTRL(aCH__CYCLE_TEST_CFG_DELAY_SEC,str_name);

		str_name = "CYCLE_TEST.CUR.DELAY_SEC";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__CYCLE_TEST_CUR_DELAY_SEC,str_name);

		str_name = "CYCLE_TEST.MSG";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__CYCLE_TEST_MSG,str_name);

		//
		str_name = "CYCLE_TEST.OP_TIME.CUR";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__CYCLE_TEST_OP_TIME_CUR, str_name);

		str_name = "CYCLE_TEST.OP_TIME.AVG";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__CYCLE_TEST_OP_TIME_AVG, str_name);

		str_name = "CYCLE_TEST.OP_TIME.MIN";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__CYCLE_TEST_OP_TIME_MIN, str_name);

		str_name = "CYCLE_TEST.OP_TIME.MAX";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__CYCLE_TEST_OP_TIME_MAX, str_name);

		//
		str_name = "CYCLE_TEST.CL_TIME.CUR";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__CYCLE_TEST_CL_TIME_CUR, str_name);

		str_name = "CYCLE_TEST.CL_TIME.AVG";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__CYCLE_TEST_CL_TIME_AVG, str_name);

		str_name = "CYCLE_TEST.CL_TIME.MIN";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__CYCLE_TEST_CL_TIME_MIN, str_name);

		str_name = "CYCLE_TEST.CL_TIME.MAX";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__CYCLE_TEST_CL_TIME_MAX, str_name);
	}

	// ...
	{
		p_variable->Add__MONITORING_PROC(3.0, MON_ID__IO_MONITOR);
	}
	return 1;
}


// ...
#define  ACT__RETRY_ABORT						\
l_act.RemoveAll();								\
l_act.Add("RETRY");								\
l_act.Add("ABORT");

#define  ACT__RETRY_ABORT_IGNORE				\
l_act.RemoveAll();								\
l_act.Add("RETRY");								\
l_act.Add("ABORT");								\
l_act.Add("IGNORE");								

#define  ACT__IGNORE_ABORT						\
l_act.RemoveAll();								\
l_act.Add("IGNORE");							\
l_act.Add("ABORT");


int CObj__PMC_STD::__DEFINE__ALARM(p_alarm)
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
		alarm_id = ALID__VALVE_OPEN__TIMEOUT;

		alarm_title  = title;
		alarm_title += "Valve Open TimeOut.";

		alarm_msg = "Please, Check I/O Object !\n";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__SLOT_VALVE_PM_CLOSE__TIMEOUT;

		alarm_title  = title;
		alarm_title += "PM Slot Valve.. PM Close TimeOut.";

		alarm_msg = "Please, Check I/O Status !\n";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__SLOT_VALVE_TM_CLOSE__TIMEOUT;

		alarm_title  = title;
		alarm_title += "PM Slot Valve.. TM Close TimeOut.";

		alarm_msg = "Please, Check I/O Status !\n";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VAC_RB_NOT_RETRACTED;

		alarm_title  = title;
		alarm_title += "VAC Robot's Arm not retracted sts.";

		alarm_msg = "Please, Check VAC Robot's Arm Status !\n";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__SV_OPEN__NOT_ATM_ERROR;

		alarm_title  = title;
		alarm_title += "Slit Valve can not open, because of not ATM Pressure !";

		alarm_msg.Format("Please, check PM and TM pressure Status.\n");

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__SV_OPEN__NOT_VAC_ERROR;

		alarm_title  = title;
		alarm_title += "Slit Valve can not open, because of not VAC Pressure !";

		alarm_msg.Format("Please, check PM and TM pressure Status.\n");

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VALVE_PM_SV_NOT_UP_ACTION_CONDITION;

		alarm_title  = title;
		alarm_title += "PM Slit Valve can not UP, because of not PM or TM SV OPEN Sts !";

		alarm_msg.Format("Please, check PM and TM SV OPEN Sensor Status.\n");

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__MAX_DIFFER_PRESS_ALARM;

		alarm_title  = title;
		alarm_title += "Max Difference Press Alarm Occur !";

		alarm_msg.Format("Please, check PM and TM Pressure Status.\n");

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__PM_NOT_EXIST_CONFIG_ALARAM;

		alarm_title  = title;
		alarm_title += "PM Not Exist Config Value !";

		alarm_msg.Format("Please, check PM Station Status.\n");

		ACT__IGNORE_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__SV_CLOSE_NOT_EQUAL_PRESSURE_ALARAM;

		alarm_title  = title;
		alarm_title += "Can't Close Slot Valve ! \n";

		alarm_msg.Format("PM and TM Station's Pressure Must be Equal.\n");

		ACT__RETRY_ABORT_IGNORE;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__TMC_PMC_PRESSURE_IS_NOT_SAME;

		alarm_title  = title;
		alarm_title += "TMC and PMC's pressure is not same !";

		alarm_msg = "";

		ACT__RETRY_ABORT_IGNORE;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	return 1;
}

//-------------------------------------------------------------------------
int CObj__PMC_STD::__INITIALIZE__OBJECT(p_variable,p_ext_obj_create)
{
	xI_ASYNC_TIMER->REGISTER__COUNT_CHANNEL(sObject_Name,aCH__TIME_COUNT->Get__VARIABLE_NAME());

	// ...
	CString def_name;
	CString def_data;
	CString str_name;
	CString obj_name, var_name;
	int i;

	// DB OBJECT
	{
		def_name = "PM_SIZE";
		p_variable->Get__DEF_CONST_DATA(def_name, def_data);

		m_nPM_LIMIT = atoi(def_data);
		if(m_nPM_LIMIT <= 0)
		{
			m_nPM_LIMIT = CFG_PMx__SIZE;
		}

		// ...
		def_name = "OBJ__DB";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name,def_data);

		//
		str_name = "CFG.SETUP.TEST.MODE";
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__CFG_SETUP_TEST_MODE, def_data,str_name);

		str_name = "CFG.SETUP.AUTO.VENTING_PUMPING";
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__CFG_SETUP_AUTO_VENTING_PUMPING, def_data,str_name);

		str_name = "CFG.SETUP.LLx.DOOR_SLOT_VLV.CONTROL";
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__CFG_SETUP_LLx_DOOR_SLOT_VLV_CONTROL, def_data,str_name);

		//
		str_name = "CFG.TRANSFER.MODE";
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__CFG_TRANSFER_MODE, def_data,str_name);

		for(i=0;i<m_nPM_LIMIT;i++)
		{
			str_name.Format("CFG.PM%1d.EXIST.FLAG",i+1);
			LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__CFG_PMx_EXIST_FLAG[i], def_data,str_name);
		}

		str_name = "CFG.SIM.SLOT.VLV.CLOSE.TIME";
		LINK__EXT_VAR_ANALOG_CTRL(aEXT_CH__CFG_SIM_SLOT_VLV_CLOSE_TIME, def_data,str_name);

		str_name = "CFG.SIM.SLOT.VLV.OPEN.TIME";
		LINK__EXT_VAR_ANALOG_CTRL(aEXT_CH__CFG_SIM_SLOT_VLV_OPEN_TIME, def_data,str_name);
	}

	// PHY_TM : TM의 Pressure Status (ATM, VAC, BTW)
	str_name.Format("VAR__PHY_TM_PRESS_STS");
	p_ext_obj_create->Get__DEF_CONST_DATA(str_name,def_data);
	p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name, var_name);
	LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__PHY_TM__PRESS_STS, obj_name,var_name);

	// PHY_TM : TM의 Pressure torr
	str_name.Format("VAR__PHY_TM_PRESS_TORR");
	p_ext_obj_create->Get__DEF_CONST_DATA(str_name,def_data);
	p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name, var_name);
	LINK__EXT_VAR_ANALOG_CTRL(aEXT_CH__PHY_TM__PRESS_TORR, obj_name,var_name);

	// I/O OBJECT
	{
		for(i=0;i<m_nPM_LIMIT;i++)
		{
			// do
			str_name.Format("VAR__IO_DO_PM%1d_SV_PM_OPEN",i+1);
			p_ext_obj_create->Get__DEF_CONST_DATA(str_name,def_data);
			p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name, var_name);
			LINK__EXT_VAR_DIGITAL_CTRL(doEXT_CH__PMx__SV_PM_OPEN[i], obj_name,var_name);

			str_name.Format("VAR__IO_DO_PM%1d_SV_PM_CLOSE",i+1);
			p_ext_obj_create->Get__DEF_CONST_DATA(str_name,def_data);
			p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name, var_name);
			LINK__EXT_VAR_DIGITAL_CTRL(doEXT_CH__PMx__SV_PM_CLOSE[i], obj_name,var_name);

			str_name.Format("VAR__IO_DO_PM%1d_SV_TM_OPEN",i+1);
			p_ext_obj_create->Get__DEF_CONST_DATA(str_name,def_data);
			p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name, var_name);
			LINK__EXT_VAR_DIGITAL_CTRL(doEXT_CH__PMx__SV_TM_OPEN[i], obj_name,var_name);

			str_name.Format("VAR__IO_DO_PM%1d_SV_TM_CLOSE",i+1);
			p_ext_obj_create->Get__DEF_CONST_DATA(str_name,def_data);
			p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name, var_name);
			LINK__EXT_VAR_DIGITAL_CTRL(doEXT_CH__PMx__SV_TM_CLOSE[i], obj_name,var_name);

			str_name.Format("VAR__IO_DO_PM%1d_SV_UP",i+1);
			p_ext_obj_create->Get__DEF_CONST_DATA(str_name,def_data);
			p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name, var_name);
			LINK__EXT_VAR_DIGITAL_CTRL(doEXT_CH__PMx__SV_UP[i], obj_name,var_name);

			str_name.Format("VAR__IO_DO_PM%1d_SV_DOWN",i+1);
			p_ext_obj_create->Get__DEF_CONST_DATA(str_name,def_data);
			p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name, var_name);
			LINK__EXT_VAR_DIGITAL_CTRL(doEXT_CH__PMx__SV_DOWN[i], obj_name,var_name);

			// di
			str_name.Format("VAR__IO_DI_PM%1d_SV_PM_OPEN",i+1);
			p_ext_obj_create->Get__DEF_CONST_DATA(str_name,def_data);
			p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name, var_name);
			LINK__EXT_VAR_DIGITAL_CTRL(diEXT_CH__PMx__SV_PM_OPEN[i], obj_name,var_name);

			str_name.Format("VAR__IO_DI_PM%1d_SV_PM_CLOSE",i+1);
			p_ext_obj_create->Get__DEF_CONST_DATA(str_name,def_data);
			p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name, var_name);
			LINK__EXT_VAR_DIGITAL_CTRL(diEXT_CH__PMx__SV_PM_CLOSE[i], obj_name,var_name);

			str_name.Format("VAR__IO_DI_PM%1d_SV_TM_OPEN",i+1);
			p_ext_obj_create->Get__DEF_CONST_DATA(str_name,def_data);
			p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name, var_name);
			LINK__EXT_VAR_DIGITAL_CTRL(diEXT_CH__PMx__SV_TM_OPEN[i], obj_name,var_name);

			str_name.Format("VAR__IO_DI_PM%1d_SV_TM_CLOSE",i+1);
			p_ext_obj_create->Get__DEF_CONST_DATA(str_name,def_data);
			p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name, var_name);
			LINK__EXT_VAR_DIGITAL_CTRL(diEXT_CH__PMx__SV_TM_CLOSE[i], obj_name,var_name);

			str_name.Format("VAR__IO_DI_PM%1d_SV_UP",i+1);
			p_ext_obj_create->Get__DEF_CONST_DATA(str_name,def_data);
			p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name, var_name);
			LINK__EXT_VAR_DIGITAL_CTRL(diEXT_CH__PMx__SV_UP[i], obj_name,var_name);

			str_name.Format("VAR__IO_DI_PM%1d_SV_DOWN",i+1);
			p_ext_obj_create->Get__DEF_CONST_DATA(str_name,def_data);
			p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name, var_name);
			LINK__EXT_VAR_DIGITAL_CTRL(diEXT_CH__PMx__SV_DOWN[i], obj_name,var_name);

			// Pressure sns
			str_name.Format("VAR__IO_DI_PM%d_ATM_SNS",i+1);
			p_ext_obj_create->Get__DEF_CONST_DATA(str_name,def_data);
			p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name, var_name);
			LINK__EXT_VAR_DIGITAL_CTRL(diEXT_CH__PMx__ATM_SNS[i], obj_name,var_name);

			str_name.Format("VAR__IO_DI_PM%d_VAC_SNS",i+1);
			p_ext_obj_create->Get__DEF_CONST_DATA(str_name,def_data);
			p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name, var_name);
			LINK__EXT_VAR_DIGITAL_CTRL(diEXT_CH__PMx__VAC_SNS[i], obj_name,var_name);
		}

		// DNET Comm Sts
		str_name.Format("VAR__IO_DI_DNET_COMM_STS");	//
		p_ext_obj_create->Get__DEF_CONST_DATA(str_name,def_data);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name, var_name);
		LINK__EXT_VAR_DIGITAL_CTRL(diEXT_CH__DNET_COMM_STS, obj_name,var_name);

		// VAC Robot RNE Sns
		str_name = "VAR__IO_DI_VAC_RB_RNE";
		p_ext_obj_create->Get__DEF_CONST_DATA(str_name,def_data);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name, var_name);
		LINK__EXT_VAR_DIGITAL_CTRL(diEXT_CH__VAC_RB_RNE, obj_name,var_name);

		// TM CHAM VAC, ATM Sns
		str_name.Format("VAR__IO_DI_TM_VAC_SNS");	//
		p_ext_obj_create->Get__DEF_CONST_DATA(str_name,def_data);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name, var_name);
		LINK__EXT_VAR_DIGITAL_CTRL(diEXT_CH__TM_VAC_SNS, obj_name,var_name);

		str_name.Format("VAR__IO_DI_TM_ATM_SNS");	//
		p_ext_obj_create->Get__DEF_CONST_DATA(str_name,def_data);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name, var_name);
		LINK__EXT_VAR_DIGITAL_CTRL(diEXT_CH__TM_ATM_SNS, obj_name,var_name);
	}

	// ...
	{
		SCX__SEQ_INFO x_seq_info;

		if(x_seq_info->Is__SIMULATION_MODE() > 0)
		{
			iSim_Flag =  1;
		}
		else
		{
			iSim_Flag = -1;
		}
	}

	// ...
	{
		CString file_name;
		CString log_msg;

		file_name.Format("%s_APP.log", sObject_Name);

		log_msg  = "\n\n";
		log_msg += "//------------------------------------------------------------------------";

		xLOG_CTRL->CREATE__SUB_DIRECTORY(sObject_Name);
		xLOG_CTRL->SET__PROPERTY(file_name,24*5,60);

		xLOG_CTRL->DISABLE__TIME_LOG();
		xLOG_CTRL->WRITE__LOG(log_msg);

		xLOG_CTRL->ENABLE__TIME_LOG();
		xLOG_CTRL->WRITE__LOG("   START   \n");
	}

	//
	return 1;
}

//-------------------------------------------------------------------------
int CObj__PMC_STD::__CALL__CONTROL_MODE(mode,p_debug,p_variable,p_alarm)
{
	int flag = -1;

	// ...
	CString para__pmc_id;
	int i__pmc_id;

	aCH__PARA_PMC_ID->Get__DATA(para__pmc_id);
	i__pmc_id = atoi(para__pmc_id);

	// ...
	{
		CString log_msg;
		log_msg.Format("-----------> Start ... :  [%s] (PM%1d)",mode,i__pmc_id);

		xLOG_CTRL->WRITE__LOG(log_msg);
	}

	sCH__OBJ_ACTIVE_MODE->Set__DATA(mode);

	// ...
	{
		IF__CTRL_MODE(sMODE__CHECK_SV_PM_ALL_CLOSE)
		{
			flag = Call__CHECK_SV_PM_ALL_CLOSE(p_variable,p_alarm);
		}
		ELSE_IF__CTRL_MODE(sMODE__INIT)
		{
			flag = Call__INIT(p_variable,p_alarm);
		}
		ELSE_IF__CTRL_MODE(sMODE__SV_PM_CLOSE)
		{
			flag = Call__SV_PM_CLOSE(p_variable,p_alarm,i__pmc_id);
		}
		ELSE_IF__CTRL_MODE(sMODE__SV_TM_CLOSE)
		{
			flag = Call__SV_TM_CLOSE(p_variable,p_alarm,i__pmc_id);
		}
		ELSE_IF__CTRL_MODE(sMODE__SV_PM_ALL_CLOSE)
		{
			flag = Call__SV_PM_ALL_CLOSE(p_variable,p_alarm);
		}
		ELSE_IF__CTRL_MODE(sMODE__SV_TM_ALL_CLOSE)
		{
			flag = Call__SV_TM_ALL_CLOSE(p_variable,p_alarm);
		}
		ELSE_IF__CTRL_MODE(sMODE__SV_CLOSE)					// 양산에서는 PM쪽으로 Close
		{
			flag = Call__SV_CLOSE(p_variable,p_alarm,i__pmc_id);
		}
		ELSE_IF__CTRL_MODE(sMODE__INIT_SV_CLOSE)
		{
			flag = Call__INIT_SV_CLOSE(p_variable,p_alarm,i__pmc_id);
		}
		ELSE_IF__CTRL_MODE(sMODE__SV_OPEN)
		{
			flag = Call__SV_OPEN(p_variable,p_alarm,i__pmc_id);
		}
		ELSE_IF__CTRL_MODE(sMODE__CYCLE_TEST)
		{
			flag = Call__CYCLE_TEST(p_variable,p_alarm);
		}

		else									
		{

		}
	}

	sCH__OBJ_ACTIVE_MODE->Set__DATA("");

	if((flag < 0)||(p_variable->Check__CTRL_ABORT() > 0))
	{
		// ...
		{
			CString log_msg;
			log_msg.Format("Aborted ... :  [%s]\n",mode);

			xLOG_CTRL->WRITE__LOG(log_msg);
		}
	}
	else
	{
		// ...
		{
			CString log_msg;
			log_msg.Format("Completed ... :  [%s]\n",mode);

			xLOG_CTRL->WRITE__LOG(log_msg);
		}
	}

	return flag;
}

int CObj__PMC_STD::__CALL__MONITORING(id,p_variable,p_alarm)
{
	switch(id)
	{
		case MON_ID__IO_MONITOR:
			Mon__IO_MONITOR(p_variable,p_alarm);
			break;
	}

	return 1;
}
