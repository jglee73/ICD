#include "StdAfx.h"
#include "CObj__BROOKS_RS232.h"
#include "CObj__BROOKS_RS232__DEF.h"


//------------------------------------------------------------------------------------
void CObj__BROOKS_RS232::
Mon__PIO_TRANSFER_MONITOR(CII_OBJECT__VARIABLE* p_variable,
						  CII_OBJECT__ALARM* p_alarm)
{
	SCX__TIMER_CTRL cx_timer_ctrl;
	CString var_data;

	while(1)
	{
		_sleep(500);

		dEXT_CH__PHY_LPx__PIO_TRANSFER_SNS->Get__DATA(var_data);
		dCH__OTR_OUT_dLP_PIO_TRANSFER->Set__DATA(var_data);
	}
}


//------------------------------------------------------------------------------------
void CObj__BROOKS_RS232::
Mon__IO_MONITOR(CII_OBJECT__VARIABLE* p_variable,
				CII_OBJECT__ALARM* p_alarm)
{
	int init_flag = -1;
	CString var_data;

	while(1)
	{
		Sleep(500);

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

		if(sCH__OTR_IN_LP__FA_MODE->Check__DATA(STR__AUTO) > 0)
		{
			Seq_LAMP(LAMP_AUTO, "ON", "AUTO_LAMP", "Mon__IO_MONITOR");
		}
		else
		{
			Seq_LAMP(LAMP_AUTO, "OFF", "AUTO_LAMP", "Mon__IO_MONITOR");
		}
	}
}

//------------------------------------------------------------------------------------
void CObj__BROOKS_RS232::
Mon__SetVal_To_Lower_Obj(CII_OBJECT__VARIABLE* p_variable,
						 CII_OBJECT__ALARM* p_alarm)
{
	SCX__TIMER_CTRL cx_timer_ctrl;
	CString var_data;

	while(1)
	{
		//cx_timer_ctrl->WAIT(0.5);
		_sleep(500);

		sCH__OTR_IN_LP__FA_MODE->Get__DATA(var_data);
		sEXT_CH__PHY_LPx__PIO_FA_MODE->Set__DATA(var_data);

		sCH__OTR_IN_LP__FA_SERVICE_MODE->Get__DATA(var_data);
		sEXT_CH__PHY_LPx__PIO_SERV_REQ->Set__DATA(var_data);
	}
}

void CObj__BROOKS_RS232::
Mon__IO_CH_OUT_MONITOR(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	SCX__TIMER_CTRL cx_timer_ctrl;
	CString var_data;

	Sleep(1000);

	while(1)
	{
		_sleep(100);

		if(sEXT_CH__LPx__MON_CLAMP_STATUS->Check__DATA("CLAMP") > 0)
		{
			dCH__OTR_OUT_MON_dLP_CLAMP_STS->Set__DATA("CLAMP");

			dCH__LP__UNCLAMP_STS->Set__DATA("OFF");
			dCH__LP__CLAMP_STS->Set__DATA("ON");
		}
		else if(sEXT_CH__LPx__MON_CLAMP_STATUS->Check__DATA("UNCLAMP") > 0)
		{
			dCH__OTR_OUT_MON_dLP_CLAMP_STS->Set__DATA("UNCLAMP");

			dCH__LP__CLAMP_STS->Set__DATA("OFF");
			dCH__LP__UNCLAMP_STS->Set__DATA("ON");
		}
		else
		{
			dCH__OTR_OUT_MON_dLP_CLAMP_STS->Set__DATA("UNKNOWN");

			dCH__LP__CLAMP_STS->Set__DATA("ON");
			dCH__LP__UNCLAMP_STS->Set__DATA("ON");
		}

		// _sleep(100);
		/*
		sEXT_CH__LPx__MON_FOUP_STATUS->Get__DATA(var_data);
		dCH__OTR_OUT_MON_dLP_CST_STS->Set__DATA(var_data);
		*/

		// _sleep(100);

		sEXT_CH__LPx__MON_DOOR_STATUS->Get__DATA(var_data);
		dCH__OTR_OUT_MON_dLP_DOOR_STS->Set__DATA(var_data);

		// _sleep(100);

		sEXT_CH__LPx__MON_FOUP_POS_STATUS->Get__DATA(var_data);
		dCH__OTR_OUT_MON_dLP_SHUTL_STS->Set__DATA(var_data);
	}
}

void CObj__BROOKS_RS232::
Mon__IO_CST_EXIST_MONITOR(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	CString var_data;
	double nCnt = 0;

	aCFG_CST_PROPERLY_EXIST_TIME->Get__DATA(var_data);
	double nTarget_Cnt = atof(var_data) * 0.01;

	while(1)
	{
		_sleep(100);

		if(sEXT_CH__LPx__MON_FOUP_STATUS->Check__DATA("EXIST") > 0)
		{
			nCnt++;

			if(nTarget_Cnt <= nCnt)
			{
				dCH__OTR_OUT_MON_dLP_CST_STS->Set__DATA("EXIST");
				nCnt = 0;
			}
			else
			{
				continue;			
			}
		}
		else if(sEXT_CH__LPx__MON_FOUP_STATUS->Check__DATA("NONE") > 0) 
		{
			dCH__OTR_OUT_MON_dLP_CST_STS->Set__DATA("NONE");
			nCnt = 0;
		}
		else
		{
			dCH__OTR_OUT_MON_dLP_CST_STS->Set__DATA("UNKNOWN");
			nCnt = 0;
		}
	}
}

//-------------------------------------------------------------------------
// Fnc__LD_buttonMon
int CObj__BROOKS_RS232::
Fnc__LD_buttonMon(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM *p_alarm)
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
	int nCnt = 0;

	while(1)
	{
		_sleep(100);

		sEXT_CH__LPx__MON_LOAD_BTN_SNS->Get__DATA(szLDBT);
		aCFG_ACCESS_SW_TIMEOUT->Get__DATA(szCfg_Data);		// 3 sec..

		if(szLDBT.CompareNoCase("ON") == 0)
		{
			if(atoi(szCfg_Data) > nCnt)
			{
				_sleep(1000);
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

		_sleep(100);
	}

	return 1;
}

//-------------------------------------------------------------------------
// Fnc__LD_buttonMon
int CObj__BROOKS_RS232::
Fnc__UNLD_buttonMon(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM *p_alarm)
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
	int        nCnt = 0;

	while(1)
	{
		_sleep(100);

		sEXT_CH__LPx__MON_UNLOAD_BTN_SNS->Get__DATA(szLDBT);
		aCFG_ACCESS_SW_TIMEOUT->Get__DATA(szCfg_Data);		// 3 sec..

		if(szLDBT.CompareNoCase("ON") == 0)
		{
			if(atoi(szCfg_Data) > nCnt)
			{
				_sleep(1000);
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
					dCH__INR_CFG_UNLD_CLAMP_OPT->Get__DATA(szCFGOpt);
					dCH__OTR_OUT_MON_dLP_CLAMP_STS->Get__DATA(szclamp_sts);
					dCH__OTR_OUT_MON_dLP_CST_STS->Get__DATA(szExist);
					dCH__OTR_OUT_MON_dLP_DOOR_STS->Get__DATA(szDoor);

					if(szCFGOpt.Compare("Type1") == 0)
					{
						if((szclamp_sts.CompareNoCase("CLAMP") == 0) 
						&& (szDoor.CompareNoCase("CLOSE")      == 0) 
						&& (szExist.CompareNoCase("EXIST")     == 0))  
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

		_sleep(100);
	}

	return 1;
}
