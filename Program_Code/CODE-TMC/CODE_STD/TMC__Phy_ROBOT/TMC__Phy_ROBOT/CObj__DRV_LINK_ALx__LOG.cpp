#include "StdAfx.h"
#include "CObj__DRV_LINK_ALx.h"
#include "CObj__DRV_LINK_ALx__DEF.h"


//------------------------------------------------------------------------------------
void CObj__DRV_LINK_ALx
::Fnc__APP_LOG(const CString& log_msg)
{
	xAPP_LOG_CTRL->WRITE__LOG(log_msg);
}
