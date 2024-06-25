#pragma once

#include "Interface_Code.h"

#include "CCommon_Error.h"
#include "CCommon_System.h"


class CObj__LPx_STD : public __IOBJ__STD_TYPE
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

	CII_EXT_OBJECT__CTRL *pLPx__OBJ_CTRL;


	//------------------------------------------------------------------------------
	CString sMODE__INIT;
	int  Call__INIT(CII_OBJECT__VARIABLE* p_variable);

	CString sMODE__MAINT;
	int  Call__MAINT(CII_OBJECT__VARIABLE* p_variable);

	//
	CString sMODE__HOME;
	int  Call__HOME(CII_OBJECT__VARIABLE* p_variable);

	CString sMODE__ALARM_RESET;
	int  Call__ALARM_RESET(CII_OBJECT__VARIABLE* p_variable);

	//
	CString sMODE__PREPLOAD;
	int  Call__PREPLOAD(const CString& mode,CII_OBJECT__VARIABLE* p_variable);

	CString sMODE__LOAD;
	int  Call__LOAD(const CString& mode,CII_OBJECT__VARIABLE* p_variable);

	//
	CString sMODE__RLSUNLOAD;
	int  Call__RLSUNLOAD(const CString& mode,CII_OBJECT__VARIABLE* p_variable);

	CString sMODE__UNLOAD;
	int  Call__UNLOAD(const CString& mode,CII_OBJECT__VARIABLE* p_variable);

	//
	CString sMODE__CYCLE;
	int  Call__CYCLE(const CString& mode,CII_OBJECT__VARIABLE* p_variable);

	// 
	CString sMODE__CLAMP;
	int  Call__CLAMP(const CString& mode,CII_OBJECT__VARIABLE* p_variable);

	CString sMODE__UNCLAMP;
	int  Call__UNCLAMP(const CString& mode,CII_OBJECT__VARIABLE* p_variable);

	//
	CString sMODE__MAP;
	int  Call__MAP(const CString& mode,CII_OBJECT__VARIABLE* p_variable);

	CString sMODE__EXTENDPADDLE;
	int  Call__EXTENDPADDLE(const CString& mode,CII_OBJECT__VARIABLE* p_variable);

	CString sMODE__RETRACTPADDLE;
	int  Call__RETRACTPADDLE(const CString& mode,CII_OBJECT__VARIABLE* p_variable);

	CString sMODE__PADDLE_DOWN;
	int  Call__PADDLE_DOWN(const CString& mode,CII_OBJECT__VARIABLE* p_variable);

	CString sMODE__PADDLE_UP;
	int  Call__PADDLE_UP(const CString& mode,CII_OBJECT__VARIABLE* p_variable);

	CString sMODE__LATCH;
	int  Call__LATCH(const CString& mode,CII_OBJECT__VARIABLE* p_variable);

	CString sMODE__UNLATCH;
	int  Call__UNLATCH(const CString& mode,CII_OBJECT__VARIABLE* p_variable);

	CString sMODE__CLOSEDOOR;
	int  Call__CLOSEDOOR(const CString& mode,CII_OBJECT__VARIABLE* p_variable);

	CString sMODE__OPENDOOR;
	int  Call__OPENDOOR(const CString& mode,CII_OBJECT__VARIABLE* p_variable);

	CString sMODE__VACON;
	int  Call__VACON(const CString& mode,CII_OBJECT__VARIABLE* p_variable);

	CString sMODE__VACOFF;
	int  Call__VACOFF(const CString& mode,CII_OBJECT__VARIABLE* p_variable);

	CString sMODE__EXTENDSHUTTLE;
	int  Call__EXTENDSHUTTLE(const CString& mode,CII_OBJECT__VARIABLE* p_variable);

	CString sMODE__RETRACTSHUTTLE;
	int  Call__RETRACTSHUTTLE(const CString& mode,CII_OBJECT__VARIABLE* p_variable);

	CString sMODE__HALT;
	int  Call__HALT(const CString& mode,CII_OBJECT__VARIABLE* p_variable);

	//
	CString sMODE__CID_READ;
	int  Call__CID_READ(CII_OBJECT__VARIABLE* p_variable);

	CString sMODE__PAGE_READ;
	int  Call__PAGE_READ(CII_OBJECT__VARIABLE* p_variable);

	CString sMODE__CID_WRITE;
	int  Call__CID_WRITE(CII_OBJECT__VARIABLE* p_variable);

	CString sMODE__PAGE_WRITE;
	int  Call__PAGE_WRITE(CII_OBJECT__VARIABLE* p_variable);
	//


public:
	CObj__LPx_STD();
	~CObj__LPx_STD();

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
