#include "StdAfx.h"
#include "CObj__ATM_RelianceATR.h"
#include "CObj__ATM_RelianceATR__DEF.h"
#include "CObj__ATM_RelianceATR__ALID.h"

#include "math.h"
#include "CCommon_DEF.h"
#include "Macro_Function.h"


//------------------------------------------------------------------------------------
int  CObj__ATM_RelianceATR
::Is__LOCAL(CII_OBJECT__ALARM* p_alarm)
{

	return 1;
}
int  CObj__ATM_RelianceATR
::Is__OFFLINE(CII_OBJECT__ALARM* p_alarm)
{
LOOP_RETRY:

	if(diCH__COMM_STS->Check__DATA(STR__OFFLINE) > 0)
	{
		int alarm_id = ALID__OFFLINE_ALARM_ACT;
		CString r_act;

		p_alarm->Popup__ALARM(alarm_id,r_act);

		if(r_act.CompareNoCase("RETRY") == 0)
		{
			goto LOOP_RETRY;
		}
		return -1;
	}

	return 1;
}

int  CObj__ATM_RelianceATR
::Get__CFG_STN_NUM(const CString& stn_name, CString& stn_id)
{
	CString cfg_stn_num;

		 if(stn_name.CompareNoCase(STR__AL1) == 0)		    aCH__CFG_AL1_STN_NUM->Get__DATA(cfg_stn_num);
	else if(stn_name.CompareNoCase(STR__LBA) == 0)		    aCH__CFG_LBA_STN_NUM->Get__DATA(cfg_stn_num);
	else if(stn_name.CompareNoCase(STR__LBB) == 0)		    aCH__CFG_LBB_STN_NUM->Get__DATA(cfg_stn_num);
	else if(stn_name.CompareNoCase(STR__BUFF1) == 0)	    aCH__CFG_BUF1_STN_NUM->Get__DATA(cfg_stn_num);
	else if(stn_name.CompareNoCase(STR__BUFF2) == 0)	    aCH__CFG_BUF2_STN_NUM->Get__DATA(cfg_stn_num);
	else if(stn_name.CompareNoCase(STR__LP1) == 0)		    aCH__CFG_LPx_STN_NUM[INT__LP1]->Get__DATA(cfg_stn_num);
	else if(stn_name.CompareNoCase(STR__LP2) == 0)		    aCH__CFG_LPx_STN_NUM[INT__LP2]->Get__DATA(cfg_stn_num);
	else if(stn_name.CompareNoCase(STR__LP3) == 0)		    aCH__CFG_LPx_STN_NUM[INT__LP3]->Get__DATA(cfg_stn_num);
	else if(stn_name.CompareNoCase(STR__LP4) == 0)		    aCH__CFG_LPx_STN_NUM[INT__LP4]->Get__DATA(cfg_stn_num);
	else if(stn_name.CompareNoCase(STR__LP5) == 0)		    aCH__CFG_LPx_STN_NUM[INT__LP5]->Get__DATA(cfg_stn_num);
	else		
	{
		return -1;
	}

	stn_id = cfg_stn_num;
	return 1;
}


//------------------------------------------------------------------------------------
int  CObj__ATM_RelianceATR
::Seq__ALIGN_PICK(CII_OBJECT__VARIABLE* p_variable,
				  CII_OBJECT__ALARM* p_alarm,
				  const CString& arm_type,
				  const CString& stn_name,
				  const CString& stn_slot,
				  const CString& ccd_pos)
{
	CString log_msg;
	CString log_bff;

	// ...
	{
		log_msg = "Seq__ALIGN_PICK() : Started ... \n";

		log_bff.Format(" *** %s <- %s \n",
			           dEXT_CH__CFG_AL1_EXIST_FLAG->Get__CHANNEL_NAME(),
					   dEXT_CH__CFG_AL1_EXIST_FLAG->Get__STRING());
		log_msg += log_bff;

		log_bff.Format("  * arm_type <- %s \n", arm_type);
		log_msg += log_bff;
		log_bff.Format("  * stn_name <- %s \n", stn_name);
		log_msg += log_bff;
		log_bff.Format("  * stn_slot <- %s \n", stn_slot);
		log_msg += log_bff;
		log_bff.Format("  * ccd_pos  <- %s \n", ccd_pos);
		log_msg += log_bff;

		Fnc__APP_LOG(log_msg);
	}

LOOP_RETRY:

	if(dEXT_CH__CFG_AL1_EXIST_FLAG->Check__DATA(STR__YES) > 0)
	{
		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -1;
		}

		// ...
		{
			aoCH__ANGLE->Set__DATA(ccd_pos);
			doCH__COMMAND->Set__DATA("ALLoadPP");				// AL Setpoint

			int status = Fnc__ONLINE_CHECK(p_alarm,1);
				
			     if(status == 0)		goto LOOP_RETRY;
			else if(status  < 0)		return -1;	
		}

		// ...
		{
			doCH__COMMAND->Set__DATA("ALSetPP");				// AL Setpoint

			int status = Fnc__ONLINE_CHECK(p_alarm,1);

				 if(status == 0)		goto LOOP_RETRY;
			else if(status  < 0)		return -1;	
		}

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -1;
		}

		return Seq__ROBOT_PICK(p_variable,p_alarm, "AlignPick",arm_type,stn_name,stn_slot);
	}

	return -1;
}

//------------------------------------------------------------------------------------
int  CObj__ATM_RelianceATR
::Seq__ALIGN_PLACE(CII_OBJECT__VARIABLE* p_variable,
				   CII_OBJECT__ALARM* p_alarm,
				   const CString& arm_type,
				   const CString& stn_name,
				   const CString& stn_slot,
				   const CString& ccd_pos)
{
	if(dEXT_CH__CFG_AL1_EXIST_FLAG->Check__DATA(STR__YES) > 0)
	{
		return Seq__ROBOT_PLACE_ALIGN(p_variable,p_alarm, arm_type,stn_name,stn_slot);
	}
	return -1;
}

int  CObj__ATM_RelianceATR
::Set_IO__ARM_TYPE(const CString& arm_type,
				   const CString& stn_name,
				   const CString& stn_slot)
{
	CString stn_id;

	if(Get__CFG_STN_NUM(stn_name, stn_id) < 0)
	{
		return -1;
	}

	doCH__STN->Set__DATA(stn_id);
	doCH__SLOT->Set__DATA(stn_slot);
	doCH__ARM->Set__DATA(arm_type);
	return 1;
}

int  CObj__ATM_RelianceATR
::Set_IO__ARM_TYPE(const CString& stn_name)
{
	CString stn_id;

	if(Get__CFG_STN_NUM(stn_name,stn_id) < 0)
	{
		return -1;
	}

	doCH__STN->Set__DATA(stn_id);
	return 1;
}

int  CObj__ATM_RelianceATR
::Seq__ROBOT_PICK(CII_OBJECT__VARIABLE* p_variable,
				  CII_OBJECT__ALARM* p_alarm,
				  const CString& act_cmmd,
				  const CString& arm_type,
				  const CString& stn_name,
				  const CString& stn_slot)
{
LOOP_RETRY:

	// ...
	int nDrv_Cmd_Ret = 0;
	CString str_log;

	if(stn_name.CompareNoCase(STR__AL1) == 0)
	{
		int nRet = Seq__ALALIGN_RESULT(p_variable, p_alarm);

		     if(nRet == OBJ_RETRY)		goto LOOP_RETRY;
		else if(nRet == OBJ_ABORT)		return -1;
	}

	if(Set_IO__ARM_TYPE(arm_type,stn_name,stn_slot) < 0)
	{
		return -1;
	}

	// ...
	{
		// STRT: Start retract location
		doCH__STRT->Set__DATA("Normal");
	
		// ENRT: End retract location
		doCH__ENRT->Set__DATA("Normal");
	}

	// Align + Pick
	if(act_cmmd.CompareNoCase("AlignPick") == 0)
	{
		nDrv_Cmd_Ret = doCH__COMMAND->Set__DATA("AlgnPick");

		str_log.Format("Set Drv Cmd:%s, return :%d", act_cmmd, nDrv_Cmd_Ret);
		Fnc__APP_LOG(str_log);
		
		if(nDrv_Cmd_Ret < 0)
		{
			int alarm_id = ALID__ALGN_AND_PICK_TIMEOUT_ALARM;
			CString err_msg;
			CString r_act;

			err_msg = "1. Check the Timeout Config Value in the Config Page. \n";
			
			p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,err_msg,r_act);
			return OBJ_ABORT;
		}

		// ...
		{
			int status = Fnc__ONLINE_CHECK(p_alarm,1);

				 if(status == 0)		goto LOOP_RETRY;
			else if(status <  0)		return -1;	

			if(p_variable->Check__CTRL_ABORT() > 0)
			{
				return -1;
			}
		}

		doCH__COMMAND->Set__DATA("ALResult");			// AL Setpoint
	}
	// delta Pick only
	else if(act_cmmd.CompareNoCase("Pick") == 0)
	{
		doCH__COMMAND->Set__DATA("ALPick");
	}
	else											
	{
		doCH__COMMAND->Set__DATA("Pick");
	}

	// ...
	{
		int status = Fnc__ONLINE_CHECK(p_alarm,1);
			 
			 if(status == 0)		goto LOOP_RETRY;
		else if(status <  0)		return -1;			// abort

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -1;
		}
	}

	if(iActive_SIM > 0)
	{
		dCH__MON_ARM_A_MATERIAL_STATUS->Set__DATA(STR__EXIST);
	}
	else
	{
		CString log_msg;	
		CString log_bff;	

		// ...
		{
			log_msg = "\n";

			log_bff.Format("%s <- %s \n", 
						   dCH__CFG_WAFER_DRIVER_CHECK->Get__VARIABLE_NAME(),
	   			   		   dCH__CFG_WAFER_DRIVER_CHECK->Get__STRING());
			log_msg += log_bff;
		}

		if(dCH__CFG_WAFER_DRIVER_CHECK->Check__DATA(STR__YES) > 0)
		{
			CString str__arm_wfr = diCH__ARM_A_WFR_MANUAL->Get__STRING();
	
				 if(str__arm_wfr.CompareNoCase(STR__Present) == 0)		dCH__MON_ARM_A_MATERIAL_STATUS->Set__DATA(STR__EXIST);
			else if(str__arm_wfr.CompareNoCase(STR__Absent)  == 0)		dCH__MON_ARM_A_MATERIAL_STATUS->Set__DATA(STR__NONE);
			else														dCH__MON_ARM_A_MATERIAL_STATUS->Set__DATA(STR__UNKNOWN);

			// ...
			{
				log_bff.Format("  * %s <- %s \n", 
							   diCH__ARM_A_WFR_MANUAL->Get__VARIABLE_NAME(),
							   str__arm_wfr);
				log_msg += log_bff;
			}
		}

		Fnc__APP_LOG(log_msg);
	}

	return 1;
}

int  CObj__ATM_RelianceATR
::Seq__ROBOT_PLACE(CII_OBJECT__VARIABLE* p_variable,
				   CII_OBJECT__ALARM* p_alarm,
				   const CString& arm_type,
				   const CString& stn_name,
				   const CString& stn_slot)
{
LOOP_RETRY:

	if(Set_IO__ARM_TYPE(arm_type,stn_name,stn_slot) < 0)
	{
		return -1;
	}

	// ...
	{
		// STRT: Start retract location
		doCH__STRT->Set__DATA("Normal");
	
		// ENRT: End retract location
		doCH__ENRT->Set__DATA("Normal");
	}

	doCH__COMMAND->Set__DATA("Place");

	// ...
	{
		int status = Fnc__ONLINE_CHECK(p_alarm,1);

			 if(status == 0)		goto LOOP_RETRY;
		else if(status <  0)		return -1;				// abort

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -1;
		}
	}

	if(iActive_SIM > 0)
	{
		dCH__MON_ARM_A_MATERIAL_STATUS->Set__DATA(STR__NONE);
	}
	else
	{
		CString str__arm_wfr = diCH__ARM_A_WFR_MANUAL->Get__STRING();

			 if(str__arm_wfr.CompareNoCase(STR__Present) == 0)		dCH__MON_ARM_A_MATERIAL_STATUS->Set__DATA(STR__EXIST);
		else if(str__arm_wfr.CompareNoCase(STR__Absent)  == 0)		dCH__MON_ARM_A_MATERIAL_STATUS->Set__DATA(STR__NONE);
		else														dCH__MON_ARM_A_MATERIAL_STATUS->Set__DATA(STR__UNKNOWN);

		// ...
		{
			CString log_msg;	
			CString log_bff;	

			log_msg = "\n";

			log_bff.Format("  * %s <- %s \n", 
						   diCH__ARM_A_WFR_MANUAL->Get__VARIABLE_NAME(),
						   str__arm_wfr);
			log_msg += log_bff;

			Fnc__APP_LOG(log_msg);
		}
	}
	return 1;
}

int  CObj__ATM_RelianceATR
::Seq__ROBOT_PLACE_ALIGN(CII_OBJECT__VARIABLE* p_variable,
						 CII_OBJECT__ALARM* p_alarm,
						 const CString& arm_type,
						 const CString& stn_name,
						 const CString& stn_slot)
{
LOOP_RETRY:

	if(Set_IO__ARM_TYPE(arm_type,stn_name,stn_slot) < 0)
	{
		return -1;
	}

	// ...
	{
		// STRT: Start retract location
		doCH__STRT->Set__DATA("Normal");
	
		// ENRT: End retract location
		doCH__ENRT->Set__DATA("Normal");
	}

	doCH__COMMAND->Set__DATA("PlaceAlgn");

	// ...
	{
		int status = Fnc__ONLINE_CHECK(p_alarm,1);

			 if(status == 0)		goto LOOP_RETRY;
		else if(status <  0)		return -1;				// abort

		if(p_variable->Check__CTRL_ABORT() > 0)	
		{
			return -1;
		}
	}

	return 1;
}

int  CObj__ATM_RelianceATR
::Seq__ROBOT_ROTATE(CII_OBJECT__VARIABLE* p_variable,
				    CII_OBJECT__ALARM* p_alarm,
				    const CString& arm_type,
				    const CString& stn_name,
				    const CString& stn_slot)
{
LOOP_RETRY:

	// ...
	{
		CString str_wafer = dCH__MON_ARM_A_MATERIAL_STATUS->Get__STRING();

		// ...
		{
			CString log_msg;
		
			log_msg.Format("Wafer state : %s ", str_wafer);
		
			Fnc__APP_LOG(log_msg);
		}

		// ...
		{
			int status = Fnc__ONLINE_CHECK(p_alarm,0);

				 if(status == 0)		goto LOOP_RETRY;
			else if(status  < 0)		return -1;				// abort

			if(p_variable->Check__CTRL_ABORT() > 0)
			{
				return -1;
			}
		}	

		// ...
		{
			if(str_wafer.CompareNoCase(STR__NONE) == 0)		doCH__ZPOS->Set__DATA("DN");
			else											doCH__ZPOS->Set__DATA("UP");
		}

		doCH__RPOS->Set__DATA("RE");	// "RE" default
	}

	if(Set_IO__ARM_TYPE(arm_type,stn_name,stn_slot) < 0)
	{
		return -1;
	}

	doCH__COMMAND->Set__DATA("Goto");

	// ...
	{
		int status = Fnc__ONLINE_CHECK(p_alarm,1);

			 if(status == 0)		goto LOOP_RETRY;
		else if(status <  0)		return -1;				// abort

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -1;
		}
	}
	return 1;
}

int  CObj__ATM_RelianceATR
::Seq__ROBOT_VacOn(CII_OBJECT__VARIABLE* p_variable,
				   CII_OBJECT__ALARM* p_alarm)
{
LOOP_RETRY:

	doCH__VACUUM->Set__DATA("ON");
	doCH__COMMAND->Set__DATA("SetVac");

	// ...
	int status = Fnc__ONLINE_CHECK(p_alarm,1);
	
		 if(status == 0)		goto LOOP_RETRY;
	else if(status <  0)		return -1;			// abort

	if(p_variable->Check__CTRL_ABORT() > 0)
	{
		return -1;
	}
	return 1;
}

int  CObj__ATM_RelianceATR
::Seq__ROBOT_VacOff(CII_OBJECT__VARIABLE* p_variable,
				    CII_OBJECT__ALARM* p_alarm)
{
LOOP_RETRY:

	// ...
	{
		Fnc__APP_LOG("Seq__ROBOT_VacOff : Started ...");
	}

	doCH__VACUUM->Set__DATA("OFF");
	doCH__COMMAND->Set__DATA("SetVac");

	// ...
	int status = Fnc__ONLINE_CHECK(p_alarm,1);

		 if(status == 0)		goto LOOP_RETRY;
	else if(status <  0)		return -1;			// abort

	if(p_variable->Check__CTRL_ABORT() > 0)
	{
		return -1;
	}
	return 1;
}

int  CObj__ATM_RelianceATR
::Seq__AL_HOME(CII_OBJECT__VARIABLE* p_variable,
			   CII_OBJECT__ALARM* p_alarm)
{
LOOP_RETRY:

	if(dEXT_CH__CFG_AL1_EXIST_FLAG->Check__DATA("YES") > 0)
	{
		doCH__COMMAND->Set__DATA("ALHome");

		// ...
		int status = Fnc__ONLINE_CHECK(p_alarm,1);

			 if(status == 0)		goto LOOP_RETRY;
		else if(status <  0)		return -1;			// abort

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -1;
		}
	}

	return 1;
}
int  CObj__ATM_RelianceATR
::Seq__AL_SCAN(CII_OBJECT__VARIABLE* p_variable,
			   CII_OBJECT__ALARM* p_alarm)
{
LOOP_RETRY:

	if(dEXT_CH__CFG_AL1_EXIST_FLAG->Check__DATA("YES") > 0)
	{
		doCH__COMMAND->Set__DATA("ALScan");

		// ...
		int status = Fnc__ONLINE_CHECK(p_alarm,1);

			 if(status == 0)		goto LOOP_RETRY;
		else if(status <  0)		return -1;			

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -1;
		}
	}

	return 1;
}
int  CObj__ATM_RelianceATR
::Seq__AL_RESET(CII_OBJECT__VARIABLE* p_variable,
			    CII_OBJECT__ALARM* p_alarm)
{
LOOP_RETRY:

	if(dEXT_CH__CFG_AL1_EXIST_FLAG->Check__DATA("YES") > 0)
	{
		doCH__COMMAND->Set__DATA("ALReset");

		// ...
		int status = Fnc__ONLINE_CHECK(p_alarm,1);

			 if(status == 0)		goto LOOP_RETRY;
		else if(status <  0)		return -1;			

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -1;
		}
	}

	return 1;
}

int  CObj__ATM_RelianceATR
::Seq__MAP(CII_OBJECT__VARIABLE* p_variable,
		   CII_OBJECT__ALARM* p_alarm)
{
	if(dEXT_CH__CFG_AL1_EXIST_FLAG->Check__DATA("YES") > 0)
	{
		if(Fnc__ROBOT_MAP(p_variable,p_alarm) < 0)
		{
			return -1;
		}
	}
	else
	{
		CString r_act;
		CString err_msg;
		
		int alarm_id = ALID__ATMRB_MODULE_NO_EXIST_ERROR;
		err_msg.Format("No Exist Module");

		p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,err_msg,r_act);

		if(r_act.CompareNoCase("ABORT") == 0)
		{
			return -1;
		}
		return -1;
	}

	return 1;
}

int  CObj__ATM_RelianceATR
::Seq__EXTEND(CII_OBJECT__VARIABLE* p_variable,
			  CII_OBJECT__ALARM* p_alarm,
			  const CString& arm_type,
			  const CString& stn_name,
			  const CString& stn_slot)
{
	return Fnc__ROBOT_EXTEND(p_variable,p_alarm, arm_type,stn_name,stn_slot);
}
int  CObj__ATM_RelianceATR
::Seq__RETRACT(CII_OBJECT__VARIABLE* p_variable,
			   CII_OBJECT__ALARM* p_alarm,
			   const CString& arm_type,
			   const CString& stn_name,
			   const CString& stn_slot)
{
	return Fnc__ROBOT_RETRACT(p_variable,p_alarm, arm_type,stn_name,stn_slot);
}

int  CObj__ATM_RelianceATR
::Seq__GOUP(CII_OBJECT__VARIABLE* p_variable,
			CII_OBJECT__ALARM* p_alarm,
		    const CString& arm_type,
		    const CString& stn_name,
		    const CString& stn_slot)
{
	return Fnc__ROBOT_GOUP(p_variable,p_alarm, arm_type,stn_name,stn_slot);
}
int  CObj__ATM_RelianceATR
::Seq__GODOWN(CII_OBJECT__VARIABLE* p_variable,
			  CII_OBJECT__ALARM* p_alarm,
			  const CString& arm_type,
			  const CString& stn_name,
			  const CString& stn_slot)
{
	return Fnc__ROBOT_GODOWN(p_variable,p_alarm, arm_type,stn_name,stn_slot);
}

int  CObj__ATM_RelianceATR
::Seq__HOME(CII_OBJECT__VARIABLE* p_variable,
			CII_OBJECT__ALARM* p_alarm)
{
	// ...
	{
		Fnc__APP_LOG("Seq__HOME : Started ...");
	}

	if(Fnc__ROBOT_HOME(p_variable, p_alarm) < 0)
	{
		return -1;
	}

	return 1;
}

int  CObj__ATM_RelianceATR
::Seq__LP_MAP(CII_OBJECT__VARIABLE* p_variable,
			  CII_OBJECT__ALARM* p_alarm,
			  const CString& stn_name)
{
	if(Fnc__LP_MAP(p_variable, p_alarm, stn_name) < 0)
	{
		return -1;
	}
	return 1;
}


//------------------------------------------------------------------------------------
int  CObj__ATM_RelianceATR
::Seq__ALALIGN(CII_OBJECT__VARIABLE* p_variable, 
			   CII_OBJECT__ALARM* p_alarm, 
			   const CString& ccd_pos)
{
LOOP_RETRY:

	if(dEXT_CH__CFG_AL1_EXIST_FLAG->Check__DATA(STR__YES) > 0)
	{
		dCH__INR_FLAG__ALGN_STS->Set__DATA("ALIGNING");

		// ...
		int status;

		// ...
		{
			aoCH__ANGLE->Set__DATA(ccd_pos);
			doCH__COMMAND->Set__DATA("ALLoadPP");				// AL Setpoint

			status = Fnc__ONLINE_CHECK(p_alarm,1);
			
			     if(status == 0)		goto LOOP_RETRY;
			else if(status  < 0)		return -1;	
		}
	
		// ...
		{
			doCH__COMMAND->Set__DATA("ALSetPP");				// AL Setpoint

			status = Fnc__ONLINE_CHECK(p_alarm,1);

				 if(status == 0)		goto LOOP_RETRY;
			else if(status  < 0)		return -1;	
		}

		// ...
		{
			doCH__COMMAND->Set__DATA("ALAlign");				// AL Command

			status = Fnc__ONLINE_CHECK(p_alarm,1);

				 if(status == 0)		goto LOOP_RETRY;
			else if(status  < 0)		return -1;	
		}

		// ...
		{
			doCH__COMMAND->Set__DATA("ALResult");				// AL Result

			status = Fnc__ONLINE_CHECK(p_alarm,1);

				 if(status == 0)		goto LOOP_RETRY;
			else if(status  < 0)		return -1;	
		}
	}

	if(p_variable->Check__CTRL_ABORT() > 0)
	{
		return -1;
	}
	return 1;
}

//------------------------------------------------------------------------------------
int  CObj__ATM_RelianceATR
::Seq__SIM_ALALIGN(CII_OBJECT__VARIABLE* p_variable, 
				   CII_OBJECT__ALARM* p_alarm, 
				   const CString& ccd_pos)
{
	SCX__TIMER_CTRL x_timer_ctrl;
	x_timer_ctrl->REGISTER__ABORT_OBJECT(sObject_Name);

	double  cfg__algn_time = 1.0;
	CString var_data;

LOOP_RETRY:

	if(dEXT_CH__CFG_AL1_EXIST_FLAG->Check__DATA(STR__YES) > 0)
	{
		dCH__INR_FLAG__ALGN_STS->Set__DATA("ALIGNING");

		// ...
		int status;

		// ...
		{
			aoCH__ANGLE->Set__DATA(ccd_pos);
			doCH__COMMAND->Set__DATA("ALLoadPP");			// AL Setpoint

			status = Fnc__ONLINE_CHECK(p_alarm,1);
			
			     if(status == 0)		goto LOOP_RETRY;
			else if(status  < 0)		return -1;	
		}

		// ...
		{
			doCH__COMMAND->Set__DATA("ALSetPP");			// AL Setpoint

			status = Fnc__ONLINE_CHECK(p_alarm,1);

				 if(status == 0)		goto LOOP_RETRY;
			else if(status  < 0)		return -1;	
		}

		// ...
		{
			doCH__COMMAND->Set__DATA("ALAlign");			// AL Command

			sCH__OTR_IN_CFG__SIMUL_ALIGN_TIME->Get__DATA(var_data);
	
			cfg__algn_time = atof(var_data);
			if(x_timer_ctrl->WAIT(cfg__algn_time) < 0)
			{
				return -1;
			}

			status = Fnc__ONLINE_CHECK(p_alarm,1);

				 if(status == 0)		goto LOOP_RETRY;
			else if(status  < 0)		return -1;	
		}

		// ...
		{
			doCH__COMMAND->Set__DATA("ALResult");			// AL Result

			status = Fnc__ONLINE_CHECK(p_alarm,1);
	
				 if(status == 0)		goto LOOP_RETRY;
			else if(status  < 0)		return -1;	
		}
	}

	if(p_variable->Check__CTRL_ABORT() > 0)
	{
		return -1;
	}
	return 1;
}

//------------------------------------------------------------------------------------
int  CObj__ATM_RelianceATR
::Seq__ALALIGN_RESULT(CII_OBJECT__VARIABLE* p_variable, 
					  CII_OBJECT__ALARM* p_alarm)
{
	int status;

	// ...
	{
		status = Fnc__ALIGNER_ONLINE_CHECK(p_alarm,1);

			 if(status == 0)		return OBJ_RETRY;
		else if(status  < 0)		return -1;	

		doCH__COMMAND->Set__DATA("ALResult");				// AL Result
	}

	// ...
	{
		status = Fnc__ALIGNER_ONLINE_CHECK(p_alarm,1);

			 if(status == 0)		return OBJ_RETRY;
		else if(status  < 0)		return -1;	
	}

	if(p_variable->Check__CTRL_ABORT() > 0)
	{
		return -1;
	}
	return 1;
}

int  CObj__ATM_RelianceATR
::Fnc__IS_COMM_ONLINE()
{
	if(diCH__COMM_STS->Check__DATA("Online") > 0)
	{
		return 1;
	}
	return -1;
}
int  CObj__ATM_RelianceATR
::Fnc__ONLINE_CHECK(CII_OBJECT__ALARM* p_alarm,
				    const int alarm_check)
{
	SCX__TIMER_CTRL x_timer_ctrl;
	x_timer_ctrl->REGISTER__ABORT_OBJECT(sObject_Name);

	if(iActive_SIM > 0)
	{
		// diCH__COMM_STS->Set__DATA("Online");
		aiCH__ERROR_ID->Set__DATA("0");

		sCH__INR__ERROR_ID->Set__DATA("0");
		sCH__INR__ALGINER_ERROR_ID->Set__DATA("0");
	}

	if(Fnc__IS_COMM_ONLINE() > 0)
	{
		CString str__err_id;
		int i__err_id;

		sCH__INR__ERROR_ID->Get__DATA(str__err_id);
		i__err_id = atoi(str__err_id);

		if((i__err_id != 0)&&(alarm_check == 1))
		{
			return Fnc__ROBOT_ALARM_POST(p_alarm,i__err_id);
		}
		return 1;
	} 

	// Offline Alarm Post
	{
		int alarm_id = ALID__OFFLINE_ALARM_ACT;

		CString err_msg;
		CString r_act;

		err_msg  = "1. Check the Serial cable\n";
		err_msg += "2. Check the Robot power";

		p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,err_msg,r_act);

			 if(r_act.CompareNoCase("ABORT") == 0)		return -1;
		else if(r_act.CompareNoCase("RETRY") == 0)		return 0;
	}

    return -1;
}

int  CObj__ATM_RelianceATR
::Fnc__ALIGNER_ONLINE_CHECK(CII_OBJECT__ALARM* p_alarm,
				            const int alarm_check)
{
	SCX__TIMER_CTRL x_timer_ctrl;
	x_timer_ctrl->REGISTER__ABORT_OBJECT(sObject_Name);

	if(iActive_SIM > 0)
	{
		diCH__COMM_STS->Set__DATA("Online");
		
		sCH__INR__ALGINER_ERROR_ID->Set__DATA("0");
	}

	if(Fnc__IS_COMM_ONLINE() > 0)
	{
		CString str__err_id;
		int i__err_id;

		sCH__INR__ALGINER_ERROR_ID->Get__DATA(str__err_id);
		i__err_id = atoi(str__err_id);

		if((i__err_id != 0)&&(alarm_check == 1))
		{
			sCH__INR__ALGINER_ERROR_ID->Set__DATA("0");			// 초기화...

			return Fnc__ROBOT_ALARM_POST(p_alarm,i__err_id);
		}
		return 1;
	} 

	// Offline Alarm Post
	{
		int alarm_id = ALID__OFFLINE_ALARM_ACT;
		CString err_msg;
		CString r_act;

		err_msg  = "1. Check the Serial cable\n";
		err_msg += "2. Check the Robot power";

		p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,err_msg,r_act);

			 if(r_act.CompareNoCase("ABORT") == 0)		return -1;
		else if(r_act.CompareNoCase("RETRY") == 0)		return 0;
	}

    return -1;
}

int  CObj__ATM_RelianceATR
::Fnc__ROBOT_INIT(CII_OBJECT__VARIABLE* p_variable,
				  CII_OBJECT__ALARM* p_alarm)
{
LOOP_RETRY:

	// ...
	Fnc__APP_LOG("==> Fnc_ROBOT_INIT... START");
	Fnc__APP_LOG("Set LL1 and LL2 Load Enable..");

	doCH__HOME_AXIS->Set__DATA("ALL");
	Sleep(200);
	doCH__COMMAND->Set__DATA("Home");

	// ...
	{
		int status = Fnc__ONLINE_CHECK(p_alarm,1);

			 if(status == 0)		goto LOOP_RETRY;
		else if(status <  0)		return -1;					

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -1;
		}
	}

	// ...
	{
		Fnc__APP_LOG("Fnc_ROBOT_MAP... START");
	
		if(Fnc__ROBOT_MAP(p_variable,p_alarm) < 0)
		{
			return -1;
		}

		Fnc__APP_LOG("Fnc_ROBOT_MAP... END");
	}

	if(iActive_SIM > 0)
	{
		Sleep(1500);
	}

	if(p_variable->Check__CTRL_ABORT() > 0)
	{
		return -1;
	}

	Fnc__APP_LOG("<== Fnc_ROBOT_INIT... COMPLETE");
	return 1;
}

int  CObj__ATM_RelianceATR
::Fnc__ALIGNER_INIT(CII_OBJECT__VARIABLE* p_variable,
				    CII_OBJECT__ALARM* p_alarm)
{
LOOP_RETRY:

	Fnc__APP_LOG("==> Fnc_ALIGNER_INIT... START");

	// Aligner Command
	if(dEXT_CH__CFG_AL1_EXIST_FLAG->Check__DATA("YES") > 0)
	{
		// Aligner Init. -----
		Fnc__APP_LOG("Aligner INIT... START");

		CString wfr_size;
		dEXT_CH__CFG_AL1_MATERIAL_TYPE->Get__DATA(wfr_size);		

		     if(wfr_size.CompareNoCase("3inRnd")    == 0)		doCH__MATERIAL->Set__DATA("1"); 
		else if(wfr_size.CompareNoCase("82.5mmRnd") == 0)		doCH__MATERIAL->Set__DATA("2"); 
		else if(wfr_size.CompareNoCase("100mmRnd")  == 0)		doCH__MATERIAL->Set__DATA("3"); 
		else if(wfr_size.CompareNoCase("125mmRnd")  == 0)		doCH__MATERIAL->Set__DATA("4"); 
		else if(wfr_size.CompareNoCase("150mmRnd")  == 0)		doCH__MATERIAL->Set__DATA("5"); 
		else if(wfr_size.CompareNoCase("200mmRnd")  == 0)		doCH__MATERIAL->Set__DATA("6");  
		else if(wfr_size.CompareNoCase("300mmRnd")  == 0)		doCH__MATERIAL->Set__DATA("7"); 
		else													doCH__MATERIAL->Set__DATA("6"); 

		// Set CCD position
		if(wfr_size.CompareNoCase("300mmRnd") == 0)			doCH__WFR_CCD->Set__DATA("2");
		else												doCH__WFR_CCD->Set__DATA("1");

		// ...
		{
			doCH__COMMAND->Set__DATA("InitAlgn");			

			int status = Fnc__ONLINE_CHECK(p_alarm,1);

				 if(status == 0)		goto LOOP_RETRY;
			else if(status <  0)		return -1;				
			
			if(p_variable->Check__CTRL_ABORT() > 0)	
			{
				return -1;
			}
		}

		// ...
		{
			doCH__COMMAND->Set__DATA("ALSetComm");			

			int status = Fnc__ONLINE_CHECK(p_alarm,1);

				 if(status == 0)		goto LOOP_RETRY;
			else if(status <  0)		return -1;			

			if(p_variable->Check__CTRL_ABORT() > 0)
			{
				return -1;
			}
		}

		// ...
		{
			doCH__COMMAND->Set__DATA("ALStoComm");			

			int status = Fnc__ONLINE_CHECK(p_alarm,1);

				 if(status == 0)		goto LOOP_RETRY;
			else if(status <  0)		return -1;			
		}

		// ...
		{
			CString cfg__ccd_pos;

			aCH__PARA_AL1_CCD_POS->Get__DATA(cfg__ccd_pos);		
			aoCH__ANGLE->Set__DATA(cfg__ccd_pos);

			doCH__COMMAND->Set__DATA("ALLoadPP");			
			
			if(p_variable->Check__CTRL_ABORT() > 0)
			{
				return -1;
			}

			int status = Fnc__ONLINE_CHECK(p_alarm,1);

				 if(status == 0)		goto LOOP_RETRY;
			else if(status <  0)		return -1;			
		}

		// ...
		{
			doCH__COMMAND->Set__DATA("ALSetPP");				

			int status = Fnc__ONLINE_CHECK(p_alarm,1);

				 if(status == 0)		goto LOOP_RETRY;
			else if(status <  0)		return -1;	

			if(p_variable->Check__CTRL_ABORT() > 0)
			{
				return -1;
			}
		}

		// End of Aligner Init -----
		if(Fnc__ROBOT_SET_WAFER(p_variable,p_alarm) < 0)
		{
			return -1;
		}

		// Aligner Home -----
		{
			doCH__COMMAND->Set__DATA("ALHome");

			int status = Fnc__ONLINE_CHECK(p_alarm,1);
			
				 if(status == 0)		goto LOOP_RETRY;
			else if(status <  0)		return -1;				

			if(p_variable->Check__CTRL_ABORT() > 0)
			{
				return -1;
			}
		}

		// AL Map -----
		{
			Fnc__APP_LOG("Aligner AlMap... START");
		
			if(Fnc__AL1_AlMap(p_variable,p_alarm) < 0)
			{
				return -1;
			}
		}
	}

	Fnc__APP_LOG("<== Fnc_ALIGNER_INIT... COMPLETE");
	return 1;
}

int  CObj__ATM_RelianceATR
::Fnc__ROBOT_MAP(CII_OBJECT__VARIABLE* p_variable,
			     CII_OBJECT__ALARM* p_alarm)
{
TRY_RB1_MAP:

	// ...
	int status = Fnc__ROBOT_VAC_OFF(p_variable,p_alarm);
	if(status < 0)		return -1;

	doCH__VACUUM->Set__DATA("ON");
	doCH__COMMAND->Set__DATA("SetVac");

	status = Fnc__ROBOT_MAP_ONLINE_CHECK(p_alarm);

	// ...
	{
		CString err_msg;

		err_msg.Format("status  :  %1d \n", status);
		Fnc__APP_LOG(err_msg);
	}

	if(status == 2)			
	{
		Set__ROBOT_MATERIAL_SLOT("NONE");
		
TRY_RB1_VACOFF:

		// ...
		doCH__VACUUM->Set__DATA("OFF");
		doCH__COMMAND->Set__DATA("SetVac");

		// ...
		int status = Fnc__ONLINE_CHECK(p_alarm,0);

			 if(status == 0)		goto TRY_RB1_VACOFF;
		else if(status <  0)		return -1;				// abort

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -1;
		}
	}
	else if(status == 1)
	{
		Set__ROBOT_MATERIAL_SLOT("EXIST");
	}
	else if(status == 0)
	{
		status = Fnc__ROBOT_VAC_OFF(p_variable,p_alarm);
		if(status < 0)		return -1;

		goto TRY_RB1_MAP;
	}
	else if(status == -1)
	{
		return -1;			// abort
	}

	if(p_variable->Check__CTRL_ABORT() > 0)
	{
		return -1;
	}

	if(iActive_SIM > 0)
	{
		Sleep(1500);
	}
	if(p_variable->Check__CTRL_ABORT() > 0)
	{
		return -1;
	}

	if(iActive_SIM > 0)
	{
		dCH__MON_ARM_A_MATERIAL_STATUS->Set__DATA(STR__NONE);
	}
	return 1;
}

int  CObj__ATM_RelianceATR
::Fnc__ROBOT_EXTEND(CII_OBJECT__VARIABLE* p_variable,
				    CII_OBJECT__ALARM* p_alarm,
				    const CString& arm_type,
				    const CString& stn_name,
				    const CString& stn_slot)
{
LOOP_RETRY:

	// ...
	{
		CString act_name;
		act_name.Format("extend from Arm-%s to %s-%s", arm_type,stn_name,stn_slot);

		Fnc__APP_LOG(act_name);
	}

	if(Set_IO__ARM_TYPE(arm_type,stn_name,stn_slot) < 0)
	{
		return -1;
	}
	
	doCH__RPOS->Set__DATA("EX");			// "EX" default
	doCH__COMMAND->Set__DATA("Goto");

	// ...
	{
		int status = Fnc__ONLINE_CHECK(p_alarm,1);

			 if(status == 0)		goto LOOP_RETRY;
		else if(status  < 0)		return -1;	

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -1;
		}	
	}
	return 1;
}

int  CObj__ATM_RelianceATR
::Fnc__ROBOT_RETRACT(CII_OBJECT__VARIABLE* p_variable,
				     CII_OBJECT__ALARM* p_alarm,
				     const CString& arm_type,
				     const CString& stn_name,
				     const CString& stn_slot)
{
LOOP_RETRY:

	// ...
	{
		CString act_name;
		act_name.Format("retract from Arm-%s to %s-%s", arm_type,stn_name,stn_slot);

		Fnc__APP_LOG(act_name);
	}

	if(Set_IO__ARM_TYPE(arm_type,stn_name,stn_slot) < 0)
	{
		return -1;
	}

	doCH__RPOS->Set__DATA("RE");			// "RE" default
	doCH__COMMAND->Set__DATA("Goto");

	// ...
	{
		int status = Fnc__ONLINE_CHECK(p_alarm,1);

			 if(status == 0)		goto LOOP_RETRY;
		else if(status  < 0)		return -1;	

		if(p_variable->Check__CTRL_ABORT() > 0)	
		{
			return -1;
		}
	}	
	return 1;
}

int  CObj__ATM_RelianceATR
::Fnc__ROBOT_GOUP(CII_OBJECT__VARIABLE* p_variable,
				  CII_OBJECT__ALARM* p_alarm,
				  const CString& arm_type,
				  const CString& stn_name,
				  const CString& stn_slot)
{
LOOP_RETRY:

	// ...
	{
		CString act_name;
		act_name.Format("goup from Arm-%s to %s-%s", arm_type,stn_name,stn_slot);

		Fnc__APP_LOG(act_name);
	}

	if(Set_IO__ARM_TYPE(arm_type,stn_name,stn_slot) < 0)
	{
		return -1;
	}

	doCH__RPOS->Set__DATA("RE");			// "RE" default
	doCH__ZPOS->Set__DATA("UP");
	doCH__COMMAND->Set__DATA("Goto");

	// ...
	{
		int status = Fnc__ONLINE_CHECK(p_alarm,1);

			 if(status == 0)		goto LOOP_RETRY;
		else if(status  < 0)		return -1;	

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -1;
		}
	}	
	return 1;
}

int  CObj__ATM_RelianceATR
::Fnc__ROBOT_GODOWN(CII_OBJECT__VARIABLE* p_variable,
				    CII_OBJECT__ALARM* p_alarm,
				    const CString& arm_type,
				    const CString& stn_name,
				    const CString& stn_slot)
{
LOOP_RETRY:

	// ...
	{
		CString act_name;
		act_name.Format("godown from Arm-%s to %s-%s", arm_type,stn_name,stn_slot);

		Fnc__APP_LOG(act_name);
	}

	if(Set_IO__ARM_TYPE(arm_type,stn_name,stn_slot) < 0)
	{
		return -1;
	}

	doCH__RPOS->Set__DATA("RE");			// "RE" default
	doCH__ZPOS->Set__DATA("DN");
	doCH__COMMAND->Set__DATA("Goto");

	// ...
	{
		int status = Fnc__ONLINE_CHECK(p_alarm,1);

			 if(status == 0)		goto LOOP_RETRY;
		else if(status  < 0)		return -1;	

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -1;
		}
	}	
	return 1;
}

int  CObj__ATM_RelianceATR
::Fnc__ROBOT_HOME(CII_OBJECT__VARIABLE* p_variable,
				  CII_OBJECT__ALARM* p_alarm)
{	
LOOP_RETRY:

	doCH__HOME_AXIS->Set__DATA("ALL");
	doCH__COMMAND->Set__DATA("Home");

	// ...
	{
		int status = Fnc__ONLINE_CHECK(p_alarm,1);

			 if(status == 0)		goto LOOP_RETRY;
		else if(status <  0)		return -1;					

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -1;
		}
	}	
	return 1;
}

int  CObj__ATM_RelianceATR
::Fnc__LP_MAP(CII_OBJECT__VARIABLE* p_variable, 
			  CII_OBJECT__ALARM* p_alarm, 
			  const CString& stn_name)
{
LOOP_RETRY:

	// Slot 정보 초기화  ...
	{
		for (int i=0; i<CFG_LPx__SLOT_SIZE; i++)
		{
			dCH__LPx_SLOT_STATUS[i]->Set__DATA("UNKNOWN");
		}
	}

	// ...
	CString log_msg;
	CString log_bff;

	if(Set_IO__ARM_TYPE(stn_name) < 0)
	{
		return -1;
	}

	doCH__RPOS->Set__DATA("RE");	// "RE" default

	// ...
	{
		log_msg = "IO Set ... \n";

		log_bff.Format(" *** %s <- %s",
			           doCH__RPOS->Get__VARIABLE_NAME(),
					   doCH__RPOS->Get__STRING());
		log_msg += log_bff;

		Fnc__APP_LOG(log_msg);
	}

	// ...
	{
		int status = 1;
		CString str_cmd = "";

		if(status > 0)
		{
			str_cmd = "MapClear";
			status = doCH__COMMAND->Set__DATA(str_cmd);

			// ...
			{
				log_msg.Format("IO Set (%s) ... \n", str_cmd);

				log_bff.Format(" *** %s <- %s \n",
							   doCH__COMMAND->Get__VARIABLE_NAME(),
							   doCH__COMMAND->Get__STRING());
				log_msg += log_bff;

				log_bff.Format(" *** state : %1d \n",	status);
				log_msg += log_bff;

				Fnc__APP_LOG(log_msg);
			}
		}

		if(status > 0)
		{
			str_cmd = "MapStn";
			status = doCH__COMMAND->Set__DATA(str_cmd);

			// ...
			{
				log_msg.Format("IO Set (%s) ... \n", str_cmd);

				log_bff.Format(" *** %s <- %s \n",
								doCH__COMMAND->Get__VARIABLE_NAME(),
								doCH__COMMAND->Get__STRING());
				log_msg += log_bff;

				log_bff.Format(" *** state : %1d \n",	status);
				log_msg += log_bff;

				Fnc__APP_LOG(log_msg);
			}
		}

		// < 0 일 때 Alarm 띄우는 부분 추가 할것...
		if(status < 0)
		{
			return -1;
		}

		if(status > 0)
		{
			CString str__err_id;
			int i__err_id;
		
			sCH__INR__ERROR_ID->Get__DATA(str__err_id);
			i__err_id = atoi(str__err_id);
			if(i__err_id != 0)
			{
				status = Fnc__ROBOT_ALARM_POST(p_alarm,i__err_id);

				if(status == 0)
				{
					goto LOOP_RETRY;
				}
				else if(status < 0)
				{
					return -1;
				}
			}

			// ...
			str_cmd = "GetMap";
			status = doCH__COMMAND->Set__DATA(str_cmd);

			// ...
			{
				log_msg.Format("IO Set (%s) ... \n", str_cmd);

				log_bff.Format(" *** %s <- %s \n",
								doCH__COMMAND->Get__VARIABLE_NAME(),
								doCH__COMMAND->Get__STRING());
				log_msg += log_bff;

				log_bff.Format(" *** state : %1d \n",	status);
				log_msg += log_bff;

				Fnc__APP_LOG(log_msg);
			}

			if(i__err_id != 0)
			{
				status = Fnc__ROBOT_ALARM_POST(p_alarm,i__err_id);
				
				if(status == 0)
				{
					goto LOOP_RETRY;
				}
				else if(status < 0)
				{
					return -1;
				}
			}
		}
	}

	if(iActive_SIM > 0)
	{
		for (int i=0; i<CFG_LPx__SLOT_SIZE; i++)
		{
			dCH__LPx_SLOT_STATUS[i]->Set__DATA("PRESENT");
		}
	}

	// ...
	{
		int status = Fnc__ONLINE_CHECK(p_alarm,1);

			 if(status == 0)		goto LOOP_RETRY;
		else if(status  < 0)		return -1;	

		if(p_variable->Check__CTRL_ABORT() > 0)	
		{
			return -1;
		}
	}
	
	return 1;
}

int  CObj__ATM_RelianceATR
::Fnc__ROBOT_VAC_OFF(CII_OBJECT__VARIABLE* p_variable,
				     CII_OBJECT__ALARM* p_alarm)
{
LOOP_RETRY:

	doCH__VACUUM->Set__DATA("OFF");
	doCH__COMMAND->Set__DATA("SetVac");

	// ...
	{
		int status = Fnc__ONLINE_CHECK(p_alarm,1);

			 if(status == 0)		goto LOOP_RETRY;
		else if(status <  0)		return -1;				// abort

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -1;
		}
	}
	return 1;
}
int  CObj__ATM_RelianceATR
::Fnc__ROBOT_MAP_ONLINE_CHECK(CII_OBJECT__ALARM* p_alarm)
{
	if(iActive_SIM > 0)
	{
		diCH__COMM_STS->Set__DATA("Online");
		aiCH__ERROR_ID->Set__DATA("0");

		sCH__INR__ERROR_ID->Set__DATA("0");
	}

	if(Fnc__IS_COMM_ONLINE() > 0)
	{
		CString str__err_id;
		int i__err_id;

		sCH__INR__ERROR_ID->Get__DATA(str__err_id);
		i__err_id = atoi(str__err_id);

		// ...
		{
			CString err_msg;

			err_msg.Format("Error ID  :  %s(%1d) \n", str__err_id,i__err_id);
			Fnc__APP_LOG(err_msg);
		}

		if(i__err_id == 712)
		{
			return 2;
		}

		if((i__err_id != 0)&&(i__err_id != 712))
		{
			return Fnc__ROBOT_ALARM_POST(p_alarm,i__err_id);
		}
		return 1;
	} 

	// Offline alarm post
	{
		int alarm_id = ALID__OFFLINE_ALARM_ACT;

		CString err_msg;
		CString r_act;
		
		err_msg  = "1. Check the Serial cable\n";
		err_msg += "2. Check the Robot power\n";

		p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,err_msg,r_act);

		if(r_act.CompareNoCase("ABORT") == 0)		return -1;
		if(r_act.CompareNoCase("RETRY") == 0)		return 0;
	}
    return -1;
}

int  CObj__ATM_RelianceATR
::Fnc__ROBOT_SET_WAFER(CII_OBJECT__VARIABLE* p_variable,
					   CII_OBJECT__ALARM* p_alarm)
{
LOOP_RETRY:

	// ...
	CString cfg__material_type;
	dEXT_CH__CFG_AL1_MATERIAL_TYPE->Get__DATA(cfg__material_type);

	// Wafer Shape ...
	{
		if((cfg__material_type.CompareNoCase("3inRnd")    == 0)	
		|| (cfg__material_type.CompareNoCase("82.5mmRnd") == 0)
		|| (cfg__material_type.CompareNoCase("100mmRnd")  == 0)	
		|| (cfg__material_type.CompareNoCase("125mmRnd")  == 0)	
		|| (cfg__material_type.CompareNoCase("150mmRnd")  == 0)
		|| (cfg__material_type.CompareNoCase("200mmRnd")  == 0)
		|| (cfg__material_type.CompareNoCase("300mmRnd")  == 0))
		{
			doCH__WFR_SHAPE->Set__DATA("Round");
		}
		else if((cfg__material_type.CompareNoCase("4.5inSqr")  == 0)	
			 || (cfg__material_type.CompareNoCase("82.5mmRnd") == 0)
			 || (cfg__material_type.CompareNoCase("125mmSqr")  == 0)	
			 || (cfg__material_type.CompareNoCase("150mmSqr")  == 0)	
			 || (cfg__material_type.CompareNoCase("175mmSqr")  == 0))
		{
			doCH__WFR_SHAPE->Set__DATA("Square");
		}
	}

	// Wafer Size ...
	{
			 if(cfg__material_type.CompareNoCase("3inRnd")    == 0)		doCH__WFR_SIZE->Set__DATA("1"); 
		else if(cfg__material_type.CompareNoCase("82.5mmRnd") == 0)		doCH__WFR_SIZE->Set__DATA("2"); 
		else if(cfg__material_type.CompareNoCase("100mmRnd")  == 0)		doCH__WFR_SIZE->Set__DATA("3"); 
		else if(cfg__material_type.CompareNoCase("125mmRnd")  == 0)		doCH__WFR_SIZE->Set__DATA("4"); 
		else if(cfg__material_type.CompareNoCase("150mmRnd")  == 0)		doCH__WFR_SIZE->Set__DATA("5"); 
		else if(cfg__material_type.CompareNoCase("200mmRnd")  == 0)		doCH__WFR_SIZE->Set__DATA("6"); 
		else if(cfg__material_type.CompareNoCase("300mmRnd")  == 0)		doCH__WFR_SIZE->Set__DATA("7"); 
		else															doCH__WFR_SIZE->Set__DATA("6");
	}

	// ...
	CString cfg__fdcl;
	dEXT_CH__CFG_AL1_FDCL->Get__DATA(cfg__fdcl);

		 if(cfg__fdcl.CompareNoCase("None") == 0)		doCH__WFR_FDCL->Set__DATA("None"); 
	else if(cfg__fdcl.CompareNoCase("Flat") == 0)		doCH__WFR_FDCL->Set__DATA("Flat"); 
	else if(cfg__fdcl.CompareNoCase("Ntch") == 0)		doCH__WFR_FDCL->Set__DATA("Ntch");
	else												doCH__WFR_FDCL->Set__DATA("Flat"); 

	// Aligner command
	if(dEXT_CH__CFG_AL1_EXIST_FLAG->Check__DATA("YES") > 0)
	{
		doCH__COMMAND->Set__DATA("ALSetWfr");

		// ...
		int status = Fnc__ONLINE_CHECK(p_alarm,1);
		
			 if(status == 0)		goto LOOP_RETRY;
		else if(status <  0)		return -1;	

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -1;
		}
	}

	return 1;
}

// AL1 -----
int  CObj__ATM_RelianceATR
::Fnc__AL1_AlMap(CII_OBJECT__VARIABLE* p_variable,
			     CII_OBJECT__ALARM* p_alarm)
{
LOOP_RETRY:

	if(dEXT_CH__CFG_AL1_EXIST_FLAG->Check__DATA("YES") > 0)
	{
		if(iActive_SIM > 0)
		{
			if(dEXT_CH__AL1_SLOT01_STATUS->Check__DATA(STR__NONE) > 0)
			{
				diCH__AL_WFR->Set__DATA("NO");
			}
			else
			{
				diCH__AL_WFR->Set__DATA("YES");
			}
		}

		// ...
		CString str__al_wfr;
		diCH__AL_WFR->Get__DATA(str__al_wfr);
			 
			 if(str__al_wfr.CompareNoCase("?")   == 0)		dEXT_CH__AL1_SLOT01_STATUS->Set__DATA(STR__UNKNOWN);
		else if(str__al_wfr.CompareNoCase("NO")  == 0)		dEXT_CH__AL1_SLOT01_STATUS->Set__DATA(STR__NONE);
		else if(str__al_wfr.CompareNoCase("YES") == 0)		dEXT_CH__AL1_SLOT01_STATUS->Set__DATA(STR__EXIST);
		else												dEXT_CH__AL1_SLOT01_STATUS->Set__DATA(STR__UNKNOWN);

		// ...
		int status = Fnc__ONLINE_CHECK(p_alarm,0);
		
			 if(status == 0)		goto LOOP_RETRY;
		else if(status <  0)		return -1;			

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -1;
		}
	}

	return 1;
}
