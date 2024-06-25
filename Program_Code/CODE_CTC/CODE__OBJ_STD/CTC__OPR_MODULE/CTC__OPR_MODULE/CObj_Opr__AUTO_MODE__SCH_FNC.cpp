#include "StdAfx.h"
#include "CObj_Opr__AUTO_MODE.h"

#include "Macro_Function.h"


// ...
MODULEx__IS_AVAILABLE_EX(CObj_Opr__AUTO_MODE,PMx, xEXT_CH__PMx__OBJ_STATUS, xEXT_CH__PMx__OBJ_CTRL, xEXT_CH__CFG__PMx_USE);


int  CObj_Opr__AUTO_MODE::SCH__PMx_JOB_START_READY(const int lp_id)
{
	CString req_portid;
	CString req_jobid;

	req_portid.Format("%1d", lp_id);
	req_jobid = "";

	if(lp_id < 0)
	{
		int i_limit = iPMx_UNIT_SIZE;
		int i;

		for(i=0; i<i_limit; i++)
		{
			if(dEXT_CH__CFG_JOB_START_READY_PMx_USE[i]->Check__DATA(STR__YES) < 0)
			{
				continue;
			}

			if(PMx__Is_Available(i) < 0)		
			{
				continue;
			}
			
			if(dEXT_CH__PMx_SLOT_STATUS[i]->Check__DATA(STR__NONE) < 0)
			{
				continue;
			}

			SCH_RUN__PMx_JOB_START_READY(i, req_portid,req_jobid);
		}
	}
	else
	{
		CStringArray l_pmx;

		xSCH_MATERIAL_CTRL->Get__LPx_CID(lp_id, req_jobid);
		xSCH_MATERIAL_CTRL->Get__ALL_PRC_MODULE_In_LPx(lp_id, l_pmx);

		int i_limit = l_pmx.GetSize();
		int i;

		for(i=0; i<i_limit; i++)
		{
			int p_index = Macro__Get_PM_INDEX(l_pmx[i]);

			if((p_index < 0)
			|| (p_index >= iPMx_UNIT_SIZE))
			{
				continue;
			}

			if(dEXT_CH__CFG_JOB_START_READY_PMx_USE[p_index]->Check__DATA(STR__YES) < 0)
			{
				continue;
			}

			if(PMx__Is_Available(p_index) < 0)		
			{
				continue;
			}

			if(dEXT_CH__PMx_SLOT_STATUS[p_index]->Check__DATA(STR__NONE) < 0)
			{
				continue;
			}

			SCH_RUN__PMx_JOB_START_READY(p_index, req_portid,req_jobid);
		}
	}
	return 1;
}
int  CObj_Opr__AUTO_MODE::SCH_RUN__PMx_JOB_START_READY(const int pm_index, const CString& port_id,const CString& job_id)
{
	if(Check__PMx_JOB_START_READY(pm_index, port_id,job_id) > 0)
	{
		return -1;
	}

	sEXT_CH__PMx_REQ_INFO_JOB_START_READY_PORTID[pm_index]->Set__DATA(port_id);
	sEXT_CH__PMx_REQ_INFO_JOB_START_READY_JOBID[pm_index]->Set__DATA(job_id);

	pPMx__OBJ_CTRL[pm_index]->Run__OBJECT("JOB_START_READY");
	return 1;
}
int  CObj_Opr__AUTO_MODE::Check__PMx_JOB_START_READY(const int pm_index, const CString& port_id,const CString& job_id)
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
int  CObj_Opr__AUTO_MODE::Clear__PMx_JOB_START_READY()
{
	printf("* CObj_Opr__AUTO_MODE::Clear__PMx_JOB_START_READY() ... \n");

	int i_limit = iPMx_UNIT_SIZE;
	int i;

	for(i=0; i<i_limit; i++)
	{
		sEXT_CH__PMx_PRE_INFO_JOB_START_READY_PORTID[i]->Set__DATA("");
		sEXT_CH__PMx_PRE_INFO_JOB_START_READY_JOBID[i]->Set__DATA("");
	}
	return 1;
}

int  CObj_Opr__AUTO_MODE::SCH__PMx_JOB_END_READY()
{
	int i_limit = iPMx_UNIT_SIZE;
	int i;

	for(i=0; i<i_limit; i++)
	{
		if(dEXT_CH__CFG_JOB_END_READY_PMx_USE[i]->Check__DATA(STR__YES) < 0)
		{
			continue;
		}

		if(PMx__Is_Available(i) < 0)		
		{
			continue;
		}

		pPMx__OBJ_CTRL[i]->Run__OBJECT("JOB_END_READY");
	}

	return 1;
}
