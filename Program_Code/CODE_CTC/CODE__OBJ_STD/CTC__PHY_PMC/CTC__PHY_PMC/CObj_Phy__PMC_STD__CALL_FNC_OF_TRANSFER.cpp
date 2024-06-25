#include "StdAfx.h"
#include "CObj_Phy__PMC_STD.h"
#include "CObj_Phy__PMC_STD__ALID.h"


//--------------------------------------------------------------------------------------
#define  DEF__CHECK_SYSTEM_ABORT							\
if(xSCH_MATERIAL_CTRL->Check__SYSTEM_ABORT() > 0)			\
{															\
	return -1;												\
}


int  CObj_Phy__PMC_STD::Call__PICK_READY(CII_OBJECT__VARIABLE* p_variable)
{
	DEF__CHECK_SYSTEM_ABORT;

	xCH__LPx_WAFER_COUNT->Set__DATA("0");

	Set__Slot_No_Parameter(p_variable);
	Fnc__SET_TIME();

	return xI_Module_Obj->Connect__Module_Obj("PICK_READY");
}
int  CObj_Phy__PMC_STD::Call__PICK_X_READY(CII_OBJECT__VARIABLE* p_variable)
{
	DEF__CHECK_SYSTEM_ABORT;

	Set__Slot_No_Parameter(p_variable);

	return Seq__PICK_X_READY(p_variable);
}
int  CObj_Phy__PMC_STD::Seq__PICK_X_READY(CII_OBJECT__VARIABLE* p_variable)
{
	DEF__CHECK_SYSTEM_ABORT;

	Fnc__SET_TIME();

	return xI_Module_Obj->Connect__Module_Obj("PICK_X_READY");
}
int  CObj_Phy__PMC_STD::Call__PICK_COMPLETE(CII_OBJECT__VARIABLE* p_variable)
{
	DEF__CHECK_SYSTEM_ABORT;

	Set__Slot_No_Parameter(p_variable);

	int flag = xI_Module_Obj->Connect__Module_Obj("PICK_COMPLETE");
	if(flag < 0)		return flag;

	Clear__PMC_Info();
	return flag;
}
int  CObj_Phy__PMC_STD::Call__PICK_X_COMPLETE(CII_OBJECT__VARIABLE* p_variable)
{
	DEF__CHECK_SYSTEM_ABORT;

	Set__Slot_No_Parameter(p_variable);

	int flag = xI_Module_Obj->Connect__Module_Obj("PICK_X_COMPLETE");
	if(flag < 0)		return flag;

	Clear__PMC_Info();
	return flag;
}

int  CObj_Phy__PMC_STD::Call__PLACE_READY(CII_OBJECT__VARIABLE* p_variable)
{
	DEF__CHECK_SYSTEM_ABORT;

	Set__Place_Ready_Parameter(p_variable);

	return Seq__PLACE_READY(p_variable);
}
int  CObj_Phy__PMC_STD::Seq__PLACE_READY(CII_OBJECT__VARIABLE* p_variable)
{
	DEF__CHECK_SYSTEM_ABORT;

	Fnc__SET_TIME();

	return xI_Module_Obj->Connect__Module_Obj("PLACE_READY");
}

int  CObj_Phy__PMC_STD::Call__PLACE_X_READY(CII_OBJECT__VARIABLE* p_variable)
{
	DEF__CHECK_SYSTEM_ABORT;

	Set__Place_Ready_Parameter(p_variable);
	Fnc__SET_TIME();

	return xI_Module_Obj->Connect__Module_Obj("PLACE_X_READY");
}

int  CObj_Phy__PMC_STD::Call__PLACE_COMPLETE(CII_OBJECT__VARIABLE* p_variable)
{
	DEF__CHECK_SYSTEM_ABORT;

	Set__Slot_No_Parameter(p_variable);

	return Seq__PLACE_COMPLETE(p_variable);
}
int  CObj_Phy__PMC_STD::Seq__PLACE_COMPLETE(CII_OBJECT__VARIABLE* p_variable)
{
	DEF__CHECK_SYSTEM_ABORT;

	if(xCH_CFG__CLEAN_MODE->Check__DATA(STR__CLEAN_MODE__SYSTEM_IDLE) < 0)
	{
		xTIMER__SYS_IDLE->START__COUNT_DOWN(1.0);
	}

	return xI_Module_Obj->Connect__Module_Obj("PLACE_COMPLETE");
}

int  CObj_Phy__PMC_STD::Call__PLACE_X_COMPLETE(CII_OBJECT__VARIABLE* p_variable)
{
	DEF__CHECK_SYSTEM_ABORT;

	if(xCH_CFG__CLEAN_MODE->Check__DATA(STR__CLEAN_MODE__SYSTEM_IDLE) < 0)
	{
		xTIMER__SYS_IDLE->START__COUNT_DOWN(1.0);
	}

	Set__Slot_No_Parameter(p_variable);

	return xI_Module_Obj->Connect__Module_Obj("PLACE_X_COMPLETE");
}

// ...
int  CObj_Phy__PMC_STD
::Call__LOAD(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	return xI_Module_Obj->Connect__Module_Obj("LOAD");
}

int  CObj_Phy__PMC_STD
::Call__UNLOAD(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	return xI_Module_Obj->Connect__Module_Obj("UNLOAD");
}

int  CObj_Phy__PMC_STD
::Call__READY_TO_UNLOAD(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	return xI_Module_Obj->Connect__Module_Obj("READY_TO_UNLOAD");
}

// ...
int  CObj_Phy__PMC_STD
::Call__SLOT_OPEN(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM *p_alarm)
{
	return xI_Module_Obj->Connect__Module_Obj("SLOT.OPEN");
}

int  CObj_Phy__PMC_STD
::Call__SLOT_CLOSE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM *p_alarm)
{
	return xI_Module_Obj->Connect__Module_Obj("SLOT.CLOSE");
}

// ...
int  CObj_Phy__PMC_STD
::Call__PIN_UP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM *p_alarm)
{
	return xI_Module_Obj->Connect__Module_Obj("PIN.UP");
}

int  CObj_Phy__PMC_STD
::Call__PIN_DOWN(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM *p_alarm)
{
	return xI_Module_Obj->Connect__Module_Obj("PIN.DOWN");
}

