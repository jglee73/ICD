#pragma once

#include "Interface_Code.h"

#include "CCommon_Error.h"
#include "CCommon_System.h"


class CObj__ATM_ROBOT_USER : public __IOBJ__STD_TYPE
{
private:
	CCommon_Error__USER_FNC mERROR__USER_FNC;

	//------------------------------------------------------------------------------
	CString sObject_Name;

	SCX__USER_LOG_CTRL xLOG_CTRL;


	//------------------------------------------------------------------------------
	// INTERNAL PROPERTY

	// PARA ...
	CX__VAR_DIGITAL_CTRL dCH__PARA_ARM;

	CX__VAR_DIGITAL_CTRL dCH__PARA_MODULE;
	CX__VAR_DIGITAL_CTRL dCH__PARA_LPx_SLOT;
	CX__VAR_DIGITAL_CTRL dCH__PARA_STx_SLOT;
	CX__VAR_DIGITAL_CTRL dCH__PARA_LLx_SLOT;
	CX__VAR_DIGITAL_CTRL dCH__PARA_ALx_SLOT;

	CX__VAR_DIGITAL_CTRL dCH__PARA_ALIGN_TARGET_STN;
	CX__VAR_DIGITAL_CTRL dCH__PARA_ALIGN_TARGET_SLOT;
	//

	//------------------------------------------------------------------------------
	// EXTERNAL PROPERTY

	// ATM_ROBOT
	CII_EXT_OBJECT__CTRL *pATM_ROBOT__OBJ_CTRL;

	CX__VAR_DIGITAL_CTRL dEXT_CH__PARA_ARM;
	CX__VAR_DIGITAL_CTRL dEXT_CH__PARA_MODULE;
	CX__VAR_DIGITAL_CTRL dEXT_CH__PARA_SLOT;
	//

	//------------------------------------------------------------------------------
	CString sMODE__INIT;
	int  Call__INIT(CII_OBJECT__VARIABLE* p_variable);

	//
	CString sMODE__HOME;
	int  Call__HOME(CII_OBJECT__VARIABLE* p_variable);

	//
	CString sMODE__ROTATE;
	int  Call__ROTATE(CII_OBJECT__VARIABLE* p_variable);

	CString sMODE__PICK;
	int  Call__PICK(CII_OBJECT__VARIABLE* p_variable);

	CString sMODE__PLACE;
	int  Call__PLACE(CII_OBJECT__VARIABLE* p_variable);

	//
	CString sMODE__ALIGN;
	int  Call__ALIGN(CII_OBJECT__VARIABLE* p_variable);

	CString sMODE__ALIGN_PICK;
	int  Call__ALIGN_PICK(CII_OBJECT__VARIABLE* p_variable);

	//
	CString sMODE__LP_MAP;
	int  Call__LP_MAP(CII_OBJECT__VARIABLE* p_variable);
	//


public:
	CObj__ATM_ROBOT_USER();
	~CObj__ATM_ROBOT_USER();

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
