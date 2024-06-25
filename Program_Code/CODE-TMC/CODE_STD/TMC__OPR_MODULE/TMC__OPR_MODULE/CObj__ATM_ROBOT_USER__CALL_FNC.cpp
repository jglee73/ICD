#include "StdAfx.h"
#include "CObj__ATM_ROBOT_USER.h"


#define _CMMD__INIT			"INIT"
#define _CMMD__HOME			"HOME"

#define _CMMD__ROTATE		"ROTATE"
#define _CMMD__PICK			"PICK"
#define _CMMD__PLACE		"PLACE"

#define _CMMD__ALIGN		"ALGN"
#define _CMMD__ALIGN_PICK	"ALGNPICK"

#define _CMMD__LP_MAP		"LP_MAP"


// ...
int  CObj__ATM_ROBOT_USER::
Call__INIT(CII_OBJECT__VARIABLE* p_variable)
{
	return pATM_ROBOT__OBJ_CTRL->Call__OBJECT(_CMMD__INIT);
}

//
int  CObj__ATM_ROBOT_USER::
Call__HOME(CII_OBJECT__VARIABLE* p_variable)
{
	return pATM_ROBOT__OBJ_CTRL->Call__OBJECT(_CMMD__HOME);
}

//
int  CObj__ATM_ROBOT_USER::
Call__ROTATE(CII_OBJECT__VARIABLE* p_variable)
{
	return pATM_ROBOT__OBJ_CTRL->Call__OBJECT(_CMMD__ROTATE);
}
int  CObj__ATM_ROBOT_USER::
Call__PICK(CII_OBJECT__VARIABLE* p_variable)
{
	return pATM_ROBOT__OBJ_CTRL->Call__OBJECT(_CMMD__PICK);
}
int  CObj__ATM_ROBOT_USER::
Call__PLACE(CII_OBJECT__VARIABLE* p_variable)
{
	return pATM_ROBOT__OBJ_CTRL->Call__OBJECT(_CMMD__PLACE);
}

//
int  CObj__ATM_ROBOT_USER::
Call__ALIGN(CII_OBJECT__VARIABLE* p_variable)
{
	return pATM_ROBOT__OBJ_CTRL->Call__OBJECT(_CMMD__ALIGN);
}
int  CObj__ATM_ROBOT_USER::
Call__ALIGN_PICK(CII_OBJECT__VARIABLE* p_variable)
{
	return pATM_ROBOT__OBJ_CTRL->Call__OBJECT(_CMMD__ALIGN_PICK);
}

//
int  CObj__ATM_ROBOT_USER::
Call__LP_MAP(CII_OBJECT__VARIABLE* p_variable)
{
	return pATM_ROBOT__OBJ_CTRL->Call__OBJECT(_CMMD__LP_MAP);
}
