#include "StdAfx.h"
#include "CObj__VIRTUAL_TYPE.h"
#include "CObj__VIRTUAL_TYPE__DEF.h"


//----------------------------------------------------------------------------------------------------
int  CObj__VIRTUAL_TYPE
::Call__INIT(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	if(Call__PUMP_ON(p_variable, p_alarm) < 0)		return -1;

	dCH__OBJ_STATUS->Set__DATA(STR__STANDBY);
	return 1;
}
int  CObj__VIRTUAL_TYPE
::Call__MAINT(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	dCH__OBJ_STATUS->Set__DATA(STR__MAINTMODE);
	return 1;
}

int  CObj__VIRTUAL_TYPE
::Call__PUMP_ON(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	sCH__MON_PUMP_RUN_STS->Set__DATA(STR__ON);
	return 1;
}
int  CObj__VIRTUAL_TYPE
::Call__PUMP_OFF(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	sCH__MON_PUMP_RUN_STS->Set__DATA(STR__OFF);
	return 1;
}
