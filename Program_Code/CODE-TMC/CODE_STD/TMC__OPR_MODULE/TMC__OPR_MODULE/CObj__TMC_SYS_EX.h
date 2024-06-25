#pragma once

#include "Interface_Code.h"

#include "CCommon_Error.h"
#include "CCommon_System.h"

#include "CObj__TMC_SYS_EX__DEF.h"


class CObj__TMC_SYS_EX : public __IOBJ__STD_TYPE
{
private:
	CCommon_Error__USER_FNC mERROR__USER_FNC;

	//------------------------------------------------------------------------------
	CString sObject_Name;

	SCX__USER_LOG_CTRL xLOG_CTRL;
	int m_nPM_SIZE;


	//------------------------------------------------------------------------------
	// INTERNAL PROPERTY

	CX__VAR_DIGITAL_CTRL dCH__OBJ_STATUS;
	CX__VAR_STRING_CTRL  sCH__INIT_SEQ_MSG_X[INIT_MSG_MAX];

	int iLPx_LIMIT;


	//------------------------------------------------------------------------------
	// EXTERNAL PROPERTY

	CX__VAR_STRING_CTRL sEXT_CH__GUI_POPUP_INIT_SEQ_MSG;

	// INIT OBJ ...
	int iSIZE__INIT_OBJ;
	CII_EXT_OBJECT__CTRL *pOBJ_CTRL__INIT_X[_CFG_SIZE__INTI_OBJ];
	CX__VAR_STRING_CTRL  sCH__INIT_MSG_X[_CFG_SIZE__INTI_OBJ];

	// PMx SV -----
	CII_EXT_OBJECT__CTRL *pPMx_SV__OBJ_CTRL;
	
	CX__VAR_ANALOG_CTRL  aCH__PARA_PMC_ID;

	// LPx -----
	CII_EXT_OBJECT__CTRL *pLPx__OBJ_CTRL[CFG_LP_LIMIT];

	CX__VAR_DIGITAL_CTRL dEXT_CH__LPx_OBJ_STATUS[CFG_LP_LIMIT];
	//


	//------------------------------------------------------------------------------
	CString sMODE__INIT;
	int  Call__INIT(CII_OBJECT__VARIABLE* p_variable);

	CString sMODE__SYSTEM_INIT;
	int  Call__SYSTEM_INIT(CII_OBJECT__VARIABLE* p_variable);

	CString sMODE__SYSTEM_MAINT;
	int  Call__SYSTEM_MAINT(CII_OBJECT__VARIABLE* p_variable);


public:
	CObj__TMC_SYS_EX();
	~CObj__TMC_SYS_EX();

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
