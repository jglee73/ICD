#pragma once

#include "Interface_Code.h"

#include "CCommon_Error.h"
#include "CCommon_System.h"


class CObj__TMC_CHM_MainFold_X : public __IOBJ__STD_TYPE
{
private:
	CCommon_Error__USER_FNC mERROR__USER_FNC;

	//-------------------------------------------------------------------------
	CString sObject_Name;

	SCX__USER_LOG_CTRL xLOG_CTRL;


	//-------------------------------------------------------------------------
	// INTERNAL CHANNEL

	CX__VAR_DIGITAL_CTRL dCH__OBJ_STATUS;
	CX__VAR_STRING_CTRL  sCH__OBJ_SEQ_MSG;


	//------------------------------------------------------------------------------
	// EXTERNAL CHANNEL

	// TMC_CHM -----
	CII_EXT_OBJECT__CTRL *pTMC_CHM__OBJ_CTRL;

	CX__VAR_STRING_CTRL  sEXT_CH__TMC_CHM__OBJ_STATUS;

	// TMC_VLV -----
	CII_EXT_OBJECT__CTRL *pTMC_VLV__OBJ_CTRL;

	// LLx -----	
	int iDATA__LLx_SIZE;
	CII_EXT_OBJECT__CTRL *pLLx__OBJ_CTRL[CFG_LLx_LIMIT];
	//


	//------------------------------------------------------------------------------
	CString sMODE__INIT;
	int  Call__INIT(CII_OBJECT__VARIABLE* p_variable);

	CString sMODE__MAINT;
	int  Call__MAINT(CII_OBJECT__VARIABLE* p_variable);

	//
	CString sMODE__PUMP;
	int  Call__PUMP(CII_OBJECT__VARIABLE* p_variable);

	CString sMODE__VENT;
	int  Call__VENT(CII_OBJECT__VARIABLE* p_variable);

	CString sMODE__ISOLATE;
	int  Call__ISOLATE(CII_OBJECT__VARIABLE* p_variable);
	int  Fnc__ISOLATE(CII_OBJECT__VARIABLE* p_variable);

	CString sMODE__LEAK_CHECK;
	int  Call__LEAK_CHECK(CII_OBJECT__VARIABLE* p_variable);

	CString sMODE__CYCLE_PURGE;
	int  Call__CYCLE_PURGE(CII_OBJECT__VARIABLE* p_variable);

	// ...
	void Mon__IO_MONITOR(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);
	//


public:
	CObj__TMC_CHM_MainFold_X();
	~CObj__TMC_CHM_MainFold_X();

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
