#include "StdAfx.h"
#include "CObj__DNET_STD.h"
#include "CObj__DNET_STD__DEF.h"

#include "CCommon_Utility.h"


//-------------------------------------------------------------------------	
int  CObj__DNET_STD
::Call__INIT(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	if(iActive__SIM_MODE > 0)
	{
		diCH__COMM_STS->Set__DATA(STR__ONLINE);
	}

	return 1;
}

//
int  CObj__DNET_STD
::Call__LINK_IO_SET_OFF(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	int i_limit = iSIZE__LINK_IO;
	int i;

	for(i=0; i<i_limit; i++)
	{
		dEXT_CH__DNET_LINK_IO[i]->Set__DATA(STR__OFF);
	}
	return 1;
}
int  CObj__DNET_STD
::Call__LINK_IO_SET_ON(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	int i_limit = iSIZE__LINK_IO;
	int i;

	for(i=0; i<i_limit; i++)
	{
		dEXT_CH__DNET_LINK_IO[i]->Set__DATA(STR__ON);
	}
	return 1;
}

// ...
int  CObj__DNET_STD
::Call__FLOAT_TO_HEXA(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	UNION_4_BYTE__FLOAT m_float;
	CString ch_data;

	sCH__FLOAT_TO_HEXA__VALUE->Get__DATA(ch_data);
	m_float.fDATA = atof(ch_data);

	// ...
	{
		CString str_hexa;
	
		int i_limit = 4;
		int i;

		ch_data = "";

		for(i=0; i<i_limit; i++)
		{
			str_hexa.Format("%02X ", 0x0ff & m_float.cBYTE[i]);
			ch_data += str_hexa;
		}
	}

	sCH__FLOAT_TO_HEXA__RESULT->Set__DATA(ch_data);
	return 1;
}
int  CObj__DNET_STD
::Call__HEXA_TO_FLOAT(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	CString ch_data;
	sCH__HEXA_TO_FLOAT__VALUE->Get__DATA(ch_data);

	ch_data = Get__HEXA_TO_FLOAT(ch_data);
	
	sCH__HEXA_TO_FLOAT__RESULT->Set__DATA(ch_data);
	return 1;
}
CString CObj__DNET_STD
::Get__HEXA_TO_FLOAT(const CString str_hexa)
{
	UNION_4_BYTE__FLOAT m_float;
	CCommon_Utility m_fnc;
	CStringArray l_data;

	m_fnc.Get__StringArrray_From_String(str_hexa, " ", l_data);

	int i_limit = l_data.GetSize();
	int i;

	if(i_limit > 4)		i_limit = 4;

	for(i=0; i<i_limit; i++)
	{
		m_float.cBYTE[i] = m_fnc.Get__Hexa_From_String(l_data[i]);
	}

	CString str_float;
	str_float.Format("%f", m_float.fDATA);

	return str_float;
}

// ...
int  CObj__DNET_STD
::Call__NODE_OUT_BYTE_UPLOAD(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	unsigned char data_bff[256];
	CString ch_data;

	int data_size = mCtrl__DNet_Node.Update__ALL_OUT_BYTE(data_bff, 255);
	int i;

	for(i=0; i<data_size; i++)
	{
		if(i < CFG__DNET_OUT_BYTE_LIMIT)
		{
			ch_data.Format("%02X", data_bff[i]);
			sCH__DNET_INFO__OUT_BYTE_DATA_X[i]->Set__DATA(ch_data);
		}
	}
	return 1;
}
int  CObj__DNET_STD
::Call__NODE_IN_BYTE_UPLOAD(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	/*
	unsigned char data_bff[256];	
	CString ch_data;

	int data_size = mCtrl__DNet_Node.Get__TOTAL_IN_BYTE();
	int i;

	if(data_size > 255)		data_size = 255;		

	for(i=0; i<data_size; i++)
	{
		if(i < CFG__DNET_OUT_BYTE_LIMIT)
		{
			sCH__DNET_INFO__IN_BYTE_DATA_X[i]->Get__DATA(ch_data);
			data_bff[i] = 0x0ff & atoi(ch_data);
		}
	}

	mCtrl__DNet_Node.Update__ALL_IN_BYTE(data_bff, data_size);
	*/
	return 1;
}

