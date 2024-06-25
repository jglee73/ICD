#include "StdAfx.h"
#include "CObj__VAC_ROBOT_EX.h"
#include "CObj__VAC_ROBOT_EX__ALID.h"
#include "CObj__VAC_ROBOT_EX__DEF.h"

#include "Macro_Function.h"


//-------------------------------------------------------------------------
CObj__VAC_ROBOT_EX::CObj__VAC_ROBOT_EX()
{

}
CObj__VAC_ROBOT_EX::~CObj__VAC_ROBOT_EX()
{

}

//-------------------------------------------------------------------------
int CObj__VAC_ROBOT_EX::__DEFINE__CONTROL_MODE(obj,l_mode)
{
	sObject_Name = obj;

	// ...
	{
		ADD__CTRL_VAR(sMODE__INIT,     "INIT");
		ADD__CTRL_VAR(sMODE__SET_PARA, "SET.PARA");

		ADD__CTRL_VAR(sMODE__PICK,    "PICK");
		ADD__CTRL_VAR(sMODE__XPICK,   "XPICK");

		ADD__CTRL_VAR(sMODE__PLACE,   "PLACE");
		ADD__CTRL_VAR(sMODE__XPLACE,  "XPLACE");

		ADD__CTRL_VAR(sMODE__ROTATE,  "ROTATE");

		ADD__CTRL_VAR(sMODE__MAP,     "MAP");
		ADD__CTRL_VAR(sMODE__HOME,    "HOME");

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
int CObj__VAC_ROBOT_EX::__DEFINE__VERSION_HISTORY(version)
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

#define APP_DSP__ARM_ACT				"RETRACT  EXTEND"
#define APP_DSP__SV_STATUS				"UNKNOWN  CLOSE  OPEN"
#define APP_DSP__ARM_DIRECTION			"UNI_DIRECTION  BI_DIRECTION"

#define APP_DSP__DISABLE_ENABLE			"DISABLE  ENABLE"

#define APP_DSP__PASSIVE_ACTIVE			"PASSIVE  ACTIVE"
#define APP_DSP__YES_NO					"YES  NO"


int CObj__VAC_ROBOT_EX::__DEFINE__VARIABLE_STD(p_variable)
{
	DECLARE__STD_VARIABLE;

	CCommon_Error__DEF_VARIABLE m_err_def_variable;
	p_variable->Link__DEF_VARIABLE__ERROR_FEEDBACK(&m_err_def_variable);

	// ...
	CString str_name;
	CString dsc_item_list;
	int i;

	// ...
	{
		str_name = "sOBJ.MESSAGE";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__OBJ_MESSAGE,str_name);
	}

	// ...
	{
		p_variable->Get__STD_DESCRIPTION("STD_OBJ_STATUS",dsc_item_list);

		str_name = "OBJ.STATUS";
		STD__ADD_DIGITAL_WITH_COMMENT(str_name,dsc_item_list,"");
		LINK__VAR_DIGITAL_CTRL(dCH__OBJ_STATUS, str_name);

		//
		str_name = "PARA.ARM";
		STD__ADD_DIGITAL_WITH_COMMENT(str_name,APP_DSP__PARA_ARM,"");
		LINK__VAR_DIGITAL_CTRL(dCH__PARA_ARM, str_name);

		str_name = "PARA.MODULE";
		STD__ADD_DIGITAL_WITH_COMMENT(str_name,APP_DSP__PARA_MODULE,"");
		LINK__VAR_DIGITAL_CTRL(dCH__PARA_MODULE, str_name);

		str_name = "PARA.SLOT";
		STD__ADD_DIGITAL_WITH_COMMENT(str_name,APP_DSP__PARA_SLOT,"");
		LINK__VAR_DIGITAL_CTRL(dCH__PARA_SLOT, str_name);

		//
		str_name = "PARA.MANUAL_MOVE.LLx.SLOT_CLOSE.SKIP";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__PARA_MANUAL_MOVE_LLx_SLOT_CLOSE_SKIP, str_name);

		str_name = "PARA.MANUAL_MOVE.PMx.SLOT_CLOSE.SKIP";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__PARA_MANUAL_MOVE_PMx_SLOT_CLOSE_SKIP, str_name);
	}

	// WAC ...
	{
		// FROM_CTC.WAC.PMX ...
		for(i=0; i<CFG_PM_LIMIT; i++)
		{
			int id = i + 1;

			str_name.Format("FROM_CTC.WAC.PM%1d.USE", id);
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__FROM_CTC_WAC_PM_USE_X[i], str_name);

			str_name.Format("FROM_CTC.WAC.PM%1d.PICK.CHECK", id);
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__FROM_CTC_WAC_PM_PICK_CHECK_X[i], str_name);

			str_name.Format("FROM_CTC.WAC.PM%1d.PLACE.CHECK", id);
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__FROM_CTC_WAC_PM_PLACE_CHECK_X[i], str_name);

			//
			str_name.Format("FROM_CTC.WAC.PM%1d.POS.ID", id);
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__FROM_CTC_WAC_PM_POS_ID_X[i], str_name);

			str_name.Format("FROM_CTC.WAC.PM%1d.POS.SLOT", id);
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__FROM_CTC_WAC_PM_POS_SLOT_X[i], str_name);

			//
			str_name.Format("FROM_CTC.WAC.PM%1d.DELAY.SEC", id);
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__FROM_CTC_WAC_PM_DELAY_SEC_X[i], str_name);
		}

		str_name = "FROM_CTC.WAC.PMC.WAFER.TYPE";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__FROM_CTC_WAC_PM_WAFER_TYPE, str_name);

		//
		str_name = "INFO.WAC.PMC.ACTIVE";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__INFO_WAC_PMC_ACTIVE, str_name);

		str_name = "INFO.WAC.PMC.STATE";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__INFO_WAC_PMC_STATE, str_name);

		str_name = "INFO.WAC.PMC.ARM.TYPE";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__INFO_WAC_PMC_ARM_TYPE, str_name);

		str_name = "INFO.WAC.PMC.WFR.INFO";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__INFO_WAC_PMC_WFR_INFO, str_name);

		str_name = "INFO.WAC.PMC.POS.ID";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__INFO_WAC_PMC_POS_ID, str_name);

		str_name = "INFO.WAC.PMC.POS.SLOT";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__INFO_WAC_PMC_POS_SLOT, str_name);

		str_name = "INFO.WAC.PMC.DELAY.COUNT";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__INFO_WAC_PMC_DELAY_COUNT, str_name);
	}

	// ...
	{
		str_name = "CFG.LLx_CLOSE.SEQUENCE_MODE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,APP_DSP__DISABLE_ENABLE,"");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_LLx_CLOSE_SEQUENCE_MODE,str_name);

		str_name = "CFG.PMx_CLOSE.SEQUENCE_MODE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,APP_DSP__DISABLE_ENABLE,"");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_PMx_CLOSE_SEQUENCE_MODE,str_name);
	}

	// CFG ...
	{
		// LLx ...
		for(i=0; i<CFG_LLx_LIMIT; i++)
		{
			int id = i + 1;

			str_name.Format("CFG.LL%1d.HANDSHAKE.MODE", id);
			STD__ADD_DIGITAL_WITH_X_OPTION(str_name, APP_DSP__PASSIVE_ACTIVE, "");
			LINK__VAR_DIGITAL_CTRL(dCH__CFG_LLx_HANDSHAKE_MODE[i], str_name);

			str_name.Format("CFG.LL%1d.ROBOT_Z_MOVE", id);
			STD__ADD_DIGITAL_WITH_X_OPTION(str_name, APP_DSP__YES_NO, "");
			LINK__VAR_DIGITAL_CTRL(dCH__CFG_LLx_ROBOT_Z_MOVE[i], str_name);
		}
	}

	return 1;
}


// ...
#define  ACT__ABORT								\
l_act.RemoveAll();								\
l_act.Add("ABORT");

#define  ACT__RETRY_ABORT						\
l_act.RemoveAll();								\
l_act.Add("RETRY");								\
l_act.Add("ABORT");

#define  ACT__CHECK_RETRY_ABORT					\
l_act.RemoveAll();								\
l_act.Add("CHECK_RETRY");						\
l_act.Add("ABORT");

#define  ACT__ABORT_RETRY_IGNORE				\
l_act.RemoveAll();								\
l_act.Add("ABORT");								\
l_act.Add("RETRY");								\
l_act.Add("IGNORE");


int CObj__VAC_ROBOT_EX::__DEFINE__ALARM(p_alarm)
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
		alarm_id = ALID__UNKNOWN_MODULE_NAME;

		alarm_title  = title;
		alarm_title += "Unknown Module Name !";

		alarm_msg = "";

		ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__PMx_SLIT_VALVE__NOT_CLOSE;

		alarm_title  = title;
		alarm_title += "PMC's Slit Valve is not closed !";

		alarm_msg = "";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ARM_A__MATERIAL_NONE_ERROR;

		alarm_title  = title;
		alarm_title += "There is no material on Robot's A-Arm.\n";

		alarm_msg  = "Please, check material on Robot's A-Arm.\n";
		alarm_msg += "PHY_VAC_ROBOT+OTR.OUT.MON.dARM_A.MATERIAL.STATUS";

		ACT__ABORT_RETRY_IGNORE;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}
	// ...
	{
		alarm_id = ALID__ARM_B__MATERIAL_NONE_ERROR;

		alarm_title  = title;
		alarm_title += "There is no material on Robot's B-Arm.\n";

		alarm_msg = "Please, check material on Robot's B-Arm.\n";
		alarm_msg += "PHY_VAC_ROBOT+OTR.OUT.MON.dARM_B.MATERIAL.STATUS";

		ACT__ABORT_RETRY_IGNORE;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ARM_A__MATERIAL_EXIST_ERROR;

		alarm_title  = title;
		alarm_title += "There is material on Robot's A-Arm.";

		alarm_msg = "Please, check material on Robot's A-Arm.\n";
		alarm_msg += "PHY_VAC_ROBOT+OTR.OUT.MON.dARM_A.MATERIAL.STATUS";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ARM_B__MATERIAL_EXIST_ERROR;

		alarm_title  = title;
		alarm_title += "There is material on Robot's B-Arm.";

		alarm_msg = "Please, check material on Robot's B-Arm.\n";
		alarm_msg += "PHY_VAC_ROBOT+OTR.OUT.MON.dARM_B.MATERIAL.STATUS";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__PMx__MATERIAL_NONE_ERROR;

		alarm_title  = title;
		alarm_title += "There is no material in PMC.";

		alarm_msg = "Please, check material in PMC.\n";

		ACT__ABORT_RETRY_IGNORE;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__PMx__MATERIAL_EXIST_ERROR;

		alarm_title  = title;
		alarm_title += "There is material in PMC.";

		alarm_msg = "Please, check material in PMC.\n";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__PMx__NOT_TRANSFER_READY_TO_PICK;

		alarm_title  = title;
		alarm_title += "Target PM's transfer state is not ready to pick.";

		alarm_msg = "Please, check transfer ready state.\n";

		ACT__CHECK_RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__PMx__NOT_TRANSFER_READY_TO_PLACE;

		alarm_title  = title;
		alarm_title += "Target PM's transfer state is not ready to place.";

		alarm_msg = "Please, check transfer ready state.\n";

		ACT__CHECK_RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__PMx__NOT_PIN_UP_ERROR;

		alarm_title  = title;
		alarm_title += "Target PM's PIN not Up.";

		alarm_msg = "Please, check PIN State.\n";

		ACT__CHECK_RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__PMx__NOT_PIN_DOWN_ERROR;

		alarm_title  = title;
		alarm_title += "Target PM's PIN not Down.";

		alarm_msg = "Please, check PIN State.\n";

		ACT__CHECK_RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}
	return 1;
}

int CObj__VAC_ROBOT_EX::__Define__USER_FUNCTION(CII_DEFINE__FUNCTION *p_fnc_ctrl)
{
	p_fnc_ctrl->Link__ERROR_FEEDBACK(&mERROR__USER_FNC);

	return 1;
}

//-------------------------------------------------------------------------
int CObj__VAC_ROBOT_EX::__INITIALIZE__OBJECT(p_variable,p_ext_obj_create)
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
	CString obj_name;
	int i;

	// ROBOT ...
	{
		def_name = "OBJ__ROBOT";
		p_variable->Get__DEF_CONST_DATA(def_name,def_data);

		pROBOT__OBJ_CTRL = p_ext_obj_create->Create__OBJECT_CTRL(def_data);

		// ANI ...
		{
			str_name = "OTR.OUT.MON.dACT.ARM";
			LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__PHY_ROBOT__MON_ACT_ARM, def_data,str_name);
			
			str_name = "OTR.OUT.MON.dTRG.ROTATE";
			LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__PHY_ROBOT__MON_TRG_ROT, def_data,str_name);
		}

		// Material Status ...
		{
			str_name = "OTR.OUT.MON.dARM_A.MATERIAL.STATUS";
			LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__PHY_ROBOT__ARM_A_MATERIAL_STATUS, def_data,str_name);

			str_name = "OTR.OUT.MON.dARM_B.MATERIAL.STATUS";
			LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__PHY_ROBOT__ARM_B_MATERIAL_STATUS, def_data,str_name);
		}
		// Material Title ...
		{
			str_name = "OTR.OUT.MON.sARM_A.MATERIAL.TITLE";
			LINK__EXT_VAR_STRING_CTRL(sEXT_CH__PHY_ROBOT__ARM_A_MATERIAL_TITLE, def_data,str_name);

			str_name = "OTR.OUT.MON.sARM_B.MATERIAL.TITLE";
			LINK__EXT_VAR_STRING_CTRL(sEXT_CH__PHY_ROBOT__ARM_B_MATERIAL_TITLE, def_data,str_name);
		}

		// PARA.ACTIVE_HANDOFF ...
		{
			str_name = "PARA.ACTIVE_HANDOFF.REQ";
			LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__PHY_ROBOT__PARA_ACTIVE_HANDOFF_REQ, def_data,str_name);

			str_name = "PARA.ACTIVE_HANDOFF.ACT";
			LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__PHY_ROBOT__PARA_ACTIVE_HANDOFF_ACT, def_data,str_name);
		}
	}

	// PMx SIZE ...
	{
		def_name = "PM_SIZE";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);
		
		iSIZE__PMx = atoi(def_data);
		if(iSIZE__PMx > CFG_PM_LIMIT)		iSIZE__PMx = CFG_PM_LIMIT;
	}
	// PMx HANDOFF ...
	{
		// ...
		{
			def_name = "DATA.PMx_HANDOFF.TYPE";
			p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);

			if(def_data.CompareNoCase("SINGLE") == 0)			bActive__Single_Handoff = true;
			else												bActive__Single_Handoff = false;
		}
		// ...
		{
			def_name = "DATA.PMx_HANDOFF.PASSIVE_USE";
			p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);

			if(def_data.CompareNoCase("YES") == 0)				bActive__PMx_Passive_Use = true;
			else												bActive__PMx_Passive_Use = false;
		}

		if(bActive__Single_Handoff)
		{
			def_name = "OBJ__PMx_ACTIVE_HANDOFF";
			p_ext_obj_create->Get__DEF_CONST_DATA(def_name, obj_name);

			pOPR_PMx_HANDOFF_CTRL = p_ext_obj_create->Create__OBJECT_CTRL(obj_name);

			str_name = "PARA.PMx.ID";
			LINK__EXT_VAR_ANALOG_CTRL(aEXT_CH__PARA_PMC_ID, obj_name,str_name);
		}
		else
		{
			for(i=0; i<iSIZE__PMx; i++)
			{
				def_name.Format("OBJ__PM%1d_ACTIVE_HANDOFF", i+1);
				p_ext_obj_create->Get__DEF_CONST_DATA(def_name,def_data);

				pOPR_PMC_HANDOFF_X[i] = p_ext_obj_create->Create__OBJECT_CTRL(def_data);
			}
		}
	}
	// PMx SV ...
	{
		def_name = "OBJ__PMC";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name,def_data);

		for(i=0; i<iSIZE__PMx; i++)
		{
			str_name.Format("CFG.PM%1d.HANDSHAKE.MODE", i+1);
			LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__PMx_CFG_HANDSHAKE_MODE[i], def_data,str_name);

			//
			str_name.Format("CFG.USE.SHUTTER.%1d", i+1);
			LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__PMx_CFG_USE_SHUTTER[i], def_data,str_name);

			str_name.Format("CFG.USE.LIFT.PIN.%1d", i+1);
			LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__PMx_CFG_USE_LIFT_PIN[i], def_data,str_name);

			str_name.Format("CFG.USE.CR.POSITION.%1d", i+1);
			LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__PMx_CFG_USE_CR_POSITION[i], def_data,str_name);

			//
			str_name.Format("MODULE.LINK.sPM%1d.SHUTTER.STS",i+1);
			LINK__EXT_VAR_STRING_CTRL(sEXT_CH__PMx_SHUTTER_STS[i], def_data,str_name);

			str_name.Format("MODULE.LINK.sPM%1d.LIFT.PIN.STS",i+1);
			LINK__EXT_VAR_STRING_CTRL(sEXT_CH__PMx_LIFT_PIN_POS_STS[i], def_data,str_name);

			str_name.Format("MODULE.LINK.sPM%1d.CR.POSITION.STS",i+1);
			LINK__EXT_VAR_STRING_CTRL(sEXT_CH__PMx_CR_POSITION_STS[i], def_data,str_name);

			//
			str_name.Format("OTR.OUT.MON.dPM%1d.SLOT01.STATUS",i+1);
			LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__PHY_PMx_SLOT01_STATUS[i], def_data,str_name);

			str_name.Format("OTR.OUT.MON.sPM%1d.SLOT01.TITLE",i+1);
			LINK__EXT_VAR_STRING_CTRL(sEXT_CH__PHY_PMx_SLOT01_TITLE[i], def_data,str_name);
		}
	}

	// DB_CFG ...
	{
		def_name = "OBJ__DB";
		p_variable->Get__DEF_CONST_DATA(def_name,def_data);

		//
		str_name = "SCH_TEST.CFG.PMC_DUMMY_BY_CTC";
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__SCH_TEST_CFG_PMC_DUMMY_BY_CTC, def_data,str_name);

		//
		str_name = "CFG.SETUP.TEST.MODE";
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__CFG_SETUP_TEST_MODE, def_data,str_name);

		str_name = "CFG.SETUP.AUTO.VENTING_PUMPING";
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__CFG_SETUP_AUTO_VENTING_PUMPING, def_data,str_name);

		str_name = "CFG.SETUP.LLx.DOOR_SLOT_VLV.CONTROL";
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__CFG_SETUP_LLx_DOOR_SLOT_VLV_CONTROL, def_data,str_name);

		//
		str_name = "CFG.TRANSFER.MODE";
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__CFG_TRANSFER_MODE, def_data,str_name);

		str_name = "CFG.REF.ATM.PRESSURE";
		LINK__EXT_VAR_ANALOG_CTRL(aEXT_CH__CFG_REF_ATM_PRESSURE, def_data,str_name);

		str_name = "CFG.REF.VAC.PRESSURE";
		LINK__EXT_VAR_ANALOG_CTRL(aEXT_CH__CFG_REF_VAC_PRESSURE, def_data,str_name);

		str_name = "CTC.CUR_MODE";
		LINK__EXT_VAR_STRING_CTRL(sEXT_CH__CTC_CUR_MODE, def_data,str_name);

		for(i=0; i<iSIZE__PMx; i++)
		{
			str_name.Format("CFG.PM%1d.CHM.TYPE", i+1);
			LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__CFG_PMx_CHM_TYPE[i], def_data,str_name);
		}
	}

	// VAC_CHM ...
	{
		def_name = "OBJ__VAC_CHM";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name,def_data);

		pVAC_CHM__OBJ_CTRL = p_ext_obj_create->Create__OBJECT_CTRL(def_data);

		// PARA ...
		{
			str_name = "PARA.BALLAST_CTRL.ACTIVE";
			LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__PARA_BALLAST_CTRL_ACTIVE, def_data,str_name);
		}
		// MON ...
		{
			str_name = "OTR.OUT.MON.dPRESSURE.STATUS";
			LINK__EXT_VAR_STRING_CTRL(sEXT_CH__VAC_CHM__PRESSURE_STATUS, def_data,str_name);

			str_name = "OTR.IN.MON.sROBOT.MATERIAL.TRANSFER.FLAG";
			LINK__EXT_VAR_STRING_CTRL(sEXT_CH__ROBOT_MATERIAL_TRANSFER_FLAG, def_data,str_name);
		}
	}

	// LLx ...
	{
		def_name = "ACTIVE.MANIFOLD_X";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name,def_data);
		
		if(def_data.CompareNoCase("YES") == 0)		bActive__LLx_MANIFOLD_X = true;
		else										bActive__LLx_MANIFOLD_X = false;

		//
		def_name = "DATA.LLx_SIZE";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name,def_data);

		iSIZE__LLx = atoi(def_data);
		if(iSIZE__LLx > CFG_LLx_LIMIT)		iSIZE__LLx = CFG_LLx_LIMIT;

		for(i=0; i<iSIZE__LLx; i++)
		{
			// FNC_LLx ...
			{
				def_name.Format("OBJ__FNC_LL%1d", i+1);
				p_ext_obj_create->Get__DEF_CONST_DATA(def_name, obj_name);

				pLLx__OBJ_CTRL[i] = p_ext_obj_create->Create__OBJECT_CTRL(obj_name);

				//
				str_name = "PARA.SLOT.ID";
				LINK__EXT_VAR_ANALOG_CTRL(aEXT_CH__LLx__PARA_SLOT_ID[i], obj_name,str_name);

				str_name = "PARA.HANDSHAKE.SLOT";
				LINK__EXT_VAR_ANALOG_CTRL(aEXT_CH__LLx__PARA_HANDSHAKE_SLOT[i], obj_name,str_name);

				//
				str_name = "OTR.OUT.MON.dPRESSURE.STATUS";
				LINK__EXT_VAR_STRING_CTRL(sEXT_CH__LLx__PRESSURE_STATUS[i], obj_name,str_name);

				str_name = "OTR.IN.MON.ROBOT.TRANSFER.FLAG";
				LINK__EXT_VAR_STRING_CTRL(sEXT_CH__LLx__ROBOT_TRANSFER_FLAG[i], obj_name,str_name);
			}

			// PHY_LLx ...
			{
				def_name.Format("OBJ__PHY_LL%1d", i+1);
				p_ext_obj_create->Get__DEF_CONST_DATA(def_name, obj_name);

				for(int k=0; k<CFG_LLx_SLOT_LIMIT; k++)
				{
					int id = k + 1;

					str_name.Format("OTR.OUT.MON.dSLOT%02d.STATUS", id);
					LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__LLx__SLOTx_STATUS[i][k], obj_name,str_name);

					str_name.Format("OTR.OUT.MON.sSLOT%02d.TITLE", id);
					LINK__EXT_VAR_STRING_CTRL(sEXT_CH__LLx__SLOTx_TITLE[i][k], obj_name,str_name);
				}
			}
		}
	}

	// TMC_VLV ...
	{
		def_name = "OBJ__TMC_VLV";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name,def_data);

		pTMC_VLV__OBJ_CTRL = p_ext_obj_create->Create__OBJECT_CTRL(def_data);

		str_name = "OTR.IN.PARA.aPMC.ID";
		LINK__EXT_VAR_ANALOG_CTRL(aEXT_CH__TMC_VLV__PMC_ID, def_data,str_name);
	}

	// ...
	{
		SCX__SEQ_INFO x_seq_info;

		iSIM_MODE = x_seq_info->Is__SIMULATION_MODE();
	}
	return 1;
}

//-------------------------------------------------------------------------
int CObj__VAC_ROBOT_EX::__CALL__CONTROL_MODE(mode,p_debug,p_variable,p_alarm)
{
	CString para_arm;
	CString para_module;
	CString para_slot;

	// ...
	{
		dCH__PARA_ARM->Get__DATA(para_arm);
		dCH__PARA_MODULE->Get__DATA(para_module);
		dCH__PARA_SLOT->Get__DATA(para_slot);
	}

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

	if((mode.CompareNoCase(sMODE__PICK)   == 0)
	|| (mode.CompareNoCase(sMODE__XPICK)  == 0)
	|| (mode.CompareNoCase(sMODE__PLACE)  == 0)
	|| (mode.CompareNoCase(sMODE__XPLACE) == 0))
	{
		sEXT_CH__ROBOT_MATERIAL_TRANSFER_FLAG->Set__DATA("YES");

		// LLx
		int ll_i = Macro__CHECK_LLx_INDEX(para_module);
		if(ll_i >= 0)
		{
			sEXT_CH__LLx__ROBOT_TRANSFER_FLAG[ll_i]->Set__DATA("YES");
		}

		// PMx
		int pm_i = Macro__CHECK_PMx_INDEX(para_module);
		if(pm_i >= 0)
		{

		}
	}

	// ...
	int flag = -1;

	if(iSIM_MODE > 0)
	{
		if(dEXT_CH__SCH_TEST_CFG_PMC_DUMMY_BY_CTC->Check__DATA(STR__YES) > 0)
			iSIM__PMC_DUMMY_MODE = 1;
		else
			iSIM__PMC_DUMMY_MODE = -1;
	}
	else
	{
		iSIM__PMC_DUMMY_MODE = -1;
	}

	// ...
	{
		int active_handoff__mode = NORMAL;

		int pm_i = Macro__CHECK_PMx_INDEX(para_module);
		if(pm_i >= 0)
		{
			if(dEXT_CH__PMx_CFG_HANDSHAKE_MODE[pm_i]->Check__DATA(STR__PASSIVE) < 0)
			{
				active_handoff__mode = ACTIVE_HANDOFF;
			}
		}

		IF__CTRL_MODE(sMODE__INIT)
		{
			flag = Call__INIT(p_variable,p_alarm);
		}
		ELSE_IF__CTRL_MODE(sMODE__SET_PARA)
		{
			flag = Call__SET_PARA(p_variable,p_alarm);
		}
		ELSE_IF__CTRL_MODE(sMODE__PICK)
		{
			flag = Call__PICK(p_variable,
								p_alarm,
								para_arm,
								para_module,
								para_slot,
								active_handoff__mode);
		}
		ELSE_IF__CTRL_MODE(sMODE__XPICK)
		{
			flag = Call__XPICK(p_variable,
								p_alarm,
								para_arm,
								para_module,
								para_slot,
								active_handoff__mode);
		}
		ELSE_IF__CTRL_MODE(sMODE__PLACE)
		{
			flag = Call__PLACE(p_variable,
								p_alarm,
								para_arm,
								para_module,
								para_slot,
								active_handoff__mode);
		}
		ELSE_IF__CTRL_MODE(sMODE__XPLACE)
		{
			flag = Call__XPLACE(p_variable,
								p_alarm,
								para_arm,
								para_module,
								para_slot,
								active_handoff__mode);
		}
		ELSE_IF__CTRL_MODE(sMODE__ROTATE)
		{
			flag = Call__ROTATE(p_variable,
								p_alarm,
								para_arm,
								para_module,
								para_slot);
		}
		ELSE_IF__CTRL_MODE(sMODE__HOME)
		{
			flag = Call__HOME(p_variable,p_alarm);
		}
		ELSE_IF__CTRL_MODE(sMODE__MAP)
		{
			flag = Call__MAP(p_variable,p_alarm);
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
		ELSE_IF__CTRL_MODE(sMODE__RQ_STN)
		{
			flag = Call__RQ_STN(p_variable,
								p_alarm,
								para_arm,
								para_module);
		}
		ELSE_IF__CTRL_MODE(sMODE__SET_STN)
		{
			flag = Call__SET_STN(p_variable,
								p_alarm,
								para_arm,
								para_module);
		}
		ELSE_IF__CTRL_MODE(sMODE__TEACHED_CPTR_SAVE)
		{
			flag = Call__TEACHED_CPTR_SAVE(p_variable,
											p_alarm,
											para_arm,
											para_module);
		}
	}

	// ...
	{
		sEXT_CH__ROBOT_MATERIAL_TRANSFER_FLAG->Set__DATA("");

		int ll_i = Macro__CHECK_LLx_INDEX(para_module);
		if(ll_i >= 0)
		{
			sEXT_CH__LLx__ROBOT_TRANSFER_FLAG[ll_i]->Set__DATA("");
		}
	}

	// ...
	{
		sCH__PARA_MANUAL_MOVE_LLx_SLOT_CLOSE_SKIP->Set__DATA("END");
		sCH__PARA_MANUAL_MOVE_PMx_SLOT_CLOSE_SKIP->Set__DATA("END");
	}

	if((flag < 0)||(p_variable->Check__CTRL_ABORT() > 0))
	{
		dCH__OBJ_STATUS->Set__DATA("MAINTMODE");

		// ...
		{
			CString log_msg;
			log_msg.Format("Aborted (%1d) ... :  [%s]", flag, mode);

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

	m_sBEFORE_MODE = mode;
	return flag;
}
int CObj__VAC_ROBOT_EX::__CALL__MONITORING(id,p_variable,p_alarm)
{

	return 1;
}
