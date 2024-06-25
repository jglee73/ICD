#include "StdAfx.h"
#include "CObj__ALx_STD.h"


//----------------------------------------------------------------------------------------------------
int  CObj__ALx_STD::Call__INIT(CII_OBJECT__VARIABLE* p_variable)
{
	int flag = pAL__OBJ_CTRL->Call__OBJECT("INIT");

	if(flag > 0)
	{
		dCH__OBJ_STATUS->Set__DATA(STR__STANDBY);
	}
	return flag;
}

int  CObj__ALx_STD::Call__MAINT(CII_OBJECT__VARIABLE* p_variable)
{
	dCH__OBJ_STATUS->Set__DATA(STR__MAINTMODE);
	return 1;
}

int  CObj__ALx_STD::Call__ALIGN(CII_OBJECT__VARIABLE* p_variable)
{
	CString ch_data;

	dCH__PARA_SLOT->Get__DATA(ch_data);
	dEXT_CH__ALx_PARA_SLOT->Set__DATA(ch_data);

	return pAL__OBJ_CTRL->Call__OBJECT("ALIGN");
}
