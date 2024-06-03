#include "StdAfx.h"
#include "CObj__MADEE_IO.h"
#include "CObj__MADEE_IO__ALID.h"

#include "CCommon_Utility.h"


// ...
int CObj__MADEE_IO
::Mon__STATUS(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm)
{
	CString pre__set_master = "";
	CString pre__set_slave  = "";

	CCommon_Utility m_fnc;
	CString ch_data;
	
	xI_LOG_CTRL->WRITE__LOG("Mon__STATUS() : Start...");


	while(1)
	{
		p_variable->Wait__SINGLE_OBJECT(0.2);


		// CHECK : SET_RATIO ...
		{
			if(aCH__PARA_SET_RATIO_MASTER->Check__DATA(pre__set_master) < 0)
			{
				aCH__PARA_SET_RATIO_MASTER->Get__DATA(pre__set_master);

				Fnc__SET_RATIO(p_variable,p_alarm);
			}
		}

		// ...
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
				byte sts__ctrl_mode = 0x0ff & uch_data[1];
				
				//
				byte sts__ctrl_master = 0x0ff & uch_data[2];

				UNION_2_BYTE__UINT m_ratio__master;
				m_ratio__master.cBYTE[0] = 0x0ff & uch_data[3];
				m_ratio__master.cBYTE[1] = 0x0ff & uch_data[4];

				UNION_2_BYTE__UINT m_flow__master;
				m_flow__master.cBYTE[0] = 0x0ff & uch_data[5];
				m_flow__master.cBYTE[1] = 0x0ff & uch_data[6];

				//
				byte sts__ctrl_slave = 0x0ff & uch_data[7];

				UNION_2_BYTE__UINT m_ratio__slave;
				m_ratio__slave.cBYTE[0] = 0x0ff & uch_data[8];
				m_ratio__slave.cBYTE[1] = 0x0ff & uch_data[9];

				UNION_2_BYTE__UINT m_flow__slave;
				m_flow__slave.cBYTE[0] = 0x0ff & uch_data[10];
				m_flow__slave.cBYTE[1] = 0x0ff & uch_data[11];

				// EXCEPTION.STATE ...
				{
					ch_data.Format("%02X", sts__exception);
					sCH__MON_EXCEPTION_STATE->Set__DATA(ch_data);
				}	
				// CONTROL.MODE ...
				{
						 if(sts__ctrl_mode == 0x00)		ch_data = "Control";
					else if(sts__ctrl_mode == 0x01)		ch_data = "Close";
					else if(sts__ctrl_mode == 0x02)		ch_data = "Open";
					else if(sts__ctrl_mode == 0xAA)		ch_data = "Equal";
					else								ch_data = "Unknown";

					sCH__MON_CONTROL_MODE->Set__DATA(ch_data);
				}
				// MASTER ...
				{
					float cur__ratio_per = ((float) m_ratio__master.uiDATA / 0x8000) * 100.0;
					float cur__flow_per  = ((float) m_flow__master.uiDATA  / 0x6000) * 100.0;

					if(iActive__SIM_MODE > 0)
					{
						aCH__PARA_SET_RATIO_MASTER->Get__DATA(ch_data);
						sCH__MON_GET_RATIO_MASTER->Set__DATA(ch_data);
					}
					else
					{
						ch_data.Format("%.1f", cur__ratio_per);
						sCH__MON_GET_RATIO_MASTER->Set__DATA(ch_data);
					}

					ch_data.Format("%.1f", cur__flow_per);
					sCH__MON_GET_FLOW_MASTER->Set__DATA(ch_data);
				}
				// SLAVE ...
				{
					float cur__ratio_per = ((float) m_ratio__slave.uiDATA / 0x8000) * 100.0;
					float cur__flow_per  = ((float) m_flow__slave.uiDATA  / 0x6000) * 100.0;

					if(iActive__SIM_MODE > 0)
					{
						aCH__PARA_SET_RATIO_SLAVE->Get__DATA(ch_data);
						sCH__MON_GET_RATIO_SLAVE->Set__DATA(ch_data);
					}
					else
					{
						ch_data.Format("%.1f", cur__ratio_per);
						sCH__MON_GET_RATIO_SLAVE->Set__DATA(ch_data);
					}

					ch_data.Format("%.1f", cur__flow_per);
					sCH__MON_GET_FLOW_SLAVE->Set__DATA(ch_data);
				}
			}
		}

		// ...
	}

	return 1;
}
