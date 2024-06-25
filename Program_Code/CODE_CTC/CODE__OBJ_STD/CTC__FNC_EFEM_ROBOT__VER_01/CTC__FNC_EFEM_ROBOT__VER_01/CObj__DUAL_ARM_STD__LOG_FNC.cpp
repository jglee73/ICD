#include "StdAfx.h"
#include "CObj__DUAL_ARM_STD.h"


#define ROBOT_NAME			"ATM_RB1"


// ...
void CObj__DUAL_ARM_STD
::Fnc__LOG_INFO(const CString& fnc_id,
				const CString& log_id,
				const CString& para_info)
{
	CString log_msg;
	CString log_bff;

	log_msg = "Fnc__LOG_INFO() ...\n";
	log_bff.Format(" * fnc_id <- [%s] \n", fnc_id);
	log_msg += log_bff;
	log_bff.Format(" * log_id <- [%s] \n", log_id);
	log_msg += log_bff;
	log_bff.Format(" * para_info <- [%s] \n", para_info);
	log_msg += log_bff;

	xAPP_LOG_CTRL->WRITE__LOG(log_msg);
}

