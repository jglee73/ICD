#include "StdAfx.h"
#include "CObj__BROOKS_IO.h"
#include "CObj__BROOKS_IO__DEF.h"


//------------------------------------------------------------------------------------
void CObj__BROOKS_IO::
Mon__PIO_TRANSFER_MONITOR(CII_OBJECT__VARIABLE* p_variable,
						  CII_OBJECT__ALARM* p_alarm)
{
	CString var_data;


	while(1)
	{
		p_variable->Wait__SINGLE_OBJECT(0.5);


		if(bActive__LINK_PIO)
		{
			var_data = dEXT_CH__LPx_PIO__TRANSFER_SNS->Get__STRING();
			dCH__OTR_OUT_dLP_PIO_TRANSFER->Set__DATA(var_data);

			var_data = sCH__MON_ACTIVE_FA_AUTO->Get__STRING();
			sEXT_CH__LPx_PIO__MON_ACTIVE_FA_AUTO->Set__DATA(var_data);
		}
	}
}


//------------------------------------------------------------------------------------
void CObj__BROOKS_IO::
Mon__IO_MONITOR(CII_OBJECT__VARIABLE* p_variable,
				CII_OBJECT__ALARM* p_alarm)
{
	CString var_data;


	while(1)
	{
		p_variable->Wait__SINGLE_OBJECT(0.5);


		if(iSIM_FLAG > 0)
		{
			if(sCH__COMM_STS->Check__DATA("ONLINE") < 0)
			{
				sCH__COMM_STS->Set__DATA("ONLINE");
			}
		}
		else
		{
			if(diEXT_CH__LPx__COMM_STS->Check__DATA("OFFLINE") > 0)
			{
				sCH__COMM_STS->Set__DATA("OFFLINE");

				// ...
				{
					CString r_act;
					int alarm_id = ALID__OFFLINE_ALARM;

					p_alarm->Check__ALARM(alarm_id,r_act);
					p_alarm->Post__ALARM(alarm_id);
				}
			}
			else
			{
				sCH__COMM_STS->Set__DATA("ONLINE");
			}
		}

		if(sCH__MON_ACTIVE_FA_AUTO->Check__DATA(STR__YES) > 0)
		{
			sCH__MON_ACTIVE_FA_AUTO->Set__DATA(STR__YES);

			Seq_LAMP(LAMP_AUTO, "ON", "AUTO_LAMP", "Mon__IO_MONITOR");
		}
		else
		{
			sCH__MON_ACTIVE_FA_AUTO->Set__DATA(STR__NO);

			Seq_LAMP(LAMP_AUTO, "OFF", "AUTO_LAMP", "Mon__IO_MONITOR");
		}
	}
}

//------------------------------------------------------------------------------------
void CObj__BROOKS_IO::
Mon__IO_CH_OUT_MONITOR(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	SCX__TIMER_CTRL cx_timer_ctrl;
	CString var_data;

	Sleep(1000);

	// ...
	aCFG_CST_PROPERLY_EXIST_TIME->Get__DATA(var_data);
	int cfg__foup_check = atof(var_data) * 0.01;
	int cnt__foup_check = 0;


	while(1)
	{
		p_variable->Wait__SINGLE_OBJECT(0.1);


		// CLAMP STATE ...
		{
			CString str_clamp   = diEXT_CH__LPx__CLAMP->Get__STRING();
			CString str_unclamp = diEXT_CH__LPx__UNCLAMP->Get__STRING();

			if((str_unclamp.CompareNoCase(STR__OFF) == 0)
			&& (str_clamp.CompareNoCase(STR__ON) == 0))
			{
				dCH__MON_CLAMP_STATUS->Set__DATA(STR__CLAMP);

				dCH__LP__UNCLAMP_STS->Set__DATA(STR__OFF);
				dCH__LP__CLAMP_STS->Set__DATA(STR__ON);
			}
			else if((str_unclamp.CompareNoCase(STR__ON) == 0)
				 && (str_clamp.CompareNoCase(STR__OFF) == 0))
			{
				dCH__MON_CLAMP_STATUS->Set__DATA(STR__UNCLAMP);

				dCH__LP__CLAMP_STS->Set__DATA(STR__OFF);
				dCH__LP__UNCLAMP_STS->Set__DATA(STR__ON);
			}
			else
			{
				dCH__MON_CLAMP_STATUS->Set__DATA(STR__UNKNOWN);

				dCH__LP__CLAMP_STS->Set__DATA(STR__ON);
				dCH__LP__UNCLAMP_STS->Set__DATA(STR__ON);
			}
		}

		// FOUP STATE ...
		{
			CString str_present = diEXT_CH__LPx__FOUP_PRESENT->Get__STRING();
			CString str_placed = diEXT_CH__LPx__FOUP_PLACED->Get__STRING();

			if((str_present.CompareNoCase(STR__ON) == 0) 
			&& (str_placed.CompareNoCase(STR__ON) == 0))
			{
				cnt__foup_check++;

				if(cfg__foup_check <= cnt__foup_check)
					dCH__MON_FOUP_STATUS->Set__DATA(STR__EXIST);
			}
			else if((str_present.CompareNoCase(STR__OFF) == 0) 
			     && (str_placed.CompareNoCase(STR__OFF) == 0))
			{	
				cnt__foup_check = 0;

				dCH__MON_FOUP_STATUS->Set__DATA(STR__NONE);
			}
			else
			{
				cnt__foup_check = 0;

				dCH__MON_FOUP_STATUS->Set__DATA(STR__UNKNOWN);
			}
		}

		// DOOR STATE ...
		{
			if((diEXT_CH__LPx__DOOR_RET->Check__DATA(STR__ON) > 0) 
			&& (diEXT_CH__LPx__DOOR_UP->Check__DATA(STR__ON) > 0))
			{
				dCH__MON_DOOR_STATUS->Set__DATA(STR__CLOSE);
			}
			else if((diEXT_CH__LPx__DOOR_EXT->Check__DATA(STR__ON) > 0) 
				 && (diEXT_CH__LPx__DOOR_DN->Check__DATA(STR__ON) > 0))
			{
				dCH__MON_DOOR_STATUS->Set__DATA(STR__OPEN);
			}
			else
			{
				dCH__MON_DOOR_STATUS->Set__DATA(STR__UNKNOWN);
			}
		}

		// SHUTTLE STATE ...
		{
			CString str_in  = diEXT_CH__LPx__SHUTTLE_IN->Get__STRING();
			CString str_out = diEXT_CH__LPx__SHUTTLE_OUT->Get__STRING();

			if((str_in.CompareNoCase(STR__ON) == 0) 
			&& (str_out.CompareNoCase(STR__OFF) == 0))
			{
				dCH__MON_FOUP_POS_STATUS->Set__DATA(STR__LOAD);
			}
			else if((str_in.CompareNoCase(STR__OFF) == 0) 
				 && (str_out.CompareNoCase(STR__ON) == 0))
			{
				dCH__MON_FOUP_POS_STATUS->Set__DATA(STR__UNLOAD);
			}
			else
			{
				dCH__MON_FOUP_POS_STATUS->Set__DATA(STR__UNKNOWN);
			}
		}

		 // ...
	}
}

int CObj__BROOKS_IO::
Fnc__LD_buttonMon(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM *p_alarm)
{
	CString szData_bak = "~";
	CString szNotPressedData_bak = "~";
	CString szLDBT;
	CString szclamp_sts;
	CString szExist;
	CString	szDoor;
	CString fnc_name = "Fnc__LD_buttonMon";
	CString szCfg_Data;

	int nCnt = 0;


	while(1)
	{
		p_variable->Wait__SINGLE_OBJECT(0.1);


		diEXT_CH__LPx__LOAD_BTN->Get__DATA(szLDBT);
		aCFG_ACCESS_SW_TIMEOUT->Get__DATA(szCfg_Data);		// 3 sec..

		if(szLDBT.CompareNoCase(STR__ON) == 0)
		{
			if(atoi(szCfg_Data) > nCnt)
			{
				_sleep(900);
				nCnt++;
				continue;
			}
			nCnt = 0;

			if(strcmp(szData_bak, szLDBT) != 0)
			{
				Fnc__APP_LOG("--------> Load Button [UNLOAD LAMP OFF, LOAD LAMP ON] Started !! ---------->");

				Seq_LAMP(LAMP_UNLOAD, "OFF", "UNLOAD_LAMP", fnc_name);
				Seq_LAMP(LAMP_LOAD,   "ON",  "LOAD_LAMP",   fnc_name);

				Fnc__APP_LOG("<-------- Load Button [UNLOAD LAMP OFF, LOAD LAMP ON] Completed !! <-------\n");

				if(Is__LP_AUTO_MODE() < 0)		// manual
				{
					dCH__MON_CLAMP_STATUS->Get__DATA(szclamp_sts);
					dCH__MON_FOUP_STATUS->Get__DATA(szExist);
					dCH__MON_DOOR_STATUS->Get__DATA(szDoor);

					if(dCH__CFG_LOAD_CLAMP_OPT->Check__DATA(STR__BUTTON) > 0)
					{
						if((szclamp_sts.CompareNoCase(STR__UNCLAMP) == 0)
						&& (szExist.CompareNoCase(STR__EXIST) == 0))
						{
							Fnc__APP_LOG("--------> Load Button [CLAMP] Started !! ---------->");

							Fnc__CLAMP(p_variable, p_alarm, fnc_name);

							Fnc__APP_LOG("<-------- Load Button Completed !! <-------");

						}
					}
				}

				szData_bak = szLDBT;
				szNotPressedData_bak = "~";
			}
		}
		else
		{
			nCnt = 0;

			if(strcmp(szNotPressedData_bak, szLDBT) != 0)
			{
				if(Is__LP_AUTO_MODE() < 0)		// manual
				{
					if(dCH__MON_FOUP_STATUS->Check__DATA(STR__EXIST) > 0)
					{
						if(dCH__CFG_LOAD_CLAMP_OPT->Check__DATA(STR__BUTTON) > 0)
						{
							Seq_LAMP(LAMP_LOAD, "ON", "LOAD_LAMP", "Fnc__LD_buttonMon()");

							Sleep(500);

							Seq_LAMP(LAMP_LOAD, "OFF", "LOAD_LAMP", "Fnc__LD_buttonMon()");
						}
					}
				}
			}

			szData_bak = "~";
		}

		// ...
	}

	return 1;
}

int CObj__BROOKS_IO::
Fnc__UNLD_buttonMon(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM *p_alarm)
{
	CString szData_bak = "~";
	CString szNotPressedData_bak = "~";
	CString szLDBT;
	CString szclamp_sts;
	CString szExist;
	CString	szDoor;
	CString fnc_name = "Fnc__UNLD_buttonMon";
	CString szCfg_Data;

	int nCnt = 0;


	while(1)
	{
		p_variable->Wait__SINGLE_OBJECT(0.1);


		diEXT_CH__LPx__UNLOAD_BTN->Get__DATA(szLDBT);
		aCFG_ACCESS_SW_TIMEOUT->Get__DATA(szCfg_Data);		// 3 sec..

		if(szLDBT.CompareNoCase(STR__ON) == 0)
		{
			if(atoi(szCfg_Data) > nCnt)
			{
				_sleep(900);
				nCnt++;
				continue;
			}
			nCnt = 0;

			if(strcmp(szData_bak, szLDBT) != 0)
			{
				Fnc__APP_LOG("--------> Unload Button [UNLOAD LAMP ON, LOAD LAMP OFF] Started !! ---------->");

				Seq_LAMP(LAMP_LOAD,   "OFF",  "LOAD_LAMP", fnc_name);
				Seq_LAMP(LAMP_UNLOAD, "ON", "UNLOAD_LAMP", fnc_name);

				Fnc__APP_LOG("<-------- Unload Button [UNLOAD LAMP ON, LOAD LAMP OFF] Completed !! <-------\n");

				if(Is__LP_AUTO_MODE() < 0)		// manual
				{
					dCH__MON_CLAMP_STATUS->Get__DATA(szclamp_sts);
					dCH__MON_FOUP_STATUS->Get__DATA(szExist);
					dCH__MON_DOOR_STATUS->Get__DATA(szDoor);

					if(dCH__CFG_UNLD_CLAMP_OPT->Check__DATA(STR__BUTTON) > 0)
					{
						if((szclamp_sts.CompareNoCase(STR__CLAMP) == 0) 
						&& (szDoor.CompareNoCase(STR__CLOSE) == 0) 
						&& (szExist.CompareNoCase(STR__EXIST) == 0))  
						{
							Fnc__APP_LOG("--------> Unload Button [UNCLAMP] Started !! ---------->");

							Fnc__UNCLAMP(p_variable, p_alarm, fnc_name);

							Fnc__APP_LOG("<-------- Unload Button Completed !! <-------");
						}
					}
				}

				szData_bak = szLDBT;
				szNotPressedData_bak = "~";
			}
		}
		else
		{
			nCnt = 0;

			if(strcmp(szNotPressedData_bak, szLDBT) != 0)
			{
				if(Is__LP_AUTO_MODE() < 0)		// manual
				{
					if(dCH__MON_FOUP_STATUS->Check__DATA(STR__EXIST) > 0)
					{
						if(dCH__CFG_UNLD_CLAMP_OPT->Check__DATA(STR__BUTTON) > 0)
						{
							Seq_LAMP(LAMP_UNLOAD, "ON", "UNLOAD_LAMP", "Fnc__UNLD_buttonMon()");
							
							Sleep(500);
							
							Seq_LAMP(LAMP_UNLOAD, "OFF", "UNLOAD_LAMP", "Fnc__UNLD_buttonMon()");
						}
					}
				}
			}

			szData_bak = "~";
		}

		// ...
	}

	return 1;
}
