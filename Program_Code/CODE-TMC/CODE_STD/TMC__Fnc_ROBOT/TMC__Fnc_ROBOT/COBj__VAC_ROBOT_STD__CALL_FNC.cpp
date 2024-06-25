#include "StdAfx.h"
#include "CObj__VAC_ROBOT_STD.h"
#include "CObj__VAC_ROBOT_STD__ALID.h"
#include "COBj__VAC_ROBOT_STD__DEF.h"

#include "Macro_Function.h"


//----------------------------------------------------------------------------------------------------
int  CObj__VAC_ROBOT_STD::
Call__HOME(CII_OBJECT__VARIABLE* p_variable,
		   CII_OBJECT__ALARM* p_alarm)
{
	return pROBOT__OBJ_CTRL->Call__OBJECT(CMMD__HOME);
}

int  CObj__VAC_ROBOT_STD::
Call__MAP(CII_OBJECT__VARIABLE* p_variable,
		  CII_OBJECT__ALARM* p_alarm)
{
	return pROBOT__OBJ_CTRL->Call__OBJECT(CMMD__MAP);
}


// ...
int  CObj__VAC_ROBOT_STD::
Call__INIT(CII_OBJECT__VARIABLE* p_variable,
		   CII_OBJECT__ALARM* p_alarm)
{
	return pROBOT__OBJ_CTRL->Call__OBJECT(CMMD__INIT);
}

//
int  CObj__VAC_ROBOT_STD::
Call__PICK(CII_OBJECT__VARIABLE* p_variable,
		   CII_OBJECT__ALARM* p_alarm,
		   const CString& para_arm,
	  	   const CString& para_module,
	       const CString& para_slot,
		   const int handoff_mode)
{
	return Seq__PICK(p_variable,
					 p_alarm,
					 para_arm,
					 para_module,
					 para_slot,
					 NOT__X_PICK_PLACE,
					 handoff_mode);
}
int  CObj__VAC_ROBOT_STD::
Call__XPICK(CII_OBJECT__VARIABLE* p_variable,
			CII_OBJECT__ALARM* p_alarm,
			const CString& para_arm,
			const CString& para_module,
			const CString& para_slot,
			const int handoff_mode)
{
	return Seq__PICK(p_variable,
					 p_alarm,
					 para_arm,
					 para_module,
					 para_slot,
					 X_PICK_PLACE,
					 handoff_mode);
}

int  CObj__VAC_ROBOT_STD::
Call__PLACE(CII_OBJECT__VARIABLE* p_variable,
			CII_OBJECT__ALARM* p_alarm,
			const CString& para_arm,
			const CString& para_module,
			const CString& para_slot,
			const int handoff_mode)
{
	return Seq__PLACE(p_variable,
					  p_alarm,
					  para_arm,
					  para_module,
					  para_slot,
					  NOT__X_PICK_PLACE,
					  handoff_mode);
}
int  CObj__VAC_ROBOT_STD::
Call__XPLACE(CII_OBJECT__VARIABLE* p_variable,
			 CII_OBJECT__ALARM* p_alarm,
			 const CString& para_arm,
			 const CString& para_module,
			 const CString& para_slot,
	         const int handoff_mode)
{
	//
	return Seq__PLACE(p_variable,
					  p_alarm,
					  para_arm,
					  para_module,
					  para_slot,
					  X_PICK_PLACE,
					  handoff_mode);
}


// Rotate -----
int  CObj__VAC_ROBOT_STD::
Call__ROTATE(CII_OBJECT__VARIABLE* p_variable,
			 CII_OBJECT__ALARM* p_alarm,
		     const CString& para_arm,
		     const CString& para_module,
		     const CString& para_slot)
{
	//
	return Fnc__CALL_ROTATE(p_variable,
							p_alarm,
							para_arm,
							para_module,
							para_slot);
}


int  CObj__VAC_ROBOT_STD::
Fnc__LBx_PICK(CII_OBJECT__VARIABLE* p_variable,
			  CII_OBJECT__ALARM* p_alarm,
			  const CString& para_arm,
			  const CString& para_module,
			  const CString& para_slot,
			  const int ex_flag)
{
	CString str_log;

	// ...
	{
		CString log_bff;

		str_log = "Fnc__LBx_PICK() : Start ...\n";

		log_bff.Format("  * para_arm    <- %s  \n", para_arm);
		str_log += log_bff;
		log_bff.Format("  * para_module <- %s  \n", para_module);
		str_log += log_bff;
		log_bff.Format("  * para_slot   <- %s  \n", para_slot);
		str_log += log_bff;
		log_bff.Format("  * ex_flag     <- %1d \n", ex_flag);
		str_log += log_bff;

		log_bff.Format("  * %s <- %s \n", 
					   dCH__CFG_LLx_CLOSE_SEQUENCE_MODE->Get__CHANNEL_NAME(),
					   dCH__CFG_LLx_CLOSE_SEQUENCE_MODE->Get__STRING());
		str_log += log_bff;

		Fnc__APP_LOG(str_log);
	}

	// ...
	int active_mode  = -1;
	int robot_z_move =  1;

	int ll_i = Macro__CHECK_LLx_INDEX(para_module);
	if(ll_i >= 0)
	{
		if(dCH__CFG_LLx_HANDSHAKE_MODE[ll_i]->Check__DATA(STR__ACTIVE) > 0)
		{
			active_mode = 1;
		}
		if(dCH__CFG_LLx_ROBOT_Z_MOVE[ll_i]->Check__DATA(STR__NO) > 0)
		{
			robot_z_move = -1;
		}
	}

	// ...
	{
		CString log_bff;

		str_log.Format("%s[%s] Handshake Info ...\n", para_module,para_slot);

		log_bff.Format("  * active_mode  <- %1d \n", active_mode);
		str_log += log_bff;
		log_bff.Format("  * robot_z_move <- %1d \n", robot_z_move);
		str_log += log_bff;

		int ll_i = Macro__CHECK_LLx_INDEX(para_module);
		if(ll_i >= 0)
		{
			log_bff.Format("  * %s <- %s \n", 
							dCH__CFG_LLx_HANDSHAKE_MODE[ll_i]->Get__CHANNEL_NAME(),
							dCH__CFG_LLx_HANDSHAKE_MODE[ll_i]->Get__STRING());
			str_log += log_bff;

			log_bff.Format("  * %s <- %s \n", 
							dCH__CFG_LLx_ROBOT_Z_MOVE[ll_i]->Get__CHANNEL_NAME(),
							dCH__CFG_LLx_ROBOT_Z_MOVE[ll_i]->Get__STRING());
			str_log += log_bff;
		}

		Fnc__APP_LOG(str_log);
	}

	if(active_mode > 0)
	{
		// LLx : PICK_READY
		int ll_i = Macro__CHECK_LLx_INDEX(para_module);
		if(ll_i >= 0)
		{
			aEXT_CH__LLx__PARA_HANDSHAKE_SLOT[ll_i]->Set__DATA(para_slot);
			
			if(pLLx__OBJ_CTRL[ll_i]->Call__OBJECT(CMMD__PICK_READY) < 0)
			{
				return -101;
			}
		}

		if(robot_z_move > 0)
		{
			// ROBOT : PICK
			if(Fnc__CALL_PICK(p_variable,
							p_alarm,
							para_arm,
							para_module,
							para_slot) < 0)
			{
				str_log.Format("Lower Object Pick Aborted..");
				Fnc__APP_LOG(str_log);
				return -111;
			}
		}
		else
		{
			CStringArray l_para;

			l_para.Add(para_arm);
			l_para.Add(para_module);
			l_para.Add(para_slot);

			// ROBOT : EXTEND
			if(pROBOT__OBJ_CTRL->Call__OBJ_MODE(CMMD__EXTEND,l_para) < 0)
			{
				str_log.Format("Lower Object Extend Aborted..");
				Fnc__APP_LOG(str_log);
				return -121;
			}

			// LLx : PICK_EXTEND
			if(ll_i >= 0)
			{
				aEXT_CH__LLx__PARA_HANDSHAKE_SLOT[ll_i]->Set__DATA(para_slot);

				if(pLLx__OBJ_CTRL[ll_i]->Call__OBJECT(CMMD__PICK_EXTEND) < 0)
				{
					return -102;
				}
			}

			// ROBOT : RETRACT
			if(pROBOT__OBJ_CTRL->Call__OBJ_MODE(CMMD__RETRACT,l_para) < 0)
			{
				str_log.Format("Lower Object Retract Aborted..");
				Fnc__APP_LOG(str_log);
				return -122;
			}

			Fnc__CHANGE_MATERIAL_INFO(-1, para_arm,para_module,para_slot);
		}

		// LLx : PICK_COMPLETE
		if(ll_i >= 0)
		{
			aEXT_CH__LLx__PARA_HANDSHAKE_SLOT[ll_i]->Set__DATA(para_slot);

			if(pLLx__OBJ_CTRL[ll_i]->Call__OBJECT(CMMD__PICK_COMPLETE) < 0)
			{
				return -103;
			}
		}
	}
	else
	{
		if(Fnc__CALL_PICK(p_variable,
						  p_alarm,
						  para_arm,
						  para_module,
						  para_slot) < 0)
		{
			str_log.Format("Lower Object Pick Aborted..");
			Fnc__APP_LOG(str_log);
			return -131;
		}
	}

	if(ex_flag > 0)
	{

	}
	else
	{
		if((dEXT_CH__CFG_SETUP_TEST_MODE->Check__DATA(STR__ENABLE) > 0)
		&& (dEXT_CH__CFG_SETUP_AUTO_VENTING_PUMPING->Check__DATA(STR__DISABLE) > 0)
		&& (dEXT_CH__CFG_SETUP_LLx_DOOR_SLOT_VLV_CONTROL->Check__DATA(STR__DISABLE) > 0))
		{
			return 1;
		}

		// ...
		int ll_i = Macro__CHECK_LLx_INDEX(para_module);
		if(ll_i >= 0)
		{
			CString ll_name = Macro__GET_LLx_NAME(ll_i);

			CString cmmd_ll;
			cmmd_ll.Format("%s_SV_CLOSE", ll_name);

			if(dCH__CFG_LLx_CLOSE_SEQUENCE_MODE->Check__DATA(STR__ENABLE) > 0)
			{
				if(pTMC_VLV__OBJ_CTRL->Call__OBJECT(cmmd_ll) < 0)
				{
					return -201;
				}
			}
			else
			{
				if(pTMC_VLV__OBJ_CTRL->Run__OBJECT(cmmd_ll) < 0)
				{
					return -211;
				}
			}
		}
		else
		{
			return -203;
		}
	}
	return OBJ_AVAILABLE;
}

int  CObj__VAC_ROBOT_STD::
Fnc__LBx_PLACE(CII_OBJECT__VARIABLE* p_variable,
			  CII_OBJECT__ALARM* p_alarm,
			  const CString& para_arm,
			  const CString& para_module,
			  const CString& para_slot,
			  const int ex_flag)
{
	CString str_log;

	// ...
	int active_mode  = -1;
	int robot_z_move =  1;

	int ll_i = Macro__CHECK_LLx_INDEX(para_module);
	if(ll_i >= 0)
	{
		if(dCH__CFG_LLx_HANDSHAKE_MODE[ll_i]->Check__DATA(STR__ACTIVE) > 0)
		{
			active_mode = 1;
		}
		if(dCH__CFG_LLx_ROBOT_Z_MOVE[ll_i]->Check__DATA(STR__NO) > 0)
		{
			robot_z_move = -1;
		}
	}

	// ...
	{
		CString log_bff;

		str_log.Format("%s[%s] Handshake Info ...\n", para_module,para_slot);

		log_bff.Format("  * active_mode  <- %1d \n", active_mode);
		str_log += log_bff;
		log_bff.Format("  * robot_z_move <- %1d \n", robot_z_move);
		str_log += log_bff;

		if(ll_i >= 0)
		{
			log_bff.Format("  * %s <- %s \n", 
							dCH__CFG_LLx_HANDSHAKE_MODE[ll_i]->Get__CHANNEL_NAME(),
							dCH__CFG_LLx_HANDSHAKE_MODE[ll_i]->Get__STRING());
			str_log += log_bff;

			log_bff.Format("  * %s <- %s \n", 
							dCH__CFG_LLx_ROBOT_Z_MOVE[ll_i]->Get__CHANNEL_NAME(),
							dCH__CFG_LLx_ROBOT_Z_MOVE[ll_i]->Get__STRING());
			str_log += log_bff;
		}

		Fnc__APP_LOG(str_log);
	}

	if(active_mode > 0)
	{
		// LLx : PLACE_READY
		if(ll_i >= 0)
		{
			aEXT_CH__LLx__PARA_HANDSHAKE_SLOT[ll_i]->Set__DATA(para_slot);

			if(pLLx__OBJ_CTRL[ll_i]->Call__OBJECT(CMMD__PLACE_READY) < 0)
			{
				return -101;
			}
		}

		if(robot_z_move > 0)
		{
			// ROBOT : PLACE
			if(Fnc__CALL_PLACE(p_variable,
							   p_alarm,
							   para_arm,
							   para_module,
							   para_slot) < 0)
			{
				str_log.Format("Lower Object Place Aborted..");
				Fnc__APP_LOG(str_log);
				return -111;
			}
		}
		else
		{
			CStringArray l_para;

			l_para.Add(para_arm);
			l_para.Add(para_module);
			l_para.Add(para_slot);

			// ROBOT : EXTEND
			if(pROBOT__OBJ_CTRL->Call__OBJ_MODE(CMMD__EXTEND,l_para) < 0)
			{
				str_log.Format("Lower Object Extend Aborted..");
				Fnc__APP_LOG(str_log);
				return -121;
			}

			// LLx : PLACE_EXTEND
			if(ll_i >= 0)
			{
				aEXT_CH__LLx__PARA_HANDSHAKE_SLOT[ll_i]->Set__DATA(para_slot);

				if(pLLx__OBJ_CTRL[ll_i]->Call__OBJECT(CMMD__PLACE_EXTEND) < 0)
				{
					return -102;
				}
			}

			// ROBOT : RETRACT
			if(pROBOT__OBJ_CTRL->Call__OBJ_MODE(CMMD__RETRACT,l_para) < 0)
			{
				str_log.Format("Lower Object Retract Aborted..");
				Fnc__APP_LOG(str_log);
				return -122;
			}

			Fnc__CHANGE_MATERIAL_INFO(1, para_arm,para_module,para_slot);
		}

		// LLx : PLACE_COMPLETE
		if(ll_i >= 0)
		{
			aEXT_CH__LLx__PARA_HANDSHAKE_SLOT[ll_i]->Set__DATA(para_slot);

			if(pLLx__OBJ_CTRL[ll_i]->Call__OBJECT(CMMD__PLACE_COMPLETE) < 0)
			{
				return -103;
			}
		}
	}
	else
	{
		if(Fnc__CALL_PLACE(p_variable,
						   p_alarm,
						   para_arm,
						   para_module,
						   para_slot) < 0)
		{
			str_log.Format("Lower Object Place Aborted..");
			Fnc__APP_LOG(str_log);
			return -131;
		}
	}

	if(ex_flag > 0)
	{

	}
	else
	{
		if((dEXT_CH__CFG_SETUP_TEST_MODE->Check__DATA(STR__ENABLE) > 0)
		&& (dEXT_CH__CFG_SETUP_AUTO_VENTING_PUMPING->Check__DATA(STR__DISABLE) > 0)
		&& (dEXT_CH__CFG_SETUP_LLx_DOOR_SLOT_VLV_CONTROL->Check__DATA(STR__DISABLE) > 0))
		{
			return 1;
		}

		// ...
		int ll_i = Macro__CHECK_LLx_INDEX(para_module);
		if(ll_i >= 0)
		{
			CString ll_name = Macro__GET_LLx_NAME(ll_i);

			CString cmmd_ll;
			cmmd_ll.Format("%s_SV_CLOSE", ll_name);

			if(dCH__CFG_LLx_CLOSE_SEQUENCE_MODE->Check__DATA(STR__ENABLE) > 0)
			{
				if(pTMC_VLV__OBJ_CTRL->Call__OBJECT(cmmd_ll) < 0)
				{
					return -201;
				}
			}	
			else
			{
				if(pTMC_VLV__OBJ_CTRL->Run__OBJECT(cmmd_ll) < 0)
				{
					return -211;
				}
			}
		}
		else
		{
			return -203;
		}
	}

	return OBJ_AVAILABLE;
}

int  CObj__VAC_ROBOT_STD::
Fnc__PMx_PICK(CII_OBJECT__VARIABLE* p_variable,
			  CII_OBJECT__ALARM* p_alarm,
			  const CString& para_arm,
			  const CString& para_module,
			  const CString& para_slot,
			  const int ex_flag,
			  const int handoff_mode)
{
	int pm_index = Macro__CHECK_PMx_INDEX(para_module);
	if(pm_index < 0)
	{
		return OBJ_ABORT;
	}

	// Material Check ...
	if(Interlock__CHECK_MATERIAL(p_alarm,-1,para_arm,para_module,para_slot) < 0)
	{
		return -1;
	}

	// Active Handoff ...
	if(handoff_mode == ACTIVE_HANDOFF)
	{
		int flag;
		flag = Fnc__ACT_HOFF_PICK(p_variable,
								  p_alarm,
								  para_arm,
								  para_module,
								  para_slot,
								  pm_index,
								  handoff_mode);
		if(flag < 0)
		{
			return -112;
		}
	}
	else
	{
		if(Fnc__CALL_PICK(p_variable,
						  p_alarm,
						  para_arm,
						  para_module,
						  para_slot) < 0)
		{
			return -112;
		}
	}

	if(ex_flag > 0)
	{

	}
	else
	{
		if(dCH__CFG_PMx_CLOSE_SEQUENCE_MODE->Check__DATA(STR__ENABLE) > 0)
		{
			if(Fnc__PMx_SV_CLOSE(pm_index+1) < 0)
			{
				return -203;
			}
		}
		else
		{
			if(Run__PMx_SV_CLOSE(pm_index+1) < 0)
			{
				return -213;
			}
		}
	}

	/*
	// Active Handoff ...
	if(handoff_mode == ACTIVE_HANDOFF)
	{
		if(Sub_OBJCALL_PMC_ACT_HANDOFF(pm_index, "S3") < 0)
		{
			return OBJ_ABORT;
		}
	}
	*/

	return OBJ_AVAILABLE;
}

int  CObj__VAC_ROBOT_STD::
Fnc__PMx_PLACE(CII_OBJECT__VARIABLE* p_variable,
			   CII_OBJECT__ALARM* p_alarm,
			   const CString& para_arm,
			   const CString& para_module,
			   const CString& para_slot,
			   const int ex_flag,
			   const int handoff_mode)
{
	// ...
	int pm_index = Macro__CHECK_PMx_INDEX(para_module);

	if(pm_index < 0)
	{
		return OBJ_ABORT;
	}

	// Material Check -----
	if(Interlock__CHECK_MATERIAL(p_alarm,1,para_arm,para_module,para_slot) < 0)
	{
		return -1;
	}
	
	// ------------ Active Handoff
	// 1. PIN DOWN
	if(handoff_mode == ACTIVE_HANDOFF)
	{
		if(Sub_OBJCALL_PMC_ACT_HANDOFF(pm_index, "R1") < 0)
		{
			return OBJ_ABORT;
		}
	}

	// ------------ Active Handoff
	if(handoff_mode == ACTIVE_HANDOFF)
	{
		int flag;
		flag = Fnc__ACT_HOFF_PLACE(p_variable,
								   p_alarm,
								   para_arm,
								   para_module,
								   para_slot,
								   pm_index,
								   handoff_mode);
		if(flag < 0)
		{
			return -112;
		}
	}
	else
	{
		if(Fnc__CALL_PLACE(p_variable,
						  p_alarm,
						  para_arm,
						  para_module,
						  para_slot) < 0)
		{
			return -112;
		}
	}
	
	if(ex_flag > 0)
	{

	}
	else
	{
		if(dCH__CFG_PMx_CLOSE_SEQUENCE_MODE->Check__DATA(STR__ENABLE) > 0)
		{
			if(Fnc__PMx_SV_CLOSE(pm_index+1) < 0)
			{
				return -203;
			}
		}
		else
		{
			if(Run__PMx_SV_CLOSE(pm_index+1) < 0)
			{
				return -213;
			}
		}
	}

	// ------------ Active Handoff
	if(handoff_mode == ACTIVE_HANDOFF)
	{
		if(sEXT_CH__CTC_CUR_MODE->Check__DATA(STR__MAINT) > 0)
		{
			if(Sub_OBJCALL_PMC_ACT_HANDOFF(pm_index, "R3") < 0)
			{
				return OBJ_ABORT;
			}
		}
	}

	return OBJ_AVAILABLE;
}


//----------------------------------------------------------------------------------------------------
int  CObj__VAC_ROBOT_STD::
Seq__PICK(CII_OBJECT__VARIABLE* p_variable,
		  CII_OBJECT__ALARM* p_alarm,
		  const CString& para_arm,
		  const CString& para_module,
		  const CString& para_slot,
		  const int ex_flag,
		  const int handoff_mode)
{
	int flag = OBJ_ABORT;
	int slot_id = atoi(para_slot);

	// ...
	CString act_name;
	CString str_log;

	// ...
	{
		act_name.Format("pick from %s-%s to Arm-%s",
						para_module,
						para_slot,
						para_arm);

		Fnc__APP_LOG(act_name);
	}

	// ...
	int cfg__auto_pumping = 1;

	if((dEXT_CH__CFG_SETUP_TEST_MODE->Check__DATA(STR__ENABLE) > 0)
	&& (dEXT_CH__CFG_SETUP_AUTO_VENTING_PUMPING->Check__DATA(STR__DISABLE) > 0))
	{
		cfg__auto_pumping = -1;
	}

	if(cfg__auto_pumping > 0)
	{
		if(Fnc__TRANSFER_AUTO_PUMPING(para_arm, para_module, para_slot) < 0)
		{
			return OBJ_ABORT;
		}
	}

	// ...
	{
		act_name.Format("Run_SV_OPEN[%s] : Start ...", para_module);
		Fnc__APP_LOG(act_name);

		if(Fnc__RUN_SV_OPEN(para_module) < 0)
		{
			return OBJ_ABORT;
		}

		act_name.Format("Run_ROTATE[%s] : Start ...", para_module);
		Fnc__APP_LOG(act_name);

		if(Fnc__RUN_ROTATE(p_variable,
						   p_alarm,
						   para_arm,
						   para_module,
						   para_slot) < 0)
		{
			return OBJ_ABORT;
		}

		act_name.Format("TMC_VLV : Checking ...");
		Fnc__APP_LOG(act_name);

		if(pTMC_VLV__OBJ_CTRL->When__OBJECT() < 0)
		{
			return -222;
		}

		act_name.Format("ROBOT : Checking ...");
		Fnc__APP_LOG(act_name);

		if(pROBOT__OBJ_CTRL->When__OBJECT() < 0)
		{
			return -111;
		}
	}

	// ...
	int ll_i = Macro__CHECK_LLx_INDEX(para_module);
	if(ll_i >= 0)
	{
		flag = Fnc__LBx_PICK(p_variable, 
							 p_alarm,
							 para_arm, 
							 para_module, 
							 para_slot,
							 ex_flag);
	}
	else
	{
		flag = Fnc__PMx_PICK(p_variable, 
							 p_alarm,
							 para_arm, 
							 para_module, 
							 para_slot,
							 ex_flag,
							 handoff_mode);		
	}

	return flag;
}
int  CObj__VAC_ROBOT_STD::
Seq__PLACE(CII_OBJECT__VARIABLE* p_variable,
		   CII_OBJECT__ALARM* p_alarm,
		   const CString& para_arm,
		   const CString& para_module,
		   const CString& para_slot,
		   const int ex_flag,
		   const int handoff_mode)
{
	// ...
	int flag = OBJ_ABORT;
	int slot_id = atoi(para_slot);

	// ...
	CString act_name;
	CString str_log;
	
	// ...
	{
		act_name.Format("place from Arm-%s to %s-%s",
						para_arm,
						para_module,
						para_slot);

		Fnc__APP_LOG(act_name);
	}

	// ...
	CString str_from_ctc_wac_use;
	CString str_from_ctc_wac_pos;
	CString str_from_ctc_wac_delay;
	double cfg__wac_delay;

	SCX__TIMER_CTRL x_timer__wac_delay;
	x_timer__wac_delay->REGISTER__ABORT_OBJECT(sObject_Name);

	int pm_index = Macro__CHECK_PMx_INDEX(para_module);
	if(pm_index >= 0)
	{
		sCH__FROM_CTC_WAC_PM_USE_X[pm_index]->Get__DATA(str_from_ctc_wac_use);

		if(str_from_ctc_wac_use.CompareNoCase(STR__YES) == 0)
		{
			sCH__FROM_CTC_WAC_PM_POS_X[pm_index]->Get__DATA(str_from_ctc_wac_pos);

			sCH__FROM_CTC_WAC_PM_DELAY_SEC_X[pm_index]->Get__DATA(str_from_ctc_wac_delay);
			cfg__wac_delay = atof(str_from_ctc_wac_delay);

			//
			sCH__INFO_WAC_PMC_STATE->Set__DATA(para_module);
			sCH__INFO_WAC_PMC_POS->Set__DATA(str_from_ctc_wac_pos);

			//
			x_timer__wac_delay->REGISTER__COUNT_CHANNEL(sCH__INFO_WAC_PMC_DELAY_COUNT->Get__CHANNEL_NAME());
			x_timer__wac_delay->INIT__COUNT_DOWN();
		}
	}

	// ...
	int cfg__auto_pumping = 1;

	if((dEXT_CH__CFG_SETUP_TEST_MODE->Check__DATA(STR__ENABLE) > 0)
	&& (dEXT_CH__CFG_SETUP_AUTO_VENTING_PUMPING->Check__DATA(STR__DISABLE) > 0))
	{
		cfg__auto_pumping = -1;
	}

	if(cfg__auto_pumping > 0)
	{
		// ...
		{
			act_name.Format("Transfer Auto Pumping - Started ...");		
			Fnc__APP_LOG(act_name);
		}

		if(Fnc__TRANSFER_AUTO_PUMPING(para_arm, para_module, para_slot) < 0)
		{
			return OBJ_ABORT;
		}

		// ...
		{
			act_name.Format("Transfer Auto Pumping - Completed ...");		
			Fnc__APP_LOG(act_name);
		}
	}

	if((str_from_ctc_wac_use.CompareNoCase(STR__YES) == 0) 
	&& (pm_index >= 0))
	{
		// SV OPEN -> START ...
		{
			// ...
			{
				act_name.Format("Start SV Open : (%s)", para_module);
				Fnc__APP_LOG(act_name);
			}

			if(Fnc__RUN_SV_OPEN(para_module) < 0)
			{
				return OBJ_ABORT;
			}
		}

		// WAC DELAY ...
		{
			// ...
			{
				act_name.Format("WAC Delay - Started : (%.1f)", cfg__wac_delay);
				Fnc__APP_LOG(act_name);
			}
	
			if(x_timer__wac_delay->WAIT(cfg__wac_delay) < 0)
			{
				return OBJ_ABORT;
			}
		}

		// ROBOT ROTATE ...
		{
			// ...
			{
				act_name.Format("Call Rotate - Started : (%.1f)", cfg__wac_delay);
				Fnc__APP_LOG(act_name);
			}

			if(Fnc__CALL_ROTATE(p_variable,
							    p_alarm,
							    para_arm,
							    para_module,
							    para_slot) < 0)
			{
				return OBJ_ABORT;
			}

			// ...
			{
				act_name.Format("Call Rotate - Completed : (%.1f)", cfg__wac_delay);
				Fnc__APP_LOG(act_name);
			}
		}

		// SV OPEN -> WAIT ...
		{
			if(Fnc__WAIT_SV_OPEN(para_module) < 0)
			{
				return OBJ_ABORT;
			}

			// ...
			{
				act_name.Format("Wait SV Open : (%s)", para_module);
				Fnc__APP_LOG(act_name);
			}
		}
	}
	else
	{
		// ...
		{
			act_name.Format("SV Open & Rotate Run - Started : (%s)", para_module);
			Fnc__APP_LOG(act_name);
		}

		if(Fnc__RUN_SV_OPEN(para_module) < 0)
		{
			return OBJ_ABORT;
		}

		if(Fnc__RUN_ROTATE(p_variable,
						   p_alarm,
						   para_arm,
						   para_module,
						   para_slot) < 0)
		{
			return OBJ_ABORT;
		}

		if(pTMC_VLV__OBJ_CTRL->When__OBJECT() < 0)
		{
			return -222;
		}

		if(pROBOT__OBJ_CTRL->When__OBJECT() < 0)
		{
			return -111;
		}

		// ...
		{
			act_name.Format("SV Open & Rotate Run - Completed : (%s)", para_module);
			Fnc__APP_LOG(act_name);
		}
	}

	// ...
	int ll_i = Macro__CHECK_LLx_INDEX(para_module);
	if(ll_i >= 0)
	{
		flag = Fnc__LBx_PLACE(p_variable, 
							 p_alarm,
							 para_arm, 
							 para_module, 
							 para_slot,
							 ex_flag);
	}
	else
	{
		flag = Fnc__PMx_PLACE(p_variable, 
							 p_alarm,
							 para_arm, 
							 para_module, 
							 para_slot,
							 ex_flag,
							 handoff_mode);		
	}

	return flag;
}

int  CObj__VAC_ROBOT_STD::
Call__RETRACT(CII_OBJECT__VARIABLE* p_variable,
			  CII_OBJECT__ALARM* p_alarm,
			  const CString& arm_type,
			  const CString& stn_name, 
			  const CString& stn_slot)
{
	//.....
	CStringArray l_para;
	int flag;

	// ...
	{
		l_para.Add(arm_type);
		l_para.Add(stn_name);
		l_para.Add(stn_slot);

		flag = pROBOT__OBJ_CTRL->Call__OBJ_MODE(CMMD__RETRACT,l_para);
	}
	return flag;
}

int  CObj__VAC_ROBOT_STD::
Call__EXTEND(CII_OBJECT__VARIABLE* p_variable,
			 CII_OBJECT__ALARM* p_alarm,
			 const CString& arm_type,
			 const CString& stn_name, 
			 const CString& stn_slot)
{
	//.....
	CStringArray l_para;
	int flag;

	// ...
	{
		l_para.Add(arm_type);
		l_para.Add(stn_name);
		l_para.Add(stn_slot);

		flag = pROBOT__OBJ_CTRL->Call__OBJ_MODE(CMMD__EXTEND,l_para);
	}
	return flag;
}

int  CObj__VAC_ROBOT_STD::
Call__GOUP(CII_OBJECT__VARIABLE* p_variable,
		   CII_OBJECT__ALARM* p_alarm,
		   const CString& arm_type,
		   const CString& stn_name, 
		   const CString& stn_slot)
{
	//.....
	CStringArray l_para;
	int flag;

	// ...
	{
		l_para.Add(arm_type);
		l_para.Add(stn_name);
		l_para.Add(stn_slot);

		flag = pROBOT__OBJ_CTRL->Call__OBJ_MODE(CMMD__GOUP,l_para);
	}
	return flag;
}

int  CObj__VAC_ROBOT_STD::
Call__GODOWN(CII_OBJECT__VARIABLE* p_variable,
			 CII_OBJECT__ALARM* p_alarm,
			 const CString& arm_type,
			 const CString& stn_name, 
			 const CString& stn_slot)
{
	//.....
	CStringArray l_para;
	int flag;

	// ...
	{
		l_para.Add(arm_type);
		l_para.Add(stn_name);
		l_para.Add(stn_slot);

		flag = pROBOT__OBJ_CTRL->Call__OBJ_MODE(CMMD__GODOWN,l_para);
	}
	return flag;
}

int  CObj__VAC_ROBOT_STD::
Call__RQ_STN(CII_OBJECT__VARIABLE* p_variable,
			 CII_OBJECT__ALARM* p_alarm,
			 const CString& arm_type,
			 const CString& stn_name)
{
	//.....
	CString act_name;

	// ...
	{
		act_name.Format("RQ STN from Arm-%s to %s",
		                arm_type,
						stn_name);
	
		Fnc__APP_LOG(act_name);
	}

	// ...
	CStringArray l_para;
	int flag;

	// ...
	{
		l_para.Add(arm_type);
		l_para.Add(stn_name);
		
		flag = pROBOT__OBJ_CTRL->Call__OBJ_MODE("RQ_STN",l_para);
	}
	return flag;
}

int  CObj__VAC_ROBOT_STD::
Call__SET_STN(CII_OBJECT__VARIABLE* p_variable,
			  CII_OBJECT__ALARM* p_alarm,
			  const CString& arm_type,
			  const CString& stn_name)
{
	//.....
	CString act_name;

	// ...
	{
		act_name.Format("SET STN from Arm-%s to %s",
			            arm_type,
						stn_name);
	
		Fnc__APP_LOG(act_name);
	}

	// ...
	CStringArray l_para;
	int flag;

	// ...
	{
		l_para.Add(arm_type);
		l_para.Add(stn_name);
		
		flag = pROBOT__OBJ_CTRL->Call__OBJ_MODE("SET_STN",l_para);
	}
	return flag;
}

int  CObj__VAC_ROBOT_STD::
Call__TEACHED_CPTR_SAVE(CII_OBJECT__VARIABLE* p_variable,
						CII_OBJECT__ALARM* p_alarm,
						const CString& arm_type,
						const CString& stn_name)
{
	//.....
	CString act_name;

	// ...
	{
		act_name.Format("TEACHED CPTR SAVE from Arm-%s to %s",
			            arm_type,
						stn_name);
	
		Fnc__APP_LOG(act_name);
	}

	// ...
	CStringArray l_para;
	int flag;

	// ...
	{
		l_para.Add(arm_type);
		l_para.Add(stn_name);
		
		flag = pROBOT__OBJ_CTRL->Call__OBJ_MODE("TEACHED_CPTR_SAVE",l_para);
	}
	return flag;
}


