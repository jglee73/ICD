#include "StdAfx.h"
#include "CObj__DUAL_ARM_STD.h"


#define ROBOT_NAME			"VAC_RB1"


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
void CObj__DUAL_ARM_STD::
Fnc__LOG_PROC(const CString& fnc_id,
			  const CString& log_id,
			  const CString& sch_name,
			  const bool active_next)
{
	CString log_msg;
	CString log_bff;

	log_msg = "Fnc__LOG_PROC() ...\n";
	log_bff.Format(" * fnc_id <- [%s] \n", fnc_id);
	log_msg += log_bff;
	log_bff.Format(" * log_id <- [%s] \n", log_id);
	log_msg += log_bff;
	log_msg += " * Current Process Info ... \n";

	log_bff.Format("sch_name <- [%s] \n", sch_name);
	log_msg += log_bff;

	// ...
	CStringArray l_pm_name;
	CStringArray l_pm_rcp;

	if(active_next)		xSCH_MATERIAL_CTRL->Get__NEXT_PROCESS_INFO(sch_name, l_pm_name,l_pm_rcp);
	else				xSCH_MATERIAL_CTRL->Get__CUR_PROCESS_INFO(sch_name, l_pm_name,l_pm_rcp);

	// ...
	{
		int i_limit = l_pm_name.GetSize();
		int i;

		for(i=0; i<i_limit; i++)
		{
			log_bff.Format(" ** %1d) [%s][%s] \n", i+1,l_pm_name[i],l_pm_rcp[i]);
			log_msg += log_bff;
		}
	}

	xAPP_LOG_CTRL->WRITE__LOG(log_msg);
}

