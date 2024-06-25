#include "StdAfx.h"
#include "CObj_Opr__MAINT_MODE.h"
#include "CObj_Opr__MAINT_MODE__DEF.h"

#include "Macro_Function.h"


// ...
int  CObj_Opr__MAINT_MODE::Call__MACRO_MOVE(CII_OBJECT__VARIABLE *p_variable)
{
	printf(" * CObj_Opr__MAINT_MODE::Call__MACRO_MOVE() ... \n");

	// ...
	{
		sCH__MACRO_MOVE__LOG_STATUS->Set__DATA(STR__STOP);
		sCH__MACRO_MOVE__CUR_CYCLE_COUNT->Set__DATA("");

		Fnc__MOVE_HISTORY_TO_SCR();

		sCH__MACRO_MOVE__SCR_NAME->Set__DATA(SCR_NAME__POPUP_MACRO_MOVE_HISTORY_CTRL);
		sCH__MACRO_MOVE__SCR_NAME->When__NOT_DATA(SCR_NAME__POPUP_MACRO_MOVE_HISTORY_CTRL);

		if(sCH__MACRO_MOVE__SCR_NAME->Check__DATA(STR__START) < 0)
		{
			return -1;
		}
	}

	// ...
	{
		CString var_data;

		aCH__MACRO_MOVE__CYCLE_COUNT->Get__DATA(var_data);
		sCH__MACRO_MOVE__CUR_CYCLE_COUNT->Set__DATA(var_data);

		int i_count = atoi(var_data); 

		while(i_count > 0)
		{
			i_count--;

			// ...
			{
				var_data.Format("%1d", i_count);
				sCH__MACRO_MOVE__CUR_CYCLE_COUNT->Set__DATA(var_data);
			}

			// ...
			CUIntArray   l_move_type;
			CStringArray l_data_para1;
			CStringArray l_data_para2;
			CStringArray l_data_para3;
			CStringArray l_data_para4;

			mMacro_Log.Get__ALL_MOVE_HISTORY(l_move_type,
											 l_data_para1,
											 l_data_para2,
											 l_data_para3,
											 l_data_para4);

			// ...
			{
				int para_move_type;
				CString para_data1;
				CString para_data2;
				CString para_data3;
				CString para_data4;

				CString proc__pmx_name = "";
				CString proc__rcp_pre  = "";
				CString proc__rcp_main = "";
				CString proc__rcp_post = "";

				int loop_active = -1;
				int loop_count  = -1;
				int loop_index  = -1;
				int loop_repaet = -1;
				int loop_start  = -1;
				int loop_end    = -1;

				int i_limit = l_move_type.GetSize();
				int i;

				for(i=0; (i < i_limit) || (loop_active > 0); i++)
				{
					if(loop_active > 0)
					{
						i = loop_index++;

						printf(" * Loop_Active ... \n");
						printf("   ** Loop_Index : %1d \n", loop_index);
						printf("   ** Loop_Count : %1d \n", loop_count);

						// ...
						CString str_data;

						str_data.Format("%1d", loop_start);
						sCH__MACRO_MOVE__STEP_REPEAT_START->Set__DATA(str_data);

						str_data.Format("%1d", loop_end);
						sCH__MACRO_MOVE__STEP_REPEAT_END->Set__DATA(str_data);

						str_data.Format("%1d", loop_count);
						sCH__MACRO_MOVE__STEP_REPEAT_COUNT->Set__DATA(str_data);

						str_data.Format("%1d", loop_index);
						sCH__MACRO_MOVE__STEP_REPEAT_INDEX->Set__DATA(str_data);
					}
					else
					{
						printf(" * Step_Info ... \n");
						printf("   ** i : %1d \n", i);

						sCH__MACRO_MOVE__STEP_REPEAT_START->Set__DATA("_");
						sCH__MACRO_MOVE__STEP_REPEAT_END->Set__DATA("_");
						sCH__MACRO_MOVE__STEP_REPEAT_COUNT->Set__DATA("_");
						sCH__MACRO_MOVE__STEP_REPEAT_INDEX->Set__DATA("_");
					}

					if(i >= i_limit)
					{
						printf(" * Error : index Over ! (%1d) \n", i_limit);
						break;
					}

					// ...
					{
						para_move_type  = l_move_type[i];

						para_data1 = l_data_para1[i];
						para_data2 = l_data_para2[i];
						para_data3 = l_data_para3[i];
						para_data4 = l_data_para4[i];
					}

					if(p_variable->Check__CTRL_ABORT() > 0)
					{
						return -11;
					}

					if(para_move_type == _CMMD__MACRO_MOVE__MANUAL_MOVE)
					{
						CString src_module = para_data1;
						CString src_slot   = para_data2;
						CString trg_module = para_data3;
						CString trg_slot   = para_data4;

						// ...
						{
							printf(" * Macro_Move : Manual_Move ... \n");
							printf("   ** src_module <- %s \n", src_module);
							printf("   ** src_slot   <- %s \n", src_slot);
							printf("   ** trg_module <- %s \n", trg_module);
							printf("   ** trg_slot   <- %s \n", trg_slot);
						}

						// Module_X : State Check ...
						{
							// Source Module ...
							while(1)
							{
								Sleep(90);

								if(p_variable->Check__CTRL_ABORT() > 0)
								{
									return -101;
								}

								//
								sEXT_CH__VAC_RB__CHECK_MODULE->Set__DATA(src_module);

								if(pVAC_RB__OBJ_CTRL->Call__OBJECT(VAC_CMMD__CHECK_STANDBY) > 0)
								{
									break;
								}
							}

							// Target Module ...
							while(1)
							{
								Sleep(90);

								if(p_variable->Check__CTRL_ABORT() > 0)
								{
									return -102;
								}

								//
								sEXT_CH__VAC_RB__CHECK_MODULE->Set__DATA(trg_module);

								if(pVAC_RB__OBJ_CTRL->Call__OBJECT(VAC_CMMD__CHECK_STANDBY) > 0)
								{
									break;
								}
							}
						}

						if((proc__pmx_name.GetLength() > 0)
						&& (proc__pmx_name.CompareNoCase(src_module) == 0))
						{
							CString str__vac_rb   = "VAC_RB1_A";
							CString str__arm_type = "A";

							// ...
							{
								CString query_name = _QRY__GET_EMPTY_ARM;
								CStringArray l_sub_query;
								CStringArray l_data;

								if(pVAC_RB__OBJ_CTRL->Call__QUERY_LIST(query_name, l_sub_query, l_data) > 0)
								{
									if(l_data.GetSize() > 0)
									{
										if(l_data[0].CompareNoCase("B") == 0)
										{
											str__vac_rb   = "VAC_RB1_B";
											str__arm_type = "B";
										}
									}
								}
							}

							// PMx -> VAC_RB1 ...
							{
								sCH__MOVE_SRC_MODULE->Set__DATA(src_module);
								sCH__MOVE_SRC_SLOT->Set__DATA(src_slot);

								sCH__MOVE_TRG_MODULE->Set__DATA(str__vac_rb);
								sCH__MOVE_TRG_SLOT->Set__DATA(str__arm_type);

								if(Call__MANUAL_MOVE(p_variable) < 0)
								{
									return -121;
								}	   
							}

							// ...
							{
								int pmx_index = Macro__Get_PM_INDEX(proc__pmx_name);

								// PMx Clean ...
								if(proc__rcp_post.GetLength() > 0)
								{
									CStringArray l_para;
									l_para.RemoveAll();

									l_para.Add(proc__rcp_post);
									l_para.Add("");
									l_para.Add("");

									if(pPMx__OBJ_CTRL[pmx_index]->Run__OBJ_MODE("AUTO_MNT", l_para) < 0)
									{
										return -122;
									}
								}

								proc__pmx_name = "";
								proc__rcp_post = "";
							}

							// VAC_RB -> MODULE_X ...
							{
								sCH__MOVE_SRC_MODULE->Set__DATA(str__vac_rb);
								sCH__MOVE_SRC_SLOT->Set__DATA(str__arm_type);

								sCH__MOVE_TRG_MODULE->Set__DATA(trg_module);
								sCH__MOVE_TRG_SLOT->Set__DATA(trg_slot);

								if(Call__MANUAL_MOVE(p_variable) < 0)
								{
									return -123;
								}	   
							}
						}
						else
						{
							// ...
							{
								sCH__MOVE_SRC_MODULE->Set__DATA(src_module);
								sCH__MOVE_SRC_SLOT->Set__DATA(src_slot);

								sCH__MOVE_TRG_MODULE->Set__DATA(trg_module);
								sCH__MOVE_TRG_SLOT->Set__DATA(trg_slot);
							}

							if(Call__MANUAL_MOVE(p_variable) < 0)
							{
								return -12;
							}	   
						}
					}	
					else if(para_move_type == _CMMD__MACRO_MOVE__TOOL_CLEAR)
					{
						// ...
						{
							printf(" * Macro_Move : Tool_Clear ... \n");
						}

						if(Call__TOOL_CLEAR(p_variable) < 0)
						{
							return -13;
						}	   
					}	
					else if(para_move_type == _CMMD__MACRO_MOVE__PROCESS)
					{
						proc__pmx_name = para_data1;

						// ...
						{
							printf(" * Macro_Move : Process ... \n");
							printf("   ** process_module <- %s \n", proc__pmx_name);
						}

						// ...
						int pmx_index = Macro__Get_PM_INDEX(proc__pmx_name);
						int llx_index = Macro__Get_LL_INDEX(proc__pmx_name);

						if(pmx_index >= 0)
						{
							proc__rcp_pre  = sCH__MACRO_MOVE__PROCESS_PRE_RECIPE_PMx[pmx_index]->Get__STRING();
							proc__rcp_main = sCH__MACRO_MOVE__PROCESS_MAIN_RECIPE_PMx[pmx_index]->Get__STRING();
							proc__rcp_post = sCH__MACRO_MOVE__PROCESS_POST_RECIPE_PMx[pmx_index]->Get__STRING();

							if(proc__rcp_main.GetLength() > 0)
							{
								CII_EXT_OBJECT__CTRL *p_obj_ctrl = pPMx__OBJ_CTRL[pmx_index];
								if(p_obj_ctrl == NULL)		return -21;

								// PMx Process ...
								{
									CStringArray l_para;
									l_para.RemoveAll();

									l_para.Add(proc__rcp_main);
									l_para.Add("");
									l_para.Add("");

									p_obj_ctrl->Run__OBJ_MODE("AUTO_MNT", l_para);

									while(1)
									{
										Sleep(500);

										if(p_variable->Check__CTRL_ABORT() > 0)
										{
											return -101;
										}

										//
										p_obj_ctrl->Get__OBJECT_STATUS(var_data);

										if((var_data.CompareNoCase(STR__ABORTED)   == 0)
										|| (var_data.CompareNoCase(STR__AVAILABLE) == 0))
										{
												break;
										}
									}

									proc__rcp_main = "";
								}
							
								// State Check ...
								while(1)
								{
									Sleep(90);

									if(p_variable->Check__CTRL_ABORT() > 0)
									{
										return -102;
									}

									//
									sEXT_CH__VAC_RB__CHECK_MODULE->Set__DATA(proc__pmx_name);

									if(pVAC_RB__OBJ_CTRL->Call__OBJECT(VAC_CMMD__CHECK_STANDBY) > 0)
									{
										break;
									}
								}

								// ...
							}
						}
						else if(llx_index >= 0)
						{
							proc__rcp_pre  = sCH__MACRO_MOVE__PROCESS_PRE_RECIPE_LLx[llx_index]->Get__STRING();
							proc__rcp_main = sCH__MACRO_MOVE__PROCESS_MAIN_RECIPE_LLx[llx_index]->Get__STRING();
							proc__rcp_post = sCH__MACRO_MOVE__PROCESS_POST_RECIPE_LLx[llx_index]->Get__STRING();

							if(proc__rcp_main.GetLength() > 0)
							{
								CII_EXT_OBJECT__CTRL *p_obj_ctrl = pLLx__OBJ_CTRL[llx_index];
								if(p_obj_ctrl == NULL)		return -21;

								// LLx Process ...
								{
									sEXT_CH__LLx_PARA_PROC_RECIPE_NAME[llx_index]->Set__DATA(proc__rcp_main);
									sEXT_CH__LLx_PARA_PROC_SCH_NAME[llx_index]->Set__DATA("");

									p_obj_ctrl->Run__OBJECT("PROC_MAINT");

									while(1)
									{
										Sleep(500);

										if(p_variable->Check__CTRL_ABORT() > 0)
										{
											return -101;
										}

										//
										p_obj_ctrl->Get__OBJECT_STATUS(var_data);

										if((var_data.CompareNoCase(STR__ABORTED)   == 0)
										|| (var_data.CompareNoCase(STR__AVAILABLE) == 0))
										{
											break;
										}
									}

									proc__rcp_main = "";
								}

								// State Check ...
								while(1)
								{
									Sleep(90);

									if(p_variable->Check__CTRL_ABORT() > 0)
									{
										return -102;
									}

									//
									sEXT_CH__VAC_RB__CHECK_MODULE->Set__DATA(proc__pmx_name);

									if(pVAC_RB__OBJ_CTRL->Call__OBJECT(VAC_CMMD__CHECK_STANDBY) > 0)
									{
										break;
									}
								}

								// ...
							}
						}

						// ...
					}	
					else if(para_move_type == _CMMD__MACRO_MOVE__LOOP)
					{
						if((loop_active == i) || (loop_active == 0))
						{
							printf(" * Macro_Move : Loop End ... \n");

							loop_active = -1;
						}
						else if(loop_active > i)
						{
							printf(" * Macro_Move : Loop Ignore ... \n");
						}
						else
						{
							printf(" * Macro_Move : Loop Apply ... \n");
							printf("   ** loop_active (%1d) & step_index (%1d) ... \n", loop_active,i);
							printf("   ** repeat <- %s \n", para_data1);
							printf("   ** start  <- %s \n", para_data2);
							printf("   ** end    <- %s \n", para_data3);

							// ...
							loop_repaet = atoi(para_data1);
							loop_start  = atoi(para_data2);
							loop_end    = atoi(para_data3);

							loop_active = i;
							loop_count  = loop_repaet;
							loop_index  = loop_start - 1;

							// Error Check ...
							if(loop_active < loop_end)		
							{
								CString err_msg;
								CString err_bff;

								err_msg = " * Loop Parameter Error(1) ! \n";
								err_bff.Format("  ** loop_active : %1d \n", loop_active);
								err_msg += err_bff;
								err_bff.Format("  ** loop_end : %1d \n", loop_end);
								err_msg += err_bff;

								printf(err_msg);								
								return -1801;
							}
							if(loop_start >= loop_end)		
							{
								CString err_msg;
								CString err_bff;

								err_msg = " * Loop Parameter Error(2) ! \n";
								err_bff.Format("  ** loop_start : %1d \n", loop_start);
								err_msg += err_bff;
								err_bff.Format("  ** loop_end : %1d \n", loop_end);
								err_msg += err_bff;

								printf(err_msg);								
								return -1802;
							}
						}
						continue;
					}	

					if(loop_active > 0)
					{
						if(loop_index >= loop_end)
						{
							loop_count--;
							
							if(loop_count <= 0)		loop_active = 0;
							else					loop_index  = loop_start - 1;
						}
					}
					else
					{
						sCH__MACRO_MOVE__STEP_REPEAT_START->Set__DATA("");
						sCH__MACRO_MOVE__STEP_REPEAT_END->Set__DATA("");
						sCH__MACRO_MOVE__STEP_REPEAT_COUNT->Set__DATA("");
						sCH__MACRO_MOVE__STEP_REPEAT_INDEX->Set__DATA("");
					}
				}
			}

			// ...
		}
	}

	return 1;
}

int  CObj_Opr__MAINT_MODE::Call__MANUAL_MOVE(CII_OBJECT__VARIABLE *p_variable)
{
	printf(" * CObj_Opr__MAINT_MODE::Call__MANUAL_MOVE() ... \n");

	// ...
	CString para_src_module;
	CString para_src_slot;

	CString para_trg_module;
	CString para_trg_slot;

	// ...
	{
		sCH__MOVE_SRC_MODULE->Get__DATA(para_src_module);
		sCH__MOVE_SRC_SLOT->Get__DATA(para_src_slot);

		sCH__MOVE_TRG_MODULE->Get__DATA(para_trg_module);
		sCH__MOVE_TRG_SLOT->Get__DATA(para_trg_slot);

		//
		int pmc_index = Macro__Get_PM_INDEX(para_trg_module);
		if(pmc_index >= 0)
		{
			sEXT_CH__PARA_MANUAL_MOVE_ALIGN_TARGET_PMx->Set__DATA(para_trg_module);
		}
		else
		{
			CString ch_data = dCH__CFG_MANUAL_MOVE_ALIGN_TARGET_PMx->Get__STRING();
			sEXT_CH__PARA_MANUAL_MOVE_ALIGN_TARGET_PMx->Set__DATA(ch_data);
		}
	}

	// ...
	{
		printf(" * PARA.SRC ... \n");

		printf("  * %s <- %s \n", 
				sCH__MOVE_SRC_MODULE->Get__CHANNEL_NAME(),
				sCH__MOVE_SRC_MODULE->Get__STRING());
		printf("  * %s <- %s \n", 
				sCH__MOVE_SRC_SLOT->Get__CHANNEL_NAME(),
				sCH__MOVE_SRC_SLOT->Get__STRING());

		printf("  * %s <- %s \n", 
				sCH__MOVE_TRG_MODULE->Get__CHANNEL_NAME(),
				sCH__MOVE_TRG_MODULE->Get__STRING());
		printf("  * %s <- %s \n", 
				sCH__MOVE_TRG_SLOT->Get__CHANNEL_NAME(),
				sCH__MOVE_TRG_SLOT->Get__STRING());

		printf("\n");
	}

	// ...
	{
		CString ll_name;
		CString ll_slot;

		if(Check__LLx_ISLOT(para_src_module, ll_name,ll_slot))
		{
			para_src_module = ll_name;
			para_src_slot   = ll_slot;
		}
		else
		{
			para_src_module = Get__LLx_NAME(para_src_module);
		}

		if(Check__LLx_ISLOT(para_trg_module, ll_name,ll_slot))
		{
			para_trg_module = ll_name;
			para_trg_slot   = ll_slot;
		}
		else
		{
			para_trg_module = Get__LLx_NAME(para_trg_module);
		}
	}

	// VAC-Module Check ...
	{
		// Source Module ...
		{
			sEXT_CH__VAC_RB__CHECK_MODULE->Set__DATA(para_src_module);

			if(pVAC_RB__OBJ_CTRL->Call__OBJECT(VAC_CMMD__CHECK_STANDBY) < 0)
			{
				return -301;
			}
		}

		// Target Module ...
		{
			sEXT_CH__VAC_RB__CHECK_MODULE->Set__DATA(para_trg_module);

			if(pVAC_RB__OBJ_CTRL->Call__OBJECT(VAC_CMMD__CHECK_STANDBY) < 0)
			{
				return -302;
			}
		}
	}

	// ...
	{
		int lp_index;

		// Source Module ...
		{
			lp_index = Macro__Get_LP_INDEX(para_src_module);

			if((lp_index >= 0)
			&& (lp_index <  iLPx_SIZE))
			{
				sEXT_CH__LPx_LOCK_FLAG[lp_index]->Set__DATA(STR__YES);
			}
		}

		// Target Module ...
		{
			lp_index = Macro__Get_LP_INDEX(para_trg_module);

			if((lp_index >= 0)
			&& (lp_index <  iLPx_SIZE))
			{
				sEXT_CH__LPx_LOCK_FLAG[lp_index]->Set__DATA(STR__YES);
			}
		}
	}

	// ...
	int s_atm_rb_area = -1;
	int s_vac_rb_area = -1;
	int t_atm_rb_area = -1;
	int t_vac_rb_area = -1;

	para_src_module.MakeUpper();
	para_src_slot.MakeUpper();
	para_trg_module.MakeUpper();
	para_trg_slot.MakeUpper();

	// ...
	int limit;
	int i;

	if(s_vac_rb_area < 0)
	{
		printf(" ** s_vac_rb_area <- [%1d] \n", s_vac_rb_area);

		limit = sList__ATM_AREA.GetSize();
		for(i=0;i<limit;i++)
		{
			if(para_src_module.Find(sList__ATM_AREA[i]) < 0)
			{
				continue;
			}

			s_atm_rb_area = 1;
			break;
		}
	}
	if(s_atm_rb_area < 0)
	{
		printf(" ** s_atm_rb_area <- [%1d] \n", s_atm_rb_area);

		limit = sList__VAC_AREA.GetSize();
		for(i=0;i<limit;i++)
		{
			if(para_src_module.Find(sList__VAC_AREA[i]) < 0)
			{
				continue;
			}

			s_vac_rb_area = 1;
			break;
		}
	}

	if(t_vac_rb_area < 0)
	{
		printf(" ** t_vac_rb_area <- [%1d] \n", t_vac_rb_area);

		limit = sList__ATM_AREA.GetSize();
		for(i=0;i<limit;i++)
		{
			if(para_trg_module.Find(sList__ATM_AREA[i]) < 0)
			{
				continue;
			}

			t_atm_rb_area = 1;
			break;
		}
	}
	if(t_atm_rb_area < 0)
	{
		printf(" ** t_atm_rb_area <- [%1d] \n", t_atm_rb_area);

		limit = sList__VAC_AREA.GetSize();
		for(i=0;i<limit;i++)
		{
			if(para_trg_module.Find(sList__VAC_AREA[i]) < 0)
			{
				continue;
			}

			t_vac_rb_area = 1;
			break;
		}
	}

	// ...
	{
		printf(" * Robot.Area Report ... \n");
		printf(" * para_src_module <- %s \n", para_src_module);
		printf(" * para_trg_module <- %s \n", para_trg_module);
		printf("  * s_atm_rb_area <- %1d \n", s_atm_rb_area);
		printf("  * s_vac_rb_area <- %1d \n", s_vac_rb_area);
		printf("  * t_atm_rb_area <- %1d \n", t_atm_rb_area);
		printf("  * t_vac_rb_area <- %1d \n", t_vac_rb_area);
	}

	// ...
	{
		CString log_msg;
		CString log_bff;

		log_msg  = "\n";
		log_msg += "Manual Move : Started ...\n";

		log_bff.Format("[%s][%s] -> [%s][%s]\n", 
						para_src_module,para_src_slot,
						para_trg_module,para_trg_slot);
		log_msg += log_bff;

		log_bff.Format("s_atm_rb_area [%1d], s_vac_rb_area [%1d]\n",
						s_atm_rb_area, s_vac_rb_area);
		log_msg += log_bff;

		log_bff.Format("t_atm_rb_area [%1d], t_vac_rb_area [%1d]\n",
						t_atm_rb_area, t_vac_rb_area);
		log_msg += log_bff;

		xAPP_LOG_CTRL->WRITE__LOG(log_msg);
	}

	int ll_i = Get__LLx_INDEX(para_trg_module);
	if(ll_i >= 0)
	{
		sEXT_CH__TARGET_LLx_NAME->Set__DATA(para_trg_module);
		sEXT_CH__TARGET_LLx_SLOT->Set__DATA(para_trg_slot);		
	}

	// ...
	int flag = -1;

	if((s_atm_rb_area > 0)&&(t_atm_rb_area > 0))
	{
		printf(" ** routine (1) ... \n");

		flag = Fnc__ATM_RB__Manual_Move_To(p_variable,
											para_src_module,para_src_slot,
											para_trg_module,para_trg_slot);		
	}
	else if((s_atm_rb_area > 0)&&(t_vac_rb_area > 0))
	{
		int ll_i = Get__LLx_INDEX(para_src_module);
		if(ll_i >= 0)
		{
			printf(" ** routine (2) ... \n");

			flag = Fnc__VAC_RB__Manual_Move_To(p_variable,
												para_src_module,para_src_slot,
												para_trg_module,para_trg_slot);		
		}
		else
		{
			printf(" ** routine (3) ... \n");

			CString bff_module = "LBI";
			CString bff_slot   = "X";

			flag = Fnc__ATM_RB__Manual_Move_To(p_variable,
												para_src_module,para_src_slot,
												bff_module,bff_slot);		

			if(flag > 0)
			{
				flag = Fnc__VAC_RB__Manual_Move_To(p_variable,
													bff_module,bff_slot,
													para_trg_module,para_trg_slot);
			}
		}
	}
	else if((s_vac_rb_area > 0)&&(t_atm_rb_area > 0))
	{
		if((para_trg_module.CompareNoCase("LBA") == 0)
		|| (para_trg_module.CompareNoCase("LBB") == 0)
		|| (para_trg_module.CompareNoCase("LBC") == 0)
		|| (para_trg_module.CompareNoCase("LBD") == 0))
		{
			printf(" ** routine (4) ... \n");

			flag = Fnc__VAC_RB__Manual_Move_To(p_variable,
												para_src_module,para_src_slot,
												para_trg_module,para_trg_slot);		
		}
		else
		{
			printf(" ** routine (5) ... \n");

			CString bff_module = "LBO";
			CString bff_slot   = "X";

			flag = Fnc__VAC_RB__Manual_Move_To(p_variable,
												para_src_module,para_src_slot,
												bff_module,bff_slot);		

			if(flag > 0)
			{
				if(para_trg_module.CompareNoCase("LLX") == 0)
				{
					sCH__MOVE_TRG_MODULE->Set__DATA(bff_module);
					sCH__MOVE_TRG_SLOT->Set__DATA(bff_slot);
				}
				else
				{
					flag = Fnc__ATM_RB__Manual_Move_To(p_variable,
														bff_module,bff_slot,
														para_trg_module,para_trg_slot);
				}
			}
		}
	}
	else if((s_vac_rb_area > 0)&&(t_vac_rb_area > 0))
	{
		printf(" ** routine (6) ... \n");

		flag = Fnc__VAC_RB__Manual_Move_To(p_variable,
											para_src_module,para_src_slot,
											para_trg_module,para_trg_slot);		
	}
	else
	{
		return -11;
	}

	// ...
	{
		CString log_msg;
		CString log_bff;

		log_msg  = "\n";
		log_msg += "Manual Move : Completed ...\n";

		log_bff.Format("Result Flag : [%1d]\n", flag);
		log_msg += log_bff;

		xAPP_LOG_CTRL->WRITE__LOG(log_msg);
	}

	if(sCH__MACRO_MOVE__LOG_STATUS->Check__DATA(STR__START) > 0)
	{
		CString str_cmmd;

		int log_index = mMacro_Log.Load__MOVE_INFO(_CMMD__MACRO_MOVE__MANUAL_MOVE, para_src_module,para_src_slot, para_trg_module,para_trg_slot, str_cmmd);

		if((log_index >= 0)
		&& (log_index < CFG_SIZE__MOVE_HISTORY))
		{
			sCH__MACRO_MOVE__HISTORY[log_index]->Set__DATA(str_cmmd);

			CString ch_data;
			ch_data.Format("%1d", log_index+1);
			sCH__MACRO_MOVE__LOG_COUNT->Set__DATA(ch_data);
		}
	}

	return flag;
}
int  CObj_Opr__MAINT_MODE::Call__TOOL_CLEAR(CII_OBJECT__VARIABLE *p_variable)
{
	DECLARE__EXT_CTRL(p_variable);

	// ...
	CStringArray l_para;
	CString para_data;

	int material_count = 0;

	while(1)
	{
		Sleep(500);

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -1;
		}

		// ATM AREA -> LPx ...
		{
			if(pATM_RB__OBJ_CTRL->Call__OBJECT(ATM_CMMD__CLEAR) < 0)
			{
				return -2;
			}
			if(p_variable->Check__CTRL_ABORT() > 0)
			{
				return -3;
			}
		}

		// VAC AREA -> LBx ...
		{
			if(pVAC_RB__OBJ_CTRL->Call__OBJECT(VAC_CMMD__CLEAR) < 0)
			{
				return -4;
			}
			if(p_variable->Check__CTRL_ABORT() > 0)
			{
				return -5;
			}
		}

		// MATERIAL CHECK ...
		material_count = 0;

		// ATM AREA ...
		{
			pATM_RB__OBJ_CTRL->Call__OBJECT(ATM_CMMD__CHECK_MATERIAL);
			pATM_RB__OBJ_CTRL->Get__RETURN_FNC_PARAMETER(l_para);

			if(l_para.GetSize() > 0)
			{
				material_count += atoi(l_para[0]);
			}
		}

		// VAC AREA ...
		{
			pVAC_RB__OBJ_CTRL->Call__OBJECT(VAC_CMMD__CHECK_MATERIAL);
			pVAC_RB__OBJ_CTRL->Get__RETURN_FNC_PARAMETER(l_para);

			if(l_para.GetSize() > 0)
			{
				material_count += atoi(l_para[0]);
			}
		}

		if(material_count < 1)
		{
			if(sCH__MACRO_MOVE__LOG_STATUS->Check__DATA(STR__START) > 0)
			{
				CString str_cmmd;
				int log_index = mMacro_Log.Load__MOVE_INFO(_CMMD__MACRO_MOVE__TOOL_CLEAR, "","", "","", str_cmmd);

				if((log_index >= 0)
				&& (log_index < CFG_SIZE__MOVE_HISTORY))
				{
					sCH__MACRO_MOVE__HISTORY[log_index]->Set__DATA(str_cmmd);

					CString ch_data;
					ch_data.Format("%1d", log_index+1);
					sCH__MACRO_MOVE__LOG_COUNT->Set__DATA(ch_data);
				}
			}
			return 1;
		}
	}

	return 1;
}


//-------------------------------------------------------------------------
int  CObj_Opr__MAINT_MODE::
Fnc__ATM_RB__Manual_Move_To(CII_OBJECT__VARIABLE *p_variable,
							const CString& s_module,
							const CString& s_slot,
							CString& t_module,
							CString& t_slot)
{
	DECLARE__EXT_CTRL(p_variable);

	printf(" * Fnc__ATM_RB__Manual_Move_To() ... \n");
	printf(" ** src : [%s][%s] \n", s_module,s_slot);
	printf(" ** trg : [%s][%s] \n", t_module,t_slot);

	// ...
	CString arm_type;

	if((s_module.CompareNoCase(MODULE__ATM_RB1_A) == 0)
	|| (s_module.CompareNoCase(MODULE__ATM_RB1_B) == 0))
	{
		arm_type = s_module;

		if(ATM_RB__Place_Module(p_variable,arm_type,t_module,t_slot) < 0)
		{
			return -11;
		}
	}
	else if((t_module.CompareNoCase(MODULE__ATM_RB1_A) == 0)
		 || (t_module.CompareNoCase(MODULE__ATM_RB1_B) == 0))
	{
		arm_type = t_module;

		if(ATM_RB__Pick_Module_With_Arm(p_variable,arm_type,s_module,s_slot) < 0)
		{
			return -12;
		}
	}
	else
	{
		bool active__src_al = false;
		if(s_module.Find("AL") == 0)		active__src_al = true;

		printf(" ** s_module <- [%s] \n", s_module);
		printf(" ** t_module <- [%s] \n", t_module);
		printf(" ** active__src_al <- [%1d] \n", active__src_al);

		if(active__src_al)
		{
			printf(" ** ATM_RB__Pick_Module() : [%s %s] -> [%s] \n", s_module,s_slot,arm_type);

			if(ATM_RB__Pick_Module(p_variable,s_module,s_slot,arm_type)  < 0)
			{
				return -131;
			}

			printf(" ** ATM_RB__Place_Module() : [%s]<- [%s %s] \n", arm_type,t_module,t_slot);

			if(ATM_RB__Place_Module(p_variable,arm_type,t_module,t_slot, false) < 0)
			{
				return -141;
			}
		}
		else
		{
			printf(" ** ATM_RB__Pick_Module() : [%s %s] -> [%s] \n", s_module,s_slot,arm_type);

			if(ATM_RB__Pick_Module(p_variable,s_module,s_slot,arm_type)  < 0)
			{
				return -13;
			}

			printf(" ** ATM_RB__Place_Module() : [%s] <- [%s %s] \n", arm_type,t_module,t_slot);

			if(ATM_RB__Place_Module(p_variable,arm_type,t_module,t_slot) < 0)
			{
				return -14;
			}
		}
	}

	return 1;
}

int  CObj_Opr__MAINT_MODE::ATM_RB__Pick_Module(CII_OBJECT__VARIABLE *p_variable,
										  const CString& str_module,
										  const CString& str_slot,
										  CString& arm_type)
{
	if((str_module.CompareNoCase(MODULE__ATM_RB1_A) == 0)
	|| (str_module.CompareNoCase(MODULE__ATM_RB1_B) == 0))
	{
		arm_type = str_module;
		return 1;
	}

	arm_type = "X";
	
	return ATM_RB__Pick_Module_With_Arm(p_variable,arm_type,str_module,str_slot);
}
int  CObj_Opr__MAINT_MODE::
ATM_RB__Pick_Module_With_Arm(CII_OBJECT__VARIABLE *p_variable,
							 CString& str_arm,
							 const CString& str_module,
							 const CString& str_slot)
{
	DECLARE__EXT_CTRL(p_variable);

	// LLx ...
	{
		int ll_index = Macro__Get_LL_INDEX(str_module);
		if(ll_index >= 0)
		{
			if(dCH__PARTICLE_PARA_LLx_COOL_TYPE->Check__DATA(STR__TMC) > 0)
			{
				if(dEXT_CH__LLx_PRESSURE_STATUS[ll_index]->Check__DATA(STR__ATM) < 0)
				{
					CString ch_data = aCH__PARTICLE_PARA_LLx_COOLING_SEC->Get__STRING();
					aEXT_CH__LLx_PARA_VENT_COOLING_SEC[ll_index]->Set__DATA(ch_data);
				}
			}

			if(pLLx__OBJ_CTRL[ll_index]->Call__OBJECT("VENT") < 0)
			{
				return -1001;
			}
		}
	}
	// STx ...
	{

	}

	// ...
	CString arm_type = Get__ATM_RB1__ARM_TYPE(str_arm);
	
	CStringArray l_para;

	// ...
	{
		l_para.RemoveAll();
		l_para.Add(arm_type);
		l_para.Add(str_module);
		l_para.Add(str_slot);
		l_para.Add("Manual Move - Pick");
	}

	// ...
	int flag = pATM_RB__OBJ_CTRL->Call__OBJ_MODE(ATM_CMMD__MAINT_PICK,l_para);

	if(str_arm == "X")
	{
		CStringArray l_para;
		pATM_RB__OBJ_CTRL->Get__RETURN_FNC_PARAMETER(l_para);

		int limit = l_para.GetSize();
		int i;

		for(i=0;i<limit;i++)
		{
			switch(i)
			{
				case 0:		
					str_arm = l_para[i];
					break;
			}
		}
	}

	return flag;
}
int  CObj_Opr__MAINT_MODE
::ATM_RB__Place_Module(CII_OBJECT__VARIABLE *p_variable,
					   const CString& str_arm,
					   CString& str_module,
					   CString& str_slot,
					   const bool active__al_retry)
{
	DECLARE__EXT_CTRL(p_variable);

	// ...
	{
		if(str_module.CompareNoCase(MODULE__ATM_RB1_A) == 0)
		{
			if(str_arm == "A")		return 1;

			// Error
			return -1;
		}
		if(str_module.CompareNoCase(MODULE__ATM_RB1_B) == 0)
		{
			if(str_arm == "B")		return 1;

			// Error
			return -1;
		}
	}

	// ...
	CString arm_type = Get__ATM_RB1__ARM_TYPE(str_arm);

	if(active__al_retry)
	{
		/*
		if((str_module.CompareNoCase("LBI") == 0)
		|| (str_module.CompareNoCase("LBA") == 0)
		|| (str_module.CompareNoCase("LBB") == 0)
		|| (str_module.CompareNoCase("LBC") == 0)
		|| (str_module.CompareNoCase("LBD") == 0))
		{
			CString str_module = "AL1";
			CString str_slot   = "1";

			if(Seq__ATM_RB__Place(p_variable,arm_type,str_module,str_slot) < 0)
			{
				return -1;
			}
			if(Seq__ATM_RB__Pick(p_variable,arm_type,str_module,str_slot)  < 0)
			{
				return -1;
			}
		}
		*/
	}

	// ...
	CStringArray l_para;

	// ...
	{
		l_para.RemoveAll();
		l_para.Add(arm_type);
		l_para.Add(str_module);
		l_para.Add(str_slot);
		l_para.Add("Manual Move - Place");
	}

	// ...
	int flag = pATM_RB__OBJ_CTRL->Call__OBJ_MODE(ATM_CMMD__MAINT_PLACE,l_para);

	if(str_module.CompareNoCase("LBI") == 0)
	{
		pATM_RB__OBJ_CTRL->Get__RETURN_FNC_PARAMETER(l_para);

		// ...
		int limit = l_para.GetSize();
		int i;

		for(i=0;i<limit;i++)
		{
			switch(i)
			{
				case 0:		
					str_module = l_para[i];		
					break;

				case 1:		
					str_slot = l_para[i];		
					break;
			}
		}
	}
	else
	{
		if((xCH__ACTIVE_MODE->Check__DATA(sMODE__MACRO_MOVE) > 0)
		&& (flag > 0))	
		{
			int check_flag = -1;

			if(check_flag < 0)
			{
				int lp_i = Macro__Get_LP_INDEX(str_module);

				if(lp_i >= 0)
				{
					check_flag = 1;

					// ...
					SCX__TIMER_CTRL x_timer_ctrl;
					double cfg_sec = 0.0;

					x_timer_ctrl->REGISTER__COUNT_CHANNEL(sCH__MACRO_MOVE__FOUP_WAIT_CUR_SEC->Get__CHANNEL_NAME());

					// ...
					{
						CString var_data;

						aCH__MACRO_MOVE__CFG_FOUP_WAIT_SEC->Get__DATA(var_data);
						cfg_sec = atof(var_data);
					}

					flag = x_timer_ctrl->WAIT(cfg_sec);
				}
			}

			if(check_flag < 0)
			{
				if((str_module.CompareNoCase(STR__BUFF1) == 0)
				|| (str_module.CompareNoCase(STR__BUFF2) == 0))
				{
					check_flag = 1;

					// ...
					SCX__TIMER_CTRL x_timer_ctrl;
					double cfg_sec = 0.0;

					x_timer_ctrl->REGISTER__COUNT_CHANNEL(sCH__MACRO_MOVE__BUFFER_WAIT_CUR_SEC->Get__CHANNEL_NAME());

					// ...
					{
						CString var_data;

						aCH__MACRO_MOVE__CFG_BUFFER_WAIT_SEC->Get__DATA(var_data);
						cfg_sec = atof(var_data);
					}

					flag = x_timer_ctrl->WAIT(cfg_sec);
				}
			}
		}

		// ...
	}

	return flag;
}

int  CObj_Opr__MAINT_MODE::
Seq__ATM_RB__Place(CII_OBJECT__VARIABLE* p_variable,
				   const CString& arm_type,
				   const CString& str_module,
				   const CString& str_slot)
{
	DECLARE__EXT_CTRL(p_variable);

	// ...
	CStringArray l_para;

	// ...
	{
		l_para.RemoveAll();
		l_para.Add(arm_type);
		l_para.Add(str_module);
		l_para.Add(str_slot);
		l_para.Add("Manual Move - Place");
	}

	return pATM_RB__OBJ_CTRL->Call__OBJ_MODE(ATM_CMMD__MAINT_PLACE,l_para);
}
int  CObj_Opr__MAINT_MODE::
Seq__ATM_RB__Pick(CII_OBJECT__VARIABLE* p_variable,
				  const CString& arm_type,
				  const CString& str_module,
				  const CString& str_slot)
{
	DECLARE__EXT_CTRL(p_variable);

	// ...
	CStringArray l_para;

	// ...
	{
		l_para.RemoveAll();
		l_para.Add(arm_type);
		l_para.Add(str_module);
		l_para.Add(str_slot);
		l_para.Add("Manual Move - Pick");
	}

	return pATM_RB__OBJ_CTRL->Call__OBJ_MODE(ATM_CMMD__MAINT_PICK,l_para);
}


//-------------------------------------------------------------------------
int  CObj_Opr__MAINT_MODE
::Fnc__VAC_RB__Manual_Move_To(CII_OBJECT__VARIABLE *p_variable,
							  const CString& s_module,
							  const CString& s_slot,
							  CString& t_module,
							  CString& t_slot)
{
	CString arm_type;

	if((s_module == MODULE__VAC_RB1_A)
	|| (s_module == MODULE__VAC_RB1_B))
	{
		arm_type = s_module;

		if(VAC_RB__Place_Module(p_variable,arm_type,t_module,t_slot) < 0)			
		{
			return -1;
		}
	}
	else if((t_module == MODULE__VAC_RB1_A)
		 || (t_module == MODULE__VAC_RB1_B))
	{
		arm_type = t_module;

		if(VAC_RB__Pick_Module_With_Arm(p_variable,arm_type,s_module,s_slot) < 0)
		{
			return -2;
		}
	}
	else
	{
		if(VAC_RB__Pick_Module(p_variable,s_module,s_slot,arm_type)  < 0)
		{
			return -3;
		}
		if(VAC_RB__Place_Module(p_variable,arm_type,t_module,t_slot) < 0)
		{
			return -4;
		}
	}

	return 1;
}

int  CObj_Opr__MAINT_MODE
::VAC_RB__Pick_Module(CII_OBJECT__VARIABLE *p_variable,
					  const CString& str_module,
					  const CString& str_slot,
					  CString& arm_type)
{
	if((str_module == MODULE__VAC_RB1_A)
	|| (str_module == MODULE__VAC_RB1_B))
	{
		arm_type = str_module;
		return 1;
	}

	arm_type = "X";
	return VAC_RB__Pick_Module_With_Arm(p_variable,arm_type,str_module,str_slot);
}
int  CObj_Opr__MAINT_MODE
::VAC_RB__Pick_Module_With_Arm(CII_OBJECT__VARIABLE *p_variable,
							   CString& str_arm,
							   const CString& str_module,
							   const CString& str_slot)
{
	DECLARE__EXT_CTRL(p_variable);

	// ...
	CString arm_type = Get__VAC_RB1__ARM_TYPE(str_arm);

	// ...
	CStringArray l_para;

	// ...
	{
		l_para.RemoveAll();
		l_para.Add(arm_type);
		l_para.Add(str_module);
		l_para.Add(str_slot);
		l_para.Add("Manual Move - Pick");
	}

	// ...
	int flag = pVAC_RB__OBJ_CTRL->Call__OBJ_MODE(VAC_CMMD__MAINT_PICK,l_para);

	if(str_arm == "X")
	{
		CStringArray l_para;
		pVAC_RB__OBJ_CTRL->Get__RETURN_FNC_PARAMETER(l_para);

		// ...
		int limit = l_para.GetSize();
		int i;

		for(i=0;i<limit;i++)
		{
			switch(i)
			{
				case 0:		
					str_arm = l_para[i];		
					break;
			}
		}
	}

	return flag;
}
int  CObj_Opr__MAINT_MODE
::VAC_RB__Place_Module(CII_OBJECT__VARIABLE *p_variable,
					   const CString& str_arm,
					   CString& str_module,
					   CString& str_slot)
{
	DECLARE__EXT_CTRL(p_variable);

	// ...
	CString arm_type = Get__VAC_RB1__ARM_TYPE(str_arm);

	if(str_module.CompareNoCase(MODULE__VAC_RB1_A) == 0)
	{
		if(arm_type == "A")		return 1;

		// Error
		return -1;
	}
	if(str_module.CompareNoCase(MODULE__VAC_RB1_B) == 0)
	{
		if(arm_type == "B")		return 1;

		// Error
		return -1;
	}

	// ...
	CStringArray l_para;

	// ...
	{
		l_para.RemoveAll();
		l_para.Add(arm_type);
		l_para.Add(str_module);
		l_para.Add(str_slot);
		l_para.Add("Manual Move - Place");
	}

	// ...
	{
		CString log_msg;
		CString log_bff;

		log_msg = "VAC_RB__Place_Module() : Started ! \n";
		log_bff.Format(" * arm_type <- %s \n", arm_type);
		log_msg += log_bff;
		log_bff.Format(" * str_module <- %s \n", str_module);
		log_msg += log_bff;
		log_bff.Format(" * str_slot <- %s \n", str_slot);
		log_msg += log_bff;

		xAPP_LOG_CTRL->WRITE__LOG(log_msg);
	}

	int flag = pVAC_RB__OBJ_CTRL->Call__OBJ_MODE(VAC_CMMD__MAINT_PLACE,l_para);

	if(str_module.CompareNoCase("LBO") == 0)
	{
		CStringArray l_para;
		pVAC_RB__OBJ_CTRL->Get__RETURN_FNC_PARAMETER(l_para);

		int i_limit = l_para.GetSize();
		for(int i=0; i<i_limit; i++)
		{
			switch(i)
			{
				case 0:		str_module = l_para[i];		break;
				case 1:		str_slot = l_para[i];		break;
			}
		}
	}
	else
	{
		if((xCH__ACTIVE_MODE->Check__DATA(sMODE__MACRO_MOVE) > 0)
		&& (flag > 0))	
		{
			// ...
			{
				int pm_i = Macro__Get_PM_INDEX(str_module);

				if(pm_i >= 0)
				{	
					SCX__TIMER_CTRL x_timer_ctrl;
					double cfg_sec = 0.0;

					// ...
					{
						x_timer_ctrl->REGISTER__COUNT_CHANNEL(sCH__MACRO_MOVE__PMC_WAIT_CUR_SEC->Get__CHANNEL_NAME());
					}

					// ...
					{
						CString var_data;

						aCH__MACRO_MOVE__CFG_PMC_WAIT_SEC->Get__DATA(var_data);
						cfg_sec = atof(var_data);
					}

					flag = x_timer_ctrl->WAIT(cfg_sec);
				}
			}
		}
	}

	return flag;
}

int  CObj_Opr__MAINT_MODE::
Seq__VAC_RB__Place(CII_OBJECT__VARIABLE* p_variable,
				   const CString& arm_type,
				   const CString& str_module,
				   const CString& str_slot)
{
	DECLARE__EXT_CTRL(p_variable);

	// ...
	CStringArray l_para;

	// ...
	{
		l_para.RemoveAll();

		l_para.Add(arm_type);
		l_para.Add(str_module);
		l_para.Add(str_slot);
		l_para.Add("Manual Move - Place");
	}

	return pVAC_RB__OBJ_CTRL->Call__OBJ_MODE(VAC_CMMD__MAINT_PLACE, l_para);
}
int  CObj_Opr__MAINT_MODE::
Seq__VAC_RB__Pick(CII_OBJECT__VARIABLE* p_variable,
				  const CString& arm_type,
				  const CString& str_module,
				  const CString& str_slot)
{
	DECLARE__EXT_CTRL(p_variable);

	// ...
	CStringArray l_para;

	// ...
	{
		l_para.RemoveAll();
		
		l_para.Add(arm_type);
		l_para.Add(str_module);
		l_para.Add(str_slot);
		l_para.Add("Manual Move - Pick");
	}

	return pVAC_RB__OBJ_CTRL->Call__OBJ_MODE(VAC_CMMD__MAINT_PICK, l_para);
}

