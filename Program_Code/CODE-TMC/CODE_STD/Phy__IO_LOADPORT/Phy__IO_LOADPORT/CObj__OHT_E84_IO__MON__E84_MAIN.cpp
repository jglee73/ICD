#include "StdAfx.h"
#include "CObj__OHT_E84_IO.h"
#include "CObj__OHT_E84_IO__DEF.h"


//------------------------------------------------------------------------------------
void CObj__OHT_E84_IO::
Mon__IO_E84_MAIN(CII_OBJECT__VARIABLE* p_variable,
				 CII_OBJECT__ALARM* p_alarm)
{
	int n_ret_cs_valid_wait;
	int n_call_ret;


	while (1)
	{
		p_variable->Wait__SINGLE_OBJECT(0.1);

		
		// Initial ...
		{
			dEXT_CH__LINK_PIO_TRANSFER_STATE->Set__DATA(_NO);
			dCH__REPORT_PIO_E84_RUN_STATE->Set__DATA(_STOP);

			dCH__PIO_RESET->Set__DATA(_OFF);
		}

		// ...
		bool active__pio_check = false;

		if(dEXT_CH__LINK_PIO_ACTIVE_FA_AUTO->Check__DATA(_ON) > 0)
		{
			if((dEXT_CH__LINK_PIO_ACTIVE_LOAD_REQ->Check__DATA(_ON)   > 0)
			|| (dEXT_CH__LINK_PIO_ACTIVE_UNLOAD_REQ->Check__DATA(_ON) > 0))
			{
				active__pio_check = true;
			}
		}

		if(active__pio_check)
		{
			if(dEXT_CH__LINK_PIO_ACTIVE_RUN->Check__DATA(_ON) < 0)
			{
				active__pio_check = false;
			}
		}

		if(!active__pio_check)
		{
			Set__HOAVBL(_OFF);
			Set__AllOff_Except_ES();
			continue;
		}
		
		// ...
		{
			CString active__pio_mode;

				 if(dEXT_CH__LINK_PIO_ACTIVE_LOAD_REQ->Check__DATA(_ON)   > 0)		active__pio_mode = _PIO_CMMD__LOAD_REQ;
			else if(dEXT_CH__LINK_PIO_ACTIVE_UNLOAD_REQ->Check__DATA(_ON) > 0)		active__pio_mode = _PIO_CMMD__UNLOAD_REQ;
			else																	continue;

			//
			Set__AllOff_Except_ES();
			Set__HOAVBL(_ON);

			// 3. CS0, VALID [ON] Waiting....
			Fnc__E84_LOG("Full Auto... CS and VALID [ON] Waiting... !!");
			n_ret_cs_valid_wait = Fnc__CS_VALID_WAITING(p_variable, p_alarm, active__pio_mode);

			if(n_ret_cs_valid_wait == OBJ_AVAILABLE)
			{
				// PIO Start !!
			}
			else if(n_ret_cs_valid_wait == SEQ_COMPLETE)
			{
				dCH__REPORT_PIO_E84_RUN_STATE->Set__DATA(_DONE);
				
				_sleep(1000);
				continue;
			}
			else
			{
				continue;
			}

			if(active__pio_mode.CompareNoCase(_PIO_CMMD__LOAD_REQ) == 0)
			{
				sCH__CUR_CHECK_TPx->Set__DATA("__");
				n_call_ret = Fnc__LOAD(p_variable, p_alarm);
				sCH__CUR_CHECK_TPx->Set__DATA("");

				_sleep(1000);
			}
			else if(active__pio_mode.CompareNoCase(_PIO_CMMD__UNLOAD_REQ) == 0)
			{
				sCH__CUR_CHECK_TPx->Set__DATA("__");
				n_call_ret = Fnc__UNLOAD(p_variable, p_alarm);
				sCH__CUR_CHECK_TPx->Set__DATA("");

				_sleep(1000);
			}
			else
			{
				n_call_ret = -1;
			}

			if(n_call_ret == SEQ_COMPLETE)
			{
				dCH__REPORT_PIO_E84_RUN_STATE->Set__DATA(_DONE); 

				_sleep(1000);
			}

			Set__AllOff_Except_ES();
		}

		// ...
	}
}

