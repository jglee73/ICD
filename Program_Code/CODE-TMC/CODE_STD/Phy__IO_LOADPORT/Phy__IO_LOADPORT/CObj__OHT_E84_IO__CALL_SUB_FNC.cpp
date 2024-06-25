#include "StdAfx.h"
#include "CObj__OHT_E84_IO.h"
#include "CObj__OHT_E84_IO__DEF.h"


//-------------------------------------------------------------------------	
int  CObj__OHT_E84_IO::
Fnc__INIT(CII_OBJECT__VARIABLE* p_variable,
		  CII_OBJECT__ALARM* p_alarm)
{
	return OBJ_AVAILABLE;
}

int  CObj__OHT_E84_IO
::Fnc__CS_VALID_WAITING(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString& active__pio_cmmd)
{
	// Read TP config
	Get__Read_TP_Config();

	CString szFOUP_Sns;
	CString szUser_Log;
	CString str_data;

	int nDo_Loop = OBJ_ABORT;
	int nzCS0_Alarm_Count = 0;
	double dblfoup_violation_time = 0.0;
	double dblfoup_logic_time = 0.0;
	SCX__TIMER_CTRL Wait_Time;

	int nPlace_First      = -1;
	int nPresent_First    = -1;
	int nFoup_None_First  = -1;
	int nFoup_Exist_First = -1;
	
	/*
	1. Check AUTO mode
	2. Wait CS0 : On , VALID : On
	3. Check FOUP Exist or not
	4. Set ES : On , HOAVBL : On
	*/
	
	SCX__ASYNC_TIMER_CTRL LP_Present_Timer;
	SCX__ASYNC_TIMER_CTRL LP_Place_Timer;
	SCX__ASYNC_TIMER_CTRL LP_None_Timer;
	SCX__ASYNC_TIMER_CTRL LP_Exist_Timer;

	int timer_present_ret, timer_place_ret;
	int timer_none_ret,    timer_exist_ret;

	timer_present_ret = LP_Present_Timer->REGISTER__COUNT_CHANNEL(sObject_Name, sCH__INR_CST_PRESENT_TIMER_DISPLAY->Get__VARIABLE_NAME());
	timer_place_ret   = LP_Place_Timer->REGISTER__COUNT_CHANNEL(sObject_Name, sCH__INR_CST_PLACE_TIMER_DISPLAY->Get__VARIABLE_NAME());
	timer_none_ret    = LP_None_Timer->REGISTER__COUNT_CHANNEL(sObject_Name, sCH__INR_CST_NONE_TIMER_DISPLAY->Get__VARIABLE_NAME());
	timer_exist_ret   = LP_Exist_Timer->REGISTER__COUNT_CHANNEL(sObject_Name, sCH__INR_CST_EXIST_TIMER_DISPLAY->Get__VARIABLE_NAME());

	LP_Present_Timer->STOP();
	LP_Place_Timer->STOP();
	LP_None_Timer->STOP();
	LP_Exist_Timer->STOP();

	aCH__CFG_PIO_FOUP_VIOLATION_TIME->Get__DATA(str_data);
	dblfoup_violation_time = atof(str_data);

	aCH__CFG_PIO_FOUP_LOGIC_ERROR_TIME->Get__DATA(str_data);
	dblfoup_logic_time = atof(str_data);

	nDo_Loop = OBJ_ABORT;
	nzCS0_Alarm_Count = 0;
	
	if(iActive__SIM_MODE > 0)
	{
		if(active__pio_cmmd.CompareNoCase(_PIO_CMMD__LOAD_REQ) == 0)
		{
			dEXT_CH__LPx_IO__FOUP_PLACED_DI->Set__DATA(_OFF);
			dEXT_CH__LPx_IO__FOUP_PRESENT_DI->Set__DATA(_OFF);
		}
		else if(active__pio_cmmd.CompareNoCase(_PIO_CMMD__UNLOAD_REQ) == 0)
		{
			dEXT_CH__LPx_IO__FOUP_PLACED_DI->Set__DATA(_ON);
			dEXT_CH__LPx_IO__FOUP_PRESENT_DI->Set__DATA(_ON);
		}

		diEXT_CH__LPx__E84_CS_0->Set__DATA(_ON);
		diEXT_CH__LPx__E84_VALID->Set__DATA(_ON);
		diEXT_CH__LPx__E84_TR_REQ->Set__DATA(_OFF);
		diEXT_CH__LPx__E84_BUSY->Set__DATA(_OFF);
		diEXT_CH__LPx__E84_COMPT->Check__DATA(_OFF);
	}

	do 
	{
		// 1. CS_0 [ON] 또는 CS_0 [ON], VALID [ON] 인가 ?
		if(Is__Sns_CS0() > 0)
		{
			nDo_Loop = OBJ_AVAILABLE;
		}
		else if( (Is__Sns_CS0() > 0) && (Is__Sns_VALID() > 0))
		{
			nDo_Loop = OBJ_AVAILABLE;
		}
		
		// 2. FULL AUTO 인가 ?
		if(Is__LP_AUTO_MODE() < 0)
		{
			szUser_Log.Format("During Wait CS & VALID ON, [NOT FULL AUTO] return [-1] !!");
			Fnc__E84_LOG(szUser_Log);

			return NOT_FULL_AUTO;
		}

		// 3. Object 가 Abort?
		if(Is__Abort(p_variable) > 0)
		{
			szUser_Log.Format("During Wait CS & VALID ON, [Object Aborted] return [-1] !!");
			Fnc__E84_LOG(szUser_Log);

			return OBJ_ABORT;
		}

		// 5. LT가 BROKEN ?
		if(Is__LT_BROKEN(p_variable, p_alarm) < 0)	
		{
			return OBJ_ABORT;
		}

		// 6. [PREPLOAD] LP에 Material ON ?
		// 7. [UNLOAD]   LP에 FOUP이 NONE ?
		szFOUP_Sns = Is__FOUP_STATE_PIO();

		// Load 때 Foup이 임의로 놓인 경우...
		if(szFOUP_Sns.CompareNoCase(_ON) == 0)
		{
			// stop timer
			if(nPresent_First > 0)
			{
				szUser_Log.Format("===> Before Scenario Start Wait or Started, PR[ON] Async Timer[%.f sec] End1 - Alarm None", dblfoup_violation_time);
				Fnc__E84_LOG(szUser_Log);
				
				LP_Present_Timer->STOP();

				nPresent_First = -1;
			}
			
			if(nPlace_First > 0)
			{
				szUser_Log.Format("===> Before Scenario Start Wait or Started, PL[ON] Async Timer[%.f sec] End1 - Alarm None", dblfoup_violation_time);
				Fnc__E84_LOG(szUser_Log);
				
				LP_Place_Timer->STOP();
				
				nPlace_First   = -1;
			}

			if(nFoup_None_First > 0)
			{
				szUser_Log.Format("===> Before Scenario Start Wait or Started, FOUP NONE Async Timer[%.f sec] End1 - Alarm None", dblfoup_violation_time);
				Fnc__E84_LOG(szUser_Log);
				
				LP_None_Timer->STOP();
				
				nFoup_None_First   = -1;
			}

			if(active__pio_cmmd.CompareNoCase(_PIO_CMMD__LOAD_REQ) == 0)
			{
				nPlace_First     = -1;
				nPresent_First   = -1;
				nFoup_None_First = -1;

				if(nFoup_Exist_First < 0)
				{
					// start timer
					LP_Exist_Timer->STOP();
					LP_Exist_Timer->START__COUNT_DOWN(dblfoup_violation_time);
					
					nFoup_Exist_First = 1;

					szUser_Log.Format("===> Before Scenario Start Wait or Started, FOUP NONE Async Timer[%.f sec] Start", dblfoup_violation_time);
					Fnc__E84_LOG(szUser_Log);
				}
				else if(LP_Exist_Timer->Get__CURRENT_TIME() <= 0.1)
				{
					Set__HOAVBL("OFF");
					
					szUser_Log.Format("===> Before Scenario Start Wait or Started, Now Foup[EXIST] == ERROR Occur");
					Fnc__E84_LOG(szUser_Log);
					
					// ...
					{
						int alarm_id = ALID__LP_E84A_LOAD_FOUP_EXIST;
						CString r_act;
					
						p_alarm->Popup__ALARM_With_MESSAGE(alarm_id, szUser_Log, r_act);
						nFoup_Exist_First = -1;
					
						     if(r_act.CompareNoCase(ACT__RETRY)    == 0)			return OBJ_RETRY;
						else if(r_act.CompareNoCase(ACT__COMPLETE) == 0)			return SEQ_COMPLETE;
	
						return OBJ_ABORT;
					}
				}
			}
		}
		// Foup이 사라진 경우 !!
		else if(szFOUP_Sns.CompareNoCase(_OFF) == 0)
		{
			// stop timer
			if(nPresent_First > 0)
			{
				szUser_Log.Format("===> Before Scenario Start Wait or Started, PR[ON] Async Timer[%.f sec] End1 - Alarm None", dblfoup_violation_time);
				Fnc__E84_LOG(szUser_Log);
				
				LP_Present_Timer->STOP();

				nPresent_First = -1;
			}
			
			if(nPlace_First > 0)
			{
				szUser_Log.Format("===> Before Scenario Start Wait or Started, PL[ON] Async Timer[%.f sec] End1 - Alarm None", dblfoup_violation_time);
				Fnc__E84_LOG(szUser_Log);
				
				LP_Place_Timer->STOP();
				
				nPlace_First   = -1;
			}

			if(nFoup_Exist_First > 0)
			{
				szUser_Log.Format("===> Before Scenario Start Wait or Started, FOUP EXIST Async Timer[%.f sec] End1 - Alarm None", dblfoup_violation_time);
				Fnc__E84_LOG(szUser_Log);
				
				LP_Exist_Timer->STOP();
				
				nFoup_Exist_First   = -1;
			}

			if(active__pio_cmmd.CompareNoCase(_PIO_CMMD__UNLOAD_REQ) == 0)
			{
				nPlace_First      = -1;
				nPresent_First    = -1;
				nFoup_Exist_First = -1;

				if(nFoup_None_First < 0)
				{
					// start timer
					LP_None_Timer->STOP();
					LP_None_Timer->START__COUNT_DOWN(dblfoup_violation_time);
					
					nFoup_None_First = 1;

					szUser_Log.Format("===> Before Scenario Start Wait or Started, FOUP NONE Async Timer[%.f sec] Start", dblfoup_violation_time);
					Fnc__E84_LOG(szUser_Log);
				}
				else if(LP_None_Timer->Get__CURRENT_TIME() <= 0.1)
				{
					Set__HOAVBL(_OFF);
					
					szUser_Log.Format("===> Before Scenario Start Wait or Started, Now FOUP [NONE] == ERROR Occur");
					Fnc__E84_LOG(szUser_Log);
					
					// ...
					{
						int alarm_id = ALID__LP_E84A_UNLOAD_FOUP_NONE;
						CString r_act;
					
						p_alarm->Popup__ALARM_With_MESSAGE(alarm_id, szUser_Log, r_act);
						nFoup_None_First = -1;
						
						     if(r_act.CompareNoCase(ACT__RETRY)    == 0)		return OBJ_RETRY;
						else if(r_act.CompareNoCase(ACT__COMPLETE) == 0)		return SEQ_COMPLETE;
					
						return OBJ_ABORT;
					}
				}
			}
		}
		else if(szFOUP_Sns.CompareNoCase(_UNKNOWN) == 0)
		{
			if(dEXT_CH__LPx_IO__FOUP_PRESENT_DI->Check__DATA(_ON) > 0)
			{
				nPlace_First      = -1;
				nFoup_None_First  = -1;
				nFoup_Exist_First = -1;

				if(nPresent_First < 0)
				{
					// start timer
					LP_Present_Timer->STOP();
					LP_Present_Timer->START__COUNT_DOWN(dblfoup_logic_time);
					
					nPresent_First = 1;

					szUser_Log.Format("===> Before Scenario Start Wait or Started, PR[ON] Async Timer[%.f sec] Start", dblfoup_logic_time);
					Fnc__E84_LOG(szUser_Log);
				}
				else if(LP_Present_Timer->Get__CURRENT_TIME() <= 0.1)
				{
					Set__HOAVBL(_OFF);

					szUser_Log.Format("===> Before Scenario Start Wait or Started, PR[ON] Async Timer[%.f sec] End - Alarm Occur", dblfoup_logic_time);
					Fnc__E84_LOG(szUser_Log);

					// ...
					{
						int alarm_id = ALID__LP_E84A_PRESENT_SNS_ON;
						CString r_act;
					
						p_alarm->Popup__ALARM_With_MESSAGE(alarm_id, szUser_Log, r_act);
						
						     if(r_act.CompareNoCase(ACT__RETRY)    == 0)		return OBJ_RETRY;
						else if(r_act.CompareNoCase(ACT__COMPLETE) == 0)		return SEQ_COMPLETE;
					
						return OBJ_ABORT;
					}
				}
			}
			else if(dEXT_CH__LPx_IO__FOUP_PLACED_DI->Check__DATA(_ON) > 0)
			{
				nPresent_First = -1;
				nFoup_None_First  = -1;
				nFoup_Exist_First = -1;
				
				// start timer
				if(nPlace_First < 0)
				{
					// start timer
					LP_Place_Timer->STOP();
					LP_Place_Timer->START__COUNT_DOWN(dblfoup_logic_time);
					
					nPlace_First = 1;

					szUser_Log.Format("===> Before Scenario Start Wait or Started, PL[ON] Async Timer[%.f sec] Start", dblfoup_logic_time);
					Fnc__E84_LOG(szUser_Log);
				}
				else if(LP_Place_Timer->Get__CURRENT_TIME() <= 0.1)
				{
					szUser_Log.Format("===> Before Scenario Start Wait or Started, PL[ON] Async Timer[%.f sec] End - Alarm Occur", dblfoup_logic_time);
					Fnc__E84_LOG(szUser_Log);

					// ... OFF
					Set__HOAVBL("OFF");

					int alarm_id = ALID__LP_E84A_PLACE_SNS_ON;
					CString r_act;

					p_alarm->Popup__ALARM_With_MESSAGE(alarm_id, szUser_Log, r_act);

					     if(r_act.CompareNoCase(ACT__RETRY)    == 0)		return OBJ_RETRY;
					else if(r_act.CompareNoCase(ACT__COMPLETE) == 0)		return SEQ_COMPLETE;
					
					return OBJ_ABORT;
				}
			}
 			else
 			{
				// stop timer
				if(nPresent_First > 0)
				{
					szUser_Log.Format("===> Before Scenario Start Wait or Started, PR[ON] Async Timer[%.f sec] End2 - Alarm None", dblfoup_logic_time);
					Fnc__E84_LOG(szUser_Log);

					LP_Present_Timer->STOP();

					nPresent_First = -1;
				}
				
				if(nPlace_First > 0)
				{
					szUser_Log.Format("===> Before Scenario Start Wait or Started, PL[ON] Async Timer[%.f sec] End2 - Alarm None", dblfoup_logic_time);
					Fnc__E84_LOG(szUser_Log);
					
					LP_Place_Timer->STOP();
					
					nPlace_First = -1;
				}

				if(nFoup_None_First > 0)
				{
					szUser_Log.Format("===> Before Scenario Start Wait or Started, FOUP NONE Async Timer[%.f sec] End3 - Alarm None", dblfoup_violation_time);
					Fnc__E84_LOG(szUser_Log);
					
					LP_None_Timer->STOP();
					
					nFoup_None_First = -1;
				}

				if(nFoup_Exist_First > 0)
				{
					szUser_Log.Format("===> Before Scenario Start Wait or Started, FOUP EXIST Async Timer[%.f sec] End3 - Alarm None", dblfoup_violation_time);
					Fnc__E84_LOG(szUser_Log);
					
					LP_Exist_Timer->STOP();
					
					nFoup_Exist_First = -1;
				}
 			}
		}
		else
		{
			// stop timer
			if(nPresent_First > 0)
			{
				szUser_Log.Format("===> Before Scenario Start Wait or Started, PR[ON] Async Timer[%.f sec] End3 - Alarm None", dblfoup_logic_time);
				Fnc__E84_LOG(szUser_Log);
				
				LP_Present_Timer->STOP();

				nPresent_First = -1;
			}
			
			if(nPlace_First > 0)
			{
				szUser_Log.Format("===> Before Scenario Start Wait or Started, PL[ON] Async Timer[%.f sec] End3 - Alarm None", dblfoup_logic_time);
				Fnc__E84_LOG(szUser_Log);
				
				LP_Place_Timer->STOP();
				
				nPlace_First = -1;
			}

			if(nFoup_None_First > 0)
			{
				szUser_Log.Format("===> Before Scenario Start Wait or Started, FOUP NONE Async Timer[%.f sec] End3 - Alarm None", dblfoup_violation_time);
				Fnc__E84_LOG(szUser_Log);
				
				LP_None_Timer->STOP();
				
				nFoup_None_First = -1;
			}

			if(nFoup_Exist_First > 0)
			{
				szUser_Log.Format("===> Before Scenario Start Wait or Started, FOUP EXIST Async Timer[%.f sec] End3 - Alarm None", dblfoup_violation_time);
				Fnc__E84_LOG(szUser_Log);
				
				LP_Exist_Timer->STOP();
				
				nFoup_Exist_First = -1;
			}
		}

		Wait_Time->WAIT(0.05);

		if((nDo_Loop > 0)  
		&& ((nPresent_First < 0) && (nPlace_First < 0) && (nFoup_None_First < 0) && (nFoup_Exist_First < 0)))
		{
			szUser_Log.Format(">>>>>>>>>>>> SCENARIO START !! -- CS0[ON], VALID[ON] & Foup Sensor Normal >>>>>>>>>>>>");
			Fnc__E84_LOG(szUser_Log);
			break;
		}
	} 
	while(1);

	return nDo_Loop;
}

int  CObj__OHT_E84_IO::
Fnc__LOAD_PIO(CII_OBJECT__VARIABLE* p_variable,
			  CII_OBJECT__ALARM* p_alarm)
{
	CString sMsg;

	// Msg Write !!
	Fnc__E84_LOG("\n");
	sMsg.Format("LOAD Starting ...");
	Fnc__APP_MSG(sMsg);	Fnc__E84_LOG(sMsg);

	// Check Online !!
	if(Is__ONLINE(p_variable, p_alarm) < 0)	
	{
		return -1;
	}

	//-------------------------------------------------------------------------
	CString data;
	CString str_alarm_msg;
	CString sLog_Msg;
	CString rc_act;

	int status, TP_temp = -1;
	int retCode = -1;
	int unknown_timer_ret = 0;
	int tp_timer_ret      = 0;
	
	double dblChattering_time = 0.0;
	double dblCheck_Polling_time = 0.05;	// 0.05 초...
	int nCnt_Active_Signal_Abnormal = 0;
	int nCnt_Temp = 0;
	
	SCX__TIMER_CTRL Wait_Time;
	SCX__ASYNC_TIMER_CTRL LP_UNKNOWN_Timer;
	SCX__ASYNC_TIMER_CTRL LP_TP_Timer;

	aCH__CFG_PIO_SIGNAL_CHATTERING_TIME->Get__DATA(data);
	dblChattering_time = atof(data);

	unknown_timer_ret = LP_UNKNOWN_Timer->REGISTER__COUNT_CHANNEL(sObject_Name, sCH__INR_CST_UNKNOWN_TIMER_COUNTER->Get__VARIABLE_NAME());
	tp_timer_ret	  = LP_TP_Timer->REGISTER__COUNT_CHANNEL(sObject_Name, sCH__INR_TP_TIMER_COUNTER->Get__VARIABLE_NAME());

	LP_UNKNOWN_Timer->STOP();
	LP_TP_Timer->STOP();	

	sLog_Msg.Format("[E84] PIO_LOAD Start...(Register:UNKNOWN_TIMER ret:%d, TP_TIMER ret:%d)",  unknown_timer_ret, tp_timer_ret);
	Fnc__E84_LOG(sLog_Msg);

	sLog_Msg.Format("Config Chattering Time:%.f)",  dblChattering_time);
	Fnc__E84_LOG(sLog_Msg);
 
 	LP_TP_Timer->STOP();
	LP_UNKNOWN_Timer->STOP();

	nCnt_Active_Signal_Abnormal = int(dblChattering_time/(dblCheck_Polling_time*1000));	// 0.05초 * 1000 = msec

	CString valid_chk;
	
// 	1. LREQ -> On
// 	2. Wait TR_REQ -> On : TP1
// 	3. READY -> On
// 	4. Wait Busy On : TP2
// 	5. Check FOUP Sns : TP3, LREQ -> Off, HOAVBL -> Off
// 	6. Wait BUSY Off and COMPT -> On : TP4
// 	7. READY -> Off
// 	8. Wait CS0, VALID, COMPT -> Off : TP5

	// Before L_REQ set.. Signal Check..
	nCnt_Temp = 0;

	do 
	{
		if(Active_Signal_Check(true, "START", str_alarm_msg, valid_chk) != OBJ_AVAILABLE)
		{
			nCnt_Temp++;
			if(nCnt_Temp > nCnt_Active_Signal_Abnormal)
			{
				nCnt_Temp = 0;
				// Active Signal Abnormal !!
				Set__AllOff_Except_ES();
								
				Fnc__E84_LOG(" * Starting Before Set L_REQ[ON], Active Abnormal Signal Alarm Occur !!.");

				int alarm_id;
				CString r_act;

				if(valid_chk.CompareNoCase("VALID_OFF") ==0)
				{
					alarm_id = ALID__LP_E84A_ACTIVE_VALID_ABNORMAL_ERROR_OCCUR;
					p_alarm->Check__ALARM(alarm_id, r_act);
					p_alarm->Popup__ALARM(alarm_id, r_act);

						 if(r_act.CompareNoCase("RETRY") == 0)			return OBJ_RETRY;
					else if(r_act.CompareNoCase("COMPLETE") == 0)		return SEQ_COMPLETE;
					
					return OBJ_ABORT;

				}
				else
				{
					alarm_id = ALID__LP_E84A_ACTIVE_ABNORMAL_SIGNAL_OCCUR;
					p_alarm->Check__ALARM(ALID__LP_E84A_ACTIVE_ABNORMAL_SIGNAL_OCCUR, r_act);
					p_alarm->Popup__ALARM_With_MESSAGE(alarm_id, str_alarm_msg, r_act);

						 if(r_act.CompareNoCase("RETRY") == 0)			return OBJ_RETRY;
					else if(r_act.CompareNoCase("COMPLETE") == 0)		return SEQ_COMPLETE;
					
					return OBJ_ABORT;
				}
			}
		}
		else if(dCH__PIO_RESET->Check__DATA(_ON) > 0)
		{
			return OBJ_ABORT;
		}
		else
		{
			break;
		}
		
		// ...
		{
			if(Is__LT_BROKEN(p_variable, p_alarm) < 0)			return OBJ_ABORT;
			if(Is__Abort(p_variable) > 0)						return OBJ_ABORT;
		}

		if(Is__FOUP_LOGIC_ERROR(p_variable, p_alarm, rc_act) < 0)
		{
				 if(rc_act.CompareNoCase("RETRY")    == 0)		return OBJ_RETRY;
			else if(rc_act.CompareNoCase("COMPLETE") == 0)		return SEQ_COMPLETE;
			
			return OBJ_ABORT;
		}
		if(Is__FOUP_VIOLATION_ERROR(p_variable, p_alarm, "NONE") < 0)
		{
				 if(rc_act.CompareNoCase("RETRY")    == 0)		return OBJ_RETRY;
			else if(rc_act.CompareNoCase("COMPLETE") == 0)		return SEQ_COMPLETE;
			
			return OBJ_ABORT;
		}
		
		Wait_Time->POLL(dblCheck_Polling_time);		
	} 
	while(1);	

	//1. LREQ -> On
	Set__LREQ("ON");
	
	//2. Wait TR_REQ -> On : TP1
	sLog_Msg.Format("[E84] PIO_LOAD Start...(TP1:%d)", m_nTP1);
	Fnc__E84_LOG(sLog_Msg);

	status    = -1;
	nCnt_Temp = 0;
	LP_TP_Timer->START__COUNT_DOWN((double)m_nTP1);

	do 
	{
		if(Is__Sns_TRREQ() > 0)	
		{
			status = 1;
			sLog_Msg.Format("[E84] PIO_LOAD Complete...(TP1)\n");
			Fnc__E84_LOG(sLog_Msg);
		}
		else if(LP_TP_Timer->Get__CURRENT_TIME() <= 0.1)
		{
			Set__AllOff_Except_ES();
					
			Fnc__E84_LOG(" * TP1 Timeout Occur, When Load Sequence.");

			// ...
			{
				int alarm_id = ALID__LP_E84A_TP1;
				CString r_act;
				p_alarm->Check__ALARM(ALID__LP_E84A_TP1, r_act);
				p_alarm->Popup__ALARM(alarm_id, r_act);

					 if(r_act.CompareNoCase("RETRY")    == 0)		return OBJ_RETRY;
				else if(r_act.CompareNoCase("COMPLETE") == 0)		return SEQ_COMPLETE;

				return OBJ_ABORT;
			}
		}
		else if(Active_Signal_Check(true, "TP1", str_alarm_msg,valid_chk) != OBJ_AVAILABLE)
		{
			nCnt_Temp++;

			if(nCnt_Temp > nCnt_Active_Signal_Abnormal)
			{
				nCnt_Temp = 0;
				Set__AllOff_Except_ES();
								
				Fnc__E84_LOG(" * Waiting TP1:TR_REQ[ON], Active Abnormal Signal Alarm Occur !!.");

				// ...
				int alarm_id;
				CString r_act;

				if(valid_chk.CompareNoCase("VALID_OFF") ==0)
				{
					alarm_id = ALID__LP_E84A_ACTIVE_VALID_ABNORMAL_ERROR_OCCUR;
					p_alarm->Check__ALARM(alarm_id, r_act);
					p_alarm->Popup__ALARM(alarm_id, r_act);

					     if(r_act.CompareNoCase("RETRY")    == 0)		return OBJ_RETRY;
					else if(r_act.CompareNoCase("COMPLETE") == 0)		return SEQ_COMPLETE;

					return OBJ_ABORT;
				}
				else
				{
					alarm_id = ALID__LP_E84A_ACTIVE_ABNORMAL_SIGNAL_OCCUR;
					p_alarm->Check__ALARM(ALID__LP_E84A_ACTIVE_ABNORMAL_SIGNAL_OCCUR, r_act);
					p_alarm->Popup__ALARM_With_MESSAGE(alarm_id, str_alarm_msg, r_act);

					     if(r_act.CompareNoCase("RETRY")    == 0)		return OBJ_RETRY;
					else if(r_act.CompareNoCase("COMPLETE") == 0)		return SEQ_COMPLETE;
					
					return OBJ_ABORT;
				}
			}
		}
		else if(dCH__PIO_RESET->Check__DATA(_ON) > 0)
		{
			return OBJ_ABORT;
		}

		// ...
		{
			if(Is__LT_BROKEN(p_variable, p_alarm) < 0)			return OBJ_ABORT;
			if(Is__Abort(p_variable) > 0)						return OBJ_ABORT;
		}

		if(Is__FOUP_LOGIC_ERROR(p_variable, p_alarm, rc_act) < 0)
		{
				 if(rc_act.CompareNoCase("RETRY")    == 0)		return OBJ_RETRY;
			else if(rc_act.CompareNoCase("COMPLETE") == 0)		return SEQ_COMPLETE;

			return OBJ_ABORT;
		}

		if(Is__FOUP_VIOLATION_ERROR(p_variable, p_alarm, "NONE") < 0)
		{
				 if(rc_act.CompareNoCase("RETRY")    == 0)		return OBJ_RETRY;
			else if(rc_act.CompareNoCase("COMPLETE") == 0)		return SEQ_COMPLETE;

			return OBJ_ABORT;
		}

		Wait_Time->POLL(dblCheck_Polling_time);		
	} 
	while(status < 0);

 	LP_TP_Timer->STOP();

	//3. READY -> On
	Set__READY("ON");

	//4. Wait Busy -> On : TP2
	sLog_Msg.Format("[E84] PIO_LOAD Start...(TP2:%d)", m_nTP2);
	Fnc__E84_LOG(sLog_Msg);

	status    = -1;
	nCnt_Temp = 0;
	LP_TP_Timer->START__COUNT_DOWN((double)m_nTP2);

	do 
	{
		if(Is__Sns_BUSY() > 0)
		{
			status = 1;
			sLog_Msg.Format("[E84] PIO_LOAD Complete...(TP2)\n");
			Fnc__E84_LOG(sLog_Msg);
		}
		else if(LP_TP_Timer->Get__CURRENT_TIME() <= 0.1)
		{
			Set__AllOff_Except_ES();
			
			Fnc__E84_LOG(" * TP2 Timeout Occur, When Load Sequence.");

			// ...
			{
				int alarm_id = ALID__LP_E84A_TP2;
				CString r_act;

				p_alarm->Check__ALARM(ALID__LP_E84A_TP2, r_act);
				p_alarm->Popup__ALARM(alarm_id, r_act);

					 if(r_act.CompareNoCase("RETRY")    == 0)		return OBJ_RETRY;
				else if(r_act.CompareNoCase("COMPLETE") == 0)		return SEQ_COMPLETE;
				
				return OBJ_ABORT;
			}
		}
		else if(Active_Signal_Check(true, "TP2", str_alarm_msg,valid_chk) != OBJ_AVAILABLE)
		{
			nCnt_Temp++;

			if(nCnt_Temp > nCnt_Active_Signal_Abnormal)
			{
				nCnt_Temp = 0;
				Set__AllOff_Except_ES();
								
				Fnc__E84_LOG(" * Waiting TP2:BUSY[ON], Active Abnormal Signal Alarm Occur !!.");

				int alarm_id;
				CString r_act;

				if(valid_chk.CompareNoCase("VALID_OFF") ==0)
				{
					alarm_id = ALID__LP_E84A_ACTIVE_VALID_ABNORMAL_ERROR_OCCUR;
					p_alarm->Check__ALARM(alarm_id, r_act);
					p_alarm->Popup__ALARM(alarm_id, r_act);

					     if(r_act.CompareNoCase("RETRY")    == 0)		return OBJ_RETRY;
					else if(r_act.CompareNoCase("COMPLETE") == 0)		return SEQ_COMPLETE;

					return OBJ_ABORT;
				}
				else
				{
					alarm_id = ALID__LP_E84A_ACTIVE_ABNORMAL_SIGNAL_OCCUR;
					
					p_alarm->Check__ALARM(ALID__LP_E84A_ACTIVE_ABNORMAL_SIGNAL_OCCUR, r_act);
					p_alarm->Popup__ALARM_With_MESSAGE(alarm_id, str_alarm_msg, r_act);

					     if(r_act.CompareNoCase("RETRY")    == 0)		return OBJ_RETRY;
					else if(r_act.CompareNoCase("COMPLETE") == 0)		return SEQ_COMPLETE;
					
					return OBJ_ABORT;
				}
			}
		}
		else if(dCH__PIO_RESET->Check__DATA(_ON) > 0)
		{
			return OBJ_ABORT;
		}
		
		// ...
		{
			if(Is__LT_BROKEN(p_variable, p_alarm) < 0)		return OBJ_ABORT;
			if(Is__Abort(p_variable) > 0)					return OBJ_ABORT;
		}

		if(Is__FOUP_LOGIC_ERROR(p_variable, p_alarm, rc_act) < 0)
		{
				 if(rc_act.CompareNoCase("RETRY")    == 0)	return OBJ_RETRY;
			else if(rc_act.CompareNoCase("COMPLETE") == 0)	return SEQ_COMPLETE;
			else											return OBJ_ABORT;

		}
		if(Is__FOUP_VIOLATION_ERROR(p_variable, p_alarm, "NONE") < 0)
		{
				 if(rc_act.CompareNoCase("RETRY")    == 0)	return OBJ_RETRY;
			else if(rc_act.CompareNoCase("COMPLETE") == 0)	return SEQ_COMPLETE;
			else											return OBJ_ABORT;

		}

		Wait_Time->POLL(dblCheck_Polling_time);		
	} 
	while(status < 0);
 	
	LP_TP_Timer->STOP();
	
	// 5. Check FOUP Sns : TP3, LREQ -> Off	
	sLog_Msg.Format("[E84] PIO_LOAD Start...(TP3:%d)", m_nTP3);
	Fnc__E84_LOG(sLog_Msg);

	status = -1;
	nCnt_Temp = 0;
	LP_TP_Timer->START__COUNT_DOWN((double)m_nTP3);

	do 
	{
		if(Is__FOUP_EXIST() > 0)
		{
			status = 1;

			sLog_Msg.Format("[E84] PIO_LOAD Complete...(FOUP EXIST)\n");
			Fnc__E84_LOG(sLog_Msg);
		}
		else if(LP_TP_Timer->Get__CURRENT_TIME() <= 0.1)
		{
			Set__AllOff_Except_ES();
			
			Fnc__E84_LOG(" * TP3 Timeout Occur, When Load Sequence.");

			// ...
			{
				int alarm_id = ALID__LP_E84A_TP3;
				CString r_act;

				p_alarm->Check__ALARM(ALID__LP_E84A_TP3, r_act);
				p_alarm->Popup__ALARM(alarm_id, r_act);

				     if(r_act.CompareNoCase("RETRY")    == 0)		return OBJ_RETRY;
				else if(r_act.CompareNoCase("COMPLETE") == 0)		return SEQ_COMPLETE;
				
				return OBJ_ABORT;
			}
		}
		else if(Active_Signal_Check(true, "TP3", str_alarm_msg,valid_chk) != OBJ_AVAILABLE)
		{
			nCnt_Temp++;
			if(nCnt_Temp > nCnt_Active_Signal_Abnormal)
			{
				nCnt_Temp = 0;
				Set__AllOff_Except_ES();
								
				Fnc__E84_LOG(" * Waiting TP3:FOUP Sensor Check 1, Active Abnormal Signal Alarm Occur !!.");

				int alarm_id;
				CString r_act;

				if(valid_chk.CompareNoCase("VALID_OFF") ==0)
				{
					alarm_id = ALID__LP_E84A_ACTIVE_VALID_ABNORMAL_ERROR_OCCUR;
					p_alarm->Check__ALARM(alarm_id, r_act);
					p_alarm->Popup__ALARM(alarm_id, r_act);

					     if(r_act.CompareNoCase("RETRY")    == 0)		return OBJ_RETRY;
					else if(r_act.CompareNoCase("COMPLETE") == 0)		return SEQ_COMPLETE;

					return OBJ_ABORT;
				}
				else
				{
					alarm_id = ALID__LP_E84A_ACTIVE_ABNORMAL_SIGNAL_OCCUR;
					
					p_alarm->Check__ALARM(ALID__LP_E84A_ACTIVE_ABNORMAL_SIGNAL_OCCUR, r_act);
					p_alarm->Popup__ALARM_With_MESSAGE(alarm_id, str_alarm_msg, r_act);

						 if(r_act.CompareNoCase("RETRY")    == 0)		return OBJ_RETRY;
					else if(r_act.CompareNoCase("COMPLETE") == 0)		return SEQ_COMPLETE;
					
					return OBJ_ABORT;
				}
			}
		}
		else if(dCH__PIO_RESET->Check__DATA(_ON) > 0)
		{
			return OBJ_ABORT;
		}

		// ...
		{
			if(Is__LT_BROKEN(p_variable, p_alarm) < 0)		return OBJ_ABORT;
			if(Is__Abort(p_variable) > 0)					return OBJ_ABORT;
		}

		if(Is__FOUP_LOGIC_ERROR(p_variable, p_alarm, rc_act) < 0)
		{
				 if(rc_act.CompareNoCase("RETRY")    == 0)	return OBJ_RETRY;
			else if(rc_act.CompareNoCase("COMPLETE") == 0)	return SEQ_COMPLETE;

			return OBJ_ABORT;
		}

		Wait_Time->POLL(dblCheck_Polling_time);
	} 
	while(status < 0);
	
	LP_TP_Timer->STOP();

	Set__LREQ("OFF");

	//6. Wait BUSY ->Off  and  COMPT -> On : TP4	
	sLog_Msg.Format("[E84] PIO_LOAD Start...(TP4:%d)", m_nTP4);
	Fnc__E84_LOG(sLog_Msg);

	status    = -1;
	nCnt_Temp = 0;
	LP_TP_Timer->START__COUNT_DOWN((double)m_nTP4);
	
	do 
	{
		if((Is__Sns_BUSY()  < 0)
		&& (Is__Sns_TRREQ() < 0)
		&& (Is__Sns_COMPT() > 0))
		{
			status = 1;
			sLog_Msg.Format("[E84] PIO_LOAD Complete...(TP4)\n");
			Fnc__E84_LOG(sLog_Msg);
		}
		else if(LP_TP_Timer->Get__CURRENT_TIME() <= 0.1)
		{
			Set__AllOff_Except_ES();

			Fnc__E84_LOG(" * TP4 Timeout Occur, When Load Sequence.");

			// ...
			{
				int alarm_id = ALID__LP_E84A_TP4;
				CString r_act;
				
				p_alarm->Check__ALARM(ALID__LP_E84A_TP4, r_act);
				p_alarm->Popup__ALARM(alarm_id, r_act);
				
					 if(r_act.CompareNoCase("RETRY")    == 0)		return OBJ_RETRY;
				else if(r_act.CompareNoCase("COMPLETE") == 0)		return SEQ_COMPLETE;
				
				return OBJ_ABORT;
			}
		}
		else if(Active_Signal_Check(true, "TP4", str_alarm_msg,valid_chk) != OBJ_AVAILABLE)
		{
			nCnt_Temp++;
			if(nCnt_Temp > nCnt_Active_Signal_Abnormal)
			{
				nCnt_Temp = 0;

				Set__AllOff_Except_ES();
								
				Fnc__E84_LOG(" * Waiting TP4:BUSY[OFF],TR_REQ[OFF],COMPT[ON], Active Abnormal Signal Alarm Occur !!.");

				int alarm_id;
				CString r_act;

				if(valid_chk.CompareNoCase("VALID_OFF") ==0)
				{
					alarm_id = ALID__LP_E84A_ACTIVE_VALID_ABNORMAL_ERROR_OCCUR;

					p_alarm->Check__ALARM(alarm_id, r_act);
					p_alarm->Popup__ALARM(alarm_id, r_act);

					     if(r_act.CompareNoCase("RETRY")    == 0)		return OBJ_RETRY;
					else if(r_act.CompareNoCase("COMPLETE") == 0)		return SEQ_COMPLETE;

					return OBJ_ABORT;
				}
				else
				{
					alarm_id = ALID__LP_E84A_ACTIVE_ABNORMAL_SIGNAL_OCCUR;

					p_alarm->Check__ALARM(ALID__LP_E84A_ACTIVE_ABNORMAL_SIGNAL_OCCUR, r_act);
					p_alarm->Popup__ALARM_With_MESSAGE(alarm_id, str_alarm_msg, r_act);

						 if(r_act.CompareNoCase("RETRY")    == 0)		return OBJ_RETRY;
					else if(r_act.CompareNoCase("COMPLETE") == 0)		return SEQ_COMPLETE;
					
					return OBJ_ABORT;
				}
			}
		}
		else if(dCH__PIO_RESET->Check__DATA(_ON) > 0)
		{
			return OBJ_ABORT;
		}
		
		// ...
		{
			if(Is__LT_BROKEN(p_variable, p_alarm) < 0)		return OBJ_ABORT;
			if(Is__Abort(p_variable) > 0)					return OBJ_ABORT;
		}

		if(Is__FOUP_LOGIC_ERROR(p_variable, p_alarm, rc_act) < 0)
		{
				 if(rc_act.CompareNoCase("RETRY")    == 0)	return OBJ_RETRY;
			else if(rc_act.CompareNoCase("COMPLETE") == 0)	return SEQ_COMPLETE;
		
			return OBJ_ABORT;
		}
		if(Is__FOUP_VIOLATION_ERROR(p_variable, p_alarm, "EXIST") < 0)
		{
				 if(rc_act.CompareNoCase("RETRY")    == 0)	return OBJ_RETRY;
			else if(rc_act.CompareNoCase("COMPLETE") == 0)	return SEQ_COMPLETE;
			
			return OBJ_ABORT;
		}

		Wait_Time->POLL(dblCheck_Polling_time);			
	} 
	while(status < 0);
 	
	LP_TP_Timer->STOP();

	//7. READY -> Off
	Set__READY("OFF");

	//8. Wait CS0, VALID, COMPT -> Off : TP5	
	sLog_Msg.Format("[E84] PIO_LOAD Start...(TP5:%d)", m_nTP5);
	Fnc__E84_LOG(sLog_Msg);

	status    = -1;
	nCnt_Temp = 0;
	LP_TP_Timer->START__COUNT_DOWN((double)m_nTP5);	
	
	do 
	{
		if( (Is__Sns_CS0()   < 0) 
		&&  (Is__Sns_VALID() < 0)
		&&  (Is__Sns_COMPT() < 0) )
		{
			status = 1;
			sLog_Msg.Format("[E84] PIO_LOAD Complete...(TP5)\n");
			Fnc__E84_LOG(sLog_Msg);
		}
		else if(LP_TP_Timer->Get__CURRENT_TIME() <= 0.1)
		{
			Set__AllOff_Except_ES();

			Fnc__E84_LOG(" * TP5 Timeout Occur, When Load Sequence.");

			// ...
			{
				int alarm_id = ALID__LP_E84A_TP5;
				CString r_act;
				
				p_alarm->Check__ALARM(ALID__LP_E84A_TP5, r_act);
				p_alarm->Popup__ALARM(alarm_id, r_act);

				     if(r_act.CompareNoCase("RETRY")    == 0)		return OBJ_RETRY;
				else if(r_act.CompareNoCase("COMPLETE") == 0)		return SEQ_COMPLETE;
				
				return OBJ_ABORT;
			}
		}
		else if(Active_Signal_Check(true, "TP5", str_alarm_msg,valid_chk) != OBJ_AVAILABLE)
		{
			nCnt_Temp++;
			if(nCnt_Temp > nCnt_Active_Signal_Abnormal)
			{
				nCnt_Temp = 0;

				Set__AllOff_Except_ES();
								
				Fnc__E84_LOG(" * Waiting TP5:CS0[OFF],VALID[OFF],COMPT[OFF], Active Abnormal Signal Alarm Occur !!.");

				// ...
				int alarm_id;
				CString r_act;

				if(valid_chk.CompareNoCase("VALID_OFF") ==0)
				{
					alarm_id = ALID__LP_E84A_ACTIVE_VALID_ABNORMAL_ERROR_OCCUR;

					p_alarm->Check__ALARM(alarm_id, r_act);
					p_alarm->Popup__ALARM(alarm_id, r_act);

					     if(r_act.CompareNoCase("RETRY")    == 0)		return OBJ_RETRY;
					else if(r_act.CompareNoCase("COMPLETE") == 0)		return SEQ_COMPLETE;
					
					return OBJ_ABORT;
				}
				else
				{
					alarm_id = ALID__LP_E84A_ACTIVE_ABNORMAL_SIGNAL_OCCUR;
					
					p_alarm->Check__ALARM(ALID__LP_E84A_ACTIVE_ABNORMAL_SIGNAL_OCCUR, r_act);
					p_alarm->Popup__ALARM_With_MESSAGE(alarm_id, str_alarm_msg, r_act);

					     if(r_act.CompareNoCase("RETRY")    == 0)		return OBJ_RETRY;
					else if(r_act.CompareNoCase("COMPLETE") == 0)		return SEQ_COMPLETE;
					
					return OBJ_ABORT;
				}
			}
		}
		else if(dCH__PIO_RESET->Check__DATA(_ON) > 0)
		{
			return OBJ_ABORT;
		}
		
		// ...
		{
			if(Is__LT_BROKEN(p_variable, p_alarm) < 0)		return OBJ_ABORT;
			if(Is__Abort(p_variable) > 0)					return OBJ_ABORT;
		}

		if(Is__FOUP_LOGIC_ERROR(p_variable, p_alarm, rc_act) < 0)
		{
				 if(rc_act.CompareNoCase("RETRY")    == 0)	return OBJ_RETRY;
			else if(rc_act.CompareNoCase("COMPLETE") == 0)	return SEQ_COMPLETE;
			
			return OBJ_ABORT;
		}
		if(Is__FOUP_VIOLATION_ERROR(p_variable, p_alarm, "EXIST") < 0)
		{
				 if(rc_act.CompareNoCase("RETRY")    == 0)	return OBJ_RETRY;
			else if(rc_act.CompareNoCase("COMPLETE") == 0)	return SEQ_COMPLETE;
			
			return OBJ_ABORT;
		}

		Wait_Time->POLL(dblCheck_Polling_time);		
	} 
	while(status < 0);
 	
	LP_TP_Timer->STOP();

	dEXT_CH__LINK_PIO_TRANSFER_STATE->Set__DATA(_NO);

	// ...
	{
		if(status < 0)			sMsg = "LOAD Failed..";
		else					sMsg = "LOAD Completed..";
	
		Fnc__APP_MSG(sMsg);	Fnc__E84_LOG(sMsg);
	}
	return SEQ_COMPLETE;
}

int  CObj__OHT_E84_IO::
Fnc__UNLOAD_PIO(CII_OBJECT__VARIABLE* p_variable,
				CII_OBJECT__ALARM* p_alarm)
{
	CString sMsg;

	Fnc__E84_LOG("\n");
	sMsg.Format("UNLOAD Starting ...");
	Fnc__APP_MSG(sMsg);	Fnc__E84_LOG(sMsg);

	if(Is__ONLINE(p_variable, p_alarm) < 0)
	{
		return -1;
	}

	// ...
	{
		int loop_count = 15;

		while(loop_count > 0)
		{
			if(p_variable->Check__CTRL_ABORT() > 0)
			{
				return -1;
			}

			if(dEXT_CH__LPx_INFO__CLAMP_STATE->Check__DATA(_UNCLAMP) > 0)
			{
				break;
			}

			loop_count--;
			Sleep(100);	
		}
	}

	// ...
	CString data;
	CString str_alarm_msg;
	int status, TP_temp = -1;
	int retCode = -1;
	int tp_timer_ret      = 0;
	CString sLog_Msg;
	CString rc_act;

	double dblChattering_time = 0.0;
	double dblCheck_Polling_time = 0.05;	// 0.05 초...
	int nCnt_Active_Signal_Abnormal = 0;
	int nCnt_Temp = 0;
	
	SCX__TIMER_CTRL Wait_Time;
	SCX__ASYNC_TIMER_CTRL LP_TP_Timer;

	tp_timer_ret = LP_TP_Timer->REGISTER__COUNT_CHANNEL(sObject_Name, sCH__INR_TP_TIMER_COUNTER->Get__VARIABLE_NAME());
	LP_TP_Timer->STOP();

	aCH__CFG_PIO_SIGNAL_CHATTERING_TIME->Get__DATA(data);
	dblChattering_time = atof(data);

	sLog_Msg.Format("[E84] PIO_UNLOAD Start...(Register:TP_TIMER ret:%d)", tp_timer_ret);
	Fnc__E84_LOG(sLog_Msg);
 
 	LP_TP_Timer->STOP();

	// ...
	CString valid_chk;
	
// 	1. UREQ -> On , Run UnClamp
// 	2. Wait TR_REQ On : TP1
// 	3. Check Unclamp , READY -> On
// 	4. Wait Busy On : TP2
// 	5. Check FOUP Sns : TP3, UREQ -> Off
// 	6. Wait BUSY Off and COMPT -> On : TP4
// 	7. READY -> Off
// 	8. Wait CS0, VALID, COMPT -> Off : TP5

	// Before U_REQ set.. Signal Check..
	nCnt_Temp = 0;

	do 
	{
		if(Active_Signal_Check(false, "START", str_alarm_msg,valid_chk) != OBJ_AVAILABLE)
		{
			nCnt_Temp++;
			if(nCnt_Temp > nCnt_Active_Signal_Abnormal)
			{
				nCnt_Temp = 0;

				Set__AllOff_Except_ES();
								
				Fnc__E84_LOG(" * Starting Before Set U_REQ[ON], Active Abnormal Signal Alarm Occur !!.");

				// ...
				int alarm_id;
				CString r_act;

				if(valid_chk.CompareNoCase("VALID_OFF") ==0)
				{
					alarm_id = ALID__LP_E84A_ACTIVE_VALID_ABNORMAL_ERROR_OCCUR;
					
					p_alarm->Check__ALARM(alarm_id, r_act);
					p_alarm->Popup__ALARM(alarm_id, r_act);

					     if(r_act.CompareNoCase("RETRY")    == 0)		return OBJ_RETRY;
					else if(r_act.CompareNoCase("COMPLETE") == 0)		return SEQ_COMPLETE;
					
					return OBJ_ABORT;
				}
				else
				{
					alarm_id = ALID__LP_E84A_ACTIVE_ABNORMAL_SIGNAL_OCCUR;

					p_alarm->Check__ALARM(ALID__LP_E84A_ACTIVE_ABNORMAL_SIGNAL_OCCUR, r_act);
					p_alarm->Popup__ALARM_With_MESSAGE(alarm_id, str_alarm_msg, r_act);

						 if(r_act.CompareNoCase("RETRY")    == 0)		return OBJ_RETRY;
					else if(r_act.CompareNoCase("COMPLETE") == 0)		return SEQ_COMPLETE;
					
					return OBJ_ABORT;
				}
			}
		}
		else if(dCH__PIO_RESET->Check__DATA(_ON) > 0)
		{
			return OBJ_ABORT;
		}
		else
		{
			break;
		}
		
		// ...
		{
			if(Is__LT_BROKEN(p_variable, p_alarm) < 0)		return OBJ_ABORT;
			if(Is__Abort(p_variable) > 0)					return OBJ_ABORT;
		}

		if(Is__FOUP_LOGIC_ERROR(p_variable, p_alarm, rc_act) < 0)
		{
				 if(rc_act.CompareNoCase("RETRY")    == 0)	return OBJ_RETRY;
			else if(rc_act.CompareNoCase("COMPLETE") == 0)	return SEQ_COMPLETE;

			return OBJ_ABORT;
		}
		if(Is__FOUP_VIOLATION_ERROR(p_variable, p_alarm, "EXIST") < 0)
		{
				 if(rc_act.CompareNoCase("RETRY")    == 0)	return OBJ_RETRY;
			else if(rc_act.CompareNoCase("COMPLETE") == 0)	return SEQ_COMPLETE;

			return OBJ_ABORT;
		}

		Wait_Time->POLL(dblCheck_Polling_time);			
	} 
	while(1);

	//1. UREQ -> On
	Set__UREQ("ON");
	
	//2. Wait TR_REQ -> On : TP1
	sLog_Msg.Format("[E84] PIO_UNLOAD Start...(TP1:%d)", m_nTP1);
	Fnc__E84_LOG(sLog_Msg);

	status = -1;
	nCnt_Temp = 0;
	LP_TP_Timer->START__COUNT_DOWN((double)m_nTP1);

	do 
	{
		if(Is__Sns_TRREQ() > 0)
		{
			status = 1;
			
			sLog_Msg.Format("[E84] PIO_UNLOAD Complete...(TP1)\n");
			Fnc__E84_LOG(sLog_Msg);
		}
		else if(LP_TP_Timer->Get__CURRENT_TIME() <= 0.1)
		{
			Set__AllOff_Except_ES();
								
			Fnc__E84_LOG(" * TP1 Timeout Occur, When Load Sequence.");

			// ...
			{
				int alarm_id = ALID__LP_E84A_TP1_UNLOAD;
				CString r_act;
				p_alarm->Check__ALARM(ALID__LP_E84A_TP1_UNLOAD, r_act);
				p_alarm->Popup__ALARM(alarm_id, r_act);

				if(r_act.CompareNoCase("RETRY") == 0)			return OBJ_RETRY;
				else if(r_act.CompareNoCase("COMPLETE") == 0)	return SEQ_COMPLETE;

				return OBJ_ABORT;
			}
		}
		else if(Active_Signal_Check(false, "TP1", str_alarm_msg,valid_chk) != OBJ_AVAILABLE)
		{
			nCnt_Temp++;

			if(nCnt_Temp > nCnt_Active_Signal_Abnormal)
			{
				nCnt_Temp = 0;
				Set__AllOff_Except_ES();
									
				Fnc__E84_LOG(" * Waiting TP1:TR_REQ[ON], Active Abnormal Signal Alarm Occur !!.");

				// ...
				int alarm_id;
				CString r_act;

				if(valid_chk.CompareNoCase("VALID_OFF") ==0)
				{
					alarm_id = ALID__LP_E84A_ACTIVE_VALID_ABNORMAL_ERROR_OCCUR;
					
					p_alarm->Check__ALARM(alarm_id, r_act);
					p_alarm->Popup__ALARM(alarm_id, r_act);

						 if(r_act.CompareNoCase("RETRY")    == 0)		return OBJ_RETRY;
					else if(r_act.CompareNoCase("COMPLETE") == 0)		return SEQ_COMPLETE;

					return OBJ_ABORT;
				}
				else
				{
					alarm_id = ALID__LP_E84A_ACTIVE_ABNORMAL_SIGNAL_OCCUR;

					p_alarm->Check__ALARM(ALID__LP_E84A_ACTIVE_ABNORMAL_SIGNAL_OCCUR, r_act);
					p_alarm->Popup__ALARM_With_MESSAGE(alarm_id, str_alarm_msg, r_act);

						 if(r_act.CompareNoCase("RETRY")    == 0)		return OBJ_RETRY;
					else if(r_act.CompareNoCase("COMPLETE") == 0)		return SEQ_COMPLETE;

					return OBJ_ABORT;
				}
			}
		}
		else if(dCH__PIO_RESET->Check__DATA(_ON) > 0)
		{
			return OBJ_ABORT;
		}
			
		// ...
		{
			if(Is__LT_BROKEN(p_variable, p_alarm) < 0)		return OBJ_ABORT;
			if(Is__Abort(p_variable) > 0)					return OBJ_ABORT;
		}

		if(Is__FOUP_LOGIC_ERROR(p_variable, p_alarm, rc_act) < 0)
		{
				 if(rc_act.CompareNoCase("RETRY")    == 0)	return OBJ_RETRY;
			else if(rc_act.CompareNoCase("COMPLETE") == 0)	return SEQ_COMPLETE;
			
			return OBJ_ABORT;
		}
		if(Is__FOUP_VIOLATION_ERROR(p_variable, p_alarm, "EXIST") < 0)
		{
				 if(rc_act.CompareNoCase("RETRY")    == 0)	return OBJ_RETRY;
			else if(rc_act.CompareNoCase("COMPLETE") == 0)	return SEQ_COMPLETE;
			
			return OBJ_ABORT;
		}

		Wait_Time->POLL(dblCheck_Polling_time);
	} 
	while(status < 0);
 	
	LP_TP_Timer->STOP();

	// Check the UNCLAMP
    // 3. Check Unclamp , READY -> On  
	if(dEXT_CH__LPx_INFO__CLAMP_STATE->Check__DATA(_UNCLAMP) < 0)
	{
		Set__HOAVBL("OFF");
		
		Fnc__E84_LOG(" * Clamp Error Occur, When Unload Sequence Now Clamped Status.");

		// ...
		{
			int alarm_id = ALID__LP_E84A_UNLOAD_UNCLAMP_ERR;
			CString r_act;

			p_alarm->Check__ALARM(ALID__LP_E84A_UNLOAD_UNCLAMP_ERR, r_act);
			p_alarm->Popup__ALARM(alarm_id, r_act);

				 if(r_act.CompareNoCase("RETRY")    == 0)		return OBJ_RETRY;
			else if(r_act.CompareNoCase("COMPLETE") == 0)		return SEQ_COMPLETE;

			return OBJ_ABORT;
		}
	}
	else
	{
		//3. READY -> On
		Set__READY("ON");
	}

	//4. Wait Busy -> On : TP2
	sLog_Msg.Format("[E84] PIO_UNLOAD Start...(TP2:%d)", m_nTP2);
	Fnc__E84_LOG(sLog_Msg);

	status = -1;
	nCnt_Temp = 0;
	LP_TP_Timer->START__COUNT_DOWN((double)m_nTP2);
	
	do 
	{
		if(Is__Sns_BUSY() > 0)
		{
			status = 1;
			sLog_Msg.Format("[E84] PIO_UNLOAD Complete...(TP2)\n");
			Fnc__E84_LOG(sLog_Msg);
		}
		else if(LP_TP_Timer->Get__CURRENT_TIME() <= 0.1)
		{
			Set__AllOff_Except_ES();
				
			Fnc__E84_LOG(" * TP2 Timeout Occur, When Load Sequence.");

			// ...
			{
				int alarm_id = ALID__LP_E84A_TP2_UNLOAD;
				CString r_act;

				p_alarm->Check__ALARM(ALID__LP_E84A_TP2_UNLOAD, r_act);
				p_alarm->Popup__ALARM(alarm_id, r_act);

					 if(r_act.CompareNoCase("RETRY")    == 0)		return OBJ_RETRY;
				else if(r_act.CompareNoCase("COMPLETE") == 0)		return SEQ_COMPLETE;

				return OBJ_ABORT;
			}
		}
		else if(Active_Signal_Check(false, "TP2", str_alarm_msg,valid_chk) != OBJ_AVAILABLE)
		{
			nCnt_Temp++;

			if(nCnt_Temp > nCnt_Active_Signal_Abnormal)
			{
				nCnt_Temp = 0;

				Set__AllOff_Except_ES();
									
				Fnc__E84_LOG(" * Waiting TP2:BUSY[ON], Active Abnormal Signal Alarm Occur !!.");

				// ...
				int alarm_id;
				CString r_act;

				if(valid_chk.CompareNoCase("VALID_OFF") ==0)
				{
					alarm_id = ALID__LP_E84A_ACTIVE_VALID_ABNORMAL_ERROR_OCCUR;

					p_alarm->Check__ALARM(alarm_id, r_act);
					p_alarm->Popup__ALARM(alarm_id, r_act);

					if(r_act.CompareNoCase("RETRY") == 0)			return OBJ_RETRY;
					else if(r_act.CompareNoCase("COMPLETE") == 0)	return SEQ_COMPLETE;
					
					return OBJ_ABORT;
				}
				else
				{
					alarm_id = ALID__LP_E84A_ACTIVE_ABNORMAL_SIGNAL_OCCUR;
					
					p_alarm->Check__ALARM(ALID__LP_E84A_ACTIVE_ABNORMAL_SIGNAL_OCCUR, r_act);
					p_alarm->Popup__ALARM_With_MESSAGE(alarm_id, str_alarm_msg, r_act);

					if(r_act.CompareNoCase("RETRY") == 0)			return OBJ_RETRY;
					else if(r_act.CompareNoCase("COMPLETE") == 0)	return SEQ_COMPLETE;
					
					return OBJ_ABORT;
				}
			}
		}
		else if(dCH__PIO_RESET->Check__DATA(_ON) > 0)
		{
			return OBJ_ABORT;
		}
		
		// ...
		{
			if(Is__LT_BROKEN(p_variable, p_alarm) < 0)		return OBJ_ABORT;
			if(Is__Abort(p_variable) > 0)					return OBJ_ABORT;
		}

		if(Is__FOUP_LOGIC_ERROR(p_variable, p_alarm, rc_act) < 0)
		{
				 if(rc_act.CompareNoCase("RETRY")    == 0)	return OBJ_RETRY;
			else if(rc_act.CompareNoCase("COMPLETE") == 0)	return SEQ_COMPLETE;
			
			return OBJ_ABORT;
		}
		if(Is__FOUP_VIOLATION_ERROR(p_variable, p_alarm, "EXIST") < 0)
		{
				 if(rc_act.CompareNoCase("RETRY")    == 0)	return OBJ_RETRY;
			else if(rc_act.CompareNoCase("COMPLETE") == 0)	return SEQ_COMPLETE;
			
			return OBJ_ABORT;
		}

		Wait_Time->POLL(dblCheck_Polling_time);
	} 
	while(status < 0);
 	
	LP_TP_Timer->STOP();

	//5. Check FOUP Sns : TP3, UREQ -> Off	
	sLog_Msg.Format("[E84] PIO_UNLOAD Start...(TP3:%d)", m_nTP3);
	Fnc__E84_LOG(sLog_Msg);

	status = -1;
	nCnt_Temp = 0;
	LP_TP_Timer->START__COUNT_DOWN((double)m_nTP3);

	do 
	{
		if( Is__FOUP_NONE() > 0)
		{
			status = 1;
			sLog_Msg.Format("[E84] PIO_UNLOAD Complete...(TP3)\n");
			Fnc__E84_LOG(sLog_Msg);
		}
		else if(LP_TP_Timer->Get__CURRENT_TIME() <= 0.1)
		{
			Set__AllOff_Except_ES();

			Fnc__E84_LOG(" * TP3 Timeout Occur, When Unload Sequence.");

			// ...
			{
				int alarm_id = ALID__LP_E84A_TP3_UNLOAD;
				CString r_act;

				p_alarm->Check__ALARM(ALID__LP_E84A_TP3_UNLOAD, r_act);
				p_alarm->Popup__ALARM(alarm_id, r_act);

					 if(r_act.CompareNoCase("RETRY")    == 0)		return OBJ_RETRY;
				else if(r_act.CompareNoCase("COMPLETE") == 0)		return SEQ_COMPLETE;
				
				return OBJ_ABORT;
			}
		}
		else if(Active_Signal_Check(false, "TP3", str_alarm_msg,valid_chk) != OBJ_AVAILABLE)
		{
			nCnt_Temp++;
			if(nCnt_Temp > nCnt_Active_Signal_Abnormal)
			{
				nCnt_Temp = 0;

				Set__AllOff_Except_ES();
								
				Fnc__E84_LOG(" * Waiting TP3:FOUP Sensor Check 1, Active Abnormal Signal Alarm Occur !!.");

				// ...
				int alarm_id;
				CString r_act;

				if(valid_chk.CompareNoCase("VALID_OFF") ==0)
				{
					alarm_id = ALID__LP_E84A_ACTIVE_VALID_ABNORMAL_ERROR_OCCUR;
					p_alarm->Check__ALARM(alarm_id, r_act);
					p_alarm->Popup__ALARM(alarm_id, r_act);

						 if(r_act.CompareNoCase("RETRY")    == 0)		return OBJ_RETRY;
					else if(r_act.CompareNoCase("COMPLETE") == 0)		return SEQ_COMPLETE;

					return OBJ_ABORT;
				}
				else
				{
					int alarm_id = ALID__LP_E84A_ACTIVE_ABNORMAL_SIGNAL_OCCUR;
					CString r_act;

					p_alarm->Check__ALARM(ALID__LP_E84A_ACTIVE_ABNORMAL_SIGNAL_OCCUR, r_act);
					p_alarm->Popup__ALARM_With_MESSAGE(alarm_id, str_alarm_msg, r_act);

						 if(r_act.CompareNoCase("RETRY")    == 0)		return OBJ_RETRY;
					else if(r_act.CompareNoCase("COMPLETE") == 0)		return SEQ_COMPLETE;

					return OBJ_ABORT;
				}
			}
		}
		else if(dCH__PIO_RESET->Check__DATA(_ON) > 0)
		{
			return OBJ_ABORT;
		}
		
		// ...
		{
			if(Is__LT_BROKEN(p_variable, p_alarm) < 0)		return OBJ_ABORT;
			if(Is__Abort(p_variable) > 0)					return OBJ_ABORT;
		}
		
		if(Is__FOUP_LOGIC_ERROR(p_variable, p_alarm, rc_act) < 0)
		{
				 if(rc_act.CompareNoCase("RETRY")    == 0)	return OBJ_RETRY;
			else if(rc_act.CompareNoCase("COMPLETE") == 0)	return SEQ_COMPLETE;
			
			return OBJ_ABORT;
		}

		Wait_Time->POLL(dblCheck_Polling_time);
	} 
	while(status < 0);
	
	LP_TP_Timer->STOP();
	Set__UREQ("OFF");

	//6. Wait BUSY ->Off  and  COMPT -> On : TP4	
	status = -1;

	sLog_Msg.Format("[E84] PIO_UNLOAD Start...(TP4:%d)", m_nTP4);
	Fnc__E84_LOG(sLog_Msg);

	status = -1;
	nCnt_Temp = 0;
	LP_TP_Timer->START__COUNT_DOWN((double)m_nTP4);
	
	do 
	{
		if(Is__Sns_BUSY() < 0 
		&& Is__Sns_COMPT() > 0)
		{
			status = 1;

			sLog_Msg.Format("[E84] PIO_UNLOAD Complete...(TP4)\n");
			Fnc__E84_LOG(sLog_Msg);
		}
		else if(LP_TP_Timer->Get__CURRENT_TIME() <= 0.1)
		{
			Set__AllOff_Except_ES();

			Fnc__E84_LOG(" * TP4 Timeout Occur, When Load Sequence.");

			// ...
			{
				int alarm_id = ALID__LP_E84A_TP4_UNLOAD;
				CString r_act;
				
				p_alarm->Popup__ALARM(alarm_id, r_act);

				     if(r_act.CompareNoCase("RETRY")    == 0)		return OBJ_RETRY;
				else if(r_act.CompareNoCase("COMPLETE") == 0)		return SEQ_COMPLETE;
				
				return OBJ_ABORT;
			}
		}
		else if(Active_Signal_Check(false, "TP4", str_alarm_msg,valid_chk) != OBJ_AVAILABLE)
		{
			nCnt_Temp++;
			if(nCnt_Temp > nCnt_Active_Signal_Abnormal)
			{
				nCnt_Temp = 0;

				Set__AllOff_Except_ES();
									
				Fnc__E84_LOG(" * Waiting TP4:BUSY[OFF],TR_REQ[OFF],COMPT[ON], Active Abnormal Signal Alarm Occur !!.");

				// ...
				int alarm_id;
				CString r_act;

				if(valid_chk.CompareNoCase("VALID_OFF") ==0)
				{
					alarm_id = ALID__LP_E84A_ACTIVE_VALID_ABNORMAL_ERROR_OCCUR;

					p_alarm->Check__ALARM(alarm_id, r_act);
					p_alarm->Popup__ALARM(alarm_id, r_act);

						 if(r_act.CompareNoCase("RETRY")    == 0)		return OBJ_RETRY;
					else if(r_act.CompareNoCase("COMPLETE") == 0)		return SEQ_COMPLETE;

					return OBJ_ABORT;
				}
				else
				{
					alarm_id = ALID__LP_E84A_ACTIVE_ABNORMAL_SIGNAL_OCCUR;

					p_alarm->Check__ALARM(ALID__LP_E84A_ACTIVE_ABNORMAL_SIGNAL_OCCUR, r_act);
					p_alarm->Popup__ALARM_With_MESSAGE(alarm_id, str_alarm_msg, r_act);

						 if(r_act.CompareNoCase("RETRY")    == 0)		return OBJ_RETRY;
					else if(r_act.CompareNoCase("COMPLETE") == 0)		return SEQ_COMPLETE;

					return OBJ_ABORT;
				}
			}
		}
		else if(dCH__PIO_RESET->Check__DATA(_ON) > 0)
		{
			return OBJ_ABORT;
		}

		// ...
		{
			if(Is__LT_BROKEN(p_variable, p_alarm) < 0)		return OBJ_ABORT;
			if(Is__Abort(p_variable) > 0)					return OBJ_ABORT;
		}
		
		if(Is__FOUP_LOGIC_ERROR(p_variable, p_alarm, rc_act) < 0)
		{
				 if(rc_act.CompareNoCase("RETRY")    == 0)	return OBJ_RETRY;
			else if(rc_act.CompareNoCase("COMPLETE") == 0)	return SEQ_COMPLETE;
			
			return OBJ_ABORT;
		}
			
		Wait_Time->POLL(dblCheck_Polling_time);
	} 
	while(status < 0);
 	
	LP_TP_Timer->STOP();

	if(Is__FOUP_VIOLATION_ERROR(p_variable, p_alarm, "NONE") < 0)
	{
		return OBJ_ABORT;
	}

	//7. READY -> Off
	Set__READY("OFF");

	//8. Wait CS0, VALID, COMPT -> Off : TP5	
	sLog_Msg.Format("[E84] PIO_UNLOAD Start...(TP5:%d)", m_nTP5);
	Fnc__E84_LOG(sLog_Msg);

	status = -1;
	nCnt_Temp = 0;
	LP_TP_Timer->START__COUNT_DOWN((double)m_nTP5);	
	
	do 
	{
		if(Is__Sns_CS0()   < 0 
		&& Is__Sns_VALID() < 0
		&& Is__Sns_COMPT() < 0)
		{
			status = 1;
			sLog_Msg.Format("[E84] PIO_UNLOAD Complete...(TP5)\n");
			Fnc__E84_LOG(sLog_Msg);
		}
		else if(LP_TP_Timer->Get__CURRENT_TIME() <= 0.1)
		{
			Set__AllOff_Except_ES();

			Fnc__E84_LOG(" * TP5 Timeout Occur, When Load Sequence.");

			// ...
			{
				int alarm_id = ALID__LP_E84A_TP5_UNLOAD;
				CString r_act;

				p_alarm->Check__ALARM(ALID__LP_E84A_TP5_UNLOAD, r_act);
				p_alarm->Popup__ALARM(alarm_id, r_act);

					 if(r_act.CompareNoCase("RETRY")    == 0)		return OBJ_RETRY;
				else if(r_act.CompareNoCase("COMPLETE") == 0)		return SEQ_COMPLETE;
				
				return OBJ_ABORT;
			}
		}
		else if(Active_Signal_Check(false, "TP5", str_alarm_msg,valid_chk) != OBJ_AVAILABLE)
		{
			nCnt_Temp++;
			if(nCnt_Temp > nCnt_Active_Signal_Abnormal)
			{
				nCnt_Temp = 0;

				Set__AllOff_Except_ES();
									
				Fnc__E84_LOG(" * Waiting TP5:CS0[OFF],VALID[OFF],COMPT[OFF], Active Abnormal Signal Alarm Occur !!.");

				// ...
				int alarm_id;
				CString r_act;

				if(valid_chk.CompareNoCase("VALID_OFF") ==0)
				{
					alarm_id = ALID__LP_E84A_ACTIVE_VALID_ABNORMAL_ERROR_OCCUR;
					p_alarm->Check__ALARM(alarm_id, r_act);
					p_alarm->Popup__ALARM(alarm_id, r_act);

						 if(r_act.CompareNoCase("RETRY")    == 0)		return OBJ_RETRY;
					else if(r_act.CompareNoCase("COMPLETE") == 0)		return SEQ_COMPLETE;
					
					return OBJ_ABORT;
				}
				else
				{
					alarm_id = ALID__LP_E84A_ACTIVE_ABNORMAL_SIGNAL_OCCUR;
					p_alarm->Check__ALARM(ALID__LP_E84A_ACTIVE_ABNORMAL_SIGNAL_OCCUR, r_act);
					p_alarm->Popup__ALARM_With_MESSAGE(alarm_id, str_alarm_msg, r_act);

						 if(r_act.CompareNoCase("RETRY")    == 0)		return OBJ_RETRY;
					else if(r_act.CompareNoCase("COMPLETE") == 0)		return SEQ_COMPLETE;

					return OBJ_ABORT;
				}
			}
		}
		else if(dCH__PIO_RESET->Check__DATA(_ON) > 0)
		{
			return OBJ_ABORT;
		}

		// ...
		{
			if(Is__LT_BROKEN(p_variable, p_alarm) < 0)		return OBJ_ABORT;
			if(Is__Abort(p_variable) > 0)					return OBJ_ABORT;
		}
		
		if(Is__FOUP_LOGIC_ERROR(p_variable, p_alarm, rc_act) < 0)
		{
				 if(rc_act.CompareNoCase("RETRY")    == 0)	return OBJ_RETRY;
			else if(rc_act.CompareNoCase("COMPLETE") == 0)	return SEQ_COMPLETE;
			
			return OBJ_ABORT;
		}
		if(Is__FOUP_VIOLATION_ERROR(p_variable, p_alarm, "NONE") < 0)
		{
				 if(rc_act.CompareNoCase("RETRY")    == 0)	return OBJ_RETRY;
			else if(rc_act.CompareNoCase("COMPLETE") == 0)	return SEQ_COMPLETE;
			
			return OBJ_ABORT;
		}

		Wait_Time->POLL(dblCheck_Polling_time);
	} 
	while(status < 0);

 	LP_TP_Timer->STOP();

	dEXT_CH__LINK_PIO_TRANSFER_STATE->Set__DATA(_NO);

	// ...
	{
		if(status < 0)		sMsg = "UNLOAD Failed..";
		else				sMsg = "UNLOAD Completed..";
	
		Fnc__APP_MSG(sMsg);	Fnc__E84_LOG(sMsg);
	}
	return SEQ_COMPLETE;
}

int CObj__OHT_E84_IO::
Set__LREQ(const CString& ch_data)
{
	if(ch_data.CompareNoCase(_OFF) == 0) 		doEXT_CH__LPx__E84_L_REQ->Set__DATA(_OFF);
	else										doEXT_CH__LPx__E84_L_REQ->Set__DATA(_ON);
	
	return 1;
}

int CObj__OHT_E84_IO::
Is__Sns_TRREQ()
{
	return diEXT_CH__LPx__E84_TR_REQ->Check__DATA(_ON);
}

int CObj__OHT_E84_IO::
Is__Sns_BUSY()
{
	return diEXT_CH__LPx__E84_BUSY->Check__DATA(_ON);
}

int CObj__OHT_E84_IO::
Active_Signal_Check(const bool active__load_seq,
					const CString tp_time, 
					CString& str_log, 
					CString& valid_chk)
{
	if(iActive__SIM_MODE > 0)
	{
		if(tp_time.CompareNoCase("START") == 0)			// CS0[ON], VALID[ON] <--> Before Set... L_REQ[ON]
		{
			// CS0    "ON"
			// VALID  "ON"
			// TR_REQ "OFF"
			// BUSY   "OFF"
			// COMPT  "OFF"

			diEXT_CH__LPx__E84_CS_0->Set__DATA(_ON);
			diEXT_CH__LPx__E84_VALID->Set__DATA(_ON);
			diEXT_CH__LPx__E84_TR_REQ->Set__DATA(_OFF);
			diEXT_CH__LPx__E84_BUSY->Set__DATA(_OFF);
			diEXT_CH__LPx__E84_COMPT->Check__DATA(_OFF);
		}
		else if(tp_time.CompareNoCase("TP1") == 0)		// L_REQ[ON] <--> TR_REQ[ON]
		{
			// CS0    "ON"
			// VALID  "ON"
			// BUSY   "OFF"
			// COMPT  "OFF"

			diEXT_CH__LPx__E84_CS_0->Set__DATA(_ON);
			diEXT_CH__LPx__E84_VALID->Set__DATA(_ON);
			diEXT_CH__LPx__E84_BUSY->Set__DATA(_OFF);
			diEXT_CH__LPx__E84_COMPT->Check__DATA(_OFF);

			diEXT_CH__LPx__E84_TR_REQ->Set__DATA(_OFF);
			Sleep(1000);
			diEXT_CH__LPx__E84_TR_REQ->Set__DATA(_ON);
		}
		else if(tp_time.CompareNoCase("TP2") == 0)		// READY[ON] <--> BUSY[ON]
		{
			// CS0    "ON"
			// VALID  "ON"
			// TR_REQ "ON"
			// COMPT  "OFF"

			diEXT_CH__LPx__E84_CS_0->Set__DATA(_ON);
			diEXT_CH__LPx__E84_VALID->Set__DATA(_ON);
			diEXT_CH__LPx__E84_TR_REQ->Set__DATA(_ON);
			diEXT_CH__LPx__E84_COMPT->Check__DATA(_OFF);

			diEXT_CH__LPx__E84_BUSY->Set__DATA(_OFF);
			Sleep(1000);
			diEXT_CH__LPx__E84_BUSY->Set__DATA(_ON);
		}
		else if(tp_time.CompareNoCase("TP3") == 0)		// FOUP Sensor Check
		{
			// CS0    "ON"
			// VALID  "ON"
			// TR_REQ "ON"
			// BUSY   "ON"
			// COMPT  "OFF"

			diEXT_CH__LPx__E84_CS_0->Set__DATA(_ON);
			diEXT_CH__LPx__E84_VALID->Set__DATA(_ON);
			diEXT_CH__LPx__E84_TR_REQ->Set__DATA(_ON);
			diEXT_CH__LPx__E84_BUSY->Set__DATA(_ON);
			diEXT_CH__LPx__E84_COMPT->Check__DATA(_OFF);

			Sleep(3000);

			if(active__load_seq)
			{
				dEXT_CH__LPx_IO__FOUP_PLACED_DI->Set__DATA(_ON);
				dEXT_CH__LPx_IO__FOUP_PRESENT_DI->Set__DATA(_ON);
			}
			else
			{
				dEXT_CH__LPx_IO__FOUP_PLACED_DI->Set__DATA(_OFF);
				dEXT_CH__LPx_IO__FOUP_PRESENT_DI->Set__DATA(_OFF);
			}
		}
		else if(tp_time.CompareNoCase("TP4") == 0)		// L_REQ[OFF] <--> TR_REQ[OFF], BUSY[OFF], COMPT[ON]
		{
			// CS0    "ON"
			// VALID  "ON"
			// TR_REQ "OFF"
			// BUSY   "OFF"
			// COMPT  "ON"

			diEXT_CH__LPx__E84_CS_0->Set__DATA(_ON);
			diEXT_CH__LPx__E84_VALID->Set__DATA(_ON);
			diEXT_CH__LPx__E84_TR_REQ->Set__DATA(_OFF);

			Sleep(1000);

			diEXT_CH__LPx__E84_BUSY->Set__DATA(_OFF);
			diEXT_CH__LPx__E84_COMPT->Set__DATA(_ON);
		}
		else if(tp_time.CompareNoCase("TP5") == 0)		// READY[OFF] <--> CS0[OFF], VALID[OFF], COMPT[OFF]
		{
			// CS0    "OFF"
			// VALID  "OFF"
			// TR_REQ "OFF"
			// BUSY   "OFF"
			// COMPT  "OFF"

			Sleep(1000);

			diEXT_CH__LPx__E84_CS_0->Set__DATA(_OFF);
			diEXT_CH__LPx__E84_VALID->Set__DATA(_OFF);
			diEXT_CH__LPx__E84_TR_REQ->Set__DATA(_OFF);
			diEXT_CH__LPx__E84_BUSY->Set__DATA(_OFF);
			diEXT_CH__LPx__E84_COMPT->Set__DATA(_OFF);
		}
	}

	// ...
	CString str_log_temp;

	int nCS0   = Is__Sns_CS0();
	int nVALID = Is__Sns_VALID();
	int nTR_REQ= Is__Sns_TRREQ();
	int nBUSY  = Is__Sns_BUSY();
	int nCOMPT = Is__Sns_COMPT();

	valid_chk = "OK";
	sCH__CUR_CHECK_TPx->Set__DATA(tp_time);

	if(tp_time.CompareNoCase("START") == 0)		// CS0[ON], VALID[ON] <--> Before Set... L_REQ[ON]
	{
		// CS0    "ON"
		// VALID  "ON"
		// TR_REQ "OFF"
		// BUSY   "OFF"
		// COMPT  "OFF"

		if( (nCS0   > 0)	
		&&  (nVALID > 0)
		&&  (nTR_REQ< 0)
		&&  (nBUSY  < 0)	
		&&  (nCOMPT < 0) )
		{
			return OBJ_AVAILABLE;
		}

		// ...
		if(nVALID < 0)
		{
			valid_chk = "VALID_OFF";
		}
		else
		{
			valid_chk = "OK";
		}

		// Abnormal Signal !! 
		{
			str_log_temp.Format("Abnormal Signal Error: CS0[ON]&VALID[ON]<-->TR_REQ[ON] !!\n", tp_time);
			str_log = str_log_temp;

			str_log_temp.Format("Normal  :[CS0:1],[VALID:1],[TR_REQ:-1],[BUSY:-1],[COMPT:-1] \n");
			str_log += str_log_temp;
			
			str_log_temp.Format("Abnormal:[CS0:%d],[VALID:%d],[TR_REQ:%d],[BUSY:%d],[COMPT:%d] \n", 
				                 nCS0, nVALID, nTR_REQ, nBUSY, nCOMPT);
			str_log += str_log_temp;

			Fnc__E84_LOG(str_log);
		}
		return OBJ_ABORT;
	}
	
	if(tp_time.CompareNoCase("TP1") == 0)		// L_REQ[ON] <--> TR_REQ[ON]
	{
		// CS0    "ON"
		// VALID  "ON"
		// BUSY   "OFF"
		// COMPT  "OFF"
		if( (nCS0   > 0)	
		&&  (nVALID > 0)	
		&&  (nBUSY  < 0)	
		&&  (nCOMPT < 0) )
		{
			return OBJ_AVAILABLE;
		}

		// ...
		if(nVALID < 0)
		{
			valid_chk = "VALID_OFF";
		}
		else
		{
			valid_chk = "OK";
		}

		// Abnormal Signal !! 
		{
			str_log_temp.Format("%s : Abnormal Signal Error !!\n", tp_time);
			str_log = str_log_temp;

			str_log_temp.Format("Normal  :[CS0:1],[VALID:1],[BUSY:-1],[COMPT:-1] \n");
			str_log += str_log_temp;
			
			str_log_temp.Format("Abnormal:[CS0:%d],[VALID:%d],[BUSY:%d],[COMPT:%d] \n", 
				                 nCS0, nVALID, nBUSY, nCOMPT);
			str_log += str_log_temp;

			Fnc__E84_LOG(str_log);
		}
		return OBJ_ABORT;
	}
	
	if(tp_time.CompareNoCase("TP2") == 0)	// READY[ON] <--> BUSY[ON]
	{
		// CS0    "ON"
		// VALID  "ON"
		// TR_REQ "ON"
		// COMPT  "OFF"
		if( (nCS0    > 0)	
		&&  (nVALID  > 0)	
		&&  (nTR_REQ > 0)	
		&&  (nCOMPT  < 0) )
		{
			return OBJ_AVAILABLE;
		}

		// ...
		if(nVALID < 0)
		{
			valid_chk = "VALID_OFF";
		}
		else
		{
			valid_chk = "OK";
		}

		// Abnormal Signal !! 
		{
			str_log_temp.Format("%s : Abnormal Signal Error !!\n", tp_time);
			str_log = str_log_temp;
			
			str_log_temp.Format("Normal  :[CS0:1],[VALID:1],[TR_REQ:1],[COMPT:-1] \n");
			str_log += str_log_temp;
			
			str_log_temp.Format("Abnormal:[CS0:%d],[VALID:%d],[TR_REQ:%d],[COMPT:%d] \n", 
				                 nCS0, nVALID, nTR_REQ, nCOMPT);
			str_log += str_log_temp;

			Fnc__E84_LOG(str_log);
		}
		return OBJ_ABORT;
	}
	
	if(tp_time.CompareNoCase("TP3") == 0)	// FOUP Sns Check
	{
		// CS0    "ON"
		// VALID  "ON"
		// TR_REQ "ON"
		// BUSY   "ON"
		// COMPT  "OFF"
		if( (nCS0    > 0)	
		&&  (nVALID  > 0)	
		&&  (nTR_REQ > 0)
		&&  (nBUSY   > 0))
		{
			return OBJ_AVAILABLE;
		}

		// ...
		if(nVALID < 0)
		{
			valid_chk = "VALID_OFF";
		}
		else
		{
			valid_chk = "OK";
		}

		// Abnormal Signal !! 
		{
			str_log_temp.Format("%s : Abnormal Signal Error !!\n", tp_time);
			str_log = str_log_temp;
			
			str_log_temp.Format("Normal  :[CS0:1],[VALID:1],[TR_REQ:1],[BUSY:1],[COMPT:-1] \n");
			str_log += str_log_temp;
		
			str_log_temp.Format("Abnormal:[CS0:%d],[VALID:%d],[TR_REQ:%d],[BUSY:%d],[COMPT:%d] \n", 
				                 nCS0, nVALID, nTR_REQ, nBUSY, nCOMPT);
			str_log += str_log_temp;

			Fnc__E84_LOG(str_log);
		}
		return OBJ_ABORT;
	}
	
	if(tp_time.CompareNoCase("TP4") == 0)	// L_REQ[OFF] <--> TR_REQ[OFF], BUSY[OFF], COMPT[ON]
	{
		// CS0    "ON"
		// VALID  "ON"
		// TR_REQ "OFF"
		// BUSY   "OFF"
		// COMPT  "ON"
		if( (nCS0    > 0)	
		&&  (nVALID  > 0) )
		{
			return OBJ_AVAILABLE;
		}

		// ...
		if(nVALID < 0)
		{
			valid_chk = "VALID_OFF";
		}
		else
		{
			valid_chk = "OK";
		}
			
		// Abnormal Signal !! 
		{
			str_log_temp.Format("%s : Abnormal Signal Error !!\n", tp_time);
			str_log = str_log_temp;
			
			str_log_temp.Format("Normal  :[CS0:1],[VALID:1] \n");
			str_log += str_log_temp;
			
			str_log_temp.Format("Abnormal:[CS0:%d],[VALID:%d] \n", 
								 nCS0, nVALID);
			str_log += str_log_temp;

			Fnc__E84_LOG(str_log);
		}
		return OBJ_ABORT;
	}
	
	if(tp_time.CompareNoCase("TP5") == 0)	// READY[OFF] <--> CS0[OFF], VALID[OFF], COMPT[OFF]
	{
		// CS0    "OFF"
		// VALID  "OFF"
		// TR_REQ "OFF"
		// BUSY   "OFF"
		// COMPT  "OFF"
		if(nBUSY < 0)
		{
			return OBJ_AVAILABLE;
		}

		// Abnormal Signal !! 
		{
			str_log_temp.Format("%s : Abnormal Signal Error !!\n", tp_time);
			str_log = str_log_temp;
		
			str_log_temp.Format("Normal  :[TR_REQ:-1],[BUSY:-1] \n");
			str_log += str_log_temp;

			str_log_temp.Format("Abnormal:[TR_REQ:%d],[BUSY:%d] \n", 
				                 nTR_REQ, nBUSY);
			str_log += str_log_temp;

			Fnc__E84_LOG(str_log);
		}
		return OBJ_ABORT;
	}

	return OBJ_ABORT;
}

int CObj__OHT_E84_IO::
Set__READY(const CString& ch_data)
{
	if(ch_data.CompareNoCase(_OFF) == 0) 		doEXT_CH__LPx__E84_READY->Set__DATA(_OFF);
	else										doEXT_CH__LPx__E84_READY->Set__DATA(_ON);
	
	return 1;
}

int	CObj__OHT_E84_IO::
Is__FOUP_EXIST()
{
	if(dEXT_CH__LPx_INFO__FOUP_STATE->Check__DATA(_EXIST) > 0)		return 1;
	else															return -1;
}
int	CObj__OHT_E84_IO::
Is__FOUP_NONE()
{
	if(dEXT_CH__LPx_INFO__FOUP_STATE->Check__DATA(_NONE) > 0)		return 1;
	else															return -1;
}

CString	CObj__OHT_E84_IO::
Is__FOUP_STATE_PIO()
{
	CString ch_data1 = dEXT_CH__LPx_IO__FOUP_PLACED_DI->Get__STRING();
	CString ch_data2 = dEXT_CH__LPx_IO__FOUP_PRESENT_DI->Get__STRING();;

	if((ch_data1.CompareNoCase(_ON) == 0)
	&& (ch_data2.CompareNoCase(_ON) == 0))
	{
		return _ON;
	}
	if((ch_data1.CompareNoCase(_OFF) == 0)
	&& (ch_data2.CompareNoCase(_OFF) == 0))
	{
		return _OFF;
	}

	return _UNKNOWN;
}

int CObj__OHT_E84_IO::
Is__Sns_COMPT()
{
	return diEXT_CH__LPx__E84_COMPT->Check__DATA(_ON);
}

int CObj__OHT_E84_IO::
Is__Sns_CS0()
{
	return diEXT_CH__LPx__E84_CS_0->Check__DATA(_ON);
}

int CObj__OHT_E84_IO::
Is__Sns_VALID()
{
	return diEXT_CH__LPx__E84_VALID->Check__DATA(_ON);
}

int CObj__OHT_E84_IO::
Is__Sns_CS0_VALID()
{
	if((Is__Sns_CS0()   > 0) 
	&& (Is__Sns_VALID() > 0))
	{
		return 1;
	}
	return -1;
}

int CObj__OHT_E84_IO::
Set__HO_ON()
{
	if(Is__LP_AUTO_MODE() < 0)			Set__HOAVBL(_OFF);
	else								Set__HOAVBL(_ON);

	return 1;
}

int CObj__OHT_E84_IO::
Set__HOAVBL(const CString& ch_data)
{
	if(ch_data.CompareNoCase(_OFF) == 0) 	
	{	
		doEXT_CH__LPx__E84_HO_AVBL->Set__DATA(_OFF);
	}
	else
	{
		doEXT_CH__LPx__E84_HO_AVBL->Set__DATA(_ON);
	}

	return 1;
}

int CObj__OHT_E84_IO::
Set__AllOff_Except_ES()
{
	// ...
	{
		Set__LREQ(_OFF);
		Set__UREQ(_OFF);
		Set__READY(_OFF);
		Set__HOAVBL(_OFF);
	}

	if(dEXT_CH__LPx_IO__LIGHT_CURTAIN_DI->Check__DATA(_ON) > 0)
	{	
		Set__ES(_OFF);
	}
	else
	{
		Set__ES(_ON);
	}

	return 1;
}

int CObj__OHT_E84_IO::
Set__ES(const CString& ch_data)
{
	if(ch_data.CompareNoCase(_OFF) == 0) 		doEXT_CH__LPx__E84_ES->Set__DATA(_OFF);
	else										doEXT_CH__LPx__E84_ES->Set__DATA(_ON);

	return 1;
}

int CObj__OHT_E84_IO::
Set__UREQ(const CString& ch_data)
{
	if(ch_data.CompareNoCase(_OFF) == 0) 		doEXT_CH__LPx__E84_U_REQ->Set__DATA(_OFF);
	else										doEXT_CH__LPx__E84_U_REQ->Set__DATA(_ON);

	return 1;
}

//-------------------------------------------------------------------------
int  CObj__OHT_E84_IO::
Fnc__CLAMP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller)
{
	CString sMsg;
	CString str_log;

	CString fnc_name = "Fnc__CLAMP()";

	str_log.Format("---> From : [%s], CALL : [%s]", str_caller, fnc_name);
	Fnc__E84_LOG(str_log);

	// Msg Write !!
	sMsg.Format("CLAMP Starting ...");
	Fnc__APP_MSG(sMsg);

	// Check Online !!
	int state = Is__ONLINE(p_variable, p_alarm);
	if(state > 0)
	{
		dEXT_CH__LPx_IO__UNCLAMP_DO->Set__DATA(_OFF);
	}

	if(state > 0)
	{
		state = CMD_STAMP(_ON, _CLAMP,
						  dEXT_CH__LPx_IO__CLAMP_DO.Get__PTR(), 
						  dEXT_CH__LPx_IO__CLAMP_DI.Get__PTR(), 
						  p_alarm);
	}
	
	if(state < 0)	sMsg = "CLAMP Failed..";
	else			sMsg = "CLAMP Completed..";
	Fnc__APP_MSG(sMsg);

	str_log.Format("<--- END : From : [%s], CALL : [%s], ret:[%d]\n", str_caller, fnc_name, state);
	Fnc__E84_LOG(str_log);

	return state;
}

int  CObj__OHT_E84_IO::
Fnc__UNCLAMP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller)
{
	CString sMsg;
	CString str_log;

	CString fnc_name = "Fnc__UNCLAMP()";

	str_log.Format("---> From : [%s], CALL : [%s]", str_caller, fnc_name);
	Fnc__E84_LOG(str_log);

	// Msg Write !!
	sMsg.Format("UNCLAMP Starting ...");
	Fnc__APP_MSG(sMsg);

	// Check Online !!
	int state = Is__ONLINE(p_variable, p_alarm);

	if(state > 0)
	{
		dEXT_CH__LPx_IO__CLAMP_DO->Set__DATA(_OFF);
	}

	if(state > 0)
	{
		state = CMD_STAMP(_ON, _UNCLAMP, 
						  dEXT_CH__LPx_IO__UNCLAMP_DO.Get__PTR(), 
						  dEXT_CH__LPx_IO__UNCLAMP_DI.Get__PTR(), 
						  p_alarm);
	}

	if( (state > 0) && (iActive__SIM_MODE > 0) )
	{
		dEXT_CH__LPx_IO__CLAMP_DI->Set__DATA(_OFF);
		dEXT_CH__LPx_IO__UNCLAMP_DI->Set__DATA(_ON);
	}

	if(state < 0)	sMsg = "UNCLAMP Failed..";
	else			sMsg = "UNCLAMP Completed..";
	Fnc__APP_MSG(sMsg);

	str_log.Format("<--- END : From : [%s], CALL : [%s], ret:[%d]\n", str_caller, fnc_name, state);
	Fnc__E84_LOG(str_log);

	return state;
}

int  CObj__OHT_E84_IO::
Is__LT_BROKEN(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm)
{
	CString str_data;
	CString str_recovery;
	CString r_act;

	if(dEXT_CH__LPx_IO__LIGHT_CURTAIN_DI->Check__DATA(_ON) > 0)
	{
		Set__ES("OFF");
		Set__HOAVBL("Off");

		p_alarm->Check__ALARM(ALID__LP_LIGHT_CURTAIN, str_recovery);
		p_alarm->Popup__ALARM(ALID__LP_LIGHT_CURTAIN, r_act);
	}
	else
	{
		return OBJ_AVAILABLE;
	}

	Fnc__E84_LOG("Waiting.. LT Clear..");
	
	// Waiting !!
	do 
	{
		_sleep(100);
		
		if(Is__Abort(p_variable) > 0)	return OBJ_ABORT;
		if(Is__LP_AUTO_MODE() < 0)		return OBJ_ABORT;

	} 
	while(dEXT_CH__LPx_IO__LIGHT_CURTAIN_DI->Check__DATA(_ON) > 0);

	Fnc__E84_LOG("LT Cleared..");

	// ...
	{
		Set__HOAVBL("ON");
		Set__ES("ON");
	}
	return OBJ_AVAILABLE;
}

int  CObj__OHT_E84_IO::
Is__FOUP_LOGIC_ERROR(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm, const CString& rc_act)
{
	if(dCH__INR_LOGIC_ERROR_FLAG->Check__DATA("On") > 0)
	{
		Set__HOAVBL("Off");

		CString r_act;
		p_alarm->Popup__ALARM(ALID__LP_E84A_FOUP_LOGIC_ERROR, r_act);

		return OBJ_ABORT;
	}
	
	return OBJ_AVAILABLE;
}

int  CObj__OHT_E84_IO::
Is__FOUP_VIOLATION_ERROR(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm, const CString& mode)
{
	CString str_data;
	CString str_recovery;
	CString r_act;

	if(mode.CompareNoCase("EXIST") == 0)
	{
		if(dCH__INR_NONE_VIOLATION_ERROR_FLAG->Check__DATA("ON") > 0)
		{
			Set__HOAVBL("Off");

			p_alarm->Popup__ALARM(ALID__LP_E84A_FOUP_VIOLATION_ERROR, r_act);
			return OBJ_ABORT;
		}
	}
	else	// NONE
	{
		if(dCH__INR_EXIST_VIOLATION_ERROR_FLAG->Check__DATA("ON") > 0)
		{
			Set__HOAVBL("Off");

			p_alarm->Popup__ALARM(ALID__LP_E84A_FOUP_VIOLATION_ERROR, r_act);
			return OBJ_ABORT;
		}
	}
	
	return OBJ_AVAILABLE;
}


int  CObj__OHT_E84_IO::
Is_CS_VALID_OFF(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm)
{
	int fnc_ret = OBJ_ABORT;
	CString str_cs0_data;
	CString str_valid_data;
	CString str_log;

	// ...
	diEXT_CH__LPx__E84_CS_0->Get__DATA(str_cs0_data);
	diEXT_CH__LPx__E84_VALID->Get__DATA(str_valid_data);
	
	str_log.Format("Now... CS0 [%s], VALID [%s] Status !!", str_cs0_data, str_valid_data);
	Fnc__E84_LOG(str_log);

	Fnc__E84_LOG("Both signal [Off].. Waiting Start !!");

	while(1)
	{
		_sleep(100);

		diEXT_CH__LPx__E84_CS_0->Get__DATA(str_cs0_data);
		diEXT_CH__LPx__E84_VALID->Get__DATA(str_valid_data);

		if( (str_cs0_data.CompareNoCase("Off")   == 0) 
		&&  (str_valid_data.CompareNoCase("Off") == 0) )
		{
			fnc_ret = OBJ_AVAILABLE;
			Fnc__E84_LOG("Both Signal Off .. OK");
			break;
		}

		if(Is__Abort(p_variable) > 0)
		{
			Fnc__E84_LOG("Object Aborted...");
			fnc_ret = OBJ_ABORT;
			break;
		}
		if(Is__LP_AUTO_MODE() < 0)
		{
			Fnc__E84_LOG("Not FullAuto...");
			fnc_ret = OBJ_ABORT;
			break;
		}
	}

	// ...
	diEXT_CH__LPx__E84_CS_0->Get__DATA(str_cs0_data);
	diEXT_CH__LPx__E84_VALID->Get__DATA(str_valid_data);
	
	str_log.Format("End... CS0 [%s], VALID [%s] Status !!", str_cs0_data, str_valid_data);
	Fnc__E84_LOG(str_log);

	return fnc_ret;
}
