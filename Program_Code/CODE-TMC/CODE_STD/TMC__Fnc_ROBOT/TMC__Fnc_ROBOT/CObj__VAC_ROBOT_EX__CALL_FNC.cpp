#include "StdAfx.h"
#include "CObj__VAC_ROBOT_EX.h"
#include "CObj__VAC_ROBOT_EX__ALID.h"
#include "CObj__VAC_ROBOT_EX__DEF.h"

#include "Macro_Function.h"


//----------------------------------------------------------------------------------------------------
int  CObj__VAC_ROBOT_EX::
Call__HOME(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm)
{
	return pROBOT__OBJ_CTRL->Call__OBJECT(CMMD__HOME);
}

int  CObj__VAC_ROBOT_EX::
Call__MAP(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm)
{
	return pROBOT__OBJ_CTRL->Call__OBJECT(CMMD__MAP);
}


// ...
int  CObj__VAC_ROBOT_EX::
Call__INIT(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm)
{
	return pROBOT__OBJ_CTRL->Call__OBJECT(CMMD__INIT);
}

int  CObj__VAC_ROBOT_EX::
Call__SET_PARA(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm)
{
	return pROBOT__OBJ_CTRL->Call__OBJECT(CMMD__SET_PARA);
}

//
int  CObj__VAC_ROBOT_EX::
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
int  CObj__VAC_ROBOT_EX::
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

int  CObj__VAC_ROBOT_EX::
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
int  CObj__VAC_ROBOT_EX::
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
int  CObj__VAC_ROBOT_EX::
Call__ROTATE(CII_OBJECT__VARIABLE* p_variable,
			 CII_OBJECT__ALARM* p_alarm,
		     const CString& para_arm,
		     const CString& para_module,
		     const CString& para_slot)
{

	return Fnc__CALL_ROTATE(p_variable,
							p_alarm,
							para_arm,
							para_module,
							para_slot);
}

int  CObj__VAC_ROBOT_EX::
Fnc__LBx_PICK(CII_OBJECT__VARIABLE* p_variable,
			  CII_OBJECT__ALARM* p_alarm,
			  const CString& para_arm,
			  const CString& para_module,
			  const CString& para_slot,
			  const int ex_flag)
{
	CString str_log;

	// ...
	CStringArray l__arm_type;
	CStringArray l__stn_name;
	CStringArray l__stn_slot;

	_Get__ARM_INFO(para_arm,para_module,para_slot, l__arm_type,l__stn_name,l__stn_slot);

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

		//
		int k_limit = l__arm_type.GetSize();

		log_bff.Format(" * parameter limit <- [%1d] ... \n", k_limit);
		str_log += log_bff;

		for(int k=0; k<k_limit; k++)
		{
			log_bff.Format(" ** %1d) [%s] [%s] [%s] \n", k+1,l__arm_type[k],l__stn_name[k],l__stn_slot[k]);
			str_log += log_bff;
		}

		Fnc__APP_LOG(str_log);
	}

	// ...
	int active_mode  = -1;
	int robot_z_move =  1;

	// LLx ...
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
		// LLx : PICK_READY ...
		if(ll_i >= 0)
		{
			int k_limit = l__stn_slot.GetSize();

			for(int k=0; k<k_limit; k++)
			{
				CString cur__stn_slot = l__stn_slot[k];

				aEXT_CH__LLx__PARA_HANDSHAKE_SLOT[ll_i]->Set__DATA(cur__stn_slot);
			
				if(pLLx__OBJ_CTRL[ll_i]->Call__OBJECT(CMMD__PICK_READY) < 0)
				{
					return -101;
				}
			}
		}

		if(robot_z_move > 0)
		{
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

			if(pROBOT__OBJ_CTRL->Call__OBJ_MODE(CMMD__EXTEND, l_para) < 0)
			{
				str_log.Format("Lower Object Extend Aborted ...");
				Fnc__APP_LOG(str_log);
				return -122;
			}

			if(ll_i >= 0)
			{
				int k_limit = l__stn_slot.GetSize();

				for(int k=0; k<k_limit; k++)
				{
					CString cur__stn_slot = l__stn_slot[k];

					aEXT_CH__LLx__PARA_HANDSHAKE_SLOT[ll_i]->Set__DATA(cur__stn_slot);

					if(pLLx__OBJ_CTRL[ll_i]->Call__OBJECT(CMMD__PICK_EXTEND) < 0)
					{
						return -101;
					}
				}
			}

			// ROBOT : RETRACT
			{
				dEXT_CH__PHY_ROBOT__PARA_ACTIVE_HANDOFF_REQ->Set__DATA(STR__ON);
				dEXT_CH__PHY_ROBOT__PARA_ACTIVE_HANDOFF_ACT->Set__DATA(STR__PICK);

				if(pROBOT__OBJ_CTRL->Call__OBJ_MODE(CMMD__RETRACT,l_para) < 0)
				{
					str_log.Format("Lower Object Retract Aborted..");
					Fnc__APP_LOG(str_log);
					return -124;
				}
			}

			Fnc__CHANGE_MATERIAL_INFO(-1, para_arm,para_module,para_slot);
		}

		// LLx : PICK_COMPLETE
		if(ll_i >= 0)
		{
			int k_limit = l__stn_slot.GetSize();

			for(int k=0; k<k_limit; k++)
			{
				CString cur__stn_slot = l__stn_slot[k];

				aEXT_CH__LLx__PARA_HANDSHAKE_SLOT[ll_i]->Set__DATA(cur__stn_slot);
	
				if(pLLx__OBJ_CTRL[ll_i]->Call__OBJECT(CMMD__PICK_COMPLETE) < 0)
				{
					return -103;
				}
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

		if(ll_i >= 0)
		{
			CString para_data = sCH__PARA_MANUAL_MOVE_LLx_SLOT_CLOSE_SKIP->Get__STRING();

			if(para_data.Find(STR__YES) >= 0)
			{
				CString str_bff;

				str_log.Format("LL%1d - NOT SV_VLOSE ! \n", ll_i+1);

				str_bff.Format("  * %s <- %s \n", 
							   sCH__PARA_MANUAL_MOVE_LLx_SLOT_CLOSE_SKIP->Get__CHANNEL_NAME(),
							   sCH__PARA_MANUAL_MOVE_LLx_SLOT_CLOSE_SKIP->Get__STRING());
				str_log += str_bff;

				Fnc__APP_LOG(str_log);
			}
			else
			{
				if(bActive__LLx_MANIFOLD_X)
				{
					if(ll_i < iSIZE__LLx)
					{
						int k_limit = l__stn_slot.GetSize();

						for(int k=0; k<k_limit; k++)
						{
							CString cmmd__sv_close = "SV_TRANSFER.CLOSE";

							CStringArray l__cmmd_slot;
							l__cmmd_slot.Add(l__stn_slot[k]);

							if(dCH__CFG_LLx_CLOSE_SEQUENCE_MODE->Check__DATA(STR__ENABLE) > 0)
							{
								if(pLLx__OBJ_CTRL[ll_i]->Call__OBJ_MODE(cmmd__sv_close, l__cmmd_slot) < 0)		return -301;
							}
							else
							{
								if(pLLx__OBJ_CTRL[ll_i]->Run__OBJ_MODE(cmmd__sv_close, l__cmmd_slot) < 0)		return -302;
							}
						}
					}
					else
					{
						return -303;
					}
				}
				else
				{
					CString ll_name = Macro__GET_LLx_NAME(ll_i);

					CString cmmd__sv_close;
					cmmd__sv_close.Format("%s_SV_CLOSE", ll_name);

					if(dCH__CFG_LLx_CLOSE_SEQUENCE_MODE->Check__DATA(STR__ENABLE) > 0)
					{
						if(pTMC_VLV__OBJ_CTRL->Call__OBJECT(cmmd__sv_close) < 0)			return -201;
					}
					else
					{
						if(pTMC_VLV__OBJ_CTRL->Run__OBJECT(cmmd__sv_close) < 0)				return -202;
					}
				}
			}

			// ...
		}
		else
		{
			return -203;
		}
	}

	return 1;
}
int  CObj__VAC_ROBOT_EX::
Fnc__PMx_PICK(CII_OBJECT__VARIABLE* p_variable,
			  CII_OBJECT__ALARM* p_alarm,
			  const CString& para_arm,
			  const CString& para_module,
			  const CString& para_slot,
			  const int ex_flag,
			  const int handoff_mode)
{
	CString str_log;

	// ...
	CStringArray l__arm_type;
	CStringArray l__stn_name;
	CStringArray l__stn_slot;

	_Get__ARM_INFO(para_arm,para_module,para_slot, l__arm_type,l__stn_name,l__stn_slot);

	// ...
	{
		CString log_bff;

		str_log = "Fnc__PMx_PICK() : Start ...\n";

		log_bff.Format("  * para_arm    <- %s  \n", para_arm);
		str_log += log_bff;
		log_bff.Format("  * para_module <- %s  \n", para_module);
		str_log += log_bff;
		log_bff.Format("  * para_slot   <- %s  \n", para_slot);
		str_log += log_bff;
		log_bff.Format("  * ex_flag     <- %1d \n", ex_flag);
		str_log += log_bff;

		log_bff.Format("  * %s <- %s \n", 
					   dCH__CFG_PMx_CLOSE_SEQUENCE_MODE->Get__CHANNEL_NAME(),
				       dCH__CFG_PMx_CLOSE_SEQUENCE_MODE->Get__STRING());
		str_log += log_bff;

		//
		int k_limit = l__arm_type.GetSize();

		log_bff.Format(" * parameter limit <- [%1d] ... \n", k_limit);
		str_log += log_bff;

		for(int k=0; k<k_limit; k++)
		{
			log_bff.Format(" ** %1d) [%s] [%s] [%s] \n", k+1,l__arm_type[k],l__stn_name[k],l__stn_slot[k]);
			str_log += log_bff;
		}

		Fnc__APP_LOG(str_log);
	}

	// ...
	int pm_index = Macro__CHECK_PMx_INDEX(para_module);
	if(pm_index < 0)
	{
		return -11;
	}

	// Material Check ...
	if(Interlock__CHECK_MATERIAL(p_alarm,-1,para_arm,para_module,para_slot) < 0)
	{
		return -12;
	}

	// Active Handoff ...
	if(handoff_mode == ACTIVE_HANDOFF)
	{
		int flag;

		flag = Fnc__ACT_HOFF_PICK(p_variable,
								  p_alarm,
								  para_arm,
								  para_module,
								  para_slot);

		if(flag < 0)
		{
			return -112;
		}
	}
	else
	{
		if(Fnc__PASSIVE_TRANSFER_READY(pm_index) < 0)
		{
			return -111;
		}

		if(Fnc__CALL_PICK(p_variable,
						  p_alarm,
						  para_arm,
						  para_module,
						  para_slot) < 0)
		{
			return -112;
		}

		if(ex_flag < 0)
		{
			if(Fnc__PASSIVE_TRANSFER_END(pm_index) < 0)
			{
				return -113;
			}
		}
	}

	if(ex_flag > 0)
	{

	}
	else
	{
		CString para_data = sCH__PARA_MANUAL_MOVE_PMx_SLOT_CLOSE_SKIP->Get__STRING();

		if(para_data.CompareNoCase(STR__YES) >= 0)
		{
			CString str_bff;

			str_log.Format("PM%1d - NOT SV_CLOSE ! \n", pm_index + 1);

			str_bff.Format("  * %s <- %s \n", 
							sCH__PARA_MANUAL_MOVE_PMx_SLOT_CLOSE_SKIP->Get__CHANNEL_NAME(),
							sCH__PARA_MANUAL_MOVE_PMx_SLOT_CLOSE_SKIP->Get__STRING());
			str_log += str_bff;

			Fnc__APP_LOG(str_log);			
		}
		else
		{
			int k_limit = l__stn_name.GetSize();
	
			for(int k=0; k<k_limit; k++)
			{
				CString cur__stn_name = l__stn_name[k];
	
				int p_index = Macro__CHECK_PMx_INDEX(cur__stn_name);
				if(p_index < 0)		continue;

				int pm_id = p_index + 1;

				if(dCH__CFG_PMx_CLOSE_SEQUENCE_MODE->Check__DATA(STR__ENABLE) > 0)
				{
					if(Fnc__PMx_SV_CLOSE(pm_id) < 0)
					{
						return -203;
					}
				}
				else
				{
					if(Run__PMx_SV_CLOSE(pm_id) < 0)
					{
						return -213;
					}
				}
			}
		}

		// ...
	}

	return 1;
}

int  CObj__VAC_ROBOT_EX
::Fnc__LBx_PLACE(CII_OBJECT__VARIABLE* p_variable,
			     CII_OBJECT__ALARM* p_alarm,
			     const CString& para_arm,
			     const CString& para_module,
			     const CString& para_slot,
			     const int ex_flag)
{
	CString str_log;

	// ...
	CStringArray l__arm_type;
	CStringArray l__stn_name;
	CStringArray l__stn_slot;

	_Get__ARM_INFO(para_arm,para_module,para_slot, l__arm_type,l__stn_name,l__stn_slot);

	// ...
	{
		CString log_bff;

		str_log = "Fnc__LBx_PLACE() : Start ...\n";

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

		//
		int k_limit = l__arm_type.GetSize();

		log_bff.Format(" * parameter limit <- [%1d] ... \n", k_limit);
		str_log += log_bff;

		for(int k=0; k<k_limit; k++)
		{
			log_bff.Format(" ** %1d) [%s] [%s] [%s] \n", k+1,l__arm_type[k],l__stn_name[k],l__stn_slot[k]);
			str_log += log_bff;
		}

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
			int k_limit = l__stn_slot.GetSize();

			for(int k=0; k<k_limit; k++)
			{
				CStringArray l__cmmd_slot;
				l__cmmd_slot.Add(l__stn_slot[k]);

				if(pLLx__OBJ_CTRL[ll_i]->Call__OBJ_MODE(CMMD__PLACE_READY, l__cmmd_slot) < 0)
				{
					return -101;
				}
			}
		}

		if(robot_z_move > 0)
		{
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
			{
				if(pROBOT__OBJ_CTRL->Call__OBJ_MODE(CMMD__EXTEND,l_para) < 0)
				{
					str_log.Format("Lower Object Extend Aborted..");
					Fnc__APP_LOG(str_log);
					return -122;
				}
			}

			// LLx : PLACE_EXTEND
			if(ll_i >= 0)
			{
				int k_limit = l__stn_slot.GetSize();

				for(int k=0; k<k_limit; k++)
				{
					CStringArray l__cmmd_slot;
					l__cmmd_slot.Add(l__stn_slot[k]);

					if(pLLx__OBJ_CTRL[ll_i]->Call__OBJ_MODE(CMMD__PLACE_EXTEND, l__cmmd_slot) < 0)
					{
						return -102;
					}
				}
			}

			// ROBOT : RETRACT
			{
				dEXT_CH__PHY_ROBOT__PARA_ACTIVE_HANDOFF_REQ->Set__DATA(STR__ON);
				dEXT_CH__PHY_ROBOT__PARA_ACTIVE_HANDOFF_ACT->Set__DATA(STR__PLACE);
				
				if(pROBOT__OBJ_CTRL->Call__OBJ_MODE(CMMD__RETRACT, l_para) < 0)
				{
					str_log.Format("Lower Object Retract Aborted..");
					Fnc__APP_LOG(str_log);
					return -124;
				}
			}

			Fnc__CHANGE_MATERIAL_INFO(1, para_arm,para_module,para_slot);
		}

		// LLx : PLACE_COMPLETE
		if(ll_i >= 0)
		{
			int k_limit = l__stn_slot.GetSize();

			for(int k=0; k<k_limit; k++)
			{
				CStringArray l__cmmd_slot;
				l__cmmd_slot.Add(l__stn_slot[k]);

				if(pLLx__OBJ_CTRL[ll_i]->Call__OBJ_MODE(CMMD__PLACE_COMPLETE, l__cmmd_slot) < 0)
				{
					return -103;
				}
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

		if(ll_i >= 0)
		{
			CString para_data = sCH__PARA_MANUAL_MOVE_LLx_SLOT_CLOSE_SKIP->Get__STRING();

			if(para_data.Find(STR__YES) >= 0)
			{
				CString str_bff;

				str_log.Format("LL%1d - NOT SV_VLOSE ! \n", ll_i+1);

				str_bff.Format("  * %s <- %s \n", 
								sCH__PARA_MANUAL_MOVE_LLx_SLOT_CLOSE_SKIP->Get__CHANNEL_NAME(),
								sCH__PARA_MANUAL_MOVE_LLx_SLOT_CLOSE_SKIP->Get__STRING());
				str_log += str_bff;

				Fnc__APP_LOG(str_log);
			}
			else
			{
				if(bActive__LLx_MANIFOLD_X)
				{
					if(ll_i < iSIZE__LLx)
					{
						int k_limit = l__stn_slot.GetSize();

						for(int k=0; k<k_limit; k++)
						{
							CString cmmd__sv_close = "SV_TRANSFER.CLOSE";

							CStringArray l__cmmd_slot;
							l__cmmd_slot.Add(l__stn_slot[k]);

							if(dCH__CFG_LLx_CLOSE_SEQUENCE_MODE->Check__DATA(STR__ENABLE) > 0)
							{
								if(pLLx__OBJ_CTRL[ll_i]->Call__OBJ_MODE(cmmd__sv_close, l__cmmd_slot) < 0)
								{
									return -301;
								}
							}
							else
							{
								if(pLLx__OBJ_CTRL[ll_i]->Run__OBJ_MODE(cmmd__sv_close, l__cmmd_slot) < 0)
								{
									return -302;
								}
							}
						}
					}
					else
					{
						return -303;
					}
				}
				else
				{
					CString ll_name = Macro__GET_LLx_NAME(ll_i);
					CString cmmd__sv_close;

					int k_limit = l__stn_slot.GetSize();

					for(int k=0; k<k_limit; k++)
					{
						cmmd__sv_close.Format("%s_SV_CLOSE", ll_name);

						CStringArray l__cmmd_slot;
						l__cmmd_slot.Add(l__stn_slot[k]);

						if(dCH__CFG_LLx_CLOSE_SEQUENCE_MODE->Check__DATA(STR__ENABLE) > 0)
						{
							if(pTMC_VLV__OBJ_CTRL->Call__OBJ_MODE(cmmd__sv_close, l__cmmd_slot) < 0)
							{
								return -201;
							}
						}		
						else
						{
							if(pTMC_VLV__OBJ_CTRL->Run__OBJ_MODE(cmmd__sv_close, l__cmmd_slot) < 0)
							{
								return -202;
							}
						}
					}
				}
			}

			// ...
		}
		else
		{
			return -203;
		}
	}

	return 1;
}
int  CObj__VAC_ROBOT_EX
::Fnc__PMx_PLACE(CII_OBJECT__VARIABLE* p_variable,
			     CII_OBJECT__ALARM* p_alarm,
			     const CString& para_arm,
			     const CString& para_module,
			     const CString& para_slot,
			     const int ex_flag,
			     const int handoff_mode)
{
	CString str_log;

	// ...
	CStringArray l__arm_type;
	CStringArray l__stn_name;
	CStringArray l__stn_slot;

	_Get__ARM_INFO(para_arm,para_module,para_slot, l__arm_type,l__stn_name,l__stn_slot);

	// ...
	{
		CString log_bff;

		str_log = "Fnc__PMx_PLACE() : Start ...\n";

		log_bff.Format("  * para_arm    <- %s  \n", para_arm);
		str_log += log_bff;
		log_bff.Format("  * para_module <- %s  \n", para_module);
		str_log += log_bff;
		log_bff.Format("  * para_slot   <- %s  \n", para_slot);
		str_log += log_bff;
		log_bff.Format("  * ex_flag     <- %1d \n", ex_flag);
		str_log += log_bff;

		log_bff.Format("  * %s <- %s \n", 
						dCH__CFG_PMx_CLOSE_SEQUENCE_MODE->Get__CHANNEL_NAME(),
						dCH__CFG_PMx_CLOSE_SEQUENCE_MODE->Get__STRING());
		str_log += log_bff;

		//
		int k_limit = l__arm_type.GetSize();

		log_bff.Format(" * parameter limit <- [%1d] ... \n", k_limit);
		str_log += log_bff;

		for(int k=0; k<k_limit; k++)
		{
			log_bff.Format(" ** %1d) [%s] [%s] [%s] \n", k+1,l__arm_type[k],l__stn_name[k],l__stn_slot[k]);
			str_log += log_bff;
		}

		Fnc__APP_LOG(str_log);
	}

	// ...
	int pm_index = Macro__CHECK_PMx_INDEX(para_module);
	if(pm_index < 0)
	{
		return -11;
	}

	// Material Check ...
	if(Interlock__CHECK_MATERIAL(p_alarm,1,para_arm,para_module,para_slot) < 0)
	{
		return -12;
	}
	
	if(handoff_mode == ACTIVE_HANDOFF)
	{
		// PMx : PIN DOWN
		{
			int k_limit = l__stn_name.GetSize();

			for(int k=0; k<k_limit; k++)
			{
				CString cur__stn_name = l__stn_name[k];

				int p_index = Macro__CHECK_PMx_INDEX(cur__stn_name);
				if(p_index < 0)			return -101;

				if(Sub_OBJCALL_PMC_ACT_HANDOFF(p_index, "R1") < 0)
				{
					return -102;
				}
			}
		}

		// ROBOT : PLACE 
		{
			int flag = Fnc__ACT_HOFF_PLACE(p_variable,
										   p_alarm,
										   para_arm,
										   para_module,
										   para_slot);

			if(flag < 0)
			{
				return -103;
			}
		}
	}
	else
	{
		if(Fnc__PASSIVE_TRANSFER_READY(pm_index) < 0)
		{
			return -111;
		}

		if(Fnc__CALL_PLACE(p_variable,
						  p_alarm,
						  para_arm,
						  para_module,
						  para_slot) < 0)
		{
			return -112;
		}

		if(ex_flag < 0)
		{
			if(Fnc__PASSIVE_TRANSFER_END(pm_index) < 0)
			{
				return -113;
			}
		}
	}
	
	if(ex_flag > 0)
	{

	}
	else
	{
		CString para_data = sCH__PARA_MANUAL_MOVE_PMx_SLOT_CLOSE_SKIP->Get__STRING();

		if(para_data.CompareNoCase(STR__YES) >= 0)
		{
			CString str_bff;

			str_log.Format("PM%1d - NOT SV_CLOSE ! \n", pm_index + 1);

			str_bff.Format("  * %s <- %s \n", 
							sCH__PARA_MANUAL_MOVE_PMx_SLOT_CLOSE_SKIP->Get__CHANNEL_NAME(),
							sCH__PARA_MANUAL_MOVE_PMx_SLOT_CLOSE_SKIP->Get__STRING());
			str_log += str_bff;

			Fnc__APP_LOG(str_log);			
		}
		else
		{
			int k_limit = l__stn_name.GetSize();
			for(int k=0; k<k_limit; k++)
			{
				CString cur__stn_name = l__stn_name[k];

				int p_index = Macro__CHECK_PMx_INDEX(cur__stn_name);
				if(p_index < 0)			return -201;

				int pm_id = p_index + 1;

				if(dCH__CFG_PMx_CLOSE_SEQUENCE_MODE->Check__DATA(STR__ENABLE) > 0)
				{
					if(Fnc__PMx_SV_CLOSE(pm_id) < 0)
					{
						return -202;
					}
				}
				else
				{
					if(Run__PMx_SV_CLOSE(pm_id) < 0)
					{
						return -203;
					}
				}
			}
		}
	}

	if(handoff_mode == ACTIVE_HANDOFF)
	{
		int k_limit = l__stn_name.GetSize();

		for(int k=0; k<k_limit; k++)
		{
			CString cur__stn_name = l__stn_name[k];

			int p_index = Macro__CHECK_PMx_INDEX(cur__stn_name);
			if(p_index < 0)			return -211;

			if(sEXT_CH__CTC_CUR_MODE->Check__DATA(STR__MAINT) > 0)
			{
				if(Sub_OBJCALL_PMC_ACT_HANDOFF(p_index, "R3") < 0)
				{
					return -212;
				}
			}
		}
	}

	return 1;
}


//----------------------------------------------------------------------------------------------------
int  CObj__VAC_ROBOT_EX::
Fnc__WAC_ACT(CII_OBJECT__VARIABLE* p_variable,
			 CII_OBJECT__ALARM* p_alarm,
			 const CString& wac_arm,
			 const CString& para_arm,
			 const CString& para_module,
			 const CString& para_slot)
{
	sCH__INFO_WAC_PMC_ACTIVE->Set__DATA("ON");

	int r_flag = _Fnc__WAC_ACT(p_variable,p_alarm, wac_arm, para_arm,para_module,para_slot);

	sCH__INFO_WAC_PMC_ACTIVE->Set__DATA("OFF");
	return r_flag;
}
int  CObj__VAC_ROBOT_EX::
_Fnc__WAC_ACT(CII_OBJECT__VARIABLE* p_variable,
			  CII_OBJECT__ALARM* p_alarm,
			  const CString& wac_arm,
			  const CString& act_arm,
			  const CString& para_module,
			  const CString& para_slot)
{
	int pm_index = Macro__CHECK_PMx_INDEX(para_module);
	if(pm_index < 0)		return -11;

	CString act_name;

	// ...
	SCX__TIMER_CTRL x_timer__wac_delay;

	x_timer__wac_delay->REGISTER__ABORT_OBJECT(sObject_Name);
	x_timer__wac_delay->REGISTER__COUNT_CHANNEL(sCH__INFO_WAC_PMC_DELAY_COUNT->Get__CHANNEL_NAME());
	x_timer__wac_delay->INIT__COUNT_DOWN();

	// ...
	CString arm__wfr_info = "??";

		 if(wac_arm.CompareNoCase(_ARM_A) == 0)		arm__wfr_info = sEXT_CH__PHY_ROBOT__ARM_A_MATERIAL_TITLE->Get__STRING();
	else if(wac_arm.CompareNoCase(_ARM_B) == 0)		arm__wfr_info = sEXT_CH__PHY_ROBOT__ARM_B_MATERIAL_TITLE->Get__STRING();

	sCH__INFO_WAC_PMC_ARM_TYPE->Set__DATA(wac_arm);
	sCH__INFO_WAC_PMC_WFR_INFO->Set__DATA(arm__wfr_info);

	// PARAMETER ...
	CString para__wac_pos_id   = sCH__FROM_CTC_WAC_PM_POS_ID_X[pm_index]->Get__STRING();
	CString para__wac_pos_slot = sCH__FROM_CTC_WAC_PM_POS_SLOT_X[pm_index]->Get__STRING();

	CString para__wac_delay_sec = sCH__FROM_CTC_WAC_PM_DELAY_SEC_X[pm_index]->Get__STRING();
	double cfg__wac_delay_sec = atof(para__wac_delay_sec);

	sCH__INFO_WAC_PMC_STATE->Set__DATA(para_module);

	// ...
	bool active__rot_check = true;

	CString cur__rot_arm = dEXT_CH__PHY_ROBOT__MON_ACT_ARM->Get__STRING();
	CString cur__rot_pos = dEXT_CH__PHY_ROBOT__MON_TRG_ROT->Get__STRING();

	if(para__wac_pos_id.CompareNoCase("LLx") == 0)
	{
		if(wac_arm.CompareNoCase(cur__rot_arm) == 0)
		{
			int s_index = cur__rot_pos.Find("LB");
			if(s_index == 0)		active__rot_check = false;
		}
	}

	// ROBOT ROTATE ...
	if(active__rot_check)
	{
		CString wac__rot_pos_id   = _MODULE_LBA;
		CString wac__rot_pos_slot = para__wac_pos_slot;

			 if(para__wac_pos_id.CompareNoCase("LL2") == 0)			wac__rot_pos_id = _MODULE_LBB;
		else if(para__wac_pos_id.CompareNoCase("LL3") == 0)			wac__rot_pos_id = _MODULE_LBC;
		else if(para__wac_pos_id.CompareNoCase("LL4") == 0)			wac__rot_pos_id = _MODULE_LBD;

		sCH__INFO_WAC_PMC_POS_ID->Set__DATA(wac__rot_pos_id);
		sCH__INFO_WAC_PMC_POS_SLOT->Set__DATA(wac__rot_pos_slot);

		// ...
		{
			act_name.Format("WAC.Rotate - Start : (%s-%s) Arm_%s(%s)", 
							para__wac_pos_id,
							para__wac_pos_slot,
							wac_arm,
							arm__wfr_info);

			Fnc__APP_LOG(act_name);
		}

		if(Fnc__CALL_ROTATE(p_variable,
							p_alarm,
							wac_arm,
							wac__rot_pos_id,
							wac__rot_pos_slot) < 0)
		{
			return -21;
		}

		// ...
		{
			act_name.Format("WAC.Rotate - End");
			Fnc__APP_LOG(act_name);
		}
	}
	else
	{
		sCH__INFO_WAC_PMC_POS_ID->Set__DATA(cur__rot_pos);
		sCH__INFO_WAC_PMC_POS_SLOT->Set__DATA("__");
	}

	// SV OPEN -> START ...
	{
		// ...
		{
			act_name.Format("Start SV Open : %s(%s)", para_module,para_slot);
			Fnc__APP_LOG(act_name);
		}

		if(Fnc__RUN_SV_OPEN(act_arm, para_module, para_slot) < 0)
		{
			return -31;
		}
	}

	// WAC.DELAY ...
	{
		// ...
		{
			act_name.Format("WAC Delay (sec) - Started : (%.1f)", cfg__wac_delay_sec);
			Fnc__APP_LOG(act_name);
		}

		if(x_timer__wac_delay->WAIT(cfg__wac_delay_sec) < 0)
		{
			return -41;
		}
	}

	// ROBOT ROTATE ...
	{
		// ...
		{
			act_name.Format("Call Rotate - Started : (%.1f)", cfg__wac_delay_sec);
			Fnc__APP_LOG(act_name);
		}

		if(Fnc__CALL_ROTATE(p_variable,
							p_alarm,
							act_arm,
							para_module,
							para_slot) < 0)
		{
			return -51;
		}

		// ...
		{
			act_name.Format("Call Rotate - Completed : (%.1f)", cfg__wac_delay_sec);
			Fnc__APP_LOG(act_name);
		}
	}

	// SV OPEN -> WAIT ...
	{
		if(Fnc__WAIT_SV_OPEN(act_arm, para_module, para_slot) < 0)
		{
			return -61;
		}

		// ...
		{
			act_name.Format("Wait SV Open : %s(%s)", para_module,para_slot);
			Fnc__APP_LOG(act_name);
		}
	}

	return 1;
}

int  CObj__VAC_ROBOT_EX::
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

	// WAC.CHeck ...
	CString str_from_ctc_wac_use;

	int pm_index = Macro__CHECK_PMx_INDEX(para_module);
	if(pm_index >= 0)
	{
		str_from_ctc_wac_use = sCH__FROM_CTC_WAC_PM_USE_X[pm_index]->Get__STRING();
	}

	bool active__wfr_wac_check = false;

	if((str_from_ctc_wac_use.CompareNoCase(STR__YES) == 0) 
	&& (pm_index >= 0))
	{
		if(sCH__FROM_CTC_WAC_PM_PICK_CHECK_X[pm_index]->Check__DATA(STR__YES) > 0)
		{
			CString arm_a__wfr = dEXT_CH__PHY_ROBOT__ARM_A_MATERIAL_STATUS->Get__STRING();
			CString arm_b__wfr = dEXT_CH__PHY_ROBOT__ARM_B_MATERIAL_STATUS->Get__STRING();

			if(sCH__FROM_CTC_WAC_PM_WAFER_TYPE->Check__DATA(STR__ALL) > 0)
			{
				if((arm_a__wfr.CompareNoCase(STR__NONE) != 0)
				|| (arm_b__wfr.CompareNoCase(STR__NONE) != 0))
				{
					active__wfr_wac_check = true;
				}
			}
			else if(sCH__FROM_CTC_WAC_PM_WAFER_TYPE->Check__DATA(STR__UNPROCESSED) > 0)
			{
				if((arm_a__wfr.CompareNoCase(STR__MAPPED) == 0)
				|| (arm_b__wfr.CompareNoCase(STR__MAPPED) == 0))
				{
					active__wfr_wac_check = true;
				}
			}
		}
	}

	if(active__wfr_wac_check)
	{
		CString wfr_arm = para_arm;

			 if(para_arm.CompareNoCase(_ARM_A) == 0)		wfr_arm = _ARM_B;
		else if(para_arm.CompareNoCase(_ARM_B) == 0)		wfr_arm = _ARM_A;

		int r_act = Fnc__WAC_ACT(p_variable,p_alarm, wfr_arm, para_arm,para_module,para_slot);
		if(r_act < 0)			return r_act;
	}
	else
	{
		act_name.Format("Run_SV_OPEN[%s(%s)] : Start ...", para_module,para_slot);
		Fnc__APP_LOG(act_name);

		if(Fnc__RUN_SV_OPEN(para_arm, para_module, para_slot) < 0)
		{
			return OBJ_ABORT;
		}

		act_name.Format("Run_ROTATE[%s(%s)] : Start ...", para_module,para_slot);
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

		// ...
		{
			bool active__tmc_vlv__obj_ctrl = true;

			int ll_i = Macro__CHECK_LLx_INDEX(para_module);
			if(ll_i >= 0)
			{
				if(bActive__LLx_MANIFOLD_X)
				{
					if(ll_i < iSIZE__LLx)			active__tmc_vlv__obj_ctrl = false;
				}
			}

			if(active__tmc_vlv__obj_ctrl)
			{
				if(pTMC_VLV__OBJ_CTRL->When__OBJECT() < 0)
				{
					return -222;
				}
			}
			else
			{
				if(pLLx__OBJ_CTRL[ll_i]->When__OBJECT() < 0)
				{
					return -223;
				}
			}
		}

		act_name.Format("ROBOT : Checking ...");
		Fnc__APP_LOG(act_name);

		if(pROBOT__OBJ_CTRL->When__OBJECT() < 0)
		{
			return -111;
		}
	}
	
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
int  CObj__VAC_ROBOT_EX::
Seq__PLACE(CII_OBJECT__VARIABLE* p_variable,
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
		act_name.Format("place from Arm-%s to %s-%s",
						para_arm,
						para_module,
						para_slot);

		Fnc__APP_LOG(act_name);
	}

	// ...
	CString str_from_ctc_wac_use;
	
	int pm_index = Macro__CHECK_PMx_INDEX(para_module);
	if(pm_index >= 0)
	{
		str_from_ctc_wac_use = sCH__FROM_CTC_WAC_PM_USE_X[pm_index]->Get__STRING();
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

	// WAC.CHeck ...
	bool active__wfr_wac_check = false;

	if((str_from_ctc_wac_use.CompareNoCase(STR__YES) == 0) 
	&& (pm_index >= 0))
	{
		if(sCH__FROM_CTC_WAC_PM_PLACE_CHECK_X[pm_index]->Check__DATA(STR__YES) > 0)
		{
			CString arm_a__wfr = dEXT_CH__PHY_ROBOT__ARM_A_MATERIAL_STATUS->Get__STRING();
			CString arm_b__wfr = dEXT_CH__PHY_ROBOT__ARM_B_MATERIAL_STATUS->Get__STRING();

			if(sCH__FROM_CTC_WAC_PM_WAFER_TYPE->Check__DATA(STR__ALL) > 0)
			{
				if((arm_a__wfr.CompareNoCase(STR__NONE) != 0)
				|| (arm_b__wfr.CompareNoCase(STR__NONE) != 0))
				{
					active__wfr_wac_check = true;
				}
			}
			else if(sCH__FROM_CTC_WAC_PM_WAFER_TYPE->Check__DATA(STR__UNPROCESSED) > 0)
			{
				if(para_arm.CompareNoCase(_ARM_A) == 0)
				{
					if(arm_a__wfr.CompareNoCase(STR__MAPPED) == 0)
						active__wfr_wac_check = true;
				}
				else if(para_arm.CompareNoCase(_ARM_B) == 0)
				{
					if(arm_b__wfr.CompareNoCase(STR__MAPPED) == 0)
						active__wfr_wac_check = true;
				}
			}
		}
	}

	if(active__wfr_wac_check)
	{		
		int r_act = Fnc__WAC_ACT(p_variable,p_alarm, para_arm, para_arm,para_module,para_slot);
		if(r_act < 0)			return r_act;
	}
	else
	{
		// ...
		{
			act_name.Format("SV Open & Rotate Run - Started : %s(%s)", para_module,para_slot);
			Fnc__APP_LOG(act_name);
		}

		if(Fnc__RUN_SV_OPEN(para_arm, para_module, para_slot) < 0)
		{
			return -111;
		}

		if(Fnc__RUN_ROTATE(p_variable,
						   p_alarm,
						   para_arm,
						   para_module,
						   para_slot) < 0)
		{
			return -112;
		}

		// ...
		{
			bool active__tmc_vlv__obj_ctrl = true;

			int ll_i = Macro__CHECK_LLx_INDEX(para_module);
			if(ll_i >= 0)
			{
				if(bActive__LLx_MANIFOLD_X)
				{
					if(ll_i < iSIZE__LLx)			active__tmc_vlv__obj_ctrl = false;
				}
			}

			if(active__tmc_vlv__obj_ctrl)
			{
				if(pTMC_VLV__OBJ_CTRL->When__OBJECT() < 0)
				{
					return -1130;
				}
			}
			else
			{
				if(pLLx__OBJ_CTRL[ll_i]->When__OBJECT() < 0)
				{
					return -1131;
				}
			}
		}

		if(pROBOT__OBJ_CTRL->When__OBJECT() < 0)
		{
			return -114;
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

int  CObj__VAC_ROBOT_EX::
Call__RETRACT(CII_OBJECT__VARIABLE* p_variable,
			  CII_OBJECT__ALARM* p_alarm,
			  const CString& arm_type,
			  const CString& stn_name, 
			  const CString& stn_slot)
{
	CStringArray l_para;
	int flag;

	// ...
	{
		l_para.Add(arm_type);
		l_para.Add(stn_name);
		l_para.Add(stn_slot);

		if(pROBOT__OBJ_CTRL->Call__OBJ_MODE(CMMD__RETRACT,l_para) < 0)
			return -21;
	}
	return 1;
}

int  CObj__VAC_ROBOT_EX::
Call__EXTEND(CII_OBJECT__VARIABLE* p_variable,
			 CII_OBJECT__ALARM* p_alarm,
			 const CString& arm_type,
			 const CString& stn_name, 
			 const CString& stn_slot)
{
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

int  CObj__VAC_ROBOT_EX::
Call__GOUP(CII_OBJECT__VARIABLE* p_variable,
		   CII_OBJECT__ALARM* p_alarm,
		   const CString& arm_type,
		   const CString& stn_name, 
		   const CString& stn_slot)
{
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

int  CObj__VAC_ROBOT_EX::
Call__GODOWN(CII_OBJECT__VARIABLE* p_variable,
			 CII_OBJECT__ALARM* p_alarm,
			 const CString& arm_type,
			 const CString& stn_name, 
			 const CString& stn_slot)
{
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

int  CObj__VAC_ROBOT_EX::
Call__RQ_STN(CII_OBJECT__VARIABLE* p_variable,
			 CII_OBJECT__ALARM* p_alarm,
			 const CString& arm_type,
			 const CString& stn_name)
{
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

int  CObj__VAC_ROBOT_EX::
Call__SET_STN(CII_OBJECT__VARIABLE* p_variable,
			  CII_OBJECT__ALARM* p_alarm,
			  const CString& arm_type,
			  const CString& stn_name)
{
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

int  CObj__VAC_ROBOT_EX::
Call__TEACHED_CPTR_SAVE(CII_OBJECT__VARIABLE* p_variable,
						CII_OBJECT__ALARM* p_alarm,
						const CString& arm_type,
						const CString& stn_name)
{
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
