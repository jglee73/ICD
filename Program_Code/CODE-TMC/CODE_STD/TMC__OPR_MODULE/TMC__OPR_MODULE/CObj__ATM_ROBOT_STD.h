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

	CX__VAR_DIGITAL_CTRL xCH__OBJ_STATUS;
	CX__VAR_STRING_CTRL  sCH__OBJ_MSG;

	CX__VAR_DIGITAL_CTRL xCH__PARA_ARM;
	CX__VAR_DIGITAL_CTRL xCH__PARA_MODULE;
	CX__VAR_DIGITAL_CTRL xCH__PARA_SLOT;


	//------------------------------------------------------------------------------
	// EXTERNAL PROPERTY

	// ATM_ROBOT
	CII_EXT_OBJECT__CTRL *pATM_ROBOT__OBJ_CTRL;

	CX__VAR_STRING_CTRL  xEXT_CH__OBJ_STATUS;
	CX__VAR_DIGITAL_CTRL xEXT_CH__PARA_ARM;
	CX__VAR_DIGITAL_CTRL xEXT_CH__PARA_MODULE;
	CX__VAR_DIGITAL_CTRL xEXT_CH__PARA_SLOT;

	// ATM_ROBOT IO(RS232)
	CX__VAR_DIGITAL_CTRL	dEXT_PHY__IO_CH__HYPER_TERMINAL_LOCK;

	SCX__TIMER_CTRL xI_ATM_RB_TIMER;


	//------------------------------------------------------------------------------
	CString sMODE__INIT;
	int  Call__INIT(CII_OBJECT__VARIABLE* p_variable);

	CString sMODE__MAINT;
	int  Call__MAINT(CII_OBJECT__VARIABLE* p_variable);

	//
	CString sMODE__PICK;
	int  Call__PICK(CII_OBJECT__VARIABLE* p_variable);

	CString sMODE__XPICK;
	int  Call__XPICK(CII_OBJECT__VARIABLE* p_variable);

	CString sMODE__PLACE;
	int  Call__PLACE(CII_OBJECT__VARIABLE* p_variable);

	CString sMODE__XPLACE;
	int  Call__XPLACE(CII_OBJECT__VARIABLE* p_variable); 

	CString sMODE__ROTATE;
	int  Call__ROTATE(CII_OBJECT__VARIABLE* p_variable);


	// MAP -----
	CString sMODE__MAP;
	int  Call__MAP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	// HOME -----
	CString sMODE__HOME;
	CString sMODE__VACOFF_HOME;
	int  Call__HOME(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const int req__vac_off);

	// VACON -----
	CString sMODE__VACON;
	int  Call__VACON(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm,const CString& arm_type);

	// VACOFF -----
	CString sMODE__VACOFF;
	int  Call__VACOFF(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm,const CString& arm_type);

	// RETRACT -----
	CString sMODE__RETRACT;
	int  Call__RETRACT(CII_OBJECT__VARIABLE* p_variable);

	// EXTEND -----
	CString sMODE__EXTEND;
	int  Call__EXTEND(CII_OBJECT__VARIABLE* p_variable);

	// GOUP -----
	CString sMODE__GOUP;
	int  Call__GOUP(CII_OBJECT__VARIABLE* p_variable);

	// GODOWN -----
	CString sMODE__GODOWN;
	int  Call__GODOWN(CII_OBJECT__VARIABLE* p_variable);

	// ALGN ----- 
	CString sMODE__ALGN;
	int  Call__ALGN(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	// ALGNPICK -----
	CString sMODE__ALGNPICK;
	int  Call__ALGNPICK(CII_OBJECT__VARIABLE* p_variable,
						CII_OBJECT__ALARM* p_alarm,
						const CString& arm_type,
						const CString& stn_name,
						const CString& stn_slot);

	CString sMODE__PLACE_ALGN_PICK;
	int  Call__PLACE_ALIGN_PICK(CII_OBJECT__VARIABLE* p_variable,
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

	//  LoadPort Mapping -----
	CString sMODE__LP_MAP;
	int   Call__LP_MAP(CII_OBJECT__VARIABLE* p_variable,
						CII_OBJECT__ALARM* p_alarm,
						const CString& arm_type,
						const CString& stn_name,
						const CString& stn_slot);

	// ...
	void Mon__HYPER_TERMINAL_INTERLOCK(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);
	//


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
