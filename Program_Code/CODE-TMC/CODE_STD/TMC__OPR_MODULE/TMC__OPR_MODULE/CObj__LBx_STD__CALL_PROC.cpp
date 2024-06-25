#include "StdAfx.h"
#include "CObj__LBx_STD.h"


#define _CMMD__PROC_READY				"PROC_READY"
#define _CMMD__PROC_START				"PROC_START"

#define _CMMD__PRE_READY				"PRE_READY"
#define _CMMD__PRE_START				"PRE_START"

#define _CMMD__CLN_READY				"CLN_READY"
#define _CMMD__CLN_START				"CLN_START"

#define _CMMD__MNT_READY				"MNT_READY"
#define _CMMD__MNT_START				"MNT_START"


// ...
int  CObj__LBx_STD
::Call__PROC_READY(CII_OBJECT__VARIABLE* p_variable,
				   CII_OBJECT__ALARM* p_alarm)
{
	if(!bActive__LLx_PROC)			return -1;

	return _Fnc__PROC_MODE(_CMMD__PROC_READY);
}
int  CObj__LBx_STD
::Call__PROC_START(CII_OBJECT__VARIABLE* p_variable,
				   CII_OBJECT__ALARM* p_alarm)
{
	if(!bActive__LLx_PROC)			return -1;

	return _Fnc__PROC_MODE(_CMMD__PROC_START);
}

int  CObj__LBx_STD
::Call__PRE_READY(CII_OBJECT__VARIABLE* p_variable,
				  CII_OBJECT__ALARM* p_alarm)
{
	if(!bActive__LLx_PROC)			return -1;

	return _Fnc__PROC_MODE(_CMMD__PRE_READY);
}
int  CObj__LBx_STD
::Call__PRE_START(CII_OBJECT__VARIABLE* p_variable,
				  CII_OBJECT__ALARM* p_alarm)
{
	if(!bActive__LLx_PROC)			return -1;

	return _Fnc__PROC_MODE(_CMMD__PRE_START);
}

int  CObj__LBx_STD
::Call__CLN_READY(CII_OBJECT__VARIABLE* p_variable,
				  CII_OBJECT__ALARM* p_alarm)
{
	if(!bActive__LLx_PROC)			return -1;

	return _Fnc__PROC_MODE(_CMMD__CLN_READY);
}
int  CObj__LBx_STD
::Call__CLN_START(CII_OBJECT__VARIABLE* p_variable,
				  CII_OBJECT__ALARM* p_alarm)
{
	if(!bActive__LLx_PROC)			return -1;

	return _Fnc__PROC_MODE(_CMMD__CLN_START);
}

int  CObj__LBx_STD
::Call__MNT_READY(CII_OBJECT__VARIABLE* p_variable,
				  CII_OBJECT__ALARM* p_alarm)
{
	if(!bActive__LLx_PROC)			return -1;

	return _Fnc__PROC_MODE(_CMMD__MNT_READY);
}
int  CObj__LBx_STD
::Call__MNT_START(CII_OBJECT__VARIABLE* p_variable,
				  CII_OBJECT__ALARM* p_alarm)
{
	if(!bActive__LLx_PROC)			return -1;

	return _Fnc__PROC_MODE(_CMMD__MNT_START);
}

int  CObj__LBx_STD
::_Fnc__PROC_MODE(const CString& cmmd_proc)
{
	CString ch_name = sCH__PARA_PROC_RECIPE_NAME->Get__CHANNEL_NAME();
	CString ch_data = sCH__PARA_PROC_RECIPE_NAME->Get__STRING();

	// ...
	{
		CString log_msg;
	
		log_msg.Format(" * %s <- %s \n", ch_name,ch_data);
		xLOG_CTRL->WRITE__LOG(log_msg);
	}

	sEXT_CH__PARA_PROC_RCP_NAME->Set__DATA(ch_data);

	return pOBJ_CTRL__LLx_PROC->Call__OBJECT(cmmd_proc);
}
