#include "StdAfx.h"
#include "CObj_Opr__AUTO_MODE.h"
#include "CObj_Opr__AUTO_MODE__DEF.h"


// ...
CString CObj_Opr__AUTO_MODE::Get__LLx_NAME(const int ll_i)
{
	if(ll_i == _LLx_INDEX__LBA)			return STR__LBA;
	if(ll_i == _LLx_INDEX__LBB)			return STR__LBB;
	if(ll_i == _LLx_INDEX__LBC)			return STR__LBC;
	if(ll_i == _LLx_INDEX__LBD)			return STR__LBD;

	return STR__LLx;
}

int  CObj_Opr__AUTO_MODE::Update__CFG_DB_TO_SCH_DB()
{
	CString ch_data;

	int i_limit;
	int i;

	// PMx INFO ...
	{
		i_limit = CFG_PM_LIMIT;

		for(i=0; i<i_limit; i++)
		{
			sEXT_CH__PMx__CUR_PRE_COUNT[i]->Set__DATA("");
			sEXT_CH__PMx__CUR_POST_COUNT[i]->Set__DATA("");
		}
	}

	// ALx ...
	{
		ch_data = xEXT_CH__CFG_DB_AL1_USE_FLAG->Get__STRING();
		xEXT_CH__SCH_DB_AL1_USE_FLAG->Set__DATA(ch_data);

		ch_data = xEXT_CH__CFG_DB_AL1_SLOT01_USE_FLAG->Get__STRING();
		xEXT_CH__SCH_DB_AL1_SLOT01_USE_FLAG->Set__DATA(ch_data);

		ch_data = xEXT_CH__CFG_DB_AL1_SLOT02_USE_FLAG->Get__STRING();
		xEXT_CH__SCH_DB_AL1_SLOT02_USE_FLAG->Set__DATA(ch_data);
	}

	// LLx ...
	{
		for(int ll_i=0; ll_i<iLLx_SIZE; ll_i++)
		{
			ch_data = xEXT_CH__CFG_DB_LLx_USE_FLAG[ll_i]->Get__STRING();
			xEXT_CH__SCH_DB_LLx_USE_FLAG[ll_i]->Set__DATA(ch_data);

			ch_data = xEXT_CH__CFG_DB_LLx_SLOT_MAX[ll_i]->Get__STRING();
			xEXT_CH__SCH_DB_LLx_SLOT_MAX[ll_i]->Set__DATA(ch_data);

			ch_data = xEXT_CH__CFG_DB_LLx_IDLE_STATUS[ll_i]->Get__STRING();
			xEXT_CH__SCH_DB_LLx_IDLE_STATUS[ll_i]->Set__DATA(ch_data);

			ch_data = pEXT_CH__CFG_DB_LLx_MODE_TYPE[ll_i]->Get__STRING();
			pEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_i]->Set__DATA(ch_data);

			//
			i_limit = CFG_LLx__SLOT_MAX;

			for(i=0; i<i_limit; i++)
			{
				ch_data = pEXT_CH__CFG_DB_LLx_SLOT_STATUS[ll_i][i]->Get__STRING();
				pEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_i][i]->Set__DATA(ch_data);

				ch_data = pEXT_CH__CFG_DB_LLx_SLOT_MODE[ll_i][i]->Get__STRING();
				pEXT_CH__SCH_DB_LLx_SLOT_MODE[ll_i][i]->Set__DATA(ch_data);
			}
		}
	}

	// STx ...
	{
		sEXT_CH__SCH_DB_ST1_USE_FLAG->Set__DATA(STR__DISABLE);
		sEXT_CH__SCH_DB_ST2_USE_FLAG->Set__DATA(STR__DISABLE);

		for(i=0; i<iBUFF_UNIT_SIZE; i++)
		{
			if(i >= CFG_BUFFER__UNIT_LIMIT)
			{
				continue;
			}
			if(xEXT_CH__CFG_DB_STx_USE_FLAG[i]->Check__DATA(STR__ENABLE) < 0)
			{
				continue;
			}

			// ...
			ch_data = xEXT_CH__CFG_DB_STx_USE_FLAG[i]->Get__STRING();

				 if(i == 0)		sEXT_CH__SCH_DB_ST1_USE_FLAG->Set__DATA(ch_data);
			else if(i == 1)		sEXT_CH__SCH_DB_ST2_USE_FLAG->Set__DATA(ch_data);
		}

		//
		ch_data = xEXT_CH__CFG_DB_STx_TRANSFER_MODE->Get__STRING();
		xEXT_CH__SCH_DB_STx_TRANSFER_MODE->Set__DATA(ch_data);

		//
		ch_data = xEXT_CH__CFG_DB_STx_APPLY_MODE->Get__STRING();
		xEXT_CH__SCH_DB_STx_APPLY_MODE->Set__DATA(ch_data);

		//
		ch_data = dEXT_CH__CFG_DB_STx_OUT_MODE_BUFFER_TO_LPo->Get__STRING();
		dEXT_CH__SCH_DB_ST1_OUT_MODE_BUFFER_TO_LPo->Set__DATA(ch_data);
		dEXT_CH__SCH_DB_ST2_OUT_MODE_BUFFER_TO_LPo->Set__DATA(ch_data);

		//
		ch_data = xEXT_CH__CFG_DB_STx_WAFER_PICK_MODE->Get__STRING();
		xEXT_CH__SCH_DB_ST1_WAFER_PICK_MODE->Set__DATA(ch_data);
		xEXT_CH__SCH_DB_ST2_WAFER_PICK_MODE->Set__DATA(ch_data);

		//
		xEXT_CH__CFG_DB_BUFFER_WAFER_CLEAN_TIME->Get__DATA(ch_data);

		xEXT_CH__SCH_DB_ST1_WAFER_CLEAN_TIME->Set__DATA(ch_data);
		xEXT_CH__SCH_DB_ST2_WAFER_CLEAN_TIME->Set__DATA(ch_data);
	}

	return 1;
}

// ...
int  CObj_Opr__AUTO_MODE::Update__ROUTE_DB_TO_SCH_DB(const int lp_id)
{
	int r_flag = _Update__ROUTE_DB_TO_SCH_DB(lp_id);

	// ...
	{
		CString log_msg;
		CString log_bff;

		log_msg = "\n";
		log_bff.Format("Update__ROUTE_DB_TO_SCH_DB(LP%1d) ... \n", lp_id);
		log_msg += log_bff;
		log_bff.Format(" * r_flag <- [%1d] \n", r_flag);
		log_msg += log_bff;

		xLOG_CTRL->WRITE__LOG(log_msg);
	}
	return r_flag;
}
int  CObj_Opr__AUTO_MODE::_Update__ROUTE_DB_TO_SCH_DB(const int lp_id)
{
	// ...
	{
		int err_check = 0;

		if(xCH__CFG_SCH_REF->Check__DATA(STR__ROUTE) < 0)
		{
			err_check = 1;
		}
		if(xSCH_MATERIAL_CTRL->Check__EDIT_TYPE__STATE() < 0)
		{
			err_check = 2;
		}
		if(xSCH_MATERIAL_CTRL->Check__SCH_CFG() > 0)
		{
			err_check = 3;
		}

		if(err_check > 0)
		{
			sEXT_CH__SCH_MODE_LLx_IN->Set__DATA("");
			sEXT_CH__SCH_MODE_LLx_OUT->Set__DATA("");

			return -err_check;
		}

		// ...
		{
			sEXT_CH__SCH_MODE_LLx_IN->Set__DATA(_SCH_MODE__ROUTE);
			sEXT_CH__SCH_MODE_LLx_OUT->Set__DATA(_SCH_MODE__ROUTE);
		}
	}

	// MODULE_X : DISABLE ...
	{
		// ALx ...
		{
			xEXT_CH__SCH_DB_AL1_USE_FLAG->Set__DATA(STR__DISABLE);
		}

		// LLx ...
		{
			for(int ll_i=0; ll_i<iLLx_SIZE; ll_i++)
			{
				xEXT_CH__SCH_DB_LLx_USE_FLAG[ll_i]->Set__DATA(STR__DISABLE);
				pEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_i]->Set__DATA(STR__ALL);
			}
		}
	}

	// ALx ...
	{
		CStringArray l_id;
		CStringArray l_mode; 
		CStringArray l_slot;
	
		xSCH_MATERIAL_CTRL->Get__ALx_OF_EDIT_TYPE(l_id, l_mode, l_slot);

		int i_limit = l_id.GetSize();
		int i;

		for(i=0; i<i_limit; i++)
		{
			int al_id = atoi(l_id[i]);

			CString str_mode = l_mode[i];
			CString str_slot = l_slot[i];

			if(al_id == 1)
			{
				CString mode_slot01 = STR__DISABLE;
				CString mode_slot02 = STR__DISABLE;

				//
				xEXT_CH__SCH_DB_AL1_USE_FLAG->Set__DATA(STR__ENABLE);

				if(str_slot.CompareNoCase("11") == 0)
				{
					mode_slot01 = STR__ENABLE;
					mode_slot02 = STR__ENABLE;
				}
				else if(str_slot.CompareNoCase("10") == 0)
				{
					mode_slot01 = STR__ENABLE;
				}
				else if(str_slot.CompareNoCase("01") == 0)
				{
					mode_slot02 = STR__ENABLE;
				}
				else
				{
					xEXT_CH__SCH_DB_AL1_USE_FLAG->Set__DATA(STR__DISABLE);
				}

				// SLOT01 ...
				{
					if(xEXT_CH__CFG_DB_AL1_SLOT01_USE_FLAG->Check__DATA(STR__DISABLE) > 0)
						xEXT_CH__SCH_DB_AL1_SLOT01_USE_FLAG->Set__DATA(STR__DISABLE);
					else
						xEXT_CH__SCH_DB_AL1_SLOT01_USE_FLAG->Set__DATA(mode_slot01);
				}
				// SLOT02 ...
				{
					if(xEXT_CH__CFG_DB_AL1_SLOT02_USE_FLAG->Check__DATA(STR__DISABLE) > 0)
						xEXT_CH__SCH_DB_AL1_SLOT02_USE_FLAG->Set__DATA(STR__DISABLE);
					else
						xEXT_CH__SCH_DB_AL1_SLOT02_USE_FLAG->Set__DATA(mode_slot02);
				}
			}
		}
	}

	// LLx IN & OUT : TYPE ...
	bool active__ll_all_in_out = false;

	Get__LLx_ALL_MODE_STATE(active__ll_all_in_out);

	// LLx_IN ...
	{
		CStringArray l_id;
		CStringArray l_mode;
		CStringArray l_slot;
		CString ch_data;

		xSCH_MATERIAL_CTRL->Get__LLx_IN_OF_EDIT_TYPE(l_id, l_mode, l_slot);

		int i_limit = l_id.GetSize();
		int i;

		ch_data.Format("%1d", i_limit);
		sEXT_CH__SCH_START_JOB_LLx_IN_SIZE->Set__DATA(ch_data);

		for(i=0; i<i_limit; i++)
		{
			int ll_id = atoi(l_id[i]);

			CString str_mode = l_mode[i];
			CString str_slot = l_slot[i];

			CString mode_slot01 = STR__DISABLE;
			CString mode_slot02 = STR__DISABLE;

			CString type_slot01 = STR__ALL;
			CString type_slot02 = STR__ALL;

			if(str_slot.CompareNoCase("11") == 0)
			{
				mode_slot01 = STR__ENABLE;
				mode_slot02 = STR__ENABLE;

				type_slot01 = STR__INPUT;
				type_slot02 = STR__OUTPUT;
			}
			else if(str_slot.CompareNoCase("10") == 0)
			{
				if(active__ll_all_in_out)
				{
					mode_slot01 = STR__ENABLE;
					mode_slot02 = STR__ENABLE;

					type_slot01 = STR__INPUT;
					type_slot02 = STR__OUTPUT;
				}
				else
				{
					mode_slot01 = STR__ENABLE;

					type_slot01 = STR__INPUT;
				}
			}
			else if(str_slot.CompareNoCase("01") == 0)
			{
				if(active__ll_all_in_out)
				{
					mode_slot01 = STR__ENABLE;
					mode_slot02 = STR__ENABLE;

					type_slot01 = STR__OUTPUT;
					type_slot02 = STR__INPUT;
				}
				else
				{
					mode_slot02 = STR__ENABLE;

					type_slot02 = STR__INPUT;
				}
			}

			if(ll_id >= 1)
			{
				int ll_i = ll_id - 1;

				pEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_i]->Set__DATA(str_mode);
				
				if(str_mode.CompareNoCase("ALL") == 0)
				{
					pEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_i][0]->Set__DATA(mode_slot01);
					pEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_i][1]->Set__DATA(mode_slot02);

					pEXT_CH__SCH_DB_LLx_SLOT_MODE[ll_i][0]->Set__DATA(type_slot01);
					pEXT_CH__SCH_DB_LLx_SLOT_MODE[ll_i][1]->Set__DATA(type_slot02);
				}
				else
				{
					pEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_i][0]->Set__DATA(mode_slot01);
					pEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_i][1]->Set__DATA(mode_slot02);
				}

				if((mode_slot01.CompareNoCase(STR__ENABLE) == 0)
				|| (mode_slot02.CompareNoCase(STR__ENABLE) == 0))
				{
					if(xEXT_CH__CFG_DB_LLx_USE_FLAG[ll_i]->Check__DATA(STR__DISABLE) < 0)
						xEXT_CH__SCH_DB_LLx_USE_FLAG[ll_i]->Set__DATA(STR__ENABLE);
				}
				else
				{
					xEXT_CH__SCH_DB_LLx_USE_FLAG[ll_i]->Set__DATA(STR__DISABLE);
				}
			}

			// ...
		}
	}
	// LLx_OUT ...
	{
		CStringArray l_id;
		CStringArray l_mode;
		CStringArray l_slot;
		CString ch_data;

		xSCH_MATERIAL_CTRL->Get__LLx_OUT_OF_EDIT_TYPE(l_id, l_mode, l_slot);

		int i_limit = l_id.GetSize();
		int i;

		ch_data.Format("%1d", i_limit);
		sEXT_CH__SCH_START_JOB_LLx_OUT_SIZE->Set__DATA(ch_data);

		for(i=0; i<i_limit; i++)
		{
			int ll_id = atoi(l_id[i]);

			CString str_mode = l_mode[i];
			CString str_slot = l_slot[i];

			CString mode_slot01 = STR__DISABLE;
			CString mode_slot02 = STR__DISABLE;

			CString type_slot01 = STR__ALL;
			CString type_slot02 = STR__ALL;

			if(str_slot.CompareNoCase("11") == 0)
			{
				mode_slot01 = STR__ENABLE;
				mode_slot02 = STR__ENABLE;

				type_slot01 = STR__INPUT;
				type_slot02 = STR__OUTPUT;
			}
			else if(str_slot.CompareNoCase("10") == 0)
			{
				if(active__ll_all_in_out)
				{
					mode_slot01 = STR__ENABLE;
					mode_slot02 = STR__ENABLE;

					type_slot01 = STR__OUTPUT;
					type_slot02 = STR__INPUT;
				}
				else
				{
					mode_slot01 = STR__ENABLE;

					type_slot01 = STR__ALL;
				}
			}
			else if(str_slot.CompareNoCase("01") == 0)
			{
				if(active__ll_all_in_out)
				{
					mode_slot01 = STR__ENABLE;
					mode_slot02 = STR__ENABLE;

					type_slot01 = STR__INPUT;
					type_slot02 = STR__OUTPUT;
				}
				else
				{
					mode_slot02 = STR__ENABLE;

					type_slot02 = STR__ALL;
				}
			}

			if(ll_id >= 1)
			{
				int ll_i = ll_id - 1;

				pEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_i]->Set__DATA(str_mode);

				if(str_mode.CompareNoCase("ALL") == 0)
				{
					pEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_i][0]->Set__DATA(mode_slot01);
					pEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_i][1]->Set__DATA(mode_slot02);

					pEXT_CH__SCH_DB_LLx_SLOT_MODE[ll_i][0]->Set__DATA(type_slot01);
					pEXT_CH__SCH_DB_LLx_SLOT_MODE[ll_i][1]->Set__DATA(type_slot02);
				}
				else
				{
					pEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_i][0]->Set__DATA(mode_slot01);
					pEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_i][1]->Set__DATA(mode_slot02);
				}

				if((mode_slot01.CompareNoCase(STR__ENABLE) == 0)
				|| (mode_slot02.CompareNoCase(STR__ENABLE) == 0))
				{
					if(xEXT_CH__CFG_DB_LLx_USE_FLAG[ll_i]->Check__DATA(STR__DISABLE) < 0)
						xEXT_CH__SCH_DB_LLx_USE_FLAG[ll_i]->Set__DATA(STR__ENABLE);
				}
				else
				{
					xEXT_CH__SCH_DB_LLx_USE_FLAG[ll_i]->Set__DATA(STR__DISABLE);
				}
			}

			// ...
		}
	}

	// STx ...
	{
		// STx : Init ...
		{
			xEXT_CH__SCH_DB_STx_APPLY_MODE->Set__DATA(STR__DISABLE);
			
			sEXT_CH__SCH_DB_ST1_USE_FLAG->Set__DATA(STR__DISABLE);
			sEXT_CH__SCH_DB_ST2_USE_FLAG->Set__DATA(STR__DISABLE);

			xEXT_CH__SCH_DB_STx_TRANSFER_MODE->Set__DATA(STR__AFTER_PROCESS);
		}

		// STx : Check ...
		{
			CStringArray l_id;
			CStringArray l_mode;
			CStringArray l_slot; 
			CStringArray l_sec;

			xSCH_MATERIAL_CTRL->Get__STx_OF_EDIT_TYPE(l_id, l_mode, l_slot, l_sec);

			int i_limit = l_id.GetSize();
			int i;

			for(i=0; i<i_limit; i++)
			{
				int x_id = atoi(l_id[i]);

				// ...
				int x_index = x_id - 1;

				if((x_index < 0)
				|| (x_index >= CFG_BUFFER__UNIT_LIMIT))
				{
					continue;
				}
				if(xEXT_CH__CFG_DB_STx_USE_FLAG[x_index]->Check__DATA(STR__ENABLE) < 0)
				{
					continue;
				}

				// ...
				CString str_mode = l_mode[i];
				CString str_slot = l_slot[i];
				CString str_sec  = l_sec[i];

				if(x_id == 1)
				{
					sEXT_CH__SCH_DB_ST1_USE_FLAG->Set__DATA(STR__ENABLE);

					dEXT_CH__SCH_DB_ST1_OUT_MODE_BUFFER_TO_LPo->Set__DATA(str_mode);
					xEXT_CH__SCH_DB_ST1_WAFER_PICK_MODE->Set__DATA(str_slot);
					xEXT_CH__SCH_DB_ST1_WAFER_CLEAN_TIME->Set__DATA(str_sec);
				}
				else if(x_id == 2)
				{
					sEXT_CH__SCH_DB_ST2_USE_FLAG->Set__DATA(STR__ENABLE);

					dEXT_CH__SCH_DB_ST2_OUT_MODE_BUFFER_TO_LPo->Set__DATA(str_mode);
					xEXT_CH__SCH_DB_ST2_WAFER_PICK_MODE->Set__DATA(str_slot);
					xEXT_CH__SCH_DB_ST2_WAFER_CLEAN_TIME->Set__DATA(str_sec);
				}
			}

			if((sEXT_CH__SCH_DB_ST1_USE_FLAG->Check__DATA(STR__ENABLE) > 0)
			|| (sEXT_CH__SCH_DB_ST2_USE_FLAG->Check__DATA(STR__ENABLE) > 0))
			{
				xEXT_CH__SCH_DB_STx_APPLY_MODE->Set__DATA(STR__ROUTE);
			}

			// ...
			{
				CString log_msg;
				CString log_bff;

				log_msg.Format("_Update__ROUTE_DB_TO_SCH_DB(%1d) ... \n", lp_id);
				log_msg += "* STx Info ... \n";

				i_limit = l_id.GetSize();

				for(i=0; i<i_limit; i++)
				{
					CString id_name = l_id[i];
					int x_id = atoi(id_name);
					CString str_mode = l_mode[i];
					CString str_slot = l_slot[i];
					CString str_sec  = l_sec[i];

					log_bff.Format(" %1d) ID <- %s(%1d) \n", i,id_name,x_id);
					log_msg += log_bff;

					log_bff.Format("  ** Mode <- %s \n", str_mode);
					log_msg += log_bff;

					log_bff.Format("  ** Slot <- %s \n", str_slot);
					log_msg += log_bff;

					log_bff.Format("  ** Sec  <- %s \n", str_sec);
					log_msg += log_bff;
				}

				xLOG_CTRL->WRITE__LOG(log_msg);
			}
		}
	}

	return 1;
}

int  CObj_Opr__AUTO_MODE::
Update__ADD_LLx_DB_TO_SCH_DB(SCI__CTC__JOB_FILE_CTRL *p_job_file,
							 const int slot_id)
{
	int active__ll_in  = -1;
	int active__ll_out = -1;

	CString ll_in__mode_type  = "";
	CString ll_out__mode_type = "";

	CString ll_in__slot_type  = "";
	CString ll_out__slot_type = "";

	// LLx : Add ...
	{
		int kk_limit = 2;
		int kk_i;

		for(kk_i=0; kk_i<kk_limit; kk_i++)
		{
			CStringArray l_id;
			CStringArray l_mode;
			CStringArray l_slot;

			int active__in_check  = -1;
			int active__out_check = -1;

			if(kk_i == 0)				// LLx_IN ...
			{
				active__in_check = 1;
				p_job_file->Get__LLx_IN_OF_EDIT_TYPE(slot_id, l_id,l_mode,l_slot);
			}
			else if(kk_i == 1)			// LLx_OUT ...
			{
				active__out_check = 1;
				p_job_file->Get__LLx_OUT_OF_EDIT_TYPE(slot_id, l_id,l_mode,l_slot);
			}

			// ...
			int id_limit = l_id.GetSize();
			int index_ll;

			for(index_ll = 0; index_ll < id_limit; index_ll++)
			{
				if(index_ll >= 2)
				{
					continue;
				}

				int ll_id = atoi(l_id[index_ll]);
				CString str_mode = l_mode[index_ll];
				CString str_slot = l_slot[index_ll];

				// ...
				int ll_i = ll_id - 1;

				if((ll_i >= 0) && (ll_i < iLLx_SIZE))
				{
					if(xEXT_CH__SCH_DB_LLx_USE_FLAG[ll_i]->Check__DATA(STR__DISABLE) > 0)
					{
						if(active__in_check > 0)
						{
							active__ll_in  = ll_id;
							
							ll_in__mode_type = str_mode;
							ll_in__slot_type = str_slot;
						}
						else if(active__out_check > 0)
						{
							active__ll_out = ll_id;

							ll_out__mode_type = str_mode;
							ll_out__slot_type = str_slot;
						}
					}
				}
			}

			// ...
		}
	}

	// ...
	{
		int kk_limit = 2;
		int kk_i;

		if(active__ll_in == active__ll_out)
		{
			kk_limit = 1;
		}

		for(kk_i=0; kk_i<kk_limit; kk_i++)
		{
			int ll_i = -1;
			CString ll__mode_type;
			CString ll__slot_type;

			if(kk_i == 0)				// LLx_IN ...
			{
				if(active__ll_in < 1)			continue;

				ll_i = active__ll_in - 1;

				ll__mode_type = ll_in__mode_type;
				ll__slot_type = ll_in__slot_type;
			}
			else if(kk_i == 1)			// LLx_OUT ...
			{
				if(active__ll_out < 1)			continue;

				ll_i = active__ll_out - 1;

				ll__mode_type = ll_out__mode_type;
				ll__slot_type = ll_out__slot_type;
			}

			if(ll_i < 0)
			{
				continue;
			}

			// ...
			{
				pEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_i]->Set__DATA(ll__mode_type);

				if(ll__slot_type.CompareNoCase("11") == 0)
				{
					if(ll__mode_type.CompareNoCase("ALL") == 0)
					{
						pEXT_CH__SCH_DB_LLx_SLOT_MODE[ll_i][0]->Set__DATA("OUTPUT");
						pEXT_CH__SCH_DB_LLx_SLOT_MODE[ll_i][1]->Set__DATA("INPUT");
					}
					else
					{
						pEXT_CH__SCH_DB_LLx_SLOT_MODE[ll_i][0]->Set__DATA("ALL");
						pEXT_CH__SCH_DB_LLx_SLOT_MODE[ll_i][1]->Set__DATA("ALL");
					}

					pEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_i][0]->Set__DATA(STR__ENABLE);
					pEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_i][1]->Set__DATA(STR__ENABLE);

					if(xEXT_CH__CFG_DB_LLx_USE_FLAG[ll_i]->Check__DATA(STR__DISABLE) < 0)
						xEXT_CH__SCH_DB_LLx_USE_FLAG[ll_i]->Set__DATA(STR__ENABLE);
				}
				else if(ll__slot_type.CompareNoCase("10") == 0)
				{
					pEXT_CH__SCH_DB_LLx_SLOT_MODE[ll_i][0]->Set__DATA("ALL");
					pEXT_CH__SCH_DB_LLx_SLOT_MODE[ll_i][1]->Set__DATA("ALL");

					pEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_i][0]->Set__DATA(STR__ENABLE);
					pEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_i][1]->Set__DATA(STR__DISABLE);

					if(xEXT_CH__CFG_DB_LLx_USE_FLAG[ll_i]->Check__DATA(STR__DISABLE) < 0)
						xEXT_CH__SCH_DB_LLx_USE_FLAG[ll_i]->Set__DATA(STR__ENABLE);
				}
				else if(ll__slot_type.CompareNoCase("01") == 0)
				{
					pEXT_CH__SCH_DB_LLx_SLOT_MODE[ll_i][0]->Set__DATA("ALL");
					pEXT_CH__SCH_DB_LLx_SLOT_MODE[ll_i][1]->Set__DATA("ALL");

					pEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_i][0]->Set__DATA(STR__DISABLE);
					pEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_i][1]->Set__DATA(STR__ENABLE);

					if(xEXT_CH__CFG_DB_LLx_USE_FLAG[ll_i]->Check__DATA(STR__DISABLE) < 0)
						xEXT_CH__SCH_DB_LLx_USE_FLAG[ll_i]->Set__DATA(STR__ENABLE);
				}
			}
			
			// ...
		}
	}

	// STx : Add ...
	{
		CStringArray l_id;
		CStringArray l_mode;
		CStringArray l_slot; 
		CStringArray l_sec;

		p_job_file->Get__STx_OF_EDIT_TYPE(slot_id, l_id, l_mode, l_slot, l_sec);

		int i_limit = l_id.GetSize();
		int i;

		for(i=0; i<i_limit; i++)
		{
			int st_id = atoi(l_id[i]);

			// ...
			int st_index = st_id - 1;

			if((st_index < 0)
			|| (st_index >= CFG_BUFFER__UNIT_LIMIT))
			{
				continue;
			}
			if(xEXT_CH__CFG_DB_STx_USE_FLAG[st_index]->Check__DATA(STR__ENABLE) < 0)
			{
				continue;
			}

			// ...
			CString str_mode = l_mode[i];
			CString str_slot = l_slot[i];
			CString str_sec  = l_sec[i];

			if(st_id == 1)
			{
				sEXT_CH__SCH_DB_ST1_USE_FLAG->Set__DATA(STR__ENABLE);

				dEXT_CH__SCH_DB_ST1_OUT_MODE_BUFFER_TO_LPo->Set__DATA(str_mode);
				xEXT_CH__SCH_DB_ST1_WAFER_PICK_MODE->Set__DATA(str_slot);
				xEXT_CH__SCH_DB_ST1_WAFER_CLEAN_TIME->Set__DATA(str_sec);
			}
			else if(st_id == 2)
			{
				sEXT_CH__SCH_DB_ST2_USE_FLAG->Set__DATA(STR__ENABLE);

				dEXT_CH__SCH_DB_ST2_OUT_MODE_BUFFER_TO_LPo->Set__DATA(str_mode);
				xEXT_CH__SCH_DB_ST2_WAFER_PICK_MODE->Set__DATA(str_slot);
				xEXT_CH__SCH_DB_ST2_WAFER_CLEAN_TIME->Set__DATA(str_sec);
			}
		}

		if((sEXT_CH__SCH_DB_ST1_USE_FLAG->Check__DATA(STR__ENABLE) > 0)
		|| (sEXT_CH__SCH_DB_ST2_USE_FLAG->Check__DATA(STR__ENABLE) > 0))
		{
			xEXT_CH__SCH_DB_STx_APPLY_MODE->Set__DATA(STR__ROUTE);
		}
	}

	return 1;
}
int  CObj_Opr__AUTO_MODE::
Check__ADD_LLx_IN_OF_SCH_DB(SCI__CTC__JOB_FILE_CTRL *p_job_file,
							const int slot_id)
{
	CStringArray l_id;
	CStringArray l_mode;
	CStringArray l_slot;

	p_job_file->Get__LLx_IN_OF_EDIT_TYPE(slot_id, l_id,l_mode,l_slot);

	return _Check__ADD_LLx_OF_SCH_DB(1,-1, l_id,l_mode,l_slot);
}
int  CObj_Opr__AUTO_MODE::
Check__ADD_LLx_OUT_OF_SCH_DB(SCI__CTC__JOB_FILE_CTRL *p_job_file,
							 const int slot_id)
{
	CStringArray l_id;
	CStringArray l_mode;
	CStringArray l_slot;

	p_job_file->Get__LLx_OUT_OF_EDIT_TYPE(slot_id, l_id,l_mode,l_slot);

	return _Check__ADD_LLx_OF_SCH_DB(-1,1, l_id,l_mode,l_slot);
}
int  CObj_Opr__AUTO_MODE::
_Check__ADD_LLx_OF_SCH_DB(const int ll_in_check,
						  const int ll_out_check,
						  const CStringArray& l_id,
						  const CStringArray& l_mode,
						  const CStringArray& l_slot)
{
	int i_limit = l_id.GetSize();

	// LLx_IN : Size Check ...
	if(ll_in_check > 0)
	{
		CString ch_data = sEXT_CH__SCH_START_JOB_LLx_IN_SIZE->Get__STRING();
		int start_size = atoi(ch_data);
		
		if(start_size != i_limit)		return -11;
	}
	// LLx_OUT : Size Check ...
	if(ll_out_check > 0)
	{
		CString ch_data = sEXT_CH__SCH_START_JOB_LLx_OUT_SIZE->Get__STRING();
		int start_size = atoi(ch_data);

		if(start_size != i_limit)		return -12;
	}

	if(i_limit == 1)
	{		
		int index_ll = 0;

		int ll_id = atoi(l_id[index_ll]);
		CString str_mode = l_mode[index_ll];
		CString str_slot = l_slot[index_ll];
		
		// ...
		int ll_i = ll_id - 1;

		if(xEXT_CH__SCH_DB_LLx_USE_FLAG[ll_i]->Check__DATA(STR__DISABLE) > 0)
		{
			// return 1;
		}
		else
		{
			if(str_mode.CompareNoCase("ALL") == 0)
			{
				if(str_slot.CompareNoCase("11") == 0)
				{
					if((pEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_i]->Check__DATA(str_mode)    > 0)
					&& (pEXT_CH__SCH_DB_LLx_SLOT_MODE[ll_i][0]->Check__DATA("OUTPUT") > 0)
					&& (pEXT_CH__SCH_DB_LLx_SLOT_MODE[ll_i][1]->Check__DATA("INPUT")  > 0))
					{
						return 1;
					}
				}
				else if(str_slot.CompareNoCase("10") == 0)
				{
					if((pEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_i]->Check__DATA(str_mode) > 0)
					&& (pEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_i][0]->Check__DATA(STR__ENABLE)  > 0)
					&& (pEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_i][1]->Check__DATA(STR__DISABLE) > 0)
					&& (pEXT_CH__SCH_DB_LLx_SLOT_MODE[ll_i][0]->Check__DATA("ALL") > 0))
					{
						return 1;
					}
				}
				else if(str_slot.CompareNoCase("01") == 0)
				{
					if((pEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_i]->Check__DATA(str_mode) > 0)
					&& (pEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_i][0]->Check__DATA(STR__DISABLE) > 0)
					&& (pEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_i][1]->Check__DATA(STR__ENABLE)  > 0)
					&& (pEXT_CH__SCH_DB_LLx_SLOT_MODE[ll_i][1]->Check__DATA("ALL") > 0))
					{
						return 1;
					}
				}
			}
		}

		// ...
	}

	return -1;
}

int  CObj_Opr__AUTO_MODE::
Check__ALx_OF_SCH_DB(const CStringArray& l_id,
					 const CStringArray& l_mode,
					 const CStringArray& l_slot)
{
	int i_limit = l_id.GetSize();
	int i;

	for(i=0; i<i_limit; i++)
	{
		int al_id = atoi(l_id[i]);

		CString str_mode = l_mode[i];
		CString str_slot = l_slot[i];

		if(al_id == 1)
		{
			CString mode_slot01 = STR__DISABLE;
			CString mode_slot02 = STR__DISABLE;

			if(str_slot.CompareNoCase("11") == 0)
			{
				mode_slot01 = STR__ENABLE;
				mode_slot02 = STR__ENABLE;
			}
			else if(str_slot.CompareNoCase("10") == 0)
			{
				mode_slot01 = STR__ENABLE;
			}
			else if(str_slot.CompareNoCase("01") == 0)
			{
				mode_slot02 = STR__ENABLE;
			}
			else
			{
				if(xEXT_CH__SCH_DB_AL1_USE_FLAG->Check__DATA(STR__DISABLE) < 0)
				{
					return -11;
				}
			}

			if(xEXT_CH__SCH_DB_AL1_USE_FLAG->Check__DATA(STR__ENABLE) < 0)
			{
				return -12;
			}

			// ...
			int err_check = 1;

			if(xEXT_CH__SCH_DB_AL1_SLOT01_USE_FLAG->Check__DATA(mode_slot01) > 0)
			{
				err_check = -1;
			}
			if(xEXT_CH__SCH_DB_AL1_SLOT02_USE_FLAG->Check__DATA(mode_slot02) < 0)
			{
				err_check = -1;
			}

			if(err_check > 0)
			{
				return -21;
			}
		}
	}

	return 1;
}
int  CObj_Opr__AUTO_MODE::
Check__LLx_IN_OF_SCH_DB(const CStringArray& l_id,
						const CStringArray& l_mode,
						const CStringArray& l_slot)
{
	int i_limit = l_id.GetSize();
	int i;

	// LLx_IN : Size Check ...
	{
		CString ch_data = sEXT_CH__SCH_START_JOB_LLx_IN_SIZE->Get__STRING();
		int start_size = atoi(ch_data);

		if(start_size != i_limit)		return -11;
	}

	for(i=0; i<i_limit; i++)
	{
		int ll_id = atoi(l_id[i]);

		CString str_mode = l_mode[i];
		CString str_slot = l_slot[i];

		CString mode_slot01 = STR__DISABLE;
		CString mode_slot02 = STR__DISABLE;

		if(str_slot.CompareNoCase("11") == 0)
		{
			mode_slot01 = STR__ENABLE;
			mode_slot02 = STR__ENABLE;
		}
		else if(str_slot.CompareNoCase("10") == 0)
		{
			mode_slot01 = STR__ENABLE;
		}
		else if(str_slot.CompareNoCase("01") == 0)
		{
			mode_slot02 = STR__ENABLE;
		}

		// ...
		int ll_lba = _LLx_INDEX__LBA;
		int ll_lbb = _LLx_INDEX__LBB;

		if(ll_id == 1)
		{
			// LBA : Ref ...
			if(xEXT_CH__SCH_DB_LLx_USE_FLAG[ll_lba]->Check__DATA(STR__ENABLE) < 0)
			{
				return -1;
			}
			if(pEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_lba]->Check__DATA(str_mode) < 0)
			{
				return -101;
			}

			// LBA : Check ...
			if(str_mode.CompareNoCase("ALL") == 0)
			{
				if(str_slot.CompareNoCase("11") == 0)
				{
					if(pEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_lba][0]->Check__DATA(mode_slot01) < 0)
					{
						return -111;
					}
					if(pEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_lba][1]->Check__DATA(mode_slot02) < 0)
					{
						return -112;
					}

					if(pEXT_CH__SCH_DB_LLx_SLOT_MODE[ll_lba][0]->Check__DATA("OUTPUT") < 0)
					{
						return -113;
					}
					if(pEXT_CH__SCH_DB_LLx_SLOT_MODE[ll_lba][1]->Check__DATA("INPUT") < 0)
					{
						return -114;
					}
				}
				else
				{
					if(pEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_lba][0]->Check__DATA(mode_slot01) < 0)
					{
						return -121;
					}
					if(pEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_lba][1]->Check__DATA(mode_slot02) < 0)
					{
						return -122;
					}

					if(pEXT_CH__SCH_DB_LLx_SLOT_MODE[ll_lba][0]->Check__DATA("ALL") < 0)
					{
						return -123;
					}
					if(pEXT_CH__SCH_DB_LLx_SLOT_MODE[ll_lba][1]->Check__DATA("ALL") < 0)
					{
						return -124;
					}
				}
			}
			else
			{
				// LBB : Check ...
				if(pEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_lbb][0]->Check__DATA(mode_slot01) < 0)
				{
					return -131;
				}
				if(pEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_lbb][1]->Check__DATA(mode_slot02) < 0)
				{
					return -132;
				}
			}

			// LBA : Check ...
			if((mode_slot01.CompareNoCase(STR__ENABLE) == 0)
			|| (mode_slot02.CompareNoCase(STR__ENABLE) == 0))
			{
				if(xEXT_CH__SCH_DB_LLx_USE_FLAG[ll_lba]->Check__DATA(STR__ENABLE) < 0)
				{
					return -141;
				}
			}
			else
			{
				if(xEXT_CH__SCH_DB_LLx_USE_FLAG[ll_lba]->Check__DATA(STR__DISABLE) < 0)
				{
					return -142;
				}
			}
		}
		else if(ll_id == 2)
		{
			// LBB : Ref ...
			if(xEXT_CH__SCH_DB_LLx_USE_FLAG[ll_lbb]->Check__DATA(STR__ENABLE) < 0)
			{
				return -1;
			}
			if(pEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_lbb]->Check__DATA(str_mode) < 0)
			{
				return -201;
			}

			// LBB : Check ...
			if(str_mode.CompareNoCase("ALL") == 0)
			{
				if(str_slot.CompareNoCase("11") == 0)
				{
					if(pEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_lbb][0]->Check__DATA(mode_slot01) < 0)
					{
						return -211;
					}
					if(pEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_lbb][1]->Check__DATA(mode_slot02) < 0)
					{
						return -212;
					}

					if(pEXT_CH__SCH_DB_LLx_SLOT_MODE[ll_lbb][0]->Check__DATA("OUTPUT") < 0)
					{
						return -213;
					}
					if(pEXT_CH__SCH_DB_LLx_SLOT_MODE[ll_lbb][1]->Check__DATA("INPUT") < 0)
					{
						return -214;
					}
				}
				else
				{
					if(pEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_lbb][0]->Check__DATA(mode_slot01) < 0)
					{
						return -221;
					}
					if(pEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_lbb][1]->Check__DATA(mode_slot02) < 0)
					{
						return -222;
					}

					if(pEXT_CH__SCH_DB_LLx_SLOT_MODE[ll_lbb][0]->Check__DATA("ALL") < 0)
					{
						return -223;
					}
					if(pEXT_CH__SCH_DB_LLx_SLOT_MODE[ll_lbb][1]->Check__DATA("ALL") < 0)
					{
						return -224;
					}
				}
			}
			else
			{
				// LBA : Check ...
				if(pEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_lba][0]->Check__DATA(mode_slot01) < 0)
				{
					return -231;
				}
				if(pEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_lba][1]->Check__DATA(mode_slot02) < 0)
				{
					return -232;
				}
			}

			// LBB : Check ...
			if((mode_slot01.CompareNoCase(STR__ENABLE) == 0)
			|| (mode_slot02.CompareNoCase(STR__ENABLE) == 0))
			{
				if(xEXT_CH__SCH_DB_LLx_USE_FLAG[ll_lbb]->Check__DATA(STR__ENABLE) < 0)
				{
					return -241;
				}
			}
			else
			{
				if(xEXT_CH__SCH_DB_LLx_USE_FLAG[ll_lbb]->Check__DATA(STR__DISABLE) < 0)
				{
					return -242;
				}
			}
		}

		// ...
	}

	return 1;
}

int  CObj_Opr__AUTO_MODE::
Check__LLx_OUT_OF_SCH_DB(const CStringArray& l_id,
						 const CStringArray& l_mode,
						 const CStringArray& l_slot)
{
	int i_limit = l_id.GetSize();
	int i;

	// LLx_OUT : Size Check ...
	{
		CString ch_data = sEXT_CH__SCH_START_JOB_LLx_OUT_SIZE->Get__STRING();
		int start_size = atoi(ch_data);

		if(start_size != i_limit)		return -11;
	}

	for(i=0; i<i_limit; i++)
	{
		int ll_id = atoi(l_id[i]);

		CString str_mode = l_mode[i];
		CString str_slot = l_slot[i];

		CString mode_slot01 = STR__DISABLE;
		CString mode_slot02 = STR__DISABLE;

		if(str_slot.CompareNoCase("11") == 0)
		{
			mode_slot01 = STR__ENABLE;
			mode_slot02 = STR__ENABLE;
		}
		else if(str_slot.CompareNoCase("10") == 0)
		{
			mode_slot01 = STR__ENABLE;
		}
		else if(str_slot.CompareNoCase("01") == 0)
		{
			mode_slot02 = STR__ENABLE;
		}

		// ...
		{
			int ll_i = ll_id - 1;

			if(xEXT_CH__CFG_DB_LLx_USE_FLAG[ll_i]->Check__DATA(STR__DISABLE) > 0)
			{
				continue;
			}

			if(xEXT_CH__SCH_DB_LLx_USE_FLAG[ll_i]->Check__DATA(STR__ENABLE) < 0)
			{
				return -100;
			}
			if(pEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_i]->Check__DATA(str_mode) < 0)
			{
				return -101;
			}

			if(str_mode.CompareNoCase("ALL") == 0)
			{
				if(str_slot.CompareNoCase("11") == 0)
				{
					if(pEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_i][0]->Check__DATA(mode_slot01) < 0)
					{
						return -111;
					}
					if(pEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_i][1]->Check__DATA(mode_slot02) < 0)
					{
						return -112;
					}

					if(pEXT_CH__SCH_DB_LLx_SLOT_MODE[ll_i][0]->Check__DATA("OUTPUT") < 0)
					{
						return -113;
					}
					if(pEXT_CH__SCH_DB_LLx_SLOT_MODE[ll_i][1]->Check__DATA("INPUT") < 0)
					{
						return -114;
					}
				}
				else
				{
					if(pEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_i][0]->Check__DATA(mode_slot01) < 0)
					{
						return -121;
					}
					if(pEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_i][1]->Check__DATA(mode_slot02) < 0)
					{
						return -122;
					}

					if(pEXT_CH__SCH_DB_LLx_SLOT_MODE[ll_i][0]->Check__DATA("ALL") < 0)
					{
						return -123;
					}
					if(pEXT_CH__SCH_DB_LLx_SLOT_MODE[ll_i][1]->Check__DATA("ALL") < 0)
					{
						return -124;
					}
				}
			}
			else
			{
				if(pEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_i][0]->Check__DATA(mode_slot01) < 0)
				{
					return -131;
				}
				if(pEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_i][1]->Check__DATA(mode_slot02) < 0)
				{
					return -132;
				}
			}

			if((mode_slot01.CompareNoCase(STR__ENABLE) == 0)
			|| (mode_slot02.CompareNoCase(STR__ENABLE) == 0))
			{
				if(xEXT_CH__SCH_DB_LLx_USE_FLAG[ll_i]->Check__DATA(STR__ENABLE) < 0)
				{
					return -141;
				}
			}
			else
			{
				if(xEXT_CH__SCH_DB_LLx_USE_FLAG[ll_i]->Check__DATA(STR__DISABLE) < 0)
				{
					return -142;
				}
			}
		}

		// ...
	}

	return 1;
}

int  CObj_Opr__AUTO_MODE::
Check__STx_OF_SCH_DB(const CStringArray& l_id,
					 const CStringArray& l_mode,
					 const CStringArray& l_slot,
					 const CStringArray& l_sec)
{
	int i_limit = l_id.GetSize();
	int i;

	if(i_limit < 1)
	{
		return 1;
	}

	for(i=0; i<i_limit; i++)
	{
		int st_id = atoi(l_id[i]);

		CString str_mode = l_mode[i];
		CString str_slot = l_slot[i];
		CString str_sec  = l_sec[i];

		// ...
		int st_index = st_id - 1;

		if(st_index <  0)					continue;
		if(st_index >= iBUFF_UNIT_SIZE)		continue;

		CString cfg__use_mode = xEXT_CH__CFG_DB_STx_USE_FLAG[st_index]->Get__STRING();
		if(cfg__use_mode.CompareNoCase(STR__ENABLE) != 0)
		{
			return -(st_id*10 + 1);
		}

		if(st_id == 1)
		{
			if(dEXT_CH__SCH_DB_ST1_OUT_MODE_BUFFER_TO_LPo->Check__DATA(str_mode) < 0)
			{
				return -12;
			}
			if(xEXT_CH__SCH_DB_ST1_WAFER_PICK_MODE->Check__DATA(str_slot) < 0)
			{
				return -13;
			}

			/*
			// ...
			{
				int new_sec = atoi(str_sec);
				int cmp_sec = (int) xEXT_CH__SCH_DB_ST1_WAFER_CLEAN_TIME->Get__VALUE();
			
				if(new_sec != cmp_sec)
				{
					return -14;
				}
			}
			*/
		}
		else if(st_id == 2)
		{
			if(dEXT_CH__SCH_DB_ST2_OUT_MODE_BUFFER_TO_LPo->Check__DATA(str_mode) < 0)
			{
				return -22;
			}
			if(xEXT_CH__SCH_DB_ST2_WAFER_PICK_MODE->Check__DATA(str_slot) < 0)
			{
				return -23;
			}

			/*
			// ...
			{
				int new_sec = atoi(str_sec);
				int cmp_sec = (int) xEXT_CH__SCH_DB_ST2_WAFER_CLEAN_TIME->Get__VALUE();

				if(new_sec != cmp_sec)
				{
					return -24;
				}
			}
			*/
		}
	}

	return 1;
}
