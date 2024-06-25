#include "StdAfx.h"
#include "CObj__LBx_STD.h"


#define _CMMD_CHM__INIT					"INIT"
#define _CMMD_CHM__MAINT				"MAINT"

#define _CMMD_CHM__DV_OPEN				"DV_OPEN"
#define _CMMD_CHM__DV_CLOSE				"DV_CLOSE"

#define _CMMD_CHM__SV_OPEN				"SV_OPEN"
#define _CMMD_CHM__SV_CLOSE				"SV_CLOSE"

#define _CMMD_CHM__LIFT_PIN_UP			"LIFT_PIN.UP"
#define _CMMD_CHM__LIFT_PIN_DOWN		"LIFT_PIN.DOWN"

#define _CMMD_CHM__PUMP					"PUMP"
#define _CMMD_CHM__VENT					"VENT"

#define _CMMD_CHM__PREPMATER			"PREPMATER"
#define _CMMD_CHM__COMPMATER			"COMPMATER"

#define _CMMD_CHM__IN_PROC				"IN_PROC"
#define _CMMD_CHM__OUT_PROC				"OUT_PROC"

#define _CMMD_CHM__ISOLATE				"ISOLATE"
#define _CMMD_CHM__LEAK_CHECK			"LEAK_CHECK"
#define _CMMD_CHM__CYCLE_PURGE			"CYCLE_PURGE"


// ...
int  CObj__LBx_STD::
Call__INIT(CII_OBJECT__VARIABLE* p_variable)
{
	int flag = pOBJ_CTRL__LLx_CHM->Call__OBJECT(_CMMD_CHM__INIT);

	if(flag > 0)
	{
		dCH__OBJ_STATUS->Set__DATA(STR__STANDBY);
	}
	return flag;
}

int  CObj__LBx_STD::
Call__MAINT(CII_OBJECT__VARIABLE* p_variable)
{
	dCH__OBJ_STATUS->Set__DATA(STR__MAINTMODE);
	return 1;
}

int  CObj__LBx_STD::
Call__DV_CLOSE(CII_OBJECT__VARIABLE* p_variable)
{
	return pOBJ_CTRL__LLx_CHM->Call__OBJECT(_CMMD_CHM__DV_CLOSE);
}

int  CObj__LBx_STD::
Call__SV_CLOSE(CII_OBJECT__VARIABLE* p_variable)
{
	return pOBJ_CTRL__LLx_CHM->Call__OBJECT(_CMMD_CHM__SV_CLOSE);
}

int  CObj__LBx_STD::
Call__PUMP(CII_OBJECT__VARIABLE* p_variable)
{
	return pOBJ_CTRL__LLx_CHM->Call__OBJECT(_CMMD_CHM__PUMP);
}
int  CObj__LBx_STD::
Call__VENT(CII_OBJECT__VARIABLE* p_variable)
{
	return pOBJ_CTRL__LLx_CHM->Call__OBJECT(_CMMD_CHM__VENT);
}


// ...
int  CObj__LBx_STD::Call__DOOR_OPEN(CII_OBJECT__VARIABLE* p_variable)
{
	return pOBJ_CTRL__LLx_CHM->Call__OBJECT(_CMMD_CHM__DV_OPEN);
}
int  CObj__LBx_STD::Call__DOOR_CLOSE(CII_OBJECT__VARIABLE* p_variable)
{
	return pOBJ_CTRL__LLx_CHM->Call__OBJECT(_CMMD_CHM__DV_CLOSE);
}

int  CObj__LBx_STD::Call__SLOT_OPEN(CII_OBJECT__VARIABLE* p_variable)
{
	return pOBJ_CTRL__LLx_CHM->Call__OBJECT(_CMMD_CHM__SV_OPEN);
}
int  CObj__LBx_STD::Call__SLOT_CLOSE(CII_OBJECT__VARIABLE* p_variable)
{
	return pOBJ_CTRL__LLx_CHM->Call__OBJECT(_CMMD_CHM__SV_CLOSE);
}

int  CObj__LBx_STD::Call__PIN_UP(CII_OBJECT__VARIABLE* p_variable)
{
	return pOBJ_CTRL__LLx_CHM->Call__OBJECT(_CMMD_CHM__LIFT_PIN_UP);
}
int  CObj__LBx_STD::Call__PIN_DOWN(CII_OBJECT__VARIABLE* p_variable)
{
	return pOBJ_CTRL__LLx_CHM->Call__OBJECT(_CMMD_CHM__LIFT_PIN_DOWN);
}


// ...
int  CObj__LBx_STD::
Call__PREPMATER(CII_OBJECT__VARIABLE* p_variable)
{
	return pOBJ_CTRL__LLx_CHM->Call__OBJECT(_CMMD_CHM__PREPMATER);
}
int  CObj__LBx_STD::
Call__COMPMATER(CII_OBJECT__VARIABLE* p_variable)
{
	return pOBJ_CTRL__LLx_CHM->Call__OBJECT(_CMMD_CHM__COMPMATER);
}

int  CObj__LBx_STD::
Call__IN_PROC(CII_OBJECT__VARIABLE* p_variable)
{
	return pOBJ_CTRL__LLx_CHM->Call__OBJECT(_CMMD_CHM__IN_PROC);
}
int  CObj__LBx_STD::
Call__OUT_PROC(CII_OBJECT__VARIABLE* p_variable)
{
	return pOBJ_CTRL__LLx_CHM->Call__OBJECT(_CMMD_CHM__OUT_PROC);
}

int  CObj__LBx_STD::
Call__ISOLATE(CII_OBJECT__VARIABLE* p_variable)
{
	return pOBJ_CTRL__LLx_CHM->Call__OBJECT(_CMMD_CHM__ISOLATE);
}

int  CObj__LBx_STD::
Call__LEAK_CHECK(CII_OBJECT__VARIABLE* p_variable)
{
	return pOBJ_CTRL__LLx_CHM->Call__OBJECT(_CMMD_CHM__LEAK_CHECK);
}

int  CObj__LBx_STD::
Call__CYCLE_PURGE(CII_OBJECT__VARIABLE* p_variable)
{
	return pOBJ_CTRL__LLx_CHM->Call__OBJECT(_CMMD_CHM__CYCLE_PURGE);
}
