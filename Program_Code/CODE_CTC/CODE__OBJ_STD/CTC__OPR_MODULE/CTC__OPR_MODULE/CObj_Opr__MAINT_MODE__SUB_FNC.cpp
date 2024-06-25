#include "StdAfx.h"
#include "CObj_Opr__MAINT_MODE.h"
#include "CObj_Opr__MAINT_MODE__DEF.h"


// ...
CString CObj_Opr__MAINT_MODE
::Get__ATM_RB1__ARM_TYPE(const CString& str_arm)
{
	if(str_arm == MODULE__ATM_RB1_A)		return "A";
	if(str_arm == MODULE__ATM_RB1_B)		return "B";

	return str_arm;
}

CString CObj_Opr__MAINT_MODE
::Get__VAC_RB1__ARM_TYPE(const CString& str_arm)
{
	if(str_arm == MODULE__VAC_RB1_A)		return "A";
	if(str_arm == MODULE__VAC_RB1_B)		return "B";

	return str_arm;
}

bool CObj_Opr__MAINT_MODE
::Check__LLx_ISLOT(const CString str_module, CString& ll_name,CString& ll_slot)
{
	CString str_name;
	int i;

	for(i=0; i<iLLx_SIZE; i++)
	{
		int id = i + 1;

			 if(id == 1)		ll_name = STR__LBA;
		else if(id == 2)		ll_name = STR__LBB;
		else if(id == 3)		ll_name = STR__LBC;
		else if(id == 4)		ll_name = STR__LBD;
		else					continue;

		str_name.Format("LL%1d-1", id);
		if(str_module.CompareNoCase(str_name) == 0)
		{
			ll_slot = "1";
			return true;
		}

		str_name.Format("LL%1d-2", id);
		if(str_module.CompareNoCase(str_name) == 0)
		{
			ll_slot = "2";
			return true;
		}
	}

	return false;
}

int CObj_Opr__MAINT_MODE
::Get__LLx_INDEX(const CString str_module)
{
	if(str_module.CompareNoCase(STR__LBA) == 0)			return _LLx_INDEX__LBA;
	if(str_module.CompareNoCase(STR__LBB) == 0)			return _LLx_INDEX__LBB;
	if(str_module.CompareNoCase(STR__LBC) == 0)			return _LLx_INDEX__LBC;
	if(str_module.CompareNoCase(STR__LBD) == 0)			return _LLx_INDEX__LBD;

	if(str_module.Find("LL1") == 0)						return _LLx_INDEX__LBA;
	if(str_module.Find("LL2") == 0)						return _LLx_INDEX__LBB;
	if(str_module.Find("LL3") == 0)						return _LLx_INDEX__LBC;
	if(str_module.Find("LL4") == 0)						return _LLx_INDEX__LBD;

	return -1;
}
CString CObj_Opr__MAINT_MODE
::Get__LLx_NAME(const int ll_i)
{
	if(ll_i == _LLx_INDEX__LBA)							return STR__LBA;
	if(ll_i == _LLx_INDEX__LBB)							return STR__LBB;
	if(ll_i == _LLx_INDEX__LBC)							return STR__LBC;
	if(ll_i == _LLx_INDEX__LBD)							return STR__LBD;

	return STR__LLx;
}
CString CObj_Opr__MAINT_MODE
::Get__LLx_NAME(const CString& ll_name)
{
	if(ll_name.CompareNoCase(STR__LL1) == 0)							return STR__LBA;
	if(ll_name.CompareNoCase(STR__LL2) == 0)							return STR__LBB;
	if(ll_name.CompareNoCase(STR__LL3) == 0)							return STR__LBC;
	if(ll_name.CompareNoCase(STR__LL4) == 0)							return STR__LBD;

	return ll_name;
}
