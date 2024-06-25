#include "StdAfx.h"
#include "CObj__LBx_STD.h"


//----------------------------------------------------------------------------------------------------
void CObj__LBx_STD
::Mon__IO_MONITOR(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	CString var__data;

	while(1)
	{
		Sleep(9);

		// OBJ STATUS -----
		dCH__OBJ_STATUS->Get__DATA(var__data);
		sEXT_CH__OBJ_STATUS->Set__DATA(var__data);
	}	
}
