#pragma once

#include "Interface_Code.h"

#include "CCommon_Error.h"
#include "CCommon_System.h"


class CObj__VAC_ROBOT_STD : public __IOBJ__STD_TYPE
{
private:
	CCommon_Error__USER_FNC mERROR__USER_FNC;

	//-------------------------------------------------------------------------
	CString sObject_Name;

	SCX__USER_LOG_CTRL xLOG_CTRL;


	//-------------------------------------------------------------------------
	// INTERNAL PROPERTY

	CX__VAR_DIGITAL_CTRL dCH__OBJ_STATUS;
	CX__VAR_STRING_CTRL  sCH__OBJ_MSG;

	CX__VAR_DIGITAL_CTRL dCH__PARA_ARM;
	CX__VAR_DIGITAL_CTRL dCH__PARA_MODULE;
	CX__VAR_DIGITAL_CTRL dCH__PARA_SLOT;


	//-------------------------------------------------------------------------
	// EXTERNAL PROPERTY

	// VAC_ROBOT
	CII_EXT_OBJECT__CTRL *pVAC_ROBOT__OBJ_CTRL;

	CX__VAR_DIGITAL_CTRL dEXT_CH__PARA_ARM;
	CX__VAR_DIGITAL_CTRL dEXT_CH__PARA_MODULE;
	CX__VAR_DIGITAL_CTRL dEXT_CH__PARA_SLOT;

	// VAC_ROBOT IO(RS232)
	CX__VAR_DIGITAL_CTRL	dEXT_PHY__IO_CH__HYPER_TERMINAL_LOCK;


	//------------------------------------------------------------------------------
	CString sMODE__INIT;
	int  Call__INIT(CII_OBJECT__VARIABLE* p_variable);

	CString sMODE__MAINT;
	int  Call__MAINT(CII_OBJECT__VARIABLE* p_variable);

	CString sMODE__SET_PARA;
	int  Call__SET_PARA(CII_OBJECT__VARIABLE* p_variable);

	//
	CString sMODE__PICK;
	int  Call__PICK(CII_OBJECT__VARIABLE* p_variable);

	CString sMODE__XPICK;
	int  Call__XPICK(CII_OBJECT__VARIABLE* p_variable);

	//
	CString sMODE__PLACE;
	int  Call__PLACE(CII_OBJECT__VARIABLE* p_variable);

	CString sMODE__XPLACE;
	int  Call__XPLACE(CII_OBJECT__VARIABLE* p_variabl);

	//
	CString sMODE__ROTATE;
	int  Call__ROTATE(CII_OBJECT__VARIABLE* p_variable);


	// Home -----
	CString sMODE__HOME;
	int  Call__HOME(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	// Map -----
	CString sMODE__MAP;
	int  Call__MAP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);


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

	// -----
	CString sMODE__RQ_STN;
	int  Call__RQ_STN(CII_OBJECT__VARIABLE* p_variable);

	// -----
	CString sMODE__SET_STN;
	int  Call__SET_STN(CII_OBJECT__VARIABLE* p_variable);

	// -----
	CString sMODE__TEACHED_CPTR_SAVE;
	int  Call__TEACHED_CPTR_SAVE(CII_OBJECT__VARIABLE* p_variable);


	//.....
	void Mon__HYPER_TERMINAL_INTERLOCK(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);
	//


public:
	CObj__VAC_ROBOT_STD();
	~CObj__VAC_ROBOT_STD();

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
