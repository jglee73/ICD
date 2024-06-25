#include "StdAfx.h"
#include "CObj__STD_TYPE.h"
#include "CObj__STD_TYPE__DEF.h"


void CObj__STD_TYPE
::Mon__IO_MONITOR(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	CString var__data;


	while(1)
	{
		p_variable->Wait__SINGLE_OBJECT(0.1);


		// PRESSURE ...
		{
			aiEXT_CH__VACLINE_PRESS->Get__DATA(var__data);
			aCH__PRESSURE_TORR->Set__DATA(var__data);
		}

		// PUMP IO COMMUNICATION STS ...
		{
			sEXT_CH__PUMP_COMM_STS->Get__DATA(var__data);
			sCH__MON_PUMP_COMM_STS->Set__DATA(var__data);
		}

		// PUMP IO RUN STS ...
		{
			sEXT_CH__PUMP_STS->Get__DATA(var__data);
			sCH__MON_PUMP_RUN_STS->Set__DATA(var__data);
		}
	}	
}
