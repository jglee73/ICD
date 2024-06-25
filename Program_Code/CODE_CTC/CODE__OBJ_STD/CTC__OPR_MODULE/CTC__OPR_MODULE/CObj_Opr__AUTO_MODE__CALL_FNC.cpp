#include "StdAfx.h"
#include "CObj_Opr__AUTO_MODE.h"


//-------------------------------------------------------------------------
int  CObj_Opr__AUTO_MODE
::Call__MAINT_MODE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	if(xCH__JOB_STATUS->Check__DATA("IDLE") < 0)
	{
		CString r_act;

		p_alarm->Popup__ALARM(ALID__SYSTEM_IN_PROCESS,r_act);
		return -1;
	}

	iMAINT_FLAG = 1;
	xCH__JOB_STATUS->Set__DATA("MAINT");

	while(p_variable->Check__CTRL_ABORT() < 0)
	{
		Sleep(300);
	}

	xCH__JOB_STATUS->Set__DATA("IDLE");
	iMAINT_FLAG = -1;
	return 1;
}
