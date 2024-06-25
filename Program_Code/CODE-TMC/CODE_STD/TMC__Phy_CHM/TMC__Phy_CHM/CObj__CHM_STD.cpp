#include "StdAfx.h"
#include "CObj__CHM_STD.h"
#include "CObj__CHM_STD__ALID.h"
#include "CObj__CHM_STD__DEF.h"

#include "CCommon_Utility.h"


//-------------------------------------------------------------------------
CObj__CHM_STD::CObj__CHM_STD()
{

}
CObj__CHM_STD::~CObj__CHM_STD()
{

}

//-------------------------------------------------------------------------
int CObj__CHM_STD::__DEFINE__CONTROL_MODE(obj,l_mode)
{
	sObject_Name = obj;

	// ...
	{
		ADD__CTRL_VAR(sMODE__INIT, "INIT");

		ADD__CTRL_VAR(sMODE__CL_ALL_VLV,  "CL.ALL_VLV" );
		ADD__CTRL_VAR(sMODE__CL_PUMP_VLV, "CL.PUMP_VLV");
		ADD__CTRL_VAR(sMODE__OP_PUMP_VLV, "OP.PUMP_VLV");

		ADD__CTRL_VAR(sMODE__PUMP, "PUMP");
		ADD__CTRL_VAR(sMODE__VENT, "VENT");

		ADD__CTRL_VAR(sMODE__AUTO_PUMP_VLV_CLOSE, "AUTO_PUMP_VLV_CLOSE");
	}
	return 1;
}
int CObj__CHM_STD::__DEFINE__VERSION_HISTORY(version)
{

	return 1;
}


// ...
#define  MON_ID__IO_MONITOR					1
#define  MON_ID__BALLAST_CONTROL			2

// ...
#define APP_DSP__CLOSE_OPEN					"UNKNOWN  CLOSE  OPEN"
#define APP_DSP__PRESS_STS					"UNKNOWN  ATM  BTW  VAC"


int CObj__CHM_STD::__DEFINE__VARIABLE_STD(p_variable)
{
	DECLARE__STD_VARIABLE;

	// ...
	CCommon_Error__DEF_VARIABLE m_err_def_variable;
	p_variable->Link__DEF_VARIABLE__ERROR_FEEDBACK(&m_err_def_variable);

	// ...
	CString str_name;

	// OBJ ...
	{
		str_name = "OTR.OUT.MSG";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(sCH__MSG,str_name);
	}

	// PARA ...
	{
		str_name = "PARA.BALLAST_CTRL.ACTIVE";
		STD__ADD_DIGITAL(str_name, "OFF ON");
		LINK__VAR_DIGITAL_CTRL(dCH__PARA_BALLAST_CTRL_ACTIVE, str_name);
	}

	// MON ...
	{
		str_name = "MON.PUMPING_SEQ.ACTIVE";
		STD__ADD_DIGITAL(str_name, "OFF ON");
		LINK__VAR_DIGITAL_CTRL(dCH__MON_PUMPING_SEQ_ACTIVE, str_name);

		//
		str_name = "OTR.OUT.MON.OBJ.STATUS";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(sCH__OBJ_STATUS,str_name);

		str_name = "OTR.OUT.MON.PUMP.VLV.OPEN.FLAG";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(sCH__PUMP_VLV_OPEN_FLAG,str_name);

		str_name = "OTR.OUT.MON.PRESSURE.CTRL.FLAG";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(sCH__PRESSURE_CTRL_FLAG,str_name);

		//
		str_name = "OTR.OUT.MON.aPRESSURE.TORR";
		STD__ADD_ANALOG_WITH_COMMENT(str_name,"torr",3,0,1000,"");
		LINK__VAR_ANALOG_CTRL(aCH__TMC_CHM_PRESSURE_TORR,str_name);

		str_name = "OTR.OUT.MON.aPRESSURE.mTORR";
		STD__ADD_ANALOG_WITH_COMMENT(str_name,"mtorr",3,0,1000000,"");
		LINK__VAR_ANALOG_CTRL(aCH__TMC_CHM_PRESSURE_mTORR,str_name);

		str_name = "OTR.OUT.MON.dPRESSURE.STATUS";
		STD__ADD_DIGITAL_WITH_COMMENT(str_name,APP_DSP__PRESS_STS,"");
		LINK__VAR_DIGITAL_CTRL(dCH__TMC_CHM_PRESSURE_STATUS, str_name);

		str_name = "OTR.OUT.MON.dVAC.SNS";
		STD__ADD_DIGITAL_WITH_COMMENT(str_name, "OFF ON","");
		LINK__VAR_DIGITAL_CTRL(dCH__TMC_CMH_VAC_SNS, str_name);

		//
		str_name = "MON.CHM.PRESSURE.DISPLAY";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__MON_CHM_PRESSURE_DISPLAY, str_name);
	}

	// CFG.CHAMBER_MANOMETER ...
	{
		str_name = "CFG.CHAMBER_MANOMETER.INTERLOCK.HIGH.LIMIT.PRESSURE";
		STD__ADD_ANALOG_WITH_COMMENT(str_name,"torr", 3, 1,100,"");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_CHAMBER_MANOMETER_INTERLOCK_HIGH_LIMIT_PRESSURE, str_name);
		
		for(int i=0; i<_CFG__CHM_GAUGE_SIZE; i++)
		{
			int id = i + 1;

			str_name.Format("CFG.CHAMBER_MANOMETER.MAX_PRESSURE.mTORR.%1d", id);
			STD__ADD_ANALOG_WITH_COMMENT(str_name, "mtorr", 1, 1,10000,"");
			LINK__VAR_ANALOG_CTRL(aCH__CFG_CHAMBER_MANOMETER_MAX_PRESSURE_mTORR_X[i], str_name);
			
			str_name.Format("CFG.CHAMBER_MANOMETER.LIMIT_PRESSURE.mTORR.%1d", id);
			STD__ADD_ANALOG_WITH_COMMENT(str_name, "mtorr", 1, 1,10000,"");
			LINK__VAR_ANALOG_CTRL(aCH__CFG_CHAMBER_MANOMETER_LIMIT_PRESSURE_mTORR_X[i], str_name);
		}
	}

	// CFG ...
	{
		str_name = "OTR.IN.CFG.aEXHAUST.PUMP.PRESSURE.TORR";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "torr", 3, 0, 10, "");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_EXHAUST_PUMP_PRESSURE_TORR, str_name);

		str_name = "OTR.IN.CFG.aEXHAUST.PUMP.TIMEOUT";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "sec", 0, 0, 9999, "");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_EXHAUST_PUMP_TIMEOUT, str_name);

		//
		str_name = "OTR.IN.CFG.aSOFT.PUMP.PRESSURE.TORR";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "torr", 3, 0, 1000, "");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_SOFT_PUMP_PRESSURE_TORR, str_name);

		str_name = "OTR.IN.CFG.aSOFT.PUMP.TIMEOUT";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "sec", 0, 0, 9999, "");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_SOFT_PUMP_TIMEOUT, str_name);

		str_name = "OTR.IN.CFG.aSOFT.PUMP.COMPLETE.STABLE.TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "sec", 0, 0, 100, "");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_SOFT_PUMP_COMPLETE_STABLE_TIME, str_name);

		str_name = "OTR.IN.CFG.aVAC.PRESSURE.TORR";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"torr", 3, 0, 100, "");	
		LINK__VAR_ANALOG_CTRL(aCH__CFG_FAST_PUMP_PRESSURE_TORR, str_name);

		str_name = "OTR.IN.CFG.aFAST.PUMP.TIMEOUT";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "sec", 0, 0, 9999, "");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_FAST_PUMP_TIMEOUT, str_name);

		//
		str_name = "OTR.IN.CFG.aFAST.VENT.PRESSURE.TORR";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"torr", 3, 0, 1000, "");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_SOFT_VENT_PRESSURE_TORR, str_name);

		str_name = "OTR.IN.CFG.aSOFT.VENT.TIMEOUT";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "sec", 0, 0, 9999, "");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_SOFT_VENT_TIMEOUT, str_name);

		str_name = "OTR.IN.CFG.aATM.PRESSURE.TORR";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"torr", 3, 0, 1000, "");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_FAST_VENT_PRESSURE_TORR, str_name);

		str_name = "OTR.IN.CFG.aFAST.VENT.TIMEOUT";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "sec", 0, 0, 9999, "");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_FAST_VENT_TIMEOUT, str_name);

		// 
		str_name = "CFG.aVAC.LOWER.TOLERANCE";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "mtorr", 0, 0, 500, "");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_VAC_LOWER_TOLERANCE,str_name);

		str_name = "CFG.aVAC.UPPER.TOLERANCE";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "mtorr", 0, 0, 500, "");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_VAC_UPPER_TOLERANCE,str_name);

		//
		str_name = "OTR.IN.CFG.aSOFT.PUMP.DELAY.TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec", 1, 0, 60,"");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_SOFT_PUMP_DELAY_TIME, str_name);

		str_name = "OTR.IN.CFG.aSOFT.VENT.DELAY.TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec", 1, 0, 60,"");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_SOFT_VENT_DELAY_TIME, str_name);

		//
		str_name = "OTR.IN.CFG.aVALVE.CLOSE.DELAY.TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec", 1, 0.3, 5.0,"");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_VALVE_CLOSE_DELAY_TIME, str_name);

		//
		str_name = "aTIME.COUNT";
		STD__ADD_ANALOG_WITH_COMMENT(str_name,"sec",0,0,9999,"");
		LINK__VAR_ANALOG_CTRL(aCH__TIME_COUNT,str_name);

		//
		str_name = "OTR.IN.CFG.aOVER.VENT.TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",0,0,100,"rec:10");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_OVER_VENT_TIME, str_name);

		//
		str_name = "CFG.aATM.PRESS.STS.TOLERANCE";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"torr", 0, -20, 20, "");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_ATM_PRESS_STS_TOLERANCE, str_name);

		//
		str_name = "OTR.IN.CFG.EQUALIZE.VENT.TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec", 1, 0, 10, "");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_EQUALIZE_VENT_TIME, str_name);

		str_name = "OTR.IN.CFG.EQUAL_VLV.OPEN.WHEN_ATM";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "DISABLE ENABLE", "");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_EQUAL_VLV_OPEN_WHEN_ATM, str_name);

		str_name = "CFG.ATM.HIGH.PRESSURE.TORR";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "torr", 0, 0, 1000, "");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_ATM_HIGH_PRESSURE_TORR, str_name);
	}

	// CFG.BALLAST ...
	{
		str_name = "CFG.dTM.BALLAST.CTRL";			
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "DISABLE ENABLE", "");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_TM_BALLAST_CONTROL,str_name);

		str_name = "CFG.dTM.BALLAST.MODE";			
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "VALVE  PRESSURE.SET  PRESSURE.PID", "");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_TM_BALLAST_MODE,str_name);

		//
		str_name = "CFG.aTM.BALLAST.N2.PRESSURE.mTORR";	
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "mtorr", 0, 10, 3000, "");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_TM_BALLAST_N2_PRESSURE_mTORR, str_name);

		str_name = "CFG.aTM.BALLAST.N2.PRESSURE.MIN";	
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "mtorr", 0, 0, 1000, "");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_TM_BALLAST_N2_PRESSURE_MIN, str_name);

		str_name = "CFG.aTM.BALLAST.N2.PRESSURE.MAX";	
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "mtorr", 0, 0, 10000, "");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_TM_BALLAST_N2_PRESSURE_MAX, str_name);

		//
		str_name = "CFG.aTM.BALLAST.N2.P.GAIN";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "val", 3, 0.001, 100.00, "");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_TM_BALLAST_N2_P_GAIN,str_name);

		str_name = "CFG.aTM.BALLAST.N2.I.GAIN";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,	"val", 3, 0.001, 100.00, "");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_TM_BALLAST_N2_I_GAIN,str_name);

		str_name = "CFG.aTM.BALLAST.N2.D.GAIN";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,	"val", 3, 0.001, 100.00, "");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_TM_BALLAST_N2_D_GAIN,str_name);

		//
		str_name = "CFG.TM.BALLAST.IDLE.FLOW";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "NO YES", "");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_TM_BALLAST_IDLE_FLOW, str_name);

		str_name = "CFG.TM.BALLAST.TRANSFER.FLOW";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "NO YES", "");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_TM_BALLAST_TRANSFER_FLOW, str_name);
	}

	// ...
	{
		p_variable->Add__MONITORING_PROC(3.0, MON_ID__IO_MONITOR);
		p_variable->Add__MONITORING_PROC(3.0, MON_ID__BALLAST_CONTROL);
	}
	return 1;
}


// ...
#define  ACT__RETRY_ABORT						\
l_act.RemoveAll();								\
l_act.Add("RETRY");								\
l_act.Add("ABORT");

#define  ACT__ABORT								\
l_act.RemoveAll();								\
l_act.Add("ABORT");

#define  ACT__CHECK								\
l_act.RemoveAll();								\
l_act.Add("CHECK");


int CObj__CHM_STD::__DEFINE__ALARM(p_alarm)
{
	DECLARE__ALARM;

	// ...
	CString module_name = "TMC_CHM";

	CString title;
	title.Format("%s - ",sObject_Name);

	CString alarm_title;
	CString alarm_msg;
	CStringArray l_act;
	int alarm_id;

	// ...
	{
		alarm_id = ALID__SOFT_PUMPING__TIMEOUT;

		alarm_title  = title;
		alarm_title += "Soft Pumping Timeout !";

		alarm_msg.Format("Please, check %s's pressure.\n", module_name);

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}
	// ...
	{
		alarm_id = ALID__FAST_PUMPING__TIMEOUT;

		alarm_title  = title;
		alarm_title += "Fast Pumping Timeout !";

		alarm_msg.Format("Please, check %s's pressure.\n", module_name);

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}
	// ...
	{
		alarm_id = ALID__SOFT_VENTING__TIMEOUT;

		alarm_title  = title;
		alarm_title += "Soft Venting Timeout !";

		alarm_msg.Format("Please, check %s's pressure.\n", module_name);

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}
	// ...
	{
		alarm_id = ALID__FAST_VENTING__TIMEOUT;

		alarm_title  = title;
		alarm_title += "Fast Venting Timeout !";

		alarm_msg.Format("Please, check %s's pressure.\n", module_name);

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}
	// ...
	{
		alarm_id = ALID__VENT_VALVE__PUMP_INTERLOCK;

		alarm_title  = title;
		alarm_title += "Vent Valve Interlock With Pumping Valve Open !";

		alarm_msg  = "All pumping valve must be closed !\n";

		ACT__CHECK;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}
	// ...
	{
		alarm_id = ALID__PUMP_OFFLINE_OFF_INTERLOCK;

		alarm_title  = title;
		alarm_title += "Dry Pump is Offline or Not Run Status !";

		alarm_msg  = "TMC CHAMBER's Fast Rough Valve will be closed.. !\n";
		alarm_msg += "Please, check pump status !\n";

		ACT__CHECK;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}
	// ...
	{
		alarm_id = ALID__EXHAUST_PRESSURE_TIMEOUT;

		alarm_title  = title;
		alarm_title += "Exhaust Pressure Timeout !";

		alarm_msg = "Please, check exhaust pressure gauge !\n";

		ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__CHM_LID_NOT_CLOSED;

		alarm_title  = title;
		alarm_title += "Chamber's lid is not closed !";

		alarm_msg = "Please, check the lid of chamber !\n";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATM_HIGH_PRESSURE_LIMIT;

		alarm_title  = title;
		alarm_title += "ATM High Pressure limit !";

		alarm_msg.Format("Please, check config pressure or gauge status.\n");

		ACT__CHECK;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__SR_VALVE_NOT_CLOSE;

		alarm_title  = title;
		alarm_title += "SR Valve Close Timeout !";

		alarm_msg.Format("Please, check the state of SR valve.\n");

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}
	// ...
	{
		alarm_id = ALID__SR_VALVE_NOT_OPEN;

		alarm_title  = title;
		alarm_title += "SR Valve Open Timeout !";

		alarm_msg.Format("Please, check the state of SR valve.\n");

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__FR_VALVE_NOT_CLOSE;

		alarm_title  = title;
		alarm_title += "FR Valve Close Timeout !";

		alarm_msg.Format("Please, check the state of FR valve.\n");

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}
	// ...
	{
		alarm_id = ALID__FR_VALVE_NOT_OPEN;

		alarm_title  = title;
		alarm_title += "FR Valve Open Timeout !";

		alarm_msg.Format("Please, check the state of FR valve.\n");

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__BALLAST_VLV_CLOSE;

		alarm_title  = title;
		alarm_title += "Ballast Open must be closed !";

		alarm_msg.Format("Please, check the state of pumping. \n");

		ACT__CHECK;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	return 1;
}

//-------------------------------------------------------------------------
int CObj__CHM_STD::__INITIALIZE__OBJECT(p_variable,p_ext_obj_create)
{
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

	// ...
	CString def_name;
	CString def_data;
	CString str_name;

	CString ch_name;
	CString obj_name;
	CString var_name;

	// ...
	CCommon_Utility x_utility;
	bool def_check;


	// OBJ : DB ...
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

		//
		str_name = "CFG.SIM.TMx.VENT.TIME";
		LINK__EXT_VAR_ANALOG_CTRL(aEXT_CH__CFG_SIM_VENT_TIME, def_data,str_name);

		str_name = "CFG.SIM.TMx.PUMP.TIME";
		LINK__EXT_VAR_ANALOG_CTRL(aEXT_CH__CFG_SIM_PUMP_TIME, def_data,str_name);
	}

	// OBJ : BALLAST_LOG ...
	{
		def_name = "OBJ__BALLAST_LOG";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, obj_name);

		def_check = x_utility.Check__Link(obj_name);
		bActive__BALLAST_LOG = def_check;

		if(def_check)
		{
			pOBJ__BALLAST_LOG = p_ext_obj_create->Create__OBJECT_CTRL(obj_name);
		}
	}

	// OBJ : IO ...
	{
		// VENT VLV ...
		{
			def_name = "CH__IO_DO_FAST_VENT_VLV";
			p_ext_obj_create->Get__DEF_CONST_DATA(def_name,def_data);
			p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name, var_name);
			LINK__EXT_VAR_DIGITAL_CTRL(doEXT_CH__FAST_VENT_VLV__SET, obj_name,var_name);

			//
			def_name = "CH__IO_DO_SOFT_VENT_VLV";
			p_ext_obj_create->Get__DEF_CONST_DATA(def_name,def_data);

			def_check = x_utility.Check__Link(def_data);
			bActive__SOFT_VENT_VLV__SET = def_check;

			if(def_check)
			{
				p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name,var_name);
				LINK__EXT_VAR_DIGITAL_CTRL(doEXT_CH__SOFT_VENT_VLV__SET, obj_name,var_name);
			}

			//
			def_name = "CH__IO_DO_EQUAL_VLV";
			p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);

			def_check = x_utility.Check__Link(def_data);
			bActive__ATM_EQUAL_VLV = def_check;

			if(def_check)
			{
				p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name,var_name);
				LINK__EXT_VAR_DIGITAL_CTRL(doEXT_CH__ATM_EQUAL_VLV__SET, obj_name,var_name);
			}
		}
		// PUMP VLV ...
		{
			def_name = "CH__IO_DO_FAST_PUMP_VLV";
			p_ext_obj_create->Get__DEF_CONST_DATA(def_name,def_data);
			p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name, var_name);
			LINK__EXT_VAR_DIGITAL_CTRL(doEXT_CH__FAST_PUMP_VLV__SET, obj_name,var_name);

			//
			def_name = "CH__IO_DO_SOFT_PUMP_VLV";
			p_ext_obj_create->Get__DEF_CONST_DATA(def_name,def_data);

			def_check = x_utility.Check__Link(def_data);
			bActive__SOFT_PUMP_VLV__SET = def_check;

			if(def_check)
			{
				p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name, var_name);
				LINK__EXT_VAR_DIGITAL_CTRL(doEXT_CH__SOFT_PUMP_VLV__SET, obj_name,var_name);
			}
		}
		// FR.VLV SENSOR ...
		{
			def_name = "CH__IO_DI_FR_VLV_OPEN";
			p_ext_obj_create->Get__DEF_CONST_DATA(def_name,def_data);

			def_check = x_utility.Check__Link(def_data);
			bActive__DI_FR_VLV_OPEN = def_check;

			if(def_check)
			{
				p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name, var_name);
				LINK__EXT_VAR_DIGITAL_CTRL(diEXT_CH__DI_FR_VLV_OPEN, obj_name,var_name);
			}

			//
			def_name = "CH__IO_DI_FR_VLV_CLOSE";
			p_ext_obj_create->Get__DEF_CONST_DATA(def_name,def_data);

			def_check = x_utility.Check__Link(def_data);
			bActive__DI_FR_VLV_CLOSE = def_check;

			if(def_check)
			{
				p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name, var_name);
				LINK__EXT_VAR_DIGITAL_CTRL(diEXT_CH__DI_FR_VLV_CLOSE, obj_name,var_name);
			}
		}
		// SR.VLV SENSOR ...
		{
			def_name = "CH__IO_DI_SR_VLV_OPEN";
			p_ext_obj_create->Get__DEF_CONST_DATA(def_name,def_data);

			def_check = x_utility.Check__Link(def_data);
			bActive__DI_SR_VLV_OPEN = def_check;

			if(def_check)
			{
				p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name, var_name);
				LINK__EXT_VAR_DIGITAL_CTRL(diEXT_CH__DI_SR_VLV_OPEN, obj_name,var_name);
			}

			//
			def_name = "CH__IO_DI_SR_VLV_CLOSE";
			p_ext_obj_create->Get__DEF_CONST_DATA(def_name,def_data);

			def_check = x_utility.Check__Link(def_data);
			bActive__DI_SR_VLV_CLOSE = def_check;

			if(def_check)
			{
				p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name, var_name);
				LINK__EXT_VAR_DIGITAL_CTRL(diEXT_CH__DI_SR_VLV_CLOSE, obj_name,var_name);
			}
		}

		// PRESSURE SENSOR ...
		{
			// ATM.VIRTUAL ...
			{
				def_name = "DATA.ATM_SNS.VIRTUAL_TYPE";
				p_ext_obj_create->Get__DEF_CONST_DATA(def_name,def_data);

				def_check = x_utility.Check__Link(def_data);
				bActive__ATM_SNS_Virtual_Type = def_check;
			}
			// ATM.SNS ...
			{
				def_name = "CH__IO_DI_ATM_SNS";
				p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);
				p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name, var_name);
				LINK__EXT_VAR_DIGITAL_CTRL(diEXT_CH__ATM_SENSOR, obj_name,var_name);

				//
				def_name = "DATA.ATM_ON";
				p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);
				sDATA__ATM_ON = def_data;

				def_name = "DATA.ATM_OFF";
				p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);
				sDATA__ATM_OFF = def_data;
			}

			// VAC.VIRTUAL ...
			{
				def_name = "DATA.VAC_SNS.VIRTUAL_TYPE";
				p_ext_obj_create->Get__DEF_CONST_DATA(def_name,def_data);

				def_check = x_utility.Check__Link(def_data);
				bActive__VAC_SNS_Virtual_Type = def_check;
			}
			// VAC.SNS ...
			{
				def_name = "CH__IO_DI_VAC_SNS";
				p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);
				p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name, var_name);
				LINK__EXT_VAR_DIGITAL_CTRL(diEXT_CH__VAC_SENSOR, obj_name,var_name);

				//
				def_name = "DATA.VAC_ON";
				p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);
				sDATA__VAC_ON = def_data;

				def_name = "DATA.VAC_OFF";
				p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);
				sDATA__VAC_OFF = def_data;
			}
		}

		// LID ...
		{
			def_name = "CH__IO_DI_LID_CLOSE_SNS";
			p_ext_obj_create->Get__DEF_CONST_DATA(def_name,def_data);

			def_check = x_utility.Check__Link(def_data);
			bActive__LID_CLOSE = def_check;

			if(def_check)
			{
				p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name,var_name);
				LINK__EXT_VAR_DIGITAL_CTRL(diEXT_CH__CHM_LID_CLOSE_SMS, obj_name,var_name);
			}
		}

		// BALLAST CONTROL  ...
		{
			// DO.VALVE ...
			{
				def_name = "CH__IO_DO_BALLAST_VALVE_SET";
				p_ext_obj_create->Get__DEF_CONST_DATA(def_name,def_data);

				def_check = x_utility.Check__Link(def_data);
				bActive__DO_BALLAST_VALVE_SET = def_check;
				
				if(def_check)
				{
					p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name,var_name);
					LINK__EXT_VAR_DIGITAL_CTRL(doEXT_CH__DO_BALLAST_VALVE_SET, obj_name,var_name);
				}	
			}
			
			// AO.PRESSURE ...
			{
				def_name = "CH__IO_AO_BALLAST_PRESSURE_TORR";
				p_ext_obj_create->Get__DEF_CONST_DATA(def_name,def_data);

				def_check = x_utility.Check__Link(def_data);
				bActive__AO_BALLAST_PRESSURE_TORR = def_check;

				if(def_check)
				{
					p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name,var_name);
					LINK__EXT_VAR_ANALOG_CTRL(aoEXT_CH__AO_BALLAST_PRESSURE_TORR, obj_name,var_name);
				}
			}
			// AI.PRESSURE ...
			{
				def_name = "CH__IO_AI_BALLAST_PRESSURE_TORR";
				p_ext_obj_create->Get__DEF_CONST_DATA(def_name,def_data);

				def_check = x_utility.Check__Link(def_data);
				bActive__AI_BALLAST_PRESSURE_TORR = def_check;

				if(def_check)
				{
					p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name,var_name);
					LINK__EXT_VAR_ANALOG_CTRL(aoEXT_CH__AI_BALLAST_PRESSURE_TORR, obj_name,var_name);
				}
			}
		}
	}

	// OBJ : PUMP ...
	{
		def_name = "OBJ__PUMP";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name,def_data);

		pPUMP__OBJ_CTRL = p_ext_obj_create->Create__OBJECT_CTRL(def_data);

		//
		str_name = "MON.PUMP.COMM_STS";
		LINK__EXT_VAR_STRING_CTRL(sEXT_CH__MON_PUMP_COMM_STS, def_data,str_name);

		str_name = "MON.PUMP.RUN_STS";
		LINK__EXT_VAR_STRING_CTRL(sEXT_CH__MON_PUMP_RUN_STS, def_data,str_name);
	}

	// CH.CGM_PRESSURE_TORR ...
	{
		def_name = "CH__CHM_PRESSURE_TORR";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name,var_name);
		LINK__EXT_VAR_ANALOG_CTRL(aiEXT_CH__CHM_PRESSURE_TORR, obj_name,var_name);		
	}

	// OBJ.CHM_GAUGE ...
	{
		def_name = "DATA.CHM_GAUGE_SIZE";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);

		iSIZE__CHM_GAUGE = atoi(def_data);
		if(iSIZE__CHM_GAUGE > _CFG__CHM_GAUGE_SIZE)		iSIZE__CHM_GAUGE = _CFG__CHM_GAUGE_SIZE;

		for(int i=0; i<iSIZE__CHM_GAUGE; i++)
		{
			int id = i + 1;

			// GAUGE.VALVE ...
			{
				def_name.Format("CH.DO_CHM_GAUGE_VLV.%1d", id);
				p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);
				p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name,var_name);
				LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__DO_CHM_ISO_VLV_X[i], obj_name,var_name);
			}

			// GAUGE.TORR ...
			{
				def_name.Format("CH.AI_CHM_GAUGE_TORR.%1d", id);
				p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);

				def_check = x_utility.Check__Link(def_data);
				bActive__AI_CHM_GAUGE_TORR_X[i] = def_check;
				
				if(def_check)
				{
					p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name,var_name);
					LINK__EXT_VAR_ANALOG_CTRL(aEXT_CH__AI_CHM_GAUGE_TORR_X[i], obj_name,var_name);
				}
			}
			// GAUGE.mTORR ...
			{
				def_name.Format("CH.AI_CHM_GAUGE_mTORR.%1d", id);
				p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);
				
				def_check = x_utility.Check__Link(def_data);
				bActive__AI_CHM_GAUGE_mTORR_X[i] = def_check;

				if(def_check)
				{
					p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name,var_name);
					LINK__EXT_VAR_ANALOG_CTRL(aEXT_CH__AI_CHM_GAUGE_mTORR_X[i], obj_name,var_name);
				}
			}
		}
	}
	
	// CH.EXHAUST_PRESSURE ...
	{
		def_name = "CH__EXHAUST_PRESSURE";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name,def_data);

		def_check = x_utility.Check__Link(def_data);
		bActive__TMC_CHM__EXHAUST_PRESSURE = def_check;

		if(def_check)
		{
			p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name,var_name);
			LINK__EXT_VAR_ANALOG_CTRL(aiEXT_CH__TMC_CHM__EXHAUST_PRESSURE, obj_name,var_name);
		}
	}

	// PMx SLOT VALVE ...
	{
		def_name = "DATA_SIZE.PMx_SLOT_VLV";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name,def_data);

		iSIZE__PMx_SLOT_VLV = atoi(def_data);
		if(iSIZE__PMx_SLOT_VLV > _CFG__PMx_SLOT_VLV_SIZE)			iSIZE__PMx_SLOT_VLV = _CFG__PMx_SLOT_VLV_SIZE;

		for(int i=0; i<iSIZE__PMx_SLOT_VLV; i++)
		{
			def_name.Format("CH.PMx_SLOT_VLV.%1d", i+1);
			p_ext_obj_create->Get__DEF_CONST_DATA(def_name, ch_name);

			p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(ch_name, obj_name,var_name);
			LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__PMx_SLOT_VLV_X[i], obj_name,var_name);
		}
	}
	// LLx SLOT VALVE ...
	{
		def_name = "DATA_SIZE.LLx_SLOT_VLV";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name,def_data);

		p_ext_obj_create->Get__DEF_CONST_DATA(def_name,def_data);

		iSIZE__LLx_SLOT_VLV = atoi(def_data);
		if(iSIZE__LLx_SLOT_VLV > _CFG__LLx_SLOT_VLV_SIZE)			iSIZE__LLx_SLOT_VLV = _CFG__LLx_SLOT_VLV_SIZE;

		for(int i=0; i<iSIZE__LLx_SLOT_VLV; i++)
		{
			def_name.Format("CH.LLx_SLOT_VLV.%1d", i+1);
			p_ext_obj_create->Get__DEF_CONST_DATA(def_name, ch_name);

			p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(ch_name, obj_name,var_name);
			LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__LLx_SLOT_VLV_X[i], obj_name,var_name);
		}
	}

	// ...
	{
		SCX__SEQ_INFO x_seq_info;

		iActive__SIM_MODE = x_seq_info->Is__SIMULATION_MODE();
	}

	if(iActive__SIM_MODE > 0)
	{
		if(bActive__LID_CLOSE)
			diEXT_CH__CHM_LID_CLOSE_SMS->Set__DATA(STR__ON);

		if(bActive__TMC_CHM__EXHAUST_PRESSURE)
			aiEXT_CH__TMC_CHM__EXHAUST_PRESSURE->Set__VALUE(0.01);
	}

	return 1;
}

//-------------------------------------------------------------------------
int CObj__CHM_STD::__CALL__CONTROL_MODE(mode,p_debug,p_variable,p_alarm)
{
	DECLARE__EXT_CTRL(p_variable);

	// ...
	{
		CString log_msg;
		xLOG_CTRL->WRITE__LOG("------------------------------>");

		log_msg.Format("Start  [%s] ... Called By %s", 
					   mode,
					   p_ext_mode_ctrl->Get__UPPER_OBJECT_NAME());

		Fnc__LOG(log_msg);
	}

	// ...
	{
		CString obj_msg;

		obj_msg.Format("Start  [%s] ...",  mode);
		Fnc__MSG(obj_msg);
	}

	// ...
	int flag = -1;

	if(dCH__PARA_BALLAST_CTRL_ACTIVE->Set__DATA(STR__ON) > 0)
	{
		Fnc__BALLAST_CLOSE();
	}

	// ...
	{
		IF__CTRL_MODE(sMODE__INIT)
		{
			flag = Call__INIT(p_variable,p_alarm);
		}
		ELSE_IF__CTRL_MODE(sMODE__CL_ALL_VLV)
		{
			flag = Call__CL_ALL_VLV(p_variable, p_alarm);
		}
		ELSE_IF__CTRL_MODE(sMODE__CL_PUMP_VLV)
		{
			flag = Call__CL_PUMP_VLV(p_variable, p_alarm);
		}
		ELSE_IF__CTRL_MODE(sMODE__OP_PUMP_VLV)
		{
			flag = Call__OP_PUMP_VLV(p_variable, p_alarm);
		}
		ELSE_IF__CTRL_MODE(sMODE__PUMP)
		{
			dCH__MON_PUMPING_SEQ_ACTIVE->Set__DATA(STR__ON);
			sCH__PRESSURE_CTRL_FLAG->Set__DATA("PUMP");

			flag = Call__PUMP(p_variable,p_alarm);

			dCH__MON_PUMPING_SEQ_ACTIVE->Set__DATA(STR__OFF);
		}
		ELSE_IF__CTRL_MODE(sMODE__VENT)
		{
			sCH__PRESSURE_CTRL_FLAG->Set__DATA("VENT");

			flag = Call__VENT(p_variable,p_alarm);
		}
		ELSE_IF__CTRL_MODE(sMODE__AUTO_PUMP_VLV_CLOSE)
		{
			flag = Call__AUTO_PUMP_VLV_CLOSE(p_variable,p_alarm);
		}
	}

	// ...
	{
		dCH__PARA_BALLAST_CTRL_ACTIVE->Set__DATA(STR__OFF);
	}

	if((flag < 0)||(p_variable->Check__CTRL_ABORT() > 0))
	{
		CString log_msg;
		log_msg.Format("[%s] Aborted (%1d) ...", mode, flag);

		Fnc__LOG(log_msg);
		Fnc__MSG(log_msg);
	}
	else
	{
		CString log_msg;
		log_msg.Format("[%s] Completed ...", mode);

		Fnc__LOG(log_msg);
		Fnc__MSG(log_msg);
	}

	xLOG_CTRL->WRITE__LOG("<------------------------------\n");
	return flag;
}

int CObj__CHM_STD::__CALL__MONITORING(id,p_variable,p_alarm)
{

	switch(id)
	{
		case MON_ID__IO_MONITOR:
			Mon__IO_MONITOR(p_variable,p_alarm);
			break;

		case MON_ID__BALLAST_CONTROL:
			Mon__BALLAST_CTRL(p_variable, p_alarm);
			break;
	}

	return 1;
}

