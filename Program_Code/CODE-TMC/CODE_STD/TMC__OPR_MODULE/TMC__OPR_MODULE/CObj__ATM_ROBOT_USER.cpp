#include "StdAfx.h"
#include "CObj__ATM_ROBOT_USER.h"

#include "Macro_Function.h"


//-------------------------------------------------------------------------
CObj__ATM_ROBOT_USER::CObj__ATM_ROBOT_USER()
{

}
CObj__ATM_ROBOT_USER::~CObj__ATM_ROBOT_USER()
{

}

//-------------------------------------------------------------------------
int CObj__ATM_ROBOT_USER::__DEFINE__CONTROL_MODE(obj,l_mode)
{
	sObject_Name = obj;

	// ...
	{
		ADD__CTRL_VAR(sMODE__INIT, "INIT");
		ADD__CTRL_VAR(sMODE__HOME, "HOME");

		ADD__CTRL_VAR(sMODE__ROTATE, "ROTATE");
		ADD__CTRL_VAR(sMODE__PICK,   "PICK" );
		ADD__CTRL_VAR(sMODE__PLACE,  "PLACE");

		ADD__CTRL_VAR(sMODE__ALIGN, "ALIGN");
		ADD__CTRL_VAR(sMODE__ALIGN_PICK, "ALIGN_PICK");

		ADD__CTRL_VAR(sMODE__LP_MAP, "LP_MAP");
	}
	return 1;
}
int CObj__ATM_ROBOT_USER::__DEFINE__VERSION_HISTORY(version)
{

	return 1;
}


// ...
#define APP_DSP__PARA_ARM				\
"A  B  AB"

#define APP_DSP__PARA_MODULE			\
"LP1  LP2  LP3  LP4  LP5				\
LBA  LBB  LBC  LBD 						\
AL1										\
BUFF1 BUFF2"

#define APP_DSP__PARA_SLOT				\
"1  2  3  4  5  6  7  8  9  10			\
11 12 13 14 15 16 17 18 19 20			\
21 22 23 24 25"


int CObj__ATM_ROBOT_USER::__DEFINE__VARIABLE_STD(p_variable)
{
	DECLARE__STD_VARIABLE;

	CCommon_Error__DEF_VARIABLE m_err_def_variable;
	p_variable->Link__DEF_VARIABLE__ERROR_FEEDBACK(&m_err_def_variable);

	// ...
	CString str_name;

	// ...
	{
		str_name = "PARA.ARM";
		STD__ADD_DIGITAL(str_name,APP_DSP__PARA_ARM);
		LINK__VAR_DIGITAL_CTRL(dCH__PARA_ARM, str_name);

		str_name = "PARA.MODULE";
		STD__ADD_DIGITAL(str_name,APP_DSP__PARA_MODULE);
		LINK__VAR_DIGITAL_CTRL(dCH__PARA_MODULE, str_name);

		//
		str_name = "PARA.LPx.SLOT";
		STD__ADD_DIGITAL(str_name,APP_DSP__PARA_SLOT);
		LINK__VAR_DIGITAL_CTRL(dCH__PARA_LPx_SLOT, str_name);

		str_name = "PARA.STx.SLOT";
		STD__ADD_DIGITAL(str_name,APP_DSP__PARA_SLOT);
		LINK__VAR_DIGITAL_CTRL(dCH__PARA_STx_SLOT, str_name);

		str_name = "PARA.LLx.SLOT";
		STD__ADD_DIGITAL(str_name, "1 2");
		LINK__VAR_DIGITAL_CTRL(dCH__PARA_LLx_SLOT, str_name);

		str_name = "PARA.ALx.SLOT";
		STD__ADD_DIGITAL(str_name, "1");
		LINK__VAR_DIGITAL_CTRL(dCH__PARA_ALx_SLOT, str_name);
	}

	return 1;
}
int CObj__ATM_ROBOT_USER::__DEFINE__ALARM(p_alarm)
{

	return 1;
}

int CObj__ATM_ROBOT_USER::__Define__USER_FUNCTION(CII_DEFINE__FUNCTION *p_fnc_ctrl)
{
	p_fnc_ctrl->Link__ERROR_FEEDBACK(&mERROR__USER_FNC);

	return 1;
}

//-------------------------------------------------------------------------
int CObj__ATM_ROBOT_USER::__INITIALIZE__OBJECT(p_variable,p_ext_obj_create)
{
	CString def_name;
	CString def_data;
	CString str_name;

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

	// OBJ__ROBOT ...
	{
		def_name = "OBJ__ROBOT";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);

		pATM_ROBOT__OBJ_CTRL = p_ext_obj_create->Create__OBJECT_CTRL(def_data);

		// ...
		{
			str_name = "PARA.ARM";
			LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__PARA_ARM, def_data,str_name);

			str_name = "PARA.MODULE";
			LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__PARA_MODULE, def_data,str_name);

			str_name = "PARA.SLOT";
			LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__PARA_SLOT, def_data,str_name);
		}
	}

	return 1;
}

//-------------------------------------------------------------------------
int CObj__ATM_ROBOT_USER::__CALL__CONTROL_MODE(mode,p_debug,p_variable,p_alarm)
{
	CString para_arm;
	CString para_module;
	CString para_slot;

	// PARA.LINK ...
	{
		dCH__PARA_ARM->Get__DATA(para_arm);
		dEXT_CH__PARA_ARM->Set__DATA(para_arm);

		dCH__PARA_MODULE->Get__DATA(para_module);
		dEXT_CH__PARA_MODULE->Set__DATA(para_module);
	}

	// PARA.SLOT ...
	{
		int lp_index = Macro__CHECK_LPx_INDEX(para_module);
		if(lp_index >= 0)		para_slot = dCH__PARA_LPx_SLOT->Get__STRING();

		int st_index = Macro__CHECK_STx_INDEX(para_module);
		if(st_index >= 0)		para_slot = dCH__PARA_STx_SLOT->Get__STRING();

		int ll_index = Macro__CHECK_LLx_INDEX(para_module);
		if(ll_index >= 0)		para_slot = dCH__PARA_LLx_SLOT->Get__STRING();

		if(para_module.CompareNoCase("AL1") == 0)
		{
			para_slot = dCH__PARA_ALx_SLOT->Get__STRING();
		}

		dEXT_CH__PARA_SLOT->Set__DATA(para_slot);
	}

	// ...
	{
		CString log_msg;

		log_msg.Format("[%s] Start ... (%s %s %s)",
						mode,
						para_arm,
						para_module,
						para_slot);

		xLOG_CTRL->WRITE__LOG(log_msg);
	}

	int flag = -1;

	// ...
	{
			 IF__CTRL_MODE(sMODE__INIT)				flag = Call__INIT(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__HOME)				flag = Call__HOME(p_variable);

		ELSE_IF__CTRL_MODE(sMODE__ROTATE)			flag = Call__ROTATE(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__PICK)				flag = Call__PICK(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__PLACE)			flag = Call__PLACE(p_variable);

		ELSE_IF__CTRL_MODE(sMODE__ALIGN)			flag = Call__ALIGN(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__ALIGN_PICK)		flag = Call__ALIGN_PICK(p_variable);

		ELSE_IF__CTRL_MODE(sMODE__LP_MAP)			flag = Call__LP_MAP(p_variable);
	}

	if((flag < 0)||(p_variable->Check__CTRL_ABORT() > 0))
	{
		// ...
		{
			CString log_msg;

			log_msg.Format("Aborted ... :  [%s]", mode);
			xLOG_CTRL->WRITE__LOG(log_msg);
		}
	}
	else
	{
		// ...
		{
			CString log_msg;

			log_msg.Format("Completed ... :  [%s]",mode);
			xLOG_CTRL->WRITE__LOG(log_msg);
		}
	}

	return flag;
}
int CObj__ATM_ROBOT_USER::__CALL__MONITORING(id,p_variable,p_alarm)
{

	return 1;
}
