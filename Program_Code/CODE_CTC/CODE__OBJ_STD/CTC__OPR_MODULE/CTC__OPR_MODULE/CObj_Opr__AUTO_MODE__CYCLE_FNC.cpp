#include "StdAfx.h"
#include "CObj_Opr__AUTO_MODE.h"


#include "CMacro_FA.h"
extern CMacro_FA mFA_Link;


#define CYCLE_STEADY__COUNT				10


//------------------------------------------------------------------------------
void CObj_Opr__AUTO_MODE::Fnc__CYCLE_END(CII_OBJECT__ALARM *p_alarm)
{
	if(Check__CYCLE_MODE() < 0)
	{
		return;
	}

	mFA_Link.CJOB__ALL_ABORT();

	if(xSCH_MATERIAL_CTRL->Get__MATERIAL_PICK_COUNT() < 1)
	{
		return;
	}

	// ...
	GetLocalTime(&mCYCLE_END_TIME);

	CString title;
	CString msg;
	CString bff;
	CString var_data;

	title = "CYCLE TEST is completed !";
	msg = "Cycle Message";

	bff.Format("START : %00004d.%002d.%002d-%002d:%002d:%002d \n",
				mCYCLE_START_TIME.wYear,
				mCYCLE_START_TIME.wMonth, 
				mCYCLE_START_TIME.wDay,
				mCYCLE_START_TIME.wHour,
				mCYCLE_START_TIME.wMinute,
				mCYCLE_START_TIME.wSecond);
	msg += bff;

	bff.Format("END   : %00004d.%002d.%002d-%002d:%002d:%002d \n",
				mCYCLE_END_TIME.wYear,
				mCYCLE_END_TIME.wMonth, 
				mCYCLE_END_TIME.wDay,
				mCYCLE_END_TIME.wHour,
				mCYCLE_END_TIME.wMinute,
				mCYCLE_END_TIME.wSecond);
	msg += bff;
	msg += "\n";

	xCH_CFG__CYCLE_COUNT_PICK->Get__DATA(var_data);
	bff.Format("PICK_COUNT  : %s\n",var_data);
	msg += bff;

	xCH_CFG__CYCLE_COUNT_PLACE->Get__DATA(var_data);
	bff.Format("PLACE_COUNT : %s\n",var_data);
	msg += bff;

	//
	CStringArray l_option;
	CString r_act;

	l_option.RemoveAll();
	l_option.Add("YES");

	p_alarm->Popup__MESSAGE_BOX(title,msg,l_option,r_act);
}

int  CObj_Opr__AUTO_MODE::Fnc__CYCLE_START(CII_OBJECT__ALARM *p_alarm)
{
		 if(xCH_CFG__CYCLE_MODE->Check__DATA(STR__ENABLE)  > 0)		iCYCLE_MODE = CYCLE_MODE__ENABLE;
	else if(xCH_CFG__CYCLE_MODE->Check__DATA(STR__FOREVER) > 0)		iCYCLE_MODE = CYCLE_MODE__FOREVER;
	else															iCYCLE_MODE = CYCLE_MODE__DISABLE;

	if(Check__CYCLE_MODE() < 0)	
	{
		return 1;
	}

	iCYCLE_LOT_COUNT = 1;

	// ...
	CString title;
	CString msg;
	CStringArray l_option;
	CString r_act;

	title = "CYCLE MODE is Enabled";
	msg   = "Do you want to start job at the cycle mode ?";

	l_option.RemoveAll();
	l_option.Add(STR__YES);
	l_option.Add(STR__NO);

	p_alarm->Popup__MESSAGE_BOX(title,msg,l_option,r_act);

	if(r_act != STR__YES)
	{
		Seq__CLEAR_ALL_PORT(p_alarm,-1);

		mFA_Link.CJOB__ALL_ABORT();
		return -1;
	}

	// ...
	GetLocalTime(&mCYCLE_START_TIME);

	iCYCLE_REF_TICK = GetTickCount();
	dCYCLE_ELAPSE_SEC = 0.0;

	iCYCLE_STEADY_REF_TICK = GetTickCount();
	dCYCLE_STEADY_ELAPSE_SEC = 0.0;

	CString start_time;
	start_time.Format("%00004d.%002d.%002d-%002d:%002d:%002d",
						mCYCLE_START_TIME.wYear,
						mCYCLE_START_TIME.wMonth, 
						mCYCLE_START_TIME.wDay,
						mCYCLE_START_TIME.wHour,
						mCYCLE_START_TIME.wMinute,
						mCYCLE_START_TIME.wSecond);

	xCH_CFG__CYCLE_START_TIME->Set__DATA(start_time);
	xCH_CFG__CYCLE_ELAPSE_TIME->Set__DATA("0 sec");
	xCH_CFG__CYCLE_THROUGHPUT->Set__DATA("0");

	xCH_CFG__CYCLE_STEADY_ELAPSE_TIME->Set__DATA("0 sec");
	xCH_CFG__CYCLE_STEADY_THROUGHPUT->Set__DATA("0");
	return 1;
}			
void CObj_Opr__AUTO_MODE::Fnc__CYCLE_COUNT()
{
	if(Check__CYCLE_MODE() < 0)	
	{
		return;
	}

	// ...
	CString var_data;
	int cur_count;

	cur_count = xSCH_MATERIAL_CTRL->Get__MATERIAL_PICK_COUNT();
	if(iPICK_COUNT != cur_count)
	{
		iPICK_COUNT = cur_count;

		var_data.Format("%1d",cur_count);
		xCH_CFG__CYCLE_COUNT_PICK->Set__DATA(var_data);

		if(iCYCLE_MODE == CYCLE_MODE__ENABLE)
		{
			int cycle_limit;

			xCH_CFG__CYCLE_LIMIT->Get__DATA(var_data);
			cycle_limit = atoi(var_data);

			if(cycle_limit <= cur_count)
			{
				xCH__JOB_STATUS->Set__DATA(STR__END);
			}
		}
	}

	cur_count = xSCH_MATERIAL_CTRL->Get__MATERIAL_PLACE_COUNT();
	if(iPLACE_COUNT != cur_count)
	{
		iPLACE_COUNT = cur_count;

		var_data.Format("%1d",cur_count);
		xCH_CFG__CYCLE_COUNT_PLACE->Set__DATA(var_data);

		//.....
		{
			int cur_tick = GetTickCount();
			int dif_tick = cur_tick - iCYCLE_REF_TICK;
			iCYCLE_REF_TICK = cur_tick;

			if(dif_tick > 0)
			{
				dCYCLE_ELAPSE_SEC += (dif_tick / 1000.0); 
			}

			CString start_time;
			start_time.Format("%00004d.%002d.%002d-%002d:%002d:%002d",
								mCYCLE_START_TIME.wYear,
								mCYCLE_START_TIME.wMonth, 
								mCYCLE_START_TIME.wDay,
								mCYCLE_START_TIME.wHour,
								mCYCLE_START_TIME.wMinute,
								mCYCLE_START_TIME.wSecond);

			CString elapse_time;
			elapse_time.Format("%.1f sec",dCYCLE_ELAPSE_SEC);

			CString wafer_throughput;
			if(dCYCLE_ELAPSE_SEC >= 1.0)
				wafer_throughput.Format("%.1f",cur_count*3600.0/dCYCLE_ELAPSE_SEC);
			else
				wafer_throughput = "0";

			xCH_CFG__CYCLE_ELAPSE_TIME->Set__DATA(elapse_time);
			xCH_CFG__CYCLE_THROUGHPUT->Set__DATA(wafer_throughput);

			if(cur_count > CYCLE_STEADY__COUNT)
			{
				CString str_data;
				int count_limit;

				xCH_CFG__CYCLE_STEADY_COUNT_LIMIT->Get__DATA(str_data);
				count_limit = atoi(str_data);

				int steady_count = cur_count - CYCLE_STEADY__COUNT;

				if((steady_count <= count_limit)
				&& (xCH__JOB_STATUS->Check__DATA(STR__END) < 0))
				{
					int dif_tick = cur_tick - iCYCLE_STEADY_REF_TICK;
					iCYCLE_STEADY_REF_TICK = cur_tick;

					dCYCLE_STEADY_ELAPSE_SEC += (dif_tick / 1000.0); 

					str_data.Format("%.1f sec : %1d",dCYCLE_STEADY_ELAPSE_SEC,steady_count);
					xCH_CFG__CYCLE_STEADY_ELAPSE_TIME->Set__DATA(str_data);

					str_data.Format("%.1f",steady_count*3600.0/dCYCLE_STEADY_ELAPSE_SEC);
					xCH_CFG__CYCLE_STEADY_THROUGHPUT->Set__DATA(str_data);
				}
			}
			else if(cur_count <= CYCLE_STEADY__COUNT)
			{
				iCYCLE_STEADY_REF_TICK = GetTickCount();
				dCYCLE_STEADY_ELAPSE_SEC = 0;				

				xCH_CFG__CYCLE_STEADY_ELAPSE_TIME->Set__DATA("0.0 sec : 0");
				xCH_CFG__CYCLE_STEADY_THROUGHPUT->Set__DATA("0.0");
			}
		}

		// ...
	}
}
