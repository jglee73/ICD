#include "StdAfx.h"
#include "CObj__SIDE_STORAGE.h"


//-------------------------------------------------------------------------
CObj__SIDE_STORAGE::CObj__SIDE_STORAGE()
{

}
CObj__SIDE_STORAGE::~CObj__SIDE_STORAGE()
{

}

//-------------------------------------------------------------------------
int CObj__SIDE_STORAGE::__DEFINE__CONTROL_MODE(obj,l_mode)
{
	sObject_Name = obj;

	// ...
	{
		ADD__CTRL_VAR(sMODE__INIT, "INIT");
		ADD__CTRL_VAR(sMODE__ALL_WAFER_CLEAR, "ALL_WAFER.CLEAR");
	}
	return 1;
}
int CObj__SIDE_STORAGE::__DEFINE__VERSION_HISTORY(version)
{

	return 1;
}

int CObj__SIDE_STORAGE::__DEFINE__VARIABLE_STD(p_variable)
{
	DECLARE__STD_VARIABLE;

	// ...
	CString str_name;
	CString dsc_item_list;
	int i;

	// ...
	{
		str_name = "CST.EXIST.FLAG";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"EXIST NONE","");
		LINK__VAR_DIGITAL_CTRL(dCH__CST_EXIST_FLAG, str_name);

		//
		p_variable->Get__STD_DESCRIPTION(DSP__OBJ_STATUS, dsc_item_list);

		str_name = "OBJ.STATUS";
		STD__ADD_DIGITAL(str_name, dsc_item_list);
		LINK__VAR_DIGITAL_CTRL(dCH__OBJ_CSTATUS, str_name);
	}

	// ...
	{
		str_name = "CFG.SLOT.MAX";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "count", 0, 1, 30, "");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_SLOT_MAX, str_name);

		// 
		p_variable->Get__STD_DESCRIPTION(DSP__SLOT_STATUS,dsc_item_list);

		for(i=0;i<CFG_LPx__SLOT_SIZE;i++)
		{	
			str_name.Format("SLOT%002d.STATUS", i+1);
			STD__ADD_DIGITAL_WITH_X_OPTION(str_name, dsc_item_list,"");
			LINK__VAR_DIGITAL_CTRL(dCH__SLOT_STATUS[i], str_name);

			str_name.Format("SLOT%002d.TITLE", i+1);
			STD__ADD_STRING_WITH_X_OPTION(str_name,"");
			LINK__VAR_STRING_CTRL(sCH__SLOT_TITLE[i], str_name);
		}
	}

	return 1;
}
int CObj__SIDE_STORAGE::__DEFINE__ALARM(p_alarm)
{
	DECLARE__ALARM;
	
	return 1;
}

//-------------------------------------------------------------------------
int CObj__SIDE_STORAGE::__INITIALIZE__OBJECT(p_variable,p_ext_obj_create)
{
	// ...
	{
		dCH__OBJ_CSTATUS->Set__DATA("STANDBY");
	}
	return 1;
}

//-------------------------------------------------------------------------
int CObj__SIDE_STORAGE::__CALL__CONTROL_MODE(mode,p_debug,p_variable,p_alarm)
{
	int flag = -1;


	     IF__CTRL_MODE(sMODE__INIT)					flag = Call__INIT(p_variable,p_alarm);
	ELSE_IF__CTRL_MODE(sMODE__ALL_WAFER_CLEAR)		flag = Call__ALL_WAFER_CLEAR(p_variable,p_alarm);

	if((flag < 0)||(p_variable->Check__CTRL_ABORT() > 0))
	{

	}
	else
	{
	}

	return flag;
}
int CObj__SIDE_STORAGE::__CALL__MONITORING(id,p_variable,p_alarm)
{

	return 1;
}
