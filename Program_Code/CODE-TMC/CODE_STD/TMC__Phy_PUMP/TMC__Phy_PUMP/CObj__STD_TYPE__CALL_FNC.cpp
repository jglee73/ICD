#include "StdAfx.h"
#include "CObj__STD_TYPE.h"
#include "CObj__STD_TYPE__DEF.h"
#include "CObj__STD_TYPE__ALID.h"


//----------------------------------------------------------------------------------------------------
// INIT ----- 
int  CObj__STD_TYPE
::Call__INIT(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	if(pPUMP__OBJ_CTRL->Call__OBJECT("INIT") < 0)
	{
		return -1;
	}

	dCH__OBJ_STATUS->Set__DATA(STR__STANDBY);
	return 1;
}

// PUMP -----
int  CObj__STD_TYPE
::Call__PUMP_ON(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	if(sEXT_CH__PUMP_STS->Check__DATA(STR__ON) < 0)
	{
		if(pPUMP__OBJ_CTRL->Call__OBJECT("PUMP_ON") < 0)
		{
			return -1;
		}
	}
	return 1;
}
