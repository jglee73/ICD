#include "StdAfx.h"
#include "CObj__TMC_SYS_STD.h"


//-------------------------------------------------------------------------
CObj__TMC_SYS_STD::CObj__TMC_SYS_STD()
{

}
CObj__TMC_SYS_STD::~CObj__TMC_SYS_STD()
{

}

//-------------------------------------------------------------------------
int CObj__TMC_SYS_STD::__DEFINE__CONTROL_MODE(obj,l_mode)
{
	sObject_Name = obj;

	// ...
	{
		ADD__CTRL_VAR(sMODE__SYSTEM_INIT,  "SYSTEM_INIT");
		ADD__CTRL_VAR(sMODE__SYSTEM_MAINT, "SYSTEM_MAINT");
	}
	return 1;
}
int CObj__TMC_SYS_STD::__DEFINE__VERSION_HISTORY(version)
{

	return 1;
}

int CObj__TMC_SYS_STD::__DEFINE__VARIABLE_STD(p_variable)
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

		for(int i=0; i<INIT_MSG_MAX; i++)
		{
			str_name.Format("GEN.Message_Initial%02d", i+1);
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__INIT_SEQ_MSG[i],str_name);
		}
	}
	return 1;
}
int CObj__TMC_SYS_STD::__DEFINE__ALARM(p_alarm)
{

	return 1;
}

int CObj__TMC_SYS_STD::__Define__USER_FUNCTION(CII_DEFINE__FUNCTION *p_fnc_ctrl)
{
	p_fnc_ctrl->Link__ERROR_FEEDBACK(&mERROR__USER_FNC);

	return 1;
}

//-------------------------------------------------------------------------
int CObj__TMC_SYS_STD::__INITIALIZE__OBJECT(p_variable,p_ext_obj_create)
{
	CString def_name;
	CString def_data;
	CString str_name;
	CString var_data;

	CString obj_name, var_name;
	int i;

	// ...
	{
		def_name = "EXECUTE_MSG_POPUP";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name, var_name);
		LINK__EXT_VAR_STRING_CTRL(sEXT_CH__GUI_POPUP_INIT_SEQ_MSG, obj_name, var_name);
	}

	// DB_CFG
	{
		def_name = "PM_SIZE";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);

		m_nPM_SIZE = atoi(def_data);
		if(m_nPM_SIZE > CFG_PM_LIMIT)		m_nPM_SIZE = CFG_PM_LIMIT;
	}

	// TMC_CHM
	{
		def_name = "OBJ__TMC_CHM";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);

		pTMC_CHM__OBJ_CTRL = p_ext_obj_create->Create__OBJECT_CTRL(def_data);
	}

	// ATM_ROBOT
	{
		def_name = "OBJ__ATM_ROBOT";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);

		pATM_ROBOT__OBJ_CTRL = p_ext_obj_create->Create__OBJECT_CTRL(def_data);
	}

	// TMC_PUMP
	{
		def_name = "OBJ__TMC_PUMP";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);

		pOBJ_CTRL__TMC_PUMP = p_ext_obj_create->Create__OBJECT_CTRL(def_data);
	}
	// LLx_PUMP
	{
		def_name = "OBJ__LLx_PUMP";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);

		pOBJ_CTRL__LLx_PUMP = p_ext_obj_create->Create__OBJECT_CTRL(def_data);
	}

	// VAC_ROBOT
	{
		def_name = "OBJ__VAC_ROBOT";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);

		pVAC_ROBOT__OBJ_CTRL = p_ext_obj_create->Create__OBJECT_CTRL(def_data);
	}

	// AL1
	{
		def_name = "OBJ__AL1";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);

		pAL1__OBJ_CTRL = p_ext_obj_create->Create__OBJECT_CTRL(def_data);
	}

	// LBA
	{
		def_name = "OBJ__LBA";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);

		pLBA__OBJ_CTRL = p_ext_obj_create->Create__OBJECT_CTRL(def_data);
	}

	// LBB
	{
		def_name = "OBJ__LBB";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);

		pLBB__OBJ_CTRL = p_ext_obj_create->Create__OBJECT_CTRL(def_data);
	}

	/*
	// VIS1
	{
		def_name = "OBJ__VIS1";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);

		pVIS1__OBJ_CTRL = p_ext_obj_create->Create__OBJECT_CTRL(def_data);
	}
	*/

	// LPx
	{
		p_ext_obj_create->Get__DEF_CONST_DATA("LP_SIZE",var_data);

		iLPx_LIMIT = atoi(var_data);
		if(iLPx_LIMIT > CFG_LP_LIMIT)    iLPx_LIMIT = CFG_LP_LIMIT;

		for(i=0;i<iLPx_LIMIT;i++)
		{
			def_name.Format("OBJ__LP%1d",i+1);
			p_ext_obj_create->Get__DEF_CONST_DATA(def_name,def_data);

			pLPx__OBJ_CTRL[i] = p_ext_obj_create->Create__OBJECT_CTRL(def_data);

			str_name = "OBJ.STATUS";
			LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__LPx_OBJ_STATUS[i], def_data,str_name);
		}
	}

	// PMx Slot Valve
	{
		def_name = "OBJ__PMx_VLV";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);

		pPMx_SV__OBJ_CTRL = p_ext_obj_create->Create__OBJECT_CTRL(def_data);

		def_name = "OTR.IN.PARA.aPMC.ID";
		LINK__EXT_VAR_ANALOG_CTRL(aCH__PARA_PMC_ID, def_data, def_name);
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
	return 1;
}

//-------------------------------------------------------------------------
int CObj__TMC_SYS_STD::__CALL__CONTROL_MODE(mode,p_debug,p_variable,p_alarm)
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
		     IF__CTRL_MODE(sMODE__SYSTEM_INIT)		flag = Call__SYSTEM_INIT(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__SYSTEM_MAINT)		flag = Call__SYSTEM_MAINT(p_variable);

		else									
		{

		}
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
int CObj__TMC_SYS_STD::__CALL__MONITORING(id,p_variable,p_alarm)
{

	return 1;
}
