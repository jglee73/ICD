#include "StdAfx.h"
#include "CObj__MADEE_IO.h"
#include "CCommon_Utility.h"


// ...
int CObj__MADEE_IO
::Call__INIT(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm)
{
	sCH__OBJ_STATUS->Set__DATA("STANDBY");

	return OBJ_AVAILABLE;
}

// ...
int CObj__MADEE_IO
::Call__ALL_CLOSE(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm)
{

	return OBJ_AVAILABLE;
}

int CObj__MADEE_IO
::Call__CONTROL_CFG(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm)
{
	// ...
	{
		double cfg_ratio = aCH__CFG_SET_RATIO_MASTER->Get__VALUE();
		aCH__PARA_SET_RATIO_MASTER->Set__VALUE(cfg_ratio);
	}

	return Call__CONTROL(p_variable,p_alarm);
}
int CObj__MADEE_IO
::Call__CONTROL(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm)
{
	Fnc__SET_RATIO(p_variable,p_alarm);

	return OBJ_AVAILABLE;
}

int CObj__MADEE_IO
::Fnc__SET_RATIO(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm)
{
	CString log_msg;
	CString log_bff;

	CString	ch_data = aCH__PARA_SET_RATIO_MASTER->Get__STRING();

	float set_ratio__master = atof(ch_data);
	float set_ratio__slave  = 100.0 - set_ratio__master;

	// ...
	{
		ch_data.Format("%0.1f", set_ratio__slave);
		aCH__PARA_SET_RATIO_SLAVE->Set__DATA(ch_data);
	}

	// ...
	{
		log_msg = "Fnc__SET_RATIO() : Started ... \n";

		log_bff.Format("  * %s <- %s \n",
						aCH__PARA_SET_RATIO_MASTER->Get__CHANNEL_NAME(),
						aCH__PARA_SET_RATIO_MASTER->Get__STRING());
		log_msg += log_bff;

		log_bff.Format("  * %s <- %s \n",
						aCH__PARA_SET_RATIO_SLAVE->Get__CHANNEL_NAME(),
						aCH__PARA_SET_RATIO_SLAVE->Get__STRING());
		log_msg += log_bff;

		xI_LOG_CTRL->WRITE__LOG(log_msg);		
	}

	// SW Ratio & Set Ratio
	{
		byte uch_data[11];
		memset(uch_data, 0, 11);

		//
		byte ctrl_mode = 0x00;

		UNION_2_BYTE__UINT m_ratio__master;
		m_ratio__master.uiDATA = 0x0ffff & (unsigned int) ((set_ratio__master / 100.0) * 0x8000);

		UNION_2_BYTE__UINT m_ratio__slave;
		m_ratio__slave.uiDATA = 0x0ffff & (unsigned int) ((set_ratio__slave / 100.0) * 0x8000);

		//
		uch_data[0] = 0x0ff & ctrl_mode;

		uch_data[1] = 0x0ff & m_ratio__master.cBYTE[0];		// Low
		uch_data[2] = 0x0ff & m_ratio__master.cBYTE[1];		// High

		uch_data[3] = 0x0ff & m_ratio__slave.cBYTE[0];		// Low
		uch_data[4] = 0x0ff & m_ratio__slave.cBYTE[1];		// High

		//
		CString ch_hexa;

		int i_limit = 5;
		int i;

		ch_data = "";

		for(i=0; i<i_limit; i++)
		{
			ch_hexa.Format("%02X ", 0x0ff & uch_data[i]);
			ch_data += ch_hexa;
		}
	}

	sEXT_CH__IO_OUT_BYTE_HEXA->Set__DATA(ch_data);

	// ...
	{
		log_msg = "Fnc__CH1_SET_RATIO() : End ... \n";

		log_bff.Format("  * %s <- %s \n",
						sEXT_CH__IO_OUT_BYTE_HEXA->Get__CHANNEL_NAME(),
						ch_data);
		log_msg += log_bff;

		xI_LOG_CTRL->WRITE__LOG(log_msg);		
	}

	return OBJ_AVAILABLE;
}
