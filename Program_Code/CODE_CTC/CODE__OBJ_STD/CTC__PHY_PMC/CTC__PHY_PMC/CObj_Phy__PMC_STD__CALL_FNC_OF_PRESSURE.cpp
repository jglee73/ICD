#include "StdAfx.h"
#include "CObj_Phy__PMC_STD.h"
#include "CObj_Phy__PMC_STD__ALID.h"

#include "Macro_Function.h"


#include "CMacro_FA.h"
extern CMacro_FA mFA_Link;


//--------------------------------------------------------------------------------------
int  CObj_Phy__PMC_STD::Call__PUMP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM *p_alarm)
{
	int loop = -1;
	int flag;

	do
	{
		loop = -1;
		flag = xI_Module_Obj->Connect__Module_Obj("PUMP");

		if(flag > 0)
		{
			CString var_data;
			double ref_press;
			double cur_press;

			xEXT_CH_CFG__REF_VAC_PRESSURE->Get__DATA(var_data);
			ref_press = atof(var_data);

			xCH__PRESSURE_VALUE->Get__DATA(var_data);
			cur_press = atof(var_data);

			if(cur_press > ref_press)
			{
				CString alarm_msg;
				CString r_act;

				alarm_msg.Format("PM%1d's pressure (%.3f torr) must be less than %.3f torr.",
					iPMx_ID,cur_press,ref_press);

				p_alarm->Popup__ALARM_With_MESSAGE(ALID__PUMP_ERROR,alarm_msg,r_act);

				if(r_act.CompareNoCase("RETRY") == 0)
				{
					loop = 1;
				}
				else
				{
					flag = -1;
				}
			}	
		}

		Sleep(1);
	}
	while(loop > 0);

	return flag;
}
int  CObj_Phy__PMC_STD::Call__VENT(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM *p_alarm)
{
	int loop = -1;
	int flag;

	do
	{
		loop = -1;
		flag = xI_Module_Obj->Connect__Module_Obj("VENT");

		if(flag > 0)
		{
			CString var_data;
			double ref_press;
			double cur_press;

			xEXT_CH_CFG__REF_ATM_PRESSURE->Get__DATA(var_data);
			ref_press = atof(var_data);

			xCH__PRESSURE_VALUE->Get__DATA(var_data);
			cur_press = atof(var_data);

			if(cur_press < ref_press)
			{
				CString alarm_msg;
				CString r_act;

				alarm_msg.Format("PM%1d's pressure (%.3f torr) must be more than %.3f torr.",
					iPMx_ID,cur_press,ref_press);

				p_alarm->Popup__ALARM_With_MESSAGE(ALID__VENT_ERROR,alarm_msg,r_act);

				if(r_act.CompareNoCase("RETRY") == 0)
				{
					loop = 1;
				}
				else
				{
					flag = -1;
				}
			}	

			Sleep(1);
		}
	}
	while(loop > 0);

	return flag;
}
