#include "StdAfx.h"
#include "CObj_Phy__PMC_STD.h"
#include "CObj_Phy__PMC_STD__ALID.h"


// ...
void CObj_Phy__PMC_STD
::Fnc__Process_Time(double& total_time)
{
	CString	data;
	double	cur_time, percent, time, change_time;
	double	wait_time;

	// ...
	{
		wait_time = xSCH_MATERIAL_CTRL->Get__PMx_FRONT_TIME();

		xCH__TOTAL_PRC_TIME->Get__DATA(data);
		time = atof(data);

		if(time > 0)
		{
			xCH__PROCESS_FLAG->Set__DATA("YES");
			total_time = time;

			xCH__CHANGE_PRC_TIME->Set__DATA("0");
			xCH__TOTAL_PRC_TIME->Set__DATA("0");

			data.Format("%.1f",total_time);

			xCH__CURRENT_PRC_TIME->Set__DATA(data);
			xCH__PROCESS_STATUS->Set__DATA("");
		}

		xCH__CURRENT_PRC_TIME->Get__DATA(data);
		cur_time = atof(data);

		if(cur_time > 0)
		{
			if(iPRC_STS < 0)
			{
				xCH__CHANGE_PRC_TIME->Set__DATA("0");
				xCH__CURRENT_PRC_TIME->Set__DATA("0.0");
				xCH__PROCESS_STATUS->Set__DATA("");

				cur_time = 0;
			}
		}

		if((total_time >= 1)&&(cur_time > 0))
		{
			xCH__CHANGE_PRC_TIME->Get__DATA(data);
			change_time = atoi(data);

			if(change_time != 0)
			{
				cur_time += change_time;
				if(cur_time >= total_time)
				{
					total_time = cur_time;
				}

				//
				data.Format("%1d sec",cur_time);

				xCH__CURRENT_PRC_TIME->Set__DATA(data);	
				xCH__CHANGE_PRC_TIME->Set__DATA("0");
			}
			
			percent = (cur_time/total_time)*100;

			cur_time--;
			data.Format("%.1f",cur_time);
			xCH__CURRENT_PRC_TIME->Set__DATA(data);

			if(cur_time <= wait_time)		xCH__PROCESS_STATUS->Set__DATA("WAIT");
			else							xCH__PROCESS_STATUS->Set__DATA("");
		}
		else
		{
			percent = 0;
			xCH__PROCESS_STATUS->Set__DATA("");
		}

		data.Format("%.1f",percent);	
		xCH__PROCESS_PERCENT->Set__DATA(data);
	}
}

