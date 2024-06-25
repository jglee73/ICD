#include "StdAfx.h"

#include "CObj__CDA.h"
#include "CObj__CDA__ALID.h"


//----------------------------------------------------------------------------------------------------
void CObj__CDA
::Mon__CDA_PRESSURE(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm)
{
	CString ch_data;

	int cda__alarm_count[_CFG__CDA_SIZE];
	int i;

	for(i=0; i<_CFG__CDA_SIZE; i++)
	{
		cda__alarm_count[i] = 0;
	}

	if(iActive__SIM_MODE > 0)
	{
		for(i=0; i<_CFG__CDA_SIZE; i++)
		{
			aCH__CFG_CDA_LOW_LIMIT[i]->Get__DATA(ch_data);
			aEXT_CH__CDA_PRESSURE[i]->Set__DATA(ch_data);
		}
	}

	while(1)
	{
		p_variable->Wait__SINGLE_OBJECT(0.1);

		// Check : Low Pressure ...
		for(i=0; i<_CFG__CDA_SIZE; i++)
		{
			double cfg_pressure = aCH__CFG_CDA_LOW_LIMIT[i]->Get__VALUE();
			double cur_pressure = aEXT_CH__CDA_PRESSURE[i]->Get__VALUE();

			if(cur_pressure >= cfg_pressure)
			{
				cda__alarm_count[i] = 0;
				continue;
			}

			cda__alarm_count[i]++;
			if(cda__alarm_count[i] < 5)
			{
				continue;
			}
			cda__alarm_count[i] = 0;

			// ...
			{
				int alarm_id = ALID__CDA_GN2__LOW_PRESSURE + i;
				CString alm_msg;
				CString alm_bff;
				CString r_act;

				alm_msg.Format("The current pressure is %.3f. \n", cur_pressure);
				alm_bff.Format("The config pressure is %.3f. \n",  cfg_pressure);
				alm_msg += alm_bff;

				p_alarm->Check__ALARM(alarm_id, r_act);
				p_alarm->Post__ALARM_With_MESSAGE(alarm_id, alm_msg);
			}
		}
	}	
}

