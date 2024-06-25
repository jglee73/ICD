#pragma once

#include "Interface_Code.h"

#include "CCommon_Error.h"
#include "CCommon_System.h"


class CObj__ALx_STD : public __IOBJ__STD_TYPE
{
private:
	CCommon_Error__USER_FNC mERROR__USER_FNC;

	//-------------------------------------------------------------------------
	CString sObject_Name;

	SCX__USER_LOG_CTRL xLOG_CTRL;


	//-------------------------------------------------------------------------
	// INTERNAL PROPERTY

	CX__VAR_DIGITAL_CTRL dCH__OBJ_STATUS;

	CX__VAR_DIGITAL_CTRL dCH__PARA_SLOT;
	//
	
	//------------------------------------------------------------------------------
	// EXTERNAL PROPERTY

	CII_EXT_OBJECT__CTRL *pAL__OBJ_CTRL;

	CX__VAR_DIGITAL_CTRL dEXT_CH__ALx_PARA_SLOT;
	//

	//------------------------------------------------------------------------------
	CString sMODE__INIT;
	int  Call__INIT(CII_OBJECT__VARIABLE* p_variable);

	CString sMODE__MAINT;
	int  Call__MAINT(CII_OBJECT__VARIABLE* p_variable);

	CString sMODE__ALIGN;
	int  Call__ALIGN(CII_OBJECT__VARIABLE* p_variable);


public:
	CObj__ALx_STD();
	~CObj__ALx_STD();

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
