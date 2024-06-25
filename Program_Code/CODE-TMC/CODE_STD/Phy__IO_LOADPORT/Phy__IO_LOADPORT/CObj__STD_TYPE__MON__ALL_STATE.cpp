#include "StdAfx.h"
#include "CObj__STD_TYPE.h"
#include "CObj__STD_TYPE__DEF.h"


// ...
void CObj__STD_TYPE
::Mon__ALL_STATE(CII_OBJECT__VARIABLE* p_variable,
				 CII_OBJECT__ALARM* p_alarm)
{
	int nCnt_CST = 0;

	while(1)
	{
		p_variable->Wait__SINGLE_OBJECT(0.1);

		Fnc__PIO_TRANSFER_MONITOR(p_variable, p_alarm);
		// Fnc__IO_MONITOR(p_variable, p_alarm);
		Fnc__SetVal_To_Lower_Obj(p_variable, p_alarm);
		Fnc__IO_CH_OUT_MONITOR(p_variable, p_alarm);
		Fnc__IO_CST_EXIST_MONITOR(p_variable, p_alarm, nCnt_CST);
	}
}

void CObj__STD_TYPE
::Fnc__PIO_TRANSFER_MONITOR(CII_OBJECT__VARIABLE* p_variable,
						    CII_OBJECT__ALARM* p_alarm)
{
	CString var_data;

	dEXT_CH__PHY_LPx__PIO_TRANSFER_SNS->Get__DATA(var_data);
	dCH__OTR_OUT_dLP_PIO_TRANSFER->Set__DATA(var_data);
}
void CObj__STD_TYPE
::Fnc__IO_MONITOR(CII_OBJECT__VARIABLE* p_variable,
				  CII_OBJECT__ALARM* p_alarm)
{
	if(sCH__OTR_IN_LP__FA_MODE->Check__DATA("AUTO") > 0)
		Seq_LAMP(LAMP_AUTO, "ON", "AUTO_LAMP", "Mon__IO_MONITOR");
	else
		Seq_LAMP(LAMP_AUTO, "OFF", "AUTO_LAMP", "Mon__IO_MONITOR");
}
void CObj__STD_TYPE
::Fnc__SetVal_To_Lower_Obj(CII_OBJECT__VARIABLE* p_variable,
						   CII_OBJECT__ALARM* p_alarm)
{
	CString var_data;

	sCH__OTR_IN_LP__FA_MODE->Get__DATA(var_data);
	sEXT_CH__PHY_LPx__PIO_FA_MODE->Set__DATA(var_data);

	sCH__OTR_IN_LP__FA_SERVICE_MODE->Get__DATA(var_data);
	sEXT_CH__PHY_LPx__PIO_SERV_REQ->Set__DATA(var_data);
}
void CObj__STD_TYPE
::Fnc__IO_CH_OUT_MONITOR(CII_OBJECT__VARIABLE* p_variable,
						 CII_OBJECT__ALARM* p_alarm)
{
	CString var_data;

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

	sEXT_CH__LPx__MON_DOOR_STATUS->Get__DATA(var_data);
	dCH__OTR_OUT_MON_dLP_DOOR_STS->Set__DATA(var_data);

	sEXT_CH__LPx__MON_FOUP_POS_STATUS->Get__DATA(var_data);
	dCH__OTR_OUT_MON_dLP_SHUTL_STS->Set__DATA(var_data);
}
void CObj__STD_TYPE
::Fnc__IO_CST_EXIST_MONITOR(CII_OBJECT__VARIABLE* p_variable, 
							CII_OBJECT__ALARM* p_alarm,
							int& nCnt)
{
	CString var_data;

	aCFG_CST_PROPERLY_EXIST_TIME->Get__DATA(var_data);
	double nTarget_Cnt = atof(var_data)*0.01;

	// ...
	{
		if(sEXT_CH__LPx__MON_FOUP_STATUS->Check__DATA("EXIST") > 0)
		{
			nCnt++;

			if(nTarget_Cnt <= (double) nCnt)
			{
				dCH__OTR_OUT_MON_dLP_CST_STS->Set__DATA("EXIST");
				nCnt = 0;
			}
			else
			{
				return;
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

