#include "StdAfx.h"
#include "CObj__ATM_ROBOT_STD.h"
#include "CObj__ATM_ROBOT_STD__DEF.h"


//----------------------------------------------------------------------------------------------------
int  CObj__ATM_ROBOT_STD::
Fnc__CALL_ROTATE(CII_OBJECT__VARIABLE* p_variable,
				 CII_OBJECT__ALARM* p_alarm,
				 const CString& para_arm,
				 const CString& para_module,
				 const CString& para_slot)
{
	CStringArray l_para;

	l_para.Add(para_arm);
	l_para.Add(para_module);
	l_para.Add(para_slot);

	return pATM_RB__OBJ_CTRL->Call__OBJ_MODE(CMMD__ROTATE,l_para);
}
int  CObj__ATM_ROBOT_STD::
Fnc__RUN_ROTATE(CII_OBJECT__VARIABLE* p_variable,
				CII_OBJECT__ALARM* p_alarm,
				const CString& para_arm,
				const CString& para_module,
				const CString& para_slot)
{
	CStringArray l_para;

	l_para.Add(para_arm);
	l_para.Add(para_module);
	l_para.Add(para_slot);

	return pATM_RB__OBJ_CTRL->Run__OBJ_MODE(CMMD__ROTATE,l_para);
}

int  CObj__ATM_ROBOT_STD::
Fnc__CALL_PICK(CII_OBJECT__VARIABLE* p_variable,
			   CII_OBJECT__ALARM* p_alarm,
			   const CString& para_arm,
			   const CString& para_module,
			   const CString& para_slot)
{
	CStringArray l_para;

	l_para.Add(para_arm);
	l_para.Add(para_module);
	l_para.Add(para_slot);

	return pATM_RB__OBJ_CTRL->Call__OBJ_MODE(CMMD__PICK,l_para);
}
int  CObj__ATM_ROBOT_STD::
Fnc__CALL_PLACE(CII_OBJECT__VARIABLE* p_variable,
				CII_OBJECT__ALARM* p_alarm,
				const CString& para_arm,
				const CString& para_module,
				const CString& para_slot)
{
	CStringArray l_para;

	l_para.Add(para_arm);
	l_para.Add(para_module);
	l_para.Add(para_slot);

	return pATM_RB__OBJ_CTRL->Call__OBJ_MODE(CMMD__PLACE,l_para);
}
