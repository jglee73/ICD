#include "StdAfx.h"
#include "CObj__LBx_STD.h"


//-------------------------------------------------------------------------
CObj__LBx_STD::CObj__LBx_STD()
{

}
CObj__LBx_STD::~CObj__LBx_STD()
{

}

//-------------------------------------------------------------------------
int CObj__LBx_STD::__DEFINE__CONTROL_MODE(obj,l_mode)
{
	sObject_Name = obj;

	// ...
	{
		ADD__CTRL_VAR(sMODE__INIT, "INIT" );
		ADD__CTRL_VAR(sMODE__MAINT,"MAINT");

		ADD__CTRL_VAR(sMODE__DV_CLOSE, "DV_CLOSE");
		ADD__CTRL_VAR(sMODE__SV_CLOSE, "SV_CLOSE");

		ADD__CTRL_VAR(sMODE__PUMP, "PUMP");
		ADD__CTRL_VAR(sMODE__VENT, "VENT");

		//
		ADD__CTRL_VAR(sMODE__DOOR_OPEN,  "DOOR.OPEN");
		ADD__CTRL_VAR(sMODE__DOOR_CLOSE, "DOOR.CLOSE");

		ADD__CTRL_VAR(sMODE__SLOT_OPEN,  "SLOT.OPEN");
		ADD__CTRL_VAR(sMODE__SLOT_CLOSE, "SLOT.CLOSE");

		ADD__CTRL_VAR(sMODE__PIN_UP,   "PIN.UP");
		ADD__CTRL_VAR(sMODE__PIN_DOWN, "PIN.DOWN");

		//
		ADD__CTRL_VAR(sMODE__PREPMATER, "PREPMATER");
		ADD__CTRL_VAR(sMODE__COMPMATER, "COMPMATER");

		ADD__CTRL_VAR(sMODE__IN_PROC,  "IN_PROC");
		ADD__CTRL_VAR(sMODE__OUT_PROC, "OUT_PROC");

		ADD__CTRL_VAR(sMODE__ISOLATE,     "ISOLATE");
		ADD__CTRL_VAR(sMODE__LEAK_CHECK,  "LEAK_CHECK");
		ADD__CTRL_VAR(sMODE__CYCLE_PURGE, "CYCLE_PURGE");

		//
		ADD__CTRL_VAR(sMODE__PROC_READY, "PROC_READY");
		ADD__CTRL_VAR(sMODE__PROC_START, "PROC_START");

		ADD__CTRL_VAR(sMODE__PRE_READY,	"PRE_READY");
		ADD__CTRL_VAR(sMODE__PRE_START,	"PRE_START");

		ADD__CTRL_VAR(sMODE__CLN_READY,	"CLN_READY");
		ADD__CTRL_VAR(sMODE__CLN_START,	"CLN_START");

		ADD__CTRL_VAR(sMODE__MNT_READY,	"MNT_READY");
		ADD__CTRL_VAR(sMODE__MNT_START,	"MNT_START");
	}

	return 1;
}
int CObj__LBx_STD::__DEFINE__VERSION_HISTORY(version)
{

	return 1;
}


// ...
#define MON_ID__IO_MONITOR				1


int CObj__LBx_STD::__DEFINE__VARIABLE_STD(p_variable)
{
	DECLARE__STD_VARIABLE;

	CCommon_Error__DEF_VARIABLE m_err_def_variable;
	p_variable->Link__DEF_VARIABLE__ERROR_FEEDBACK(&m_err_def_variable);

	// ...
	CString str_name;
	CString dsc_item_list;

	// ...
	{
		p_variable->Get__STD_DESCRIPTION("STD_OBJ_STATUS",dsc_item_list);

		str_name = "OBJ.STATUS";
		STD__ADD_DIGITAL_WITH_COMMENT(str_name,dsc_item_list,"");
		LINK__VAR_DIGITAL_CTRL(dCH__OBJ_STATUS,str_name);

		//
		str_name = "PARA.SLOT.ID";
		STD__ADD_DIGITAL(str_name, "1 2");
		LINK__VAR_DIGITAL_CTRL(dCH__PARA_SLOT_ID, str_name);

		//
		str_name = "PARA.PREHEAT.TIME";
		STD__ADD_ANALOG_WITH_COMMENT(str_name,"sec",1,0,9999,"");
		LINK__VAR_ANALOG_CTRL(aCH__PARA_PREHEAT_TIME,str_name);

		str_name = "PARA.COOLING.TIME";
		STD__ADD_ANALOG_WITH_COMMENT(str_name,"sec",1,0,9999,"");
		LINK__VAR_ANALOG_CTRL(aCH__PARA_COOLING_TIME,str_name);

		str_name = "PARA.PREMATERIAL.SLOT";
		STD__ADD_ANALOG_WITH_COMMENT(str_name,"sec",1,0,9999,"");
		LINK__VAR_ANALOG_CTRL(aCH__PARA_PREMATERIAL_SLOT,str_name);

		str_name = "sOBJ.SEQ.MSG";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__OBJ_SEQ_MSG,str_name);

		//
		str_name = "PARA.PROC.RECIPE.NAME";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__PARA_PROC_RECIPE_NAME, str_name);
	}

	// ...
	{
		p_variable->Add__MONITORING_PROC(1.0, MON_ID__IO_MONITOR);
	}
	return 1;
}
int CObj__LBx_STD::__DEFINE__ALARM(p_alarm)
{

	return 1;
}

int CObj__LBx_STD::__Define__USER_FUNCTION(CII_DEFINE__FUNCTION *p_fnc_ctrl)
{
	p_fnc_ctrl->Link__ERROR_FEEDBACK(&mERROR__USER_FNC);

	return 1;
}

//-------------------------------------------------------------------------
int CObj__LBx_STD::__INITIALIZE__OBJECT(p_variable,p_ext_obj_create)
{
	CString def_name;
	CString obj_name;
	CString var_name;

	// OBJ : LLx_CHM ...
	{
		def_name = "OBJ.LLx_CHM";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, obj_name);

		pOBJ_CTRL__LLx_CHM = p_ext_obj_create->Create__OBJECT_CTRL(obj_name);

		// ...
		{
			var_name = "OTR.OUT.MON.OBJ.STATUS";
			LINK__EXT_VAR_STRING_CTRL(sEXT_CH__OBJ_STATUS, obj_name,var_name);

			//
			var_name = "PARA.SLOT.ID";
			LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__PARA_SLOT_ID, obj_name,var_name);

			//
			var_name = "OTR.IN.PARA.aPREHEAT.TIME";
			LINK__EXT_VAR_ANALOG_CTRL(aEXT_CH__PARA_PREHEAT_TIME, obj_name,var_name);

			var_name = "OTR.IN.PARA.aCOOLING.TIME";
			LINK__EXT_VAR_ANALOG_CTRL(aEXT_CH__PARA_COOLING_TIME, obj_name,var_name);

			var_name = "OTR.IN.PARA.aPREMATERIAL.SLOT";
			LINK__EXT_VAR_ANALOG_CTRL(aEXT_CH__PARA_PREMATERIAL_SLOT, obj_name,var_name);
		}
	}

	// OBJ : LLx_PROC ...
	{
		def_name = "OBJ.LLx_PROC";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, obj_name);

		if(obj_name.CompareNoCase("NO") == 0)		pOBJ_CTRL__LLx_PROC = NULL;
		else										pOBJ_CTRL__LLx_PROC = p_ext_obj_create->Create__OBJECT_CTRL(obj_name);

		if(pOBJ_CTRL__LLx_PROC == NULL)				bActive__LLx_PROC = FALSE;
		else										bActive__LLx_PROC = TRUE;
	}

	// OBJ : LLx_DB ...
	if(bActive__LLx_PROC)
	{
		def_name = "OBJ.LLx_DB";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, obj_name);

		//
		var_name = "SLOT01.RECIPE";
		LINK__EXT_VAR_STRING_CTRL(sEXT_CH__PARA_PROC_RCP_NAME, obj_name,var_name);
	}

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
	{
		dCH__OBJ_STATUS->Set__DATA(STR__MAINTMODE);
	}
	return 1;
}

//-------------------------------------------------------------------------
int CObj__LBx_STD::__CALL__CONTROL_MODE(mode,p_debug,p_variable,p_alarm)
{
	int flag = -1;
	CString log_msg;

	// ...
	{
		CString para__slot_id;
		CString para__preheat_time;
		CString para__cooling_time;
		CString para__prematerial_slot;

		//
		para__slot_id = dCH__PARA_SLOT_ID->Get__STRING();
		dEXT_CH__PARA_SLOT_ID->Set__DATA(para__slot_id);

		//
		aCH__PARA_PREHEAT_TIME->Get__DATA(para__preheat_time);
		aEXT_CH__PARA_PREHEAT_TIME->Set__DATA(para__preheat_time);

		aCH__PARA_COOLING_TIME->Get__DATA(para__cooling_time);
		aEXT_CH__PARA_COOLING_TIME->Set__DATA(para__cooling_time);

		aCH__PARA_PREMATERIAL_SLOT->Get__DATA(para__prematerial_slot);
		aEXT_CH__PARA_PREMATERIAL_SLOT->Set__DATA(para__prematerial_slot);

		log_msg.Format("Start ... :  [%s] (%1d) (%s,%s,%s)",
						mode,
						para__slot_id,
						para__preheat_time,
						para__cooling_time,
						para__prematerial_slot);

		sCH__OBJ_SEQ_MSG->Set__DATA(log_msg);

		xLOG_CTRL->WRITE__LOG(log_msg);
	}

	//.....
	{
			 IF__CTRL_MODE(sMODE__INIT)				flag = Call__INIT(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__MAINT)			flag = Call__MAINT(p_variable);

		ELSE_IF__CTRL_MODE(sMODE__DV_CLOSE)			flag = Call__DV_CLOSE(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__SV_CLOSE)			flag = Call__SV_CLOSE(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__PUMP)				flag = Call__PUMP(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__VENT)				flag = Call__VENT(p_variable);

		ELSE_IF__CTRL_MODE(sMODE__DOOR_OPEN)		flag = Call__DOOR_OPEN(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__DOOR_CLOSE)		flag = Call__DOOR_CLOSE(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__SLOT_OPEN)		flag = Call__SLOT_OPEN(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__SLOT_CLOSE)		flag = Call__SLOT_CLOSE(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__PIN_UP)			flag = Call__PIN_UP(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__PIN_DOWN)			flag = Call__PIN_DOWN(p_variable);

		ELSE_IF__CTRL_MODE(sMODE__PREPMATER)		flag = Call__PREPMATER(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__COMPMATER)		flag = Call__COMPMATER(p_variable);

		ELSE_IF__CTRL_MODE(sMODE__IN_PROC)			flag = Call__IN_PROC(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__OUT_PROC)			flag = Call__OUT_PROC(p_variable);

		ELSE_IF__CTRL_MODE(sMODE__ISOLATE)			flag = Call__ISOLATE(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__LEAK_CHECK)		flag = Call__LEAK_CHECK(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__CYCLE_PURGE)		flag = Call__CYCLE_PURGE(p_variable);

		ELSE_IF__CTRL_MODE(sMODE__PROC_READY)		flag = Call__PROC_READY(p_variable, p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__PROC_START)		flag = Call__PROC_START(p_variable, p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__PRE_READY)		flag = Call__PRE_READY(p_variable, p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__PRE_START)		flag = Call__PRE_START(p_variable, p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__CLN_READY)		flag = Call__CLN_READY(p_variable, p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__CLN_START)		flag = Call__CLN_START(p_variable, p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__MNT_READY)		flag = Call__MNT_READY(p_variable, p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__MNT_START)		flag = Call__MNT_START(p_variable, p_alarm);
	}

	if((flag < 0)||(p_variable->Check__CTRL_ABORT() > 0))
	{
		dCH__OBJ_STATUS->Set__DATA("MAINTMODE");

		log_msg.Format("Aborted ... :  [%s]",mode);
		xLOG_CTRL->WRITE__LOG(log_msg);
	}
	else
	{
		log_msg.Format("Completed ... :  [%s]",mode);
		xLOG_CTRL->WRITE__LOG(log_msg);
	}

	sCH__OBJ_SEQ_MSG->Set__DATA(log_msg);
	return flag;
}
int CObj__LBx_STD::__CALL__MONITORING(id,p_variable,p_alarm)
{
	switch(id)
	{
		case MON_ID__IO_MONITOR:
			Mon__IO_MONITOR(p_variable,p_alarm);
			break;
	}

	return 1;
}
