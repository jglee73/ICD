#include "StdAfx.h"
#include "CObj__TMC_SYS_STD.h"
#include "CObj__TMC_SYS_STD__DEF.h"


// ...
int  CObj__TMC_SYS_STD::Call__SYSTEM_INIT(CII_OBJECT__VARIABLE* p_variable)
{
	CString obj_sts;
	int n_msg_cnt = 0;
	int nRet = -1;
	int nIndex;
	int i;

	// 초기화 한다!!
	for (i=0; i<INIT_MSG_MAX; i++)
	{
		sCH__INIT_SEQ_MSG[i]->Set__DATA("");
	}

	// SEQ_POPUP을 띄운다..!!
	sEXT_CH__GUI_POPUP_INIT_SEQ_MSG->Set__DATA("TMC_POPUP_INITIAL");

	// TMC PUMP -----
	{
		nIndex = n_msg_cnt++;

		sCH__INIT_SEQ_MSG[nIndex]->Set__DATA("TMC PUMP : Initialize... Starting");
		
		nRet = pOBJ_CTRL__TMC_PUMP->Run__OBJECT(STR__INIT);
		if(nRet < 0)
		{
			sCH__INIT_SEQ_MSG[nIndex]->Set__DATA("TMC PUMP : Init Failed...");
			return -1;
		}

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -1;
		}
	}
	// LLx PUMP -----
	{
		nIndex = n_msg_cnt++;

		sCH__INIT_SEQ_MSG[nIndex]->Set__DATA("LLx PUMP : Initialize... Starting");
		
		nRet = pOBJ_CTRL__LLx_PUMP->Run__OBJECT(STR__INIT);
		if(nRet < 0)
		{
			sCH__INIT_SEQ_MSG[nIndex]->Set__DATA("LLx PUMP : Init Failed...");
			return -1;
		}

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -1;
		}
	}

	// ATM_ROBOT -----
	{
		nIndex = n_msg_cnt++;

		sCH__INIT_SEQ_MSG[nIndex]->Set__DATA("ATM ROBOT : Initialize...");

		nRet = pATM_ROBOT__OBJ_CTRL->Call__OBJECT(STR__INIT);
		if(nRet < 0)
		{
			sCH__INIT_SEQ_MSG[nIndex]->Set__DATA("ATM ROBOT : Init Failed...");
			return -1;
		}

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -1;
		}
	}

	// AL1 -----
	{
		nIndex = n_msg_cnt++;

		sCH__INIT_SEQ_MSG[nIndex]->Set__DATA("ATM ALIGNER : Initialize...");
		
		nRet = pAL1__OBJ_CTRL->Call__OBJECT(STR__INIT);
		if(nRet < 0)
		{
			sCH__INIT_SEQ_MSG[nIndex]->Set__DATA("ATM ALIGNER : Init Failed...");
			return -1;
		}

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -1;
		}
	}

	// LPx -----
	for(i=0;i<iLPx_LIMIT;i++)
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

		nIndex = n_msg_cnt++;
		sCH__INIT_SEQ_MSG[nIndex]->Set__DATA(sz_msg_temp);

		nRet = pLPx__OBJ_CTRL[i]->Run__OBJECT(STR__INIT);
		if(nRet < 0)
		{
			sz_msg_temp.Format("LP%d : Init Failed...", i+1);
			sCH__INIT_SEQ_MSG[nIndex]->Set__DATA(sz_msg_temp);
			return -1;
		}

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -1;
		}
	}

	// VAC_ROBOT -----
	{
		nIndex = n_msg_cnt++;

		sCH__INIT_SEQ_MSG[nIndex]->Set__DATA("VAC ROBOT : Initialize...");
		
		nRet = pVAC_ROBOT__OBJ_CTRL->Call__OBJECT(STR__INIT);
		if(nRet < 0)
		{
			sCH__INIT_SEQ_MSG[nIndex]->Set__DATA("VAC ROBOT : Init Failed ...");
			return -1;
		}

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -1;
		}
	}

	//------------------------------------------------------------------
	// LBA -------------------------------------------------------------
	// LBA DV_CLOSE-----
	{
		nIndex = n_msg_cnt++;

		sCH__INIT_SEQ_MSG[nIndex]->Set__DATA("LBA DOOR VALVE: Closing...");
		
		nRet = pLBA__OBJ_CTRL->Call__OBJECT(STR_LBx_DV_CLOSE);
		if(nRet < 0)
		{
			sCH__INIT_SEQ_MSG[nIndex]->Set__DATA("LBA DOOR VALVE: Closing Failed ...");
			return -1;
		}

		if(p_variable->Check__CTRL_ABORT() > 0)	
		{
			return -1;
		}
	}

	// LBA SV_CLOSE-----
	{
		nIndex = n_msg_cnt++;

		sCH__INIT_SEQ_MSG[nIndex]->Set__DATA("LBA SLOT VALVE : Closing...");
		
		nRet = pLBA__OBJ_CTRL->Call__OBJECT(STR_LBx_SV_CLOSE);
		if(nRet < 0)
		{
			sCH__INIT_SEQ_MSG[nIndex]->Set__DATA("LBA SLOT VALVE : Closing Failed ...");
			return -1;
		}

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -1;
		}
	}

	// LBA -----
	{
		nIndex = n_msg_cnt++;

		sCH__INIT_SEQ_MSG[nIndex]->Set__DATA("LBA : Initialize...");
		nRet = pLBA__OBJ_CTRL->Call__OBJECT(STR__INIT);
		if(nRet < 0)
		{
			sCH__INIT_SEQ_MSG[nIndex]->Set__DATA("LBA : Init Failed ...");
			return -1;
		}

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -1;
		}
	}

	// LBB ---------------------------------------------------------------
	// LBB DV_CLOSE-----
	{
		nIndex = n_msg_cnt++;

		sCH__INIT_SEQ_MSG[nIndex]->Set__DATA("LBB DOOR VALVE: Closing...");
		
		nRet = pLBB__OBJ_CTRL->Call__OBJECT(STR_LBx_DV_CLOSE);
		if(nRet < 0)
		{
			sCH__INIT_SEQ_MSG[nIndex]->Set__DATA("LBB DOOR VALVE: Closing Failed ...");
			return -1;
		}

		if(p_variable->Check__CTRL_ABORT() > 0)	
		{
			return -1;
		}
	}

	// LBB SV_CLOSE-----
	{
		nIndex = n_msg_cnt++;

		sCH__INIT_SEQ_MSG[nIndex]->Set__DATA("LBB SLOT VALVE : Closing...");
		
		nRet = pLBB__OBJ_CTRL->Call__OBJECT(STR_LBx_SV_CLOSE);
		if(nRet < 0)
		{
			sCH__INIT_SEQ_MSG[nIndex]->Set__DATA("LBB SLOT VALVE : Closing Failed ...");
			return -1;
		}

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -1;
		}
	}

	// LBB -----
	{
		nIndex = n_msg_cnt++;

		sCH__INIT_SEQ_MSG[nIndex]->Set__DATA("LBB : Initialize...");
		
		nRet = pLBB__OBJ_CTRL->Call__OBJECT(STR__INIT);
		if(nRet < 0)
		{
			sCH__INIT_SEQ_MSG[nIndex]->Set__DATA("LBB : Init Failed...");
			return -1;
		}

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -1;
		}
	}

	// PMx SV CLOSE ------
	{
		CString str_set;
		int nID;

		for (int i=0; i<m_nPM_SIZE; i++)
		{
			nID = i+1;
			str_set.Format("%d", nID);
			aCH__PARA_PMC_ID->Set__DATA(str_set);

			CString sz_log_temp;
			sz_log_temp.Format("PM%d SLOT VALVE : Closing ...", i+1);
			
			nIndex = n_msg_cnt++;
			sCH__INIT_SEQ_MSG[nIndex]->Set__DATA(sz_log_temp);

			nRet = pPMx_SV__OBJ_CTRL->Call__OBJECT(STR_PMx_INIT_SV_CLOSE);
			if(nRet < 0)
			{
				sz_log_temp.Format("PM%d SLOT VALVE : Closing Failed...", i+1);
				sCH__INIT_SEQ_MSG[nIndex]->Set__DATA(sz_log_temp);
				return -1;
			}
			if(p_variable->Check__CTRL_ABORT() > 0)
			{
				return -1;
			}
		}
	}

	// TMC PUMP -----
	{
		nIndex = n_msg_cnt++;

		sCH__INIT_SEQ_MSG[nIndex]->Set__DATA("TMC PUMP : Complete... Waiting");
		
		nRet = pOBJ_CTRL__TMC_PUMP->When__OBJECT();
		if(nRet < 0)
		{
			sCH__INIT_SEQ_MSG[nIndex]->Set__DATA("TMC PUMP : Init Complete Failed...");
			return -1;
		}

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -1;
		}
	}
	// LLx PUMP -----
	{
		nIndex = n_msg_cnt++;

		sCH__INIT_SEQ_MSG[nIndex]->Set__DATA("LLx PUMP : Complete... Waiting");

		nRet = pOBJ_CTRL__LLx_PUMP->When__OBJECT();
		if(nRet < 0)
		{
			sCH__INIT_SEQ_MSG[nIndex]->Set__DATA("LLx PUMP : Init Complete Failed...");
			return -1;
		}

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -1;
		}
	}

	// TMC_CHM -----------------------------------------------------------
	{
		nIndex = n_msg_cnt++;

		sCH__INIT_SEQ_MSG[nIndex]->Set__DATA("TM CHAMBER : Initialize...");
		
		nRet = pTMC_CHM__OBJ_CTRL->Call__OBJECT(STR__INIT);
		if(nRet < 0)
		{
			sCH__INIT_SEQ_MSG[nIndex]->Set__DATA("TM CHAMBER : Init Failed...");
			return -1;
		}

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -1;
		}
	}
	//------------------------------------------------------------------

	sEXT_CH__GUI_POPUP_INIT_SEQ_MSG->Set__DATA("Close");
	return 1;
}

int  CObj__TMC_SYS_STD::Call__SYSTEM_MAINT(CII_OBJECT__VARIABLE* p_variable)
{
	int i;

	// ATM_ROBOT -----
	{
		pATM_ROBOT__OBJ_CTRL->Call__OBJECT(STR__MAINT);

		if(p_variable->Check__CTRL_ABORT() > 0)			return -1;
	}

	// AL1 -----
	{
		pAL1__OBJ_CTRL->Call__OBJECT(STR__MAINT);

		if(p_variable->Check__CTRL_ABORT() > 0)			return -1;
	}

	// LBA -----
	{
		pLBA__OBJ_CTRL->Call__OBJECT(STR__MAINT);

		if(p_variable->Check__CTRL_ABORT() > 0)			return -1;
	}

	// LBB -----
	{
		pLBB__OBJ_CTRL->Call__OBJECT(STR__MAINT);

		if(p_variable->Check__CTRL_ABORT() > 0)			return -1;
	}

	/*
	// VIS1 -----
	{
		pVIS1__OBJ_CTRL->Call__OBJECT(STR__MAINT);

		if(p_variable->Check__CTRL_ABORT() > 0)			return -1;
	}
	*/

	// LPx -----
	for(i=0;i<iLPx_LIMIT;i++)
	{
		dEXT_CH__LPx_OBJ_STATUS[i]->Set__DATA(STR__MAINTMODE);

		if(p_variable->Check__CTRL_ABORT() > 0)			return -1;
	}

	// VAC_ROBOT -----
	{
		pVAC_ROBOT__OBJ_CTRL->Call__OBJECT(STR__MAINT);

		if(p_variable->Check__CTRL_ABORT() > 0)			return -1;
	}

	// TMC_CHM -----
	{
		pTMC_CHM__OBJ_CTRL->Call__OBJECT(STR__MAINT);

		if(p_variable->Check__CTRL_ABORT() > 0)			return -1;
	}

	return 1;
}
