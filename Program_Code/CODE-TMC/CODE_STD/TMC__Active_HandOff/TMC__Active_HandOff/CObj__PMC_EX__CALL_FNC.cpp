#include "StdAfx.h"
#include "CObj__PMC_EX.h"


// ...
int  CObj__PMC_EX::
Call__INIT(CII_OBJECT__VARIABLE* p_variable, const int pm_i)
{
	return xI_PMx_Obj[pm_i]->Connect__Module_Obj(CMMD__INIT);
}

// ...
int  CObj__PMC_EX::
Call__TRANSFER_READY(CII_OBJECT__VARIABLE* p_variable, const int pm_i)
{
	return xI_PMx_Obj[pm_i]->Connect__Module_Obj(CMMD__TRANSFER_READY);
}
int  CObj__PMC_EX::
Call__TRANSFER_END(CII_OBJECT__VARIABLE* p_variable, const int pm_i)
{
	return xI_PMx_Obj[pm_i]->Connect__Module_Obj(CMMD__TRANSFER_END);
}

// ...
int  CObj__PMC_EX::
Call__S1(CII_OBJECT__VARIABLE* p_variable, const int pm_i)
{
	return xI_PMx_Obj[pm_i]->Connect__Module_Obj(CMMD__S1);
}
int  CObj__PMC_EX::
Call__S2(CII_OBJECT__VARIABLE* p_variable, const int pm_i)
{
	return xI_PMx_Obj[pm_i]->Connect__Module_Obj(CMMD__S2);
}
int  CObj__PMC_EX::
Call__S3(CII_OBJECT__VARIABLE* p_variable, const int pm_i)
{
	return xI_PMx_Obj[pm_i]->Connect__Module_Obj(CMMD__S3);
}

// ...
int  CObj__PMC_EX::
Call__R1(CII_OBJECT__VARIABLE* p_variable, const int pm_i)
{
	return xI_PMx_Obj[pm_i]->Connect__Module_Obj(CMMD__R1);
}
int  CObj__PMC_EX::
Call__R2(CII_OBJECT__VARIABLE* p_variable, const int pm_i)
{
	return xI_PMx_Obj[pm_i]->Connect__Module_Obj(CMMD__R2);
}
int  CObj__PMC_EX::
Call__R3(CII_OBJECT__VARIABLE* p_variable, const int pm_i)
{
	return xI_PMx_Obj[pm_i]->Connect__Module_Obj(CMMD__R3);
}
