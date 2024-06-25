#include "StdAfx.h"
#include "CObj__MP2100_VIRTUAL.h"


//-------------------------------------------------------------------------	
int  CObj__MP2100_VIRTUAL
::Call__INIT(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	if(Call__HOME(p_variable,p_alarm) < 0)
	{
		return -1;
	}

	return Call__DOWN(p_variable,p_alarm);
}
int  CObj__MP2100_VIRTUAL
::Fnc__INIT(CII_OBJECT__ALARM* p_alarm)
{
	return 1;
}

int  CObj__MP2100_VIRTUAL
::Call__HOME(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	SCX__ASYNC_TIMER_CTRL x_timer_ctrl;
	CString ch_data;

	x_timer_ctrl->REGISTER__COUNT_CHANNEL_NAME(sCH__OBJ_TIME_COUNT->Get__CHANNEL_NAME());
	x_timer_ctrl->STOP_ZERO();
	x_timer_ctrl->START__COUNT_UP(9999);

	sCH__MON_IO_APOS_PULSE_SET->Set__DATA("0");

	while(1)
	{
		Sleep(50);

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			Fnc__GOTO_POS_STOP(p_variable,p_alarm);
			return -1;
		}

		double cfg_sec = aCH__CFG_HOME_TIMEOUT->Get__VALUE();
		if(x_timer_ctrl->Get__CURRENT_TIME() > cfg_sec)
		{
			Fnc__GOTO_POS_STOP(p_variable,p_alarm);
			return -2;
		}

		double sim_sec = aCH__SIM_HOME_TIME->Get__VALUE();
		if(x_timer_ctrl->Get__CURRENT_TIME() >= sim_sec)
		{
			break;
		}
	}
	return 1;
}

// ...
int  CObj__MP2100_VIRTUAL
::Call__MOVE_STOP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	if((sCH__TEACH_ACTIVE_JOG_UP->Check__DATA(STR__YES)   > 0)
	|| (sCH__TEACH_ACTIVE_JOG_DOWN->Check__DATA(STR__YES) > 0))
	{
		sCH__TEACH_ACTIVE_JOG_UP->Set__DATA("");
		sCH__TEACH_ACTIVE_JOG_DOWN->Set__DATA("");

		return Fnc__TEACH_JOG_STOP(p_variable,p_alarm);
	}

	return Fnc__GOTO_POS_STOP(p_variable,p_alarm);
}

// ...
int  CObj__MP2100_VIRTUAL
::Call__TEACH_ABS_START(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const int up_mode)
{
	Update__ABS_OF_TEACHING_PROPERTY(up_mode);

	if(up_mode > 0)			sCH__TEACH_ACTIVE_ABS_UP->Set__DATA(STR__YES);
	else					sCH__TEACH_ACTIVE_ABS_DOWN->Set__DATA(STR__YES);

	int rc = Fnc__GOTO_POS_START(p_variable,p_alarm);

	sCH__TEACH_ACTIVE_ABS_UP->Set__DATA("");
	sCH__TEACH_ACTIVE_ABS_DOWN->Set__DATA("");

	return rc;
}

int  CObj__MP2100_VIRTUAL
::Update__JOG_OF_TEACHING_PROPERTY()
{
	double cfg_value = 0.0;

	cfg_value = aCH__CFG_TEACH_JOG_SPEED->Get__VALUE();
	aCH__PARA_GOTO_POS_SPEED->Set__VALUE(cfg_value);

	cfg_value = aCH__CFG_TEACH_JOG_ACCEL->Get__VALUE();
	aCH__PARA_GOTO_POS_ACCEL->Set__VALUE(cfg_value);

	cfg_value = aCH__CFG_TEACH_JOG_DECEL->Get__VALUE();
	aCH__PARA_GOTO_POS_DECEL->Set__VALUE(cfg_value);

	return 1;
}
int  CObj__MP2100_VIRTUAL
::Update__ABS_OF_TEACHING_PROPERTY(const int up_mode)
{
	double cfg_unit  = aCH__CFG_CONVERSION_UNIT_PULSE_TO_MM->Get__VALUE();
	if(cfg_unit < 0.001)		cfg_unit = 0.001;		

	double cfg_value = 0.0;

	cfg_value = aCH__CFG_TEACH_ABS_SPEED->Get__VALUE();
	cfg_value /= cfg_unit;
	aCH__PARA_GOTO_POS_SPEED->Set__VALUE(cfg_value);

	cfg_value = aCH__CFG_TEACH_ABS_ACCEL->Get__VALUE();
	cfg_value /= cfg_unit;
	aCH__PARA_GOTO_POS_ACCEL->Set__VALUE(cfg_value);

	cfg_value = aCH__CFG_TEACH_ABS_DECEL->Get__VALUE();
	cfg_value /= cfg_unit;
	aCH__PARA_GOTO_POS_DECEL->Set__VALUE(cfg_value);

	// ...
	double trg_pos    = 0.0;
	double goto_pulse = 0.0;

	CString ch_data = sCH__MON_IO_APOS_MM_GET->Get__STRING();
	double cur_pos = atof(ch_data);

	if(up_mode > 0)
	{
		double goto_pos = aCH__CFG_GOTO_POS_UP_ABS_POS->Get__VALUE();
		trg_pos = cur_pos + goto_pos;
	}
	else
	{
		double goto_pos = aCH__CFG_GOTO_POS_DOWN_ABS_POS->Get__VALUE();
		trg_pos = cur_pos - goto_pos;
	}

	goto_pulse = trg_pos / cfg_unit;
	aCH__PARA_GOTO_POS_DATA_PULSE->Set__VALUE(goto_pulse);
	return 1;
}

int  CObj__MP2100_VIRTUAL
::Update__GOTO_POS_OF_PROC_POS(const int pos_index)
{
	double cfg_unit  = aCH__CFG_CONVERSION_UNIT_PULSE_TO_MM->Get__VALUE();
	if(cfg_unit < 0.001)		cfg_unit = 0.001;		

	double cfg_value = 0.0;

	cfg_value = aCH__CFG_PROC_POS_X_DATA_POS[pos_index]->Get__VALUE();
	cfg_value /= cfg_unit;
	aCH__PARA_GOTO_POS_DATA_PULSE->Set__VALUE(cfg_value);

	return 1;
}
int  CObj__MP2100_VIRTUAL
::Update__GOTO_POS_OF_PROC_SPEED(const int pos_index)
{
	double cfg_unit  = aCH__CFG_CONVERSION_UNIT_PULSE_TO_MM->Get__VALUE();
	if(cfg_unit < 0.001)		cfg_unit = 0.001;		

	double cfg_value = 0.0;

	cfg_value = aCH__CFG_PROC_POS_X_SPEED[pos_index]->Get__VALUE();
	cfg_value /= cfg_unit;
	aCH__PARA_GOTO_POS_SPEED->Set__VALUE(cfg_value);

	cfg_value = aCH__CFG_PROC_POS_X_ACCEL[pos_index]->Get__VALUE();
	cfg_value /= cfg_unit;
	aCH__PARA_GOTO_POS_ACCEL->Set__VALUE(cfg_value);

	cfg_value = aCH__CFG_PROC_POS_X_DECEL[pos_index]->Get__VALUE();
	cfg_value /= cfg_unit;
	aCH__PARA_GOTO_POS_DECEL->Set__VALUE(cfg_value);

	return 1;
}

int  CObj__MP2100_VIRTUAL
::Update__GOTO_POS_OF_MOVE_POS(const int pos_index)
{
	double cfg_unit  = aCH__CFG_CONVERSION_UNIT_PULSE_TO_MM->Get__VALUE();
	if(cfg_unit < 0.001)		cfg_unit = 0.001;		

	double cfg_value = 0.0;

	cfg_value = aCH__CFG_TRANSFER_POS_X_DATA_POS[pos_index]->Get__VALUE();
	cfg_value /= cfg_unit;
	aCH__PARA_GOTO_POS_DATA_PULSE->Set__VALUE(cfg_value);

	// ...
	{
		CString log_msg;
		CString log_bff;

		log_msg  = "\n";
		log_msg += "Update__GOTO_POS_OF_MOVE_POS() Started ...";
		log_msg += "\n";

		log_bff.Format("  * pos_index <- %1d \n", pos_index);
		log_msg += log_bff;

		CString ch_name = aCH__PARA_GOTO_POS_DATA_PULSE->Get__CHANNEL_NAME();
		CString ch_data;
		aCH__PARA_GOTO_POS_DATA_PULSE->Get__DATA(ch_data);

		log_bff.Format("  * %s <- %s \n", ch_name,ch_data), 
		log_msg += log_bff;

		xI__APP_LOG_CTRL->WRITE__LOG(log_msg);
	}
	return 1;
}
int  CObj__MP2100_VIRTUAL
::Update__GOTO_POS_OF_MOVE_SPEED(const int pos_index)
{
	double cfg_unit  = aCH__CFG_CONVERSION_UNIT_PULSE_TO_MM->Get__VALUE();
	if(cfg_unit < 0.001)		cfg_unit = 0.001;		

	double cfg_value = 0.0;

	cfg_value = aCH__CFG_TRANSFER_POS_X_SPEED[pos_index]->Get__VALUE();
	cfg_value /= cfg_unit;
	aCH__PARA_GOTO_POS_SPEED->Set__VALUE(cfg_value);

	cfg_value = aCH__CFG_TRANSFER_POS_X_ACCEL[pos_index]->Get__VALUE();
	cfg_value /= cfg_unit;
	aCH__PARA_GOTO_POS_ACCEL->Set__VALUE(cfg_value);

	cfg_value = aCH__CFG_TRANSFER_POS_X_DECEL[pos_index]->Get__VALUE();
	cfg_value /= cfg_unit;
	aCH__PARA_GOTO_POS_DECEL->Set__VALUE(cfg_value);

	return 1;
}

int  CObj__MP2100_VIRTUAL
::Fnc__GOTO_POS_START(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	SCX__ASYNC_TIMER_CTRL x_timer_ctrl;
	CString ch_data;

	x_timer_ctrl->REGISTER__COUNT_CHANNEL_NAME(sCH__OBJ_TIME_COUNT->Get__CHANNEL_NAME());
	x_timer_ctrl->STOP_ZERO();
	x_timer_ctrl->START__COUNT_UP(9999);

	double cfg_unit  = aCH__CFG_CONVERSION_UNIT_PULSE_TO_MM->Get__VALUE();
	if(cfg_unit < 0.001)		cfg_unit = 0.001;		

	double cfg_offset = aCH__CFG_HOME_ZERO_BASE_OFFSET->Get__VALUE();
	double offset_pulse = cfg_offset / cfg_unit;
	
	double cur_pulse = aCH__PARA_GOTO_POS_DATA_PULSE->Get__VALUE();
	cur_pulse += offset_pulse;

	ch_data.Format("%.0f", cur_pulse);
	sCH__MON_IO_APOS_PULSE_SET->Set__DATA(ch_data);

	while(1)
	{
		Sleep(50);

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			Fnc__GOTO_POS_STOP(p_variable,p_alarm);
			return -1;
		}

		double cfg_sec = aCH__CFG_MOVING_TIMEOUT->Get__VALUE();
		if(x_timer_ctrl->Get__CURRENT_TIME() > cfg_sec)
		{
			Fnc__GOTO_POS_STOP(p_variable,p_alarm);

			return -2;
		}

		double sim_sec = aCH__SIM_MOVE_TIME->Get__VALUE();
		if(x_timer_ctrl->Get__CURRENT_TIME() >= sim_sec)
		{
			break;
		}
	}
	return 1;
}

int  CObj__MP2100_VIRTUAL
::Fnc__GOTO_POS_STOP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	return 1;
}


// ...
int  CObj__MP2100_VIRTUAL
::Call__TEACH_JOG_START(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const int plus_mode)
{
	Update__JOG_OF_TEACHING_PROPERTY();

	return Fnc__TEACH_JOG_START(p_variable,p_alarm, plus_mode);
}

int  CObj__MP2100_VIRTUAL
::Fnc__TEACH_JOG_START(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const int plus_mode)
{
	if(plus_mode > 0)		sCH__TEACH_ACTIVE_JOG_UP->Set__DATA(STR__YES);
	else					sCH__TEACH_ACTIVE_JOG_DOWN->Set__DATA(STR__YES);
	
	while(1)
	{
		Sleep(50);

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			Fnc__TEACH_JOG_STOP(p_variable,p_alarm);
			return -1;
		}
	}
	return 1;
}
int  CObj__MP2100_VIRTUAL
::Fnc__TEACH_JOG_STOP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	sCH__TEACH_ACTIVE_JOG_UP->Set__DATA("");
	sCH__TEACH_ACTIVE_JOG_DOWN->Set__DATA("");

	return 1;
}


// ...
int  CObj__MP2100_VIRTUAL
::Call__DOWN(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm)
{
	int p_index;

	// UP -> MIDDLE.UP
	if(Check__TRANSFER_POS__MORE_THAN__MIDDLE_UP() > 0)
	{
		p_index = TRANSFER_POS__MIDDLE_UP;
		Update__GOTO_POS_OF_MOVE_POS(p_index);

		p_index = TRANSFER_POS__UP;
		Update__GOTO_POS_OF_MOVE_SPEED(p_index);

		if(Fnc__GOTO_POS_START(p_variable,p_alarm) < 0)
		{
			return -1;
		}
	}
	
	// MIDDLE.UP -> MIDDLE.DOWN
	if(Check__TRANSFER_POS__MORE_THAN__MIDDLE_DOWN() > 0)
	{
		p_index = TRANSFER_POS__MIDDLE_DOWN;
		Update__GOTO_POS_OF_MOVE_POS(p_index);
		Update__GOTO_POS_OF_MOVE_SPEED(p_index);

		if(Fnc__GOTO_POS_START(p_variable,p_alarm) < 0)
		{
			return -1;
		}
	}
	
	// MIDDLE.DOWN -> DOWN
	{
		p_index = TRANSFER_POS__DOWN;
		Update__GOTO_POS_OF_MOVE_POS(p_index);
		Update__GOTO_POS_OF_MOVE_SPEED(p_index);

		if(Fnc__GOTO_POS_START(p_variable,p_alarm) < 0)
		{
			return -3;
		}
	}
	return 1;
}
int  CObj__MP2100_VIRTUAL
::Call__DOWN_FAST(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm)
{
	int p_index;

	// XXX -> DOWN
	{
		p_index = TRANSFER_POS__DOWN;
		Update__GOTO_POS_OF_MOVE_POS(p_index);

		p_index = TRANSFER_POS__UP;
		Update__GOTO_POS_OF_MOVE_SPEED(p_index);

		if(Fnc__GOTO_POS_START(p_variable,p_alarm) < 0)
		{
			return -1;
		}
	}
	return 1;
}

int  CObj__MP2100_VIRTUAL
::Call__MIDDLE_DOWN(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm)
{
	int p_index;

	// DOWN -> MIDDLE.DOWN
	if(Check__TRANSFER_POS__LESS_THAN__MIDDLE_DOWN() > 0)
	{
		p_index = TRANSFER_POS__MIDDLE_DOWN;
		Update__GOTO_POS_OF_MOVE_POS(p_index);

		p_index = TRANSFER_POS__DOWN;
		Update__GOTO_POS_OF_MOVE_SPEED(p_index);

		return Fnc__GOTO_POS_START(p_variable,p_alarm);
	}

	// UP -> MIDDLE.UP
	if(Check__TRANSFER_POS__MORE_THAN__MIDDLE_UP() > 0)
	{
		p_index = TRANSFER_POS__MIDDLE_UP;
		Update__GOTO_POS_OF_MOVE_POS(p_index);

		p_index = TRANSFER_POS__UP;
		Update__GOTO_POS_OF_MOVE_SPEED(p_index);

		if(Fnc__GOTO_POS_START(p_variable,p_alarm) < 0)
		{
			return -1;
		}
	}

	// MIDDLE.UP -> MIDDLE.DOWN
	p_index = TRANSFER_POS__MIDDLE_DOWN;
	Update__GOTO_POS_OF_MOVE_POS(p_index);
	Update__GOTO_POS_OF_MOVE_SPEED(p_index);

	return Fnc__GOTO_POS_START(p_variable,p_alarm);
}
int  CObj__MP2100_VIRTUAL
::Call__MIDDLE_UP(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm)
{
	int p_index;

	// UP -> MIDDLE.UP
	if(Check__TRANSFER_POS__MORE_THAN__MIDDLE_UP() > 0)
	{
		p_index = TRANSFER_POS__MIDDLE_UP;
		Update__GOTO_POS_OF_MOVE_POS(p_index);

		p_index = TRANSFER_POS__UP;
		Update__GOTO_POS_OF_MOVE_SPEED(p_index);

		return Fnc__GOTO_POS_START(p_variable,p_alarm);
	}

	// DOWN -> MIDDLE.DOWN
	if(Check__TRANSFER_POS__LESS_THAN__MIDDLE_DOWN() > 0)
	{
		p_index = TRANSFER_POS__MIDDLE_DOWN;
		Update__GOTO_POS_OF_MOVE_POS(p_index);

		p_index = TRANSFER_POS__DOWN;
		Update__GOTO_POS_OF_MOVE_SPEED(p_index);

		if(Fnc__GOTO_POS_START(p_variable,p_alarm) < 0)
		{
			return -1;
		}
	}

	// MIDDLE.DOWN -> MIDDLE.UP
	p_index = TRANSFER_POS__MIDDLE_UP;
	Update__GOTO_POS_OF_MOVE_POS(p_index);
	Update__GOTO_POS_OF_MOVE_SPEED(p_index);

	return Fnc__GOTO_POS_START(p_variable,p_alarm);
}

int  CObj__MP2100_VIRTUAL
::Call__UP(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm)
{
	int p_index;

	// DOWN -> MIDDLE.DOWN
	if(Check__TRANSFER_POS__LESS_THAN__MIDDLE_DOWN() > 0)
	{
		p_index = TRANSFER_POS__MIDDLE_DOWN;
		Update__GOTO_POS_OF_MOVE_POS(p_index);

		p_index = TRANSFER_POS__DOWN;
		Update__GOTO_POS_OF_MOVE_SPEED(p_index);

		if(Fnc__GOTO_POS_START(p_variable,p_alarm) < 0)
		{
			return -1;
		}
	}

	// MIDDLE.DOWN -> MIDDLE.UP
	if(Check__TRANSFER_POS__LESS_THAN__MIDDLE_UP() > 0)
	{
		p_index = TRANSFER_POS__MIDDLE_UP;
		Update__GOTO_POS_OF_MOVE_POS(p_index);
		Update__GOTO_POS_OF_MOVE_SPEED(p_index);

		if(Fnc__GOTO_POS_START(p_variable,p_alarm) < 0)
		{
			return -1;
		}
	}

	// MIDDLE.UP -> UP
	{
		p_index = TRANSFER_POS__UP;
		Update__GOTO_POS_OF_MOVE_POS(p_index);
		Update__GOTO_POS_OF_MOVE_SPEED(p_index);

		if(Fnc__GOTO_POS_START(p_variable,p_alarm) < 0)
		{
			return -3;
		}
	}
	return 1;
}
int  CObj__MP2100_VIRTUAL
::Call__UP_FAST(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm)
{
	int p_index;

	// XXX -> UP
	{
		p_index = TRANSFER_POS__UP;
		Update__GOTO_POS_OF_MOVE_POS(p_index);
		Update__GOTO_POS_OF_MOVE_SPEED(p_index);

		if(Fnc__GOTO_POS_START(p_variable,p_alarm) < 0)
		{
			return -3;
		}
	}
	return 1;
}

// ...
int  CObj__MP2100_VIRTUAL
::Call__PROC_DOWN(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm)
{
	int p_index;

	// XXX -> PROC.MIDDLE
	if(Check__PROCESS_POS__MORE_THAN__MIDDLE() > 0)
	{
		p_index = PROC_POS__MIDDLE;		
		Update__GOTO_POS_OF_PROC_POS(p_index);

		p_index = PROC_POS__UP;		
		Update__GOTO_POS_OF_PROC_SPEED(p_index);

		if(Fnc__GOTO_POS_START(p_variable,p_alarm) < 0)
		{
			return -1;
		}
	}

	// PROC.MIDDLE
	{
		p_index = PROC_POS__DOWN;
		
		Update__GOTO_POS_OF_PROC_POS(p_index);
		Update__GOTO_POS_OF_PROC_SPEED(p_index);
	}
	return Fnc__GOTO_POS_START(p_variable,p_alarm);
}
int  CObj__MP2100_VIRTUAL
::Call__PROC_MIDDLE(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm)
{
	int p_index;

	// UP -> MIDDLE
	if(Check__PROCESS_POS__MORE_THAN__MIDDLE() > 0)
	{
		p_index = PROC_POS__MIDDLE;
		Update__GOTO_POS_OF_PROC_POS(p_index);

		p_index = PROC_POS__UP;
		Update__GOTO_POS_OF_PROC_SPEED(p_index);

		return Fnc__GOTO_POS_START(p_variable,p_alarm);
	}

	// DOWN -> MIDDLE
	p_index = PROC_POS__MIDDLE;
	Update__GOTO_POS_OF_PROC_POS(p_index);
	Update__GOTO_POS_OF_PROC_SPEED(p_index);

	return Fnc__GOTO_POS_START(p_variable,p_alarm);
}
int  CObj__MP2100_VIRTUAL
::Call__PROC_UP(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm)
{
	int p_index;

	// DOWN -> MIDDLE
	if(Check__PROCESS_POS__LESS_THAN__MIDDLE() > 0)
	{
		p_index = PROC_POS__MIDDLE;
		Update__GOTO_POS_OF_PROC_POS(p_index);
		Update__GOTO_POS_OF_PROC_SPEED(p_index);

		if(Fnc__GOTO_POS_START(p_variable,p_alarm) < 0)
		{
			return -1;
		}
	}

	// MIDDLE -> UP
	p_index = PROC_POS__UP;
	Update__GOTO_POS_OF_PROC_POS(p_index);
	Update__GOTO_POS_OF_PROC_SPEED(p_index);

	return Fnc__GOTO_POS_START(p_variable,p_alarm);
}


// ...
int  CObj__MP2100_VIRTUAL
::Call__CHECK_ALARM(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm)
{
	return 1;
}

int  CObj__MP2100_VIRTUAL
::Call__CLEAR_ALARM(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm)
{
	return 1;
}
int  CObj__MP2100_VIRTUAL
::Call__CLEAR_SERVO_ALARM(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm)
{
	return 1;
}


// ...
int  CObj__MP2100_VIRTUAL
::Call__SERVO_ON(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm)
{
	return 1;
}

int  CObj__MP2100_VIRTUAL
::Call__SERVO_OFF(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm)
{
	return 1;
}


// ...
int CObj__MP2100_VIRTUAL
::Call__DEF_POSITION(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm)
{
	return 1;
}
