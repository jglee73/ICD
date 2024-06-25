#include "StdAfx.h"
#include "CObj__SIDE_STORAGE.h"
#include "CObj__SIDE_STORAGE__DEF.h"



//----------------------------------------------------------------------------------------------------
int  CObj__SIDE_STORAGE
::Call__INIT(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{

	return 1;
}

int  CObj__SIDE_STORAGE
::Call__ALL_WAFER_CLEAR(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	int i;

	for(i=0;i<CFG_LPx__SLOT_SIZE;i++)
	{	
		dCH__SLOT_STATUS[i]->Set__DATA("NONE");
		sCH__SLOT_TITLE[i]->Set__DATA("");
	}

	return 1;
}
