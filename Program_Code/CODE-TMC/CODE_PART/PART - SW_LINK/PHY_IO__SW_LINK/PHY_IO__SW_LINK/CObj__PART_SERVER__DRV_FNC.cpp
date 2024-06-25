#include "stdafx.h"
#include "CObj__PART_SERVER.h"


//-------------------------------------------------------------------------------------
int CObj__PART_SERVER
::__Read__ANALOG(const CString& var_name, 
				 const CDS_IO__CHANNEL_INFO& io_info, 
				 double& read_data)
{

	return -1;
}

int CObj__PART_SERVER
::__Read__STRING(const CString& var_name, 
				 const CDS_IO__CHANNEL_INFO& io_info, 
				 CString& read_data)
{

	return -1;
}

int CObj__PART_SERVER
::__Read__DIGITAL(const CString& var_name,
				  const CDS_IO__CHANNEL_INFO& io_info, 
				  CString& read_data,
				  int& item_index)
{

	return -1;
}


// ...
int CObj__PART_SERVER
::__Write__ANALOG(const CString& var_name, 
				  const CDS_IO__CHANNEL_INFO& io_info, 
				  const double set_data)
{
	return -1;
}

int CObj__PART_SERVER
::__Write__DIGITAL(const CString& var_name,
				   const CDS_IO__CHANNEL_INFO& io_info, 
				   const CString& set_data,
				   const int item_index)
{

	return -1;
}

int CObj__PART_SERVER
::__Write__STRING(const CString& var_name, 
				  const CDS_IO__CHANNEL_INFO& io_info, 
				  const CString& set_data)
{

	return -1;
}


// ...
int CObj__PART_SERVER
::SEND__COMMAND(const CString& s_data)
{
	CString r_err;

	int r_flag = mX__Net_Server->DATA__SEND(s_data, 1, &r_err);

	// ...
	{
		CString log_msg;
		CString log_bff;

		log_msg.Format("Send >> [%s] \n", s_data); 

		if(r_flag < 0)
		{
			log_bff.Format("r_flag <- [%1d] \n", r_flag);
			log_msg += log_bff;

			log_bff.Format("r_err <- [%s] \n", r_err);
			log_msg += log_bff;
		}

		mX__Log_Ctrl->WRITE__LOG(log_msg);
	}
	return r_flag;
}
