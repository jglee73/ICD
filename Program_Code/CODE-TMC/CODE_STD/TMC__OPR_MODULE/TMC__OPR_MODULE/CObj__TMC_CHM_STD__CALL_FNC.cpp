#include "StdAfx.h"
#include "CObj__TMC_CHM_STD.h"


//----------------------------------------------------------------------------------------------------
int  CObj__TMC_CHM_STD::
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

int  CObj__TMC_CHM_STD::
Call__MAINT(CII_OBJECT__VARIABLE* p_variable)
{
	dCH__OBJ_STATUS->Set__DATA(STR__MAINTMODE);
	return 1;
}

//
int  CObj__TMC_CHM_STD::
Call__PUMP(CII_OBJECT__VARIABLE* p_variable)
{
	return pTMC_CHM__OBJ_CTRL->Call__OBJECT("PUMP");
}
int  CObj__TMC_CHM_STD::
Call__VENT(CII_OBJECT__VARIABLE* p_variable)
{
	return pTMC_CHM__OBJ_CTRL->Call__OBJECT("VENT");
}

//
int  CObj__TMC_CHM_STD::
Call__ISOLATE(CII_OBJECT__VARIABLE* p_variable)
{
	int flag,flag1,flag2,flag3;

	sEXT_CH__PHY__TMC_OBJ_MSG->Set__DATA("Start ... : [ISOLATE]");

	flag = pPress_VLV__OBJ_CTRL->Call__OBJECT("TMC_CHM_VLV_ALL_CLOSE");
	if(flag < 0)	return -1;

	flag1 = pTMC_VLV__OBJ_CTRL->Call__OBJECT("PMC_ALL_SV_CLOSE");
	flag2 = pTMC_VLV__OBJ_CTRL->Call__OBJECT("LBA_SV_CLOSE");
	flag3 = pTMC_VLV__OBJ_CTRL->Call__OBJECT("LBB_SV_CLOSE");

	if( (flag1 < 0) || (flag2 < 0) || (flag3 < 0))
	{
		sEXT_CH__PHY__TMC_OBJ_MSG->Set__DATA("Aborted ... : [ISOLATE]");
		return -1;
	}

	sEXT_CH__PHY__TMC_OBJ_MSG->Set__DATA("Completed ... : [ISOLATE]");
	return 1;
}

int  CObj__TMC_CHM_STD::
Call__LEAK_CHECK(CII_OBJECT__VARIABLE* p_variable)
{
	return pTMC_CHM__OBJ_CTRL->Call__OBJECT("LEAK_CHECK");
}

int  CObj__TMC_CHM_STD::
Call__CYCLE_PURGE(CII_OBJECT__VARIABLE* p_variable)
{
	return pTMC_CHM__OBJ_CTRL->Call__OBJECT("CYCLE_PURGE");
}


// ...
int  CObj__TMC_CHM_STD::
Call__PMx_SV_OPEN(CII_OBJECT__VARIABLE* p_variable)
{
	CString ch_data = dCH__PARA_PMx_ID->Get__STRING();
	dEXT_CH__TMC_VLV__PARA_PMx_ID->Set__DATA(ch_data);

	return pTMC_VLV__OBJ_CTRL->Call__OBJECT("PMC_SV_OPEN");
}

int  CObj__TMC_CHM_STD::
Call__PMx_SV_CLOSE(CII_OBJECT__VARIABLE* p_variable)
{
	CString ch_data = dCH__PARA_PMx_ID->Get__STRING();
	dEXT_CH__TMC_VLV__PARA_PMx_ID->Set__DATA(ch_data);

	return pTMC_VLV__OBJ_CTRL->Call__OBJECT("PMC_SV_CLOSE");
}
