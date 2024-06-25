#include "StdAfx.h"
#include "CObj__VAC_ROBOT_STD.h"
#include "CObj__VAC_ROBOT_STD__DEF.h"
#include "CObj__VAC_ROBOT_STD__ALID.h"

#include "Macro_Function.h"


//------------------------------------------------------------------------------------
int  CObj__VAC_ROBOT_STD::
Interlock__CHECK_PRESSURE_VAC(CII_OBJECT__ALARM* p_alarm,
							  const CString& stn_name,
							  const CString& act_name)
{
	double  ref__pressure;
	double  cur__pressure;
	CString var_data;

LOOP__RETRY:

	// TMC_CHM -----
	{
		int alarm_id = ALID__TM_CHM__NOT_VAC_PRESSURE;

		aEXT_CH__CFG_REF_VAC_PRESSURE->Get__DATA(var_data);
		ref__pressure = atof(var_data);

		aEXT_CH__TMC_CHM_PRESSURE_TORR->Get__DATA(var_data);
		cur__pressure = atof(var_data);

		if(cur__pressure > ref__pressure)
		{
			CString err_msg;
			CString err_bff;
			CString r_act;

			err_msg.Format("%s's (%.3f torr) Pressure must be less than (%.3f torr) Config VAC Pressure.\n",
							"TMC_CHM",
							cur__pressure,
							ref__pressure);

			err_bff.Format("Robot can not %s.\n",act_name);
			err_msg += err_bff;

			p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,err_msg,r_act);

			if(r_act.CompareNoCase(ACT__RETRY) == 0)		goto LOOP__RETRY;
			return -1;
		}
	}

	// LLx ...
	int ll_i = Macro__CHECK_LLx_INDEX(stn_name);
	if(ll_i >= 0)
	{
		CII__VAR_ANALOG_CTRL *p_lbx_pressure;
		p_lbx_pressure = aEXT_CH__LLx_PRESSURE_TORR[ll_i].Get__PTR();

		aEXT_CH__CFG_REF_VAC_PRESSURE->Get__DATA(var_data);
		ref__pressure = atof(var_data);

		p_lbx_pressure->Get__DATA(var_data);
		cur__pressure = atof(var_data);

		if(cur__pressure > ref__pressure)
		{
			int alarm_id = ALID__LLx__NOT_VAC_PRESSURE;
			CString err_msg;
			CString err_bff;
			CString r_act;

			err_msg.Format("%s's (%.3f torr) Pressure must be less than (%.3f torr) Config VAC Pressure.\n",
							stn_name,
							cur__pressure,
							ref__pressure);

			err_bff.Format("Robot can not %s.\n",act_name);
			err_msg += err_bff;

			p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,err_msg,r_act);

			if(r_act.CompareNoCase(ACT__RETRY) == 0)		goto LOOP__RETRY;
			return -1;
		}
	}

	// ...
	int pm_index = Macro__CHECK_PMx_INDEX(stn_name);
	if(pm_index >= 0)
	{
		aEXT_CH__PMx_PRESSURE_TORR[pm_index]->Get__DATA(var_data);
		cur__pressure = atof(var_data);

		aEXT_CH__CFG_REF_VAC_PRESSURE->Get__DATA(var_data);
		ref__pressure = atof(var_data);

		if(cur__pressure > ref__pressure)
		{
			int alarm_id = ALID__PMx__NOT_VAC_PRESSURE;
			CString err_msg;
			CString err_bff;
			CString r_act;

			err_msg.Format("%s's (%.3f torr) Pressure must be less than (%.3f torr) Config VAC Pressure.\n",
							stn_name,
							cur__pressure,
							ref__pressure);

			err_bff.Format("Robot can not %s.\n",act_name);
			err_msg += err_bff;

			p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,err_msg,r_act);

			if(r_act.CompareNoCase(ACT__RETRY) == 0)		goto LOOP__RETRY;
			return -1;
		}

	}

	return 1;
}

int  CObj__VAC_ROBOT_STD::
Interlock__CHECK_PRESSURE_ATM(CII_OBJECT__ALARM* p_alarm,
							  const CString& stn_name,
							  const CString& act_name)
{
	double  ref__pressure;
	double  cur__pressure;
	CString var_data;

LOOP__RETRY:

	// TMC_CHM -----
	{
		int alarm_id = ALID__TM_CHM__NOT_ATM_PRESSURE;

		aEXT_CH__CFG_REF_ATM_PRESSURE->Get__DATA(var_data);
		ref__pressure = atof(var_data);

		aEXT_CH__TMC_CHM_PRESSURE_TORR->Get__DATA(var_data);
		cur__pressure = atof(var_data);

		if(cur__pressure < ref__pressure)
		{
			CString err_msg;
			CString err_bff;
			CString r_act;

			err_msg.Format("%s's (%.3f torr) Pressure must be more than (%.3f torr) Config ATM Pressure.\n",
							"TMC_CHM",
							cur__pressure,
							ref__pressure);

			err_bff.Format("Robot can not %s.\n",act_name);
			err_msg += err_bff;

			p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,err_msg,r_act);

			if(r_act.CompareNoCase(ACT__RETRY) == 0)		goto LOOP__RETRY;
			return -1;
		}
	}

	// LLx ...
	int ll_i = Macro__CHECK_LLx_INDEX(stn_name);
	if(ll_i >= 0)
	{
		CII__VAR_ANALOG_CTRL *p_lbx_pressure;
		p_lbx_pressure = aEXT_CH__LLx_PRESSURE_TORR[ll_i].Get__PTR();

		aEXT_CH__CFG_REF_ATM_PRESSURE->Get__DATA(var_data);
		ref__pressure = atof(var_data);

		p_lbx_pressure->Get__DATA(var_data);
		cur__pressure = atof(var_data);

		if(cur__pressure < ref__pressure)
		{
			int alarm_id = ALID__LLx__NOT_ATM_PRESSURE;
			CString err_msg;
			CString err_bff;
			CString r_act;

			err_msg.Format("%s's (%.3f torr) Pressure must be more than (%.3f torr) Config ATM Pressure.\n",
							stn_name,
							cur__pressure,
							ref__pressure);

			err_bff.Format("Robot can not %s.\n",act_name);
			err_msg += err_bff;

			p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,err_msg,r_act);

			if(r_act.CompareNoCase(ACT__RETRY) == 0)		goto LOOP__RETRY;
			return -1;
		}
	}

	// ...
	int pm_index = Macro__CHECK_PMx_INDEX(stn_name);
	if(pm_index >= 0)
	{
		aEXT_CH__PMx_PRESSURE_TORR[pm_index]->Get__DATA(var_data);
		cur__pressure = atof(var_data);

		aEXT_CH__CFG_REF_ATM_PRESSURE->Get__DATA(var_data);
		ref__pressure = atof(var_data);

		if(cur__pressure < ref__pressure)
		{
			int alarm_id = ALID__PMx__NOT_ATM_PRESSURE;
			CString err_msg;
			CString err_bff;
			CString r_act;

			err_msg.Format("%s's (%.3f torr) Pressure must be more than (%.3f torr) Config ATM Pressure.\n",
							stn_name,
							cur__pressure,
							ref__pressure);

			err_bff.Format("Robot can not %s.\n",act_name);
			err_msg += err_bff;

			p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,err_msg,r_act);

			if(r_act.CompareNoCase(ACT__RETRY) == 0)		goto LOOP__RETRY;
			return -1;
		}
	}

	return 1;
}
