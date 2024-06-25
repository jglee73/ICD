#include "StdAfx.h"
#include "CObj__DRV_LINK_RBx.h"
#include "CObj__DRV_LINK_RBx__DEF.h"


//------------------------------------------------------------------------------------
void CObj__DRV_LINK_RBx
::Fnc__APP_LOG(const CString& log_msg)
{
	xAPP_LOG_CTRL->WRITE__LOG(log_msg);
}
