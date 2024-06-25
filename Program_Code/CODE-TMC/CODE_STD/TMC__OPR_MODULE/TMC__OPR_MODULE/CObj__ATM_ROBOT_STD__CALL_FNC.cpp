#include "StdAfx.h"
#include "CObj__ATM_ROBOT_STD.h"


#define  CMMD__MAP				"MAP"
#define  CMMD__HOME				"HOME"
#define  CMMD__VACON			"VACON"
#define  CMMD__VACOFF			"VACOFF"
#define  CMMD__RETRACT			"RETRACT"
#define  CMMD__EXTEND			"EXTEND"
#define  CMMD__GOUP				"GOUP"
#define  CMMD__GODOWN			"GODOWN"
#define  CMMD__ALGN				"ALGN"
#define  CMMD__ALGNPICK			"ALGNPICK"
#define  CMMD__PLACE_ALGN_PICK  "PLACE_PICK"
#define  CMMD__ALGNPLACE		"ALGNPLACE"
#define  CMMD__LP_MAP			"LP_MAP"


// ...
int  CObj__ATM_ROBOT_STD::
Call__INIT(CII_OBJECT__VARIABLE* p_variable)
{
	int flag = pATM_ROBOT__OBJ_CTRL->Call__OBJECT("INIT");

	if(flag > 0)
	{
		xCH__OBJ_STATUS->Set__DATA(STR__STANDBY);
	}
	return flag;
}

int  CObj__ATM_ROBOT_STD::
Call__MAINT(CII_OBJECT__VARIABLE* p_variable)
{
	xCH__OBJ_STATUS->Set__DATA(STR__MAINTMODE);
	return 1;
}

//
int  CObj__ATM_ROBOT_STD::
Call__PICK(CII_OBJECT__VARIABLE* p_variable)
{
	return pATM_ROBOT__OBJ_CTRL->Call__OBJECT("PICK");
}
int  CObj__ATM_ROBOT_STD::
Call__XPICK(CII_OBJECT__VARIABLE* p_variable)
{
	return pATM_ROBOT__OBJ_CTRL->Call__OBJECT("XPICK");
}

//
int  CObj__ATM_ROBOT_STD::
Call__PLACE(CII_OBJECT__VARIABLE* p_variable)
{
	return pATM_ROBOT__OBJ_CTRL->Call__OBJECT("PLACE");
}
int  CObj__ATM_ROBOT_STD::
Call__XPLACE(CII_OBJECT__VARIABLE* p_variable)
{
	return pATM_ROBOT__OBJ_CTRL->Call__OBJECT("XPLACE");
}

//
int  CObj__ATM_ROBOT_STD::
Call__ROTATE(CII_OBJECT__VARIABLE* p_variable)
{
	return pATM_ROBOT__OBJ_CTRL->Call__OBJECT("ROTATE");
}


// MAP -----
int  CObj__ATM_ROBOT_STD::
Call__MAP(CII_OBJECT__VARIABLE* p_variable,
		  CII_OBJECT__ALARM* p_alarm)
{
	return pATM_ROBOT__OBJ_CTRL->Call__OBJECT(CMMD__MAP);
}

// HOME -----
int  CObj__ATM_ROBOT_STD::
Call__HOME(CII_OBJECT__VARIABLE* p_variable,
		   CII_OBJECT__ALARM* p_alarm,
		   const int req__vac_off)
{
	if(req__vac_off > 0)
	{
		return pATM_ROBOT__OBJ_CTRL->Call__OBJECT("VAC_OFF.HOME");
	}

	return pATM_ROBOT__OBJ_CTRL->Call__OBJECT(CMMD__HOME);
}

// VACON -----
int  CObj__ATM_ROBOT_STD::
Call__VACON(CII_OBJECT__VARIABLE* p_variable,
			CII_OBJECT__ALARM* p_alarm,
			const CString& arm_type)
{
	int flag;

	// ...
	{
		CStringArray l_para;
		l_para.Add(arm_type);

		flag = pATM_ROBOT__OBJ_CTRL->Call__OBJ_MODE(CMMD__VACON,l_para);
	}
	return flag;
}

// VACOFF -----
int  CObj__ATM_ROBOT_STD::
Call__VACOFF(CII_OBJECT__VARIABLE* p_variable,
			 CII_OBJECT__ALARM* p_alarm,
			 const CString& arm_type)
{
	int flag;

	// ...
	{
		CStringArray l_para;
		l_para.Add(arm_type);

		flag = pATM_ROBOT__OBJ_CTRL->Call__OBJ_MODE(CMMD__VACOFF,l_para);
	}
	return flag;
}

// RETRACT -----
int  CObj__ATM_ROBOT_STD::
Call__RETRACT(CII_OBJECT__VARIABLE* p_variable)
{
	return pATM_ROBOT__OBJ_CTRL->Call__OBJECT(CMMD__RETRACT);
}

// EXTEND -----
int  CObj__ATM_ROBOT_STD::
Call__EXTEND(CII_OBJECT__VARIABLE* p_variable)
{
	return pATM_ROBOT__OBJ_CTRL->Call__OBJECT(CMMD__EXTEND);
}

// GOUP -----
int  CObj__ATM_ROBOT_STD::
Call__GOUP(CII_OBJECT__VARIABLE* p_variable)
{
	return pATM_ROBOT__OBJ_CTRL->Call__OBJECT(CMMD__GOUP);
}

// GODOWN -----
int  CObj__ATM_ROBOT_STD::
Call__GODOWN(CII_OBJECT__VARIABLE* p_variable)
{
	return pATM_ROBOT__OBJ_CTRL->Call__OBJECT(CMMD__GODOWN);
}

// ALGN ----- 
int  CObj__ATM_ROBOT_STD::
Call__ALGN(CII_OBJECT__VARIABLE* p_variable,
		   CII_OBJECT__ALARM* p_alarm)
{
	return pATM_ROBOT__OBJ_CTRL->Call__OBJECT(CMMD__ALGN);
}

// ALGNPICK -----
int  CObj__ATM_ROBOT_STD::
Call__ALGNPICK(CII_OBJECT__VARIABLE* p_variable,
			   CII_OBJECT__ALARM* p_alarm,
			   const CString& arm_type,
			   const CString& stn_name,
			   const CString& stn_slot)
{
	int flag;

	// ...
	{
		CStringArray l_para;
		l_para.Add(arm_type);
		l_para.Add(stn_name);
		l_para.Add(stn_slot);

		flag = pATM_ROBOT__OBJ_CTRL->Call__OBJ_MODE(CMMD__ALGNPICK,l_para);
	}
	return flag;
}

int  CObj__ATM_ROBOT_STD::
Call__PLACE_ALIGN_PICK(CII_OBJECT__VARIABLE* p_variable,
					   CII_OBJECT__ALARM* p_alarm,
					   const CString& arm_type,
					   const CString& stn_name,
					   const CString& stn_slot)
{
	int flag;

	// ...
	{
		CStringArray l_para;
		l_para.Add(arm_type);
		l_para.Add(stn_name);
		l_para.Add(stn_slot);

		flag = pATM_ROBOT__OBJ_CTRL->Call__OBJ_MODE(CMMD__PLACE_ALGN_PICK,l_para);
	}
	return flag;
}

// ALGNPLACE -----
int  CObj__ATM_ROBOT_STD::
Call__ALGNPLACE(CII_OBJECT__VARIABLE* p_variable,
				CII_OBJECT__ALARM* p_alarm,
				const CString& arm_type,
				const CString& stn_name,
				const CString& stn_slot)
{
	int flag;

	// ...
	{
		CStringArray l_para;
		l_para.Add(arm_type);
		l_para.Add(stn_name);
		l_para.Add(stn_slot);

		flag = pATM_ROBOT__OBJ_CTRL->Call__OBJ_MODE(CMMD__ALGNPLACE,l_para);
	}
	return flag;
}

// LP_MAP -----
int  CObj__ATM_ROBOT_STD::
Call__LP_MAP(CII_OBJECT__VARIABLE* p_variable,
			 CII_OBJECT__ALARM* p_alarm,
			 const CString& arm_type,
			 const CString& stn_name,
			 const CString& stn_slot)
{
	int flag;

	// ...
	{
		CStringArray l_para;
		l_para.Add(arm_type);
		l_para.Add(stn_name);
		l_para.Add(stn_slot);

		flag = pATM_ROBOT__OBJ_CTRL->Call__OBJ_MODE(CMMD__LP_MAP,l_para);
	}
	return flag;
}

