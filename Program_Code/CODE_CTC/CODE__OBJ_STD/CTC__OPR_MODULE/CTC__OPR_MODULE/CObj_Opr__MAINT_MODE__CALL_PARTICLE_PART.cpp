#include "StdAfx.h"
#include "CObj_Opr__MAINT_MODE.h"
#include "CObj_Opr__MAINT_MODE__DEF.h"

#include "Macro_Function.h"


// ...
int  CObj_Opr__MAINT_MODE
::Call__PARTICLE_PART_CHECK(CII_OBJECT__VARIABLE *p_variable, CII_OBJECT__ALARM *p_alarm)
{
	CString cur_mode = dCH__PARTICLE_PARA_SEL_AREA->Get__STRING();

	sCH__PARTICLE_PARA_CUR_AREA->Set__DATA(cur_mode);

	if(cur_mode.CompareNoCase("ALL") == 0)
	{
		_Reserve__PARTICLE_TRANSFER_PART(p_variable, p_alarm);
		_Reserve__PARTICLE_PROCESS_PART(p_variable, p_alarm);
	}
	else
	{
		_Release__PARTICLE_TRANSFER_PART(p_variable, p_alarm);
		_Release__PARTICLE_PROCESS_PART(p_variable, p_alarm);
	}

	if((cur_mode.CompareNoCase("ALL") == 0)
	|| (cur_mode.CompareNoCase("TRANSFER") == 0))
	{
		int r_flag = Call__PARTICLE_TRANSFER_PART(p_variable, p_alarm);
		if(r_flag < 0)			return r_flag;
	}

	if((cur_mode.CompareNoCase("ALL") == 0)
	|| (cur_mode.CompareNoCase("PROCESS") == 0))
	{
		int r_flag = Call__PARTICLE_PROCESS_PART(p_variable, p_alarm);
		if(r_flag < 0)			return r_flag;
	}

	return 1;
}

int  CObj_Opr__MAINT_MODE
::Call__PARTICLE_TRANSFER_PART(CII_OBJECT__VARIABLE *p_variable, CII_OBJECT__ALARM *p_alarm)
{
	// ...
	{
		sEXT_CH__PARA_MANUAL_MOVE_ALIGN_SKIP->Set__DATA("");
		sCH__PARA_MANUAL_MOVE_LLx_DOOR_CLOSE_SKIP->Set__DATA("");
		sCH__PARA_MANUAL_MOVE_LLx_SLOT_CLOSE_SKIP->Set__DATA("");

		sCH__PARA_MANUAL_MOVE_PMx_SLOT_CLOSE_SKIP->Set__DATA("");
		sCH__PARA_MANUAL_MOVE_PMx_LIFT_PIN_SKIP->Set__DATA("");
	}

	_Reserve__PARTICLE_TRANSFER_PART(p_variable, p_alarm);

	int r_flag = _Run__PARTICLE_TRANSFER_PART(p_variable, p_alarm);
	if(r_flag < 0)
	{
		for(int i=0; i<_ACT__TRANSFER_SIZE; i++)
		{
			if(sCH__PARA_TRANSFER_CTRL_STATE_X[i]->Check__DATA(STR__RUN) > 0)
				sCH__PARA_TRANSFER_CTRL_STATE_X[i]->Set__DATA(STR__ABORT);
		}
	}

	// ...
	{
		sEXT_CH__PARA_MANUAL_MOVE_ALIGN_SKIP->Set__DATA("");
		sCH__PARA_MANUAL_MOVE_LLx_DOOR_CLOSE_SKIP->Set__DATA("");
		sCH__PARA_MANUAL_MOVE_LLx_SLOT_CLOSE_SKIP->Set__DATA("");
		
		sCH__PARA_MANUAL_MOVE_PMx_SLOT_CLOSE_SKIP->Set__DATA("");
		sCH__PARA_MANUAL_MOVE_PMx_LIFT_PIN_SKIP->Set__DATA("");
	}
	return r_flag;
}
int  CObj_Opr__MAINT_MODE
::_Reserve__PARTICLE_TRANSFER_PART(CII_OBJECT__VARIABLE *p_variable, CII_OBJECT__ALARM *p_alarm)
{
	printf(" _Reserve__PARTICLE_TRANSFER_PART() ... \n");

	// ...
	int i;

	for(i=0; i<_ACT__TRANSFER_SIZE; i++)
	{
		if(sCH__PARA_TRANSFER_ACT_CHECK_X[i]->Check__DATA(STR__YES) < 0)
		{
			sCH__PARA_TRANSFER_CTRL_STATE_X[i]->Set__DATA(STR__IDLE);
			sCH__PARA_TRANSFER_CUR_COUNT_X[i]->Set__DATA("___");
			continue;
		}

		sCH__PARA_TRANSFER_CUR_COUNT_X[i]->Set__DATA("");
		sCH__PARA_TRANSFER_CTRL_STATE_X[i]->Set__DATA(STR__RESERVE);
	}

	return 1;
}
int  CObj_Opr__MAINT_MODE
::_Release__PARTICLE_TRANSFER_PART(CII_OBJECT__VARIABLE *p_variable, CII_OBJECT__ALARM *p_alarm)
{
	printf(" _Release__PARTICLE_TRANSFER_PART() ... \n");

	for(int i=0; i<_ACT__TRANSFER_SIZE; i++)
	{
		sCH__PARA_TRANSFER_CTRL_STATE_X[i]->Set__DATA(STR__IDLE);
		sCH__PARA_TRANSFER_CUR_COUNT_X[i]->Set__DATA("___");
	}
	return 1;
}
int  CObj_Opr__MAINT_MODE
::_Run__PARTICLE_TRANSFER_PART(CII_OBJECT__VARIABLE *p_variable, CII_OBJECT__ALARM *p_alarm)
{
	printf(" _Run__PARTICLE_TRANSFER_PART() ... \n");

	// ...
	int i;

	// ...
	{
		CString ch_data;

		ch_data = dCH__PARTICLE_PARA_LPx->Get__STRING();
		sCH__PARTICLE_CUR_LPx->Set__DATA(ch_data);

		//
		ch_data = dCH__PARTICLE_PARA_LLx->Get__STRING();
		sCH__PARTICLE_CUR_LLx->Set__DATA(ch_data);

		//
		sCH__PARTICLE_CUR_LLx_COOLING_SEC->Set__DATA("___");

		//
		ch_data = dCH__PARTICLE_PARA_EFEM_ROBOT->Get__STRING();
		sCH__PARTICLE_CUR_EFEM_ROBOT->Set__DATA(ch_data);

		ch_data = dCH__PARTICLE_PARA_TM_ROBOT->Get__STRING();
		sCH__PARTICLE_CUR_TM_ROBOT->Set__DATA(ch_data);

		//
		for(i=0; i<CFG_ST_LIMIT; i++)
		{
			ch_data = aCH__PARTICLE_PARA_STx_SLOT[i]->Get__STRING();
			sCH__PARTICLE_CUR_STx_SLOT[i]->Set__DATA(ch_data);
		}
	}

	for(i=0; i<_ACT__TRANSFER_SIZE; i++)
	{
		if(sCH__PARA_TRANSFER_ACT_CHECK_X[i]->Check__DATA(STR__YES) < 0)
		{
			sCH__PARA_TRANSFER_CTRL_STATE_X[i]->Set__DATA(STR__IDLE);
			continue;
		}

		// ...		
		CString act_name = _Get__Transfer_Action_Name(i);

		//
		{
			printf(" * Act_Name <- %s (%1d) \n", act_name, i+1);
			printf("  * %s <- %s \n",
					sCH__PARA_TRANSFER_STN_SRC_X[i]->Get__CHANNEL_NAME(),
					sCH__PARA_TRANSFER_STN_SRC_X[i]->Get__STRING());
			printf("  * %s <- %s \n",
					sCH__PARA_TRANSFER_STN_TRG_X[i]->Get__CHANNEL_NAME(),
					sCH__PARA_TRANSFER_STN_TRG_X[i]->Get__STRING());
			printf("  * %s <- %s \n",
					sCH__PARA_TRANSFER_CFG_COUNT_X[i]->Get__CHANNEL_NAME(),
					sCH__PARA_TRANSFER_CFG_COUNT_X[i]->Get__STRING());
			printf("\n");
		}

		sCH__PARA_TRANSFER_CUR_COUNT_X[i]->Set__DATA("0");
		sCH__PARA_TRANSFER_CTRL_STATE_X[i]->Set__DATA(STR__RUN);

		// ...
		CString para__stn_src = sCH__PARA_TRANSFER_STN_SRC_X[i]->Get__STRING();
		CString para__stn_trg = sCH__PARA_TRANSFER_STN_TRG_X[i]->Get__STRING();

		int loop_size  = atoi(para__stn_trg) - atoi(para__stn_src) + 1;
		int loop_count = 0;

		while(loop_count < loop_size)
		{
			loop_count++;

			// ...
			{
				sCH__PARA_TRANSFER_CUR_COUNT_X[i]->Set__DATA("0");
			}

			// ...
			CString para__src_module;
			CString para__src_slot;
			CString para__trg_module;
			CString para__trg_slot;
			CString para__efem_module;
			CString para__efem_slot;
			CString ch_data;

			// PARA.SRC ...
			{
				para__src_module.Format("LP%s", sCH__PARTICLE_CUR_LPx->Get__STRING());

				ch_data = sCH__PARA_TRANSFER_STN_SRC_X[i]->Get__STRING();
				int cur__src_slot = atoi(ch_data);
				para__src_slot.Format("%1d", cur__src_slot + (loop_count-1));
			}
			// PARA.EFEM_ARM ...
			{
				ch_data = sCH__PARTICLE_CUR_EFEM_ROBOT->Get__STRING();
					 if(ch_data.CompareNoCase("A") == 0)			para__efem_module = "ATM_RB1_A";
				else if(ch_data.CompareNoCase("B") == 0)			para__efem_module = "ATM_RB1_B";

				para__efem_slot = "";
			}

			// LPx -> EFEM.ROBOT
			{
				sCH__MOVE_SRC_MODULE->Set__DATA(para__src_module);
				sCH__MOVE_SRC_SLOT->Set__DATA(para__src_slot);

				sCH__MOVE_TRG_MODULE->Set__DATA(para__efem_module);
				sCH__MOVE_TRG_SLOT->Set__DATA(para__efem_slot);

				if(Call__MANUAL_MOVE(p_variable) < 0)		return -101;
			}

			if(i == _ACT_ID__MOVE_TO_LPx)
			{
				// MOVE_TO_LPx
				while(1)
				{
					ch_data = sCH__PARA_TRANSFER_CFG_COUNT_X[i]->Get__STRING();
					int cfg_count = atoi(ch_data);

					ch_data = sCH__PARA_TRANSFER_CUR_COUNT_X[i]->Get__STRING();
					int cur_count = atoi(ch_data);

					if(cur_count >= cfg_count)			break;

					ch_data.Format("%1d", cur_count + 1);
					sCH__PARA_TRANSFER_CUR_COUNT_X[i]->Set__DATA(ch_data);

					// EFEM.ROBOT -> LPx
					{
						sCH__MOVE_SRC_MODULE->Set__DATA(para__efem_module);
						sCH__MOVE_SRC_SLOT->Set__DATA(para__efem_slot);

						sCH__MOVE_TRG_MODULE->Set__DATA(para__src_module);
						sCH__MOVE_TRG_SLOT->Set__DATA(para__src_slot);

						if(Call__MANUAL_MOVE(p_variable) < 0)		return -21;

						Sleep(500);
					}			

					// LPx -> EFEM.ROBOT
					{
						sCH__MOVE_SRC_MODULE->Set__DATA(para__src_module);
						sCH__MOVE_SRC_SLOT->Set__DATA(para__src_slot);

						sCH__MOVE_TRG_MODULE->Set__DATA(para__efem_module);
						sCH__MOVE_TRG_SLOT->Set__DATA(para__efem_slot);

						if(Call__MANUAL_MOVE(p_variable) < 0)		return -22;

						Sleep(500);
					}
				}

				// ...
			}
			else if(i == _ACT_ID__MOVE_TO_LLx_IN_ATM)
			{
				// PARA.TRG ...
				{
					para__trg_module = sCH__PARTICLE_CUR_LLx->Get__STRING();
					Check__LLx_ISLOT(para__trg_module, para__trg_module,para__trg_slot);
				}

				// EFEM.ROBOT -> LLx_In_ATM
				{
					sCH__MOVE_SRC_MODULE->Set__DATA(para__efem_module);
					sCH__MOVE_SRC_SLOT->Set__DATA(para__efem_slot);

					sCH__MOVE_TRG_MODULE->Set__DATA(para__trg_module);
					sCH__MOVE_TRG_SLOT->Set__DATA(para__trg_slot);

					// Add Option : DOOR.NOT_CLOSE 
					{
						ch_data = STR__YES;
						sCH__PARA_MANUAL_MOVE_LLx_DOOR_CLOSE_SKIP->Set__DATA(ch_data);
					}

					if(Call__MANUAL_MOVE(p_variable) < 0)		return -11;
				}

				// MOVE_TO_LLx_IN_ATM
				while(1)
				{
					ch_data = sCH__PARA_TRANSFER_CFG_COUNT_X[i]->Get__STRING();
					int cfg_count = atoi(ch_data);

					ch_data = sCH__PARA_TRANSFER_CUR_COUNT_X[i]->Get__STRING();
					int cur_count = atoi(ch_data);

					if(cur_count >= cfg_count)			break;

					ch_data.Format("%1d", cur_count + 1);
					sCH__PARA_TRANSFER_CUR_COUNT_X[i]->Set__DATA(ch_data);

					// LLx -> EFEM.ROBOT
					{
						sCH__MOVE_SRC_MODULE->Set__DATA(para__trg_module);
						sCH__MOVE_SRC_SLOT->Set__DATA(para__trg_slot);

						sCH__MOVE_TRG_MODULE->Set__DATA(para__efem_module);
						sCH__MOVE_TRG_SLOT->Set__DATA(para__efem_slot);

						// Add Option : DOOR.NOT_CLOSE 
						{
							ch_data.Format("YES.%1d_1", cur_count);

							sCH__PARA_MANUAL_MOVE_LLx_DOOR_CLOSE_SKIP->Set__DATA(ch_data);
							sEXT_CH__PARA_MANUAL_MOVE_ALIGN_SKIP->Set__DATA(ch_data);
						}

						if(Call__MANUAL_MOVE(p_variable) < 0)		return -21;

						Sleep(500);
					}			

					// EFEM.ROBOT -> LLx
					{
						sCH__MOVE_SRC_MODULE->Set__DATA(para__efem_module);
						sCH__MOVE_SRC_SLOT->Set__DATA(para__efem_slot);

						sCH__MOVE_TRG_MODULE->Set__DATA(para__trg_module);
						sCH__MOVE_TRG_SLOT->Set__DATA(para__trg_slot);

						// Add Option : DOOR.NOT_CLOSE 
						{
							ch_data.Format("YES.%1d_2", cur_count);

							sCH__PARA_MANUAL_MOVE_LLx_DOOR_CLOSE_SKIP->Set__DATA(ch_data);
							sEXT_CH__PARA_MANUAL_MOVE_ALIGN_SKIP->Set__DATA(ch_data);
						}

						if(Call__MANUAL_MOVE(p_variable) < 0)		return -22;

						Sleep(500);
					}
				}

				// LLx_In_ATM -> EFEM.ROBOT
				{
					sCH__MOVE_SRC_MODULE->Set__DATA(para__trg_module);
					sCH__MOVE_SRC_SLOT->Set__DATA(para__trg_slot);

					sCH__MOVE_TRG_MODULE->Set__DATA(para__efem_module);
					sCH__MOVE_TRG_SLOT->Set__DATA(para__efem_slot);

					// ...
					{
						ch_data = STR__END;

						sCH__PARA_MANUAL_MOVE_LLx_DOOR_CLOSE_SKIP->Set__DATA(ch_data);
						sEXT_CH__PARA_MANUAL_MOVE_ALIGN_SKIP->Set__DATA(ch_data);
					}

					if(Call__MANUAL_MOVE(p_variable) < 0)		return -31;
				}
			}
			else if(i == _ACT_ID__LLx_DOOR_VLV_OP_CL)
			{
				// PARA.TRG ...
				{
					CString cur__ll_name = sCH__PARTICLE_CUR_LLx->Get__STRING();
					Check__LLx_ISLOT(cur__ll_name, para__trg_module,para__trg_slot);
				}

				int ll_index = Get__LLx_INDEX(para__trg_module);

				// EFEM.ROBOT -> LLx
				{
					sCH__MOVE_SRC_MODULE->Set__DATA(para__efem_module);
					sCH__MOVE_SRC_SLOT->Set__DATA(para__efem_slot);

					sCH__MOVE_TRG_MODULE->Set__DATA(para__trg_module);
					sCH__MOVE_TRG_SLOT->Set__DATA(para__trg_slot);

					if(Call__MANUAL_MOVE(p_variable) < 0)		return -11;
				}

				// DOOR.OPEN_CLOSE
				while(1)
				{
					ch_data = sCH__PARA_TRANSFER_CFG_COUNT_X[i]->Get__STRING();
					int cfg_count = atoi(ch_data);

					ch_data = sCH__PARA_TRANSFER_CUR_COUNT_X[i]->Get__STRING();
					int cur_count = atoi(ch_data);

					if(cur_count >= cfg_count)			break;

					ch_data.Format("%1d", cur_count + 1);
					sCH__PARA_TRANSFER_CUR_COUNT_X[i]->Set__DATA(ch_data);

					// DOOR.OPEN
					{
						dEXT_CH__LLx_PARA_SLOT_ID[ll_index]->Set__DATA(para__trg_slot);

						if(pLLx__OBJ_CTRL[ll_index]->Call__OBJECT("DOOR.OPEN") < 0)			return -21;

						Sleep(500);
					}

					// DOOR.CLOSE
					{
						dEXT_CH__LLx_PARA_SLOT_ID[ll_index]->Set__DATA(para__trg_slot);

						if(pLLx__OBJ_CTRL[ll_index]->Call__OBJECT("DOOR.CLOSE") < 0)		return -22;

						Sleep(500);
					}
				}

				// LLx -> EFEM.ROBOT
				{
					sCH__MOVE_SRC_MODULE->Set__DATA(para__trg_module);
					sCH__MOVE_SRC_SLOT->Set__DATA(para__trg_slot);

					sCH__MOVE_TRG_MODULE->Set__DATA(para__efem_module);
					sCH__MOVE_TRG_SLOT->Set__DATA(para__efem_slot);

					if(Call__MANUAL_MOVE(p_variable) < 0)		return -31;
				}			
			}
			else if((i == _ACT_ID__LLx_PUMP_VENT)
			     || (i == _ACT_ID__LLx_COOLING))
			{
				// PARA.TRG ...
				{
					CString cur__ll_name = sCH__PARTICLE_CUR_LLx->Get__STRING();
					Check__LLx_ISLOT(cur__ll_name, para__trg_module,para__trg_slot);
				}

				int ll_index = Get__LLx_INDEX(para__trg_module);

				// EFEM.ROBOT -> LLx
				{
					sCH__MOVE_SRC_MODULE->Set__DATA(para__efem_module);
					sCH__MOVE_SRC_SLOT->Set__DATA(para__efem_slot);

					sCH__MOVE_TRG_MODULE->Set__DATA(para__trg_module);
					sCH__MOVE_TRG_SLOT->Set__DATA(para__trg_slot);

					if(Call__MANUAL_MOVE(p_variable) < 0)		return -11;
				}

				// LLx.PUMP_VENT
				while(1)
				{
					ch_data = sCH__PARA_TRANSFER_CFG_COUNT_X[i]->Get__STRING();
					int cfg_count = atoi(ch_data);

					ch_data = sCH__PARA_TRANSFER_CUR_COUNT_X[i]->Get__STRING();
					int cur_count = atoi(ch_data);

					if(cur_count >= cfg_count)			break;

					ch_data.Format("%1d", cur_count + 1);
					sCH__PARA_TRANSFER_CUR_COUNT_X[i]->Set__DATA(ch_data);

					// PUMP
					{
						dEXT_CH__LLx_PARA_SLOT_ID[ll_index]->Set__DATA(para__trg_slot);

						if(pLLx__OBJ_CTRL[ll_index]->Call__OBJECT("PUMP") < 0)			return -21;

						Sleep(500);
					}

					// VENT
					{
						dEXT_CH__LLx_PARA_SLOT_ID[ll_index]->Set__DATA(para__trg_slot);

						if(i == _ACT_ID__LLx_COOLING)
						{
							if(dCH__PARTICLE_PARA_LLx_COOL_TYPE->Check__DATA(STR__TMC) > 0)
							{
								if(dEXT_CH__LLx_PRESSURE_STATUS[ll_index]->Check__DATA(STR__ATM) < 0)
								{
									CString ch_data = aCH__PARTICLE_PARA_LLx_COOLING_SEC->Get__STRING();
									aEXT_CH__LLx_PARA_VENT_COOLING_SEC[ll_index]->Set__DATA(ch_data);
								}
							}
							else
							{
								SCX__TIMER_CTRL x_timer_ctrl;

								x_timer_ctrl->REGISTER__ABORT_OBJECT(sObject_Name);
								x_timer_ctrl->REGISTER__COUNT_CHANNEL(sCH__PARTICLE_CUR_LLx_COOLING_SEC->Get__CHANNEL_NAME());
								x_timer_ctrl->INIT__COUNT_DOWN();

								double cfg_sec = aCH__PARTICLE_PARA_LLx_COOLING_SEC->Get__VALUE();

								printf(" * LLx.COOLING - Start \n");
								printf(" * Time <- %s", Macro__Get_Current_Date_Time());

								if(x_timer_ctrl->WAIT(cfg_sec) < 0)			return -103;

								printf(" * LLx.COOLING - End \n");
								printf(" * Time <- %s", Macro__Get_Current_Date_Time());
							}
						}

						if(pLLx__OBJ_CTRL[ll_index]->Call__OBJECT("VENT") < 0)			return -22;

						Sleep(500);
					}
				}

				// LLx -> EFEM.ROBOT
				{
					sCH__MOVE_SRC_MODULE->Set__DATA(para__trg_module);
					sCH__MOVE_SRC_SLOT->Set__DATA(para__trg_slot);

					sCH__MOVE_TRG_MODULE->Set__DATA(para__efem_module);
					sCH__MOVE_TRG_SLOT->Set__DATA(para__efem_slot);

					if(Call__MANUAL_MOVE(p_variable) < 0)		return -31;
				}			
			}
			else if(i == _ACT_ID__LLx_SLOT_VLV_OP_CL)
			{
				// PARA.TRG ...
				{
					CString cur__ll_name = sCH__PARTICLE_CUR_LLx->Get__STRING();
					Check__LLx_ISLOT(cur__ll_name, para__trg_module,para__trg_slot);
				}

				int ll_index = Get__LLx_INDEX(para__trg_module);

				// EFEM.ROBOT -> LLx
				{
					sCH__MOVE_SRC_MODULE->Set__DATA(para__efem_module);
					sCH__MOVE_SRC_SLOT->Set__DATA(para__efem_slot);

					sCH__MOVE_TRG_MODULE->Set__DATA(para__trg_module);
					sCH__MOVE_TRG_SLOT->Set__DATA(para__trg_slot);

					if(Call__MANUAL_MOVE(p_variable) < 0)		return -11;
				}

				// PUMP
				{
					dEXT_CH__LLx_PARA_SLOT_ID[ll_index]->Set__DATA(para__trg_slot);

					if(pLLx__OBJ_CTRL[ll_index]->Call__OBJECT("PUMP") < 0)		return -12;
				}

				// SLOT-VLV.OPEN_CLOSE
				while(1)
				{
					ch_data = sCH__PARA_TRANSFER_CFG_COUNT_X[i]->Get__STRING();
					int cfg_count = atoi(ch_data);

					ch_data = sCH__PARA_TRANSFER_CUR_COUNT_X[i]->Get__STRING();
					int cur_count = atoi(ch_data);

					if(cur_count >= cfg_count)			break;

					ch_data.Format("%1d", cur_count + 1);
					sCH__PARA_TRANSFER_CUR_COUNT_X[i]->Set__DATA(ch_data);

					// SLOT-VLV.OPEN
					{
						dEXT_CH__LLx_PARA_SLOT_ID[ll_index]->Set__DATA(para__trg_slot);

						if(pLLx__OBJ_CTRL[ll_index]->Call__OBJECT("SLOT.OPEN") < 0)			return -21;

						Sleep(500);
					}

					// SLOT-VLV.CLOSE
					{
						dEXT_CH__LLx_PARA_SLOT_ID[ll_index]->Set__DATA(para__trg_slot);

						if(pLLx__OBJ_CTRL[ll_index]->Call__OBJECT("SLOT.CLOSE") < 0)		return -22;

						Sleep(500);
					}
				}

				// LLx -> EFEM.ROBOT
				{
					sCH__MOVE_SRC_MODULE->Set__DATA(para__trg_module);
					sCH__MOVE_SRC_SLOT->Set__DATA(para__trg_slot);

					sCH__MOVE_TRG_MODULE->Set__DATA(para__efem_module);
					sCH__MOVE_TRG_SLOT->Set__DATA(para__efem_slot);

					if(Call__MANUAL_MOVE(p_variable) < 0)		return -31;
				}			
			}
			else if(i == _ACT_ID__MOVE_TO_LLx_IN_VAC)
			{
				// PARA.TRG ...
				{
					ch_data = sCH__PARTICLE_CUR_TM_ROBOT->Get__STRING();
						 if(ch_data.CompareNoCase("A") == 0)			para__trg_module = "VAC_RB1_A";
					else if(ch_data.CompareNoCase("B") == 0)			para__trg_module = "VAC_RB1_B";
				}

				// ...
				CString para__llx_module;
				CString para__llx_slot;

				CString cur__ll_name = sCH__PARTICLE_CUR_LLx->Get__STRING();
				Check__LLx_ISLOT(cur__ll_name, para__llx_module,para__llx_slot);

				// EFEM.ROBOT -> LLx
				{
					sCH__MOVE_SRC_MODULE->Set__DATA(para__efem_module);
					sCH__MOVE_SRC_SLOT->Set__DATA(para__efem_slot);

					sCH__MOVE_TRG_MODULE->Set__DATA(para__llx_module);
					sCH__MOVE_TRG_SLOT->Set__DATA(para__llx_slot);

					// Add Option : DOOR.NOT_CLOSE 
					{
						ch_data = STR__YES;
						sCH__PARA_MANUAL_MOVE_LLx_SLOT_CLOSE_SKIP->Set__DATA(ch_data);
					}

					if(Call__MANUAL_MOVE(p_variable) < 0)		return -11;
				}
				
				while(1)
				{
					ch_data = sCH__PARA_TRANSFER_CFG_COUNT_X[i]->Get__STRING();
					int cfg_count = atoi(ch_data);

					ch_data = sCH__PARA_TRANSFER_CUR_COUNT_X[i]->Get__STRING();
					int cur_count = atoi(ch_data);

					if(cur_count >= cfg_count)			break;

					ch_data.Format("%1d", cur_count + 1);
					sCH__PARA_TRANSFER_CUR_COUNT_X[i]->Set__DATA(ch_data);

					// LLx -> TM.ROBOT
					{
						sCH__MOVE_SRC_MODULE->Set__DATA(para__llx_module);
						sCH__MOVE_SRC_SLOT->Set__DATA(para__llx_slot);

						sCH__MOVE_TRG_MODULE->Set__DATA(para__trg_module);
						sCH__MOVE_TRG_SLOT->Set__DATA(para__trg_slot);

						// Add Option : DOOR.NOT_CLOSE 
						{
							ch_data.Format("YES.%1d_1", cur_count);
							sCH__PARA_MANUAL_MOVE_LLx_SLOT_CLOSE_SKIP->Set__DATA(ch_data);
						}

						if(Call__MANUAL_MOVE(p_variable) < 0)		return -21;

						Sleep(500);
					}

					// TM.ROBOT -> LLx
					{
						sCH__MOVE_SRC_MODULE->Set__DATA(para__trg_module);
						sCH__MOVE_SRC_SLOT->Set__DATA(para__trg_slot);

						sCH__MOVE_TRG_MODULE->Set__DATA(para__llx_module);
						sCH__MOVE_TRG_SLOT->Set__DATA(para__llx_slot);

						// Add Option : DOOR.NOT_CLOSE 
						{
							ch_data = sCH__PARA_TRANSFER_CUR_COUNT_X[i]->Get__STRING();
							int cur_count = atoi(ch_data);
						
							if(cur_count < cfg_count)
							{
								ch_data.Format("YES.%1d_2", cur_count);
								sCH__PARA_MANUAL_MOVE_LLx_SLOT_CLOSE_SKIP->Set__DATA(ch_data);
							}
							else
							{
								ch_data = STR__END;
								sCH__PARA_MANUAL_MOVE_LLx_SLOT_CLOSE_SKIP->Set__DATA(ch_data);
							}
						}

						if(Call__MANUAL_MOVE(p_variable) < 0)		return -22;

						Sleep(500);
					}
				}

				// LLx -> EFEM.ROBOT
				{
					sCH__MOVE_SRC_MODULE->Set__DATA(para__llx_module);
					sCH__MOVE_SRC_SLOT->Set__DATA(para__llx_slot);

					sCH__MOVE_TRG_MODULE->Set__DATA(para__efem_module);
					sCH__MOVE_TRG_SLOT->Set__DATA(para__efem_slot);

					if(Call__MANUAL_MOVE(p_variable) < 0)		return -31;
				}			
			}
			else if((i == _ACT_ID__MOVE_TO_AL1)
				 || (i == _ACT_ID__MOVE_TO_ST1)
				 || (i == _ACT_ID__MOVE_TO_ST2))
			{
				CString para__md_module = "";
				CString para__md_slot   = "";

				if(i == _ACT_ID__MOVE_TO_AL1)
				{
					para__md_module = "AL1";
					para__md_slot = "1";
				}
				else if(i == _ACT_ID__MOVE_TO_ST1)
				{
					para__md_module = STR__BUFF1;
					para__md_slot = sCH__PARTICLE_CUR_STx_SLOT[0]->Get__STRING();
				 }
				else if(i == _ACT_ID__MOVE_TO_ST2)
				{
					para__md_module = STR__BUFF2;
					para__md_slot = sCH__PARTICLE_CUR_STx_SLOT[1]->Get__STRING();
				}

				// EFEM.ROBOT -> MDx
				{
					sCH__MOVE_SRC_MODULE->Set__DATA(para__efem_module);
					sCH__MOVE_SRC_SLOT->Set__DATA(para__efem_slot);

					sCH__MOVE_TRG_MODULE->Set__DATA(para__md_module);
					sCH__MOVE_TRG_SLOT->Set__DATA(para__md_slot);

					if(Call__MANUAL_MOVE(p_variable) < 0)		return -11;
				}

				while(1)
				{
					ch_data = sCH__PARA_TRANSFER_CFG_COUNT_X[i]->Get__STRING();
					int cfg_count = atoi(ch_data);

					ch_data = sCH__PARA_TRANSFER_CUR_COUNT_X[i]->Get__STRING();
					int cur_count = atoi(ch_data);

					if(cur_count >= cfg_count)			break;

					ch_data.Format("%1d", cur_count + 1);
					sCH__PARA_TRANSFER_CUR_COUNT_X[i]->Set__DATA(ch_data);

					// MDx -> EFEM.ROBOT
					{
						sCH__MOVE_SRC_MODULE->Set__DATA(para__md_module);
						sCH__MOVE_SRC_SLOT->Set__DATA(para__md_slot);

						sCH__MOVE_TRG_MODULE->Set__DATA(para__efem_module);
						sCH__MOVE_TRG_SLOT->Set__DATA(para__efem_slot);

						if(Call__MANUAL_MOVE(p_variable) < 0)		return -21;

						Sleep(500);
					}

					// EFEM.ROBOT -> MDx
					{
						sCH__MOVE_SRC_MODULE->Set__DATA(para__efem_module);
						sCH__MOVE_SRC_SLOT->Set__DATA(para__efem_slot);

						sCH__MOVE_TRG_MODULE->Set__DATA(para__md_module);
						sCH__MOVE_TRG_SLOT->Set__DATA(para__md_slot);

						if(Call__MANUAL_MOVE(p_variable) < 0)		return -22;

						Sleep(500);
					}
				}

				// MDx -> EFEM.ROBOT
				{
					sCH__MOVE_SRC_MODULE->Set__DATA(para__md_module);
					sCH__MOVE_SRC_SLOT->Set__DATA(para__md_slot);

					sCH__MOVE_TRG_MODULE->Set__DATA(para__efem_module);
					sCH__MOVE_TRG_SLOT->Set__DATA(para__efem_slot);

					if(Call__MANUAL_MOVE(p_variable) < 0)		return -31;
				}			
			}
			else
			{
				continue;
			}

			// EFEM.ROBOT -> LPx
			{
				sCH__MOVE_SRC_MODULE->Set__DATA(para__efem_module);
				sCH__MOVE_SRC_SLOT->Set__DATA(para__efem_slot);

				sCH__MOVE_TRG_MODULE->Set__DATA(para__src_module);
				sCH__MOVE_TRG_SLOT->Set__DATA(para__src_slot);

				if(Call__MANUAL_MOVE(p_variable) < 0)		return -201;
			}			
		}

		sCH__PARA_TRANSFER_CTRL_STATE_X[i]->Set__DATA(STR__END);
	}

	return 1;
}

int  CObj_Opr__MAINT_MODE
::Call__PARTICLE_PROCESS_PART(CII_OBJECT__VARIABLE *p_variable, CII_OBJECT__ALARM *p_alarm)
{
	// ...
	{
		sEXT_CH__PARA_MANUAL_MOVE_ALIGN_SKIP->Set__DATA("");
		sCH__PARA_MANUAL_MOVE_LLx_DOOR_CLOSE_SKIP->Set__DATA("");
		sCH__PARA_MANUAL_MOVE_LLx_SLOT_CLOSE_SKIP->Set__DATA("");

		sCH__PARA_MANUAL_MOVE_PMx_SLOT_CLOSE_SKIP->Set__DATA("");
		sCH__PARA_MANUAL_MOVE_PMx_LIFT_PIN_SKIP->Set__DATA("");
	}

	_Reserve__PARTICLE_PROCESS_PART(p_variable, p_alarm);

	int r_flag = _Run__PARTICLE_PROCESS_PART(p_variable, p_alarm);
	if(r_flag < 0)
	{
		for(int i=0; i<CFG_PM_LIMIT; i++)
		{
			for(int k=0; k<_ACT__PROCESS_SIZE; k++)
			{
				if(sCH__PARA_PROCESS_CTRL_STATE_X[i][k]->Check__DATA(STR__RUN) > 0)
				{
					sCH__PARA_PROCESS_CTRL_STATE_X[i][k]->Set__DATA(STR__ABORT);
				}
			}
		}
	}

	// ...
	{
		sEXT_CH__PARA_MANUAL_MOVE_ALIGN_SKIP->Set__DATA("");
		sCH__PARA_MANUAL_MOVE_LLx_DOOR_CLOSE_SKIP->Set__DATA("");
		sCH__PARA_MANUAL_MOVE_LLx_SLOT_CLOSE_SKIP->Set__DATA("");

		sCH__PARA_MANUAL_MOVE_PMx_SLOT_CLOSE_SKIP->Set__DATA("");
		sCH__PARA_MANUAL_MOVE_PMx_LIFT_PIN_SKIP->Set__DATA("");
	}
	return r_flag;
}
int  CObj_Opr__MAINT_MODE
::_Reserve__PARTICLE_PROCESS_PART(CII_OBJECT__VARIABLE *p_variable, CII_OBJECT__ALARM *p_alarm)
{
	printf(" Reserve__PARTICLE_PROCESS_PART() ... \n");

	// ...
	int i;

	for(i=0; i<CFG_PM_LIMIT; i++)
	{
		for(int k=0; k<_ACT__PROCESS_SIZE; k++)
		{
			if(sCH__PARA_PROCESS_ACT_CHECK_X[i][k]->Check__DATA(STR__YES) < 0)
			{
				sCH__PARA_PROCESS_CTRL_STATE_X[i][k]->Set__DATA(STR__IDLE);
				sCH__PARA_PROCESS_CUR_COUNT_X[i][k]->Set__DATA("___");
				continue;
			}

			if(dEXT_CH__PNx_USE[i]->Check__DATA(STR__ENABLE) < 0)
			{
				sCH__PARA_PROCESS_CTRL_STATE_X[i][k]->Set__DATA(STR__DISABLE);
				sCH__PARA_PROCESS_CUR_COUNT_X[i][k]->Set__DATA("___");
				continue;
			}

			sCH__PARA_PROCESS_CUR_COUNT_X[i][k]->Set__DATA("");
			sCH__PARA_PROCESS_CTRL_STATE_X[i][k]->Set__DATA(STR__RESERVE);
		}
	}

	return 1;
}
int  CObj_Opr__MAINT_MODE
::_Release__PARTICLE_PROCESS_PART(CII_OBJECT__VARIABLE *p_variable, CII_OBJECT__ALARM *p_alarm)
{
	printf(" Release__PARTICLE_PROCESS_PART() ... \n");

	for(int i=0; i<CFG_PM_LIMIT; i++)
	{
		for(int k=0; k<_ACT__PROCESS_SIZE; k++)
		{
			sCH__PARA_PROCESS_CTRL_STATE_X[i][k]->Set__DATA(STR__IDLE);
			sCH__PARA_PROCESS_CUR_COUNT_X[i][k]->Set__DATA("___");
		}
	}
	return 1;
}
int  CObj_Opr__MAINT_MODE
::_Run__PARTICLE_PROCESS_PART(CII_OBJECT__VARIABLE *p_variable, CII_OBJECT__ALARM *p_alarm)
{
	printf(" Run__PARTICLE_PROCESS_PART() ... \n");

	// ...
	int i;

	// ...
	{
		CString ch_data;

		ch_data = dCH__PARTICLE_PARA_LPx->Get__STRING();
		sCH__PARTICLE_CUR_LPx->Set__DATA(ch_data);

		//
		ch_data = dCH__PARTICLE_PARA_LLx->Get__STRING();
		sCH__PARTICLE_CUR_LLx->Set__DATA(ch_data);

		//
		sCH__PARTICLE_CUR_LLx_COOLING_SEC->Set__DATA("___");

		//
		ch_data = dCH__PARTICLE_PARA_EFEM_ROBOT->Get__STRING();
		sCH__PARTICLE_CUR_EFEM_ROBOT->Set__DATA(ch_data);

		ch_data = dCH__PARTICLE_PARA_TM_ROBOT->Get__STRING();
		sCH__PARTICLE_CUR_TM_ROBOT->Set__DATA(ch_data);

		//
		for(i=0; i<CFG_ST_LIMIT; i++)
		{
			ch_data = aCH__PARTICLE_PARA_STx_SLOT[i]->Get__STRING();
			sCH__PARTICLE_CUR_STx_SLOT[i]->Set__DATA(ch_data);
		}
	}

	for(i=0; i<iPMx_SIZE; i++)
	{
		for(int k=0; k<_ACT__PROCESS_SIZE; k++)
		{
			if(sCH__PARA_PROCESS_ACT_CHECK_X[i][k]->Check__DATA(STR__YES) < 0)
			{
				sCH__PARA_PROCESS_CTRL_STATE_X[i][k]->Set__DATA(STR__IDLE);
				continue;
			}
			if(dEXT_CH__PNx_USE[i]->Check__DATA(STR__ENABLE) < 0)
			{
				sCH__PARA_PROCESS_CTRL_STATE_X[i][k]->Set__DATA(STR__DISABLE);
				continue;
			}

			// ...
			CString act_name = _Get__Process_Action_Name(k);

			// ...
			{
				printf(" * PM%1d.Act_Name <- %s (%1d) \n", i+1, act_name,k);
				printf("  * %s <- %s \n",
						sCH__PARA_PROCESS_STN_SRC_X[i][k]->Get__CHANNEL_NAME(),
						sCH__PARA_PROCESS_STN_SRC_X[i][k]->Get__STRING());
				printf("  * %s <- %s \n",
						sCH__PARA_PROCESS_STN_TRG_X[i][k]->Get__CHANNEL_NAME(),
						sCH__PARA_PROCESS_STN_TRG_X[i][k]->Get__STRING());
				printf("  * %s <- %s \n",
						sCH__PARA_PROCESS_CFG_COUNT_X[i][k]->Get__CHANNEL_NAME(),
						sCH__PARA_PROCESS_CFG_COUNT_X[i][k]->Get__STRING());

				if(k == _ACT_ID__PMx_PROCESS)
				{
					printf("  * %s <- %s \n",
						sCH__PARA_PROCESS_RECIPE_NAME_X[i]->Get__CHANNEL_NAME(),
						sCH__PARA_PROCESS_RECIPE_NAME_X[i]->Get__STRING());
				}
				printf("\n");
			}

			sCH__PARA_PROCESS_CUR_COUNT_X[i][k]->Set__DATA("0");
			sCH__PARA_PROCESS_CTRL_STATE_X[i][k]->Set__DATA(STR__RUN);

			// ...
			bool active__error_chheck = false;

			CString para__stn_src = sCH__PARA_PROCESS_STN_SRC_X[i][k]->Get__STRING();
			CString para__stn_trg = sCH__PARA_PROCESS_STN_TRG_X[i][k]->Get__STRING();

			int cur__stn_trg = atoi(para__stn_trg);
			int cur__stn_src = atoi(para__stn_src);

			if(cur__stn_src < 1)				active__error_chheck = true;
			if(cur__stn_trg < 1)				active__error_chheck = true;
			if(cur__stn_src > cur__stn_trg)		active__error_chheck = true;

			if(active__error_chheck)
			{
				sCH__PARA_PROCESS_CTRL_STATE_X[i][k]->Set__DATA(STR__DISABLE);
				continue;
			}

			// ...
			int loop_size  = cur__stn_trg - cur__stn_src + 1;
			int loop_count = 0;

			while(loop_count < loop_size)
			{
				loop_count++;

				// ...
				{
					sCH__PARA_PROCESS_CUR_COUNT_X[i][k]->Set__DATA("0");
					sCH__PARTICLE_CUR_LLx_COOLING_SEC->Set__DATA("0");
				}

				// ...
				CString para__src_module;
				CString para__src_slot;
				CString para__trg_module;
				CString para__trg_slot;
				CString ch_data;

				// PARA.SRC ...
				{
					para__src_module.Format("LP%s", sCH__PARTICLE_CUR_LPx->Get__STRING());

					ch_data = sCH__PARA_PROCESS_STN_SRC_X[i][k]->Get__STRING();
					int cur__src_slot = atoi(ch_data);
					para__src_slot.Format("%1d", cur__src_slot + (loop_count-1));
				}

				if(k == _ACT_ID__MOVE_TO_PMx)
				{
					CString para__arm_module;
					CString para__arm_slot;

					// PARA.TRG ...
					{
						para__trg_module.Format("PM%1d", i+1);
						para__trg_slot = "1";
					}
					// PARA.ARM ...
					{
						ch_data = sCH__PARTICLE_CUR_TM_ROBOT->Get__STRING();

							 if(ch_data.CompareNoCase("A") == 0)			para__arm_module = "VAC_RB1_A";
						else if(ch_data.CompareNoCase("B") == 0)			para__arm_module = "VAC_RB1_B";
					}

					// LPx -> PMx
					{
						sCH__MOVE_SRC_MODULE->Set__DATA(para__src_module);
						sCH__MOVE_SRC_SLOT->Set__DATA(para__src_slot);

						sCH__MOVE_TRG_MODULE->Set__DATA(para__trg_module);
						sCH__MOVE_TRG_SLOT->Set__DATA(para__trg_slot);

						// Add_Option : SLOT_CLOSE SKIP
						{
							ch_data = STR__YES;
							sCH__PARA_MANUAL_MOVE_PMx_SLOT_CLOSE_SKIP->Set__DATA(ch_data);
						}

						if(Call__MANUAL_MOVE(p_variable) < 0)		return -11;
					}

					while(1)
					{
						ch_data = sCH__PARA_PROCESS_CFG_COUNT_X[i][k]->Get__STRING();
						int cfg_count = atoi(ch_data);

						ch_data = sCH__PARA_PROCESS_CUR_COUNT_X[i][k]->Get__STRING();
						int cur_count = atoi(ch_data);

						if(cur_count >= cfg_count)			break;

						ch_data.Format("%1d", cur_count + 1);
						sCH__PARA_PROCESS_CUR_COUNT_X[i][k]->Set__DATA(ch_data);

						// PMx -> TM.ROBOT
						{
							sCH__MOVE_SRC_MODULE->Set__DATA(para__trg_module);
							sCH__MOVE_SRC_SLOT->Set__DATA(para__trg_slot);

							sCH__MOVE_TRG_MODULE->Set__DATA(para__arm_module);
							sCH__MOVE_TRG_SLOT->Set__DATA(para__arm_slot);

							// Add_Option : SLOT_CLOSE SKIP
							{
								ch_data.Format("YES.%1d_1", cur_count);
								sCH__PARA_MANUAL_MOVE_PMx_SLOT_CLOSE_SKIP->Set__DATA(ch_data);
							}

							if(Call__MANUAL_MOVE(p_variable) < 0)		return -12;

							Sleep(500);
						}

						// TM.ROBOT -> PMx
						{
							sCH__MOVE_SRC_MODULE->Set__DATA(para__arm_module);
							sCH__MOVE_SRC_SLOT->Set__DATA(para__arm_slot);

							sCH__MOVE_TRG_MODULE->Set__DATA(para__trg_module);
							sCH__MOVE_TRG_SLOT->Set__DATA(para__trg_slot);

							// Add_Option : SLOT_CLOSE SKIP
							{
								ch_data.Format("YES.%1d_2", cur_count);
								sCH__PARA_MANUAL_MOVE_PMx_SLOT_CLOSE_SKIP->Set__DATA(ch_data);
							}

							if(Call__MANUAL_MOVE(p_variable) < 0)		return -21;

							Sleep(500);
						}
					}

					// ...
				}
				else if(k == _ACT_ID__PMx_SLOT_VLV_OP_CL)
				{
					// PARA.TRG ...
					{
						para__trg_module.Format("PM%1d", i+1);
						para__trg_slot = "1";
					}

					// LPx -> PMx
					{
						sCH__MOVE_SRC_MODULE->Set__DATA(para__src_module);
						sCH__MOVE_SRC_SLOT->Set__DATA(para__src_slot);

						sCH__MOVE_TRG_MODULE->Set__DATA(para__trg_module);
						sCH__MOVE_TRG_SLOT->Set__DATA(para__trg_slot);

						if(Call__MANUAL_MOVE(p_variable) < 0)		return -11;
					}

					// SLOT-VLV.OPEN_CLOSE
					while(1)
					{
						ch_data = sCH__PARA_PROCESS_CFG_COUNT_X[i][k]->Get__STRING();
						int cfg_count = atoi(ch_data);

						ch_data = sCH__PARA_PROCESS_CUR_COUNT_X[i][k]->Get__STRING();
						int cur_count = atoi(ch_data);

						if(cur_count >= cfg_count)			break;

						ch_data.Format("%1d", cur_count + 1);
						sCH__PARA_PROCESS_CUR_COUNT_X[i][k]->Set__DATA(ch_data);

						// SLOT-VLV.OPEN
						{
							// Add_Option : LIFT_PIN SKIP
							{
								ch_data.Format("YES.%1d_1", cur_count);
								sCH__PARA_MANUAL_MOVE_PMx_LIFT_PIN_SKIP->Set__DATA(ch_data);
							}

							// ...
							ch_data.Format("%1d", i+1);
							dEXT_CH__TMC_CHM__PARA_PMx_ID->Set__DATA(ch_data);
							if(pOBJ_CTRL__TMC_CHM->Call__OBJECT("PMx.SV_OPEN") < 0)			return -31;

							if(pPMx__OBJ_CTRL[i]->Call__OBJECT("SLOT.OPEN") < 0)			return -32;

							Sleep(500);
						}

						// SLOT-VLV.CLOSE
						{
							// Add_Option : LIFT_PIN SKIP
							{
								ch_data.Format("YES.%1d_2", cur_count);
								sCH__PARA_MANUAL_MOVE_PMx_LIFT_PIN_SKIP->Set__DATA(ch_data);
							}

							// ...
							ch_data.Format("%1d", i+1);
							dEXT_CH__TMC_CHM__PARA_PMx_ID->Set__DATA(ch_data);
							if(pOBJ_CTRL__TMC_CHM->Call__OBJECT("PMx.SV_CLOSE") < 0)		return -33;

							if(pPMx__OBJ_CTRL[i]->Call__OBJECT("SLOT.CLOSE") < 0)			return -34;

							Sleep(500);
						}
					}

					// ...
				}
				else if(k == _ACT_ID__PMx_PIN_UP_TO_DOWN)
				{
					// PARA.TRG ...
					{
						para__trg_module.Format("PM%1d", i+1);
						para__trg_slot = "1";
					}

					// LPx -> PMx
					{
						sCH__MOVE_SRC_MODULE->Set__DATA(para__src_module);
						sCH__MOVE_SRC_SLOT->Set__DATA(para__src_slot);

						sCH__MOVE_TRG_MODULE->Set__DATA(para__trg_module);
						sCH__MOVE_TRG_SLOT->Set__DATA(para__trg_slot);

						if(Call__MANUAL_MOVE(p_variable) < 0)		return -11;
					}

					// PIN.UP_TO_DOWN
					while(1)
					{
						ch_data = sCH__PARA_PROCESS_CFG_COUNT_X[i][k]->Get__STRING();
						int cfg_count = atoi(ch_data);

						ch_data = sCH__PARA_PROCESS_CUR_COUNT_X[i][k]->Get__STRING();
						int cur_count = atoi(ch_data);

						if(cur_count >= cfg_count)			break;

						ch_data.Format("%1d", cur_count + 1);
						sCH__PARA_PROCESS_CUR_COUNT_X[i][k]->Set__DATA(ch_data);

						// PIN.UP
						{
							if(pPMx__OBJ_CTRL[i]->Call__OBJECT("PIN.UP") < 0)			return -31;

							Sleep(500);
						}

						// PIN.DOWN
						{
							if(pPMx__OBJ_CTRL[i]->Call__OBJECT("PIN.DOWN") < 0)			return -32;

							Sleep(500);
						}
					}

					// ...
				}
				else if(k == _ACT_ID__PMx_PROCESS)
				{
					// PARA.TRG ...
					{
						para__trg_module.Format("PM%1d", i+1);
						para__trg_slot = "1";
					}

					// LPx -> PMx
					{
						sCH__MOVE_SRC_MODULE->Set__DATA(para__src_module);
						sCH__MOVE_SRC_SLOT->Set__DATA(para__src_slot);

						sCH__MOVE_TRG_MODULE->Set__DATA(para__trg_module);
						sCH__MOVE_TRG_SLOT->Set__DATA(para__trg_slot);

						if(Call__MANUAL_MOVE(p_variable) < 0)		return -11;
					}

					// PROCESS ...
					while(1)
					{
						ch_data = sCH__PARA_PROCESS_CFG_COUNT_X[i][k]->Get__STRING();
						int cfg_count = atoi(ch_data);

						ch_data = sCH__PARA_PROCESS_CUR_COUNT_X[i][k]->Get__STRING();
						int cur_count = atoi(ch_data);

						if(cur_count >= cfg_count)			break;

						ch_data.Format("%1d", cur_count + 1);
						sCH__PARA_PROCESS_CUR_COUNT_X[i][k]->Set__DATA(ch_data);

						// PMx Process ...
						{
							ch_data = sCH__PARA_PROCESS_RECIPE_NAME_X[i]->Get__STRING();

							CString rcp_name__main = MACRO__Get_Quotation_String(ch_data);
							if(rcp_name__main.GetLength() < 1)		return -1011;

							// ...
							CII_EXT_OBJECT__CTRL *p_obj_ctrl = pPMx__OBJ_CTRL[i];
							if(p_obj_ctrl == NULL)		return -21;

							// ...
							CStringArray l_para;
							l_para.RemoveAll();

							l_para.Add(rcp_name__main);
							l_para.Add("");
							l_para.Add("");

							p_obj_ctrl->Run__OBJ_MODE("AUTO_MNT", l_para);

							while(1)
							{
								Sleep(500);

								if(p_variable->Check__CTRL_ABORT() > 0)
								{
									return -1012;
								}

								//
								p_obj_ctrl->Get__OBJECT_STATUS(ch_data);

								if(ch_data.CompareNoCase(STR__ABORTED) == 0)
								{
									return -1013;
								}
								if(ch_data.CompareNoCase(STR__AVAILABLE) == 0)
								{
									break;
								}
							}
						}
					}

					// ...
				}
				else
				{
					continue;
				}

				printf("\n");
				printf("================================================================\n");
				printf(" * PMx -> LPx \n");

				// ...
				{
					CString para__llx_module = "LLx";
					CString para__llx_slot = "";

					// PMx -> LLx
					{
						sCH__MOVE_SRC_MODULE->Set__DATA(para__trg_module);
						sCH__MOVE_SRC_SLOT->Set__DATA(para__trg_slot);

						sCH__MOVE_TRG_MODULE->Set__DATA(para__llx_module);
						sCH__MOVE_TRG_SLOT->Set__DATA(para__llx_slot);

						// ...
						{
							ch_data = STR__END;

							sCH__PARA_MANUAL_MOVE_PMx_SLOT_CLOSE_SKIP->Set__DATA(ch_data);
							sCH__PARA_MANUAL_MOVE_PMx_LIFT_PIN_SKIP->Set__DATA(ch_data);
						}

						if(Call__MANUAL_MOVE(p_variable) < 0)		return -101;
					}

					// ...
					{
						para__llx_module = sCH__MOVE_TRG_MODULE->Get__STRING();
						para__llx_slot = sCH__MOVE_TRG_SLOT->Get__STRING();
					}
					
					// VENT
					{
						int ll_index = Get__LLx_INDEX(para__llx_module);
						dEXT_CH__LLx_PARA_SLOT_ID[ll_index]->Set__DATA(para__llx_slot);

						if(dCH__PARTICLE_PARA_LLx_COOL_TYPE->Check__DATA(STR__TMC) > 0)
						{
							if(dEXT_CH__LLx_PRESSURE_STATUS[ll_index]->Check__DATA(STR__ATM) < 0)
							{
								CString ch_data = aCH__PARTICLE_PARA_LLx_COOLING_SEC->Get__STRING();
								aEXT_CH__LLx_PARA_VENT_COOLING_SEC[ll_index]->Set__DATA(ch_data);
							}
						}

						if(pLLx__OBJ_CTRL[ll_index]->Call__OBJECT("VENT") < 0)		return -102;
					}

					// LL.COOLING
					if(dCH__PARTICLE_PARA_LLx_COOL_TYPE->Check__DATA(STR__CTC) > 0)
					{
						SCX__TIMER_CTRL x_timer_ctrl;

						x_timer_ctrl->REGISTER__ABORT_OBJECT(sObject_Name);
						x_timer_ctrl->REGISTER__COUNT_CHANNEL(sCH__PARTICLE_CUR_LLx_COOLING_SEC->Get__CHANNEL_NAME());
						x_timer_ctrl->INIT__COUNT_DOWN();

						double cfg_sec = aCH__PARTICLE_PARA_LLx_COOLING_SEC->Get__VALUE();

						printf(" * LLx.COOLING - Start \n");
						printf(" * Time <- %s", Macro__Get_Current_Date_Time());

						if(x_timer_ctrl->WAIT(cfg_sec) < 0)			return -103;

						printf(" * LLx.COOLING - End \n");
						printf(" * Time <- %s", Macro__Get_Current_Date_Time());
					}

					// LLx -> LPx
					{
						sCH__MOVE_SRC_MODULE->Set__DATA(para__llx_module);
						sCH__MOVE_SRC_SLOT->Set__DATA(para__llx_slot);

						sCH__MOVE_TRG_MODULE->Set__DATA(para__src_module);
						sCH__MOVE_TRG_SLOT->Set__DATA(para__src_slot);

						if(Call__MANUAL_MOVE(p_variable) < 0)		return -104;
					}			
				}
			}

			sCH__PARA_PROCESS_CTRL_STATE_X[i][k]->Set__DATA(STR__END);
		}
	}

	return 1;
}


// ...
CString CObj_Opr__MAINT_MODE::_Get__Transfer_Action_Name(const int act_id)
{
	if(act_id == _ACT_ID__MOVE_TO_LPx)				return _ACT_MOVE__MOVE_TO_LPx;
	if(act_id == _ACT_ID__MOVE_TO_LLx_IN_ATM)		return _ACT_MOVE__MOVE_TO_LLx_IN_ATM;
	if(act_id == _ACT_ID__LLx_DOOR_VLV_OP_CL)		return _ACT_MOVE__LLx_DOOR_VLV_OP_CL;
	if(act_id == _ACT_ID__LLx_PUMP_VENT)			return _ACT_MOVE__LLx_PUMP_VENT;
	if(act_id == _ACT_ID__LLx_COOLING)				return _ACT_MOVE__LLx_COOLING;
	if(act_id == _ACT_ID__LLx_SLOT_VLV_OP_CL)		return _ACT_MOVE__LLx_SLOT_VLV_OP_CL;
	if(act_id == _ACT_ID__MOVE_TO_LLx_IN_VAC)		return _ACT_MOVE__MOVE_TO_LLx_IN_VAC;
	if(act_id == _ACT_ID__MOVE_TO_AL1)				return _ACT_MOVE__MOVE_TO_AL1;
	if(act_id == _ACT_ID__MOVE_TO_ST1)				return _ACT_MOVE__MOVE_TO_ST1;
	if(act_id == _ACT_ID__MOVE_TO_ST2)				return _ACT_MOVE__MOVE_TO_ST2;

	return "???";
}

CString CObj_Opr__MAINT_MODE::_Get__Process_Action_Name(const int act_id)
{
	if(act_id == _ACT_ID__MOVE_TO_PMx)				return _ACT_PROC__MOVE_TO_PMx;
	if(act_id ==_ACT_ID__PMx_SLOT_VLV_OP_CL)		return _ACT_PROC__PMx_SLOT_VLV_OP_CL;
	if(act_id ==_ACT_ID__PMx_PIN_UP_TO_DOWN)		return _ACT_PROC__PMx_PIN_UP_TO_DOWN;
	if(act_id ==_ACT_ID__PMx_PROCESS)				return _ACT_PROC__PMx_PROCESS;

	return "???";
}
