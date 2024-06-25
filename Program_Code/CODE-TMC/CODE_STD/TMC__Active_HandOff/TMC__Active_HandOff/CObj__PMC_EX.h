#pragma once

#include "Interface_Code.h"
#include "CCommon_Error.h"

#include "CObj__PMC_EX__ALID.h"
#include "CObj__PMC_EX__DEF.h"


#define _CFG__PMx_SIZE			10


class CObj__PMC_EX : public __IOBJ__STD_TYPE
{
private:
	//-------------------------------------------------------------------
	CString sObject_Name;

	// ...
	int iPMx_SIZE;

	SCX__CTC__MODULE_OBJ xI_PMx_Obj[_CFG__PMx_SIZE];
	SCX__USER_LOG_CTRL   xLOG_CTRL;

	CCommon_Error__MODULE_OBJ   mERROR__MODULE_OBJ;
	CCommon_Error__DEF_VARIABLE mERROR__DEF_VAR;
	//


	//-------------------------------------------------------------------
	// INTERNAL VARIABLE

	// Parameter ...
	CX__VAR_ANALOG_CTRL  aCH__PARA_PMx_ID;

	// Remote Control ...
	CX__VAR_DIGITAL_CTRL xCH__PMx_OBJ_CTRL[_CFG__PMx_SIZE];
	CX__VAR_DIGITAL_CTRL xCH__PMx_OBJ_MODE[_CFG__PMx_SIZE];

	// Status ...
	CX__VAR_STRING_CTRL  xCH__PMx_OBJ_STATUS[_CFG__PMx_SIZE];
	//


	//-------------------------------------------------------------------
	// ...
	CString sMODE__INIT;
	int  Call__INIT(CII_OBJECT__VARIABLE* p_variable, const int pm_i);

	// ...
	CString sMODE__TRANSFER_READY;
	int  Call__TRANSFER_READY(CII_OBJECT__VARIABLE* p_variable, const int pm_i);

	CString sMODE__TRANSFER_END;
	int  Call__TRANSFER_END(CII_OBJECT__VARIABLE* p_variable, const int pm_i);

	// ...
	CString sMODE__S1;
	int  Call__S1(CII_OBJECT__VARIABLE* p_variable, const int pm_i);

	CString sMODE__S2;
	int  Call__S2(CII_OBJECT__VARIABLE* p_variable, const int pm_i);

	CString sMODE__S3;
	int  Call__S3(CII_OBJECT__VARIABLE* p_variable, const int pm_i);

	// ...
	CString sMODE__R1;
	int  Call__R1(CII_OBJECT__VARIABLE* p_variable, const int pm_i);

	CString sMODE__R2;
	int  Call__R2(CII_OBJECT__VARIABLE* p_variable, const int pm_i);

	CString sMODE__R3;
	int  Call__R3(CII_OBJECT__VARIABLE* p_variable, const int pm_i);
	//

	// ...
	void Mon__MODULE_STATUS(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);
	//


public:
	CObj__PMC_EX();
	~CObj__PMC_EX();

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
