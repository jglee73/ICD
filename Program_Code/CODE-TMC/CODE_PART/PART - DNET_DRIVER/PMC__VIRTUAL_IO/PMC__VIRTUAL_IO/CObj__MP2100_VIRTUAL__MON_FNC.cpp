#include "StdAfx.h"
#include "CObj__MP2100_VIRTUAL.h"


#define ADDR__WARNING               2      // ILxx02 : Warning
#define ADDR__ALARM                 4      // ILxx04 : Alarm 
#define ADDR__APOS					22     // ILxx16 : Machine Coordinate Feedback Position (Pulse)
#define ADDR__FSPD                  64     // ILxx40 : 피드백속도 [지령단위/sec]   [10^n 지령단위/min]  [0.01%]  <1>
#define ADDR__TORQUE                66     // ILxx42 : Torque (thrust) reference monitor 

#define ADDR__DRIVE_STATUS          0      // IWxx00 : Drive Status
#define ADDR__MCMD_STATUS           9      // IWxx09 : Servo Module Command Status
#define ADDR__SERVO_STATUS          44     // IWxx2C : Network Servo Status
#define ADDR__SERVO_IO_MON          46     // IWxx2E : Network Servo I/O Monitor



//------------------------------------------------------------------------------------
int  CObj__MP2100_VIRTUAL
::Mon__MONITOR(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	SCX__TIMER_CTRL cx_timer_ctrl;
	CString ch_data;
	int i;

	double pre__cfg_min = -1;
	double pre__cfg_max = -1;
	int    pre__cfg_dec = -1;

	while(1)
	{
		cx_timer_ctrl->WAIT(0.5);

		if(iSIM_FLAG > 0)
		{
			// ...
			{
				ch_data = sCH__MON_IO_APOS_PULSE_SET->Get__STRING();
				sCH__MON_IO_APOS_PULSE_GET->Set__DATA(ch_data);
			}

			// ...
			{
				double cfg_unit = aCH__CFG_CONVERSION_UNIT_PULSE_TO_MM->Get__VALUE();
				double cfg_offset = aCH__CFG_HOME_ZERO_BASE_OFFSET->Get__VALUE();

				//
				ch_data = sCH__MON_IO_APOS_PULSE_SET->Get__STRING();
				double trg_pulse = atof(ch_data);
				double trg_pos = (trg_pulse * cfg_unit) - cfg_offset;
				ch_data.Format("%.1f", trg_pos);
				sCH__MON_IO_APOS_MM_SET->Set__DATA(ch_data);

				ch_data = sCH__MON_IO_APOS_PULSE_GET->Get__STRING();
				double cur_pulse = atof(ch_data);
				double cur_pos = (cur_pulse * cfg_unit) - cfg_offset;
				ch_data.Format("%.1f", cur_pos);
				sCH__MON_IO_APOS_MM_GET->Set__DATA(ch_data);

				//
				double set_speed = aCH__PARA_GOTO_POS_SPEED->Get__VALUE() * cfg_unit;
				ch_data.Format("%.3f", set_speed);
				sCH__MON_IO_FSPD_SPEED_SET->Set__DATA(ch_data);
				sCH__MON_IO_FSPD_SPEED_GET->Set__DATA(ch_data);
			}

			// PIN State ...
			Update__PIN_STATE();
		}

		// ...
		{
			ch_data = sCH__MON_LIFT_PIN_STATE->Get__STRING();
			sEXT_CH__LIFT_PIN_STATE->Set__DATA(ch_data);
		}

		// ...
		{
			double cfg_min = aCH__CFG_RANGE_POSITION_MIN->Get__VALUE();
			double cfg_max = aCH__CFG_RANGE_POSITION_MAX->Get__VALUE();
			int    cfg_dec = (int) aCH__CFG_RANGE_POSITION_DEC->Get__VALUE();

			if((pre__cfg_min != cfg_min)
				|| (pre__cfg_max != cfg_max)
				|| (pre__cfg_dec != cfg_dec))
			{
				pre__cfg_min = cfg_min;
				pre__cfg_max = cfg_max;
				pre__cfg_dec = cfg_dec;

				cfg_min = -10.0;

				aCH__CFG_GOTO_POS_UP_ABS_POS->Change__MIN_MAX_DEC(cfg_min,cfg_max,cfg_dec);
				aCH__CFG_GOTO_POS_DOWN_ABS_POS->Change__MIN_MAX_DEC(cfg_min,cfg_max,cfg_dec);

				for(i=0; i<CFG_SIZE__TRANSFER_POS; i++)		
					aCH__CFG_TRANSFER_POS_X_DATA_POS[i]->Change__MIN_MAX_DEC(cfg_min,cfg_max,cfg_dec);

				for(i=0; i<CFG_SIZE__PROC_POS; i++)		
					aCH__CFG_PROC_POS_X_DATA_POS[i]->Change__MIN_MAX_DEC(cfg_min,cfg_max,cfg_dec);
			}
		}

		// ...
	}

	return 1;
}

void CObj__MP2100_VIRTUAL
::Update__MON_IO_DRIVE_STATUS(DWORD hexa_data, const int ok_state)
{
	if(ok_state < 0)
	{
		sCH__MON_IO_DRIVE_STATUS_DATA_HEXA->Set__DATA("ERROR");

		sCH__MON_IO_DRIVE_STATUS_OPR_READY->Set__DATA(STR__OFF);
		sCH__MON_IO_DRIVE_STATUS_SERVO_ON->Set__DATA(STR__OFF);
		sCH__MON_IO_DRIVE_STATUS_SYSTEM_BUSY->Set__DATA(STR__OFF);
		sCH__MON_IO_DRIVE_STATUS_SERVO_READY->Set__DATA(STR__OFF);
		return;
	}

	for(int i=0; i < 32; i++)
	{	
		DWORD check_bit = (hexa_data >> i) & 0x01;

		if(i == 0)
		{
			if(check_bit)		sCH__MON_IO_DRIVE_STATUS_OPR_READY->Set__DATA(STR__ON);
			else				sCH__MON_IO_DRIVE_STATUS_OPR_READY->Set__DATA(STR__OFF);
		}
		else if(i == 1)
		{
			if(check_bit)		sCH__MON_IO_DRIVE_STATUS_SERVO_ON->Set__DATA(STR__ON);
			else				sCH__MON_IO_DRIVE_STATUS_SERVO_ON->Set__DATA(STR__OFF);
		}
		else if(i == 2)
		{
			if(check_bit)		sCH__MON_IO_DRIVE_STATUS_SYSTEM_BUSY->Set__DATA(STR__ON);
			else				sCH__MON_IO_DRIVE_STATUS_SYSTEM_BUSY->Set__DATA(STR__OFF);
		}
		else if(i == 3)
		{
			if(check_bit)		sCH__MON_IO_DRIVE_STATUS_SERVO_READY->Set__DATA(STR__ON);
			else				sCH__MON_IO_DRIVE_STATUS_SERVO_READY->Set__DATA(STR__OFF);
		}
	}
}
void CObj__MP2100_VIRTUAL
::Update__MON_IO_MCMD_STATUS(DWORD hexa_data, const int ok_state)
{
	if(ok_state < 0)
	{
		sCH__MON_ID_MCMD_STATUS_DATA_HEXA->Set__DATA("ERROR");

		sCH__MON_ID_MCMD_STATUS_BUSY->Set__DATA(STR__OFF);
		sCH__MON_ID_MCMD_STATUS_HOLDL->Set__DATA(STR__OFF);
		sCH__MON_ID_MCMD_STATUS_FAIL->Set__DATA(STR__OFF);
		sCH__MON_ID_MCMD_STATUS_ABS_RST->Set__DATA(STR__OFF);
		sCH__MON_ID_MCMD_STATUS_COMPLETE->Set__DATA(STR__OFF);
		return;
	}

	for(int i=0; i < 32; i++)
	{	
		DWORD check_bit = (hexa_data >> i) & 0x01;

		if(i == 0)			//  0 : Command Executing (BUSY)
		{
			if(check_bit)		sCH__MON_ID_MCMD_STATUS_BUSY->Set__DATA(STR__ON);
			else				sCH__MON_ID_MCMD_STATUS_BUSY->Set__DATA(STR__OFF);
		}
		else if(i == 1)		//  1 : Command Hold Completed (HOLDL)
		{
			if(check_bit)		sCH__MON_ID_MCMD_STATUS_HOLDL->Set__DATA(STR__ON);
			else				sCH__MON_ID_MCMD_STATUS_HOLDL->Set__DATA(STR__OFF);
		}
		else if(i == 3)		//  3 : Command Error Occurrence (FAIL)
		{
			if(check_bit)		sCH__MON_ID_MCMD_STATUS_FAIL->Set__DATA(STR__ON);
			else				sCH__MON_ID_MCMD_STATUS_FAIL->Set__DATA(STR__OFF);
		}
		else if(i == 7)		//  7 : Reset Absolute Encoder Completed (ABS_RST)
		{
			if(check_bit)		sCH__MON_ID_MCMD_STATUS_ABS_RST->Set__DATA(STR__ON);
			else				sCH__MON_ID_MCMD_STATUS_ABS_RST->Set__DATA(STR__OFF);
		}
		else if(i == 8)		//  8 : Command Execution Completed (COMPLETE)
		{
			if(check_bit)		sCH__MON_ID_MCMD_STATUS_COMPLETE->Set__DATA(STR__ON);
			else				sCH__MON_ID_MCMD_STATUS_COMPLETE->Set__DATA(STR__OFF);
		}
	}
}
void CObj__MP2100_VIRTUAL
::Update__MON_IO_SERVO_STATUS(DWORD hexa_data, const int ok_state)
{
	if(ok_state < 0)
	{
		sCH__MON_IO_SERVO_STATUS_DATA_HEXA->Set__DATA("ERROR");

		sCH__MON_IO_SERVO_STATUS_ALM->Set__DATA(STR__OFF);
		sCH__MON_IO_SERVO_STATUS_WARNING->Set__DATA(STR__OFF);
		sCH__MON_IO_SERVO_STATUS_CMDRDY->Set__DATA(STR__OFF);
		sCH__MON_IO_SERVO_STATUS_SVON->Set__DATA(STR__OFF);
		sCH__MON_IO_SERVO_STATUS_PON->Set__DATA(STR__OFF);
		sCH__MON_IO_SERVO_STATUS_MLOCK->Set__DATA(STR__OFF);
		sCH__MON_IO_SERVO_STATUS_ZPOINT->Set__DATA(STR__OFF);
		sCH__MON_IO_SERVO_STATUS_PSET->Set__DATA(STR__OFF);
		sCH__MON_IO_SERVO_STATUS_DEN->Set__DATA(STR__OFF);
		sCH__MON_IO_SERVO_STATUS_T_LIM->Set__DATA(STR__OFF);
		sCH__MON_IO_SERVO_STATUS_L_CMP->Set__DATA(STR__OFF);
		sCH__MON_IO_SERVO_STATUS_NEAR->Set__DATA(STR__OFF);
		sCH__MON_IO_SERVO_STATUS_P_SOT->Set__DATA(STR__OFF);
		sCH__MON_IO_SERVO_STATUS_N_SOT->Set__DATA(STR__OFF);
		return;
	}

	for(int i=0; i < 32; i++)
	{	
		DWORD check_bit = (hexa_data >> i) & 0x01;

		if(i == 0)			//  0 : Alarm Occurred (ALM)
		{
			if(check_bit)		sCH__MON_IO_SERVO_STATUS_ALM->Set__DATA(STR__ON);
			else				sCH__MON_IO_SERVO_STATUS_ALM->Set__DATA(STR__OFF);
		}
		else if(i == 1)		//  1 : Warning Occurred (WARNING)
		{
			if(check_bit)		sCH__MON_IO_SERVO_STATUS_WARNING->Set__DATA(STR__ON);
			else				sCH__MON_IO_SERVO_STATUS_WARNING->Set__DATA(STR__OFF);
		}
		else if(i == 2)		//  2 : Command Ready (CMDRDY)
		{
			if(check_bit)		sCH__MON_IO_SERVO_STATUS_CMDRDY->Set__DATA(STR__ON);
			else				sCH__MON_IO_SERVO_STATUS_CMDRDY->Set__DATA(STR__OFF);
		}
		else if(i == 3)		//  3 : Servo ON (SVON)
		{
			if(check_bit)		sCH__MON_IO_SERVO_STATUS_SVON->Set__DATA(STR__ON);
			else				sCH__MON_IO_SERVO_STATUS_SVON->Set__DATA(STR__OFF);
		}
		else if(i == 4)		//  4 : Main Power ON (PON)
		{
			if(check_bit)		sCH__MON_IO_SERVO_STATUS_PON->Set__DATA(STR__ON);
			else				sCH__MON_IO_SERVO_STATUS_PON->Set__DATA(STR__OFF);
		}
		else if(i == 5)		//  5 : Machine Lock (MLOCK) 
		{
			if(check_bit)		sCH__MON_IO_SERVO_STATUS_MLOCK->Set__DATA(STR__ON);
			else				sCH__MON_IO_SERVO_STATUS_MLOCK->Set__DATA(STR__OFF);
		}
		else if(i == 6)		//  6 : Zero Point Position (ZPOINT)
		{
			if(check_bit)		sCH__MON_IO_SERVO_STATUS_ZPOINT->Set__DATA(STR__ON);
			else				sCH__MON_IO_SERVO_STATUS_ZPOINT->Set__DATA(STR__OFF);
		}
		else if(i == 7)		//  7 : Position Completed (PSET)
		{
			if(check_bit)		sCH__MON_IO_SERVO_STATUS_PSET->Set__DATA(STR__ON);
			else				sCH__MON_IO_SERVO_STATUS_PSET->Set__DATA(STR__OFF);
		}
		else if(i == 8)		//  8 : Distribution Completed (DEN)
		{
			if(check_bit)		sCH__MON_IO_SERVO_STATUS_DEN->Set__DATA(STR__ON);
			else				sCH__MON_IO_SERVO_STATUS_DEN->Set__DATA(STR__OFF);
		}
		else if(i == 9)		//  9 : Torque Being Limited (T_LIM)
		{
			if(check_bit)		sCH__MON_IO_SERVO_STATUS_T_LIM->Set__DATA(STR__ON);
			else				sCH__MON_IO_SERVO_STATUS_T_LIM->Set__DATA(STR__OFF);
		}
		else if(i == 10)	// 10 : Latch Completed (L_CMP)
		{
			if(check_bit)		sCH__MON_IO_SERVO_STATUS_L_CMP->Set__DATA(STR__ON);
			else				sCH__MON_IO_SERVO_STATUS_L_CMP->Set__DATA(STR__OFF);
		}
		else if(i == 11)	// 11 : Position Proximity (NEAR)
		{
			if(check_bit)		sCH__MON_IO_SERVO_STATUS_NEAR->Set__DATA(STR__ON);
			else				sCH__MON_IO_SERVO_STATUS_NEAR->Set__DATA(STR__OFF);
		}
		else if(i == 12)	// 12 : Positive Software Limit (P_SOT)
		{
			if(check_bit)		sCH__MON_IO_SERVO_STATUS_P_SOT->Set__DATA(STR__ON);
			else				sCH__MON_IO_SERVO_STATUS_P_SOT->Set__DATA(STR__OFF);
		}
		else if(i == 13)	// 13 : Negative Software Limit (N_SOT)
		{
			if(check_bit)		sCH__MON_IO_SERVO_STATUS_N_SOT->Set__DATA(STR__ON);
			else				sCH__MON_IO_SERVO_STATUS_N_SOT->Set__DATA(STR__OFF);
		}
	}
}
void CObj__MP2100_VIRTUAL
::Update__MON_IO_SERVO_IO_MON(DWORD hexa_data, const int ok_state)
{
	if(ok_state < 0)
	{
		sCH__MON_IO_SERVO_IO_DATA_HEXA->Set__DATA("ERROR");

		sCH__MON_IO_SERVO_IO_P_OT->Set__DATA(STR__OFF);
		sCH__MON_IO_SERVO_IO_N_OT->Set__DATA(STR__OFF);
		sCH__MON_IO_SERVO_IO_DEC->Set__DATA(STR__OFF);
		sCH__MON_IO_SERVO_IO_PA->Set__DATA(STR__OFF);
		sCH__MON_IO_SERVO_IO_PB->Set__DATA(STR__OFF);
		sCH__MON_IO_SERVO_IO_PC->Set__DATA(STR__OFF);
		sCH__MON_IO_SERVO_IO_EXIT1->Set__DATA(STR__OFF);
		sCH__MON_IO_SERVO_IO_EXIT2->Set__DATA(STR__OFF);
		sCH__MON_IO_SERVO_IO_EXIT3->Set__DATA(STR__OFF);
		sCH__MON_IO_SERVO_IO_BRK->Set__DATA(STR__OFF);
		sCH__MON_IO_SERVO_IO_12->Set__DATA(STR__OFF);
		sCH__MON_IO_SERVO_IO_13->Set__DATA(STR__OFF);
		sCH__MON_IO_SERVO_IO_14->Set__DATA(STR__OFF);
		sCH__MON_IO_SERVO_IO_15->Set__DATA(STR__OFF);
		return;
	}

	for(int i=0; i < 32; i++)
	{	
		DWORD check_bit = (hexa_data >> i) & 0x01;

		if(i == 0)				//  0 : 정회전 구동 금지 입력
		{
			if(check_bit)		sCH__MON_IO_SERVO_IO_P_OT->Set__DATA(STR__ON);
			else				sCH__MON_IO_SERVO_IO_P_OT->Set__DATA(STR__OFF);
		}
		else if(i == 1)			//  1 : 역회전 구동 금지 입력
		{
			if(check_bit)		sCH__MON_IO_SERVO_IO_N_OT->Set__DATA(STR__ON);
			else				sCH__MON_IO_SERVO_IO_N_OT->Set__DATA(STR__OFF);
		}
		else if(i == 2)			//  2 : 원점 복귀 감속 리밋 스위치 입력
		{
			if(check_bit)		sCH__MON_IO_SERVO_IO_DEC->Set__DATA(STR__ON);
			else				sCH__MON_IO_SERVO_IO_DEC->Set__DATA(STR__OFF);
		}
		else if(i == 3)			//  3 : 인코더 A상 입력
		{
			if(check_bit)		sCH__MON_IO_SERVO_IO_PA->Set__DATA(STR__ON);
			else				sCH__MON_IO_SERVO_IO_PA->Set__DATA(STR__OFF);
		}
		else if(i == 4)			//  4 : 인코더 B상 입력
		{
			if(check_bit)		sCH__MON_IO_SERVO_IO_PB->Set__DATA(STR__ON);
			else				sCH__MON_IO_SERVO_IO_PB->Set__DATA(STR__OFF);
		}
		else if(i == 5)			//  5 : 인코더 C상 입력
		{
			if(check_bit)		sCH__MON_IO_SERVO_IO_PC->Set__DATA(STR__ON);
			else				sCH__MON_IO_SERVO_IO_PC->Set__DATA(STR__OFF);
		}
		else if(i == 6)			//  6 : 제 1외부 래치 입력
		{
			if(check_bit)		sCH__MON_IO_SERVO_IO_EXIT1->Set__DATA(STR__ON);
			else				sCH__MON_IO_SERVO_IO_EXIT1->Set__DATA(STR__OFF);
		}
		else if(i == 7)			//  7 : 제 2외부 래치 입력
		{
			if(check_bit)		sCH__MON_IO_SERVO_IO_EXIT2->Set__DATA(STR__ON);
			else				sCH__MON_IO_SERVO_IO_EXIT2->Set__DATA(STR__OFF);
		}
		else if(i == 8)			//  8 : 제 3외부 래치 입력
		{
			if(check_bit)		sCH__MON_IO_SERVO_IO_EXIT3->Set__DATA(STR__ON);
			else				sCH__MON_IO_SERVO_IO_EXIT3->Set__DATA(STR__OFF);
		}
		else if(i == 9)			//  9 : 브레이크 출력
		{
			if(check_bit)		sCH__MON_IO_SERVO_IO_BRK->Set__DATA(STR__ON);
			else				sCH__MON_IO_SERVO_IO_BRK->Set__DATA(STR__OFF);
		}
		else if(i == 12)		// 12 : Pn81E.0에서 선텍된 CN1 입력 신호
		{
			if(check_bit)		sCH__MON_IO_SERVO_IO_12->Set__DATA(STR__ON);
			else				sCH__MON_IO_SERVO_IO_12->Set__DATA(STR__OFF);
		}
		else if(i == 13)		// 13 : Pn81E.1에서 선텍된 CN1 입력 신호
		{
			if(check_bit)		sCH__MON_IO_SERVO_IO_13->Set__DATA(STR__ON);
			else				sCH__MON_IO_SERVO_IO_13->Set__DATA(STR__OFF);
		}
		else if(i == 14)		// 14 : Pn81E.2에서 선텍된 CN1 입력 신호
		{
			if(check_bit)		sCH__MON_IO_SERVO_IO_14->Set__DATA(STR__ON);
			else				sCH__MON_IO_SERVO_IO_14->Set__DATA(STR__OFF);
		}
		else if(i == 15)		// 15 : Pn81E.3에서 선텍된 CN1 입력 신호
		{
			if(check_bit)		sCH__MON_IO_SERVO_IO_15->Set__DATA(STR__ON);
			else				sCH__MON_IO_SERVO_IO_15->Set__DATA(STR__OFF);
		}
	}
}

void CObj__MP2100_VIRTUAL
::Update__PIN_STATE()
{
	CString str_state = "MOVING";
	int i;

	CString ch_data = sCH__MON_IO_APOS_MM_GET->Get__STRING();
	int cur_pos = atoi(ch_data);

	// PROCESS.POS ...
	{
		int pos_index = -1;

		for(i=0; i<CFG_SIZE__PROC_POS; i++)
		{
			int cfg_pos = (int) aCH__CFG_PROC_POS_X_DATA_POS[i]->Get__VALUE();
			if(cur_pos != cfg_pos)			continue;

			if(i == PROC_POS__DOWN)				str_state = "PROC.DOWN";
			else if(i == PROC_POS__MIDDLE)				str_state = "PROC.MIDDLE";
			else if(i == PROC_POS__UP)					str_state = "PROC.UP";
			else										continue;

			pos_index = i;
			break;
		}

		for(i=0; i<CFG_SIZE__PROC_POS; i++)
		{
			if(i == pos_index)		sCH__APP_PROC_POS_X_SNS[i]->Set__DATA(STR__ON);
			else					sCH__APP_PROC_POS_X_SNS[i]->Set__DATA(STR__OFF);
		}
	}
	// TRNASFER.POS ...
	{
		int pos_index = -1;

		for(i=0; i<CFG_SIZE__TRANSFER_POS; i++)
		{
			int cfg_pos = (int) aCH__CFG_TRANSFER_POS_X_DATA_POS[i]->Get__VALUE();
			if(cur_pos != cfg_pos)			continue;

			if(i == TRANSFER_POS__DOWN)			str_state = "DOWN";
			else if(i == TRANSFER_POS__MIDDLE_DOWN)		str_state = "MIDDLE.DOWN";
			else if(i == TRANSFER_POS__MIDDLE_UP)		str_state = "MIDDLE.UP";
			else if(i == TRANSFER_POS__UP)				str_state = "UP";
			else										continue;

			pos_index = i;
			break;
		}

		for(i=0; i<CFG_SIZE__TRANSFER_POS; i++)
		{
			if(i == pos_index)		sCH__APP_TRANSFER_POS_X_SNS[i]->Set__DATA(STR__ON);
			else					sCH__APP_TRANSFER_POS_X_SNS[i]->Set__DATA(STR__OFF);
		}
	}

	sCH__MON_LIFT_PIN_STATE->Set__DATA(str_state);
}

// TRANSFER : XXX_POS -> DOWN
int CObj__MP2100_VIRTUAL
::Check__TRANSFER_POS__MORE_THAN__MIDDLE_UP()
{
	int db_index = TRANSFER_POS__MIDDLE_UP;
	int cfg_pos  = (int) aCH__CFG_TRANSFER_POS_X_DATA_POS[db_index]->Get__VALUE();

	if(cfg_pos < 1)				return -1;

	CString ch_data = sCH__MON_IO_APOS_MM_GET->Get__STRING();
	int cur_pos = atoi(ch_data);

	if(cur_pos > cfg_pos)		return 1;

	return -1;
}
int  CObj__MP2100_VIRTUAL
::Check__TRANSFER_POS__MORE_THAN__MIDDLE_DOWN()
{
	int db_index = TRANSFER_POS__MIDDLE_DOWN;
	int cfg_pos  = (int) aCH__CFG_TRANSFER_POS_X_DATA_POS[db_index]->Get__VALUE();

	if(cfg_pos < 1)				return -1;

	CString ch_data = sCH__MON_IO_APOS_MM_GET->Get__STRING();
	int cur_pos = atoi(ch_data);

	if(cur_pos > cfg_pos)		return 1;

	return -1;
}

// TRANSFER : XXX_POS -> UP
int  CObj__MP2100_VIRTUAL
::Check__TRANSFER_POS__LESS_THAN__MIDDLE_DOWN()
{
	int db_index = TRANSFER_POS__MIDDLE_DOWN;
	int cfg_pos  = (int) aCH__CFG_TRANSFER_POS_X_DATA_POS[db_index]->Get__VALUE();

	if(cfg_pos < 1)				return -1;

	CString ch_data = sCH__MON_IO_APOS_MM_GET->Get__STRING();
	int cur_pos = atoi(ch_data);

	if(cur_pos < cfg_pos)		return 1;

	return -1;
}
int  CObj__MP2100_VIRTUAL
::Check__TRANSFER_POS__LESS_THAN__MIDDLE_UP()
{
	int db_index = TRANSFER_POS__MIDDLE_UP;
	int cfg_pos  = (int) aCH__CFG_TRANSFER_POS_X_DATA_POS[db_index]->Get__VALUE();

	if(cfg_pos < 1)				return -1;

	CString ch_data = sCH__MON_IO_APOS_MM_GET->Get__STRING();
	int cur_pos = atoi(ch_data);

	if(cur_pos < cfg_pos)		return 1;

	return -1;
}

// PROCESS : XXX_POS -> DOWN
int CObj__MP2100_VIRTUAL
::Check__PROCESS_POS__MORE_THAN__MIDDLE()
{
	int db_index = PROC_POS__MIDDLE;
	int cfg_pos  = (int) aCH__CFG_PROC_POS_X_DATA_POS[db_index]->Get__VALUE();

	if(cfg_pos < 1)				return -1;

	CString ch_data = sCH__MON_IO_APOS_MM_GET->Get__STRING();
	int cur_pos = atoi(ch_data);

	if(cur_pos > cfg_pos)		return 1;

	return -1;
}

// PROCESS : XXX_POS -> UP
int CObj__MP2100_VIRTUAL
::Check__PROCESS_POS__LESS_THAN__MIDDLE()
{
	int db_index = PROC_POS__MIDDLE;
	int cfg_pos  = (int) aCH__CFG_PROC_POS_X_DATA_POS[db_index]->Get__VALUE();

	if(cfg_pos < 1)				return -1;

	CString ch_data = sCH__MON_IO_APOS_MM_GET->Get__STRING();
	int cur_pos = atoi(ch_data);

	if(cur_pos < cfg_pos)		return 1;

	return -1;
}
