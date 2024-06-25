#include "StdAfx.h"
#include "CObj__VAC_ROBOT_STD.h"


//-------------------------------------------------------------------------
CObj__VAC_ROBOT_STD::CObj__VAC_ROBOT_STD()
{

}
CObj__VAC_ROBOT_STD::~CObj__VAC_ROBOT_STD()
{

}

//-------------------------------------------------------------------------
int CObj__VAC_ROBOT_STD::__DEFINE__CONTROL_MODE(obj,l_mode)
{
	sObject_Name = obj;

	// ...
	{
		ADD__CTRL_VAR(sMODE__INIT,  "INIT");
		ADD__CTRL_VAR(sMODE__MAINT, "MAINT");
		
		ADD__CTRL_VAR(sMODE__SET_PARA, "SET.PARA");

		ADD__CTRL_VAR(sMODE__PICK,  "PICK");
		ADD__CTRL_VAR(sMODE__XPICK, "XPICK");

		ADD__CTRL_VAR(sMODE__PLACE, "PLACE");
		ADD__CTRL_VAR(sMODE__XPLACE,"XPLACE");

		ADD__CTRL_VAR(sMODE__ROTATE,"ROTATE");

		ADD__CTRL_VAR(sMODE__HOME,  "HOME");
		ADD__CTRL_VAR(sMODE__MAP,   "MAP");

		ADD__CTRL_VAR(sMODE__RETRACT, "RETRACT");
		ADD__CTRL_VAR(sMODE__EXTEND,  "EXTEND");
		ADD__CTRL_VAR(sMODE__GOUP,    "GOUP");
		ADD__CTRL_VAR(sMODE__GODOWN,  "GODOWN");

		ADD__CTRL_VAR(sMODE__RQ_STN,  "RQ_STN");
		ADD__CTRL_VAR(sMODE__SET_STN, "SET_STN");
		ADD__CTRL_VAR(sMODE__TEACHED_CPTR_SAVE, "TEACHED_CPTR_SAVE");
	}
	return 1;
}
int CObj__VAC_ROBOT_STD::__DEFINE__VERSION_HISTORY(version)
{

	return 1;
}


// ...
#define  APP_DSP__PARA_ARM							\
"A  B  AB"

#define  APP_DSP__PARA_MODULE						\
"LBA  LBB  LBC  LBD									\
 LLL  LLU											\
 PM1   PM2   PM3   PM4   PM5   PM6"

#define  APP_DSP__PARA_SLOT							\
" 1  2  3  4  5  6  7  8  9 10						\
 11 12 13 14 15 16 17 18 19 20						\
 21 22 23 24 25 26 27 28 29 30"


int CObj__VAC_ROBOT_STD::__DEFINE__VARIABLE_STD(p_variable)
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

		str_name = "OBJ.MSG";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__OBJ_MSG, str_name);

		//
		str_name = "PARA.ARM";
		STD__ADD_DIGITAL_WITH_COMMENT(str_name,APP_DSP__PARA_ARM,"");
		LINK__VAR_DIGITAL_CTRL(dCH__PARA_ARM,str_name);

		str_name = "PARA.MODULE";
		STD__ADD_DIGITAL_WITH_COMMENT(str_name,APP_DSP__PARA_MODULE,"");
		LINK__VAR_DIGITAL_CTRL(dCH__PARA_MODULE,str_name);

		str_name = "PARA.SLOT";
		STD__ADD_DIGITAL_WITH_COMMENT(str_name,APP_DSP__PARA_SLOT,"");
		LINK__VAR_DIGITAL_CTRL(dCH__PARA_SLOT,str_name);
	}

	// ...
	{
		p_variable->Add__MONITORING_PROC(1.0, 1);
	}
	return 1;
}
int CObj__VAC_ROBOT_STD::__DEFINE__ALARM(p_alarm)
{

	return 1;
}

int CObj__VAC_ROBOT_STD::__Define__USER_FUNCTION(CII_DEFINE__FUNCTION *p_fnc_ctrl)
{
	p_fnc_ctrl->Link__ERROR_FEEDBACK(&mERROR__USER_FNC);

	return 1;
}

//-------------------------------------------------------------------------
int CObj__VAC_ROBOT_STD::__INITIALIZE__OBJECT(p_variable,p_ext_obj_create)
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

	// ...
	{
		def_name = "OBJ__ROBOT";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);

		pVAC_ROBOT__OBJ_CTRL = p_ext_obj_create->Create__OBJECT_CTRL(def_data);

		str_name = "PARA.ARM";
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__PARA_ARM, def_data,str_name);

		str_name = "PARA.MODULE";
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__PARA_MODULE, def_data,str_name);

		str_name = "PARA.SLOT";
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__PARA_SLOT, def_data,str_name);
	}

	// ...
	{
		CString obj_name;
		CString var_name;

		p_ext_obj_create->Get__DEF_CONST_DATA("dHYPER_TERMINAL_LOCK", def_data);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name, var_name);
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_PHY__IO_CH__HYPER_TERMINAL_LOCK, obj_name, var_name);
	}

	// ...
	{
		dCH__OBJ_STATUS->Set__DATA(STR__MAINTMODE);
	}
	return 1;
}

//-------------------------------------------------------------------------
int CObj__VAC_ROBOT_STD::__CALL__CONTROL_MODE(mode,p_debug,p_variable,p_alarm)
{
	CString para_arm;
	CString para_module;
	CString para_slot;

	dCH__PARA_ARM->Get__DATA(para_arm);
	dEXT_CH__PARA_ARM->Set__DATA(para_arm);

	dCH__PARA_MODULE->Get__DATA(para_module);
	dEXT_CH__PARA_MODULE->Set__DATA(para_module);

	dCH__PARA_SLOT->Get__DATA(para_slot);
	dEXT_CH__PARA_SLOT->Set__DATA(para_slot);

	// ...
	{
		CString log_msg;

		log_msg.Format("Start ... :  [%s] (%s,%s,%s)",
						mode,
						para_arm,
						para_module,
						para_slot);

		xLOG_CTRL->WRITE__LOG(log_msg);
	}

	// ...
	{
		CString obj_msg;

		obj_msg.Format("%s Started ...", mode);
		sCH__OBJ_MSG->Set__DATA(obj_msg);
	}

	int flag = -1;

	// ...
	{
		     IF__CTRL_MODE(sMODE__INIT)					flag = Call__INIT(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__MAINT)				flag = Call__MAINT(p_variable);

		ELSE_IF__CTRL_MODE(sMODE__SET_PARA)				flag = Call__SET_PARA(p_variable);

		ELSE_IF__CTRL_MODE(sMODE__PICK)					flag = Call__PICK(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__XPICK)				flag = Call__XPICK(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__PLACE)				flag = Call__PLACE(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__XPLACE)				flag = Call__XPLACE(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__ROTATE)				flag = Call__ROTATE(p_variable);

		ELSE_IF__CTRL_MODE(sMODE__HOME)					flag = Call__HOME(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__MAP)					flag = Call__MAP(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__RETRACT)				flag = Call__RETRACT(p_variable);				
		ELSE_IF__CTRL_MODE(sMODE__EXTEND)				flag = Call__EXTEND(p_variable);				
		ELSE_IF__CTRL_MODE(sMODE__GOUP)					flag = Call__GOUP(p_variable);				 	
		ELSE_IF__CTRL_MODE(sMODE__GODOWN)				flag = Call__GODOWN(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__RQ_STN)				flag = Call__RQ_STN(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__SET_STN)				flag = Call__SET_STN(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__TEACHED_CPTR_SAVE)	flag = Call__TEACHED_CPTR_SAVE(p_variable);
	}

	if((flag < 0)||(p_variable->Check__CTRL_ABORT() > 0))
	{
		dCH__OBJ_STATUS->Set__DATA("MAINTMODE");

		// ...
		{
			CString log_msg;

			log_msg.Format("Aborted ... :  [%s]", mode);
			xLOG_CTRL->WRITE__LOG(log_msg);
		}
		// ...
		{
			CString obj_msg;

			obj_msg.Format("%s Aborted ...", mode);
			sCH__OBJ_MSG->Set__DATA(obj_msg);
		}
	}
	else
	{
		// ...
		{
			CString log_msg;

			log_msg.Format("Completed ... :  [%s]", mode);
			xLOG_CTRL->WRITE__LOG(log_msg);
		}
		// ...
		{
			CString obj_msg;

			obj_msg.Format("%s Completed ...", mode);
			sCH__OBJ_MSG->Set__DATA(obj_msg);
		}
	}

	return flag;
}
int CObj__VAC_ROBOT_STD::__CALL__MONITORING(id,p_variable,p_alarm)
{
	switch(id)
	{
		case 1:
			Mon__HYPER_TERMINAL_INTERLOCK(p_variable,p_alarm);
			break;
	}

	return 1;
}
