#include "StdAfx.h"
#include "CObj__LPx_STD.h"


//----------------------------------------------------------------------------------------------------
int  CObj__LPx_STD::Call__INIT(CII_OBJECT__VARIABLE* p_variable)
{
	int flag = pLPx__OBJ_CTRL->Call__OBJECT("INIT");

	if(flag > 0)
	{
		dCH__OBJ_STATUS->Set__DATA(STR__STANDBY);
	}
	return flag;
}
int  CObj__LPx_STD::Call__MAINT(CII_OBJECT__VARIABLE* p_variable)
{
	dCH__OBJ_STATUS->Set__DATA(STR__MAINTMODE);
	return 1;
}

//
int  CObj__LPx_STD::Call__HOME(CII_OBJECT__VARIABLE* p_variable)
{
	return pLPx__OBJ_CTRL->Call__OBJECT("HOME");
}
int  CObj__LPx_STD::Call__ALARM_RESET(CII_OBJECT__VARIABLE* p_variable)
{
	return pLPx__OBJ_CTRL->Call__OBJECT("ALARM.RESET");
}

//
int  CObj__LPx_STD::Call__PREPLOAD(const CString& mode,CII_OBJECT__VARIABLE* p_variable)
{
	return pLPx__OBJ_CTRL->Call__OBJECT("PREPLOAD");
}
int  CObj__LPx_STD::Call__LOAD(const CString& mode,CII_OBJECT__VARIABLE* p_variable)
{
	return pLPx__OBJ_CTRL->Call__OBJECT("LOAD");
}

//
int  CObj__LPx_STD::Call__RLSUNLOAD(const CString& mode,CII_OBJECT__VARIABLE* p_variable)
{
	return pLPx__OBJ_CTRL->Call__OBJECT("RLSUNLOAD");
}
int  CObj__LPx_STD::Call__UNLOAD(const CString& mode,CII_OBJECT__VARIABLE* p_variable)
{
	int r_flag = pLPx__OBJ_CTRL->Call__OBJECT("RLSUNLOAD");
	if(r_flag < 0)		return -11;

	r_flag = pLPx__OBJ_CTRL->Call__OBJECT("UNLOAD");
	if(r_flag < 0)		return -21;

	return 1;
}

int  CObj__LPx_STD::Call__CYCLE(const CString& mode,CII_OBJECT__VARIABLE* p_variable)
{
	return pLPx__OBJ_CTRL->Call__OBJECT("CYCLE");
}

int  CObj__LPx_STD::Call__CLAMP(const CString& mode,CII_OBJECT__VARIABLE* p_variable)
{
	return pLPx__OBJ_CTRL->Call__OBJECT("CLAMP");
}

int  CObj__LPx_STD::Call__UNCLAMP(const CString& mode,CII_OBJECT__VARIABLE* p_variable)
{
	return pLPx__OBJ_CTRL->Call__OBJECT("UNCLAMP");
}

//
int  CObj__LPx_STD::Call__MAP(const CString& mode,CII_OBJECT__VARIABLE* p_variable)
{
	return pLPx__OBJ_CTRL->Call__OBJECT("MAP");
}

int  CObj__LPx_STD::Call__EXTENDPADDLE(const CString& mode,CII_OBJECT__VARIABLE* p_variable)
{
	return pLPx__OBJ_CTRL->Call__OBJECT("EXTENDPADDLE");
}

int  CObj__LPx_STD::Call__RETRACTPADDLE(const CString& mode,CII_OBJECT__VARIABLE* p_variable)
{
	return pLPx__OBJ_CTRL->Call__OBJECT("RETRACTPADDLE");
}

int  CObj__LPx_STD::Call__PADDLE_DOWN(const CString& mode,CII_OBJECT__VARIABLE* p_variable)
{
	return pLPx__OBJ_CTRL->Call__OBJECT("PADDLEDOWN");
}

int  CObj__LPx_STD::Call__PADDLE_UP(const CString& mode,CII_OBJECT__VARIABLE* p_variable)
{
	return pLPx__OBJ_CTRL->Call__OBJECT("PADDLEUP");
}

int  CObj__LPx_STD::Call__LATCH(const CString& mode,CII_OBJECT__VARIABLE* p_variable)
{
	return pLPx__OBJ_CTRL->Call__OBJECT("LATCH");
}

int  CObj__LPx_STD::Call__UNLATCH(const CString& mode,CII_OBJECT__VARIABLE* p_variable)
{
	return pLPx__OBJ_CTRL->Call__OBJECT("UNLATCH");
}

int  CObj__LPx_STD::Call__CLOSEDOOR(const CString& mode,CII_OBJECT__VARIABLE* p_variable)
{
	return pLPx__OBJ_CTRL->Call__OBJECT("CLOSEDOOR");
}

int  CObj__LPx_STD::Call__OPENDOOR(const CString& mode,CII_OBJECT__VARIABLE* p_variable)
{
	return pLPx__OBJ_CTRL->Call__OBJECT("OPENDOOR");
}

int  CObj__LPx_STD::Call__VACON(const CString& mode,CII_OBJECT__VARIABLE* p_variable)
{
	return pLPx__OBJ_CTRL->Call__OBJECT("VACON");
}

int  CObj__LPx_STD::Call__VACOFF(const CString& mode,CII_OBJECT__VARIABLE* p_variable)
{
	return pLPx__OBJ_CTRL->Call__OBJECT("VACOFF");
}

int  CObj__LPx_STD::Call__EXTENDSHUTTLE(const CString& mode,CII_OBJECT__VARIABLE* p_variable)
{
	return pLPx__OBJ_CTRL->Call__OBJECT("SHUTTLEOUT");
}

int  CObj__LPx_STD::Call__RETRACTSHUTTLE(const CString& mode,CII_OBJECT__VARIABLE* p_variable)
{
	return pLPx__OBJ_CTRL->Call__OBJECT("SHUTTLEIN");
}

int  CObj__LPx_STD::Call__HALT(const CString& mode,CII_OBJECT__VARIABLE* p_variable)
{
	return pLPx__OBJ_CTRL->Call__OBJECT("HALT");
}

//
int  CObj__LPx_STD::Call__CID_READ(CII_OBJECT__VARIABLE* p_variable)
{
	return pLPx__OBJ_CTRL->Call__OBJECT("CID_READ");
}

int  CObj__LPx_STD::Call__PAGE_READ(CII_OBJECT__VARIABLE* p_variable)
{
	return pLPx__OBJ_CTRL->Call__OBJECT("PAGE_READ");
}
int  CObj__LPx_STD::Call__CID_WRITE(CII_OBJECT__VARIABLE* p_variable)
{
	return pLPx__OBJ_CTRL->Call__OBJECT("CID_WRITE");
}
int  CObj__LPx_STD::Call__PAGE_WRITE(CII_OBJECT__VARIABLE* p_variable)
{
	return pLPx__OBJ_CTRL->Call__OBJECT("PAGE_WRITE");
}
