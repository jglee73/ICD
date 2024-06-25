#include "StdAfx.h"
#include "CObj__DUAL_ARM_STD.h"

#include "Macro_Function.h"


// ...
int  CObj__DUAL_ARM_STD::
SCH__PMx_JOB_START_READY(const int check_pm_index, const CString& sch_name, const CString& log_id)
{
	if(dEXT_CH__CFG_JOB_START_READY_CTRL_MODE->Check__DATA("JOB.CHANGE") < 0)
	{
		return 11;
	}

	// ...
	IDS__SCH_MATERIAL_INFO ds_info;
	if(xSCH_MATERIAL_CTRL->Get__MATERIAL_INFO(sch_name, ds_info) < 0)
	{
		return 12;
	}

	// ...
	CString fnc_id;
	fnc_id.Format("SCH__PMx_JOB_START_READY() : check_pm_index (%1d) ... \n", check_pm_index);

	int lp_ptn = ds_info.iSRC__PTN;
	int lp_stn = ds_info.iSRC__STN;

	// ...
	CString log_msg;
	CString log_bff;

	log_msg  = "\n";
	log_bff.Format("check_pm_index <- [%1d] \n", check_pm_index);
	log_msg += log_bff;
	log_bff.Format("sch_name <- [%s] \n", sch_name);
	log_msg += log_bff;
	log_bff.Format("wafer <- [P%1d:%02d] \n", lp_ptn,lp_stn);
	log_msg += log_bff;

	// ...
	CString req_portid;
	CString req_jobid;

	req_portid.Format("%1d", lp_ptn);
	xSCH_MATERIAL_CTRL->Get__LPx_CID(lp_ptn, req_jobid);

	int p_index = check_pm_index;

	if((p_index >= 0) && (p_index < iPMx_SIZE))
	{
		if(dEXT_CH__CFG_JOB_START_READY_PMx_USE[p_index]->Check__DATA(STR__YES) < 0)
		{
			return 1;
		}

		if(Check__PMx_JOB_START_READY(p_index, req_portid,req_jobid) > 0)
		{
			return 2;
		}

		if(PMx__Check_Empty__SlotStatus(p_index) < 0)
		{
			return -11;
		}

		if(PMx__Is_Available(p_index) > 0)		
		{
			Run__PMx_JOB_START_READY(p_index, req_portid,req_jobid);

			// ...
			{
				log_bff.Format(" * PM%1d_JOB_START_READY <- (%s) (%s) \n", p_index+1,req_portid,req_jobid);
				log_msg += log_bff;

				Fnc__LOG_INFO(fnc_id,log_id,log_msg);
			}
		}
	}
	return -1;
}
int  CObj__DUAL_ARM_STD::
SCH__PMx_JOB_START_READY(const CString& sch_name, const CString& log_id)
{
	if(dEXT_CH__CFG_JOB_START_READY_CTRL_MODE->Check__DATA("JOB.CHANGE") < 0)
	{
		return 11;
	}

	// ...
	IDS__SCH_MATERIAL_INFO ds_info;
	if(xSCH_MATERIAL_CTRL->Get__MATERIAL_INFO(sch_name, ds_info) < 0)
	{
		return 12;
	}

	// ...
	CString fnc_id = "SCH__PMx_JOB_START_READY() ...";

	int lp_ptn = ds_info.iSRC__PTN;
	int lp_stn = ds_info.iSRC__STN;

	// ...
	CString log_msg;
	CString log_bff;

	// ...
	{
		log_msg  = "\n";

		log_bff.Format("sch_name <- [%s] \n", sch_name);
		log_msg += log_bff;
	
		log_bff.Format("  * wafer <- [P%1d:%02d] \n", lp_ptn,lp_stn);
		log_msg += log_bff;
	}

	// ...
	int active_pm = -1;

	CString req_portid;
	CString req_jobid;

	req_portid.Format("%1d", lp_ptn);
	xSCH_MATERIAL_CTRL->Get__LPx_CID(lp_ptn, req_jobid);

	// ...
	CStringArray l__pmc_name;
	CStringArray l__rcp_name;

	xSCH_MATERIAL_CTRL->Get__CUR_PROCESS_INFO(sch_name, l__pmc_name,l__rcp_name);

	int i_limit = l__pmc_name.GetSize();
	for(int i=0; i<i_limit; i++)
	{
		CString str__pmc_name = l__pmc_name[i];

		int p_index = Macro__Get_PMC_INDEX(str__pmc_name);

		if(p_index <  0)				continue;
		if(p_index >= iPMx_SIZE)		continue;

		if(dEXT_CH__CFG_JOB_START_READY_PMx_USE[p_index]->Check__DATA(STR__YES) < 0)
		{
			continue;
		}

		if(Check__PMx_JOB_START_READY(p_index, req_portid,req_jobid) > 0)
		{
			continue;
		}

		if(PMx__Check_Empty__SlotStatus(p_index) < 0)			continue;
		if(PMx__Is_Available(p_index) < 0)						continue;

		if(xSCH_MATERIAL_CTRL->Check__PMC_TO_PROCESS_OF_PRE_DB(sch_name, str__pmc_name) > 0)
		{
			continue;
		}

		if(Run__PMx_JOB_START_READY(p_index, req_portid,req_jobid) < 0)
		{
			continue;
		}

		active_pm = 1;

		// ...
		{
			log_bff.Format("  * PM%1d_JOB_START_READY <- (%s) (%s) \n", p_index+1,req_portid,req_jobid);
			log_msg += log_bff;
		}
	}

	if(active_pm > 0)
	{
		Fnc__LOG_INFO(fnc_id, log_id, log_msg);
		return -1;
	}
	return 1;
}
int  CObj__DUAL_ARM_STD::
Run__PMx_JOB_START_READY(const int pm_index, const CString& port_id,const CString& job_id)
{
	sEXT_CH__PMx_REQ_INFO_JOB_START_READY_PORTID[pm_index]->Set__DATA(port_id);
	sEXT_CH__PMx_REQ_INFO_JOB_START_READY_JOBID[pm_index]->Set__DATA(job_id);

	Run__PMx_OBJ(pm_index, "JOB_START_READY");
	return 1;
}
int  CObj__DUAL_ARM_STD::
Check__PMx_JOB_START_READY(const int pm_index, const CString& port_id,const CString& job_id)
{
	if(sEXT_CH__PMx_PRE_INFO_JOB_START_READY_PORTID[pm_index]->Check__DATA(port_id) < 0)
	{
		return -1;
	}
	if(sEXT_CH__PMx_PRE_INFO_JOB_START_READY_JOBID[pm_index]->Check__DATA(job_id) < 0)
	{
		return -1;
	}

	return 1;
}

