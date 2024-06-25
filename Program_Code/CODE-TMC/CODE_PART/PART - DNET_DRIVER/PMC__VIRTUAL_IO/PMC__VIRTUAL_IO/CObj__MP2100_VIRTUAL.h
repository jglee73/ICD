#pragma once

#include "Interface_Code.h"
#include "CCommon_Error.h"

#include "CObj__MP2100_VIRTUAL__ALID.h"
#include "CObj__MP2100_VIRTUAL__DEF.h"


class CObj__MP2100_VIRTUAL : public __IOBJ__IO_TYPE
{
private:
	//-------------------------------------------------------------------------
	CString	sObject_Name;

	// ...
	int iSIM_FLAG;

	int iAXIS_SIZE;
	//


	//-------------------------------------------------------------------------
	SCX__USER_LOG_CTRL xI__APP_LOG_CTRL;
	SCX__USER_LOG_CTRL xI__DRV_LOG_CTRL;

	void Write__APP_LOG(const CString& log_msg);
	void Write__DRV_LOG(const CString& log_msg);


	//-------------------------------------------------------------------------
	//  INTERNAL PROPERTY

	// ...
	CX__VAR_STRING_CTRL  sCH__OBJ_MSG;
	CX__VAR_STRING_CTRL  sCH__OBJ_SUB_MSG;

	CX__VAR_STRING_CTRL  sCH__OBJ_TIME_COUNT;

	// Simulation ...
	CX__VAR_ANALOG_CTRL  aCH__SIM_HOME_TIME;
	CX__VAR_ANALOG_CTRL  aCH__SIM_MOVE_TIME;

	// Parameter ...
	CX__VAR_ANALOG_CTRL  aCH__PARA_GOTO_POS_DATA_PULSE;
	CX__VAR_ANALOG_CTRL  aCH__PARA_GOTO_POS_SPEED;
	CX__VAR_ANALOG_CTRL  aCH__PARA_GOTO_POS_ACCEL;
	CX__VAR_ANALOG_CTRL  aCH__PARA_GOTO_POS_DECEL;

	// Monitoring Channel ...
	CX__VAR_STRING_CTRL  sCH__MON_LIFT_PIN_STATE;

	CX__VAR_STRING_CTRL  sCH__MON_IO_WARNING_DATA_HEXA;
	CX__VAR_STRING_CTRL  sCH__MON_IO_WARNING_VIEW_REQ;

	CX__VAR_STRING_CTRL  sCH__MON_IO_ALARM_DATA_HEXA;
	CX__VAR_STRING_CTRL  sCH__MON_IO_ALARM_VIEW_REQ;

	CX__VAR_STRING_CTRL  sCH__MON_IO_LAST_ERROR_VIEW_REQ;

	//
	CX__VAR_STRING_CTRL  sCH__MON_IO_APOS_DATA_HEXA;				// Machine Coordinate Feedback Position (Pulse)
	CX__VAR_STRING_CTRL  sCH__MON_IO_APOS_MM_SET;					//  * Position (mm)
	CX__VAR_STRING_CTRL  sCH__MON_IO_APOS_MM_GET;					//  * Position (mm)
	CX__VAR_STRING_CTRL  sCH__MON_IO_APOS_PULSE_SET;				//  * Position (pulse)
	CX__VAR_STRING_CTRL  sCH__MON_IO_APOS_PULSE_GET;				//  * Position (pulse)

	CX__VAR_STRING_CTRL  sCH__MON_IO_FSPD_DATA_HEXA;				// Feedback Speed
	CX__VAR_STRING_CTRL  sCH__MON_IO_FSPD_SPEED_SET;				//  * Speed (unit/sec)
	CX__VAR_STRING_CTRL  sCH__MON_IO_FSPD_SPEED_GET;				//  * Speed (unit/sec)

	CX__VAR_STRING_CTRL  sCH__MON_IO_TORQUE_DATA_HEXA;				// Torque
	CX__VAR_STRING_CTRL  sCH__MON_IO_TORQUE_GET;					//  * Torque

	CX__VAR_STRING_CTRL  sCH__MON_IO_DRIVE_STATUS_DATA_HEXA;		// Drive Status : Data Hexa
	CX__VAR_STRING_CTRL  sCH__MON_IO_DRIVE_STATUS_OPR_READY;		//  0 : Operation Ready
	CX__VAR_STRING_CTRL  sCH__MON_IO_DRIVE_STATUS_SERVO_ON;			//  1 : Servo ON
	CX__VAR_STRING_CTRL  sCH__MON_IO_DRIVE_STATUS_SYSTEM_BUSY;		//  2 : System Busy
	CX__VAR_STRING_CTRL  sCH__MON_IO_DRIVE_STATUS_SERVO_READY;		//  3 : Servo Ready

	CX__VAR_STRING_CTRL  sCH__MON_ID_MCMD_STATUS_DATA_HEXA;			// Servo Module Command Status : Data Hexa
	CX__VAR_STRING_CTRL  sCH__MON_ID_MCMD_STATUS_BUSY;				//  0 : Command Executing (BUSY)
	CX__VAR_STRING_CTRL  sCH__MON_ID_MCMD_STATUS_HOLDL;				//  1 : Command Hold Completed (HOLDL)
	CX__VAR_STRING_CTRL  sCH__MON_ID_MCMD_STATUS_FAIL;				//  3 : Command Error Occurrence (FAIL)
	CX__VAR_STRING_CTRL  sCH__MON_ID_MCMD_STATUS_ABS_RST;			//  7 : Reset Absolute Encoder Completed (ABS_RST)
	CX__VAR_STRING_CTRL  sCH__MON_ID_MCMD_STATUS_COMPLETE;			//  8 : Command Execution Completed (COMPLETE)

	CX__VAR_STRING_CTRL  sCH__MON_IO_SERVO_STATUS_DATA_HEXA;		// Network Servo Status : Data Hexa
	CX__VAR_STRING_CTRL  sCH__MON_IO_SERVO_STATUS_ALM;				//  0 : Alarm Occurred (ALM)
	CX__VAR_STRING_CTRL  sCH__MON_IO_SERVO_STATUS_WARNING;			//  1 : Warning Occurred (WARNING)
	CX__VAR_STRING_CTRL  sCH__MON_IO_SERVO_STATUS_CMDRDY;			//  2 : Command Ready (CMDRDY)
	CX__VAR_STRING_CTRL  sCH__MON_IO_SERVO_STATUS_SVON;				//  3 : Servo ON (SVON)
	CX__VAR_STRING_CTRL  sCH__MON_IO_SERVO_STATUS_PON;				//  4 : Main Power ON (PON)
	CX__VAR_STRING_CTRL  sCH__MON_IO_SERVO_STATUS_MLOCK;			//  5 : Machine Lock (MLOCK) 
	CX__VAR_STRING_CTRL  sCH__MON_IO_SERVO_STATUS_ZPOINT;			//  6 : Zero Point Position (ZPOINT)
	CX__VAR_STRING_CTRL  sCH__MON_IO_SERVO_STATUS_PSET;				//  7 : Position Completed (PSET)
	CX__VAR_STRING_CTRL  sCH__MON_IO_SERVO_STATUS_DEN;				//  8 : Distribution Completed (DEN)
	CX__VAR_STRING_CTRL  sCH__MON_IO_SERVO_STATUS_T_LIM;			//  9 : Torque Being Limited (T_LIM)
	CX__VAR_STRING_CTRL  sCH__MON_IO_SERVO_STATUS_L_CMP;			// 10 : Latch Completed (L_CMP)
	CX__VAR_STRING_CTRL  sCH__MON_IO_SERVO_STATUS_NEAR;				// 11 : Position Proximity (NEAR)
	CX__VAR_STRING_CTRL  sCH__MON_IO_SERVO_STATUS_P_SOT;			// 12 : Positive Software Limit (P_SOT)
	CX__VAR_STRING_CTRL  sCH__MON_IO_SERVO_STATUS_N_SOT;			// 13 : Negative Software Limit (N_SOT)

	CX__VAR_STRING_CTRL  sCH__MON_IO_SERVO_IO_DATA_HEXA;			// Network Servo I/O Monitor : Data Hexa
	CX__VAR_STRING_CTRL  sCH__MON_IO_SERVO_IO_P_OT;					//  0 : 정회전 구동 금지 입력
	CX__VAR_STRING_CTRL  sCH__MON_IO_SERVO_IO_N_OT;					//  1 : 역회전 구동 금지 입력
	CX__VAR_STRING_CTRL  sCH__MON_IO_SERVO_IO_DEC;					//  2 : 원점 복귀 감속 리밋 스위치 입력
	CX__VAR_STRING_CTRL  sCH__MON_IO_SERVO_IO_PA;					//  3 : 인코더 A상 입력
	CX__VAR_STRING_CTRL  sCH__MON_IO_SERVO_IO_PB;					//  4 : 인코더 B상 입력
	CX__VAR_STRING_CTRL  sCH__MON_IO_SERVO_IO_PC;					//  5 : 인코더 C상 입력
	CX__VAR_STRING_CTRL  sCH__MON_IO_SERVO_IO_EXIT1;				//  6 : 제 1외부 래치 입력
	CX__VAR_STRING_CTRL  sCH__MON_IO_SERVO_IO_EXIT2;				//  7 : 제 2외부 래치 입력
	CX__VAR_STRING_CTRL  sCH__MON_IO_SERVO_IO_EXIT3;				//  8 : 제 3외부 래치 입력
	CX__VAR_STRING_CTRL  sCH__MON_IO_SERVO_IO_BRK;					//  9 : 브레이크 출력
	CX__VAR_STRING_CTRL  sCH__MON_IO_SERVO_IO_12;					// 12 : Pn81E.0에서 선텍된 CN1 입력 신호
	CX__VAR_STRING_CTRL  sCH__MON_IO_SERVO_IO_13;					// 13 : Pn81E.1에서 선텍된 CN1 입력 신호
	CX__VAR_STRING_CTRL  sCH__MON_IO_SERVO_IO_14;					// 14 : Pn81E.2에서 선텍된 CN1 입력 신호
	CX__VAR_STRING_CTRL  sCH__MON_IO_SERVO_IO_15;					// 15 : Pn81E.3에서 선텍된 CN1 입력 신호

	CX__VAR_STRING_CTRL  sCH__MON_ALARM_SIZE;		

	// Command Request ...
	CX__VAR_STRING_CTRL  sCH__CMMD_REQ_SAVE;

	// Config Channel ...
	CX__VAR_ANALOG_CTRL  aCH__CFG_RANGE_POSITION_MIN;
	CX__VAR_ANALOG_CTRL  aCH__CFG_RANGE_POSITION_MAX;
	CX__VAR_ANALOG_CTRL  aCH__CFG_RANGE_POSITION_DEC;

	CX__VAR_ANALOG_CTRL  aCH__CFG_CONVERSION_UNIT_PULSE_TO_MM;

	CX__VAR_ANALOG_CTRL  aCH__CFG_HOME_TIMEOUT;
	CX__VAR_ANALOG_CTRL  aCH__CFG_MOVING_TIMEOUT;

	//
	CX__VAR_DIGITAL_CTRL dCH__CFG_HOME_METHOD;
	CX__VAR_ANALOG_CTRL  aCH__CFG_HOME_SPEED;
	CX__VAR_ANALOG_CTRL  aCH__CFG_HOME_OFFSET_POS;
	CX__VAR_ANALOG_CTRL  aCH__CFG_HOME_APPROACH;
	CX__VAR_ANALOG_CTRL  aCH__CFG_HOME_CREEP;
	CX__VAR_ANALOG_CTRL  aCH__CFG_HOME_ACCEL;
	CX__VAR_ANALOG_CTRL  aCH__CFG_HOME_DECEL;
	CX__VAR_DIGITAL_CTRL dCH__CFG_HOME_DIRECTION;
	CX__VAR_DIGITAL_CTRL dCH__CFG_HOME_OBxx05B;
	CX__VAR_ANALOG_CTRL  aCH__CFG_HOME_ZERO_BASE_OFFSET;

	// Teaching Config ...
	CX__VAR_STRING_CTRL  sCH__ACTIVE_MOVE_DOWN_UP;
	CX__VAR_STRING_CTRL  sCH__ACTIVE_PROC_DOWN_UP;

	CX__VAR_STRING_CTRL  sCH__TEACH_ACTIVE_JOG_UP;
	CX__VAR_STRING_CTRL  sCH__TEACH_ACTIVE_JOG_DOWN;

	CX__VAR_STRING_CTRL  sCH__TEACH_ACTIVE_ABS_UP;
	CX__VAR_STRING_CTRL  sCH__TEACH_ACTIVE_ABS_DOWN;

	CX__VAR_ANALOG_CTRL  aCH__CFG_GOTO_POS_UP_ABS_POS;
	CX__VAR_ANALOG_CTRL  aCH__CFG_GOTO_POS_DOWN_ABS_POS;

	CX__VAR_ANALOG_CTRL  aCH__CFG_TEACH_JOG_SPEED;
	CX__VAR_ANALOG_CTRL  aCH__CFG_TEACH_JOG_ACCEL;
	CX__VAR_ANALOG_CTRL  aCH__CFG_TEACH_JOG_DECEL;

	CX__VAR_ANALOG_CTRL  aCH__CFG_TEACH_ABS_SPEED;
	CX__VAR_ANALOG_CTRL  aCH__CFG_TEACH_ABS_ACCEL;
	CX__VAR_ANALOG_CTRL  aCH__CFG_TEACH_ABS_DECEL;

	// 
	CX__VAR_STRING_CTRL  sCH__APP_PROC_POS_X_SNS[CFG_SIZE__PROC_POS];	
	CX__VAR_ANALOG_CTRL  aCH__CFG_PROC_POS_X_DATA_POS[CFG_SIZE__PROC_POS];	
	CX__VAR_ANALOG_CTRL  aCH__CFG_PROC_POS_X_SPEED[CFG_SIZE__PROC_POS];	
	CX__VAR_ANALOG_CTRL  aCH__CFG_PROC_POS_X_ACCEL[CFG_SIZE__PROC_POS];	
	CX__VAR_ANALOG_CTRL  aCH__CFG_PROC_POS_X_DECEL[CFG_SIZE__PROC_POS];	

	CX__VAR_STRING_CTRL  sCH__APP_TRANSFER_POS_X_SNS[CFG_SIZE__TRANSFER_POS];
	CX__VAR_ANALOG_CTRL  aCH__CFG_TRANSFER_POS_X_DATA_POS[CFG_SIZE__TRANSFER_POS];
	CX__VAR_ANALOG_CTRL  aCH__CFG_TRANSFER_POS_X_SPEED[CFG_SIZE__TRANSFER_POS];	
	CX__VAR_ANALOG_CTRL  aCH__CFG_TRANSFER_POS_X_ACCEL[CFG_SIZE__TRANSFER_POS];	
	CX__VAR_ANALOG_CTRL  aCH__CFG_TRANSFER_POS_X_DECEL[CFG_SIZE__TRANSFER_POS];	

	// CYCLE TEST : TRANSFER POS ...
	CX__VAR_ANALOG_CTRL  aCH__CFG_MOVE_DOWN_UP_CYCLE;
	CX__VAR_STRING_CTRL  sCH__RESULT_MOVE_DOWN_UP_CYCLE;
	CX__VAR_STRING_CTRL  sCH__RESULT_MOVE_DOWN_UP_AVG_UP;
	CX__VAR_STRING_CTRL  sCH__RESULT_MOVE_DOWN_UP_MIN_UP;
	CX__VAR_STRING_CTRL  sCH__RESULT_MOVE_DOWN_UP_MAX_UP;
	CX__VAR_STRING_CTRL  sCH__RESULT_MOVE_DOWN_UP_AVG_DOWN;
	CX__VAR_STRING_CTRL  sCH__RESULT_MOVE_DOWN_UP_MIN_DOWN;
	CX__VAR_STRING_CTRL  sCH__RESULT_MOVE_DOWN_UP_MAX_DOWN;

	// CYCLE TEST : PROCESS POS ...
	CX__VAR_ANALOG_CTRL  aCH__CFG_PROC_DOWN_UP_CYCLE;
	CX__VAR_STRING_CTRL  sCH__RESULT_PROC_DOWN_UP_CYCLE;
	CX__VAR_STRING_CTRL  sCH__RESULT_PROC_DOWN_UP_AVG_UP;
	CX__VAR_STRING_CTRL  sCH__RESULT_PROC_DOWN_UP_MIN_UP;
	CX__VAR_STRING_CTRL  sCH__RESULT_PROC_DOWN_UP_MAX_UP;
	CX__VAR_STRING_CTRL  sCH__RESULT_PROC_DOWN_UP_AVG_DOWN;
	CX__VAR_STRING_CTRL  sCH__RESULT_PROC_DOWN_UP_MIN_DOWN;
	CX__VAR_STRING_CTRL  sCH__RESULT_PROC_DOWN_UP_MAX_DOWN;

	// Stable Channel ...
	CX__VAR_ANALOG_CTRL  aCH__CFG_WARNING_LIMIT;
	CX__VAR_ANALOG_CTRL  aCH__CFG_FAULT_LIMIT;

	CX__VAR_ANALOG_CTRL  aCH__CFG_CHECKING_TIME_FOR_STABLE;
	CX__VAR_ANALOG_CTRL  aCH__CFG_CHECK_TIMEOUT_FOR_ERROR;
	CX__VAR_ANALOG_CTRL  aCH__CFG_STABLE_TIME_FOR_SETPINT_CHANGE;

	CX__VAR_STRING_CTRL  sCH__MON_AXIS_STABLE_STATE[CFG__AXIS_SIZE];
	CX__VAR_STRING_CTRL  sCH__MON_AXIS_STABLE_TIME[CFG__AXIS_SIZE];
	CX__VAR_STRING_CTRL  sCH__MON_AXIS_FAULT_STATE[CFG__AXIS_SIZE];
	CX__VAR_STRING_CTRL  sCH__MON_AXIS_FAULT_TIME[CFG__AXIS_SIZE];
	CX__VAR_STRING_CTRL  sCH__MON_AXIS_CHANGE_STATE[CFG__AXIS_SIZE];
	CX__VAR_STRING_CTRL  sCH__MON_AXIS_CHANGE_TIME[CFG__AXIS_SIZE];

	CX__VAR_STRING_CTRL  sCH__MON_STABLE_FLAG;
	CX__VAR_STRING_CTRL  sCH__MON_FAULT_FLAG;


	//-------------------------------------------------------------------------
	//  EXTERNAL PROPERTY

	CX__VAR_DIGITAL_CTRL  dEXT_CH__CFG_INTERLOCK_USE_FLAG;
	CX__VAR_STRING_CTRL   sEXT_CH__LIFT_PIN_STATE;


	//-------------------------------------------------------------------------
	CString sMODE__INIT;
	int  Call__INIT(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);
	int  Fnc__INIT(CII_OBJECT__ALARM* p_alarm);

	// ...
	CString sMODE__HOME;
	int  Call__HOME(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	// ...
	CString sMODE__CYCLE_DOWN_UP;
	int  Call__CYCLE_DOWN_UP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	CString sMODE__CYCLE_PROC_DOWN_UP;
	int  Call__CYCLE_PROC_DOWN_UP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	// ...
	CString sMODE__MOVE_STOP;
	int  Call__MOVE_STOP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	// ...
	CString sMODE__TEACH_ABS_UP;
	CString sMODE__TEACH_ABS_DOWN;
	int  Call__TEACH_ABS_START(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const int up_mode);

	int  Update__JOG_OF_TEACHING_PROPERTY();
	int  Update__ABS_OF_TEACHING_PROPERTY(const int up_mode);

	int  Update__GOTO_POS_OF_PROC_POS(const int pos_index);
	int  Update__GOTO_POS_OF_PROC_SPEED(const int pos_index);

	int  Update__GOTO_POS_OF_MOVE_POS(const int pos_index);
	int  Update__GOTO_POS_OF_MOVE_SPEED(const int pos_index);

	int  Fnc__GOTO_POS_START(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);
	int  Fnc__GOTO_POS_STOP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	// ...
	CString sMODE__TEACH_JOG_PLUS;
	CString sMODE__TEACH_JOG_MINUS;
	int  Call__TEACH_JOG_START(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const int plus_mode);

	int  Fnc__TEACH_JOG_START(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const int plus_mode);
	int  Fnc__TEACH_JOG_STOP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	// ...
	CString sMODE__RETURN;
	int Call__RETURN(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);

	// ...
	CString sMODE__DOWN;
	int Call__DOWN(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);

	CString sMODE__DOWN_FAST;
	int Call__DOWN_FAST(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);

	CString sMODE__MIDDLE_DOWN;
	int Call__MIDDLE_DOWN(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);

	CString sMODE__MIODLE_UP;
	int Call__MIDDLE_UP(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);

	CString sMODE__UP;
	int Call__UP(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);

	CString sMODE__UP_FAST;
	int Call__UP_FAST(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);

	// ...
	CString sMODE__PROC_DOWN;
	int Call__PROC_DOWN(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);

	CString sMODE__PROC_MIDDLE;
	int Call__PROC_MIDDLE(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);

	CString sMODE__PROC_UP;
	int Call__PROC_UP(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);

	// ...
	CString sMODE__CHECK_ALARM;
	int Call__CHECK_ALARM(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);

	CString sMODE__CLEAR_ALARM;
	int Call__CLEAR_ALARM(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);

	CString sMODE__CLEAR_SERVO_ALARM;
	int Call__CLEAR_SERVO_ALARM(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);

	// ...
	CString sMODE__SERVO_ON;
	int Call__SERVO_ON(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);

	CString sMODE__SERVO_OFF;
	int Call__SERVO_OFF(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);

	// ...
	CString sMODE__DEF_POSITION;
	int Call__DEF_POSITION(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);


	// ...
	CString Get__ERROR_CODE_AND_TROUBLE_SHOOTING(DWORD err_code);
	CString Get__WARNING_OF_MONITORING_PARAMETER(DWORD code_value);
	CString Get__ALARM_OF_MONITORING_PARAMETER(DWORD code_value);

	// ...
	int  Mon__MONITOR(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	void Update__MON_IO_DRIVE_STATUS(DWORD get_data, const int ok_state = 1);
	void Update__MON_IO_MCMD_STATUS(DWORD get_data, const int ok_state = 1);
	void Update__MON_IO_SERVO_STATUS(DWORD get_data, const int ok_state = 1);
	void Update__MON_IO_SERVO_IO_MON(DWORD get_data, const int ok_state = 1);

	void Update__PIN_STATE();

	// TRANSFER : XXX_POS -> DOWN
	int  Check__TRANSFER_POS__MORE_THAN__MIDDLE_UP();
	int  Check__TRANSFER_POS__MORE_THAN__MIDDLE_DOWN();

	// TRANSFER : XXX_POS -> UP
	int  Check__TRANSFER_POS__LESS_THAN__MIDDLE_DOWN();
	int  Check__TRANSFER_POS__LESS_THAN__MIDDLE_UP();

	// PROCESS : XXX_POS -> DOWN
	int  Check__PROCESS_POS__MORE_THAN__MIDDLE();

	// PROCESS : XXX_POS -> UP
	int  Check__PROCESS_POS__LESS_THAN__MIDDLE();

	// ...
	int	 Mon__VIEW_REQ(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);

	// ...
	int	 Mon__STABLE(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);
	int  Fnc__STABLE_CHECK(CII_OBJECT__ALARM* p_alarm, const int axis_index, const int alm_report = 1);
	//


public:
	CObj__MP2100_VIRTUAL();
	~CObj__MP2100_VIRTUAL();

	//-------------------------------------------------------------------------
	int __DEFINE__CONTROL_MODE(obj, l_mode);
	int __DEFINE__VERSION_HISTORY(version);

	int __DEFINE__VARIABLE_STD(p_variable);
	int __DEFINE__VARIABLE_IO(p_io_variable);

	int __DEFINE__ALARM(p_alarm);

	//-------------------------------------------------------------------------
	int __INITIALIZE__OBJECT(p_variable,p_ext_obj_create);
	int __INITIALIZE__IO(p_io_para);

	//-------------------------------------------------------------------------
	int __CALL__CONTROL_MODE(mode, p_debug, p_variable, p_alarm);
	int __CALL__MONITORING(id, p_variable, p_alarm);

	//-------------------------------------------------------------------------
	int __Read__ANALOG(const CString& var_name, const CDS_IO__CHANNEL_INFO& io_info, double& read_data);
	int __Read__DIGITAL(const CString& var_name,const CDS_IO__CHANNEL_INFO& io_info, CString& read_data,int& item_index);
	int __Read__STRING(const CString& var_name, const CDS_IO__CHANNEL_INFO& io_info, CString& read_data);

	int __Write__ANALOG(const CString& var_name, const CDS_IO__CHANNEL_INFO& io_info, const double set_data);
	int __Write__DIGITAL(const CString& var_name,const CDS_IO__CHANNEL_INFO& io_info, const CString& set_data,const int item_index);
	int __Write__STRING(const CString& var_name, const CDS_IO__CHANNEL_INFO& io_info, const CString& set_data);
};

