#pragma once

#include "Interface_Code.h"
#include "CCommon_System.h"


class CObj__DB_LLx : public __IOBJ__STD_TYPE
{
private:
	//-------------------------------------------------------------------------
	CString sObject_Name;


	//------------------------------------------------------------------------------
	//  INTERNAL PROPERTY

	// MNT_PROC ...
	CX__VAR_STRING_CTRL  sCH__SYSTEM_MSG;
	CX__VAR_STRING_CTRL  sCH__FNC_MSG;

	// SLOT INFO ...
	CX__VAR_STRING_CTRL  sCH__SLOT01_RCP_NAME;
	CX__VAR_DIGITAL_CTRL dCH__SLOT01_STATUS;
	CX__VAR_STRING_CTRL  sCH__SLOT01_TITLE;
	CX__VAR_STRING_CTRL  sCH__SLOT01_LOTID;
	CX__VAR_STRING_CTRL  sCH__SLOT01_MATERIALID;
	CX__VAR_STRING_CTRL  sCH__SLOT01_PORTID;
	CX__VAR_STRING_CTRL  sCH__SLOT01_SLOTID;

	// SYSTEM_PROCESS INFO ...
	CX__VAR_STRING_CTRL  sCH__SYSTEM_PROCESS_FLAG;
	CX__VAR_STRING_CTRL  sCH__SYSTEM_PROCESS_STEP_ID;
	CX__VAR_STRING_CTRL  sCH__SYSTEM_PROCESS_LAST_STEP_FLAG;

	// SYSTEM_MESSAGE INFO ...
	CX__VAR_STRING_CTRL  sCH__SYS_CTRL_NAME;
	CX__VAR_STRING_CTRL  sCH__SYS_CTRL_MSG;
	CX__VAR_STRING_CTRL  sCH__SYS_CTRL_LOCK;
	CX__VAR_STRING_CTRL  sCH__SYS_ALWAYS_LOCK_FLAG;

	CX__VAR_STRING_CTRL  sCH__SYS_FNC_NAME;
	CX__VAR_STRING_CTRL  sCH__SYS_FNC_MSG;
	CX__VAR_STRING_CTRL  sCH__SYS_FNC_LOCK;
	//

public:
	CObj__DB_LLx();
	~CObj__DB_LLx();

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
