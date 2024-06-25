#pragma once

#include "Interface_Code.h"

#include "CCommon_Error.h"
#include "CCommon_System.h"


class CObj__TMC_SYS_SETUP : public __IOBJ__STD_TYPE
{
private:
	CCommon_Error__USER_FNC mERROR__USER_FNC;

	//------------------------------------------------------------------------------
	CString sObject_Name;

	SCX__USER_LOG_CTRL xLOG_CTRL;


	//------------------------------------------------------------------------------
	// INTERNAL PROPERTY

	CX__VAR_DIGITAL_CTRL dCH__OBJ_STATUS;


	//------------------------------------------------------------------------------
	// EXTERNAL PROPERTY

	CX__VAR_DIGITAL_CTRL dEXT_CH__VAC_RB__RNE;
	CX__VAR_DIGITAL_CTRL dEXT_CH__ATM_RB__RNE;

	CX__VAR_DIGITAL_CTRL dEXT_CH__CHM1_VAC_SNS;
	CX__VAR_DIGITAL_CTRL dEXT_CH__CHM1_ATM_SNS;

	CX__VAR_DIGITAL_CTRL dEXT_CH__LBA_VAC_SNS;
	CX__VAR_DIGITAL_CTRL dEXT_CH__LBA_ATM_SNS;

	CX__VAR_DIGITAL_CTRL dEXT_CH__LBB_VAC_SNS;
	CX__VAR_DIGITAL_CTRL dEXT_CH__LBB_ATM_SNS;

	CX__VAR_DIGITAL_CTRL dEXT_CH__PM1_VAC_SNS;
	CX__VAR_DIGITAL_CTRL dEXT_CH__PM2_VAC_SNS;
	CX__VAR_DIGITAL_CTRL dEXT_CH__PM3_VAC_SNS;
	CX__VAR_DIGITAL_CTRL dEXT_CH__PM4_VAC_SNS;

	CX__VAR_DIGITAL_CTRL dEXT_CH__PM1_ATM_SNS;
	CX__VAR_DIGITAL_CTRL dEXT_CH__PM2_ATM_SNS;
	CX__VAR_DIGITAL_CTRL dEXT_CH__PM3_ATM_SNS;
	CX__VAR_DIGITAL_CTRL dEXT_CH__PM4_ATM_SNS;

	CX__VAR_ANALOG_CTRL  aEXT_CH__CHM1_PRESSURE;
	CX__VAR_ANALOG_CTRL  aEXT_CH__LBA_PRESSURE;
	CX__VAR_ANALOG_CTRL  aEXT_CH__LBB_PRESSURE;


	//------------------------------------------------------------------------------
	CString sMODE__SETUP_DISABLE;
	int  Call__SETUP_DISABLE(CII_OBJECT__VARIABLE* p_variable);

	CString sMODE__SETUP_ENABLE;
	int  Call__SETUP_ENABLE(CII_OBJECT__VARIABLE* p_variable);


public:
	CObj__TMC_SYS_SETUP();
	~CObj__TMC_SYS_SETUP();

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
