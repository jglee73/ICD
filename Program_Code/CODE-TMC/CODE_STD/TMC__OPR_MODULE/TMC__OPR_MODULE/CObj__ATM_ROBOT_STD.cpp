#include "StdAfx.h"
#include "CObj__ATM_ROBOT_STD.h"


//-------------------------------------------------------------------------
CObj__ATM_ROBOT_STD::CObj__ATM_ROBOT_STD()
{

}
CObj__ATM_ROBOT_STD::~CObj__ATM_ROBOT_STD()
{

}

//-------------------------------------------------------------------------
int CObj__ATM_ROBOT_STD::__DEFINE__CONTROL_MODE(obj,l_mode)
{
	sObject_Name = obj;

	// ...
	{
		ADD__CTRL_VAR(sMODE__INIT,   "INIT" );
		ADD__CTRL_VAR(sMODE__MAINT,  "MAINT");

		ADD__CTRL_VAR(sMODE__PICK,   "PICK" );
		ADD__CTRL_VAR(sMODE__XPICK,  "XPICK");

		ADD__CTRL_VAR(sMODE__PLACE,  "PLACE" );
		ADD__CTRL_VAR(sMODE__XPLACE, "XPLACE");

		ADD__CTRL_VAR(sMODE__ROTATE, "ROTATE");

		ADD__CTRL_VAR(sMODE__MAP,     "MAP");
		
		ADD__CTRL_VAR(sMODE__HOME,        "HOME");
		ADD__CTRL_VAR(sMODE__VACOFF_HOME, "VAC_OFF.HOME");

		ADD__CTRL_VAR(sMODE__VACON,   "VACON");
		ADD__CTRL_VAR(sMODE__VACOFF,  "VACOFF");
		ADD__CTRL_VAR(sMODE__RETRACT, "RETRACT");
		ADD__CTRL_VAR(sMODE__EXTEND,  "EXTEND");
		ADD__CTRL_VAR(sMODE__GOUP,    "GOUP");
		ADD__CTRL_VAR(sMODE__GODOWN,  "GODOWN");

		ADD__CTRL_VAR(sMODE__PLACE_ALGN_PICK, "PLACE_PICK");
		ADD__CTRL_VAR(sMODE__ALGN,      "ALGN");
		ADD__CTRL_VAR(sMODE__ALGNPICK,  "ALGNPICK");
		ADD__CTRL_VAR(sMODE__ALGNPLACE, "ALGNPLACE");
		ADD__CTRL_VAR(sMODE__LP_MAP,    "LP_MAP");
	}
	return 1;
}
int CObj__ATM_ROBOT_STD::__DEFINE__VERSION_HISTORY(version)
{

	return 1;
}


// ...
#define MON_ID__HYPER_TERMINAL_INTERLOCK			1


// ...
#define APP_DSP__PARA_ARM				\
"A  B  AB"

#define APP_DSP__PARA_MODULE			\
"LP1  LP2  LP3  LP4  LP5				\
 LBA  LBB  LBC  LBD 					\
AL1  AL2								\
CL1  CL2								\
VIS1									\
BUFF1 BUFF2 FULL_BUFF"

#define APP_DSP__PARA_SLOT				\
"1  2  3  4  5  6  7  8  9  10			\
11 12 13 14 15 16 17 18 19 20			\
21 22 23 24 25 26 27 28 29 30"


int CObj__ATM_ROBOT_STD::__DEFINE__VARIABLE_STD(p_variable)
{
	DECLARE__STD_VARIABLE;

	CCommon_Error__DEF_VARIABLE m_err_def_variable;
	p_variable->Link__DEF_VARIABLE__ERROR_FEEDBACK(&m_err_def_variable);

	// ...
	CString dsc_item_list;
	CString str_name;

	// ...
	{
		p_variable->Get__STD_DESCRIPTION("STD_OBJ_STATUS",dsc_item_list);

		str_name = "OBJ.STATUS";
		STD__ADD_DIGITAL(str_name,dsc_item_list);
		LINK__VAR_DIGITAL_CTRL(xCH__OBJ_STATUS,str_name);

		str_name = "OBJ.MSG";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__OBJ_MSG, str_name);

		//
		str_name = "PARA.ARM";
		STD__ADD_DIGITAL(str_name,APP_DSP__PARA_ARM);
		LINK__VAR_DIGITAL_CTRL(xCH__PARA_ARM,str_name);

		str_name = "PARA.MODULE";
		STD__ADD_DIGITAL(str_name,APP_DSP__PARA_MODULE);
		LINK__VAR_DIGITAL_CTRL(xCH__PARA_MODULE,str_name);

		str_name = "PARA.SLOT";
		STD__ADD_DIGITAL(str_name,APP_DSP__PARA_SLOT);
		LINK__VAR_DIGITAL_CTRL(xCH__PARA_SLOT,str_name);
	}

	// ...
	{
		p_variable->Add__MONITORING_PROC(1.0, MON_ID__HYPER_TERMINAL_INTERLOCK);
	}
	return 1;
}
int CObj__ATM_ROBOT_STD::__DEFINE__ALARM(p_alarm)
{

	return 1;
}

int CObj__ATM_ROBOT_STD::__Define__USER_FUNCTION(CII_DEFINE__FUNCTION *p_fnc_ctrl)
{
	p_fnc_ctrl->Link__ERROR_FEEDBACK(&mERROR__USER_FNC);

	return 1;
}

//-------------------------------------------------------------------------
int CObj__ATM_ROBOT_STD::__INITIALIZE__OBJECT(p_variable,p_ext_obj_create)
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

		pATM_ROBOT__OBJ_CTRL = p_ext_obj_create->Create__OBJECT_CTRL(def_data);

		//
		str_name = "OBJ.STATUS";
		LINK__EXT_VAR_STRING_CTRL(xEXT_CH__OBJ_STATUS, def_data,str_name);

		str_name = "PARA.ARM";
		LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__PARA_ARM, def_data,str_name);

		str_name = "PARA.MODULE";
		LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__PARA_MODULE, def_data,str_name);

		str_name = "PARA.SLOT";
		LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__PARA_SLOT, def_data,str_name);
	}

	// ...
	{
		CString obj_name, var_name;

		p_ext_obj_create->Get__DEF_CONST_DATA("dHYPER_TERMINAL_LOCK", def_data);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name, var_name);
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_PHY__IO_CH__HYPER_TERMINAL_LOCK, obj_name, var_name);
	}

	// ...
	{
		xCH__OBJ_STATUS->Set__DATA(STR__MAINTMODE);
	}
	return 1;
}

//-------------------------------------------------------------------------
int CObj__ATM_ROBOT_STD::__CALL__CONTROL_MODE(mode,p_debug,p_variable,p_alarm)
{
	CString para_arm;
	CString para_module;
	CString para_slot;

	// ...
	{
		xCH__PARA_ARM->Get__DATA(para_arm);
		xEXT_CH__PARA_ARM->Set__DATA(para_arm);

		xCH__PARA_MODULE->Get__DATA(para_module);
		xEXT_CH__PARA_MODULE->Set__DATA(para_module);

		xCH__PARA_SLOT->Get__DATA(para_slot);
		xEXT_CH__PARA_SLOT->Set__DATA(para_slot);
	}

	// ...
	{
		CString obj_sts;

		xCH__OBJ_STATUS->Get__DATA(obj_sts);
		xEXT_CH__OBJ_STATUS->Set__DATA(obj_sts);

		// ...
		CString log_msg;

		log_msg.Format("Start ... :  [%s] (%s,%s,%s) - (%s)",
						mode,
						para_arm,
						para_module,
						para_slot,
						obj_sts);

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
		     IF__CTRL_MODE(sMODE__INIT)			flag = Call__INIT(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__MAINT)		flag = Call__MAINT(p_variable);

		ELSE_IF__CTRL_MODE(sMODE__PICK)			flag = Call__PICK(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__XPICK)		flag = Call__XPICK(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__PLACE)		flag = Call__PLACE(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__XPLACE)		flag = Call__XPLACE(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__ROTATE)		flag = Call__ROTATE(p_variable);

		ELSE_IF__CTRL_MODE(sMODE__MAP)			flag = Call__MAP(p_variable,p_alarm);
		
		ELSE_IF__CTRL_MODE(sMODE__HOME)			flag = Call__HOME(p_variable,p_alarm, -1);
		ELSE_IF__CTRL_MODE(sMODE__VACOFF_HOME)	flag = Call__HOME(p_variable,p_alarm,  1);

		ELSE_IF__CTRL_MODE(sMODE__VACON)		flag = Call__VACON(p_variable,p_alarm,para_arm);
		ELSE_IF__CTRL_MODE(sMODE__VACOFF)		flag = Call__VACOFF(p_variable,p_alarm,para_arm);

		ELSE_IF__CTRL_MODE(sMODE__RETRACT)		flag = Call__RETRACT(p_variable);	
		ELSE_IF__CTRL_MODE(sMODE__EXTEND)		flag = Call__EXTEND(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__GOUP)			flag = Call__GOUP(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__GODOWN)		flag = Call__GODOWN(p_variable);

		ELSE_IF__CTRL_MODE(sMODE__ALGN)					flag = Call__ALGN(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__ALGNPICK)				flag = Call__ALGNPICK(p_variable,p_alarm, para_arm,para_module,para_slot);
		ELSE_IF__CTRL_MODE(sMODE__PLACE_ALGN_PICK)		flag = Call__PLACE_ALIGN_PICK(p_variable, p_alarm,para_arm,para_module,para_slot);
		ELSE_IF__CTRL_MODE(sMODE__ALGNPLACE)			flag = Call__ALGNPLACE(p_variable,p_alarm, para_arm,para_module,para_slot);

		ELSE_IF__CTRL_MODE(sMODE__LP_MAP)		flag = Call__LP_MAP(p_variable,p_alarm, para_arm,para_module,para_slot);
	}

	if((flag < 0)||(p_variable->Check__CTRL_ABORT() > 0))
	{
		xCH__OBJ_STATUS->Set__DATA("MAINTMODE");

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

			log_msg.Format("Completed ... :  [%s]",mode);
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
int CObj__ATM_ROBOT_STD::__CALL__MONITORING(id,p_variable,p_alarm)
{
	switch(id)
	{
		case MON_ID__HYPER_TERMINAL_INTERLOCK:
			Mon__HYPER_TERMINAL_INTERLOCK(p_variable,p_alarm);
			break;
	}

	return 1;
}


// ...
void CObj__ATM_ROBOT_STD::
Mon__HYPER_TERMINAL_INTERLOCK(CII_OBJECT__VARIABLE* p_variable,
							  CII_OBJECT__ALARM* p_alarm)
{
	SCX__TIMER_CTRL cx_timer_ctrl;
	cx_timer_ctrl->REGISTER__ABORT_OBJECT(sObject_Name);

	while(1)
	{
		cx_timer_ctrl->WAIT(0.5);

		if(xCH__OBJ_STATUS->Check__DATA(STR__MAINTMODE) < 0)
		{
			dEXT_PHY__IO_CH__HYPER_TERMINAL_LOCK->Set__DATA("YES");
		}
	}	
}
