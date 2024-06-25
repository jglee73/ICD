#include "StdAfx.h"
#include "CObj__ATM_RelianceATR.h"
#include "CObj__ATM_RelianceATR__DEF.h"

#include "math.h"
#include "CCommon_DEF.h"


//------------------------------------------------------------------------------------
void CObj__ATM_RelianceATR
::Fnc__APP_LOG(const CString& log_msg)
{
	if(iFlag__APP_LOG < 1)		return;

	xAPP_LOG_CTRL->WRITE__LOG(log_msg);
}

void CObj__ATM_RelianceATR
::Fnc__DRV_LOG(const CString& log_msg)
{
	if(iFlag__DRV_LOG < 1)		return;

	xDRV_LOG_CTRL->WRITE__LOG(log_msg);
}

void CObj__ATM_RelianceATR
::Fnc__DRV_LOG_FORMAT(const char *fmt, ...)
{
	CString str_msg;
	va_list args;

	va_start(args, fmt);
	str_msg.FormatV(fmt, args);
	va_end(args);

	xDRV_LOG_CTRL->WRITE__LOG(str_msg);
}

void CObj__ATM_RelianceATR
::Fnc__ACT_MSG(const CString& act_msg)
{
	sCH__ACT_MSG->Set__DATA(act_msg);
}
