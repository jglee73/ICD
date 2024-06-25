#include "StdAfx.h"
#include "CObj__DRV_TEST.h"

#include "CCommon_Utility.h"


//------------------------------------------------------------------------------------
#define _AI_TYPE__GAUGE						1
#define _AI_TYPE__FFFF						2
#define _AI_TYPE__1F40						3
#define _AI_TYPE__FLOAT						4
#define _AI_TYPE__0FFF						5


int CObj__DRV_TEST
::__Read__ANALOG(const CString& var_name, 
				 const CDS_IO__CHANNEL_INFO& io_info, 
				 double& read_data)
{
	int node_id = io_info.iMD_NO;
	if(node_id == 0)		return 1;
	if(node_id <  0)		return -1;

	int md_id = mCtrl__DNet_Node.Get_MacID_Of_Node_ID(node_id);
	if(md_id < 0)
	{
		// Error

		return -1;
	}

	/*
	// ...
	CString fnc_type = "__Read__ANALOG";
	Write__DRV_LOG(fnc_type, var_name, io_info);
	*/

	// ...
	{
		unsigned char uch_data[11];
		memset(uch_data, 0, 11);

		int ch_id = io_info.iCH_NO;

		int byte_offset = atoi(io_info.sCOMMAND2);
		int byte_size   = atoi(io_info.sCOMMAND3);

		int ai_type = -1;

		if(io_info.sCOMMAND3.CompareNoCase("GAUGE") == 0)
		{
			ai_type   = _AI_TYPE__GAUGE;
			byte_size = 2;
		}
		else if(io_info.sCOMMAND3.CompareNoCase("0FFF") == 0)
		{
			ai_type   = _AI_TYPE__0FFF;
			byte_size = 2;
		}
		else if(io_info.sCOMMAND3.CompareNoCase("FFFF") == 0)
		{
			ai_type   = _AI_TYPE__FFFF;
			byte_size = 2;
		}
		else if(io_info.sCOMMAND3.CompareNoCase("1F40") == 0)
		{
			ai_type   = _AI_TYPE__1F40;
			byte_size = 2;
		}		
		else if(io_info.sCOMMAND3.CompareNoCase("FLOAT") == 0)
		{
			ai_type   = _AI_TYPE__FLOAT;
			byte_size = 4;
		}

		if(byte_size > 10)
		{
			byte_size = 10;
		}

		if(mCtrl__DNet_Node.Get__IN_BYTE__IO_BYTE(md_id,ch_id,byte_offset,byte_size, uch_data,10) > 0)
		{
			if(ai_type == _AI_TYPE__GAUGE)
			{
				UNION_2_BYTE__UINT m_data;

				m_data.cBYTE[0] = 0xff & uch_data[0];
				m_data.cBYTE[1] = 0xff & uch_data[1];

				/*
				double max_value = io_info.dMAX_VOLT;
				if(max_value < 1.0)			max_value = 1.0;
				*/
				double max_value = 16383.0;

				read_data = (double) (10.0 * m_data.uiDATA) / max_value;

				/*
				// ...
				{
					CString ch_data;

					ch_data.Format("%04X", m_data.uiDATA);
					sCH__TEST_GAUGE_HEXA->Set__DATA(ch_data);

					ch_data.Format("%.1f", read_data);
					sCH__TEST_GAUGE_VALUE->Set__DATA(ch_data);
				}
				*/
			}
			else if(ai_type == _AI_TYPE__FFFF)
			{
				UNION_2_BYTE__UINT m_data;

				m_data.cBYTE[0] = 0xff & uch_data[0];
				m_data.cBYTE[1] = 0xff & uch_data[1];

				if(m_data.uiDATA < 0x8000)
				{
					read_data = m_data.uiDATA / 10.0;
				}
				else
				{
					read_data = (0x0ffff - m_data.uiDATA) / 10.0;
				}

				/*
				// ... 
				{
					CString ch_data;

					ch_data.Format("%o4X", m_data.uiDATA);
					sCH__TEST_FFFF_HEXA->Set__DATA(ch_data);

					ch_data.Format("%.1f", read_data);
					sCH__TEST_FFFF_VALUE->Set__DATA(ch_data);
				}
				*/
			}
			else if(ai_type == _AI_TYPE__1F40)
			{
				UNION_2_BYTE__UINT m_data;

				m_data.cBYTE[0] = 0xff & uch_data[0];
				m_data.cBYTE[1] = 0xff & uch_data[1];

				/*
				double max_value = io_info.dMAX_VOLT;
				if(max_value < 1.0)			max_value = 1.0;
				*/
				double max_value = (double) 0x1f40;

				read_data = (double) (10.0 * m_data.uiDATA) / max_value;

				/*
				// ...
				{
					CString ch_data;

					ch_data.Format("%04X", m_data.uiDATA);
					sCH__TEST_GAUGE_HEXA->Set__DATA(ch_data);

					ch_data.Format("%.1f", read_data);
					sCH__TEST_GAUGE_VALUE->Set__DATA(ch_data);
				}
				*/
			}
			else if(ai_type == _AI_TYPE__0FFF)
			{
				UNION_2_BYTE__UINT m_data;

				m_data.cBYTE[0] = 0xff & uch_data[0];
				m_data.cBYTE[1] = 0xff & uch_data[1];

				double max_value = (double) 0x0FFF;

				read_data = (double) (10.0 * m_data.uiDATA) / max_value;
			}
			else if(ai_type == _AI_TYPE__FLOAT)
			{
				UNION_4_BYTE__FLOAT m_data;

				m_data.cBYTE[0] = 0xff & uch_data[0];
				m_data.cBYTE[1] = 0xff & uch_data[1];
				m_data.cBYTE[2] = 0xff & uch_data[2];
				m_data.cBYTE[3] = 0xff & uch_data[3];

				read_data = m_data.fDATA;
			}
			else
			{
				if(byte_size == 2)
				{
					UNION_2_BYTE__UINT m_data;

					m_data.cBYTE[0] = 0xff & uch_data[0];
					m_data.cBYTE[1] = 0xff & uch_data[1];

					read_data = (double) m_data.uiDATA;
				}
			}
			return 1;
		}
	}
	return -1;
}

int CObj__DRV_TEST
::__Read__DIGITAL(const CString& var_name, 
				  const CDS_IO__CHANNEL_INFO& io_info, 
				  CString& read_data,int& item_index)
{
	int node_id = io_info.iMD_NO;
	if(node_id == 0)		return 1;
	if(node_id <  0)		return -1;

	int md_id = mCtrl__DNet_Node.Get_MacID_Of_Node_ID(node_id);
	if(md_id < 0)
	{
		// Error

		return -1;
	}

	/*
	// ...
	CString fnc_type = "__Read__DIGITAL";
	Write__DRV_LOG(fnc_type, var_name, io_info);
	*/

	// ...
	{
		int ch_id      = io_info.iCH_NO;
		int bit_offset = atoi(io_info.sCOMMAND2);

		ch_id += bit_offset;

		if(mCtrl__DNet_Node.Get__IN_BYTE__IO_BIT(md_id,ch_id, item_index) > 0)
		{
			return 1;
		}
	}
	return -1;
}

int CObj__DRV_TEST
::__Read__STRING(const CString& var_name, const CDS_IO__CHANNEL_INFO& io_info, CString& read_data)
{
	int node_id = io_info.iMD_NO;
	if(node_id == 0)		return 1;
	if(node_id <  0)		return -1;

	int md_id = mCtrl__DNet_Node.Get_MacID_Of_Node_ID(node_id);
	if(md_id < 0)
	{
		// Error

		return -1;
	}

	/*
	// ...
	CString fnc_type = "__Read__STRING";
	Write__DRV_LOG(fnc_type, var_name, io_info);
	*/

	// ...
	{
		unsigned char uch_data[101];
		memset(uch_data, 0, 101);

		int ch_id = io_info.iCH_NO;

		int byte_offset = atoi(io_info.sCOMMAND2);
		int byte_size   = atoi(io_info.sCOMMAND3);

		if(mCtrl__DNet_Node.Get__IN_BYTE__IO_BYTE(md_id,ch_id,byte_offset,byte_size, uch_data,100) > 0)
		{
			CString data_type = io_info.sCOMMAND1;

			if(data_type.CompareNoCase("ASCII") == 0)
			{
				read_data.Format("%s", uch_data);
			}
			else if(data_type.CompareNoCase("FLOAT") == 0)
			{
				UNION_4_BYTE__FLOAT m_float;
				
				for(int i=0; i<4; i++)
				{
					m_float.cBYTE[i] = 0x0ff & uch_data[i];
				}

				read_data.Format("%f", m_float.fDATA);
			}
			else
			{
				CString str_hexa;
				read_data = "";
			
				int i_len = byte_size;
				if(i_len > 100)		i_len = 100;

				for(int i=0; i<i_len; i++)
				{
					str_hexa.Format("%02X ", 0x0ff & uch_data[i]);
					read_data += str_hexa;
				}
			}
			return 1;
		}
	}
	return -1;
}


//------------------------------------------------------------------------------------
int CObj__DRV_TEST
::__Write__ANALOG(const CString& var_name, const CDS_IO__CHANNEL_INFO& io_info, const double set_data)
{
	int node_id = io_info.iMD_NO;
	if(node_id == 0)		return 1;
	if(node_id <  0)		return -1;

	int md_id = mCtrl__DNet_Node.Get_MacID_Of_Node_ID(node_id);
	if(md_id < 0)
	{
		// Error

		return -1;
	}

	// ...
	CString fnc_type = "__Write__ANALOG";
	Write__DRV_LOG(fnc_type, var_name, io_info, set_data);

	// ...
	{
		unsigned char uch_data[11];
		memset(uch_data, 0, 11);

		int ch_id       = io_info.iCH_NO;
		int byte_offset = atoi(io_info.sCOMMAND2);
		int byte_size   = atoi(io_info.sCOMMAND3);

		if(byte_size > 10)		byte_size = 10;
		
		if(byte_size == 2)
		{
			UNION_2_BYTE__UINT m_data;

			m_data.uiDATA = (int) set_data;	

			uch_data[0] = m_data.cBYTE[0];
			uch_data[1] = m_data.cBYTE[1];
		}

		if(mCtrl__DNet_Node.Set__OUT_BYTE__IO_BYTE(md_id,ch_id,byte_offset,byte_size, uch_data) > 0)
		{
			return 1;
		}
	}
	return 1;
}

int CObj__DRV_TEST
::__Write__DIGITAL(const CString& var_name, const CDS_IO__CHANNEL_INFO& io_info, const CString& set_data,const int item_index)
{
	int node_id = io_info.iMD_NO;
	if(node_id == 0)		return 1;
	if(node_id <  0)		return -1;

	int md_id = mCtrl__DNet_Node.Get_MacID_Of_Node_ID(node_id);
	if(md_id < 0)
	{
		// Error

		return -1;
	}

	// ...
	CString fnc_type = "__Write__DIGITAL";
	Write__DRV_LOG(fnc_type, var_name, io_info, set_data,item_index);

	// ...
	int ch_id      = io_info.iCH_NO;
	int bit_offset = atoi(io_info.sCOMMAND2);
	int bit_size   = atoi(io_info.sCOMMAND3);
	int data_mask  = 0x01;

	if(bit_size < 1)		bit_size = 1;

	for(int i=0; i<bit_size; i++)
	{
		ch_id += bit_offset;
		ch_id += i;

		int data_index = 0x0ff & (item_index & data_mask);

		mCtrl__DNet_Node.Set__OUT_BYTE__IO_BIT(md_id,ch_id, data_index);

		data_mask <<= 1;
	}
	return 1;
}

int CObj__DRV_TEST
::__Write__STRING(const CString& var_name, const CDS_IO__CHANNEL_INFO& io_info, const CString& set_data)
{
	int node_id = io_info.iMD_NO;
	if(node_id == 0)		return 1;
	if(node_id <  0)		return -1;

	int md_id = mCtrl__DNet_Node.Get_MacID_Of_Node_ID(node_id);
	if(md_id < 0)
	{
		// Error

		return -1;
	}

	// ...
	CString fnc_type = "__Write__STRING";
	Write__DRV_LOG(fnc_type, var_name, io_info);

	// ...
	{
		unsigned char uch_data[101];
		memset(uch_data, 0, 101);

		int ch_id = io_info.iCH_NO;

		int byte_offset = atoi(io_info.sCOMMAND2);
		int byte_size   = atoi(io_info.sCOMMAND3);

		CString data_type = io_info.sCOMMAND1;

		if(data_type.CompareNoCase("ASCII") == 0)
		{
			int i_len = set_data.GetLength();
			if(i_len > 100)		i_len = 100;

			for(int i=0; i<i_len; i++)
			{
				uch_data[i] = (char) set_data[i];
			}
		}
		else
		{
			CString log_msg;
			CString log_bff;
			
			log_msg  = set_data;
			log_msg += "\n";

			// ...
			CStringArray l_data;
			CString str_hexa;

			CCommon_Utility m_fnc;
			m_fnc.Get__StringArrray_From_String(set_data," ", l_data);

			int i_len = l_data.GetSize();
			if(i_len > 100)		i_len = 100;

			for(int i=0; i<i_len; i++)
			{
				str_hexa = l_data[i];
				uch_data[i] = 0x0ff & m_fnc.Get__Hexa_From_String(str_hexa);

				log_bff.Format(" * %1d) %s [%02X] \n", i+1,str_hexa,0x0ff&uch_data[i]);
				log_msg += log_bff;
			}			

			Write__DRV_LOG(log_msg);
		}

		if(mCtrl__DNet_Node.Set__OUT_BYTE__IO_BYTE(md_id,ch_id,byte_offset,byte_size, uch_data) > 0)
		{
			return 1;
		}
	}
	return 1;
}
