#include "StdAfx.h"
#include "CObj__OHT_E84_IO.h"
#include "CObj__OHT_E84_IO__DEF.h"


// ...
void CObj__OHT_E84_IO::
Mon__ALL_STATE(CII_OBJECT__VARIABLE* p_variable,
			   CII_OBJECT__ALARM* p_alarm)
{
	double nUnldCnt = 0;
	double nLdCnt   = 0;
	double nErrCnt  = 0;

	// ...
	double nLogic_time;
	double nLogic_Cnt;

	CString str__fuop_pio;

	// ...
	int loop_count = 0;

	if(iActive__SIM_MODE > 0)
	{
		dEXT_CH__LPx_IO__LIGHT_CURTAIN_DI->Set__DATA(_OFF);
	}


	while(1)
	{
		p_variable->Wait__SINGLE_OBJECT(0.01);

		loop_count++;
		if(loop_count > 40)			loop_count = 1;	

		// PIO LOG ...
		if(dCH__ACTIVE_PIO_LOG->Check__DATA(_YES) > 0)
		{
			Fnc__IO_LP_PIO_LOG(p_variable);
		}

		// FOUP VIOLATION ERROR ...
		if(loop_count == 1)
		{
			int state__pio_trans = dEXT_CH__LINK_PIO_TRANSFER_STATE->Check__DATA(_YES);
			CString str__fuop_pio = Is__FOUP_STATE_PIO();

			// FOUP VIOLATION ERROR ...
			{
				int check_state = -1;

				// CFG ...
				nLogic_time = aCH__CFG_PIO_FOUP_VIOLATION_TIME->Get__VALUE();
				nLogic_Cnt  = nLogic_time / 0.5;

				// CHECK : NONE ...
				if(state__pio_trans > 0)
				{
					check_state = 1;

					if(str__fuop_pio.CompareNoCase(_OFF) == 0)
					{
						nUnldCnt++;
						if(nUnldCnt >= nLogic_Cnt)
						{
							dCH__INR_NONE_VIOLATION_ERROR_FLAG->Set__DATA(_ON);
							dCH__INR_EXIST_VIOLATION_ERROR_FLAG->Set__DATA(_OFF);
						}
					}
					else if(str__fuop_pio.CompareNoCase(_ON) == 0)
					{
						nLdCnt++;
						if(nLdCnt >= nLogic_Cnt)
						{
							dCH__INR_NONE_VIOLATION_ERROR_FLAG->Set__DATA(_OFF);
							dCH__INR_EXIST_VIOLATION_ERROR_FLAG->Set__DATA(_ON);
						}
					}
				}
				else
				{
					nUnldCnt = 0;
					dCH__INR_NONE_VIOLATION_ERROR_FLAG->Set__DATA(_OFF);

					nLdCnt = 0;
					dCH__INR_EXIST_VIOLATION_ERROR_FLAG->Set__DATA(_OFF);
				}
			}

			// FOUP LOGIC ERROR ..
			{
				int check_state = -1;

				// CFG ...
				nLogic_time = aCH__CFG_PIO_FOUP_LOGIC_ERROR_TIME->Get__VALUE();
				nLogic_Cnt  = nLogic_time / 0.5;

				if(state__pio_trans > 0)
				{
					check_state = 1;

					if(str__fuop_pio.CompareNoCase(_UNKNOWN) == 0)
					{
						nErrCnt++;
						if(nErrCnt >= nLogic_Cnt)
							dCH__INR_LOGIC_ERROR_FLAG->Set__DATA(_ON);
					}
					else
					{
						check_state = -1;
					}
				}

				if(check_state < 0)
				{
					nErrCnt = 0;
					dCH__INR_LOGIC_ERROR_FLAG->Set__DATA(_OFF);
				}
			}

			// ERROR STATE ...
			{
				if((dCH__INR_NONE_VIOLATION_ERROR_FLAG->Check__DATA(_ON)  > 0)
				|| (dCH__INR_EXIST_VIOLATION_ERROR_FLAG->Check__DATA(_ON) > 0)
				|| (dCH__INR_LOGIC_ERROR_FLAG->Check__DATA(_ON) > 0))
				{
					dCH__REPORT_PIO_ERROR_ACTIVE->Set__DATA(_ON);
				}
				else
				{
					dCH__REPORT_PIO_ERROR_ACTIVE->Set__DATA(_OFF);
				}
			}
		}

		// ...
	}
}

void CObj__OHT_E84_IO::
Fnc__IO_LP_PIO_LOG(CII_OBJECT__VARIABLE* p_variable)
{
	int	log_update;

	// read
	CString LP_VALID_Read;		char clp_VALID_Read;
	CString LP_CS0_Read;		char clp_CS0_Read;
	CString LP_COMPT_Read;		char clp_COMPT_Read;
	CString LP_CS1_Read;		char clp_CS1_Read;
	CString LP_TRREQ_Read;	    char clp_TRREQ_Read;
	CString LP_BUSY_Read;	    char clp_BUSY_Read;
	CString LP_CONT_Read;	    char clp_CONT_Read;

	// write
	CString LP_LREQ_Set;		char clp_LREQ_Set;
	CString LP_UREQ_Set;		char clp_UREQ_Set;
	CString LP_READY_Set;		char clp_READY_Set;
	CString LP_HOVAL_Set;		char clp_HOVAL_Set;
	CString LP_ES_Set;		    char clp_ES_Set;

	// ...
	{
		log_update = -1;

		LP_VALID_Read = "~";
		LP_CS0_Read = "~";
		LP_COMPT_Read = "~";
		LP_CS1_Read = "~";
		LP_TRREQ_Read = "~";
		LP_BUSY_Read = "~";
		LP_CONT_Read = "~";

		LP_LREQ_Set  = "~";
		LP_UREQ_Set  = "~";
		LP_READY_Set = "~";
		LP_HOVAL_Set = "~";
		LP_ES_Set    = "~";
	}

	// ...
	{
		CString data;

		diEXT_CH__LPx__E84_VALID->Get__DATA(data);
		if(LP_VALID_Read.CompareNoCase(data) != 0)
		{
			LP_VALID_Read = data;
			log_update = 1;

			if(LP_VALID_Read.CompareNoCase(_ON) == 0)		clp_VALID_Read = '1';
			else											clp_VALID_Read = '.';
		}
		diEXT_CH__LPx__E84_CS_0->Get__DATA(data);
		if(LP_CS0_Read.CompareNoCase(data) != 0)
		{
			LP_CS0_Read = data;
			log_update = 1;

			if(LP_CS0_Read.CompareNoCase(_ON) == 0)			clp_CS0_Read = '1';
			else											clp_CS0_Read = '.';
		}

		diEXT_CH__LPx__E84_COMPT->Get__DATA(data);
		if(LP_COMPT_Read.CompareNoCase(data) != 0)
		{
			LP_COMPT_Read = data;
			log_update = 1;

			if(LP_COMPT_Read.CompareNoCase(_ON) == 0)		clp_COMPT_Read = '1';
			else											clp_COMPT_Read = '.';
		}
		diEXT_CH__LPx__E84_CS_1->Get__DATA(data);
		if(LP_CS1_Read.CompareNoCase(data) != 0)
		{
			LP_CS1_Read = data;
			log_update = 1;

			if(LP_CS1_Read.CompareNoCase(_ON) == 0)			clp_CS1_Read = '1';
			else											clp_CS1_Read = '.';
		}

		diEXT_CH__LPx__E84_TR_REQ->Get__DATA(data);
		if(LP_TRREQ_Read.CompareNoCase(data) != 0)
		{
			LP_TRREQ_Read = data;
			log_update = 1;

			if(LP_TRREQ_Read.CompareNoCase(_ON) == 0)		clp_TRREQ_Read = '1';
			else											clp_TRREQ_Read = '.';
		}
		diEXT_CH__LPx__E84_BUSY->Get__DATA(data);
		if(LP_BUSY_Read.CompareNoCase(data) != 0)
		{
			LP_BUSY_Read = data;
			log_update = 1;
			
			if(LP_BUSY_Read.CompareNoCase(_ON) == 0)		clp_BUSY_Read = '1';
			else											clp_BUSY_Read = '.';
		}

		diEXT_CH__LPx__E84_CONT->Get__DATA(data);
		if(LP_CONT_Read.CompareNoCase(data) != 0)
		{
			LP_CONT_Read = data;
			log_update = 1;

			if(LP_CONT_Read.CompareNoCase(_ON) == 0)		clp_CONT_Read = '1';
			else											clp_CONT_Read = '.';
		}

		doEXT_CH__LPx__E84_L_REQ->Get__DATA(data);
		if(LP_LREQ_Set.CompareNoCase(data) != 0)
		{
			LP_LREQ_Set = data;
			log_update = 1;

			if(LP_LREQ_Set.CompareNoCase(_ON) == 0)			clp_LREQ_Set = '1';
			else											clp_LREQ_Set = '.';
		}
		doEXT_CH__LPx__E84_U_REQ->Get__DATA(data);
		if(LP_UREQ_Set.CompareNoCase(data) != 0)
		{
			LP_UREQ_Set = data;
			log_update = 1;

			if(LP_UREQ_Set.CompareNoCase(_ON) == 0)			clp_UREQ_Set = '1';
			else											clp_UREQ_Set = '.';
		}

		doEXT_CH__LPx__E84_READY->Get__DATA(data);
		if(LP_READY_Set.CompareNoCase(data) != 0)
		{
			LP_READY_Set = data;
			log_update = 1;

			if(LP_READY_Set.CompareNoCase(_ON) == 0)		clp_READY_Set = '1';
			else											clp_READY_Set = '.';
		}
		doEXT_CH__LPx__E84_HO_AVBL->Get__DATA(data);
		if(LP_HOVAL_Set.CompareNoCase(data) != 0)
		{
			LP_HOVAL_Set = data;
			log_update = 1;

			if(LP_HOVAL_Set.CompareNoCase(_ON) == 0)		clp_HOVAL_Set = '1';
			else											clp_HOVAL_Set = '.';
		}

		doEXT_CH__LPx__E84_ES->Get__DATA(data);
		if(LP_ES_Set.CompareNoCase(data) != 0)
		{
			LP_ES_Set = data;
			log_update = 1;

			if(LP_ES_Set.CompareNoCase(_ON) == 0)			clp_ES_Set = '1';
			else											clp_ES_Set = '.';
		}

		if(log_update > 0)	
		{
			log_update = -1;

			CString msg;

			msg.Format("LP  = IN[   %c      %c     %c      %c      %c      %c     %c  ] OUT[   %c      %c      %c       %c      %c     ]",
						// input
						clp_VALID_Read,
						clp_CS0_Read,
						clp_COMPT_Read,
						clp_CS1_Read,
						clp_TRREQ_Read,
						clp_BUSY_Read,
						clp_CONT_Read,
						// output
						clp_LREQ_Set,
						clp_UREQ_Set,
						clp_READY_Set,
						clp_HOVAL_Set,
						clp_ES_Set
						//clp_AMHS_STOP_Set
						);
		
			Fnc__PIO_LOG(msg);
		}
	}
}
