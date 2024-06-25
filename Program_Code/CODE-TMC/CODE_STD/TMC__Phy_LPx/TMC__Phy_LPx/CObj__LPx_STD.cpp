#include "StdAfx.h"
#include "CObj__LPx_STD.h"
#include "CObj__LPx_STD__ALID.h"


//-------------------------------------------------------------------------
CObj__LPx_STD::CObj__LPx_STD()
{

}
CObj__LPx_STD::~CObj__LPx_STD()
{

}

//-------------------------------------------------------------------------
int CObj__LPx_STD::__DEFINE__CONTROL_MODE(obj,l_mode)
{
	sObject_Name = obj;

	// ...
	{
		ADD__CTRL_VAR(sMODE__INIT, "INIT");
		ADD__CTRL_VAR(sMODE__HOME, "HOME");

		ADD__CTRL_VAR(sMODE__DOOR_OPEN,  "DOOR_OPEN");
		ADD__CTRL_VAR(sMODE__CHECK_DOOR_OPEN, "CHECK_DOOR_OPEN");

		ADD__CTRL_VAR(sMODE__LOAD,   "LOAD");
		ADD__CTRL_VAR(sMODE__UNLOAD, "UNLOAD");

		ADD__CTRL_VAR(sMODE__PREPLOAD,  "PREPLOAD");
		ADD__CTRL_VAR(sMODE__RLSUNLOAD, "RLSUNLOAD");

		ADD__CTRL_VAR(sMODE__CYCLE, "CYCLE");

		ADD__CTRL_VAR(sMODE__CID_READ,	"CID_READ");
		ADD__CTRL_VAR(sMODE__PAGE_READ,	"PAGE_READ");

		ADD__CTRL_VAR(sMODE__CID_WRITE,	 "CID_WRITE");
		ADD__CTRL_VAR(sMODE__PAGE_WRITE, "PAGE_WRITE");

		ADD__CTRL_VAR(sMODE__MAP,	  "MAP");
		ADD__CTRL_VAR(sMODE__CLAMP,	  "CLAMP");
		ADD__CTRL_VAR(sMODE__UNCLAMP, "UNCLAMP");

		// 
		ADD__CTRL_VAR(sMODE__EXTENDPADDLE,  "EXTENDPADDLE");
		ADD__CTRL_VAR(sMODE__RETRACTPADDLE, "RETRACTPADDLE");
		ADD__CTRL_VAR(sMODE__PADDLEDOWN,    "PADDLEDOWN");
		ADD__CTRL_VAR(sMODE__PADDLEUP,      "PADDLEUP");
		ADD__CTRL_VAR(sMODE__LATCH,      "LATCH");
		ADD__CTRL_VAR(sMODE__UNLATCH,    "UNLATCH");
		ADD__CTRL_VAR(sMODE__CLOSEDOOR,  "CLOSEDOOR");

		ADD__CTRL_VAR(sMODE__OPENDOOR,   "OPENDOOR");
		ADD__CTRL_VAR(sMODE__OPENDOOR_WITH_NO_UNLATCH, "OPENDOOR_WITH_NO_UNLATCH");

		ADD__CTRL_VAR(sMODE__VACON,      "VACON");
		ADD__CTRL_VAR(sMODE__VACOFF,     "VACOFF");

		ADD__CTRL_VAR(sMODE__EXTENDSHUTTLE,  "SHUTTLEOUT");
		ADD__CTRL_VAR(sMODE__RETRACTSHUTTLE, "SHUTTLEIN");
	}

	return 1;
}

int CObj__LPx_STD::__DEFINE__VERSION_HISTORY(version)
{

	return 1;
}


// ...
#define  MON_ID__IO_MONITOR				1


int CObj__LPx_STD::__DEFINE__VARIABLE_STD(p_variable)
{
	DECLARE__STD_VARIABLE;

	CCommon_Error__DEF_VARIABLE m_err_def_variable;
	p_variable->Link__DEF_VARIABLE__ERROR_FEEDBACK(&m_err_def_variable);

	// ...
	CString str_name;
	CString dsc_item_list;
	int i;

	// ...
	p_variable->Get__STD_DESCRIPTION(DSP__SLOT_STATUS,dsc_item_list);

	for(i=0;i<CFG_LPx__SLOT_MAX;i++)
	{
		// Slot Status 
		str_name.Format("OTR.OUT.MON.dSLOT%002d.STATUS",i+1);
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, dsc_item_list,"");
		LINK__VAR_DIGITAL_CTRL(dCH__SLOT_STATUS[i], str_name);

		// Slot Title
		str_name.Format("OTR.OUT.MON.sSLOT%002d.TITLE",i+1);
		STD__ADD_STRING_WITH_X_OPTION(str_name, "");
		LINK__VAR_STRING_CTRL(sCH__SLOT_TITLE[i], str_name);
	}

	// ...
	{
		str_name = "OTR.OUT.MON.dFOUP.CLAMP.STATUS";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(sCH__CLAMP_STS, str_name);

		str_name = "OTR.OUT.MON.dFOUP.EXIST.FLAG";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(sCH__FOUP_STS, str_name);

		str_name = "OTR.OUT.MON.dFOUP.POS.STATUS";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(sCH__FOUP_POS, str_name);

		str_name = "OTR.OUT.MON.dDOOR.STATUS";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(sCH__DOOR_STS, str_name);

		str_name = "OTR.OUT.MON.sTRANSFER.PIO.STATUS";
		STD__ADD_STRING_WITH_X_OPTION(str_name,"");
		LINK__VAR_STRING_CTRL(sCH__TRANSFER_PIO_STATUS,str_name);
	}

	// ...
	{
		str_name = "OTR.IN.CFG.FA.MODE";
		STD__ADD_STRING_WITH_X_OPTION(str_name,"");
		LINK__VAR_STRING_CTRL(sCH__CFG_FA_MODE,str_name);

		str_name = "OTR.IN.CFG.FA.SERVICE.MODE";
		STD__ADD_STRING_WITH_X_OPTION(str_name,"");
		LINK__VAR_STRING_CTRL(sCH__CFG_FA_SERVICE_MODE,str_name);

		str_name = "MON.ACTIVE.FA.AUTO";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__MON_ACTIVE_FA_AUTO, str_name);

		//
		str_name = "OTR.IN.CFG.BYPASS.READ.ID.FLAG";
		STD__ADD_STRING_WITH_X_OPTION(str_name,"");
		LINK__VAR_STRING_CTRL(sCH__CFG_BYPASS_READ_ID_FLAG,str_name);

		str_name = "OTR.IN.CFG.RELOAD.FLAG";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__CFG_RELOAD_FLAG,str_name);
	}

	// ...
	{
		str_name = "OTR.OUT.MON.FOUP.CID.STRING";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__FOUP_CID_STRING,str_name);

		str_name = "OTR.OUT.CFG.RF.EXIST.FLAG";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"YES NO","");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_RF_EXIST_FLAG,str_name);
	}

	// ...
	{
		str_name = "MAP_SEQ.LOCK";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(sCH__MAP_SEQ_LOCK,str_name);
	}

	// CFG ...
	{
		str_name = "CFG.PREPLOAD.CID.MODE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "DISABLE ENABLE", "");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_PREPLOAD_CID_MODE, str_name);

		str_name = "CFG.LOAD.MAP.MODE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "DISABLE ENABLE", "");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_LOAD_MAP_MODE, str_name);

		str_name = "CFG.LOAD_ACT.AUTO_MAP.MODE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "DISABLE ENABLE", "");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_LOAD_ACT_AUTO_MAP_MODE, str_name);
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


int CObj__LPx_STD::__DEFINE__ALARM(p_alarm)
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
		alarm_id = ALID__PUMP_PRESSURE__TIMEOUT;

		alarm_title  = title;
		alarm_title += "Pump Pressure TimeOut.";

		alarm_msg = "Please, check pump pressure config !\n";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}
	// ...
	{
		alarm_id = ALID__DOOR__NOT_OPEN;

		alarm_title  = title;
		alarm_title += "Door Status is not open !";

		alarm_msg = "Please, check loadport door status !\n";
	
		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}
	// ...
	{
		alarm_id = ALID__MAP_SEQ_LOCK__INTERLOCK;

		alarm_title  = title;
		alarm_title += "Currently, Load Port's status is \"Mapping_Sequence Lock\".";

		alarm_msg = "Please, check ATM Robot's status !\n";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}
	// ...
	{
		alarm_id = ALID__ROBOT_ARM__NOT_RETRACTED;

		alarm_title  = title;
		alarm_title += "Robot's arm is not retracted !\".";

		alarm_msg = "Please, check the RNE sensor of ATM Robot's arm ! \n";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	return 1;
}

//-------------------------------------------------------------------------
int CObj__LPx_STD::__INITIALIZE__OBJECT(p_variable,p_ext_obj_create)
{
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

	// ...
	CString def_name;
	CString def_data;
	CString str_name;
	CString ch_name;
	CString obj_name;
	CString var_name;
	int i;

	// ...
	CString obj_lp;
	CString obj_rf;

	bOBJ_TYPE__DUAL = TRUE;

	// CH_LINK : OBJ_STATE ...
	{
		def_name = "CH__OBJ_STATE";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, ch_name);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(ch_name, obj_name,str_name);
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__OBJ_STATE, obj_name,str_name);
	}

	// OBJ : LPx ...
	{
		def_name = "OBJ.LP";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);

		obj_lp = def_data;
		pLPx__OBJ_CTRL = p_ext_obj_create->Create__OBJECT_CTRL(def_data);

		//
		str_name = "OTR.OUT.MON.dFOUP.CLAMP.STATUS";
		LINK__EXT_VAR_STRING_CTRL(sEXT_CH__LPx__CLAMP_STS, def_data,str_name);

		str_name = "OTR.OUT.MON.dFOUP.EXIST.FLAG";
		LINK__EXT_VAR_STRING_CTRL(sEXT_CH__LPx__FOUP_STS, def_data,str_name);

		str_name = "OTR.OUT.MON.dFOUP.POS.STATUS";
		LINK__EXT_VAR_STRING_CTRL(sEXT_CH__LPx__FOUP_POS, def_data,str_name);

		str_name = "OTR.OUT.MON.dDOOR.STATUS";
		LINK__EXT_VAR_STRING_CTRL(sEXT_CH__LPx__DOOR_STS, def_data,str_name);

		str_name = "OTR.OUT.MON.sTRANSFER.PIO.STATUS";
		LINK__EXT_VAR_STRING_CTRL(sEXT_CH__TRANSFER_PIO_STATUS, def_data,str_name);

		//
		for(i=0; i<CFG_LPx__SLOT_MAX; i++)
		{
			str_name.Format("OTR.OUT.MON.dSLOT%002d.MAP.RESULT",i+1);
			LINK__EXT_VAR_STRING_CTRL(sEXT_CH__LPx__SLOT_MAP_RESULT[i], def_data,str_name);
		}

		str_name = "OTR.IN.sLP.RELOAD.FLAG";
		LINK__EXT_VAR_STRING_CTRL(sEXT_CH__RELOAD_FLAG, def_data,str_name);

		str_name = "MON.ACTIVE.FA.AUTO";
		LINK__EXT_VAR_STRING_CTRL(sEXT_CH__LPx__MON_ACTIVE_FA_AUTO, def_data,str_name);
	}

	// OBJ : LPx_RF ...
	{
		def_name = "OBJ.RF";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);

		obj_rf = def_data;

		if((def_data.CompareNoCase(STR__NO)   == 0)
		|| (def_data.CompareNoCase(STR__NULL) == 0))
		{
			bActive__RFx_OBJ = false;
		}
		else
		{
			bActive__RFx_OBJ = true;
		}

		if(bActive__RFx_OBJ)
		{
			pRFx__OBJ_CTRL = p_ext_obj_create->Create__OBJECT_CTRL(def_data);

			//
			str_name = "OTR.OUT.MON.sCID.STRING";
			LINK__EXT_VAR_STRING_CTRL(sEXT_CH__RFx__CID_STRING, def_data,str_name);

			str_name = "MON.ACTIVE.FA.AUTO";
			LINK__EXT_VAR_STRING_CTRL(sEXT_CH__RFx__MON_ACTIVE_FA_AUTO, def_data,str_name);
		}
	}

	// ARM_RNE.SENSOR ...
	{
		def_name = "ROBOT.ARM_RNE_SNS";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);

		if((def_data.CompareNoCase(STR__NO)   == 0)
		|| (def_data.CompareNoCase(STR__NULL) == 0))
		{
			bActive__ROBOT_ARM_RNE_SNS = false;
		}
		else
		{
			bActive__ROBOT_ARM_RNE_SNS = true;

			p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name,var_name);
			LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__ROBOT_ARM_RNE_SNS, obj_name,var_name);
		}

		//
		def_name = "DATA.RNE_ON";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);
		sDATA__RNE_ON = def_data;

		def_name = "DATA.RNE_OFF";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);
		sDATA__RNE_OFF = def_data;
	}

	// ...
	if(obj_lp.CompareNoCase(obj_rf) == 0)
	{
		bOBJ_TYPE__DUAL = FALSE;
	}
	else
	{
		bOBJ_TYPE__DUAL = TRUE;
	}

	// ...
	{
		SCX__SEQ_INFO x_seq_info;

		iActive_SIM = x_seq_info->Is__SIMULATION_MODE();

		if(iActive_SIM > 0)
		{
			/*
			sEXT_CH__LPx__CLAMP_STS->Set__DATA(CMMD__CLAMP);
			sEXT_CH__LPx__FOUP_STS->Set__DATA(STR__EXIST);
			sEXT_CH__LPx__FOUP_POS->Set__DATA(CMMD__PREPLOAD);
			sEXT_CH__LPx__DOOR_STS->Set__DATA(STR__CLOSE);
			sEXT_CH__TRANSFER_PIO_STATUS->Set__DATA(STR__NO);

			for(i=0; i<CFG_LPx__SLOT_MAX; i++)
			{
				dCH__SLOT_STATUS[i]->Set__DATA(STR__EXIST);
			}
			*/

			//
			if(bActive__ROBOT_ARM_RNE_SNS)
			{
				dEXT_CH__ROBOT_ARM_RNE_SNS->Set__DATA(sDATA__RNE_ON);
			}
		}
	}

	return 1;
}

//-------------------------------------------------------------------------
int CObj__LPx_STD::__CALL__CONTROL_MODE(mode,p_debug,p_variable,p_alarm)
{
LOOP_RETRY:

	// ...
	{
		CString log_msg;

		log_msg.Format("Start ... :  [%s]", mode);
		xLOG_CTRL->WRITE__LOG(log_msg);
	}

	if(sCH__MAP_SEQ_LOCK->Check__DATA("YES") > 0)
	{
		CString log_msg;

		log_msg.Format("Current MAP_SEQ Lock occurred !");
		xLOG_CTRL->WRITE__LOG(log_msg);

		// ...
		{
			int alarm_id = ALID__MAP_SEQ_LOCK__INTERLOCK;
			CString r_act;

			p_alarm->Popup__ALARM(alarm_id,r_act);

			if(r_act.CompareNoCase("RETRY") == 0)
			{
				goto LOOP_RETRY;
			}
		}
		return -1;
	}

	int flag = -1;

	// ...
	{
			 IF__CTRL_MODE(sMODE__INIT)					flag = Call__INIT(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__HOME)					flag = Call__HOME(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__DOOR_OPEN)			flag = Call__DOOR_OPEN(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__CHECK_DOOR_OPEN)		flag = Call__CHECK_DOOR_OPEN(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__LOAD)					flag = Call__LOAD(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__UNLOAD)				flag = Call__UNLOAD(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__PREPLOAD)				flag = Call__PREPLOAD(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__RLSUNLOAD)			flag = Call__RLSUNLOAD(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__CYCLE)				flag = Call__CYCLE(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__CID_READ)				flag = Call__CID_READ(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__PAGE_READ)			flag = Call__PAGE_READ(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__CID_WRITE)			flag = Call__CID_WRITE(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__PAGE_WRITE)			flag = Call__PAGE_WRITE(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__MAP)					flag = Call__MAP(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__CLAMP)				flag = Call__CLAMP(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__UNCLAMP)				flag = Call__UNCLAMP(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__EXTENDPADDLE)			flag = Call__EXTENDPADDLE(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__PADDLEDOWN)			flag = Call__PADDLEDOWN(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__PADDLEUP)				flag = Call__PADDLEUP(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__RETRACTPADDLE)		flag = Call__RETRACTPADDLE(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__LATCH)				flag = Call__LATCH(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__UNLATCH)				flag = Call__UNLATCH(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__CLOSEDOOR)					flag = Call__CLOSEDOOR(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__OPENDOOR)						flag = Call__OPENDOOR(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__OPENDOOR_WITH_NO_UNLATCH)     flag = Call__OPENDOOR_WITH_NO_UNLATCH(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__VACON)				flag = Call__VACON(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__VACOFF)				flag = Call__VACOFF(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__EXTENDSHUTTLE)		flag = Call__EXTENDSHUTTLE(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__RETRACTSHUTTLE)		flag = Call__RETRACTSHUTTLE(p_variable,p_alarm);
	}

	if((flag < 0)||(p_variable->Check__CTRL_ABORT() > 0))
	{
		CString log_msg;

		log_msg.Format("Aborted ... :  [%s]", mode);
		xLOG_CTRL->WRITE__LOG(log_msg);
	}
	else
	{
		CString log_msg;

		log_msg.Format("Completed ... :  [%s]", mode);
		xLOG_CTRL->WRITE__LOG(log_msg);
	}

	return flag;
}

int CObj__LPx_STD::__CALL__MONITORING(id,p_variable,p_alarm)
{
	switch(id)
	{
		case MON_ID__IO_MONITOR:
			Mon__IO_MONITOR(p_variable,p_alarm);
			break;
	}

	return 1;
}
