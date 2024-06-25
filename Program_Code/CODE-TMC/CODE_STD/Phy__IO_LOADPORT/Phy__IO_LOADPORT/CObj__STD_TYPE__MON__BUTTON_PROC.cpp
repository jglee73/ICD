#include "StdAfx.h"
#include "CObj__STD_TYPE.h"
#include "CObj__STD_TYPE__DEF.h"


// ...
int CObj__STD_TYPE
::Mon__Button_Proc(CII_OBJECT__VARIABLE* p_variable, 
				   CII_OBJECT__ALARM *p_alarm)
{
	int cnt_load = 0;
	int cnt_unld = 0;

	while(1)
	{
		p_variable->Wait__SINGLE_OBJECT(0.1);

		Fnc__LD_buttonMon(p_variable, p_alarm, cnt_load);
		Fnc__UNLD_buttonMon(p_variable, p_alarm, cnt_unld); 
	}
	return 1;
}

int CObj__STD_TYPE
::Fnc__LD_buttonMon(CII_OBJECT__VARIABLE* p_variable, 
					CII_OBJECT__ALARM *p_alarm, 
					int& nCnt)
{
	CString szData_bak = "~";
	CString szNotPressedData_bak = "~";
	CString szLDBT;
	CString szclamp_sts;
	CString szExist;
	CString szCFGOpt;
	CString	szDoor;
	CString fnc_name = "Fnc__LD_buttonMon";
	CString szCfg_Data;

	// ...
	{
		sEXT_CH__LPx__MON_LOAD_BTN_SNS->Get__DATA(szLDBT);
		aCFG_ACCESS_SW_TIMEOUT->Get__DATA(szCfg_Data);		// 3 sec..

		if(szLDBT.CompareNoCase("ON") == 0)
		{
			if(atoi(szCfg_Data) > nCnt)
			{
				_sleep(1000);
				nCnt++;
				return 1;
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
					dCH__INR_CFG_DYNAMIC_USE->Get__DATA(szCFGOpt);
					dCH__OTR_OUT_MON_dLP_CLAMP_STS->Get__DATA(szclamp_sts);
					dCH__OTR_OUT_MON_dLP_CST_STS->Get__DATA(szExist);
					dCH__OTR_OUT_MON_dLP_DOOR_STS->Get__DATA(szDoor);

					if(szCFGOpt.Compare("NO") == 0)
					{
						if((szclamp_sts.CompareNoCase("UNCLAMP") == 0) 
							&& (szExist.CompareNoCase("EXIST")       == 0))
						{
							Fnc__APP_LOG("--------> Load Button [CLAMP] Started !! ---------->");
							Fnc__CLAMP(p_variable, p_alarm,fnc_name);
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
			if(strcmp(szNotPressedData_bak, szLDBT) != 0)
			{
				if(Is__LP_AUTO_MODE() < 0)		// manual
				{
					if(dCH__OTR_OUT_MON_dLP_CST_STS->Check__DATA("EXIST") > 0)
					{
						if(dCH__INR_CFG_DYNAMIC_USE->Check__DATA("NO") > 0)
						{
							Seq_LAMP(LAMP_LOAD,   "ON",  "LOAD_LAMP",   "Fnc__LD_buttonMon()");
							Sleep(1000);
							Seq_LAMP(LAMP_LOAD,   "OFF", "LOAD_LAMP",   "Fnc__LD_buttonMon()");

						}
					}
				}
			}

			szData_bak = "~";
		}
	}
	return 1;
}
int CObj__STD_TYPE
::Fnc__UNLD_buttonMon(CII_OBJECT__VARIABLE* p_variable, 
					  CII_OBJECT__ALARM *p_alarm, 
					  int& nCnt)
{
	CString    szData_bak			= "~";
	CString    szNotPressedData_bak	= "~";
	CString    szLDBT;
	CString    szclamp_sts;
	CString    szExist;
	CString    szCFGOpt;
	CString	   szDoor;
	CString    fnc_name = "Fnc__UNLD_buttonMon";
	CString    szCfg_Data;

	// ...
	{
		sEXT_CH__LPx__MON_UNLOAD_BTN_SNS->Get__DATA(szLDBT);
		aCFG_ACCESS_SW_TIMEOUT->Get__DATA(szCfg_Data);		// 3 sec..

		if(szLDBT.CompareNoCase("ON") == 0)
		{
			if(atoi(szCfg_Data) > nCnt)
			{
				_sleep(1000);
				nCnt++;
				return 1;
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
					dCH__INR_CFG_UNLD_CLAMP_OPT->Get__DATA(szCFGOpt);
					dCH__OTR_OUT_MON_dLP_CLAMP_STS->Get__DATA(szclamp_sts);
					dCH__OTR_OUT_MON_dLP_CST_STS->Get__DATA(szExist);
					dCH__OTR_OUT_MON_dLP_DOOR_STS->Get__DATA(szDoor);

					if(szCFGOpt.Compare("Type1") == 0)
					{
						if((szclamp_sts.CompareNoCase("CLAMP") == 0) 
							&& (szDoor.CompareNoCase("CLOSE")     == 0) 
							&& (szExist.CompareNoCase("EXIST")    == 0) )  
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
			if(strcmp(szNotPressedData_bak, szLDBT) != 0)
			{
				if(Is__LP_AUTO_MODE() < 0)		// manual
				{
					if(dCH__OTR_OUT_MON_dLP_CST_STS->Check__DATA("EXIST") > 0)
					{
						if(dCH__INR_CFG_UNLD_CLAMP_OPT->Check__DATA("Type1") > 0)
						{
							Seq_LAMP(LAMP_UNLOAD,   "ON",  "UNLOAD_LAMP",   "Fnc__UNLD_buttonMon()");
							Sleep(1000);
							Seq_LAMP(LAMP_UNLOAD,   "OFF", "UNLOAD_LAMP",   "Fnc__UNLD_buttonMon()");

						}
					}
				}
			}

			szData_bak = "~";
		}
	}

	return 1;
}
