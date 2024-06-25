#include "StdAfx.h"
#include "CMacro_FA.h"


CMacro_FA mFA_Link;


// ...
CMacro_FA::CMacro_FA()
{
	InitializeCriticalSection(&mCS_LOCK);

	iInit_Flag = -1;
}
CMacro_FA::~CMacro_FA()
{
	DeleteCriticalSection(&mCS_LOCK);
}

// ...
int  CMacro_FA::Fnc_Init()
{
	if(iInit_Flag > 0)		return 1;

	EnterCriticalSection(&mCS_LOCK);

	iInit_Flag = xFA_300mm_Link->Open("");

	LeaveCriticalSection(&mCS_LOCK);
	return iInit_Flag;
}
int  CMacro_FA::Check__Address(const void* p_addr,
							   const CString& msg)
{
	if(p_addr != NULL)		return 1;

	MessageBox(NULL,msg,"CMacro_FA : Address Null !!!",MB_OK|MB_TOPMOST);
	return -1;
}

int  CMacro_FA::Is__ONLINE()
{
	if(Fnc_Init() < 0)		return -1;

	CI_FA_300mm__E30_CTRL *p_e30_ctrl = xFA_300mm_Link->Get_FA_300mm_CTRL()->Get_E30();

	if(Check__Address(p_e30_ctrl,"Is__ONLINE()") < 0)
	{
		return -1;
	}

	return p_e30_ctrl->Is_ONLINE();
}

int  CMacro_FA::Is__REMOTE()
{
	if(Fnc_Init() < 0)		return -1;

	CI_FA_300mm__E30_CTRL *p_e30_ctrl = xFA_300mm_Link->Get_FA_300mm_CTRL()->Get_E30();

	if(Check__Address(p_e30_ctrl,"Is__REMOTE()") < 0)
	{
		return -1;
	}

	return p_e30_ctrl->Is_REMOTE();
}

int  CMacro_FA::Report_CEID(const int ceid)
{
	if(Fnc_Init() < 0)		return -1;

	return xFA_300mm_Link->Send_EVENT(ceid); 
}
int  CMacro_FA::LP__START(const int ptn)
{
	if(Fnc_Init() < 0)		return -1;

	CI_FA_300mm__E30_CTRL *p_e30_ctrl = xFA_300mm_Link->Get_FA_300mm_CTRL()->Get_E30();

	if(Check__Address(p_e30_ctrl,"LP__START") < 0)
	{
		return -1;
	}

	return p_e30_ctrl->Event__LP__START(ptn);
}

int  CMacro_FA::Is_LP__BYPASS_ID(const int ptn)
{
	if(Fnc_Init() < 0)		return -1;

	CI_FA_300mm__E30_CTRL *p_e30_ctrl = xFA_300mm_Link->Get_FA_300mm_CTRL()->Get_E30();

	if(Check__Address(p_e30_ctrl,"Is_LP__BYPASS_ID()") < 0)
	{
		return -1;
	}

	return p_e30_ctrl->Is_LP__BYPASS_READ_ID(ptn);
}

int  CMacro_FA::CJOB__HOQ(const CString& cjobid)
{
	if(Fnc_Init() < 0)		return -1;

	CI_FA_300mm__E94_CTRL *p_e94_ctrl = xFA_300mm_Link->Get_FA_300mm_CTRL()->Get_E94();

	if(Check__Address(p_e94_ctrl,"CJOB__HOQ()") < 0)
	{
		return -1;
	}

	p_e94_ctrl->Deselect_CJob();
	return p_e94_ctrl->HOQ_CJob(cjobid);
}
int  CMacro_FA::CJOB__CANCEL(const CString& cjobid)
{
	if(Fnc_Init() < 0)		return -1;

	CI_FA_300mm__E94_CTRL *p_e94_ctrl = xFA_300mm_Link->Get_FA_300mm_CTRL()->Get_E94();

	if(Check__Address(p_e94_ctrl,"CJOB__CANCEL()") < 0)
	{
		return -1;
	}

	return p_e94_ctrl->Cancel_CJob(cjobid);
}
int  CMacro_FA::CJOB__ABORT(const CString& cjobid)
{
	if(Fnc_Init() < 0)		return -1;

	CI_FA_300mm__E94_CTRL *p_e94_ctrl = xFA_300mm_Link->Get_FA_300mm_CTRL()->Get_E94();

	if(Check__Address(p_e94_ctrl,"CJOB__ABORT()") < 0)
	{
		return -1;
	}

	return p_e94_ctrl->Abort_CJob(cjobid);
}
int  CMacro_FA::CJOB__ALL_ABORT()
{
	if(Fnc_Init() < 0)		return -1;

	CI_FA_300mm__E94_CTRL *p_e94_ctrl = xFA_300mm_Link->Get_FA_300mm_CTRL()->Get_E94();

	if(Check__Address(p_e94_ctrl,"CJOB__ALL_ABORT()") < 0)
	{
		return -1;
	}

	return p_e94_ctrl->Abort__All_CJob();
}
int  CMacro_FA::CJOB__COMPLETE(const CString& cjobid)
{
	if(Fnc_Init() < 0)		return -1;

	CI_FA_300mm__E40_CTRL *p_e40_ctrl = xFA_300mm_Link->Get_FA_300mm_CTRL()->Get_E40();
	CI_FA_300mm__E94_CTRL *p_e94_ctrl = xFA_300mm_Link->Get_FA_300mm_CTRL()->Get_E94();

	p_e40_ctrl->Complete_PJOBID(cjobid);
	p_e94_ctrl->Complete_CJob(cjobid);

	return 1;
}
int  CMacro_FA::PJOB__CANCEL(const CString& pjobid)
{
	if(Fnc_Init() < 0)		return -1;

	CI_FA_300mm__E40_CTRL *p_e40_ctrl = xFA_300mm_Link->Get_FA_300mm_CTRL()->Get_E40();

	if(Check__Address(p_e40_ctrl,"PJOB__CANCEL()") < 0)
	{
		return -1;
	}

	return p_e40_ctrl->Dequeue_PJOBID(pjobid);
}
int  CMacro_FA::PJOB__DELETE(const int ptn)
{
	if(Fnc_Init() < 0)		return -1;

	CI_FA_300mm__E40_CTRL *p_e40_ctrl = xFA_300mm_Link->Get_FA_300mm_CTRL()->Get_E40();

	if(Check__Address(p_e40_ctrl,"PJOB__CANCEL()") < 0)
	{
		return -1;
	}

	return p_e40_ctrl->Delete__PJOB_FROM_PTN(ptn);
}

int  CMacro_FA::Create__Local_PJOBID(const CString& job_name,
									 const CString& cstid,
									 const int ptn,
									 const CUIntArray& l_slotmap,
									 const CString& ppid)
{
	if(Fnc_Init() < 0)		return -1;

	CI_FA_300mm__E40_CTRL *p_e40_ctrl = xFA_300mm_Link->Get_FA_300mm_CTRL()->Get_E40();

	if(Check__Address(p_e40_ctrl,"Create__Local_PJOBID()") < 0)
	{
		return -1;
	}

	return p_e40_ctrl->Create__Local_PJOBID(job_name,cstid,ptn,l_slotmap,ppid);
}
int  CMacro_FA::Create__Local_CJOBID(const CString& job_name,
									 const CString& cstid)
{
	if(Fnc_Init() < 0)		return -1;

	CI_FA_300mm__E94_CTRL *p_e94_ctrl = xFA_300mm_Link->Get_FA_300mm_CTRL()->Get_E94();

	if(Check__Address(p_e94_ctrl,"Create__Local_CJOBID()") < 0)
	{
		return -1;
	}

	return p_e94_ctrl->Create__Local_CJOBID(job_name,cstid);
}

int  CMacro_FA::Write__VISION_RECIPE(const int ptn,
									 const int slot,
									 const CString& vision_recipe)
{
	if(Fnc_Init() < 0)		return -1;

	CI_FA_VARIABLE_CTRL *p_variable_ctrl = xFA_300mm_Link->Get_FA_VARIABLE_CTRL();

	if(Check__Address(p_variable_ctrl,"Write__VISION_RECIPE()") < 0)
	{
		return -1;
	}

	// ...
	CString main_name;
	CString sub_name;
	CString var_data;

	main_name.Format("P%1d:%1d-INFO", ptn,slot);
	sub_name = "VIS_RCP";
	var_data.Format("%s", vision_recipe);

	return p_variable_ctrl->Set_Variable(main_name,sub_name,var_data);
}
int  CMacro_FA::Read__VISION_RECIPE(const int ptn,
									const int slot,
									CString& vision_recipe)
{
	if(Fnc_Init() < 0)		return -1;

	CI_FA_VARIABLE_CTRL *p_variable_ctrl = xFA_300mm_Link->Get_FA_VARIABLE_CTRL();

	if(Check__Address(p_variable_ctrl,"Read__VISION_RECIPE()") < 0)
	{
		return -1;
	}

	// ...
	CString main_name;
	CString sub_name;
	CString var_data;

	main_name.Format("P%1d:%1d-INFO", ptn,slot);
	sub_name = "VIS_RCP";

	int flag = p_variable_ctrl->Get_Variable(main_name,sub_name,var_data);

	vision_recipe.Format("%s",var_data);
	return flag;
}
