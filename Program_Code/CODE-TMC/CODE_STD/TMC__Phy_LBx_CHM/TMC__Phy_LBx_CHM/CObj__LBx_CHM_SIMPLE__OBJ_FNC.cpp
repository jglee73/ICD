#include "StdAfx.h"
#include "CObj__LBx_CHM_SIMPLE.h"


// OBJ FNC ...
int  CObj__LBx_CHM_SIMPLE
::_Fnc__OBJ_GAS__ALL_CLOSE()
{
	if(!bActive__OBJ_GAS)			return 1;

	// ...
	CString obj_cmmd = "ALL_CLOSE";

	return pOBJ_CTRL__GAS->Call__OBJECT(obj_cmmd);
}

int  CObj__LBx_CHM_SIMPLE
::_Fnc__OBJ_VAT__OPEN()
{
	if(!bActive__OBJ_VAT)			return 1;

	// ...
	CString obj_cmmd = "OPEN";

	return pOBJ_CTRL__VAT->Call__OBJECT(obj_cmmd);
}
int  CObj__LBx_CHM_SIMPLE
::_Fnc__OBJ_VAT__CLOSE()
{
	if(!bActive__OBJ_VAT)			return 1;

	// ...
	CString obj_cmmd = "CLOSE";

	return pOBJ_CTRL__VAT->Call__OBJECT(obj_cmmd);
}
