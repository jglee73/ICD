#include "stdafx.h"
#include "CObj__PART_CLIENT.h"


//-------------------------------------------------------------------------------------
int CObj__PART_CLIENT
::__Read__ANALOG(const CString& var_name, 
				 const CDS_IO__CHANNEL_INFO& io_info, 
				 double& read_data)
{

	return -1;
}

int CObj__PART_CLIENT
::__Read__STRING(const CString& var_name, 
				 const CDS_IO__CHANNEL_INFO& io_info, 
				 CString& r_data)
{

	if(siCH__CONTROL_MODE->Check__VARIABLE_NAME(var_name) > 0)
	{
		CString s_data = _CMD__QUERY_CTRL_MODE;

		int r_flag = SEND__COMMAND(var_name,s_data, r_data);

		sCH__MON_CONTROL_MODE->Set__DATA(r_data);
		return r_flag;
	}

	if(siCH__CHM_PRESSURE->Check__VARIABLE_NAME(var_name) > 0)
	{
		CString s_data = _CMD__QUERY_PRESSURE;

		int r_flag = SEND__COMMAND(var_name,s_data, r_data);

		sCH__MON_CHM_PRESSURE->Set__DATA(r_data);
		return r_flag;
	}

	if(siCH__PIN_STATE->Check__VARIABLE_NAME(var_name) > 0)
	{
		CString s_data = _CMD__QUERY_PIN_STATE;

		int r_flag = SEND__COMMAND(var_name,s_data, r_data);
		
		sCH__MON_LIFT_PIN_STATE->Set__DATA(r_data);
		return r_flag;
	}

	if(siCH__SHUTTER_STATE->Check__VARIABLE_NAME(var_name) > 0)
	{
		CString s_data = _CMD__QUERY_SHUTTER_STATE;

		int r_flag = SEND__COMMAND(var_name,s_data, r_data);

		sCH__MON_SHUTTER_STATE->Set__DATA(r_data);
		return r_flag;
	}

	if(siCH__PROCESS_STATE->Check__VARIABLE_NAME(var_name) > 0)
	{
		CString s_data = _CMD__QUERY_PROCESS_STATE;

		int r_flag = SEND__COMMAND(var_name,s_data, r_data);
		
		sCH__MON_PROCESS_STATE->Set__DATA(r_data);
		return r_flag;
	}

	if(siCH__CHAMBER_STATE->Check__VARIABLE_NAME(var_name) > 0)
	{
		CString s_data = _CMD__QUERY_CHAMBER_STATE;

		int r_flag = SEND__COMMAND(var_name,s_data, r_data);

		sCH__MON_CHAMBER_STATE->Set__DATA(r_data);
		return r_flag;
	}

	return -1;
}

int CObj__PART_CLIENT
::__Read__DIGITAL(const CString& var_name,
				  const CDS_IO__CHANNEL_INFO& io_info, 
				  CString& read_data,
				  int& item_index)
{
	
	return -1;
}


// ...
int CObj__PART_CLIENT
::__Write__ANALOG(const CString& var_name, 
				  const CDS_IO__CHANNEL_INFO& io_info, 
				  const double set_data)
{

	return -1;
}

int CObj__PART_CLIENT
::__Write__DIGITAL(const CString& var_name,
				   const CDS_IO__CHANNEL_INFO& io_info, 
				   const CString& set_data,
				   const int item_index)
{

	if(doCH__Control_SET->Check__VARIABLE_NAME(var_name) > 0)
	{
		CString r_data;

		sCH__INFO_DRV_ACT_NAME->Set__DATA(set_data);

		SEND__COMMAND(var_name,set_data, r_data);

		sCH__INFO_DRV_ACT_RSP->Set__DATA(r_data);

		if(r_data.CompareNoCase(STR__OK) == 0)			return 1;
		return -1;
	}

	return -1;
}

int CObj__PART_CLIENT
::__Write__STRING(const CString& var_name, 
				  const CDS_IO__CHANNEL_INFO& io_info, 
				  const CString& set_data)
{

	return -1;
}


// ...
int CObj__PART_CLIENT
::SEND__COMMAND(const CString& var_name,
				const CString& cmd_data,
				CString& r_data)
{
	r_data = cmd_data;

	// ...
	CString log_msg;

	// ...
	{
		log_msg.Format("Send >> [%s]", r_data); 

		mX__Log_Ctrl->WRITE__LOG(log_msg);
	}

	// ...
	{
		CString r_msg;

		mX__Net_Client->CLEAR__MEM_BUFFER(&r_msg);
	}

	// ...
	int s_len = r_data.GetLength();

	int r_flag = mX__Net_Client->DATA__SEND(&r_data, s_len, 1);
	if(r_flag < 0)
	{
		bActive__COMM_ONLINE = false;

		r_data.Format("Error.%1d", r_flag);

		// ...
		{
			log_msg.Format("Recv.Error (%1d)", r_flag); 

			mX__Log_Ctrl->WRITE__LOG(log_msg);
		}
	}
	else
	{
		bActive__COMM_ONLINE = true;

		// ...
		{
			log_msg.Format("Recv << [%s] \n", r_data); 

			mX__Log_Ctrl->WRITE__LOG(log_msg);
		}
	}

	return r_flag;
}
