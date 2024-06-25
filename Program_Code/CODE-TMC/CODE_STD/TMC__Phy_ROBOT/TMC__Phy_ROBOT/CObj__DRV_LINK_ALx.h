#pragma once

#include "Interface_Code.h"

#include "CCommon_Error.h"
#include "CObj__DRV_LINK_ALx__DEF.h"


class CObj__DRV_LINK_ALx : public __IOBJ__STD_TYPE
{
private:
	//-------------------------------------------------------------------------
	CString	sObject_Name;

	CCommon_Error__USER_FNC mERROR__USER_FNC;

	// ..
	int iActive__SIM_MODE;
	int iCFG__SIM_MODE;

	SCX__USER_LOG_CTRL xAPP_LOG_CTRL;
	//


	//-------------------------------------------------------------------------
	//  INTERNAL PROPERTY

	// PARA ... 
	CX__VAR_DIGITAL_CTRL dCH__PARA_SLOT;
	CX__VAR_ANALOG_CTRL  aCH__PARA_CCD_POS;

	// CFG ...
	CX__VAR_DIGITAL_CTRL dCH__CFG_ACTION_CONFIRM_FLAG;

	CX__VAR_DIGITAL_CTRL dCH__SIM_CFG__REAL_TEST;
	CX__VAR_ANALOG_CTRL  aCH__SIM_CFG__ALIGN_TIME;
	//


	//-------------------------------------------------------------------------
	//  EXTERNAL PROPERTY

	// OBJ : AL1_DRV ...
	CII_EXT_OBJECT__CTRL* pOBJ_CTRL__ALx_DRV;	

	CX__VAR_DIGITAL_CTRL dEXT_CH__ALx_PARA_CMMD;
	CX__VAR_STRING_CTRL  sEXT_CH__ALx_CMMD_STS;
	//


	//-------------------------------------------------------------------------
	// Init -----
	CString sMODE__INIT;
	int  Call__INIT(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	CString sMODE__HOME;
	int  Call__HOME(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	CString sMODE__ALIGN;
	int  Call__ALIGN(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	CString sMODE__ROTATE;
	int  Call__ROTATE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	CString sMODE__SCAN;
	int  Call__SCAN(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	int  Fnc__CMMD_WAIT(const CString& set_cmmd, const CString& oara_slot,const CString& para_angle = "");

	// ...
	void Mon__IO_MONITOR(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	// ...
	void Fnc__APP_LOG(const CString& log_msg);
	//

public:
	CObj__DRV_LINK_ALx();
	~CObj__DRV_LINK_ALx();

	//-------------------------------------------------------------------------
	int __DEFINE__CONTROL_MODE(obj,l_mode);
	int __DEFINE__VERSION_HISTORY(version);

	int __DEFINE__VARIABLE_STD(p_variable);

	int __DEFINE__ALARM(p_alarm);

	//-------------------------------------------------------------------------
	int __INITIALIZE__OBJECT(p_variable,p_ext_obj_create);	

	//-------------------------------------------------------------------------
	int __CALL__CONTROL_MODE(mode,p_debug,p_variable,p_alarm);
	int __CALL__MONITORING(id,p_variable,p_alarm);
};
