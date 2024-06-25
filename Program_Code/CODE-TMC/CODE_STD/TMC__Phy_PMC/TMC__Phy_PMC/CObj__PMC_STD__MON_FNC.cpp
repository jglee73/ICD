#include "StdAfx.h"
#include "CObj__PMC_STD.h"
#include "CObj__PMC_STD__DEF.h"


//----------------------------------------------------------------------------------------------------
void CObj__PMC_STD
::Mon__IO_MONITOR(CII_OBJECT__VARIABLE* p_variable,
				  CII_OBJECT__ALARM* p_alarm)
{
	SCX__TIMER_CTRL cx_timer_ctrl;

	CString str__sv_pm_open;
	CString str__sv_pm_close;
	CString str__sv_tm_open;
	CString str__sv_tm_close;
	CString str__sv_up;
	CString str__sv_dn;

	int i;
	CString str_atm_sns, str_vac_sns;

	if(iSim_Flag > 0)
	{
		for(i=0;i<m_nPM_LIMIT;i++)
		{
			if(dEXT_CH__CFG_PMx_EXIST_FLAG[i]->Check__DATA("YES") < 0)
			{
				dCH__SLIT_VALVE_STATUS[i]->Set__DATA(STR__CLOSE);
			}
			else
			{
				// PM side SV.
				diEXT_CH__PMx__SV_PM_OPEN[i]->Set__DATA(STR__OFF);
				diEXT_CH__PMx__SV_PM_CLOSE[i]->Set__DATA(STR__ON);

				// TM side SV.
				diEXT_CH__PMx__SV_TM_OPEN[i]->Set__DATA(STR__ON);
				diEXT_CH__PMx__SV_TM_CLOSE[i]->Set__DATA(STR__OFF);

				// SV Up/Down.
				diEXT_CH__PMx__SV_UP[i]->Set__DATA(STR__ON);
				diEXT_CH__PMx__SV_DOWN[i]->Set__DATA(STR__OFF);
			}
		}
	}

	while(1)
	{
		Sleep(10);

		for(i=0;i<m_nPM_LIMIT;i++)
		{
			Sleep(10);

			if(dEXT_CH__CFG_PMx_EXIST_FLAG[i]->Check__DATA("YES") < 0)
			{
				dCH__SLIT_VALVE_STATUS[i]->Set__DATA(STR__CLOSE);		// SV가 NONE이면 CLOSE로 간주
			}
			else
			{
				// PM side SV.
				diEXT_CH__PMx__SV_PM_OPEN[i]->Get__DATA(str__sv_pm_open);
				diEXT_CH__PMx__SV_PM_CLOSE[i]->Get__DATA(str__sv_pm_close);

				// TM side SV.
				diEXT_CH__PMx__SV_TM_OPEN[i]->Get__DATA(str__sv_tm_open);
				diEXT_CH__PMx__SV_TM_CLOSE[i]->Get__DATA(str__sv_tm_close);

				// SV Up/Down.
				diEXT_CH__PMx__SV_UP[i]->Get__DATA(str__sv_up);
				diEXT_CH__PMx__SV_DOWN[i]->Get__DATA(str__sv_dn);

				// ...
				int close_flag = -1;
				int open_flag  = -1;

				if((str__sv_dn.CompareNoCase(STR__OFF) == 0)
					&& (str__sv_up.CompareNoCase(STR__ON)  == 0))
				{
					if((str__sv_pm_open.CompareNoCase(STR__OFF) == 0)
						&& (str__sv_pm_close.CompareNoCase(STR__ON) == 0))
					{
						close_flag = 1;
					}
					else if((str__sv_tm_open.CompareNoCase(STR__OFF) == 0)
						&& (str__sv_tm_close.CompareNoCase(STR__ON) == 0))
					{
						close_flag = 1;
					}
				}
				else if((str__sv_dn.CompareNoCase(STR__ON) == 0)
					&& (str__sv_up.CompareNoCase(STR__OFF)  == 0))
				{
					if((str__sv_pm_open.CompareNoCase(STR__ON) == 0)
						&& (str__sv_pm_close.CompareNoCase(STR__OFF) == 0))
					{
						open_flag = 1;
					}
					else if((str__sv_tm_open.CompareNoCase(STR__ON) == 0)
						&& (str__sv_tm_close.CompareNoCase(STR__OFF) == 0))
					{
						open_flag = 1;
					}
				}

				if(close_flag > 0)
				{
					dCH__SLIT_VALVE_STATUS[i]->Set__DATA(STR__CLOSE);
				}
				else if(open_flag > 0)
				{
					dCH__SLIT_VALVE_STATUS[i]->Set__DATA(STR__OPEN);
				}
				else
				{
					dCH__SLIT_VALVE_STATUS[i]->Set__DATA(STR__UNKNOWN);
				}
			}

			// PMx Pressure Sts...
			diEXT_CH__PMx__ATM_SNS[i]->Get__DATA(str_atm_sns);
			diEXT_CH__PMx__VAC_SNS[i]->Get__DATA(str_vac_sns);

			if(iSim_Flag > 0)
			{
				if(dEXT_CH__CFG_TRANSFER_MODE->Check__DATA(STR__ATM) > 0)
				{
					diEXT_CH__PMx__ATM_SNS[i]->Set__DATA(STR__ON);
					diEXT_CH__PMx__VAC_SNS[i]->Set__DATA(STR__OFF);
				}
				else
				{
					diEXT_CH__PMx__ATM_SNS[i]->Set__DATA(STR__OFF);
					diEXT_CH__PMx__VAC_SNS[i]->Set__DATA(STR__ON);
				}

			}

			if( (str_atm_sns.CompareNoCase("ON")  == 0) && 
				(str_vac_sns.CompareNoCase("OFF") == 0) )
			{
				dCH__PRESS_STATUS[i]->Set__DATA(STR__ATM);
			}
			else if( (str_atm_sns.CompareNoCase("OFF") == 0) && 
				(str_vac_sns.CompareNoCase("ON")  == 0) )
			{
				dCH__PRESS_STATUS[i]->Set__DATA(STR__VAC);
			}
			else
			{
				dCH__PRESS_STATUS[i]->Set__DATA(STR__BTW);
			}
		}

	}	
}
