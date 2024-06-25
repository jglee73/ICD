#include "StdAfx.h"
#include "CObj__TMC_CHM_MainFold_X.h"


//----------------------------------------------------------------------------------------------------
int  CObj__TMC_CHM_MainFold_X::
Call__INIT(CII_OBJECT__VARIABLE* p_variable)
{
	dCH__OBJ_STATUS->Set__DATA(STR__MAINTMODE);

	int flag = pTMC_CHM__OBJ_CTRL->Call__OBJECT("INIT");

	if(flag > 0)
	{
		dCH__OBJ_STATUS->Set__DATA(STR__STANDBY);
	}
	return flag;
}

int  CObj__TMC_CHM_MainFold_X::
Call__MAINT(CII_OBJECT__VARIABLE* p_variable)
{
	dCH__OBJ_STATUS->Set__DATA(STR__MAINTMODE);
	return 1;
}

//
int  CObj__TMC_CHM_MainFold_X::
Call__PUMP(CII_OBJECT__VARIABLE* p_variable)
{
	return pTMC_CHM__OBJ_CTRL->Call__OBJECT("PUMP");
}
int  CObj__TMC_CHM_MainFold_X::
Call__VENT(CII_OBJECT__VARIABLE* p_variable)
{
	return pTMC_CHM__OBJ_CTRL->Call__OBJECT("VENT");
}

//
int  CObj__TMC_CHM_MainFold_X::
Call__ISOLATE(CII_OBJECT__VARIABLE* p_variable)
{
	// sEXT_CH__PHY__OBJ_MSG->Set__DATA("Start ... : [ISOLATE]");

	int r_flag = Fnc__ISOLATE(p_variable);

	/*
	if(r_flag < 0)			sEXT_CH__PHY__TMC_OBJ_MSG->Set__DATA("Aborted ... : [ISOLATE]");
	else					sEXT_CH__PHY__TMC_OBJ_MSG->Set__DATA("Completed ... : [ISOLATE]");
	*/
	return r_flag;
}
int  CObj__TMC_CHM_MainFold_X::
Fnc__ISOLATE(CII_OBJECT__VARIABLE* p_variable)
{
	int r_flag;

	// TMC ...
	{
		pTMC_CHM__OBJ_CTRL->Call__OBJECT("ISOLATE");
		if(r_flag < 0)		return -11;

		r_flag = pTMC_VLV__OBJ_CTRL->Call__OBJECT("PMC_ALL_SV_CLOSE");
		if(r_flag < 0)		return -12;
	}

	// LLx ...
	for(int i=0; i<iDATA__LLx_SIZE; i++)
	{
		r_flag = pLLx__OBJ_CTRL[i]->Call__OBJECT("SV_TRANSFER.CLOSE");
		if(r_flag < 0)		return -(21+i);
	}

	return 1;
}

int  CObj__TMC_CHM_MainFold_X::
Call__LEAK_CHECK(CII_OBJECT__VARIABLE* p_variable)
{
	return pTMC_CHM__OBJ_CTRL->Call__OBJECT("LEAK_CHECK");
}

int  CObj__TMC_CHM_MainFold_X::
Call__CYCLE_PURGE(CII_OBJECT__VARIABLE* p_variable)
{
	return pTMC_CHM__OBJ_CTRL->Call__OBJECT("CYCLE_PURGE");
}
