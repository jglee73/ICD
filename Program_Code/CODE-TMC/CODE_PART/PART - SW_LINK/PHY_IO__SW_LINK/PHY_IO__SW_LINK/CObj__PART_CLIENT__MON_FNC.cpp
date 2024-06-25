#include "stdafx.h"

#include "CObj__PART_CLIENT__ALID.h"
#include "CObj__PART_CLIENT__DEF.h"
#include "CObj__PART_CLIENT.h"


// ...
void CObj__PART_CLIENT
::Mon__IO_MONITORING(CII_OBJECT__VARIABLE *p_variable, CII_OBJECT__ALARM *p_alarm)
{
	if(iActive__SIM_MODE > 0)
	{
		bActive__COMM_ONLINE = true;
	}
	else
	{
		bActive__COMM_ONLINE = false;
	}


	while(1)
	{
		p_variable->Wait__SINGLE_OBJECT(0.1);


		if(bActive__COMM_ONLINE)
		{
			sCH__MON_COMM_STATE->Set__DATA(STR__ONLINE);
		}
		else
		{
			sCH__MON_COMM_STATE->Set__DATA(STR__OFFLINE);

			// ...
			{
				int alarm_id = ALID__OFFLINE;
				CString r_act;

				p_alarm->Check__ALARM(alarm_id,r_act);
				p_alarm->Post__ALARM(alarm_id);
			}
		}

		// ...
	}
}

