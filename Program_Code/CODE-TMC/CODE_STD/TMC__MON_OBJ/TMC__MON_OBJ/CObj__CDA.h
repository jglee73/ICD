#pragma once

#include "Interface_Code.h"


#define _CFG__CDA_SIZE				5
#define _INDEX__CDA_GN2				0
#define _INDEX__CDA_TM_LL			1
#define _INDEX__CDA_PCW				2
#define _INDEX__CDA1_PM				3
#define _INDEX__CDA2_PM				4


class CObj__CDA : public __IOBJ__STD_TYPE
{
private:
	//------------------------------------------------------------------------------
	CString sObject_Name;

	int iActive__SIM_MODE;
	
	//------------------------------------------------------------------------------
	// INTERNAL PROPERTY

	CX__VAR_ANALOG_CTRL  aCH__CFG_CDA_LOW_LIMIT[_CFG__CDA_SIZE];
	//

	//------------------------------------------------------------------------------
	// EXTERNAL PROPERTY

	CX__VAR_ANALOG_CTRL  aEXT_CH__CDA_PRESSURE[_CFG__CDA_SIZE];
	//

	//------------------------------------------------------------------------------
	void Mon__CDA_PRESSURE(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);
	//

public:
	CObj__CDA();
	~CObj__CDA();

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
