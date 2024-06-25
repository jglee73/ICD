#pragma once

#include "Interface_Code.h"
#include "CCommon_System.h"


class CObj__SIDE_STORAGE : public __IOBJ__STD_TYPE
{
private:
	//-------------------------------------------------------------------------
	CString	sObject_Name;


	//-------------------------------------------------------------------------
	//  INTERNAL PROPERTY

	CX__VAR_DIGITAL_CTRL dCH__CST_EXIST_FLAG;
	CX__VAR_DIGITAL_CTRL dCH__OBJ_CSTATUS;

	CX__VAR_ANALOG_CTRL  aCH__CFG_SLOT_MAX;
	CX__VAR_DIGITAL_CTRL dCH__SLOT_STATUS[CFG_LPx__SLOT_SIZE];
	CX__VAR_STRING_CTRL  sCH__SLOT_TITLE[CFG_LPx__SLOT_SIZE];


	//-------------------------------------------------------------------------
	//  EXTERNAL PROPERTY


	//-------------------------------------------------------------------------
	CString sMODE__INIT;
	int  Call__INIT(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	CString sMODE__ALL_WAFER_CLEAR;
	int  Call__ALL_WAFER_CLEAR(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);
	//


public:
	CObj__SIDE_STORAGE();
	~CObj__SIDE_STORAGE();

	//-------------------------------------------------------------------------
	int __DEFINE__CONTROL_MODE(obj, l_mode);
	int __DEFINE__VERSION_HISTORY(version);

	int __DEFINE__VARIABLE_STD(p_variable);
	int __DEFINE__ALARM(p_alarm);

	//-------------------------------------------------------------------------
	int __INITIALIZE__OBJECT(p_variable,p_ext_obj_create);	

	//-------------------------------------------------------------------------
	int __CALL__CONTROL_MODE(mode, p_debug, p_variable, p_alarm);
	int __CALL__MONITORING(id, p_variable, p_alarm);
};
