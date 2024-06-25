#include "StdAfx.h"
#include "CObj__ATM_RelianceATR.h"
#include "CObj__ATM_RelianceATR__DEF.h"
#include "CObj__ATM_RelianceATR__ALID.h"

#include "Macro_Function.h"


//-------------------------------------------------------------------------	
// INIT -----
int  CObj__ATM_RelianceATR
::Call__INIT(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	if(iActive_SIM > 0)
	{
		return Fnc__SIM_INIT(p_variable,p_alarm);
	}

	return Fnc__INIT(p_variable,p_alarm);;
}

int  CObj__ATM_RelianceATR
::Fnc__SIM_INIT(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	Sleep(1500);

	dCH__OTR_OUT_MON__ARM_A_ACT->Set__DATA(STR__RETRACT);
	dCH__OTR_OUT_MON__ARM_B_ACT->Set__DATA(STR__RETRACT);

	dCH__OTR_OUT_MON__TRG_MOVE->Set__DATA(STR__HOME);
	dCH__OTR_OUT_MON__TRG_ROTATE->Set__DATA(STR__HOME);

	return Fnc__ONLINE_CHECK(p_alarm, 0);
}
int  CObj__ATM_RelianceATR
::Fnc__INIT(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	int status = -1;
	CString log_msg;

	// ...
	{
		log_msg = "[ReInit] Start ...";
		Fnc__APP_LOG(log_msg);
	}
	// ...
	{
		doCH__COMMAND->Set__DATA("Reinit");

		status = Fnc__ONLINE_CHECK(p_alarm,0);
		if(status <= 0)		return -1;
	}

	// ...
	{
		log_msg = "[Fnc__ROBOT_INIT] Start ...";
		Fnc__APP_LOG(log_msg);
	}
	// ...
	{
		status = Fnc__ROBOT_INIT(p_variable,p_alarm);
		if(status > 0)
		{
			Set__ROBOT_ANI("HOME","HOME");
		}
	}

	if(status <= 0)		return -1;
	return 1;
}


// PICK -----
int  CObj__ATM_RelianceATR
::Call__PICK(CII_OBJECT__VARIABLE* p_variable,
			 CII_OBJECT__ALARM* p_alarm,
			 const CString& arm_type,
			 const CString& stn_name,
			  const CString& stn_slot)
{
	// ...
	{
		CString act_name;
		act_name.Format("pick from %s-%s to Arm-%s", stn_name,stn_slot,arm_type);

		Fnc__APP_LOG(act_name);
	}

	int r_ret = Fnc__PICK(p_variable,p_alarm, arm_type,stn_name,stn_slot);

	if(iActive_SIM > 0)
	{
		if(r_ret > 0)	Fnc__SIM_PICK_TIME(p_variable,p_alarm, arm_type,stn_name,stn_slot);
	}
	return r_ret;
}

int  CObj__ATM_RelianceATR
::Fnc__SIM_PICK_TIME(CII_OBJECT__VARIABLE* p_variable,
					 CII_OBJECT__ALARM* p_alarm,
					 const CString& arm_type,
					 const CString& stn_name,
					 const CString& stn_slot)
{
	SCX__TIMER_CTRL x_timer_ctrl;
	x_timer_ctrl->REGISTER__ABORT_OBJECT(sObject_Name);

	// ...
	double  cfg__pick_time = 1.0;
	CString var_data;

	if(Macro__CHECK_LPx_INDEX(stn_name) >= 0)
	{
		aCH__SIM_CFG__LPx_PICK_TIME->Get__DATA(var_data);
	}
	else if(stn_name.CompareNoCase(STR__AL1) == 0)
	{
		aCH__SIM_CFG__ALx_PICK_TIME->Get__DATA(var_data);
	}
	else if((stn_name.CompareNoCase(STR__LBA) == 0)
		 || (stn_name.CompareNoCase(STR__LBB) == 0))
	{
		aCH__SIM_CFG__LBx_PICK_TIME->Get__DATA(var_data);
	}

	cfg__pick_time = atof(var_data);
	if(x_timer_ctrl->WAIT(cfg__pick_time) < 0)
	{
		return -1;
	}

	return 1;
}
int  CObj__ATM_RelianceATR
::Fnc__PICK(CII_OBJECT__VARIABLE* p_variable,
			CII_OBJECT__ALARM* p_alarm,
			const CString& arm_type,
			const CString& stn_name,
			const CString& stn_slot)
{
	CString str__ccd_pos;
	CString var_data;

	aCH__PARA_AL1_CCD_POS->Get__DATA(str__ccd_pos);
	dEXT_CH__CFG_ALIGN_DEVICE->Get__DATA(var_data);

	if(stn_name.CompareNoCase(STR__AL1) == 0)
	{
		if(var_data.CompareNoCase("ATM_RB") == 0)
		{
			return Seq__ALIGN_PICK(p_variable,p_alarm, arm_type,stn_name,stn_slot,str__ccd_pos);
		}
	}

	return Seq__ROBOT_PICK(p_variable,p_alarm, "",arm_type,stn_name,stn_slot);
}


// PLACE -----
int  CObj__ATM_RelianceATR
::Call__PLACE(CII_OBJECT__VARIABLE* p_variable,
			  CII_OBJECT__ALARM* p_alarm,
			  const CString& arm_type,
			  const CString& stn_name,
			  const CString& stn_slot)
{
	// ...
	{
		CString act_name;
		act_name.Format("place from Arm-%s to %s-%s", arm_type,stn_name,stn_slot);

		Fnc__APP_LOG(act_name);
	}

	// ...
	int r_ret = Fnc__PLACE(p_variable,p_alarm,arm_type,stn_name,stn_slot);

	if(iActive_SIM > 0)
	{
		if(r_ret > 0)		Fnc__SIM_PLACE_TIME(p_variable,p_alarm,arm_type,stn_name,stn_slot);
	}
	return r_ret;
}

int  CObj__ATM_RelianceATR
::Fnc__SIM_PLACE_TIME(CII_OBJECT__VARIABLE* p_variable,
					  CII_OBJECT__ALARM* p_alarm,
					  const CString& arm_type,
					  const CString& stn_name,
					  const CString& stn_slot)
{
	SCX__TIMER_CTRL x_timer_ctrl;
	x_timer_ctrl->REGISTER__ABORT_OBJECT(sObject_Name);

	// ...
	double  cfg__place_time = 1.0;
	CString var_data;

	if(Macro__CHECK_LPx_INDEX(stn_name) >= 0)
	{
		aCH__SIM_CFG__LPx_PLACE_TIME->Get__DATA(var_data);
	}
	else if(stn_name.CompareNoCase(STR__AL1) == 0)
	{
		aCH__SIM_CFG__ALx_PLACE_TIME->Get__DATA(var_data);
	}
	else if((stn_name.CompareNoCase(STR__LBA) == 0)
		 || (stn_name.CompareNoCase(STR__LBB) == 0))
	{
		aCH__SIM_CFG__LBx_PLACE_TIME->Get__DATA(var_data);
	}

	cfg__place_time = atof(var_data);
	if(x_timer_ctrl->WAIT(cfg__place_time) < 0)	
	{
		return -1;
	}
	return 1;
}
int  CObj__ATM_RelianceATR
::Fnc__PLACE(CII_OBJECT__VARIABLE* p_variable,
		     CII_OBJECT__ALARM* p_alarm,
		     const CString& arm_type,
		     const CString& stn_name,
		     const CString& stn_slot)
{
	CString str__ccd_pos;
	CString var_data;

	aCH__PARA_AL1_CCD_POS->Get__DATA(str__ccd_pos);
	dEXT_CH__CFG_ALIGN_DEVICE->Get__DATA(var_data);

	/*
	if(stn_name.CompareNoCase(STR__AL1) == 0)
	{
		if(var_data.CompareNoCase("ATM_RB") == 0)
		{
			return Seq__ALIGN_PLACE(p_variable,p_alarm, arm_type,stn_name,stn_slot,str__ccd_pos);
		}
	}
	*/

	return Seq__ROBOT_PLACE(p_variable,p_alarm, arm_type,stn_name,stn_slot);
}


// ROTATE -----
int  CObj__ATM_RelianceATR
::Call__ROTATE(CII_OBJECT__VARIABLE* p_variable,
			   CII_OBJECT__ALARM* p_alarm,
			   const CString& arm_type,
			   const CString& stn_name,
			   const CString& stn_slot)
{
	CString var_data;
	dEXT_CH__CFG_ALIGN_DEVICE->Get__DATA(var_data);

	/*
	if(stn_name.CompareNoCase(STR__AL1) == 0)
	{
		if(var_data.CompareNoCase("ATM_RB") == 0)
		{
			// Aligner에서는 Front 할 일이 없다.
			return OBJ_AVAILABLE;
		}
	}
	*/

	if(iActive_SIM > 0)
	{
		return Fnc__SIM_ROTATE(p_variable,p_alarm,arm_type,stn_name,stn_slot);
	}
	return Fnc__ROTATE(p_variable,p_alarm,arm_type,stn_name,stn_slot);
}

int  CObj__ATM_RelianceATR
::Fnc__SIM_ROTATE(CII_OBJECT__VARIABLE* p_variable,
				  CII_OBJECT__ALARM* p_alarm,
				  const CString& arm_type,
				  const CString& stn_name,
				  const CString& stn_slot)
{
	if(dCH__OTR_OUT_MON__TRG_MOVE->Check__DATA(stn_name) > 0)
	{
		return 1;
	}

	// ...
	SCX__TIMER_CTRL x_timer_ctrl;
	x_timer_ctrl->REGISTER__ABORT_OBJECT(sObject_Name);

	double  cfg__rotate_time = 1.0;
	CString var_data;

	aCH__SIM_CFG__ROTATE_TIME->Get__DATA(var_data);
	cfg__rotate_time = atof(var_data);

	if(x_timer_ctrl->WAIT(cfg__rotate_time) < 0)
	{
		return -1; 
	}
	return 1;
}
int  CObj__ATM_RelianceATR
::Fnc__ROTATE(CII_OBJECT__VARIABLE* p_variable,
			  CII_OBJECT__ALARM* p_alarm,
			  const CString& arm_type,
			  const CString& stn_name,
			  const CString& stn_slot)
{

	return Seq__ROBOT_ROTATE(p_variable,p_alarm, arm_type,stn_name,stn_slot);
}


// VAC_ON -----
int  CObj__ATM_RelianceATR
::Call__VAC_ON(CII_OBJECT__VARIABLE* p_variable,
			   CII_OBJECT__ALARM* p_alarm)
{

	return Seq__ROBOT_VacOn(p_variable,p_alarm);
}


// VAC_OFF -----
int  CObj__ATM_RelianceATR
::Call__VAC_OFF(CII_OBJECT__VARIABLE* p_variable,
			    CII_OBJECT__ALARM* p_alarm)
{

	return Seq__ROBOT_VacOff(p_variable,p_alarm);
}


// AL_PICK -----
int  CObj__ATM_RelianceATR
::Call__AL_PICK(CII_OBJECT__VARIABLE* p_variable,
			    CII_OBJECT__ALARM* p_alarm)
{

	return 1;
}


// AL_ALIGN_PICK -----
int  CObj__ATM_RelianceATR
::Call__AL_ALIGN_PICK(CII_OBJECT__VARIABLE* p_variable,
					  CII_OBJECT__ALARM* p_alarm,
					  const CString& arm_type,
					  const CString& stn_name,
					  const CString& stn_slot)
{
LOOP__RETRY:

	if(stn_name.CompareNoCase(STR__AL1) != 0)
	{
		int alarm_id = ALID__INVALID__NOT_AL1_STATION;
		CString err_msg;
		CString r_act;

		err_msg.Format("Invalid Target Station not AL1 but %s.\n", stn_name);

		p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,err_msg,r_act);

		if(r_act.CompareNoCase(ACT__RETRY) == 0)
		{
			goto LOOP__RETRY;
		}
		return -1;			
	}

	// ...
	CString str__ccd_pos;
	aCH__PARA_AL1_CCD_POS->Get__DATA(str__ccd_pos);

	return Seq__ALIGN_PICK(p_variable,p_alarm, arm_type,stn_name,stn_slot,str__ccd_pos);
}

int  CObj__ATM_RelianceATR
::Call__PLACE_ALGNPICK(CII_OBJECT__VARIABLE* p_variable,
					   CII_OBJECT__ALARM* p_alarm,
					   const CString& arm_type,
					   const CString& stn_name,
					   const CString& stn_slot)
{
LOOP__RETRY:

	if(stn_name.CompareNoCase(STR__AL1) != 0)
	{
		int alarm_id = ALID__INVALID__NOT_AL1_STATION;
		CString err_msg;
		CString r_act;

		err_msg.Format("Invalid Target Station not AL1 but %s.\n",stn_name);

		p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,err_msg,r_act);

		if(r_act.CompareNoCase(ACT__RETRY) == 0)
		{
			goto LOOP__RETRY;
		}
		return -1;			
	}

	// ...
	CString str__ccd_pos;
	aCH__PARA_AL1_CCD_POS->Get__DATA(str__ccd_pos);

	int nRet = Seq__ALIGN_PLACE(p_variable,p_alarm, arm_type,stn_name,stn_slot,str__ccd_pos);
	if(nRet <= 0)		return OBJ_ABORT;

	return Seq__ALIGN_PICK(p_variable,p_alarm, arm_type,stn_name,stn_slot,str__ccd_pos);
}

// ALIGN PLACE -----
int  CObj__ATM_RelianceATR
::Call__ALIGN_PLACE(CII_OBJECT__VARIABLE* p_variable,
				    CII_OBJECT__ALARM* p_alarm,
				    const CString& arm_type,
				    const CString& stn_name,
				    const CString& stn_slot)
{
LOOP__RETRY:

	if(stn_name.CompareNoCase(STR__AL1) != 0)
	{
		int alarm_id = ALID__INVALID__NOT_AL1_STATION;
		CString err_msg;
		CString r_act;

		err_msg.Format("Invalid Target Station not AL1 but %s.\n",stn_name);

		p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,err_msg,r_act);

		if(r_act.CompareNoCase(ACT__RETRY) == 0)
		{
			goto LOOP__RETRY;
		}
		return -1;			
	}

	// ...
	CString str__ccd_pos;
	aCH__PARA_AL1_CCD_POS->Get__DATA(str__ccd_pos);

	return Seq__ALIGN_PLACE(p_variable,p_alarm, arm_type,stn_name,stn_slot,str__ccd_pos);
}


// AL_ALIGN -----
int  CObj__ATM_RelianceATR
::Call__AL_ALIGN(CII_OBJECT__VARIABLE* p_variable,
			     CII_OBJECT__ALARM* p_alarm)
{
	CString str__ccd_pos;
	aCH__PARA_AL1_CCD_POS->Get__DATA(str__ccd_pos);

	if(iActive_SIM > 0)
	{
		return Seq__SIM_ALALIGN(p_variable,p_alarm, str__ccd_pos);
	}
	return Seq__ALALIGN(p_variable,p_alarm, str__ccd_pos);
}

// AL_ALIGN INIT -----
int  CObj__ATM_RelianceATR
::Call__AL_ALIGN_INIT(CII_OBJECT__VARIABLE* p_variable,
					  CII_OBJECT__ALARM* p_alarm)
{
	return Fnc__ALIGNER_INIT(p_variable, p_alarm);
}

// AL_ALIGN RESULT -----
int  CObj__ATM_RelianceATR
::Call__AL_ALIGN_RESULT(CII_OBJECT__VARIABLE* p_variable,
					    CII_OBJECT__ALARM* p_alarm)
{
	return Seq__ALALIGN_RESULT(p_variable, p_alarm);
}


// AL_SCAN
int  CObj__ATM_RelianceATR
::Call__AL_SCAN(CII_OBJECT__VARIABLE* p_variable,
			    CII_OBJECT__ALARM* p_alarm)
{
	return Seq__AL_SCAN(p_variable,p_alarm);
}


// AL_HOME
int  CObj__ATM_RelianceATR
::Call__AL_HOME(CII_OBJECT__VARIABLE* p_variable,
			    CII_OBJECT__ALARM* p_alarm)
{
	return Seq__AL_HOME(p_variable,p_alarm);
}


// AL_RESET
int  CObj__ATM_RelianceATR
::Call__AL_RESET(CII_OBJECT__VARIABLE* p_variable,
			     CII_OBJECT__ALARM* p_alarm)
{
	return Seq__AL_RESET(p_variable,p_alarm);
}

// MAP
int  CObj__ATM_RelianceATR
::Call__MAP(CII_OBJECT__VARIABLE* p_variable,
		    CII_OBJECT__ALARM* p_alarm)
{
	return Seq__MAP(p_variable, p_alarm);
}

// Extend
int  CObj__ATM_RelianceATR
::Call__EXTEND(CII_OBJECT__VARIABLE* p_variable,
			   CII_OBJECT__ALARM* p_alarm,
			   const CString& arm_type,
			   const CString& stn_name,
			   const CString& stn_slot)
{
	return Seq__EXTEND(p_variable, p_alarm, arm_type, stn_name, stn_slot);
}

// Retract
int  CObj__ATM_RelianceATR
::Call__RETRACT(CII_OBJECT__VARIABLE* p_variable,
			    CII_OBJECT__ALARM* p_alarm,
			    const CString& arm_type,
			    const CString& stn_name,
			    const CString& stn_slot)
{
	return Seq__RETRACT(p_variable, p_alarm, arm_type, stn_name, stn_slot);
}

// GoUp
int  CObj__ATM_RelianceATR
::Call__GOUP(CII_OBJECT__VARIABLE* p_variable,
		     CII_OBJECT__ALARM* p_alarm,
		     const CString& arm_type,
		     const CString& stn_name,
		     const CString& stn_slot)
{
	return Seq__GOUP(p_variable, p_alarm, arm_type, stn_name, stn_slot);
}

// GoDown
int  CObj__ATM_RelianceATR
::Call__GODOWN(CII_OBJECT__VARIABLE* p_variable,
			   CII_OBJECT__ALARM* p_alarm,
			   const CString& arm_type,
			   const CString& stn_name,
			   const CString& stn_slot)
{
	return Seq__GODOWN(p_variable, p_alarm, arm_type, stn_name, stn_slot);
}

// Home
int  CObj__ATM_RelianceATR
::Call__HOME(CII_OBJECT__VARIABLE* p_variable,
		     CII_OBJECT__ALARM* p_alarm,
			 const int req__vac_off)
{
	if(req__vac_off > 0)
	{
		Seq__ROBOT_VacOff(p_variable,p_alarm);
	}
	
	return Seq__HOME(p_variable, p_alarm);
}

// LoadPort Map
int  CObj__ATM_RelianceATR
::Call__LP_MAP(CII_OBJECT__VARIABLE* p_variable,
			   CII_OBJECT__ALARM* p_alarm,
			   const CString& stn_name)
{
	return Seq__LP_MAP(p_variable, p_alarm, stn_name);
}
