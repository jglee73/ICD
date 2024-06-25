#include "StdAfx.h"
#include "CObj__OHT_E84_IO.h"
#include "CObj__OHT_E84_IO__DEF.h"

#include "math.h"


//-------------------------------- [LOG && Message] ----------------------------------------------
void CObj__OHT_E84_IO::
Fnc__PIO_LOG(const CString& log_msg)
{
	if(iFlag__PIO_LOG < 1)		return;

	xPIO_LOG_CTRL->WRITE__LOG(log_msg);
}

void CObj__OHT_E84_IO::
Fnc__E84_LOG(const CString& log_msg)
{
	if(iFlag__E84_LOG < 1)		return;

	xE84_LOG_CTRL->WRITE__LOG(log_msg);
}

void CObj__OHT_E84_IO::
Fnc__APP_MSG(const CString& log_msg)
{
	if(iFlag__APP_LOG < 1)		return;

	sCH__INR_LP__MSG->Set__DATA(log_msg);
}

int  CObj__OHT_E84_IO::
Is__LP_AUTO_MODE()
{
	if(dEXT_CH__LINK_PIO_ACTIVE_FA_AUTO->Check__DATA(_ON) > 0)
	{
		return 1;
	}

	return -1;
}

//-------------------------------------------------------------------------
// Is__Abort
int CObj__OHT_E84_IO::
Is__Abort(CII_OBJECT__VARIABLE* p_variable)
{
	if(dEXT_CH__LINK_PIO_ACTIVE_RUN->Check__DATA(_ON) < 0)
	{
		return 1;
	}

	return -1;
}

int  CObj__OHT_E84_IO::
Is__ONLINE(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm)
{
	CString title;
	CString msg;

COMM_RTY:

	if(sCH__COMM_STS->Check__DATA("OFFLINE") > 0)
	{
		int alarm_id = ALID__LOADPORT_OFFLINE_ALARM;
		CString r_act;

		p_alarm->Popup__ALARM(alarm_id,r_act);

		if(r_act.CompareNoCase("RETRY") == 0)		goto COMM_RTY;
		else										return -1;
	}
	return 1;
}

//-------------------------------------------------------------------------
// Get__Read_TP_Config
int	CObj__OHT_E84_IO::
Get__Read_TP_Config()
{
	CString ch_data;

	int i_limit;
	int i;

	// CFG : PIO.TPx ...
	{
		i_limit = _CFG__PIO_TPx;

		for(i=0; i<i_limit; i++)
		{
			aCH__EXT_CFG_PIO_TPx[i]->Get__DATA(ch_data);

				 if(i == 0)		m_nTP1 = atoi(ch_data);
			else if(i == 1)		m_nTP2 = atoi(ch_data);
			else if(i == 2)		m_nTP3 = atoi(ch_data);
			else if(i == 3)		m_nTP4 = atoi(ch_data);
			else if(i == 4)		m_nTP5 = atoi(ch_data);
		}
	}
	// CFG : PIO.TDx ...
	{
		i_limit = _CFG__PIO_TDx;

		for(i=0; i<i_limit; i++)
		{
			aCH__EXT_CFG_PIO_TDx[i]->Get__DATA(ch_data);

			if(i == 0)		m_nTD1 = atoi(ch_data);
		}
	}

	return 1;
}

//-------------------------------------------------------------------------
int CObj__OHT_E84_IO::CMD_STAMP(const CString ctrl_mode,
									const CString cmd_name,
									CII__VAR_DIGITAL_CTRL* pch_set_cmd, 
									CII__VAR_DIGITAL_CTRL* pch_get_sns,
									CII_OBJECT__ALARM* p_alarm)
{
	CString str_cmd;
	CString str_sns_value;
	CString str_log;

	str_cmd = ctrl_mode;
	str_cmd.MakeUpper();

RETRY_CMD:
	if(pch_get_sns->Check__DATA(str_cmd) > 0)
	{
		str_log.Format("  Already [%s]...Status [%s]...", cmd_name, str_cmd);
		Fnc__E84_LOG(str_log);

		return 1;
	}

	str_log.Format("  [%s] Command [%s] SET", cmd_name, str_cmd);
	Fnc__E84_LOG(str_log);

	pch_set_cmd->Set__DATA(str_cmd);

	if(iActive__SIM_MODE > 0)
	{
		pch_get_sns->Set__DATA(str_cmd);
	}

	// ...
	int state = pch_get_sns->When__DATA(str_cmd, CMD_TIMEOUT);

	if(state < 0)
	{
		str_log.Format("  [%s] Command [%s] TimeOut", cmd_name, str_cmd);
		Fnc__E84_LOG(str_log);

		int alarm_id = ALID__CMD_TIMEOUT_ALARM;
		CString r_act;
		p_alarm->Popup__ALARM(alarm_id,r_act);

		if(r_act.CompareNoCase("RETRY") == 0)
		{
			str_log.Format("  [%s] Command [%s] Retry", cmd_name, str_cmd);
			Fnc__E84_LOG(str_log);

			goto RETRY_CMD;
		}
		else
		{
			return OBJ_ABORT;
		}
	}
	else if(state == 0)
	{
		return -1;
	}

	pch_get_sns->Get__DATA(str_sns_value);
	str_log.Format("  [%s] Command Completed, Sense value [%s]", cmd_name, str_sns_value);
	Fnc__E84_LOG(str_log);

	return OBJ_AVAILABLE;
}
