#include "StdAfx.h"
#include "CObj__TMC_SYS_EX.h"
#include "CObj__TMC_SYS_EX__DEF.h"


// ...
int  CObj__TMC_SYS_EX::Call__SYSTEM_INIT(CII_OBJECT__VARIABLE* p_variable)
{
	CString log_msg;

	int n_count = 0;
	int n_index = 0;

	int r_ret = -1;
	int i;

	// Init Message ...
	{
		for (i=0; i<INIT_MSG_MAX; i++)
		{
			sCH__INIT_SEQ_MSG_X[i]->Set__DATA("");
		}

		sEXT_CH__GUI_POPUP_INIT_SEQ_MSG->Set__DATA("TMC_POPUP_INITIAL");
	}

	// INIT OBJ ...
	{
		CString ch_data;

		for(i=0; i<iSIZE__INIT_OBJ; i++)
		{
			n_index = n_count++;

			CString str_msg = sCH__INIT_MSG_X[i]->Get__STRING();
			ch_data.Format("%s : Initial...", str_msg);
			sCH__INIT_SEQ_MSG_X[n_index]->Set__DATA(ch_data);
		
			log_msg.Format("%02d) %s : Initial ...", n_count,str_msg);
			xLOG_CTRL->WRITE__LOG(log_msg);

			r_ret = pOBJ_CTRL__INIT_X[i]->Call__OBJECT(STR__INIT);
			if(r_ret < 0)
			{
				ch_data.Format("%s : Init Failed...", str_msg);
				sCH__INIT_SEQ_MSG_X[n_index]->Set__DATA(ch_data);
				return -1;
			}

			if(p_variable->Check__CTRL_ABORT() > 0)
			{
				return -1;
			}
		}
	}

	// LPx ...
	{
		CString obj_sts;

		for(i=0; i<iLPx_LIMIT; i++)
		{
			pLPx__OBJ_CTRL[i]->Get__OBJECT_STATUS(obj_sts);

			if((obj_sts.CompareNoCase(STR__AVAILABLE) != 0)
			&& (obj_sts.CompareNoCase(STR__ABORTED)   != 0)
			&& (obj_sts.CompareNoCase(STR__IDLE)      != 0))
			{
				dEXT_CH__LPx_OBJ_STATUS[i]->Set__DATA(STR__STANDBY);
				continue;
			}

			CString sz_msg_temp;
			sz_msg_temp.Format("LP%d : Initialize...", i+1);

			n_index = n_count++;
			sCH__INIT_SEQ_MSG_X[n_index]->Set__DATA(sz_msg_temp);

			log_msg.Format("%02d) LP%1d : Initial ...", n_count,i+1);
			xLOG_CTRL->WRITE__LOG(log_msg);

			r_ret = pLPx__OBJ_CTRL[i]->Run__OBJECT(STR__INIT);
			if(r_ret < 0)
			{
				sz_msg_temp.Format("LP%d : Init Failed...", i+1);
				sCH__INIT_SEQ_MSG_X[n_index]->Set__DATA(sz_msg_temp);
				return -1;
			}

			if(p_variable->Check__CTRL_ABORT() > 0)
			{
				return -1;
			}
		}
	}

	// PMx SV CLOSE ...
	{
		CString str_set;

		for(i=0; i<m_nPM_SIZE; i++)
		{
			int id = i+1;

			str_set.Format("%d", id);
			aCH__PARA_PMC_ID->Set__DATA(str_set);

			CString sz_log_temp;
			sz_log_temp.Format("PM%d SLOT VALVE : Closing ...", id);

			n_index = n_count++;
			sCH__INIT_SEQ_MSG_X[n_index]->Set__DATA(sz_log_temp);

			log_msg.Format("%02d) PM%1d : Initial ...", n_count,id);
			xLOG_CTRL->WRITE__LOG(log_msg);

			r_ret = pPMx_SV__OBJ_CTRL->Call__OBJECT(STR_PMx_INIT_SV_CLOSE);
			if(r_ret < 0)
			{
				sz_log_temp.Format("PM%d SLOT VALVE : Closing Failed...", id);
				sCH__INIT_SEQ_MSG_X[n_index]->Set__DATA(sz_log_temp);
				return -1;
			}

			if(p_variable->Check__CTRL_ABORT() > 0)
			{
				return -1;
			}
		}
	}

	sEXT_CH__GUI_POPUP_INIT_SEQ_MSG->Set__DATA("Close");
	return 1;
}

int  CObj__TMC_SYS_EX::Call__SYSTEM_MAINT(CII_OBJECT__VARIABLE* p_variable)
{
	int i;

	// INIT OBJ ...
	{
		for(i=0; i<iSIZE__INIT_OBJ; i++)
		{
			pOBJ_CTRL__INIT_X[i]->Call__OBJECT(STR__MAINT);

			if(p_variable->Check__CTRL_ABORT() > 0)			return -1;
		}
	}

	// LPx ...
	{
		for(i=0;i<iLPx_LIMIT;i++)
		{
			dEXT_CH__LPx_OBJ_STATUS[i]->Set__DATA(STR__MAINTMODE);

			if(p_variable->Check__CTRL_ABORT() > 0)			return -1;
		}
	}

	return 1;
}
