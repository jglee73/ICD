#include "StdAfx.h"
#include "CObj__DELTA_II_IO.h"
#include "CCommon_Utility.h"


// ...
int CObj__DELTA_II_IO
::Call__INIT(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm)
{
	sCH__OBJ_STATUS->Set__DATA("STANDBY");

	return OBJ_AVAILABLE;
}

int CObj__DELTA_II_IO
::Call__CH1_SET_RATIO(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm)
{
	return Fnc__CH1_SET_RATIO(p_variable,p_alarm);
}
int CObj__DELTA_II_IO
::Fnc__CH1_SET_RATIO(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm)
{
	CString log_msg;
	CString log_bff;

	CString ch_data;

	aCH__PARA_CH1_SET_RATIO->Get__DATA(ch_data);
	
	// ...
	{
		log_msg = "Fnc__CH1_SET_RATIO() : Started ... \n";

		log_bff.Format("  * %s <- %s \n",
					   aCH__PARA_CH1_SET_RATIO->Get__CHANNEL_NAME(),
					   ch_data);
		log_msg += log_bff;

		xI_LOG_CTRL->WRITE__LOG(log_msg);		
	}

	// SW Ratio & Set Ratio
	{
		byte uch_data[11];
		memset(uch_data, 0, 11);

		float ch1_value = atof(ch_data);
		float ch2_value = 10.0 - ch1_value;

		int   sw_ratio  = 1;
		float set_ratio = 1.0;

		if((ch1_value >= 1.0) && (ch1_value <= 5))
		{
			sw_ratio = 2;
			set_ratio = ch2_value / ch1_value;
		}
		else if((ch2_value >= 1.0) && (ch2_value <= 5))
		{
			sw_ratio = 1;
			set_ratio = ch1_value / ch2_value;
		}

		//
		UNION_4_BYTE__FLOAT m_value;
		m_value.fDATA = set_ratio;

		uch_data[0] = 0x0ff & sw_ratio;

		uch_data[1] = 0x0ff & m_value.cBYTE[0];		// Low
		uch_data[2] = 0x0ff & m_value.cBYTE[1];		// 
		uch_data[3] = 0x0ff & m_value.cBYTE[2];		// 
		uch_data[4] = 0x0ff & m_value.cBYTE[3];		// High

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


// ...
int CObj__DELTA_II_IO
::Call__ALL_CLOSE(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm)
{

	return OBJ_AVAILABLE;
}

int CObj__DELTA_II_IO
::Call__CONTROL_CFG(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm)
{
	// ...
	{
		double cfg_ratio = aCH__CFG_CH2_SET_RATIO->Get__VALUE();
		aCH__PARA_CH2_SET_RATIO->Set__VALUE(cfg_ratio);
	}

	return Call__CONTROL(p_variable,p_alarm);
}
int CObj__DELTA_II_IO
::Call__CONTROL(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm)
{
	// ...
	{
		CString ch_data1;
		CString ch_data2;

		aCH__PARA_CH2_SET_RATIO->Get__DATA(ch_data2);

		ch_data1.Format("%1d", (10 - atoi(ch_data2)));
		aCH__PARA_CH1_SET_RATIO->Set__DATA(ch_data1);

		Fnc__CH1_SET_RATIO(p_variable,p_alarm);
	}

	// ...
	{
		CString log_msg;
		CString log_bff;

		log_msg = "CH1 & CH2 Info ... \n";

		log_bff.Format("  * %s <- %.0f \n",
						aCH__PARA_CH1_SET_RATIO->Get__CHANNEL_NAME(),
						aCH__PARA_CH1_SET_RATIO->Get__VALUE());
		log_msg += log_bff;

		log_bff.Format("  * %s <- %.0f \n",
						aCH__PARA_CH2_SET_RATIO->Get__CHANNEL_NAME(),
						aCH__PARA_CH2_SET_RATIO->Get__VALUE());
		log_msg += log_bff;

		xI_LOG_CTRL->WRITE__LOG(log_msg);		
	}
	return OBJ_AVAILABLE;
}

int CObj__DELTA_II_IO
::Call__GASLINE_PUMP(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm)
{

	return OBJ_AVAILABLE;
}
