#include "StdAfx.h"
#include "CObj__VAC_ROBOT_STD.h"
#include "CObj__VAC_ROBOT_STD__DEF.h"


//----------------------------------------------------------------------------------------------------
#define  CMMD__INIT				"INIT"
#define  CMMD__SET_PARA			"SET.PARA"

#define  CMMD__ROTATE			"ROTATE"
#define  CMMD__PICK				"PICK"
#define  CMMD__PLACE			"PLACE"
#define  CMMD__XPICK			"XPICK"
#define  CMMD__XPLACE			"XPLACE"

#define  CMMD__HOME				"HOME"
#define  CMMD__MAP				"MAP"


// ...
int  CObj__VAC_ROBOT_STD::
Call__HOME(CII_OBJECT__VARIABLE* p_variable,
		   CII_OBJECT__ALARM* p_alarm)
{
	return pVAC_ROBOT__OBJ_CTRL->Call__OBJECT(CMMD__HOME);
}

int  CObj__VAC_ROBOT_STD::
Call__MAP(CII_OBJECT__VARIABLE* p_variable,
		  CII_OBJECT__ALARM* p_alarm)
{
	return pVAC_ROBOT__OBJ_CTRL->Call__OBJECT(CMMD__MAP);
}

//
int  CObj__VAC_ROBOT_STD::
Call__INIT(CII_OBJECT__VARIABLE* p_variable)
{
	int flag = pVAC_ROBOT__OBJ_CTRL->Call__OBJECT(CMMD__INIT);

	if(flag > 0)
	{
		dCH__OBJ_STATUS->Set__DATA(STR__STANDBY);
	}
	else
	{
		dCH__OBJ_STATUS->Set__DATA(STR__MAINTMODE);
	}
	return flag;
}
int  CObj__VAC_ROBOT_STD::
Call__MAINT(CII_OBJECT__VARIABLE* p_variable)
{
	dCH__OBJ_STATUS->Set__DATA(STR__MAINTMODE);
	return 1;
}

int  CObj__VAC_ROBOT_STD::
Call__SET_PARA(CII_OBJECT__VARIABLE* p_variable)
{
	return pVAC_ROBOT__OBJ_CTRL->Call__OBJECT(CMMD__SET_PARA);
}

//
int  CObj__VAC_ROBOT_STD::
Call__PICK(CII_OBJECT__VARIABLE* p_variable)
{
	return pVAC_ROBOT__OBJ_CTRL->Call__OBJECT(CMMD__PICK);
}
int  CObj__VAC_ROBOT_STD::
Call__XPICK(CII_OBJECT__VARIABLE* p_variable)
{
	return pVAC_ROBOT__OBJ_CTRL->Call__OBJECT(CMMD__XPICK);
}

int  CObj__VAC_ROBOT_STD::
Call__PLACE(CII_OBJECT__VARIABLE* p_variable)
{
	return pVAC_ROBOT__OBJ_CTRL->Call__OBJECT(CMMD__PLACE);
}
int  CObj__VAC_ROBOT_STD::
Call__XPLACE(CII_OBJECT__VARIABLE* p_variable)
{
	return pVAC_ROBOT__OBJ_CTRL->Call__OBJECT(CMMD__XPLACE);
}

int  CObj__VAC_ROBOT_STD::
Call__ROTATE(CII_OBJECT__VARIABLE* p_variable)
{
	return pVAC_ROBOT__OBJ_CTRL->Call__OBJECT(CMMD__ROTATE);
}


// RETRACT -----
int  CObj__VAC_ROBOT_STD::
Call__RETRACT(CII_OBJECT__VARIABLE* p_variable)
{
	return pVAC_ROBOT__OBJ_CTRL->Call__OBJECT(CMMD__RETRACT);
}

// EXTEND -----
int  CObj__VAC_ROBOT_STD::
Call__EXTEND(CII_OBJECT__VARIABLE* p_variable)
{
	return pVAC_ROBOT__OBJ_CTRL->Call__OBJECT(CMMD__EXTEND);
}

// GOUP -----
int  CObj__VAC_ROBOT_STD::
Call__GOUP(CII_OBJECT__VARIABLE* p_variable)
{
	return pVAC_ROBOT__OBJ_CTRL->Call__OBJECT(CMMD__GOUP);
}

// GODOWN -----
int  CObj__VAC_ROBOT_STD::
Call__GODOWN(CII_OBJECT__VARIABLE* p_variable)
{
	return pVAC_ROBOT__OBJ_CTRL->Call__OBJECT(CMMD__GODOWN);
}

int  CObj__VAC_ROBOT_STD::
Call__RQ_STN(CII_OBJECT__VARIABLE* p_variable)
{
	return pVAC_ROBOT__OBJ_CTRL->Call__OBJECT("RQ_STN");
}

int  CObj__VAC_ROBOT_STD::
Call__SET_STN(CII_OBJECT__VARIABLE* p_variable)
{
	return pVAC_ROBOT__OBJ_CTRL->Call__OBJECT("SET_STN");
}

int  CObj__VAC_ROBOT_STD::
Call__TEACHED_CPTR_SAVE(CII_OBJECT__VARIABLE* p_variable)
{
	return pVAC_ROBOT__OBJ_CTRL->Call__OBJECT("TEACHED_CPTR_SAVE");
}

void CObj__VAC_ROBOT_STD::
Mon__HYPER_TERMINAL_INTERLOCK(CII_OBJECT__VARIABLE* p_variable,
							  CII_OBJECT__ALARM* p_alarm)
{
	SCX__TIMER_CTRL cx_timer_ctrl;
	cx_timer_ctrl->REGISTER__ABORT_OBJECT(sObject_Name);

	while(1)
	{
		cx_timer_ctrl->WAIT(0.5);

		if(dCH__OBJ_STATUS->Check__DATA(STR__MAINTMODE) < 0)
		{
			dEXT_PHY__IO_CH__HYPER_TERMINAL_LOCK->Set__DATA("YES");
		}
	}	
}
