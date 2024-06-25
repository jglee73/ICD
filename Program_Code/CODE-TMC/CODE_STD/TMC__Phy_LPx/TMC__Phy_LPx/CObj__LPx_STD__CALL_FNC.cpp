#include "StdAfx.h"
#include "CObj__LPx_STD.h"
#include "CObj__LPx_STD__DEF.h"
#include "CObj__LPx_STD__ALID.h"


//----------------------------------------------------------------------------------------------------
// INIT ----- 
int  CObj__LPx_STD
::Call__INIT(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	int flag = 1;
	
	if(pLPx__OBJ_CTRL->Call__OBJECT(CMMD__INIT) < 0)
	{
		flag = -1;
	}

	Update__MAP_INFO(p_variable,p_alarm);
	
	if(dCH__CFG_RF_EXIST_FLAG->Check__DATA(STR__YES) > 0)
	{
		if(bOBJ_TYPE__DUAL)
		{
			if(bActive__RFx_OBJ)
			{
				if(pRFx__OBJ_CTRL->Call__OBJECT(CMMD__INIT) < 0)
					flag = -1;
			}
		}
	}
	return flag;
}

int  CObj__LPx_STD
::Call__HOME(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	return pLPx__OBJ_CTRL->Call__OBJECT(CMMD__HOME);
}

// DOOR OPEN -----	
int  CObj__LPx_STD
::Call__DOOR_OPEN(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	if(sCH__DOOR_STS->Check__DATA(STR__OPEN) > 0) 
	{
		return 1;
	}
	return pLPx__OBJ_CTRL->Call__OBJECT(CMMD__DOOR_OPEN);
}

// CHECK DOOR OPEN -----
int  CObj__LPx_STD
::Call__CHECK_DOOR_OPEN(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
LOOP_CHECK:
	
	if(sCH__DOOR_STS->Check__DATA(STR__OPEN) > 0) 
	{
		return 1;
	}

	// ...
	{
		int alarm_id = ALID__DOOR__NOT_OPEN;
		CString r_act;

		p_alarm->Popup__ALARM(alarm_id,r_act);

		if(r_act.CompareNoCase(ACT__RETRY) == 0)
		{
			goto LOOP_CHECK;
		}
	}
	return -1;
}

// LOAD -----
int  CObj__LPx_STD
::Call__LOAD(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	int flag = pLPx__OBJ_CTRL->Call__OBJECT(CMMD__LOAD);

	if(flag > 0)
	{
		if(dCH__CFG_LOAD_MAP_MODE->Check__DATA(STR__ENABLE) > 0)
		{
			flag = Fnc__MAP(p_variable, p_alarm);
		}
		else
		{
			if(dCH__CFG_LOAD_ACT_AUTO_MAP_MODE->Check__DATA(STR__ENABLE) > 0)
			{
				Update__MAP_INFO(p_variable,p_alarm);
			}
		}
	}

	Update__MAP_INFO(p_variable,p_alarm);
	return flag;
}

// UNLOAD -----
int  CObj__LPx_STD
::Call__UNLOAD(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	int r_flag;

	r_flag = Alarm_Check__ARM_NOT_RETRACTED(p_alarm);
	if(r_flag < 0)		return r_flag;

	r_flag = pLPx__OBJ_CTRL->Call__OBJECT(CMMD__UNLOAD);

	Update__MAP_INFO(p_variable,p_alarm);
	return r_flag;
}

// PREPLOAD -----
int  CObj__LPx_STD
::Call__PREPLOAD(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	for(int i=0;i<CFG_LPx__SLOT_MAX;i++)
	{
		dCH__SLOT_STATUS[i]->Set__DATA("NONE");		// PREPLOAD ÀÌ¹Ç·Î..
	}
	
	if(pLPx__OBJ_CTRL->Call__OBJECT(CMMD__PREPLOAD) < 0)
	{
		return -1;
	}

	if(dCH__CFG_PREPLOAD_CID_MODE->Check__DATA(STR__ENABLE) > 0)
	{
		Fnc__CID_READ(p_variable, p_alarm);
	}
	return 1;
}

int  CObj__LPx_STD
::Call__RLSUNLOAD(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	return pLPx__OBJ_CTRL->Call__OBJECT(CMMD__RLSUNLOAD);
}

int  CObj__LPx_STD
::Call__CYCLE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	return pLPx__OBJ_CTRL->Call__OBJECT(CMMD__CYCLE);
}

// CID_READ -----
int  CObj__LPx_STD
::Call__CID_READ(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	return Fnc__CID_READ(p_variable, p_alarm);
}
int  CObj__LPx_STD
::Fnc__CID_READ(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	if(dCH__CFG_RF_EXIST_FLAG->Check__DATA(STR__YES) > 0)
	{
		if(bActive__RFx_OBJ)
		{
			int flag = pRFx__OBJ_CTRL->Call__OBJECT(CMMD__CID_READ);
	
			// ...
			{
				CString var_data;

				sEXT_CH__RFx__CID_STRING->Get__DATA(var_data);
				sCH__FOUP_CID_STRING->Set__DATA(var_data);
			}
			return flag;
		}
	}

	sCH__FOUP_CID_STRING->Set__DATA("");
	return 1;
}
int  CObj__LPx_STD
::Call__PAGE_READ(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	if(dCH__CFG_RF_EXIST_FLAG->Check__DATA(STR__YES) > 0)
	{
		if(bActive__RFx_OBJ)
		{
			return pRFx__OBJ_CTRL->Call__OBJECT(CMMD__PAGE_READ);
		}
	}
	return 1;
}

int  CObj__LPx_STD
::Call__CID_WRITE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	if(dCH__CFG_RF_EXIST_FLAG->Check__DATA(STR__YES) > 0)
	{
		if(bActive__RFx_OBJ)
		{
			return pRFx__OBJ_CTRL->Call__OBJECT(CMMD__CID_WRITE);
		}
	}
	return 1;
}
int  CObj__LPx_STD
::Call__PAGE_WRITE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	if(dCH__CFG_RF_EXIST_FLAG->Check__DATA(STR__YES) > 0)
	{
		if(bActive__RFx_OBJ)
		{
			return pRFx__OBJ_CTRL->Call__OBJECT(CMMD__PAGE_WRITE);
		}
	}
	return 1;
}

// MAP -----
int  CObj__LPx_STD
::Call__MAP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	return Fnc__MAP(p_variable, p_alarm);
}
int  CObj__LPx_STD
::Fnc__MAP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	int flag = pLPx__OBJ_CTRL->Call__OBJECT(CMMD__MAP);

	Update__MAP_INFO(p_variable,p_alarm);
	return flag;
}
int  CObj__LPx_STD
::Update__MAP_INFO(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	CString str__slot_sts;

	CString var_data;
	int i;

	for(i=0;i<CFG_LPx__SLOT_MAX;i++)
	{
		sEXT_CH__LPx__SLOT_MAP_RESULT[i]->Get__DATA(var_data);

		if(var_data.CompareNoCase(STR__ABSENT) == 0)
		{
			str__slot_sts = STR__NONE;
		}
		else if(var_data.CompareNoCase(STR__PRESENT) == 0)
		{
			str__slot_sts = STR__EXIST;
		}
		else
		{
			str__slot_sts = STR__UNKNOWN;
		}

		dCH__SLOT_STATUS[i]->Set__DATA(str__slot_sts);
	}
	return 1;
}

// CLAMP -----
int  CObj__LPx_STD
::Call__CLAMP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	return pLPx__OBJ_CTRL->Call__OBJECT(CMMD__CLAMP);
}
	
// UNCLAMP -----
int  CObj__LPx_STD
::Call__UNCLAMP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	return pLPx__OBJ_CTRL->Call__OBJECT(CMMD__UNCLAMP);
}

// EXTENDPADDLE -----
int  CObj__LPx_STD
::Call__EXTENDPADDLE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	return pLPx__OBJ_CTRL->Call__OBJECT(CMMD__EXTENDPADDLE);
}

// RETRACTPADDLE -----
int  CObj__LPx_STD
::Call__RETRACTPADDLE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	return pLPx__OBJ_CTRL->Call__OBJECT(CMMD__RETRACTPADDLE);
}

// LATCH -----
int  CObj__LPx_STD
::Call__LATCH(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	return pLPx__OBJ_CTRL->Call__OBJECT(CMMD__LATCH);
}
    
// UNLATCH -----
int  CObj__LPx_STD
::Call__UNLATCH(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	return pLPx__OBJ_CTRL->Call__OBJECT(CMMD__UNLATCH);
}

// CLOSEDOOR -----
int  CObj__LPx_STD
::Call__CLOSEDOOR(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	int r_flag = Alarm_Check__ARM_NOT_RETRACTED(p_alarm);
	if(r_flag < 0)		return r_flag;

	return pLPx__OBJ_CTRL->Call__OBJECT(CMMD__CLOSEDOOR);
}
	
// OPENDOOR -----
int  CObj__LPx_STD
::Call__OPENDOOR(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	return pLPx__OBJ_CTRL->Call__OBJECT(CMMD__OPENDOOR);
}

int  CObj__LPx_STD
::Call__OPENDOOR_WITH_NO_UNLATCH(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	return pLPx__OBJ_CTRL->Call__OBJECT(CMMD__OPENDOOR_WITH_NO_UNLATCH);
}
	
// VACON -----
int  CObj__LPx_STD
::Call__VACON(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	return pLPx__OBJ_CTRL->Call__OBJECT(CMMD__VACON);
}
 	
// VACOFF -----
int  CObj__LPx_STD::
Call__VACOFF(CII_OBJECT__VARIABLE* p_variable,
			  CII_OBJECT__ALARM* p_alarm)
{
	//
	return pLPx__OBJ_CTRL->Call__OBJECT(CMMD__VACOFF);
}

// EXTENDSHUTTLE -----
int  CObj__LPx_STD::
Call__EXTENDSHUTTLE(CII_OBJECT__VARIABLE* p_variable,
			  CII_OBJECT__ALARM* p_alarm)
{
	//
	return pLPx__OBJ_CTRL->Call__OBJECT(CMMD__EXTENDSHUTTLE);
}

// RETRACTSHUTTLE -----
int  CObj__LPx_STD::
Call__RETRACTSHUTTLE(CII_OBJECT__VARIABLE* p_variable,
			  CII_OBJECT__ALARM* p_alarm)
{
	//
	return pLPx__OBJ_CTRL->Call__OBJECT(CMMD__RETRACTSHUTTLE);
}

int  CObj__LPx_STD::
Call__PADDLEUP(CII_OBJECT__VARIABLE* p_variable,
			  CII_OBJECT__ALARM* p_alarm)
{
	//
	return pLPx__OBJ_CTRL->Call__OBJECT(CMMD__PADDLEUP);
}

int  CObj__LPx_STD::
Call__PADDLEDOWN(CII_OBJECT__VARIABLE* p_variable,
			  CII_OBJECT__ALARM* p_alarm)
{
	//
	return pLPx__OBJ_CTRL->Call__OBJECT(CMMD__PADDLEDOWN);
}
    
// HALT -----
int  CObj__LPx_STD::
Call__HALT(CII_OBJECT__VARIABLE* p_variable,
			  CII_OBJECT__ALARM* p_alarm)
{
	//
	return pLPx__OBJ_CTRL->Call__OBJECT(CMMD__HALT);
}