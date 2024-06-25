#include "StdAfx.h"
#include "CObj__ATM_ROBOT_STD.h"
#include "CObj__ATM_ROBOT_STD__DEF.h"

#include "Macro_Function.h"


//----------------------------------------------------------------------------------------------------
int  CObj__ATM_ROBOT_STD::
Call__INIT(CII_OBJECT__VARIABLE* p_variable,
		   CII_OBJECT__ALARM* p_alarm)
{
	return pATM_RB__OBJ_CTRL->Call__OBJECT(CMMD__INIT);
}

//
int  CObj__ATM_ROBOT_STD::
Call__PICK(CII_OBJECT__VARIABLE* p_variable,
		   CII_OBJECT__ALARM* p_alarm,
		   const CString& para_arm,
		   const CString& para_module,
		   const CString& para_slot)
{
	return Seq__PICK(p_variable,
					 p_alarm,
					 para_arm,
					 para_module,
					 para_slot,
					 -1);
}
int  CObj__ATM_ROBOT_STD::
Call__XPICK(CII_OBJECT__VARIABLE* p_variable,
			CII_OBJECT__ALARM* p_alarm,
			const CString& para_arm,
			const CString& para_module,
			const CString& para_slot)
{
	return Seq__PICK(p_variable,
					 p_alarm,
					 para_arm,
					 para_module,
					 para_slot,
					 1);
}


//
int  CObj__ATM_ROBOT_STD::
Call__PLACE(CII_OBJECT__VARIABLE* p_variable,
			CII_OBJECT__ALARM* p_alarm,
			const CString& para_arm,
			const CString& para_module,
			const CString& para_slot)
{
	return Seq__PLACE(p_variable,
						p_alarm,
						para_arm,
						para_module,
						para_slot,
						-1);
}
int  CObj__ATM_ROBOT_STD::
Call__XPLACE(CII_OBJECT__VARIABLE* p_variable,
			 CII_OBJECT__ALARM* p_alarm,
			 const CString& para_arm,
			 const CString& para_module,
			 const CString& para_slot)
{
	return Seq__PLACE(p_variable,
						p_alarm,
						para_arm,
						para_module,
						para_slot,
						1);
}

//
int  CObj__ATM_ROBOT_STD::
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


int  CObj__ATM_ROBOT_STD::
Seq__PICK(CII_OBJECT__VARIABLE* p_variable,
		  CII_OBJECT__ALARM* p_alarm,
		  const CString& para_arm,
		  const CString& para_module,
		  const CString& para_slot,
		  const int ex_flag)
{
	CString str_log;

	// ...
	{
		int slot_id = atoi(para_slot);

		str_log.Format("Seq__PICK.. Arm:%s,Stn:%s,Slot:%s,ex_flag:%d", para_arm, para_module, para_slot, ex_flag);
		xLOG_CTRL->WRITE__LOG(str_log);

		str_log.Format("Fnc__RUN_ROTATE -->");
		xLOG_CTRL->WRITE__LOG(str_log);
	}

	// ...
	{
		Fnc__RUN_ROTATE(p_variable,	
						p_alarm,
						para_arm,
						para_module,
						para_slot);
	}

	// ...
	int lp_index = Macro__CHECK_LPx_INDEX(para_module);
	int ll_index = Macro__CHECK_LLx_INDEX(para_module);

	if(lp_index >= 0)
	{		
		if((dEXT_CH__LPx_FOUP_POS_STATUS[lp_index]->Check__DATA(STR__LOAD) > 0)
		&& (dEXT_CH__LPx_DOOR_STATUS[lp_index]->Check__DATA(STR__OPEN)     < 0))
		{	
			// ...
			{
				str_log.Format("LP%d <- DOOR_OPEN", lp_index+1);
				xLOG_CTRL->WRITE__LOG(str_log);
			}

			if(pLPx__OBJ_CTRL[lp_index]->Call__OBJECT(CMMD__LP_OPENDOOR) < 0)
			// if(pLPx__OBJ_CTRL[lp_index]->Call__OBJECT(CMMD__LP_OPENDOOR_WITH_NO_UNLATCH) < 0)
			{
				return -101;
			}
		}	
		else
		{
			// ...
			{
				str_log.Format("LP%d <- CHECK_DOOR_OPEN", lp_index+1);
				xLOG_CTRL->WRITE__LOG(str_log);
			}

			if(pLPx__OBJ_CTRL[lp_index]->Call__OBJECT(CMMD__LP_CHECK_DOOR_OPEN) < 0)
			{
				return -101;
			}
		}
	}
	else if((ll_index >= 0) && (ll_index < iSIZE__LLx))
	{
		CUIntArray l__slot_id;

		// ...
		{
			int slot_id = atoi(para_slot);
			l__slot_id.Add(slot_id);

			if(para_arm.CompareNoCase(_ARM_AB) == 0)
			{
				slot_id = slot_id + 1;
				l__slot_id.Add(slot_id);
			}
		}

		// ...
		{
			str_log.Format("LL%1d : Call__%s", ll_index+1, LLx_CMMD__VENT);
			xLOG_CTRL->WRITE__LOG(str_log);

			aEXT_CH__LLx__PARA_SLOT_ID[ll_index]->Set__DATA(para_slot);

			if(pLLx__OBJ_CTRL[ll_index]->Call__OBJECT(LLx_CMMD__VENT) < 0)
			{
				return -102;
			}
		}

		// ...
		CString para__ll_slot;

		int slot_size = l__slot_id.GetSize();
		for(int k=0; k<slot_size; k++)
		{
			int slot_id = l__slot_id[k];
			para__ll_slot.Format("%1d", slot_id);

			str_log.Format("LL%1d (%s) : Call__DV_OPEN", ll_index+1, para__ll_slot);
			xLOG_CTRL->WRITE__LOG(str_log);

			aEXT_CH__LLx__PARA_SLOT_ID[ll_index]->Set__DATA(para__ll_slot);
	
			if(pLLx__OBJ_CTRL[ll_index]->Call__OBJECT("DV_TRANSFER.OPEN") < 0)
			{
				return -103;
			}
		}
	}

	if(pATM_RB__OBJ_CTRL->When__OBJECT() < 0)
	{
		return -111;
	}

	// ...
	{
		str_log.Format("Fnc__RUN_ROTATE --> End Check Complete");
		xLOG_CTRL->WRITE__LOG(str_log);

		str_log.Format("Fnc__CALL_PICK --> ");
		xLOG_CTRL->WRITE__LOG(str_log);
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
		if((ll_index >= 0) && (ll_index < iSIZE__LLx))
		{
			CUIntArray l__slot_id;

			// ...
			{
				int slot_id = atoi(para_slot);
				l__slot_id.Add(slot_id);

				if(para_arm.CompareNoCase(_ARM_AB) == 0)
				{
					slot_id = slot_id + 1;
					l__slot_id.Add(slot_id);
				}
			}

			// ...
			CString para_data = sCH__PARA_MANUAL_MOVE_LLx_DOOR_CLOSE_SKIP->Get__STRING();

			if(para_data.Find(STR__YES) >= 0)
			{
				CString str_bff;

				str_log.Format("LL%1d : NOT DV_CLOSE ! \n", ll_index+1);

				str_bff.Format("  * %s <- %s \n",
							    sCH__PARA_MANUAL_MOVE_LLx_DOOR_CLOSE_SKIP->Get__CHANNEL_NAME(),
								sCH__PARA_MANUAL_MOVE_LLx_DOOR_CLOSE_SKIP->Get__STRING());
				str_log += str_bff;

				xLOG_CTRL->WRITE__LOG(str_log);
			}
			else
			{
				CString para__ll_slot;

				int slot_size = l__slot_id.GetSize();
				for(int k=0; k<slot_size; k++)
				{
					int slot_id = l__slot_id[k];
					para__ll_slot.Format("%1d", slot_id);

					str_log.Format("LL%1d (%s) : CALL_DV_CLOSE --> ", ll_index+1, para__ll_slot);
					xLOG_CTRL->WRITE__LOG(str_log);

					aEXT_CH__LLx__PARA_SLOT_ID[ll_index]->Set__DATA(para__ll_slot);

					if(pLLx__OBJ_CTRL[ll_index]->Call__OBJECT("DV_TRANSFER.CLOSE") < 0)
					{
						return -201;
					}
				}
			}

			// ...
		}
	}

	// ...
	{
		str_log.Format("Seq__PICK.. Complete !");
		xLOG_CTRL->WRITE__LOG(str_log);
	}
	return 1;
}
int  CObj__ATM_ROBOT_STD::
Seq__PLACE(CII_OBJECT__VARIABLE* p_variable,
		   CII_OBJECT__ALARM* p_alarm,
		   const CString& para_arm,
		   const CString& para_module,
		   const CString& para_slot,
		   const int ex_flag)
{
	CString str_log;

	// ...
	{
		int slot_id = atoi(para_slot);

		str_log.Format("Seq__PLACE.. Arm:%s,Stn:%s,Slot:%s,ex_flag:%d", para_arm, para_module, para_slot, ex_flag);
		xLOG_CTRL->WRITE__LOG(str_log);

		str_log.Format("Fnc__RUN_ROTATE -->");
		xLOG_CTRL->WRITE__LOG(str_log);
	}

	// ...
	{
		Fnc__RUN_ROTATE(p_variable,
						p_alarm,
						para_arm,
						para_module,
						para_slot);
	}

	// ...
	int lp_index = Macro__CHECK_LPx_INDEX(para_module);
	int ll_index = Macro__CHECK_LLx_INDEX(para_module);

	if(lp_index >= 0)
	{
		if((dEXT_CH__LPx_FOUP_POS_STATUS[lp_index]->Check__DATA(STR__LOAD) > 0)
		&& (dEXT_CH__LPx_DOOR_STATUS[lp_index]->Check__DATA(STR__OPEN)     < 0))
		{	
			// ...
			{
				str_log.Format("LP%d <- DOOR_OPEN", lp_index+1);
				xLOG_CTRL->WRITE__LOG(str_log);
			}

			if(pLPx__OBJ_CTRL[lp_index]->Call__OBJECT(CMMD__LP_OPENDOOR) < 0)
			// if(pLPx__OBJ_CTRL[lp_index]->Call__OBJECT(CMMD__LP_OPENDOOR_WITH_NO_UNLATCH) < 0)
			{
				return -101;
			}
		}		
		else
		{
			// ...
			{
				str_log.Format("LP%d <- CHECK_DOOR_OPEN", lp_index+1);
				xLOG_CTRL->WRITE__LOG(str_log);
			}

			if(pLPx__OBJ_CTRL[lp_index]->Call__OBJECT(CMMD__LP_CHECK_DOOR_OPEN) < 0)
			{
				return -101;
			}
		}
	}
	else if((ll_index >= 0) && (ll_index < iSIZE__LLx))
	{
		CUIntArray l__slot_id;

		// ...
		{
			int slot_id = atoi(para_slot);
			l__slot_id.Add(slot_id);

			if(para_arm.CompareNoCase(_ARM_AB) == 0)
			{
				slot_id = slot_id + 1;
				l__slot_id.Add(slot_id);
			}
		}

		// ...
		{
			str_log.Format("LL%1d : Call__%s", ll_index+1, LLx_CMMD__VENT);
			xLOG_CTRL->WRITE__LOG(str_log);

			aEXT_CH__LLx__PARA_SLOT_ID[ll_index]->Set__DATA(para_slot);

			if(pLLx__OBJ_CTRL[ll_index]->Call__OBJECT(LLx_CMMD__VENT) < 0)
			{
				return -102;
			}
		}

		// ...
		CString para__ll_slot;

		int slot_size = l__slot_id.GetSize();
		for(int k=0; k<slot_size; k++)
		{
			int slot_id = l__slot_id[k];
			para__ll_slot.Format("%1d", slot_id);

			str_log.Format("LL%1d (%s) : Call__DV_OPEN", ll_index+1, para__ll_slot);
			xLOG_CTRL->WRITE__LOG(str_log);

			aEXT_CH__LLx__PARA_SLOT_ID[ll_index]->Set__DATA(para__ll_slot);

			if(pLLx__OBJ_CTRL[ll_index]->Call__OBJECT("DV_TRANSFER.OPEN") < 0)
			{
				return -103;
			}
		}
	}

	if(pATM_RB__OBJ_CTRL->When__OBJECT() < 0)
	{
		return -111;
	}

	// ...
	{
		str_log.Format("Fnc__RUN_ROTATE --> End Check Complete");
		xLOG_CTRL->WRITE__LOG(str_log);

		str_log.Format("Fnc__CALL_PLACE --> ");
		xLOG_CTRL->WRITE__LOG(str_log);
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
		if((ll_index >= 0) && (ll_index < iSIZE__LLx))
		{
			CUIntArray l__slot_id;

			// ...
			{
				int slot_id = atoi(para_slot);
				l__slot_id.Add(slot_id);

				if(para_arm.CompareNoCase(_ARM_AB) == 0)
				{
					slot_id = slot_id + 1;
					l__slot_id.Add(slot_id);
				}
			}

			// ...
			CString para_data = sCH__PARA_MANUAL_MOVE_LLx_DOOR_CLOSE_SKIP->Get__STRING();

			if(para_data.Find(STR__YES) >= 0)
			{
				CString str_bff;

				str_log.Format("LL%1d : NOT DV_CLOSE ! \n", ll_index+1);

				str_bff.Format("  * %s <- %s \n",
								sCH__PARA_MANUAL_MOVE_LLx_DOOR_CLOSE_SKIP->Get__CHANNEL_NAME(),
								sCH__PARA_MANUAL_MOVE_LLx_DOOR_CLOSE_SKIP->Get__STRING());
				str_log += str_bff;

				xLOG_CTRL->WRITE__LOG(str_log);
			}
			else
			{
				CString para__ll_slot;

				int slot_size = l__slot_id.GetSize();
				for(int k=0; k<slot_size; k++)
				{
					int slot_id = l__slot_id[k];
					para__ll_slot.Format("%1d", slot_id);

					str_log.Format("LL%1d (%s) : CALL_DV_CLOSE --> ", ll_index+1, para__ll_slot);
					xLOG_CTRL->WRITE__LOG(str_log);
	
					aEXT_CH__LLx__PARA_SLOT_ID[ll_index]->Set__DATA(para__ll_slot);
	
					if(pLLx__OBJ_CTRL[ll_index]->Call__OBJECT("DV_TRANSFER.CLOSE") < 0)
					{
						return -201;
					}
				}
			}

			// ...
		}
	}

	// ...
	{
		str_log.Format("Seq__PLACE.. Complete !");
		xLOG_CTRL->WRITE__LOG(str_log);
	}
	return 1;
}


// MAP -----
int  CObj__ATM_ROBOT_STD::
Call__MAP(CII_OBJECT__VARIABLE* p_variable,
		  CII_OBJECT__ALARM* p_alarm)
{
	return pATM_RB__OBJ_CTRL->Call__OBJECT(CMMD__MAP);
}

// HOME -----
int  CObj__ATM_ROBOT_STD::
Call__HOME(CII_OBJECT__VARIABLE* p_variable,
		   CII_OBJECT__ALARM* p_alarm,
		   const int req__vac_off)
{
	if(req__vac_off > 0)
	{
		return pATM_RB__OBJ_CTRL->Call__OBJECT("VAC_OFF.HOME");
	}

	return pATM_RB__OBJ_CTRL->Call__OBJECT(CMMD__HOME);
}

// VACON -----
int  CObj__ATM_ROBOT_STD::
Call__VACON(CII_OBJECT__VARIABLE* p_variable,
			CII_OBJECT__ALARM* p_alarm,
			const CString& arm_type)
{
	CStringArray l_para;

	l_para.Add(arm_type);
	return pATM_RB__OBJ_CTRL->Call__OBJ_MODE(CMMD__VACON,l_para);
}

// VACOFF -----
int  CObj__ATM_ROBOT_STD::
Call__VACOFF(CII_OBJECT__VARIABLE* p_variable,
			 CII_OBJECT__ALARM* p_alarm,
			 const CString& arm_type)
{
	CStringArray l_para;

	l_para.Add(arm_type);
	return pATM_RB__OBJ_CTRL->Call__OBJ_MODE(CMMD__VACOFF,l_para);
}

// RETRACT -----
int  CObj__ATM_ROBOT_STD::
Call__RETRACT(CII_OBJECT__VARIABLE* p_variable,
			  CII_OBJECT__ALARM* p_alarm,
			  const CString& arm_type,
			  const CString& stn_name,
			  const CString& stn_slot)
{
	CStringArray l_para;

	// ...
	{
		l_para.Add(arm_type);
		l_para.Add(stn_name);
		l_para.Add(stn_slot);
	}
	return pATM_RB__OBJ_CTRL->Call__OBJ_MODE(CMMD__RETRACT,l_para);
}

// EXTEND -----
int  CObj__ATM_ROBOT_STD::
Call__EXTEND(CII_OBJECT__VARIABLE* p_variable,
			 CII_OBJECT__ALARM* p_alarm,
			 const CString& arm_type,
			 const CString& stn_name,
			 const CString& stn_slot)
{
	CStringArray l_para;

	// ...
	{
		l_para.Add(arm_type);
		l_para.Add(stn_name);
		l_para.Add(stn_slot);
	}
	return pATM_RB__OBJ_CTRL->Call__OBJ_MODE(CMMD__EXTEND,l_para);
}

// GOUP -----
int  CObj__ATM_ROBOT_STD::
Call__GOUP(CII_OBJECT__VARIABLE* p_variable,
		   CII_OBJECT__ALARM* p_alarm,
		   const CString& arm_type,
		   const CString& stn_name,
		   const CString& stn_slot)
{
	CStringArray l_para;

	// ...
	{
		l_para.Add(arm_type);
		l_para.Add(stn_name);
		l_para.Add(stn_slot);
	}
	return  pATM_RB__OBJ_CTRL->Call__OBJ_MODE(CMMD__GOUP,l_para);
}

// GODOWN -----
int  CObj__ATM_ROBOT_STD::
Call__GODOWN(CII_OBJECT__VARIABLE* p_variable,
			 CII_OBJECT__ALARM* p_alarm,
			 const CString& arm_type,
			 const CString& stn_name,
			 const CString& stn_slot)
{
	CStringArray l_para;

	// ...
	{
		l_para.Add(arm_type);
		l_para.Add(stn_name);
		l_para.Add(stn_slot);
	}
	return pATM_RB__OBJ_CTRL->Call__OBJ_MODE(CMMD__GODOWN,l_para);
}

// ALGN ----- 
int  CObj__ATM_ROBOT_STD::
Call__ALGN(CII_OBJECT__VARIABLE* p_variable,
		   CII_OBJECT__ALARM* p_alarm)
{
	return pATM_RB__OBJ_CTRL->Call__OBJECT(CMMD__ALGN);
}

// ALGNPICK -----
int  CObj__ATM_ROBOT_STD::
Call__ALGNPICK(CII_OBJECT__VARIABLE* p_variable,
			   CII_OBJECT__ALARM* p_alarm,
			   const CString& arm_type,
			   const CString& stn_name,
			   const CString& stn_slot)
{
	CStringArray l_para;

	// ...
	{
		l_para.Add(arm_type);
		l_para.Add(stn_name);
		l_para.Add(stn_slot);
	}
	return pATM_RB__OBJ_CTRL->Call__OBJ_MODE(CMMD__ALGNPICK,l_para);
}

// ALGNPLACE -----
int  CObj__ATM_ROBOT_STD::
Call__ALGNPLACE(CII_OBJECT__VARIABLE* p_variable,
				CII_OBJECT__ALARM* p_alarm,
				const CString& arm_type,
				const CString& stn_name,
				const CString& stn_slot)
{
	CStringArray l_para;

	// ...
	{
		l_para.Add(arm_type);
		l_para.Add(stn_name);
		l_para.Add(stn_slot);
	}
	return pATM_RB__OBJ_CTRL->Call__OBJ_MODE(CMMD__ALGNPLACE,l_para);
}

// PLACE ALGN PICK -----
int  CObj__ATM_ROBOT_STD::
Call__PLACE_ALIGN_PICK(CII_OBJECT__VARIABLE* p_variable,
					   CII_OBJECT__ALARM* p_alarm,
					   const CString& arm_type,
					   const CString& stn_name,
					   const CString& stn_slot)
{
	CStringArray l_para;

	// ...
	{
		l_para.Add(arm_type);
		l_para.Add(stn_name);
		l_para.Add(stn_slot);
	}
	return pATM_RB__OBJ_CTRL->Call__OBJ_MODE(CMMD__PLACE_ALGN_PICK,l_para);
}

// LP_MAP -----
int  CObj__ATM_ROBOT_STD::
Call__LP_MAP(CII_OBJECT__VARIABLE* p_variable,
			 CII_OBJECT__ALARM* p_alarm,
			 const CString& arm_type,
			 const CString& stn_name,
			 const CString& stn_slot)
{
	CStringArray l_para;

	// ...
	{
		l_para.Add(arm_type);
		l_para.Add(stn_name);
		l_para.Add(stn_slot);
	}
	return pATM_RB__OBJ_CTRL->Call__OBJ_MODE(CMMD__LP_MAP,l_para);
}
