#include "StdAfx.h"
#include "CObj__ATM_ROBOT_STD.h"

#include "Macro_Function.h"


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
		ADD__CTRL_VAR(sMODE__INIT,  "INIT" );

		ADD__CTRL_VAR(sMODE__PICK,  "PICK" );
		ADD__CTRL_VAR(sMODE__XPICK, "XPICK");

		ADD__CTRL_VAR(sMODE__PLACE, "PLACE" );
		ADD__CTRL_VAR(sMODE__XPLACE,"XPLACE");

		ADD__CTRL_VAR(sMODE__ROTATE,"ROTATE");

		ADD__CTRL_VAR(sMODE__MAP,      "MAP");

		ADD__CTRL_VAR(sMODE__HOME,        "HOME");
		ADD__CTRL_VAR(sMODE__VACOFF_HOME, "VAC_OFF.HOME");

		ADD__CTRL_VAR(sMODE__VACON,    "VACON");
		ADD__CTRL_VAR(sMODE__VACOFF,   "VACOFF");
		ADD__CTRL_VAR(sMODE__RETRACT,  "RETRACT");
		ADD__CTRL_VAR(sMODE__EXTEND,   "EXTEND");
		ADD__CTRL_VAR(sMODE__GOUP,     "GOUP");
		ADD__CTRL_VAR(sMODE__GODOWN,   "GODOWN");

		ADD__CTRL_VAR(sMODE__ALGN,     "ALGN");
		ADD__CTRL_VAR(sMODE__ALGNPICK, "ALGNPICK");
		ADD__CTRL_VAR(sMODE__ALGNPLACE,"ALGNPLACE");
		ADD__CTRL_VAR(sMODE__PLACE_ALGNPICK, "PLACE_PICK");
		ADD__CTRL_VAR(sMODE__LP_MAP,   "LP_MAP");
	}
	return 1;
}
int CObj__ATM_ROBOT_STD::__DEFINE__VERSION_HISTORY(version)
{

	return 1;
}


// ...
#define APP_DSP__PARA_ARM				\
"A  B  AB"

#define APP_DSP__PARA_MODULE			\
"LP1  LP2  LP3  LP4  LP5				\
 LBA  LBB  LBC  LBD						\
AL1  AL2								\
CL1  CL2								\
VIS1									\
BUFF1 BUFF2 FULL_BUFF"

#define APP_DSP__PARA_SLOT				\
"1  2  3  4  5  6  7  8  9  10			\
11 12 13 14 15 16 17 18 19 20			\
21 22 23 24 25 26 27 28 29 30"

#define APP_DSP__ARM_ACT				\
"RETRACT  EXTEND"


int CObj__ATM_ROBOT_STD::__DEFINE__VARIABLE_STD(p_variable)
{
	DECLARE__STD_VARIABLE;

	CCommon_Error__DEF_VARIABLE m_err_def_variable;
	p_variable->Link__DEF_VARIABLE__ERROR_FEEDBACK(&m_err_def_variable);

	// ...
	CString str_name;
	CString dsc_item_list;

	// ...
	{
		str_name = "OBJ.STATUS";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(xCH__OBJ_STATUS, str_name);

		//
		str_name = "PARA.ARM";
		STD__ADD_DIGITAL(str_name,APP_DSP__PARA_ARM);
		LINK__VAR_DIGITAL_CTRL(xCH__PARA_ARM, str_name);

		str_name = "PARA.MODULE";
		STD__ADD_DIGITAL(str_name,APP_DSP__PARA_MODULE);
		LINK__VAR_DIGITAL_CTRL(xCH__PARA_MODULE, str_name);

		str_name = "PARA.SLOT";
		STD__ADD_DIGITAL(str_name,APP_DSP__PARA_SLOT);
		LINK__VAR_DIGITAL_CTRL(xCH__PARA_SLOT, str_name);

		//
		str_name = "PARA.MANUAL_MOVE.LLx.DOOR_CLOSE.SKIP";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__PARA_MANUAL_MOVE_LLx_DOOR_CLOSE_SKIP, str_name);
	}

	// CFG ...
	{
		str_name = "CFG.PLACE_FRONT_TO_LLx_DOOR";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__CFG_PLACE_FRONT_TO_LLx_DOOR, str_name);

		str_name = "CFG.PICK_FRONT_TO_LLx_DOOR";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__CFG_PICK_FRONT_TO_LLx_DOOR, str_name);

		//
		str_name = "CFG.MOVE_TO_LLx.DELAY.SEC";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__CFG_MOVE_TO_LLx_DELAY_SEC, str_name);

		str_name = "CFG.FRONT_LPx_ID";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__CFG_FRONT_LPx_ID, str_name);

		str_name = "CFG.FRONT_LPx_SLOT";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__CFG_FRONT_LPx_SLOT, str_name);
	}

	// INFO ...
	{
		str_name = "INFO.FUME_CLEAR.STATE";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__INFO_FUME_CLEAR_STATE, str_name);

		str_name = "INFO.FUME_CLEAR.POS";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__INFO_FUME_CLEAR_POS, str_name);

		str_name = "INFO.FUME_CLEAR.DELAY.COUNT";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__INFO_FUME_CLEAR_DELAY_COUNT, str_name);
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
	CString obj_name;
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

	// DB_CFG ...
	{
		def_name = "OBJ__DB";
		p_variable->Get__DEF_CONST_DATA(def_name,def_data);

		//
		str_name = "CFG.SETUP.TEST.MODE";
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__CFG_SETUP_TEST_MODE, def_data,str_name);

		str_name = "CFG.SETUP.AUTO.VENTING_PUMPING";
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__CFG_SETUP_AUTO_VENTING_PUMPING, def_data,str_name);
	}

	// ATM_RB
	{
		def_name = "OBJ__ROBOT";
		p_variable->Get__DEF_CONST_DATA(def_name,def_data);

		pATM_RB__OBJ_CTRL = p_ext_obj_create->Create__OBJECT_CTRL(def_data);

		//
		str_name = "OBJ.STATUS";
		LINK__EXT_VAR_STRING_CTRL(xEXT_CH__OBJ_STATUS, def_data,str_name);
	}

	// LLx ...
	{
		def_name = "DATA.LLx_SIZE";
		p_variable->Get__DEF_CONST_DATA(def_name, def_data);
		
		iSIZE__LLx = atoi(def_data);
		if(iSIZE__LLx > CFG_LLx_LIMIT)		iSIZE__LLx = CFG_LLx_LIMIT;

		//
		for(int i=0; i<iSIZE__LLx; i++)
		{
			def_name.Format("OBJ__LL%1d", i+1);
			p_variable->Get__DEF_CONST_DATA(def_name, obj_name);

			pLLx__OBJ_CTRL[i] = p_ext_obj_create->Create__OBJECT_CTRL(obj_name);

			//
			str_name = "PARA.SLOT.ID";
			LINK__EXT_VAR_ANALOG_CTRL(aEXT_CH__LLx__PARA_SLOT_ID[i], obj_name,str_name);
		}
	}

	// LP1 ~ LPx
	{
		def_name = "LP_SIZE";
		p_variable->Get__DEF_CONST_DATA(def_name,def_data);

		int limit = atoi(def_data);
		if(limit > CFG_LP_LIMIT)		limit = CFG_LP_LIMIT;
		
		iLPx_SIZE = limit;

		//
		for(int i=0; i<limit; i++)
		{
			def_name.Format("OBJ__LP%1d",i+1);
			p_variable->Get__DEF_CONST_DATA(def_name,def_data);

			pLPx__OBJ_CTRL[i] = p_ext_obj_create->Create__OBJECT_CTRL(def_data);

			// ...
			str_name = "OTR.OUT.MON.dDOOR.STATUS";
			LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__LPx_DOOR_STATUS[i], def_data,str_name);

			str_name = "OTR.OUT.MON.dFOUP.POS.STATUS";
			LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__LPx_FOUP_POS_STATUS[i], def_data,str_name);
		}
	}

	return 1;
}

//-------------------------------------------------------------------------
int CObj__ATM_ROBOT_STD::__CALL__CONTROL_MODE(mode,p_debug,p_variable,p_alarm)
{
	CString para_arm;
	CString para_module;
	CString para_slot;

	xCH__PARA_ARM->Get__DATA(para_arm);
	xCH__PARA_MODULE->Get__DATA(para_module);
	xCH__PARA_SLOT->Get__DATA(para_slot);

	// ...
	{
		CString obj_sts;

		xCH__OBJ_STATUS->Get__DATA(obj_sts);
		xEXT_CH__OBJ_STATUS->Set__DATA(obj_sts);

		// ...
		{
			CString log_msg;

			log_msg.Format("Start ... :  [%s] (%s,%s,%s) - (%s)",
							mode,
							para_arm,
							para_module,
							para_slot,
							obj_sts);

			xLOG_CTRL->WRITE__LOG(log_msg);
		}
	}

	int flag = -1;

	// ...
	{
		IF__CTRL_MODE(sMODE__INIT)
		{
			flag = Call__INIT(p_variable,p_alarm);
		}
		ELSE_IF__CTRL_MODE(sMODE__PICK)
		{
			flag = Call__PICK(p_variable,
								p_alarm,
								para_arm,
								para_module,
								para_slot);
		}
		ELSE_IF__CTRL_MODE(sMODE__XPICK)
		{
			flag = Call__XPICK(p_variable,
								p_alarm,
								para_arm,
								para_module,
								para_slot);
		}
		ELSE_IF__CTRL_MODE(sMODE__PLACE)
		{
			flag = Call__PLACE(p_variable,
								p_alarm,
								para_arm,
								para_module,
								para_slot);
		}
		ELSE_IF__CTRL_MODE(sMODE__XPLACE)
		{
			flag = Call__XPLACE(p_variable,
								p_alarm,
								para_arm,
								para_module,
								para_slot);
		}
		ELSE_IF__CTRL_MODE(sMODE__ROTATE)
		{
			flag = Call__ROTATE(p_variable,
								p_alarm,
								para_arm,
								para_module,
								para_slot);
		}

		ELSE_IF__CTRL_MODE(sMODE__MAP)
		{
			flag = Call__MAP(p_variable,p_alarm);
		}
		ELSE_IF__CTRL_MODE(sMODE__HOME)
		{
			flag = Call__HOME(p_variable,p_alarm, -1);
		}
		ELSE_IF__CTRL_MODE(sMODE__VACOFF_HOME)
		{
			flag = Call__HOME(p_variable,p_alarm, 1);
		}
		ELSE_IF__CTRL_MODE(sMODE__VACON)
		{
			flag = Call__VACON(p_variable,p_alarm,para_arm);
		}
		ELSE_IF__CTRL_MODE(sMODE__VACOFF)
		{
			flag = Call__VACOFF(p_variable,p_alarm,para_arm);
		}
		ELSE_IF__CTRL_MODE(sMODE__RETRACT)
		{
			flag = Call__RETRACT(p_variable,
								p_alarm,
								para_arm,
								para_module,
								para_slot);
		}
		ELSE_IF__CTRL_MODE(sMODE__EXTEND)
		{
			flag = Call__EXTEND(p_variable,
								p_alarm,
								para_arm,
								para_module,
								para_slot);
		}
		ELSE_IF__CTRL_MODE(sMODE__GOUP)
		{
			flag = Call__GOUP(p_variable,
								p_alarm,
								para_arm,
								para_module,
								para_slot);
		}
		ELSE_IF__CTRL_MODE(sMODE__GODOWN)
		{
			flag = Call__GODOWN(p_variable,
								p_alarm,
								para_arm,
								para_module,
								para_slot);
		}

		ELSE_IF__CTRL_MODE(sMODE__ALGN)
		{
			flag = Call__ALGN(p_variable,p_alarm);
		}
		ELSE_IF__CTRL_MODE(sMODE__ALGNPICK)
		{
			flag = Call__ALGNPICK(p_variable,
								p_alarm,
								para_arm,
								para_module,
								para_slot);
		}
		ELSE_IF__CTRL_MODE(sMODE__ALGNPLACE)
		{
			flag = Call__ALGNPLACE(p_variable,
									p_alarm,
									para_arm,
									para_module,
									para_slot);
		}
		ELSE_IF__CTRL_MODE(sMODE__PLACE_ALGNPICK)
		{
			flag = Call__PLACE_ALIGN_PICK(p_variable,
											p_alarm,
											para_arm,
											para_module,
											para_slot);
		}
		ELSE_IF__CTRL_MODE(sMODE__LP_MAP)
		{
			flag = Call__LP_MAP(p_variable,
								p_alarm,
								para_arm,
								para_module,
								para_slot);
		}
	}

	// ...
	{
		sCH__PARA_MANUAL_MOVE_LLx_DOOR_CLOSE_SKIP->Set__DATA("END");
	}

	if((flag < 0)||(p_variable->Check__CTRL_ABORT() > 0))
	{
		// ...
		{
			CString log_msg;
			log_msg.Format("Aborted ... :  [%s]",mode);

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
int CObj__ATM_ROBOT_STD::__CALL__MONITORING(id,p_variable,p_alarm)
{

	return 1;
}
