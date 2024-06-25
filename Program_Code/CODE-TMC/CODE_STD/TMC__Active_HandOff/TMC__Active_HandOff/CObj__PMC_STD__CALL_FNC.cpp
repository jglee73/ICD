#include "StdAfx.h"
#include "CObj__PMC_STD.h"


// ...
int  CObj__PMC_STD::Call__INIT(CII_OBJECT__VARIABLE* p_variable)
{
	return xI_Module_Obj->Connect__Module_Obj(CMMD__INIT);
}

// ...
int  CObj__PMC_STD::Call__S1(CII_OBJECT__VARIABLE* p_variable)
{
	return xI_Module_Obj->Connect__Module_Obj(CMMD__S1);
}
int  CObj__PMC_STD::Call__S2(CII_OBJECT__VARIABLE* p_variable)
{
	return xI_Module_Obj->Connect__Module_Obj(CMMD__S2);
}
int  CObj__PMC_STD::Call__S3(CII_OBJECT__VARIABLE* p_variable)
{
	return xI_Module_Obj->Connect__Module_Obj(CMMD__S3);
}

//
int  CObj__PMC_STD::Call__R1(CII_OBJECT__VARIABLE* p_variable)
{
	return xI_Module_Obj->Connect__Module_Obj(CMMD__R1);
}
int  CObj__PMC_STD::Call__R2(CII_OBJECT__VARIABLE* p_variable)
{
	return xI_Module_Obj->Connect__Module_Obj(CMMD__R2);
}
int  CObj__PMC_STD::Call__R3(CII_OBJECT__VARIABLE* p_variable)
{
	return xI_Module_Obj->Connect__Module_Obj(CMMD__R3);
}
