#include "StdAfx.h"
#include "CObj__DRV_LINK_ALx.h"
#include "CObj__DRV_LINK_ALx__DEF.h"
#include "CObj__DRV_LINK_ALx__ALID.h"

#include "Macro_Function.h"


//-------------------------------------------------------------------------	
// INIT -----
int  CObj__DRV_LINK_ALx
::Call__INIT(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	return Fnc__CMMD_WAIT(_ALx_CMMD__INIT, "");
}
int  CObj__DRV_LINK_ALx
::Call__HOME(CII_OBJECT__VARIABLE* p_variable,
			 CII_OBJECT__ALARM* p_alarm)
{
	return Fnc__CMMD_WAIT(_ALx_CMMD__HOME, "");
}

int  CObj__DRV_LINK_ALx
::Call__ALIGN(CII_OBJECT__VARIABLE* p_variable,
			  CII_OBJECT__ALARM* p_alarm)
{
	CString para_slot = dCH__PARA_SLOT->Get__STRING();

	CString para_angle;
	aCH__PARA_CCD_POS->Get__DATA(para_angle);

	if(Fnc__CMMD_WAIT(_ALx_CMMD__ALIGN, para_slot,para_angle) < 0)
	{
		return -11;
	}

	return pOBJ_CTRL__ALx_DRV->Call__OBJECT(_CMMD__ALx_RESULT);
}
int  CObj__DRV_LINK_ALx
::Call__ROTATE(CII_OBJECT__VARIABLE* p_variable,
			   CII_OBJECT__ALARM* p_alarm)
{
	CString para_slot = dCH__PARA_SLOT->Get__STRING();

	return Fnc__CMMD_WAIT(_ALx_CMMD__ROTATE, para_slot);
}
int  CObj__DRV_LINK_ALx
::Call__SCAN(CII_OBJECT__VARIABLE* p_variable,
			 CII_OBJECT__ALARM* p_alarm)
{
	CString para_slot = dCH__PARA_SLOT->Get__STRING();

	return Fnc__CMMD_WAIT(_ALx_CMMD__SCAN, para_slot);
}

int  CObj__DRV_LINK_ALx
::Fnc__CMMD_WAIT(const CString& set_cmmd,
				 const CString& para_slot,
				 const CString& para_angle)
{
	dEXT_CH__ALx_PARA_CMMD->Set__DATA(set_cmmd);

	if(para_slot != "")
	{
		CStringArray l_para;
		
		l_para.Add(para_slot);
		
		if(para_angle.GetLength() > 0)
			l_para.Add(para_angle);

		if(pOBJ_CTRL__ALx_DRV->Call__OBJ_MODE(_CMMD__ALx_CMMD, l_para) < 0)
		{
			return -11;
		}
	}
	else
	{
		if(pOBJ_CTRL__ALx_DRV->Call__OBJECT(_CMMD__ALx_CMMD) < 0)
		{
			return -12;
		}
	}

	sEXT_CH__ALx_CMMD_STS->When__NOT_DATA(STR__RUN, STR__WAIT);

	if(sEXT_CH__ALx_CMMD_STS->Check__DATA(STR__IDLE) > 0)		
	{
		return 1;
	}
	return -1;
}
