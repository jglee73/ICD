#include "StdAfx.h"
#include "CObj__TMC_CHM_MainFold_X.h"


//----------------------------------------------------------------------------------------------------
void CObj__TMC_CHM_MainFold_X
::Mon__IO_MONITOR(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	CString var__data;

	while(1)
	{
		p_variable->Wait__SINGLE_OBJECT(0.1);

		// OBJ STATUS ...
		{
			dCH__OBJ_STATUS->Get__DATA(var__data);
			sEXT_CH__TMC_CHM__OBJ_STATUS->Set__DATA(var__data);
		}
	}	
}
