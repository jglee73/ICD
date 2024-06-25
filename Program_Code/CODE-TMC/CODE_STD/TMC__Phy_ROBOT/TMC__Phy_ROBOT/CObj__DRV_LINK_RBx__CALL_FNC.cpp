#include "StdAfx.h"
#include "CObj__DRV_LINK_RBx.h"
#include "CObj__DRV_LINK_RBx__DEF.h"
#include "CObj__DRV_LINK_RBx__ALID.h"

#include "Macro_Function.h"


//-------------------------------------------------------------------------	
int  CObj__DRV_LINK_RBx
::Call__INIT(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	CString set_cmmd = _RBx_CMMD__HOME;

	return Fnc__CMMD_WAIT(set_cmmd, "","","");
}
int  CObj__DRV_LINK_RBx
::Call__HOME(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	CString set_cmmd = _RBx_CMMD__HOME;

	return Fnc__CMMD_WAIT(set_cmmd, "","","");
}

int  CObj__DRV_LINK_RBx
::Call__ROTATE(CII_OBJECT__VARIABLE* p_variable,
			   CII_OBJECT__ALARM* p_alarm,
			   const CString& arm_type,
			   const CString& stn_name,
			   const CString& stn_slot)
{
	// ...
	{
		CString act_name;
		act_name.Format("Rotate : (Arm-%s) -> (%s-%s)", arm_type,stn_name,stn_slot);

		Fnc__APP_LOG(act_name);
	}

	// ...
	CString wfr_sts;

		 if(arm_type.CompareNoCase(ARM_A) == 0)		wfr_sts = dCH__MON__ARM_A_MATERIAL_STATUS->Get__STRING();
	else if(arm_type.CompareNoCase(ARM_B) == 0)		wfr_sts = dCH__MON__ARM_B_MATERIAL_STATUS->Get__STRING();
	else											return -11;

	// ...
	CString set_cmmd;

	if(wfr_sts.CompareNoCase(STR__NONE) == 0)		set_cmmd = _RBx_CMMD__PREP_PICK;
	else											set_cmmd = _RBx_CMMD__PREP_PLACE;

	return Fnc__CMMD_WAIT(set_cmmd, arm_type,stn_name,stn_slot);
}
int  CObj__DRV_LINK_RBx
::Call__PICK(CII_OBJECT__VARIABLE* p_variable,
			 CII_OBJECT__ALARM* p_alarm,
			 const CString& arm_type,
			 const CString& stn_name,
			 const CString& stn_slot)
{
	// ...
	{
		CString act_name;
		act_name.Format("Pick : (%s-%s) -> (Arm-%s)", stn_name,stn_slot,arm_type);

		Fnc__APP_LOG(act_name);
	}

	// ...
	CString set_cmmd = _RBx_CMMD__PICK;

	return Fnc__CMMD_WAIT(set_cmmd, arm_type,stn_name,stn_slot);
}
int  CObj__DRV_LINK_RBx
::Call__PLACE(CII_OBJECT__VARIABLE* p_variable,
			  CII_OBJECT__ALARM* p_alarm,
			  const CString& arm_type,
			  const CString& stn_name,
			  const CString& stn_slot)
{
	// ...
	{
		CString act_name;
		act_name.Format("Place : (Arm-%s) -> (%s-%s)", arm_type,stn_name,stn_slot);

		Fnc__APP_LOG(act_name);
	}

	// ...
	CString set_cmmd = _RBx_CMMD__PLACE;

	return Fnc__CMMD_WAIT(set_cmmd, arm_type,stn_name,stn_slot);
}

int  CObj__DRV_LINK_RBx
::Call__WAFER_CHECK_ARM(CII_OBJECT__VARIABLE* p_variable,
			            CII_OBJECT__ALARM* p_alarm)
{

	return -1;
}

int  CObj__DRV_LINK_RBx
::Fnc__CMMD_WAIT(const CString& set_cmmd,
				 const CString& arm_type,
				 const CString& stn_name,
				 const CString& stn_slot)
{
	dEXT_CH__ROBOT_PARA_CMMD->Set__DATA(set_cmmd);

	// ...
	CStringArray l_para;

	if(set_cmmd.CompareNoCase(_RBx_CMMD__HOME) != 0)
	{
		l_para.Add(arm_type);
		l_para.Add(stn_name);
		l_para.Add(stn_slot);
	}

	if(pOBJ_CTRL__ATM_DRV->Call__OBJ_MODE(_CMMD__ROBOT, l_para) < 0)
	{
		return -11;
	}

	// ...
	{
		CString log_msg;
		CString log_bff;

		log_msg.Format(" * Check Start ... \n");

		log_bff.Format(" * iCFG__SIM_MODE <- [%1d] \n", iCFG__SIM_MODE);
		log_msg += log_bff;

		log_bff.Format(" * %s <- %s \n", 
					   sEXT_CH__ROBOT_CMMD_STS->Get__CHANNEL_NAME(),
					   sEXT_CH__ROBOT_CMMD_STS->Get__STRING());
		log_msg += log_bff;

		Fnc__APP_LOG(log_msg);
	}

	sEXT_CH__ROBOT_CMMD_STS->When__NOT_DATA(STR__RUN, STR__WAIT);

	// ...
	{
		CString log_msg;
		CString log_bff;

		log_msg.Format(" * Check End ... \n");

		log_bff.Format(" * iCFG__SIM_MODE <- [%1d] \n", iCFG__SIM_MODE);
		log_msg += log_bff;

		log_bff.Format(" * %s <- %s \n", 
					   sEXT_CH__ROBOT_CMMD_STS->Get__CHANNEL_NAME(),
					   sEXT_CH__ROBOT_CMMD_STS->Get__STRING());
		log_msg += log_bff;

		Fnc__APP_LOG(log_msg);
	}

	if(sEXT_CH__ROBOT_CMMD_STS->Check__DATA(STR__IDLE) > 0)		
	{
		return 1;
	}
	return -1;
}
