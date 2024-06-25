#include "StdAfx.h"
#include "CObj__MP2100_VIRTUAL.h"


//--------------------------------------------------------------------------------
CObj__MP2100_VIRTUAL::CObj__MP2100_VIRTUAL()
{
	iAXIS_SIZE = 1;
}
CObj__MP2100_VIRTUAL::~CObj__MP2100_VIRTUAL()
{

}

//--------------------------------------------------------------------------------
int CObj__MP2100_VIRTUAL::__DEFINE__CONTROL_MODE(obj, l_mode)
{
	sObject_Name = obj;

	// ...
	{
		ADD__CTRL_VAR(sMODE__INIT,        "INIT");
		ADD__CTRL_VAR(sMODE__HOME,        "HOME");

		ADD__CTRL_VAR(sMODE__CYCLE_DOWN_UP,      "CYCLE.DOWN_UP");
		ADD__CTRL_VAR(sMODE__CYCLE_PROC_DOWN_UP, "CYCLE_PROC.DOWN_UP");

		ADD__CTRL_VAR(sMODE__MOVE_STOP,   "MOVE.STOP");

		ADD__CTRL_VAR(sMODE__TEACH_ABS_UP,    "TEACH.ABS_UP");
		ADD__CTRL_VAR(sMODE__TEACH_ABS_DOWN,  "TEACH.ABS_DOWN");
		
		ADD__CTRL_VAR(sMODE__TEACH_JOG_PLUS,  "TEACH.JOG_PLUS");
		ADD__CTRL_VAR(sMODE__TEACH_JOG_MINUS, "TEACH.JOG_MINUS");

		ADD__CTRL_VAR(sMODE__RETURN,      "RETURN");

		ADD__CTRL_VAR(sMODE__DOWN,        "DOWN");
		ADD__CTRL_VAR(sMODE__DOWN_FAST,   "DOWN.FAST");
		ADD__CTRL_VAR(sMODE__MIDDLE_DOWN, "MIDDLE.DOWN");
		ADD__CTRL_VAR(sMODE__MIODLE_UP,   "MIDDLE.UP");
		ADD__CTRL_VAR(sMODE__UP,          "UP");
		ADD__CTRL_VAR(sMODE__UP_FAST,     "UP.FAST");

		ADD__CTRL_VAR(sMODE__PROC_DOWN,   "PROC.DOWN");
		ADD__CTRL_VAR(sMODE__PROC_MIDDLE, "PROC.MIDDLE");
		ADD__CTRL_VAR(sMODE__PROC_UP,     "PROC.UP");

		ADD__CTRL_VAR(sMODE__CHECK_ALARM, "CHECK.ALARM");
		ADD__CTRL_VAR(sMODE__CLEAR_ALARM, "CLEAR.ALARM");
		ADD__CTRL_VAR(sMODE__CLEAR_SERVO_ALARM, "CLEAR.SERVO_ALARM");

		ADD__CTRL_VAR(sMODE__SERVO_ON,    "SERVO.ON");
		ADD__CTRL_VAR(sMODE__SERVO_OFF,   "SERVO.OFF");

		ADD__CTRL_VAR(sMODE__DEF_POSITION, "DEF.POSITION");
	}
	return 1;
}
int CObj__MP2100_VIRTUAL::__DEFINE__VERSION_HISTORY(version)
{
	version = "JG.Lee \n";

	return 1;
}


// ...
#define  MON_ID__MONITOR				1
#define  MON_ID__VIEW_REQ				2
#define  MON_ID__STABLE					3

#define  APP_DSP__MON_MODE				"IDLE  RUN"
#define  APP_DSP__OFF_ON				"OFF   ON"


int CObj__MP2100_VIRTUAL::__DEFINE__VARIABLE_STD(p_variable)
{
	DECLARE__STD_VARIABLE

	CCommon_Error__DEF_VARIABLE m_err_def_variable;
	p_variable->Link__DEF_VARIABLE__ERROR_FEEDBACK(&m_err_def_variable);

	// ...
	CString str_name;
	CString str_list;
	int i;

	// ...
	{
		str_name = "OBJ.MSG";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__OBJ_MSG, str_name);

		str_name = "OBJ.SUB.MSG";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__OBJ_SUB_MSG, str_name);

		//
		str_name = "OBJ.TIME.COUNT";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__OBJ_TIME_COUNT, str_name);
	}

	// Simulation ...
	{
		str_name = "SIM.HOME.TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "sec", 1, 0, 100, "");
		LINK__VAR_ANALOG_CTRL(aCH__SIM_HOME_TIME, str_name);
		
		str_name = "SIM.MOVE.TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "sec", 1, 0, 100, "");
		LINK__VAR_ANALOG_CTRL(aCH__SIM_MOVE_TIME, str_name);
	}

	// Parameter ...
	{
		str_name = "PARA.GOTO_POS.DATA_PULSE";
		STD__ADD_ANALOG(str_name, "pulse", 0, -10000, 1000000000);
		LINK__VAR_ANALOG_CTRL(aCH__PARA_GOTO_POS_DATA_PULSE, str_name);

		str_name = "PARA.GOTO_POS.SPEED";
		STD__ADD_ANALOG(str_name, "mm/sec",  3, 0, 1000000000);
		LINK__VAR_ANALOG_CTRL(aCH__PARA_GOTO_POS_SPEED, str_name);

		str_name = "PARA.GOTO_POS.ACCEL";
		STD__ADD_ANALOG(str_name, "mm/sec2", 3, 0, 1000000000);
		LINK__VAR_ANALOG_CTRL(aCH__PARA_GOTO_POS_ACCEL, str_name);

		str_name = "PARA.GOTO_POS.DECEL";
		STD__ADD_ANALOG(str_name, "mm/sec2", 3, 0, 1000000000);
		LINK__VAR_ANALOG_CTRL(aCH__PARA_GOTO_POS_DECEL, str_name);
	}

	// Monitoring Channel ...
	{
		// ...
		{
			str_name = "MON.LIFT_PIN.STATE";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_LIFT_PIN_STATE, str_name);

			//
			str_name = "MON.IO.WARNING.DATA_HEXA";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_IO_WARNING_DATA_HEXA, str_name);

			str_name = "MON.IO.WARNING.VIEW_REQ";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_IO_WARNING_VIEW_REQ, str_name);

			//
			str_name = "MON.IO.ALARM.DATA_HEXA";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_IO_ALARM_DATA_HEXA, str_name);

			str_name = "MON.IO.ALARM.VIEW_REQ";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_IO_ALARM_VIEW_REQ, str_name);

			//
			str_name = "MON.IO.LAST_ERROR.VIEW_REQ";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_IO_LAST_ERROR_VIEW_REQ, str_name);

			//
			str_name = "MON.IO.APOS.DATA_HEXA";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_IO_APOS_DATA_HEXA, str_name);

			str_name = "MON.IO.APOS.MM.SET";
			STD__ADD_STRING_WITH_OPTION(str_name, -1, "L", "");
			LINK__VAR_STRING_CTRL(sCH__MON_IO_APOS_MM_SET, str_name);

			str_name = "MON.IO.APOS.MM.GET";
			STD__ADD_STRING_WITH_OPTION(str_name, -1, "L", "");
			LINK__VAR_STRING_CTRL(sCH__MON_IO_APOS_MM_GET, str_name);

			str_name = "MON.IO.APOS.PULSE.SET";
			STD__ADD_STRING_WITH_OPTION(str_name, -1, "L", "");
			LINK__VAR_STRING_CTRL(sCH__MON_IO_APOS_PULSE_SET, str_name);

			str_name = "MON.IO.APOS.PULSE.GET";
			STD__ADD_STRING_WITH_OPTION(str_name, -1, "L", "");
			LINK__VAR_STRING_CTRL(sCH__MON_IO_APOS_PULSE_GET, str_name);

			//
			str_name = "MON.IO.FSPD.DATA_HEXA";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_IO_FSPD_DATA_HEXA, str_name);

			str_name = "MON.IO.FSPD.SPEED.SET";
			STD__ADD_STRING_WITH_OPTION(str_name, -1, "L", "");
			LINK__VAR_STRING_CTRL(sCH__MON_IO_FSPD_SPEED_SET, str_name);

			str_name = "MON.IO.FSPD.SPEED.GET";
			STD__ADD_STRING_WITH_OPTION(str_name, -1, "L", "");
			LINK__VAR_STRING_CTRL(sCH__MON_IO_FSPD_SPEED_GET, str_name);

			//
			str_name = "MON.IO.TORQUE.DATA_HEXA";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_IO_TORQUE_DATA_HEXA, str_name);

			str_name = "MON.IO.TORQUE.GET";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_IO_TORQUE_GET, str_name);

			//
			str_name = "MON.IO.DRIVE_STATUS.DATA_HEXA";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_IO_DRIVE_STATUS_DATA_HEXA, str_name);

			str_name = "MON.IO.DRIVE_STATUS.OPR_READY";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_IO_DRIVE_STATUS_OPR_READY, str_name);
			
			str_name = "MON.IO.DRIVE_STATUS.SERVO_ON";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_IO_DRIVE_STATUS_SERVO_ON, str_name);
			
			str_name = "MON.IO.DRIVE_STATUS.SYSTEM_BUSY";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_IO_DRIVE_STATUS_SYSTEM_BUSY, str_name);
			
			str_name = "MON.IO.DRIVE_STATUS.SERVO_READY";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_IO_DRIVE_STATUS_SERVO_READY, str_name);

			//
			str_name = "MON.ID.MCMD_STATUS.DATA_HEXA";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_ID_MCMD_STATUS_DATA_HEXA, str_name);

			str_name = "MON.ID.MCMD_STATUS.BUSY";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_ID_MCMD_STATUS_BUSY, str_name);

			str_name = "MON.ID.MCMD_STATUS.HOLDL";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_ID_MCMD_STATUS_HOLDL, str_name);

			str_name = "MON.ID.MCMD_STATUS.FAIL";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_ID_MCMD_STATUS_FAIL, str_name);

			str_name = "MON.ID.MCMD_STATUS.ABS_RST";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_ID_MCMD_STATUS_ABS_RST, str_name);

			str_name = "MON.ID.MCMD_STATUS.COMPLETE";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_ID_MCMD_STATUS_COMPLETE, str_name);

			//
			str_name = "MON.IO.SERVO_STATUS.DATA_HEXA";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_IO_SERVO_STATUS_DATA_HEXA, str_name);

			str_name = "MON.IO.SERVO_STATUS.ALM";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_IO_SERVO_STATUS_ALM, str_name);

			str_name = "MON.IO.SERVO_STATUS.WARNING";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_IO_SERVO_STATUS_WARNING, str_name);

			str_name = "MON.IO.SERVO_STATUS.CMDRDY";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_IO_SERVO_STATUS_CMDRDY, str_name);

			str_name = "MON.IO.SERVO_STATUS.SVON";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_IO_SERVO_STATUS_SVON, str_name);

			str_name = "MON.IO.SERVO_STATUS.PON";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_IO_SERVO_STATUS_PON, str_name);

			str_name = "MON.IO.SERVO_STATUS.MLOCK";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_IO_SERVO_STATUS_MLOCK, str_name);

			str_name = "MON.IO.SERVO_STATUS.ZPOINT";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_IO_SERVO_STATUS_ZPOINT, str_name);

			str_name = "MON.IO.SERVO_STATUS.PSET";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_IO_SERVO_STATUS_PSET, str_name);

			str_name = "MON.IO.SERVO_STATUS.DEN";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_IO_SERVO_STATUS_DEN, str_name);

			str_name = "MON.IO.SERVO_STATUS.T_LIM";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_IO_SERVO_STATUS_T_LIM, str_name);

			str_name = "MON.IO.SERVO_STATUS.L_CMP";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_IO_SERVO_STATUS_L_CMP, str_name);

			str_name = "MON.IO.SERVO_STATUS.NEAR";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_IO_SERVO_STATUS_NEAR, str_name);

			str_name = "MON.IO.SERVO_STATUS.P_SOT";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_IO_SERVO_STATUS_P_SOT, str_name);

			str_name = "MON.IO.SERVO_STATUS.N_SOT";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_IO_SERVO_STATUS_N_SOT, str_name);

			//
			str_name = "MON.IO.SERVO_IO.DATA_HEXA";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_IO_SERVO_IO_DATA_HEXA, str_name);

			str_name = "MON.IO.SERVO_IO.P_OT";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_IO_SERVO_IO_P_OT, str_name);

			str_name = "MON.IO.SERVO_IO.N_OT";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_IO_SERVO_IO_N_OT, str_name);

			str_name = "MON.IO.SERVO_IO.DEC";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_IO_SERVO_IO_DEC, str_name);

			str_name = "MON.IO.SERVO_IO.PA";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_IO_SERVO_IO_PA, str_name);

			str_name = "MON.IO.SERVO_IO.PB";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_IO_SERVO_IO_PB, str_name);

			str_name = "MON.IO.SERVO_IO.PC";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_IO_SERVO_IO_PC, str_name);

			str_name = "MON.IO.SERVO_IO.EXIT1";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_IO_SERVO_IO_EXIT1, str_name);

			str_name = "MON.IO.SERVO_IO.EXIT2";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_IO_SERVO_IO_EXIT2, str_name);

			str_name = "MON.IO.SERVO_IO.EXIT3";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_IO_SERVO_IO_EXIT3, str_name);

			str_name = "MON.IO.SERVO_IO.BRK";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_IO_SERVO_IO_BRK, str_name);

			str_name = "MON.IO.SERVO_IO.12";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_IO_SERVO_IO_12, str_name);

			str_name = "MON.IO.SERVO_IO.13";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_IO_SERVO_IO_13, str_name);

			str_name = "MON.IO.SERVO_IO.14";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_IO_SERVO_IO_14, str_name);

			str_name = "MON.IO.SERVO_IO.15";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_IO_SERVO_IO_15, str_name);
		}
	}

	// Command Request ...
	{
		str_name = "CMMD_REQ.SAVE";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__CMMD_REQ_SAVE, str_name);
	}

	// Config Channel ...
	{
		// ...
		{
			str_name = "CFG.RANGE.POSITION.MIN";
			STD__ADD_ANALOG_WITH_X_OPTION(str_name, "mm", 3, 0, 1, "");
			LINK__VAR_ANALOG_CTRL(aCH__CFG_RANGE_POSITION_MIN, str_name);

			str_name = "CFG.RANGE.POSITION.MAX";
			STD__ADD_ANALOG_WITH_X_OPTION(str_name, "mm", 3, 1, 1000, "");
			LINK__VAR_ANALOG_CTRL(aCH__CFG_RANGE_POSITION_MAX, str_name);

			str_name = "CFG.RANGE.POSITION.DEC";
			STD__ADD_ANALOG_WITH_X_OPTION(str_name, "dec", 0, 0, 6, "");
			LINK__VAR_ANALOG_CTRL(aCH__CFG_RANGE_POSITION_DEC, str_name);

			//
			str_name = "CFG.CONVERSION.UNIT.PULSE_TO_MM";
			STD__ADD_ANALOG_WITH_X_OPTION(str_name, "mm/pulse", 3, 0.001, 10, "");
			LINK__VAR_ANALOG_CTRL(aCH__CFG_CONVERSION_UNIT_PULSE_TO_MM, str_name);

			//
			str_name = "CFG.HOME.TIMEOUT";
			STD__ADD_ANALOG_WITH_X_OPTION(str_name, "sec", 0, 1, 600, "");
			LINK__VAR_ANALOG_CTRL(aCH__CFG_HOME_TIMEOUT, str_name);
			
			str_name = "CFG.MOVING.TIMEOUT";
			STD__ADD_ANALOG_WITH_X_OPTION(str_name, "sec", 0, 1, 600, "");
			LINK__VAR_ANALOG_CTRL(aCH__CFG_MOVING_TIMEOUT, str_name);

			// ...
			str_name = "CFG.HOME.METHOD";

			str_list  = STR__HMETHOD_DEC1_C;
			str_list += " ";
			str_list += STR__HMETHOD_ZERO;
			str_list += " ";
			str_list += STR__HMETHOD_DEC1_ZERO;
			str_list += " ";
			str_list += STR__HMETHOD_C;
			str_list += " ";
			str_list += STR__HMETHOD_DEC2_ZERO;
			str_list += " ";
			str_list += STR__HMETHOD_DEC1_L_ZERO;
			str_list += " ";
			str_list += STR__HMETHOD_DEC2_C;
			str_list += " ";
			str_list += STR__HMETHOD_DEC1_L_C;
			str_list += " ";
			str_list += STR__HMETHOD_C_ONLY;
			str_list += " ";
			str_list += STR__HMETHOD_POT_C;
			str_list += " ";
			str_list += STR__HMETHOD_POT_ONLY;
			str_list += " ";
			str_list += STR__HMETHOD_HOMELS_C;
			str_list += " ";
			str_list += STR__HMETHOD_HOMELS_ONLY;
			str_list += " ";
			str_list += STR__HMETHOD_NOT_C;
			str_list += " ";
			str_list += STR__HMETHOD_NOT_ONLY;
			str_list += " ";
			str_list += STR__HMETHOD_INPUT_C;
			str_list += " ";
			str_list += STR__HMETHOD_INPUT_ONLY;

			STD__ADD_DIGITAL_WITH_X_OPTION(str_name, str_list, "");
			LINK__VAR_DIGITAL_CTRL(dCH__CFG_HOME_METHOD, str_name);

			//
			str_name = "CFG.HOME.OFFSET_POS";
			STD__ADD_ANALOG_WITH_X_OPTION(str_name, "mm",  0, 1, 100, "");
			LINK__VAR_ANALOG_CTRL(aCH__CFG_HOME_OFFSET_POS, str_name);

			str_name = "CFG.HOME.SPEED";
			STD__ADD_ANALOG_WITH_X_OPTION(str_name, "mm/sec",  0, 1, 100, "");
			LINK__VAR_ANALOG_CTRL(aCH__CFG_HOME_SPEED, str_name);

			str_name = "CFG.HOME.APPROACH";
			STD__ADD_ANALOG_WITH_X_OPTION(str_name, "mm/sec",  0, 1, 100, "");
			LINK__VAR_ANALOG_CTRL(aCH__CFG_HOME_APPROACH, str_name);

			str_name = "CFG.HOME.CREEP";
			STD__ADD_ANALOG_WITH_X_OPTION(str_name, "mm/sec",  0, 1, 100, "");
			LINK__VAR_ANALOG_CTRL(aCH__CFG_HOME_CREEP, str_name);

			str_name = "CFG.HOME.ACCEL";
			STD__ADD_ANALOG_WITH_X_OPTION(str_name, "mm/sec2",  0, 1, 100, "");
			LINK__VAR_ANALOG_CTRL(aCH__CFG_HOME_ACCEL, str_name);

			str_name = "CFG.HOME.DECEL";
			STD__ADD_ANALOG_WITH_X_OPTION(str_name, "mm/sec2",  0, 1, 100, "");
			LINK__VAR_ANALOG_CTRL(aCH__CFG_HOME_DECEL, str_name);

			//
			str_name = "CFG.HOME.DIRECTION";
			
			str_list  = STR__NEGATIVE;
			str_list += " ";
			str_list += STR__POSITIVE;

			STD__ADD_DIGITAL_WITH_X_OPTION(str_name, str_list, "");
			LINK__VAR_DIGITAL_CTRL(dCH__CFG_HOME_DIRECTION, str_name);

			str_name = "CFG.HOME.OBxx05B";
			STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "OFF ON", "");
			LINK__VAR_DIGITAL_CTRL(dCH__CFG_HOME_OBxx05B, str_name);

			//
			str_name = "CFG.HOME.ZERO.BASE.OFFSET";
			STD__ADD_ANALOG_WITH_X_OPTION(str_name, "mm", 3, -10, 10, "");
			LINK__VAR_ANALOG_CTRL(aCH__CFG_HOME_ZERO_BASE_OFFSET, str_name);
		}

		// Teaching Config ...
		{
			str_name = "ACTIVE.MOVE.DOWN_UP";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__ACTIVE_MOVE_DOWN_UP, str_name);

			str_name = "ACTIVE.PROC.DOWN_UP";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__ACTIVE_PROC_DOWN_UP, str_name);

			//
			str_name = "TEACH.ACTIVE.JOG.UP";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__TEACH_ACTIVE_JOG_UP, str_name);
			
			str_name = "TEACH.ACTIVE.JOG.DOWN";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__TEACH_ACTIVE_JOG_DOWN, str_name);

			//
			str_name = "TEACH.ACTIVE.ABS.UP";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__TEACH_ACTIVE_ABS_UP, str_name);

			str_name = "TEACH.ACTIVE.ABS.DOWN";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__TEACH_ACTIVE_ABS_DOWN, str_name);

			//
			str_name = "CFG.GOTO_POS.UP.ABS_POS";
			STD__ADD_ANALOG_WITH_X_OPTION(str_name, "mm",  3, 0, 10, "");
			LINK__VAR_ANALOG_CTRL(aCH__CFG_GOTO_POS_UP_ABS_POS, str_name);

			str_name = "CFG.GOTO_POS.DOWN.ABS_POS";
			STD__ADD_ANALOG_WITH_X_OPTION(str_name, "mm",  3, 0, 10, "");
			LINK__VAR_ANALOG_CTRL(aCH__CFG_GOTO_POS_DOWN_ABS_POS, str_name);

			//
			str_name = "CFG.TEACH.JOG.SPEED";
			STD__ADD_ANALOG_WITH_X_OPTION(str_name, "mm/sec",  1, 0.1, 1000, "");
			LINK__VAR_ANALOG_CTRL(aCH__CFG_TEACH_JOG_SPEED, str_name);
			
			str_name = "CFG.TEACH.JOG.ACCEL";
			STD__ADD_ANALOG_WITH_X_OPTION(str_name, "mm/sec2", 1, 0.1, 1000000, "");
			LINK__VAR_ANALOG_CTRL(aCH__CFG_TEACH_JOG_ACCEL, str_name);
			
			str_name = "CFG.TEACH.JOG.DECEL";
			STD__ADD_ANALOG_WITH_X_OPTION(str_name, "mm/sec2", 1, 0.1, 1000000, "");
			LINK__VAR_ANALOG_CTRL(aCH__CFG_TEACH_JOG_DECEL, str_name);

			//
			str_name = "CFG.TEACH.ABS.SPEED";
			STD__ADD_ANALOG_WITH_X_OPTION(str_name, "mm/sec",  1, 0.1, 1000, "");
			LINK__VAR_ANALOG_CTRL(aCH__CFG_TEACH_ABS_SPEED, str_name);
			
			str_name = "CFG.TEACH.ABS.ACCEL";
			STD__ADD_ANALOG_WITH_X_OPTION(str_name, "mm/sec2", 1, 0.1, 1000000, "");
			LINK__VAR_ANALOG_CTRL(aCH__CFG_TEACH_ABS_ACCEL, str_name);
			
			str_name = "CFG.TEACH.ABS.DECEL";
			STD__ADD_ANALOG_WITH_X_OPTION(str_name, "mm/sec2", 1, 0.1, 1000000, "");
			LINK__VAR_ANALOG_CTRL(aCH__CFG_TEACH_ABS_DECEL, str_name);
		}

		// PROCESS.POS ...
		for(i=0; i<CFG_SIZE__PROC_POS; i++)
		{
			CString name_sns;
			CString name_data;
			CString name_speed;
			CString name_accel;
			CString name_decel;

			if(i == PROC_POS__DOWN)
			{
				name_sns   = "APP.PROC_POS.DOWN.SNS";
				name_data  = "CFG.PROC_POS.DOWN.DATA_POS";
				name_speed = "CFG.PROC_POS.DOWN.SPEED";
				name_accel = "CFG.PROC_POS.DOWN.ACCEL";
				name_decel = "CFG.PROC_POS.DOWN.DECEL";
			}
			else if(i == PROC_POS__MIDDLE)
			{
				name_sns   = "APP.PROC_POS.MIDDLE.SNS";
				name_data  = "CFG.PROC_POS.MIDDLE.DATA_POS";
				name_speed = "CFG.PROC_POS.MIDDLE.SPEED";
				name_accel = "CFG.PROC_POS.MIDDLE.ACCEL";
				name_decel = "CFG.PROC_POS.MIDDLE.DECEL";
			}
			else if(i == PROC_POS__UP)
			{
				name_sns   = "APP.PROC_POS.UP.SNS";
				name_data  = "CFG.PROC_POS.UP.DATA_POS";
				name_speed = "CFG.PROC_POS.UP.SPEED";
				name_accel = "CFG.PROC_POS.UP.ACCEL";
				name_decel = "CFG.PROC_POS.UP.DECEL";
			}
			else
			{
				continue;
			}

			STD__ADD_STRING(name_sns);
			LINK__VAR_STRING_CTRL(sCH__APP_PROC_POS_X_SNS[i], name_sns);

			STD__ADD_ANALOG_WITH_X_OPTION(name_data,  "mm",      3, -1000, 1000, "");
			LINK__VAR_ANALOG_CTRL(aCH__CFG_PROC_POS_X_DATA_POS[i], name_data);

			STD__ADD_ANALOG_WITH_X_OPTION(name_speed, "mm/sec",  1, 0.1, 1000, "");
			LINK__VAR_ANALOG_CTRL(aCH__CFG_PROC_POS_X_SPEED[i], name_speed);

			STD__ADD_ANALOG_WITH_X_OPTION(name_accel, "mm/sec2", 1, 0.1, 1000000, "");
			LINK__VAR_ANALOG_CTRL(aCH__CFG_PROC_POS_X_ACCEL[i], name_accel);

			STD__ADD_ANALOG_WITH_X_OPTION(name_decel, "mm/sec2", 1, 0.1, 1000000, "");
			LINK__VAR_ANALOG_CTRL(aCH__CFG_PROC_POS_X_DECEL[i], name_decel);
		}

		// TRNASFER.POS ...
		for(i=0; i<CFG_SIZE__TRANSFER_POS; i++)
		{
			CString name_sns;
			CString name_data;
			CString name_speed;
			CString name_accel;
			CString name_decel;

			if(i == TRANSFER_POS__DOWN)
			{
				name_sns   = "APP.TRANSFER_POS.DOWN.SNS";
				name_data  = "CFG.TRANSFER_POS.DOWN.DATA_POS";
				name_speed = "CFG.TRANSFER_POS.DOWN.SPEED";
				name_accel = "CFG.TRANSFER_POS.DOWN.ACCEL";
				name_decel = "CFG.TRANSFER_POS.DOWN.DECEL";
			}
			else if((i == TRANSFER_POS__MIDDLE_DOWN)
			     || (i == TRANSFER_POS__MIDDLE_UP  ))
			{
				if(i == TRANSFER_POS__MIDDLE_DOWN)
				{
					name_sns  = "APP.TRANSFER_POS.MIDDLE_DOWN.SNS";
					name_data = "CFG.TRANSFER_POS.MIDDLE_DOWN.DATA_POS";
				}
				else if(i == TRANSFER_POS__MIDDLE_UP)
				{
					name_sns  = "APP.TRANSFER_POS.MIDDLE_UP.SNS";
					name_data = "CFG.TRANSFER_POS.MIDDLE_UP.DATA_POS";
				}
				
				name_speed = "CFG.TRANSFER_POS.MIDDLE.SPEED";
				name_accel = "CFG.TRANSFER_POS.MIDDLE.ACCEL";
				name_decel = "CFG.TRANSFER_POS.MIDDLE.DECEL";
			}
			else if(i == TRANSFER_POS__UP)
			{
				name_sns   = "APP.TRANSFER_POS.UP.SNS";
				name_data  = "CFG.TRANSFER_POS.UP.DATA_POS";
				name_speed = "CFG.TRANSFER_POS.UP.SPEED";
				name_accel = "CFG.TRANSFER_POS.UP.ACCEL";
				name_decel = "CFG.TRANSFER_POS.UP.DECEL";
			}
			else
			{
				continue;
			}

			STD__ADD_STRING(name_sns);
			LINK__VAR_STRING_CTRL(sCH__APP_TRANSFER_POS_X_SNS[i], name_sns);

			STD__ADD_ANALOG_WITH_X_OPTION(name_data,  "mm",      0, -100, 1000, "");
			LINK__VAR_ANALOG_CTRL(aCH__CFG_TRANSFER_POS_X_DATA_POS[i], name_data);

			STD__ADD_ANALOG_WITH_X_OPTION(name_speed, "mm/sec",  1, 0.1, 1000, "");
			LINK__VAR_ANALOG_CTRL(aCH__CFG_TRANSFER_POS_X_SPEED[i], name_speed);

			STD__ADD_ANALOG_WITH_X_OPTION(name_accel, "mm/sec2", 1, 0.1, 1000000, "");
			LINK__VAR_ANALOG_CTRL(aCH__CFG_TRANSFER_POS_X_ACCEL[i], name_accel);

			STD__ADD_ANALOG_WITH_X_OPTION(name_decel, "mm/sec2", 1, 0.1, 1000000, "");
			LINK__VAR_ANALOG_CTRL(aCH__CFG_TRANSFER_POS_X_DECEL[i], name_decel);
		}

		// CYCLE TEST : TRANSFER POS ...
		{
			str_name = "CFG.MOVE.DOWN_UP.CYCLE";
			STD__ADD_ANALOG_WITH_X_OPTION(str_name, "num", 0, 1, 9999, "");
			LINK__VAR_ANALOG_CTRL(aCH__CFG_MOVE_DOWN_UP_CYCLE, str_name);
		
			str_name = "RESULT.MOVE.DOWN_UP.CYCLE";
			STD__ADD_STRING_WITH_X_OPTION(str_name, "");
			LINK__VAR_STRING_CTRL(sCH__RESULT_MOVE_DOWN_UP_CYCLE, str_name);
		
			//
			str_name = "RESULT.MOVE.DOWN_UP.AVG.UP";
			STD__ADD_STRING_WITH_X_OPTION(str_name, "");
			LINK__VAR_STRING_CTRL(sCH__RESULT_MOVE_DOWN_UP_AVG_UP, str_name);
			
			str_name = "RESULT.MOVE.DOWN_UP.MIN.UP";
			STD__ADD_STRING_WITH_X_OPTION(str_name, "");
			LINK__VAR_STRING_CTRL(sCH__RESULT_MOVE_DOWN_UP_MIN_UP, str_name);
			
			str_name = "RESULT.MOVE.DOWN_UP.MAX.UP";
			STD__ADD_STRING_WITH_X_OPTION(str_name, "");
			LINK__VAR_STRING_CTRL(sCH__RESULT_MOVE_DOWN_UP_MAX_UP, str_name);

			//
			str_name = "RESULT.MOVE.DOWN_UP.AVG.DOWN";
			STD__ADD_STRING_WITH_X_OPTION(str_name, "");
			LINK__VAR_STRING_CTRL(sCH__RESULT_MOVE_DOWN_UP_AVG_DOWN, str_name);
			
			str_name = "RESULT.MOVE.DOWN_UP.MIN.DOWN";
			STD__ADD_STRING_WITH_X_OPTION(str_name, "");
			LINK__VAR_STRING_CTRL(sCH__RESULT_MOVE_DOWN_UP_MIN_DOWN, str_name);
			
			str_name = "RESULT.MOVE.DOWN_UP.MAX.DOWN";
			STD__ADD_STRING_WITH_X_OPTION(str_name, "");
			LINK__VAR_STRING_CTRL(sCH__RESULT_MOVE_DOWN_UP_MAX_DOWN, str_name);
		}
		// CYCLE TEST : PROCESS POS ...
		{
			str_name = "CFG.PROC.DOWN_UP.CYCLE";
			STD__ADD_ANALOG_WITH_X_OPTION(str_name, "num", 0, 1, 9999, "");
			LINK__VAR_ANALOG_CTRL(aCH__CFG_PROC_DOWN_UP_CYCLE, str_name);

			str_name = "RESULT.PROC.DOWN_UP.CYCLE";
			STD__ADD_STRING_WITH_X_OPTION(str_name, "");
			LINK__VAR_STRING_CTRL(sCH__RESULT_PROC_DOWN_UP_CYCLE, str_name);

			//
			str_name = "RESULT.PROC.DOWN_UP.AVG.UP";
			STD__ADD_STRING_WITH_X_OPTION(str_name, "");
			LINK__VAR_STRING_CTRL(sCH__RESULT_PROC_DOWN_UP_AVG_UP, str_name);

			str_name = "RESULT.PROC.DOWN_UP.MIN.UP";
			STD__ADD_STRING_WITH_X_OPTION(str_name, "");
			LINK__VAR_STRING_CTRL(sCH__RESULT_PROC_DOWN_UP_MIN_UP, str_name);

			str_name = "RESULT.PROC.DOWN_UP.MAX.UP";
			STD__ADD_STRING_WITH_X_OPTION(str_name, "");
			LINK__VAR_STRING_CTRL(sCH__RESULT_PROC_DOWN_UP_MAX_UP, str_name);

			//
			str_name = "RESULT.PROC.DOWN_UP.AVG.DOWN";
			STD__ADD_STRING_WITH_X_OPTION(str_name, "");
			LINK__VAR_STRING_CTRL(sCH__RESULT_PROC_DOWN_UP_AVG_DOWN, str_name);

			str_name = "RESULT.PROC.DOWN_UP.MIN.DOWN";
			STD__ADD_STRING_WITH_X_OPTION(str_name, "");
			LINK__VAR_STRING_CTRL(sCH__RESULT_PROC_DOWN_UP_MIN_DOWN, str_name);

			str_name = "RESULT.PROC.DOWN_UP.MAX.DOWN";
			STD__ADD_STRING_WITH_X_OPTION(str_name, "");
			LINK__VAR_STRING_CTRL(sCH__RESULT_PROC_DOWN_UP_MAX_DOWN, str_name);
		}
	}

	// Stable Channel ...
	{
		// ...
		{
			str_name = "CFG.WARNING.LIMIT";
			STD__ADD_ANALOG_WITH_X_OPTION(str_name, "%", 1, 0.0, 100.0, "");
			LINK__VAR_ANALOG_CTRL(aCH__CFG_WARNING_LIMIT, str_name);

			str_name = "CFG.FAULT.LIMIT";
			STD__ADD_ANALOG_WITH_X_OPTION(str_name, "%", 1, 0.0, 100.0, "");
			LINK__VAR_ANALOG_CTRL(aCH__CFG_FAULT_LIMIT, str_name);

			//
			str_name = "CFG.CHECKING_TIME.FOR.STABLE";
			STD__ADD_ANALOG_WITH_X_OPTION(str_name, "sec", 1, 0.0, 100.0, "");
			LINK__VAR_ANALOG_CTRL(aCH__CFG_CHECKING_TIME_FOR_STABLE, str_name);

			str_name = "CFG.CHECK_TIMEOUT.FOR.ERROR";
			STD__ADD_ANALOG_WITH_X_OPTION(str_name, "sec", 1, 0.0, 100.0, "");
			LINK__VAR_ANALOG_CTRL(aCH__CFG_CHECK_TIMEOUT_FOR_ERROR, str_name);

			str_name = "CFG.STABLE_TIME.FOR.SETPINT_CHANGE";
			STD__ADD_ANALOG_WITH_X_OPTION(str_name, "sec", 1, 0.0, 100.0, "");
			LINK__VAR_ANALOG_CTRL(aCH__CFG_STABLE_TIME_FOR_SETPINT_CHANGE, str_name);
		}

		for(i=0; i<CFG__AXIS_SIZE; i++)
		{
			int id = i + 1;

			str_name.Format("MON.AXIS%1d.STABLE.STATE", id);
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_AXIS_STABLE_STATE[i], str_name);

			str_name.Format("MON.AXIS%1d.STABLE.TIME", id);
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_AXIS_STABLE_TIME[i], str_name);

			str_name.Format("MON.AXIS%1d.FAULT.STATE", id);
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_AXIS_FAULT_STATE[i], str_name);

			str_name.Format("MON.AXIS%1d.FAULT.TIME", id);
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_AXIS_FAULT_TIME[i], str_name);

			str_name.Format("MON.AXIS%1d.CHANGE.STATE", id);
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_AXIS_CHANGE_STATE[i], str_name);

			str_name.Format("MON.AXIS%1d.CHANGE.TIME", id);
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_AXIS_CHANGE_TIME[i], str_name);
		}

		// ...
		{
			str_name = "MON.STABLE.FLAG";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_STABLE_FLAG, str_name);

			str_name = "MON.FAULT.FLAG";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_FAULT_FLAG, str_name);
		}
	}

	// ...
	{
		p_variable->Add__MONITORING_PROC(3.0, MON_ID__MONITOR);
		p_variable->Add__MONITORING_PROC(3.0, MON_ID__VIEW_REQ);
		// p_variable->Add__MONITORING_PROC(3.0, MON_ID__STABLE);
	}
	return 1;
}
int CObj__MP2100_VIRTUAL::__DEFINE__ALARM(p_alarm)
{
	DECLARE__ALARM;

	// ...
	CString title;
	CString str_bff;

	title.Format("%s - ",sObject_Name);

	CString alarm_title;
	CString alarm_msg;
	CStringArray l_act;
	int alarm_id;

	// ...
	{
		alarm_id = ALID__OFFLINE_ALARM;

		alarm_title  = title;
		alarm_title += "Offline !";

		alarm_msg = "";

		l_act.RemoveAll();
		l_act.Add(STR__CLEAR);

		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__HOME_TIMEOUT;

		alarm_title  = title;
		alarm_title += "Home Timeout !";

		alarm_msg = "";

		l_act.RemoveAll();
		l_act.Add(STR__RETRY);
		l_act.Add(STR__ABORT);

		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}
	// ...
	{
		alarm_id = ALID__MOVING_TIMEOUT;

		alarm_title  = title;
		alarm_title += "Moving Timeout !";

		alarm_msg = "";

		l_act.RemoveAll();
		l_act.Add(STR__RETRY);
		l_act.Add(STR__ABORT);

		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__CHUCK_STATE__NOT_DECHUCKED;

		alarm_title  = title;
		alarm_title += "Chuck state is not dechucked !";

		alarm_msg = "Chuck 상태가 Dechuck 상태인지? 확인 바랍니다.";

		l_act.RemoveAll();
		l_act.Add(STR__RETRY);
		l_act.Add(STR__ABORT);

		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		int i_limit = 20;
		int i;

		for(i=0; i<i_limit; i++)
		{
			int post_check = -1;

				 if(i == 0)		alarm_id = ALID__ERROR_ymcOpenController;
			else if(i == 1)		alarm_id = ALID__ERROR_ymcSetAPITimeoutValue;
			else if(i == 2)		alarm_id = ALID__ERROR_ymcClearAllAxes;
			else if(i == 3)		alarm_id = ALID__ERROR_ymcDeclareAxis;
			else if(i == 4)		alarm_id = ALID__ERROR_ymcDeclareDevice;
			else if(i == 5)		alarm_id = ALID__ERROR_ymcClearServoAlarm;
			else if(i == 6)		alarm_id = ALID__ERROR_ymcServoControl;
			else if(i == 7)		alarm_id = ALID__ERROR_hDevice_NULL;
			else if(i == 8)		alarm_id = ALID__ERROR_ymcMoveHomePosition;
			else if(i == 9)		alarm_id = ALID__ERROR_ymcMoveDriverPositioning;
			else if(i == 10)	alarm_id = ALID__ERROR_ymcMoveJOG;
			else if(i == 11)	alarm_id = ALID__ERROR_ymcStopJOG;
			else if(i == 12)	alarm_id = ALID__ERROR_ymcGetAlarm;
			else if(i == 13)	alarm_id = ALID__ERROR_ymcClearAlarm;
			else if(i == 14)	alarm_id = ALID__ERROR_ymcSetMotionParameterValue;
			else if(i == 15)
			{
				alarm_id = ALID__ERROR_ymcGetMotionParameterValue;
				post_check = 1;
			}
			else if(i == 16)	alarm_id = ALID__ERROR_ymcStopMotion;
			else if(i == 17)	alarm_id = ALID__ERROR_ymcDefinePosition;
			else				continue;

			str_bff.Format("ERROR (%1d)", alarm_id);
			alarm_title  = title;
			alarm_title += str_bff;

			alarm_msg = "";

			l_act.RemoveAll();
			if(post_check > 0)
			{
				l_act.Add(STR__CLEAR);
			}
			else
			{
				l_act.Add(STR__RETRY);
				l_act.Add(STR__ABORT);
			}

			ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
		}
	}

	return 1;
}


// ...
#define APP_DSP__COMM_STATUS            "OFFLINE  ONLINE"
#define APP_DSP__HEATER_ID              "1 2 3 4 5"


int CObj__MP2100_VIRTUAL::__DEFINE__VARIABLE_IO(p_io_variable)
{
	DECLARE__IO_VARIABLE;

	CString str_name;

	/*
	// INFO ...
	{
		str_name = "do.INFO.MAIN.UPDATE";
		IO__ADD_DIGITAL_WRITE(str_name, APP_DSP__HEATER_ID);
		LINK__IO_VAR_DIGITAL_CTRL(doCH__INFO_MAIN_UPDATE, str_name);

		str_name = "do.INFO.SUB.UPDATE";
		IO__ADD_DIGITAL_WRITE(str_name, APP_DSP__HEATER_ID);
		LINK__IO_VAR_DIGITAL_CTRL(doCH__INFO_SUB_UPDATE, str_name);
	}
	*/

	return 1;
}


//--------------------------------------------------------------------------------
int CObj__MP2100_VIRTUAL::__INITIALIZE__OBJECT(p_variable,p_ext_obj_create)
{
	// ...
	{
		CString file_name;
		CString log_msg;

		// APP Log ...
		{
			file_name.Format("%s__APP.log", sObject_Name);

			log_msg  = "\n\n";
			log_msg += "//------------------------------------------------------------------------";

			xI__APP_LOG_CTRL->CREATE__SUB_DIRECTORY(sObject_Name);
			xI__APP_LOG_CTRL->SET__PROPERTY(file_name,12*1,60);

			xI__APP_LOG_CTRL->DISABLE__TIME_LOG();
			xI__APP_LOG_CTRL->WRITE__LOG(log_msg);

			xI__APP_LOG_CTRL->ENABLE__TIME_LOG();
			xI__APP_LOG_CTRL->WRITE__LOG("   START   \n");
		}

		// DRV Log ...
		{
			file_name.Format("%s__DRV.log", sObject_Name);

			log_msg  = "\n\n";
			log_msg += "//------------------------------------------------------------------------";

			xI__DRV_LOG_CTRL->CREATE__SUB_DIRECTORY(sObject_Name);
			xI__DRV_LOG_CTRL->SET__PROPERTY(file_name,12*1,60);

			xI__DRV_LOG_CTRL->DISABLE__TIME_LOG();
			xI__DRV_LOG_CTRL->WRITE__LOG(log_msg);

			xI__DRV_LOG_CTRL->ENABLE__TIME_LOG();
			xI__DRV_LOG_CTRL->WRITE__LOG("   START   \n");
		}
	}

	// ...
	CString def_name;
	CString ch_name;
	CString obj_name;
	CString var_name;

	// LINK : IO_Chammel
	{
		def_name = "CH__CFG_INTERLOCK_USE_FLAG";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, ch_name);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(ch_name, obj_name,var_name);
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__CFG_INTERLOCK_USE_FLAG, obj_name,var_name);

		//
		def_name = "CH__LIFT_PIN_STATE";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, ch_name);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(ch_name, obj_name,var_name);
		LINK__EXT_VAR_STRING_CTRL(sEXT_CH__LIFT_PIN_STATE, obj_name,var_name);
	}

	// ...
	{
		SCX__SEQ_INFO x_seq_info;

		iSIM_FLAG = x_seq_info->Is__SIMULATION_MODE();
	}
	return 1;
}
int CObj__MP2100_VIRTUAL::__INITIALIZE__IO(p_io_para)
{
	/*
	// ...
	{
		CString para_cmmd;
		CString para_data;

		// 1.
		{
			para_cmmd = "COM_PORT";

			if(p_io_para->Get__PARAMETER_DATA(para_cmmd, para_data) > 0)
			{
				com_port = atoi(para_data);
			}
		}
	}
	*/

	return 1;
}


//--------------------------------------------------------------------------------
int CObj__MP2100_VIRTUAL::__CALL__CONTROL_MODE(mode, p_debug, p_variable, p_alarm)
{
LOOP_RETRY:

	// ...
	CString msg;
	int flag = 1;

	// ...
	{
		msg.Format("%s Started...",mode);
		sCH__OBJ_MSG->Set__DATA(msg);
		sCH__OBJ_SUB_MSG->Set__DATA("");

		Write__APP_LOG(msg);
	}

	if(flag > 0)
	{
			 IF__CTRL_MODE(sMODE__INIT)					flag = Call__INIT(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__HOME)					flag = Call__HOME(p_variable,p_alarm);
		
		ELSE_IF__CTRL_MODE(sMODE__CYCLE_DOWN_UP)
		{
			sCH__ACTIVE_MOVE_DOWN_UP->Set__DATA(STR__YES);

			flag = Call__CYCLE_DOWN_UP(p_variable,p_alarm);

			sCH__ACTIVE_MOVE_DOWN_UP->Set__DATA("");
		}
		ELSE_IF__CTRL_MODE(sMODE__CYCLE_PROC_DOWN_UP)
		{
			sCH__ACTIVE_PROC_DOWN_UP->Set__DATA(STR__YES);

			flag = Call__CYCLE_PROC_DOWN_UP(p_variable,p_alarm);

			sCH__ACTIVE_PROC_DOWN_UP->Set__DATA("");
		}

		ELSE_IF__CTRL_MODE(sMODE__MOVE_STOP)			flag = Call__MOVE_STOP(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__TEACH_ABS_UP)			flag = Call__TEACH_ABS_START(p_variable,p_alarm,  1);
		ELSE_IF__CTRL_MODE(sMODE__TEACH_ABS_DOWN)		flag = Call__TEACH_ABS_START(p_variable,p_alarm, -1);

		ELSE_IF__CTRL_MODE(sMODE__TEACH_JOG_PLUS)		flag = Call__TEACH_JOG_START(p_variable,p_alarm,  1);
		ELSE_IF__CTRL_MODE(sMODE__TEACH_JOG_MINUS)		flag = Call__TEACH_JOG_START(p_variable,p_alarm, -1);

		ELSE_IF__CTRL_MODE(sMODE__RETURN)				flag = 1;

		ELSE_IF__CTRL_MODE(sMODE__DOWN)					flag = Call__DOWN(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__DOWN_FAST)			flag = Call__DOWN_FAST(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__MIDDLE_DOWN)			flag = Call__MIDDLE_DOWN(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__MIODLE_UP)			flag = Call__MIDDLE_UP(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__UP)					flag = Call__UP(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__UP_FAST)				flag = Call__UP_FAST(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__PROC_DOWN)			flag = Call__PROC_DOWN(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__PROC_MIDDLE)			flag = Call__PROC_MIDDLE(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__PROC_UP)				flag = Call__PROC_UP(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__CHECK_ALARM)			flag = Call__CHECK_ALARM(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__CLEAR_ALARM)			flag = Call__CLEAR_ALARM(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__CLEAR_SERVO_ALARM)	flag = Call__CLEAR_SERVO_ALARM(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__SERVO_ON)				flag = Call__SERVO_ON(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__SERVO_OFF)			flag = Call__SERVO_OFF(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__DEF_POSITION)			flag = Call__DEF_POSITION(p_variable,p_alarm);
	}

	if((flag < 0)||(p_variable->Check__CTRL_ABORT() > 0))
	{
		msg.Format("%s Aborted (%1d)", mode, flag);
	}
	else
	{
		msg.Format("%s Completed (%1d)", mode, flag);
	}	

	// ...
	{
		aCH__PARA_GOTO_POS_SPEED->Set__VALUE(0);
	}

	// ...
	{
		sCH__OBJ_MSG->Set__DATA(msg);
		sCH__OBJ_SUB_MSG->Set__DATA("");

		Write__APP_LOG(msg);
	}
	return flag;
}

int CObj__MP2100_VIRTUAL::__CALL__MONITORING(id, p_variable, p_alarm)
{
	if(id == MON_ID__MONITOR)			return Mon__MONITOR(p_variable,p_alarm);
	if(id == MON_ID__VIEW_REQ)			return Mon__VIEW_REQ(p_variable,p_alarm);
	if(id == MON_ID__STABLE)			return Mon__STABLE(p_variable,p_alarm);

	return 1;
}
