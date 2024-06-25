#include "StdAfx.h"
#include "CObj__CHM_OneManiFold.h"
#include "CObj__CHM_OneManiFold__DEF.h"
#include "CObj__CHM_OneManiFold__ALID.h"


//-------------------------------------------------------------------------
CObj__CHM_OneManiFold::CObj__CHM_OneManiFold()
{

}
CObj__CHM_OneManiFold::~CObj__CHM_OneManiFold()
{

}

//-------------------------------------------------------------------------
int CObj__CHM_OneManiFold::__DEFINE__CONTROL_MODE(obj,l_mode)
{
	sObject_Name = obj;

	// ...
	{
		ADD__CTRL_VAR(sMODE__INIT,    "INIT");
		ADD__CTRL_VAR(sMODE__ISOLATE, "ISOLATE");

		ADD__CTRL_VAR(sMODE__PUMP,    "PUMP");
		ADD__CTRL_VAR(sMODE__VENT,    "VENT");

		ADD__CTRL_VAR(sMODE__LEAK_CHECK,  "LEAK_CHECK");
		ADD__CTRL_VAR(sMODE__CYCLE_PURGE, "CYCLE_PURGE");
	}
	return 1;
}
int CObj__CHM_OneManiFold::__DEFINE__VERSION_HISTORY(version)
{

	return 1;
}


// ...
#define MON_ID__IO_MONITOR				1

// ...
#define APP_DSP__CLOSE_OPEN				"UNKNOWN  CLOSE  OPEN"
#define APP_DSP__DISABLE_ENABLE			"DISABLE  ENABLE"


int CObj__CHM_OneManiFold::__DEFINE__VARIABLE_STD(p_variable)
{
	DECLARE__STD_VARIABLE;

	CCommon_Error__DEF_VARIABLE m_err_def_variable;
	p_variable->Link__DEF_VARIABLE__ERROR_FEEDBACK(&m_err_def_variable);

	// ...
	CString str_name;
	int i;

	// MON ...
	{
		str_name = "OTR.OUT.MON.OBJ.STATUS";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(sCH__OBJ_STATUS,str_name);

		str_name = "OTR.OUT.MON.OBJ.MSG";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__OBJ_MSG,str_name);

		str_name = "OTR.OUT.MON.aPRESSURE.TORR";
		STD__ADD_ANALOG_WITH_COMMENT(str_name,"torr",3,0,1000,"");
		LINK__VAR_ANALOG_CTRL(aCH__TMC_CHM_PRESSURE_TORR,str_name);

		str_name = "OTR.IN.MON.sROBOT.MATERIAL.TRANSFER.FLAG";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(sCH__ROBOT_MATERIAL_TRANSFER_FLAG, str_name);
	}

	// PARA ...
	{
		str_name = "PARA.BALLAST_CTRL.ACTIVE";
		STD__ADD_DIGITAL(str_name, "OFF ON");
		LINK__VAR_DIGITAL_CTRL(dCH__PARA_BALLAST_CTRL_ACTIVE, str_name);
	}

	// CFG ...
	{
		str_name = "OTR.IN.CFG.dTMC.LEAK.CHECK";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, APP_DSP__DISABLE_ENABLE,"");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_TMC_LEAK_CHECK, str_name);
	}

	// TMC : LEAK_CHECK PARA -----
	{
		str_name = "OTR.OUT.MON.aTMC.LEAK_CHECK.TIME_COUNT";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",0,0.0,9999,"");
		LINK__VAR_ANALOG_CTRL(aCH__TMC_LEAK_CHECK__TIME_COUNT, str_name);

		str_name = "OTR.IN.CFG.aTMC.LEAK_CHECK.OVER.PUMPING.TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",0,0.0,9999,"");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_TMC_LEAK_CHECK__OVER_PUMPING_TIME, str_name);

		str_name = "OTR.IN.CFG.aTMC.LEAK_CHECK.STABLE.TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",0,0.0,9999,"");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_TMC_LEAK_CHECK__STABLE_TIME, str_name);

		str_name = "OTR.IN.CFG.aTMC.LEAK_CHECK.CHECK_TIME.MIN";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "min" ,0,1,7,"");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_TMC_LEAK_CHECK__CHECK_TIME_MIN, str_name);

		//
		str_name = "OTR.OUT.MON.sTMC.LEAK_CHECK.START.TIME";
		STD__ADD_STRING_WITH_X_OPTION(str_name,"");
		LINK__VAR_STRING_CTRL(sCH__TMC_LEAK_CHECK__START_TIME, str_name);

		str_name = "OTR.OUT.MON.sTMC.LEAK_CHECK.END.TIME";
		STD__ADD_STRING_WITH_X_OPTION(str_name,"");
		LINK__VAR_STRING_CTRL(sCH__TMC_LEAK_CHECK__END_TIME, str_name);

		str_name = "OTR.OUT.MON.aTMC.LEAK_CHECK.START.PRESSURE.TORR";
		STD__ADD_STRING_WITH_X_OPTION(str_name,"");
		LINK__VAR_STRING_CTRL(sCH__TMC_LEAK_CHECK__START_PRESSURE_TORR, str_name);

		str_name = "OTR.OUT.MON.aTMC.LEAK_CHECK.END.PRESSURE.TORR";
		STD__ADD_STRING_WITH_X_OPTION(str_name,"");
		LINK__VAR_STRING_CTRL(sCH__TMC_LEAK_CHECK__END_PRESSURE_TORR, str_name);

		str_name = "OTR.OUT.MON.sTMC.LEAK_CHECK.TOTAL.LEAK.RATE";
		STD__ADD_STRING_WITH_X_OPTION(str_name,"");
		LINK__VAR_STRING_CTRL(sCH__TMC_LEAK_CHECK__TOTAL_LEAK_RATE, str_name);
	}

	// TMC LeakBack AlarmRate
	{
		str_name = "CFG__aTMC.LEAK_CHECK.ALARM.RATE";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "mtorr_min", 0, 0, 1000, "rec:10");
		LINK__VAR_ANALOG_CTRL(CFG_aCH__TMC_LEAK_CHECK__ALARM_RATE, str_name);

		for(i=0;i<CFG_LEAK_RATE__STEP_SIZE;i++)
		{
			str_name.Format("OTR.OUT.MON.sTMC.LEAK_CHECK.STEP%1d.PRESSURE.TORR",i+1);
			STD__ADD_STRING_WITH_X_OPTION(str_name,"");
			LINK__VAR_STRING_CTRL(sCH__TMC_LEAK_CHECK__STEP_PRESSURE_TORR[i], str_name);

			str_name.Format("OTR.OUT.MON.sTMC.LEAK_CHECK.STEP%1d.LEAK.RATE",i+1);
			STD__ADD_STRING_WITH_X_OPTION(str_name,"");
			LINK__VAR_STRING_CTRL(sCH__TMC_LEAK_CHECK__STEP_LEAK_RATE[i], str_name);
		}
	}

	// ...
	{
		str_name = "OTR.OUT.MON.dPRESSURE.STATUS";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(sCH__PRESSURE_STATUS, str_name);

		str_name = "dOTR.OUT.PUMPING.STS.FLAG";
		STD__ADD_DIGITAL_WITH_COMMENT(str_name,  "END PUMPING", "");
		LINK__VAR_DIGITAL_CTRL(dCH__PUMPING_STS_FLAG,str_name);
	}

	// Cycle Purge ...
	{
		str_name = "APP.PURGE.CYCLE.COUNT";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "count", 0, 1, 100000, "");	
		LINK__VAR_ANALOG_CTRL(aCFG_CH__CYCLE_PURGE_COUNT,str_name);

		str_name = "APP.PURGE.CURRENT.COUNT";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCFG_CH__CYCLE_PURGE_CURRENT_COUNT,str_name);

		str_name = "APP.PURGE.MESSAGE";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCFG_CH__CYCLE_PURGE_SEQ_MSG,str_name);
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

#define  ACT__CHK_RETRY_ABORT					\
l_act.RemoveAll();								\
l_act.Add("CHECK_RETRY");						\
l_act.Add("ABORT");

#define  ACT__CHECK								\
l_act.RemoveAll();								\
l_act.Add("CHECK");								


int CObj__CHM_OneManiFold::__DEFINE__ALARM(p_alarm)
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
		alarm_id = ALID__SLIT_VALVE__NOT_CLOSE;

		alarm_title  = title;
		alarm_title += "Slot-Valve is not Close !";

		alarm_msg = "";

		ACT__CHK_RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__TMC_LEAK_CHECK_BAD_RESULT_ALARM;

		alarm_title  = title;
		alarm_title += "TMC Leak Check is higher than config value !";

		alarm_msg = "";

		ACT__CHECK;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	return 1;
}

//-------------------------------------------------------------------------
int CObj__CHM_OneManiFold::__INITIALIZE__OBJECT(p_variable,p_ext_obj_create)
{
	CString def_name;
	CString def_data;
	CString obj_name;
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


	// PM SIZE ...
	{
		def_name = "PM_SIZE";
		p_variable->Get__DEF_CONST_DATA(def_name, def_data);

		m_nPM_LIMIT = atoi(def_data);
		if(m_nPM_LIMIT > CFG_PMx__SIZE)			m_nPM_LIMIT = CFG_PMx__SIZE;
	}

	// DB OBJECT ...
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

		for(i=0; i<m_nPM_LIMIT; i++)
		{
			str_name.Format("CFG.PM%1d.EXIST.FLAG", i+1);
			LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__CFG_PMx_EXIST_FLAG[i], def_data,str_name);

			str_name.Format("CFG.PM%1d.SV.USE", i+1);
			LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__CFG_PMx_SV_USE[i], def_data,str_name);
		}
	}

	// TMC CHM
	{
		def_name = "OBJ__TMC_CHM";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name,def_data);

		pTMC_CHM_PRESS__OBJ_CTRL = p_ext_obj_create->Create__OBJECT_CTRL(def_data);

		// PARA ...
		{
			str_name = "PARA.BALLAST_CTRL.ACTIVE";
			LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__TMC_CHM__PARA_BALLAST_CTRL_ACTIVE, def_data,str_name);
		}

		// MON ...
		{
			str_name = "OTR.OUT.MON.OBJ.STATUS";
			LINK__EXT_VAR_STRING_CTRL(sEXT_CH__TMC_CHM__OBJ_STATUS, def_data,str_name);

			//
			str_name = "OTR.OUT.MON.PUMP.VLV.OPEN.FLAG";
			LINK__EXT_VAR_STRING_CTRL(sEXT_CH__TMC_CHM__PUMP_VLV_OPEN_FLAG, def_data,str_name);

			str_name = "OTR.OUT.MON.PRESSURE.CTRL.FLAG";
			LINK__EXT_VAR_STRING_CTRL(sEXT_CH__TMC_CHM__PRESSURE_CTRL_FLAG, def_data,str_name);

			str_name = "OTR.OUT.MON.aPRESSURE.TORR";
			LINK__EXT_VAR_ANALOG_CTRL(aEXT_CH__TMC_CHM__PRESSURE_TORR, def_data,str_name);

			str_name = "OTR.OUT.MON.dPRESSURE.STATUS";
			LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__TMC_CHM__PRESSURE_STATUS, def_data,str_name);
		}
	}

	// LLx OBJ ...
	{
		def_name = "LLx.MULTI_DOOR_VALVE";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);

		if(def_data.CompareNoCase("YES") == 0)			bActive__LLx_MULTI_DOOR_VALVE = true;
		else											bActive__LLx_MULTI_DOOR_VALVE = false;

		def_name = "LLx.MULTI_SLOT_VALVE";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name,def_data);
		
		if(def_data.CompareNoCase("YES") == 0)			bActive__LLx_MULTI_SLOT_VALVE = true;
		else											bActive__LLx_MULTI_SLOT_VALVE = false;

		if((bActive__LLx_MULTI_DOOR_VALVE) || (bActive__LLx_MULTI_SLOT_VALVE))
		{
			def_name = "LLx.SLOT_SIZE";
			p_ext_obj_create->Get__DEF_CONST_DATA(def_name,def_data);

			iLLx_SLOT_SIZE = atoi(def_data);
			if(iLLx_SLOT_SIZE > _LLx_SLOT__SIZE)		iLLx_SLOT_SIZE = _LLx_SLOT__SIZE;
		}
		else
		{
			iLLx_SLOT_SIZE = 1;
		}

		//
		def_name = "LL_SIZE";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);

		iCFG__LLx_SIZE = atoi(def_data);
		if(iCFG__LLx_SIZE > CFG_LLx__SIZE)		iCFG__LLx_SIZE = CFG_LLx__SIZE;

		for(i=0; i<iCFG__LLx_SIZE; i++)
		{
			def_name.Format("OBJ__LLx.%1d", i+1);
			p_variable->Get__DEF_CONST_DATA(def_name, obj_name);

			if(bActive__LLx_MULTI_SLOT_VALVE)
			{
				for(int k=0; k<iLLx_SLOT_SIZE; k++)
				{
					str_name.Format("OTR.OUT.MON.dSLIT.VALVE.STATUS.%1d", k+1);
					LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__LLx__SLIT_VALVE_STATUS_XX[i][k], obj_name,str_name);
				}
			}
			else
			{
				str_name = "OTR.OUT.MON.dSLIT.VALVE.STATUS";
				LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__LLx__SLIT_VALVE_STATUS_A[i], obj_name,str_name);
			}
		}
	}

	// PRESS VLV OBJ ...
	{
		def_name = "OBJ__PRESS_VLV";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name,def_data);

		pPRESS_VLV__OBJ_CTRL = p_ext_obj_create->Create__OBJECT_CTRL(def_data);
	}

	// PMx OBJ ...
	{
		def_name = "OBJ__PMx";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name,def_data);

		pPMx__OBJ_CTRL = p_ext_obj_create->Create__OBJECT_CTRL(def_data);

		for(i=0;i<m_nPM_LIMIT;i++)
		{
			str_name.Format("OTR.OUT.MON.dPM%1d.SLIT.VALVE.STATUS",i+1);
			LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__PMx__SLIT_VALVE_STATUS[i], def_data,str_name);
		}
	}
	return 1;
}

//-------------------------------------------------------------------------
int CObj__CHM_OneManiFold::__CALL__CONTROL_MODE(mode,p_debug,p_variable,p_alarm)
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
		CString ch_data = dCH__PARA_BALLAST_CTRL_ACTIVE->Get__STRING();
		dEXT_CH__TMC_CHM__PARA_BALLAST_CTRL_ACTIVE->Set__DATA(ch_data);
	}

	// ...
	{
			 IF__CTRL_MODE(sMODE__INIT)				flag = Call__INIT(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__ISOLATE)			flag = Call__ISOLATE(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__PUMP)				flag = Call__PUMP(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__VENT)				flag = Call__VENT(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__LEAK_CHECK)		flag = Call__LEAK_CHECK(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__CYCLE_PURGE)		flag = Call__CYCLE_PURGE(p_alarm,p_variable);
	}

	// ...
	{
		dEXT_CH__TMC_CHM__PARA_BALLAST_CTRL_ACTIVE->Set__DATA(STR__OFF);
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
int CObj__CHM_OneManiFold::__CALL__MONITORING(id,p_variable,p_alarm)
{
	switch(id)
	{
		case MON_ID__IO_MONITOR:
			Mon__IO_MONITOR(p_variable,p_alarm);
			break;
	}
	return 1;
}
