#pragma once

#include "Interface_Code.h"


class CMacro_FA
{
private:
	CRITICAL_SECTION mCS_LOCK;

	// ...
	CX_FA_300mm_LINK xFA_300mm_Link;
	int iInit_Flag;

	int Fnc_Init();
	int Check__Address(const void* p_addr,const CString& msg);

public:
	CMacro_FA();
	~CMacro_FA();

	// ...
	int  Is__ONLINE();
	int  Is__REMOTE();

	int  Is_LP__BYPASS_ID(const int ptn);

	int  CJOB__HOQ(const CString& cjobid);
	int  CJOB__CANCEL(const CString& cjobid);
	int  CJOB__ABORT(const CString& cjobid);
	int  CJOB__ALL_ABORT();
	int  CJOB__COMPLETE(const CString& cjobid);

	int  PJOB__CANCEL(const CString& pjobid);
	int  PJOB__DELETE(const int ptn);

	int  Create__Local_PJOBID(const CString& job_name,
								const CString& cstid,
								const int ptn,
								const CUIntArray& l_slotmap,
								const CString& ppid);
	int  Create__Local_CJOBID(const CString& job_name,const CString& cstid);

	int  Write__VISION_RECIPE(const int ptn,const int slot,const CString& vision_recipe);
	int  Read__VISION_RECIPE(const int ptn,const int slot,CString& vision_recipe);

	int  Report_CEID(const int ceid);
	int  LP__START(const int ptn);
};
