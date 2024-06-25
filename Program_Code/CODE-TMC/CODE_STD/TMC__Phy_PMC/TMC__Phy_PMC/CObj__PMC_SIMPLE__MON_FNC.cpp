#include "StdAfx.h"
#include "CObj__PMC_SIMPLE.h"
#include "CObj__PMC_SIMPLE__DEF.h"


// ...
void CObj__PMC_SIMPLE
::Mon__IO_MONITOR(CII_OBJECT__VARIABLE* p_variable,
				  CII_OBJECT__ALARM* p_alarm)
{
	CString pre__cfg_dummy;

	if(iActive__SIM_MODE > 0)
	{
		for(int i=0; i<iPM_LIMIT; i++)
		{
			if(dEXT_CH__CFG_PMx_EXIST_FLAG[i]->Check__DATA(STR__YES) < 0)
			{
				dCH__SLIT_VALVE_STATUS[i]->Set__DATA(STR__CLOSE);
			}
			else
			{
				if(bActive__PMx__SV_OPEN_X[i])			doEXT_CH__PMx__SV_OPEN_X[i]->Set__DATA(STR__OFF);
				if(bActive__PMx__SV_CLOSE_X[i])			doEXT_CH__PMx__SV_CLOSE_X[i]->Set__DATA(STR__ON);
			}
		}
	}

	
	while(1)
	{
		p_variable->Wait__SINGLE_OBJECT(0.5);


		if(dEXT_CH__SCH_TEST_CFG_PM_DUMMY_BY_CTC->Check__DATA(pre__cfg_dummy) < 0)
		{
			pre__cfg_dummy = dEXT_CH__SCH_TEST_CFG_PM_DUMMY_BY_CTC->Get__STRING();

			if(pre__cfg_dummy.CompareNoCase(STR__YES) == 0)
			{
				for(int i=0; i<iPM_LIMIT; i++)
				{
					dEXT_CH__PMx_SLOT_VLV__OBJ_STATUS_X[i]->Set__DATA(STR__STANDBY);

					aCH__PRESSURE_TORR[i]->Set__VALUE(0.0);
					sCH__PRESSURE_STATUS[i]->Set__DATA("VAC");
				}
			}
		}

		// TAS RESET ...
		{
			for(int pm_i=0; pm_i<iPM_LIMIT; pm_i++)
			{
				if(sCH__TAS_RESET_REQ__PMx[pm_i]->Check__DATA("") > 0)
				{
					continue;
				}
				sCH__TAS_RESET_REQ__PMx[pm_i]->Set__DATA("");
				
				for(int i=0; i<CFG_ACT__SIZE; i++)
				{
					sCH__TAS_ACTION_TIME_NOW__PMx[i][pm_i]->Set__DATA("");
					sCH__TAS_ACTION_TIME_MIN__PMx[i][pm_i]->Set__DATA("");
					sCH__TAS_ACTION_TIME_MAX__PMx[i][pm_i]->Set__DATA("");
					sCH__TAS_ACTION_TIME_AVG__PMx[i][pm_i]->Set__DATA("");
					sCH__TAS_ACTION_TIME_AVG_F__PMx[i][pm_i]->Set__DATA("");
					aCH__TAS_ACTION_TIME_CNT__PMx[i][pm_i]->Set__VALUE(1);
				}
			}
		}

		Update__IO_MONITOR();
	}	
}

void CObj__PMC_SIMPLE
::Update__IO_MONITOR()
{
	CString str__sv_open;
	CString str__sv_close;

	CString str_atm_sns;
	CString str_vac_sns;

	CString ch_data;
	int i;

	// ...
	bool active__io_off_mode = false;
	if(dCH__CFG_IO_OFF_MODE->Check__DATA(STR__ENABLE) > 0)			active__io_off_mode = true;

	if(iActive__SIM_MODE > 0)
	{
		if(iDATA__PMx_SLOT_VLV_CTRL_TYPE == _DEF__PMx_SLOT_VLV_TYPE__IO)
		{
			for(i=0; i<iPM_LIMIT; i++)
			{
				if(bActive__PMx__SV_OPEN_X[i])		str__sv_open = doEXT_CH__PMx__SV_OPEN_X[i]->Get__STRING();
				else								str__sv_open = STR__OFF;

				if(bActive__PMx__SV_CLOSE_X[i])		str__sv_close = doEXT_CH__PMx__SV_CLOSE_X[i]->Get__STRING();
				else								str__sv_close = STR__OFF;

				if(active__io_off_mode)
				{
					if((str__sv_open.CompareNoCase(STR__OFF)  == 0)
					&& (str__sv_close.CompareNoCase(STR__OFF) == 0))
					{
						continue;
					}
				}

				diEXT_CH__PMx__SV_OPEN[i]->Set__DATA(str__sv_open);
				diEXT_CH__PMx__SV_CLOSE[i]->Set__DATA(str__sv_close);
			}
		}
	}

	for(i=0; i<iPM_LIMIT; i++)
	{
		if(dEXT_CH__CFG_PMx_EXIST_FLAG[i]->Check__DATA("YES") < 0)
		{
			dCH__SLIT_VALVE_STATUS[i]->Set__DATA(STR__CLOSE);		// SV가 NONE이면 CLOSE로 간주
		}
		else
		{
			if(dEXT_CH__CFG_PMx_CHM_TYPE[i]->Check__DATA(STR__DUMMY) > 0)
			{
				if(iActive__SIM_MODE > 0)
				{
					diEXT_CH__PMx__SV_OPEN[i]->Get__DATA(str__sv_open);
					diEXT_CH__PMx__SV_CLOSE[i]->Get__DATA(str__sv_close);
				}
				else
				{
					str__sv_open  = STR__OFF;
					str__sv_close = STR__ON;
				}
			}
			else
			{
				diEXT_CH__PMx__SV_OPEN[i]->Get__DATA(str__sv_open);
				diEXT_CH__PMx__SV_CLOSE[i]->Get__DATA(str__sv_close);
			}

			// ...
			int close_flag = -1;
			int open_flag  = -1;

			if(dEXT_CH__CFG_PMx_SV_USE[i]->Check__DATA(STR__NO) > 0)
			{
				open_flag = 1;
			}
			else
			{
				if((str__sv_open.CompareNoCase(STR__OFF) == 0)
				&& (str__sv_close.CompareNoCase(STR__ON) == 0))
				{
					close_flag = 1;
				}
				else if((str__sv_open.CompareNoCase(STR__ON) == 0)
					 && (str__sv_close.CompareNoCase(STR__OFF) == 0))
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

		// PMx Pressure State ...
		diEXT_CH__PMx__ATM_SNS[i]->Get__DATA(str_atm_sns);
		diEXT_CH__PMx__VAC_SNS[i]->Get__DATA(str_vac_sns);

		if(dEXT_CH__CFG_PMx_CHM_TYPE[i]->Check__DATA(STR__DUMMY) > 0)
		{
			if(dEXT_CH__CFG_TRANSFER_MODE->Check__DATA(STR__ATM) > 0)
			{
				dCH__PRESS_STATUS[i]->Set__DATA(STR__ATM);
				
				aEXT_CH__PHY_TM__PRESS_TORR->Get__DATA(ch_data);
				aCH__PRESSURE_TORR[i]->Set__DATA(ch_data);
			}
			else
			{
				dCH__PRESS_STATUS[i]->Set__DATA(STR__VAC);

				aEXT_CH__PHY_TM__PRESS_TORR->Get__DATA(ch_data);
				aCH__PRESSURE_TORR[i]->Set__DATA(ch_data);
			}
		}
		else
		{
			if((str_atm_sns.CompareNoCase(sDATA__ATM_ON)  == 0) 
			&& (str_vac_sns.CompareNoCase(sDATA__VAC_OFF) == 0))
			{
				dCH__PRESS_STATUS[i]->Set__DATA(STR__ATM);
			}
			else if((str_atm_sns.CompareNoCase(sDATA__ATM_OFF) == 0) 
				 && (str_vac_sns.CompareNoCase(sDATA__VAC_ON)  == 0))
			{
				dCH__PRESS_STATUS[i]->Set__DATA(STR__VAC);
			}
			else
			{
				dCH__PRESS_STATUS[i]->Set__DATA(STR__BTW);
			}
		}
	}

	// ...
}
