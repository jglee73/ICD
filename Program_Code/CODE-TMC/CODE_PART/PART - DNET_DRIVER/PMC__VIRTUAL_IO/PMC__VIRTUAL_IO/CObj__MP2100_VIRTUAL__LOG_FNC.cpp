#include "StdAfx.h"
#include "CObj__MP2100_VIRTUAL.h"


//------------------------------------------------------------------------------------
void CObj__MP2100_VIRTUAL
::Write__APP_LOG(const CString& log_msg)
{
	xI__APP_LOG_CTRL->WRITE__LOG(log_msg);
}

void CObj__MP2100_VIRTUAL
::Write__DRV_LOG(const CString& log_msg)
{

	xI__DRV_LOG_CTRL->WRITE__LOG(log_msg);
}

