#pragma once

#include "Interface_Code.h"

#include "CCommon_Error.h"
#include "CCommon_System.h"


class CObj__ATM_ROBOT_STD : public __IOBJ__STD_TYPE
{
private:
	CCommon_Error__USER_FNC mERROR__USER_FNC;

	//------------------------------------------------------------------------------
	CString sObject_Name;

	SCX__USER_LOG_CTRL xLOG_CTRL;


	//------------------------------------------------------------------------------
	// INTERNAL PROPERTY

	CX__VAR_STRING_CTRL  xCH__OBJ_STATUS;

	CX__VAR_DIGITAL_CTRL xCH__PARA_ARM;
	CX__VAR_DIGITAL_CTRL xCH__PARA_MODULE;
	CX__VAR_DIGITAL_CTRL xCH__PARA_SLOT;

	CX__VAR_STRING_CTRL  sCH__PARA_MANUAL_MOVE_LLx_DOOR_CLOSE_SKIP;

	CX__VAR_STRING_CTRL  sCH__CFG_PLACE_FRONT_TO_LLx_DOOR;
	CX__VAR_STRING_CTRL  sCH__CFG_PICK_FRONT_TO_LLx_DOOR;
	CX__VAR_STRING_CTRL  sCH__CFG_MOVE_TO_LLx_DELAY_SEC;
	CX__VAR_STRING_CTRL  sCH__CFG_FRONT_LPx_ID;
	CX__VAR_STRING_CTRL  sCH__CFG_FRONT_LPx_SLOT;
	
	CX__VAR_STRING_CTRL  sCH__INFO_FUME_CLEAR_STATE;
	CX__VAR_STRING_CTRL  sCH__INFO_FUME_CLEAR_POS;
	CX__VAR_STRING_CTRL  sCH__INFO_FUME_CLEAR_DELAY_COUNT;


	//------------------------------------------------------------------------------
	// EXTERNAL PROPERTY

	// DB_CFG ...
	CX__VAR_DIGITAL_CTRL dEXT_CH__CFG_SETUP_TEST_MODE;
	CX__VAR_DIGITAL_CTRL dEXT_CH__CFG_SETUP_AUTO_VENTING_PUMPING;

	// ATM_RB ...
	CII_EXT_OBJECT__CTRL *pATM_RB__OBJ_CTRL;
	CX__VAR_STRING_CTRL  xEXT_CH__OBJ_STATUS;

	// LLx
	int iSIZE__LLx;
	CII_EXT_OBJECT__CTRL *pLLx__OBJ_CTRL[CFG_LLx_LIMIT];

	CX__VAR_ANALOG_CTRL  aEXT_CH__LLx__PARA_SLOT_ID[CFG_LLx_LIMIT];

	// LPx ...
	int iLPx_SIZE;
	CII_EXT_OBJECT__CTRL *pLPx__OBJ_CTRL[CFG_LP_LIMIT];

	CX__VAR_DIGITAL_CTRL dEXT_CH__LPx_DOOR_STATUS[CFG_LP_LIMIT];
	CX__VAR_DIGITAL_CTRL dEXT_CH__LPx_FOUP_POS_STATUS[CFG_LP_LIMIT];
	//


	//------------------------------------------------------------------------------
	CString sMODE__INIT;
	int  Call__INIT(CII_OBJECT__VARIABLE* p_variable,
					CII_OBJECT__ALARM* p_alarm);

	//
	CString sMODE__PICK;
	int  Call__PICK(CII_OBJECT__VARIABLE* p_variable,
					CII_OBJECT__ALARM* p_alarm,
					const CString& para_arm,
					const CString& para_module,
					const CString& para_slot);

	CString sMODE__XPICK;
	int  Call__XPICK(CII_OBJECT__VARIABLE* p_variable,
					CII_OBJECT__ALARM* p_alarm,
					const CString& para_arm,
					const CString& para_module,
					const CString& para_slot);

	int  Seq__PICK(CII_OBJECT__VARIABLE* p_variable,
					CII_OBJECT__ALARM* p_alarm,
					const CString& para_arm,
					const CString& para_module,
					const CString& para_slot,
					const int ex_flag = -1);

	//
	CString sMODE__PLACE;
	int  Call__PLACE(CII_OBJECT__VARIABLE* p_variable,
					CII_OBJECT__ALARM* p_alarm,
					const CString& para_arm,
					const CString& para_module,
					const CString& para_slot);

	CString sMODE__XPLACE;
	int  Call__XPLACE(CII_OBJECT__VARIABLE* p_variable,
					CII_OBJECT__ALARM* p_alarm,
					const CString& para_arm,
					const CString& para_module,
					const CString& para_slot); 

	int  Seq__PLACE(CII_OBJECT__VARIABLE* p_variable,
					CII_OBJECT__ALARM* p_alarm,
					const CString& para_arm,
					const CString& para_module,
					const CString& para_slot,
					const int ex_flag = -1);

	//
	CString sMODE__ROTATE;
	int  Call__ROTATE(CII_OBJECT__VARIABLE* p_variable,
					CII_OBJECT__ALARM* p_alarm,
					const CString& para_arm,
					const CString& para_module,
					const CString& para_slot);

	// MAP -----
	CString sMODE__MAP;
	int  Call__MAP(CII_OBJECT__VARIABLE* p_variable,
				   CII_OBJECT__ALARM* p_alarm);

	// HOME -----
	CString sMODE__HOME;
	CString sMODE__VACOFF_HOME;
	int  Call__HOME(CII_OBJECT__VARIABLE* p_variable,
					CII_OBJECT__ALARM* p_alarm,
					const int req__vac_off);

	// VACON -----
	CString sMODE__VACON;
	int  Call__VACON(CII_OBJECT__VARIABLE* p_variable,
					CII_OBJECT__ALARM* p_alarm,
					const CString& arm_type);

	// VACOFF -----
	CString sMODE__VACOFF;
	int  Call__VACOFF(CII_OBJECT__VARIABLE* p_variable,
					CII_OBJECT__ALARM* p_alarm,
					const CString& arm_type);

	// RETRACT -----
	CString sMODE__RETRACT;
	int  Call__RETRACT(CII_OBJECT__VARIABLE* p_variable,
					CII_OBJECT__ALARM* p_alarm,
					const CString& para_arm,
					const CString& para_module,
					const CString& para_slot);

	// EXTEND -----
	CString sMODE__EXTEND;
	int  Call__EXTEND(CII_OBJECT__VARIABLE* p_variable,
					CII_OBJECT__ALARM* p_alarm,
					const CString& arm_type,
					const CString& stn_name,
					const CString& stn_slot);

	// GOUP -----
	CString sMODE__GOUP;
	int  Call__GOUP(CII_OBJECT__VARIABLE* p_variable,
					CII_OBJECT__ALARM* p_alarm,
					const CString& arm_type,
					const CString& stn_name,
					const CString& stn_slot);

	// GODOWN -----
	CString sMODE__GODOWN;
	int  Call__GODOWN(CII_OBJECT__VARIABLE* p_variable,
					CII_OBJECT__ALARM* p_alarm,
					const CString& arm_type,
					const CString& stn_name,
					const CString& stn_slot);

	// ALGN ----- 
	CString sMODE__ALGN;
	int  Call__ALGN(CII_OBJECT__VARIABLE* p_variable,
					CII_OBJECT__ALARM* p_alarm);

	// ALGNPICK -----
	CString sMODE__ALGNPICK;
	int  Call__ALGNPICK(CII_OBJECT__VARIABLE* p_variable,
						CII_OBJECT__ALARM* p_alarm,
						const CString& arm_type,
						const CString& stn_name,
						const CString& stn_slot);

	// ALGNPLACE -----
	CString sMODE__ALGNPLACE;
	int  Call__ALGNPLACE(CII_OBJECT__VARIABLE* p_variable,
						CII_OBJECT__ALARM* p_alarm,
						const CString& arm_type,
						const CString& stn_name,
						const CString& stn_slot);

	CString sMODE__PLACE_ALGNPICK;
	int  Call__PLACE_ALIGN_PICK(CII_OBJECT__VARIABLE* p_variable,
								CII_OBJECT__ALARM* p_alarm,
								const CString& arm_type,
								const CString& stn_name,
								const CString& stn_slot);

	//  LoadPort Mapping -----
	CString sMODE__LP_MAP;
	int   Call__LP_MAP(CII_OBJECT__VARIABLE* p_variable,
						CII_OBJECT__ALARM* p_alarm,
						const CString& arm_type,
						const CString& stn_name,
						const CString& stn_slot);


	//-------------------------------------------------------------------------
	int  Fnc__CALL_ROTATE(CII_OBJECT__VARIABLE* p_variable,
							CII_OBJECT__ALARM* p_alarm,
							const CString& para_arm,
							const CString& para_module,
							const CString& para_slot);
	int  Fnc__RUN_ROTATE(CII_OBJECT__VARIABLE* p_variable,
							CII_OBJECT__ALARM* p_alarm,
							const CString& para_arm,
							const CString& para_module,
							const CString& para_slot);

	int  Fnc__CALL_PICK(CII_OBJECT__VARIABLE* p_variable,
						CII_OBJECT__ALARM* p_alarm,
						const CString& para_arm,
						const CString& para_module,
						const CString& para_slot);
	int  Fnc__CALL_PLACE(CII_OBJECT__VARIABLE* p_variable,
						CII_OBJECT__ALARM* p_alarm,
						const CString& para_arm,
						const CString& para_module,
						const CString& para_slot);


public:
	CObj__ATM_ROBOT_STD();
	~CObj__ATM_ROBOT_STD();

	//-------------------------------------------------------------------------
	int __DEFINE__CONTROL_MODE(obj,l_mode);
	int __DEFINE__VERSION_HISTORY(version);

	int __DEFINE__VARIABLE_STD(p_variable);
	int __DEFINE__ALARM(p_alarm);

	int __Define__USER_FUNCTION(CII_DEFINE__FUNCTION *p_fnc_ctrl);

	//-------------------------------------------------------------------------
	int __INITIALIZE__OBJECT(p_variable,p_ext_obj_create);

	//-------------------------------------------------------------------------
	int __CALL__CONTROL_MODE(mode,p_debug,p_variable,p_alarm);
	int __CALL__MONITORING(id,p_variable,p_alarm);
};
