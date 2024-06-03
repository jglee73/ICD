#include "StdAfx.h"
#include "CObj__DELTA_II_IO.h"
#include "CObj__DELTA_II_IO__ALID.h"

#include "CCommon_Utility.h"


// ...
int CObj__DELTA_II_IO
::Mon__STATUS(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm)
{
	CString pre__ch1_set = "";
	CString pre__ch2_set = "";

	CCommon_Utility m_fnc;
	CString ch_data;

	xI_LOG_CTRL->WRITE__LOG("Mon__STATUS() : Start...");


	while(1)
	{
		p_variable->Wait__SINGLE_OBJECT(0.2);


		// ...
		{
			if(aCH__PARA_CH2_SET_RATIO->Check__DATA(pre__ch2_set) < 0)
			{
				aCH__PARA_CH2_SET_RATIO->Get__DATA(pre__ch2_set);

				ch_data.Format("%1d", (10 - atoi(pre__ch2_set)));
				aCH__PARA_CH1_SET_RATIO->Set__DATA(ch_data);
			}

			if(aCH__PARA_CH1_SET_RATIO->Check__DATA(pre__ch1_set) < 0)
			{
				aCH__PARA_CH1_SET_RATIO->Get__DATA(pre__ch1_set);

				Fnc__CH1_SET_RATIO(p_variable,p_alarm);
			}
		}

		if(iActive__SIM_MODE)
		{
			aCH__PARA_CH1_SET_RATIO->Get__DATA(ch_data);
			sCH__MON_CH1_GET_RATIO->Set__DATA(ch_data);

			aCH__PARA_CH2_SET_RATIO->Get__DATA(ch_data);
			sCH__MON_CH2_GET_RATIO->Set__DATA(ch_data);
		}
		else
		{
			byte uch_data[21];
			memset(uch_data, 0, 21);

			sEXT_CH__IO_IN_BYTE_HEXA->Get__DATA(ch_data);

			// String_Hexa -> Int_Hexa
			{
				CStringArray l_data;
				m_fnc.Get__StringArrray_From_String(ch_data," ", l_data);

				int i_limit = l_data.GetSize();
				if(i_limit > 20)		i_limit = 20;

				for(int i=0; i<i_limit; i++)
				{
					uch_data[i] = 0x0ff & m_fnc.Get__Hexa_From_String(l_data[i]);
				}
			}

			// ...
			{
				byte sts__exception = 0x0ff & uch_data[0];
				byte sts__sw_ratio  = 0x0ff & uch_data[1];
				
				UNION_4_BYTE__FLOAT m_readback;
				m_readback.cBYTE[0] = 0x0ff & uch_data[2];
				m_readback.cBYTE[1] = 0x0ff & uch_data[3];
				m_readback.cBYTE[2] = 0x0ff & uch_data[4];
				m_readback.cBYTE[3] = 0x0ff & uch_data[5];

				//
				float flow_ratio = m_readback.fDATA;

				float ch1_ratio = 0.0;
				float ch2_ratio = 0.0;

				if(sts__sw_ratio == 1)
				{
					ch1_ratio = (flow_ratio * 10.0) / (flow_ratio + 1.0);
					ch2_ratio = 10.0 - ch1_ratio;
				}
				else
				{
					ch2_ratio = (flow_ratio * 10.0) / (flow_ratio + 1.0);
					ch1_ratio = 10.0 - ch2_ratio;
				}

				ch_data.Format("%02X", sts__exception);
				sCH__MON_EXCEPTION_STATE->Set__DATA(ch_data);

				ch_data.Format("%1d", sts__sw_ratio);
				sCH__MON_SW_RATIO->Set__DATA(ch_data);

				ch_data.Format("%.1f", ch1_ratio);
				sCH__MON_CH1_GET_RATIO->Set__DATA(ch_data);

				ch_data.Format("%.1f", ch2_ratio);
				sCH__MON_CH2_GET_RATIO->Set__DATA(ch_data);
			}
		}
	}

	return 1;
}
