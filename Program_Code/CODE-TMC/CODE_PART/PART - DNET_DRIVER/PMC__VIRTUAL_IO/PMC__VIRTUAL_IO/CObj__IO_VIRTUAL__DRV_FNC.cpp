#include "StdAfx.h"
#include "CObj__IO_VIRTUAL.h"


//------------------------------------------------------------------------------------
int CObj__IO_VIRTUAL
::__Read__ANALOG(const CString& var_name, 
				 const CDS_IO__CHANNEL_INFO& io_info, 
				 double& read_data)
{

	return -1;
}

int CObj__IO_VIRTUAL
::__Read__DIGITAL(const CString& var_name, 
				  const CDS_IO__CHANNEL_INFO& io_info, 
				  CString& read_data,int& item_index)
{

	return -1;
}

int CObj__IO_VIRTUAL
::__Read__STRING(const CString& var_name, const CDS_IO__CHANNEL_INFO& io_info, CString& read_data)
{

	return -1;
}


//------------------------------------------------------------------------------------
int CObj__IO_VIRTUAL
::__Write__ANALOG(const CString& var_name, const CDS_IO__CHANNEL_INFO& io_info, const double set_data)
{

	return 1;
}

int CObj__IO_VIRTUAL
::__Write__DIGITAL(const CString& var_name, const CDS_IO__CHANNEL_INFO& io_info, const CString& set_data,const int item_index)
{

	return 1;
}

int CObj__IO_VIRTUAL
::__Write__STRING(const CString& var_name, const CDS_IO__CHANNEL_INFO& io_info, const CString& set_data)
{

	return 1;
}
