#include "StdAfx.h"
#include "CObj__CHM_TwoManiFold.h"
#include "CObj__CHM_TwoManiFold__DEF.h"
#include "CObj__CHM_TwoManiFold__ALID.h"


// ...
int  CObj__CHM_TwoManiFold
::Check__OBJECT_BUSY(const CString& obj_sts)
{
	if(obj_sts.CompareNoCase(STR__AVAILABLE) == 0)		return -1;
	if(obj_sts.CompareNoCase(STR__ABORTED) == 0)		return -1;
	if(obj_sts.CompareNoCase(STR__IDLE) == 0)			return -1;

	return 1;
}

int  CObj__CHM_TwoManiFold::
Check__ALL_VLV_CLOSE(CII_OBJECT__VARIABLE* p_variable,
					 CII_OBJECT__ALARM* p_alarm,
					 const CString& act_name)
{
	DECLARE__EXT_CTRL(p_variable);

	// ...
	int loop_count = 0;

LOOP_RETRY:

	// ...
	CStringArray l_err_msg;
	CString err_msg;
	int i;

	// LBA
	if(dEXT_CH__LBA__SLIT_VALVE_STATUS->Check__DATA(STR__CLOSE) < 0)
	{
		err_msg = "LBA";
		l_err_msg.Add(err_msg);
	}

	// LBB
	if(dEXT_CH__LBB__SLIT_VALVE_STATUS->Check__DATA(STR__CLOSE) < 0)
	{
		err_msg = "LBB";
		l_err_msg.Add(err_msg);
	}

	// PMx
	for(i=0;i<m_nPM_LIMIT;i++)
	{
		if(dEXT_CH__CFG_PMx_EXIST_FLAG[i]->Check__DATA("YES") < 0)
		{
			continue;
		}

		if(dEXT_CH__CFG_PMx_SV_USE[i]->Check__DATA("NO") > 0)
		{
			continue;
		}

		if(dEXT_CH__PMx__SLIT_VALVE_STATUS[i]->Check__DATA(STR__CLOSE) < 0)
		{
			err_msg.Format("PM%1d",i+1);
			l_err_msg.Add(err_msg);
		}
	}

	if(l_err_msg.GetSize() > 0)
	{
		loop_count++;
		if(loop_count <= 10)
		{
			Sleep(100);
			
			goto LOOP_RETRY;
		}
	}

	if(l_err_msg.GetSize() > 0)
	{
		int alarm_id = ALID__SLIT_VALVE__NOT_CLOSE;
		CString alarm_msg;
		CString alarm_bff;
		CString r_act;

		alarm_msg.Format("Can not %s !\n",act_name);
		alarm_msg += "Please, close all slot-valve.\n";
		alarm_msg += "Check the following modules;\n";

		int limit = l_err_msg.GetSize();
		int i;

		for(i=0;i<limit;i++)
		{
			alarm_msg += l_err_msg[i];
			alarm_msg += "\n";
		}

		p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,alarm_msg,r_act);

		if(r_act.CompareNoCase(ACT__CHECK_RETRY) == 0)
		{
			goto LOOP_RETRY;
		}
		return -1;
	}

	return 1;
}
