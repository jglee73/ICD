#include "StdAfx.h"
#include "CObj__TMC_SYS_SETUP.h"


//-------------------------------------------------------------------------
CObj__TMC_SYS_SETUP::CObj__TMC_SYS_SETUP()
{

}
CObj__TMC_SYS_SETUP::~CObj__TMC_SYS_SETUP()
{

}

//-------------------------------------------------------------------------
int CObj__TMC_SYS_SETUP::__DEFINE__CONTROL_MODE(obj,l_mode)
{
	sObject_Name = obj;

	// ...
	{
		ADD__CTRL_VAR(sMODE__SETUP_DISABLE, "SETUP.DISABLE");
		ADD__CTRL_VAR(sMODE__SETUP_ENABLE,  "SETUP.ENABLE");
	}
	return 1;
}
int CObj__TMC_SYS_SETUP::__DEFINE__VERSION_HISTORY(version)
{

	return 1;
}

int CObj__TMC_SYS_SETUP::__DEFINE__VARIABLE_STD(p_variable)
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
	}
	return 1;
}
int CObj__TMC_SYS_SETUP::__DEFINE__ALARM(p_alarm)
{

	return 1;
}

int CObj__TMC_SYS_SETUP::__Define__USER_FUNCTION(CII_DEFINE__FUNCTION *p_fnc_ctrl)
{
	p_fnc_ctrl->Link__ERROR_FEEDBACK(&mERROR__USER_FNC);

	return 1;
}

//-------------------------------------------------------------------------
int CObj__TMC_SYS_SETUP::__INITIALIZE__OBJECT(p_variable,p_ext_obj_create)
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

	CString obj_name;
	CString var_name;

	// IO OBJ ...
	{
		obj_name = "IO";

		//
		var_name = "di.VAC.ROBOT.RNE";
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__VAC_RB__RNE, obj_name,var_name);
		
		var_name = "di.ATM.ROBOT.RNE";
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__ATM_RB__RNE, obj_name,var_name);

		//
		var_name = "di.CHM1.VAC.SNS";
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__CHM1_VAC_SNS, obj_name,var_name);

		var_name = "di.CHM1.ATM.SNS";
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__CHM1_ATM_SNS, obj_name,var_name);

		//
		var_name = "di.LBA.VAC.SNS";
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__LBA_VAC_SNS, obj_name,var_name);

		var_name = "di.LBA.ATM.SNS";
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__LBA_ATM_SNS, obj_name,var_name);

		//
		var_name = "di.LBB.VAC.SNS";
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__LBB_VAC_SNS, obj_name,var_name);

		var_name = "di.LBB.ATM.SNS";
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__LBB_ATM_SNS, obj_name,var_name);

		//
		var_name = "di.PM1.VAC.SNS";
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__PM1_VAC_SNS, obj_name,var_name);
		
		var_name = "di.PM2.VAC.SNS";
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__PM2_VAC_SNS, obj_name,var_name);
		
		var_name = "di.PM3.VAC.SNS";
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__PM3_VAC_SNS, obj_name,var_name);
		
		var_name = "di.PM4.VAC.SNS";
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__PM4_VAC_SNS, obj_name,var_name);

		//
		var_name = "di.PM1.ATM.SNS";
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__PM1_ATM_SNS, obj_name,var_name);

		var_name = "di.PM2.ATM.SNS";
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__PM2_ATM_SNS, obj_name,var_name);

		var_name = "di.PM3.ATM.SNS";
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__PM3_ATM_SNS, obj_name,var_name);

		var_name = "di.PM4.ATM.SNS";
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__PM4_ATM_SNS, obj_name,var_name);

		//
		var_name = "ai.CHM1.PRESSURE";
		LINK__EXT_VAR_ANALOG_CTRL(aEXT_CH__CHM1_PRESSURE, obj_name,var_name);
		
		var_name = "ai.LBA.PRESSURE";
		LINK__EXT_VAR_ANALOG_CTRL(aEXT_CH__LBA_PRESSURE, obj_name,var_name);

		var_name = "ai.LBB.PRESSURE";
		LINK__EXT_VAR_ANALOG_CTRL(aEXT_CH__LBB_PRESSURE, obj_name,var_name);
	}

	return 1;
}

//-------------------------------------------------------------------------
int CObj__TMC_SYS_SETUP::__CALL__CONTROL_MODE(mode,p_debug,p_variable,p_alarm)
{
	// ...
	{
		CString log_msg;

		log_msg.Format("Start ... :  [%s]", mode);
		xLOG_CTRL->WRITE__LOG(log_msg);
	}

	int flag = -1;

	// ...
	{
		     IF__CTRL_MODE(sMODE__SETUP_DISABLE)		flag = Call__SETUP_DISABLE(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__SETUP_ENABLE)			flag = Call__SETUP_ENABLE(p_variable);
	}

	if((flag < 0)||(p_variable->Check__CTRL_ABORT() > 0))
	{
		dCH__OBJ_STATUS->Set__DATA("MAINTMODE");

		CString log_msg;

		log_msg.Format("Aborted ... :  [%s]", mode);
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
int CObj__TMC_SYS_SETUP::__CALL__MONITORING(id,p_variable,p_alarm)
{

	return 1;
}
