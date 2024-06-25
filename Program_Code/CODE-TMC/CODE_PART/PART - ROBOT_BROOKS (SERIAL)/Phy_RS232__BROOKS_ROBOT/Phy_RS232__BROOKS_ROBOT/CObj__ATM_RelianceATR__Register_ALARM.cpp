#include "StdAfx.h"
#include "CObj__ATM_RelianceATR.h"
#include "CObj__ATM_RelianceATR__DEF.h"
#include "CObj__ATM_RelianceATR__ALID.h"

#include "math.h"
#include "CCommon_DEF.h"


//------------------------------------------------------------------------------------
#define  ACT__RETRY_ABORT						\
l_act.RemoveAll();								\
l_act.Add("RETRY");								\
l_act.Add("ABORT");


void CObj__ATM_RelianceATR
::Register__ALARM_INF(CII_DEFINE__ALARM* p_alarm)
{
	DECLARE__ALARM;

	// ...
	CString title;
	title.Format("%s - ATM-Robot Error : ",sObject_Name);

	CString alarm_title;
	CString alarm_msg;
	CStringArray l_act;
	int alarm_id;

	// ...
	{
		alarm_id = ALID__ATMRB_1_COMMAND_FAILED;

		alarm_title  = title;
		alarm_title += "Command failed";

		alarm_msg  = "This is a generic command that requires the operator to have a knowledge.\n";
		alarm_msg += "of the events leading to the failure.\n";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_2_MCC_ERROR;

		alarm_title  = title;
		alarm_title += "MCC Error";

		alarm_msg  = "MCC Error Already Displayed.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_210_STATION_ERROR;

		alarm_title  = title;
		alarm_title += "Not at Station";

		alarm_msg  = "Station based command issued while robot was\n";
		alarm_msg += "not at a station.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_220_ERROR;

		alarm_title  = title;
		alarm_title += "Radial axis not at retract position";

		alarm_msg  = "Robot must be retracted prior to executing desired move.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_221_ARM_ERROR;

		alarm_title  = title;
		alarm_title += "Invalid Arm selection";

		alarm_msg  = "Verify that arm A is being selected for single arm robot.\n";
		alarm_msg += "Arm B is available for dual arm.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_222_ERROR;

		alarm_title  = title;
		alarm_title += "Wrist axis not at unflipped position";

		alarm_msg  = "This error is displayed anytime a robot is commanded to retract from a\n";
		alarm_msg += "station and the W-axis is at the FLIPPED position with the Z-axis DOWN\n";
		alarm_msg += "for the station.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_223_ERROR;

		alarm_title  = title;
		alarm_title += "Wrist axis not at flipped position [ErrorID:%d]";

		alarm_msg  = "Wrist axis not at flipped position.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_224_ERROR;

		alarm_title  = title;
		alarm_title += "R and T axis must be referenced before homing Z";

		alarm_msg  = "R and T axis must be referenced before homing Z.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_230_EXTEND_ERROR;

		alarm_title  = title;
		alarm_title += "Robot Extended to VCE";

		alarm_msg = "Robot Extended to VCE.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_231_EXTEND_ERROR;

		alarm_title  = title;
		alarm_title += "Atmospheric Robot Extended to VCE";

		alarm_msg = "Atmospheric Robot Extended to VCE.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_232_PNEUMATIC_ERROR;

		alarm_title  = title;
		alarm_title += "Platform 97 Pneumatic Fault";

		alarm_msg = "Platform 97 Pneumatic Fault.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_233_EXTEND_ERROR;

		alarm_title  = title;
		alarm_title += "Extend to station not enabled";

		alarm_msg = "Extend to station not enabled.\n";
		alarm_msg += "ATM Robot occurred H/W Alarm.\n";
		alarm_msg += "Please, Check ATM Robot H/W Status";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_234_VALVE_ERROR;

		alarm_title  = title;
		alarm_title += "Valve not closed";

		alarm_msg = "Valve not closed.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_301_ERROR;

		alarm_title  = title;
		alarm_title += "Command argument already used";

		alarm_msg = "Command argument already used.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_305_UNKNOWN_COMMAND;

		alarm_title  = title;
		alarm_title += "Unknown command";

		alarm_msg  = "Unrecognized command. An optional parameter is missing or the command was\n";
		alarm_msg += "entered incorrectly.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_309_ERROR;

		alarm_title  = title;
		alarm_title += "Command not supported";

		alarm_msg = "Command not supported.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_350_ERROR;

		alarm_title  = title;
		alarm_title += "Parser error, bad node in parse tree";

		alarm_msg = "Parser error, bad node in parse tree.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_351_ERROR;

		alarm_title  = title;
		alarm_title += "Parser error, stack overflow";

		alarm_msg = "Parser error, stack overflow.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_352_ERROR;

		alarm_title  = title;
		alarm_title += "Parse error, no memory available";

		alarm_msg = "Parse error, no memory available.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_353_ERROR;

		alarm_title  = title;
		alarm_title += "Unexpected mail to UIO task";

		alarm_msg = "Unexpected mail to UIO task";
	
		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_390_ERROR;

		alarm_title  = title;
		alarm_title += "Command checksum error";

		alarm_msg = "Command checksum error";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_402_STATION;

		alarm_title  = title;
		alarm_title += "Bad slot number";

		alarm_msg = "Bad slot number\n";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_405_STATION;

		alarm_title  = title;
		alarm_title += "Bad Lower Position";

		alarm_msg = "Bad Lower Position.\n";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_416_POSITION;

		alarm_title  = title;
		alarm_title += "Station not initialized";

		alarm_msg = "Station not initialized.\n";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_509_ERROR;

		alarm_title  = title;
		alarm_title += "No Z-axis on robot";

		alarm_msg = "No Z-axis on robot.\n";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_602_CDM;

		alarm_title  = title;
		alarm_title += "Command sequencer busy";

		alarm_msg  = "Wait for the robot to complete its last operation. If the error persists,\n";
		alarm_msg += "reset the robot by issuing the RESET command or toggling power.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_603_CDM;

		alarm_title  = title;
		alarm_title += "Command halted";

		alarm_msg = "A HALT command was issued to stop robot motion. The robot remains referenced.\n";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_604_CDM;

		alarm_title  = title;
		alarm_title += "CDM in control of the robot";

		alarm_msg  = "The command issued requires control of the robot.\n";
		alarm_msg += "Turn off the CDM prior to continuing.";
	}

	// ...
	{
		alarm_id = ALID__ATMRB_605_CDM;

		alarm_title  = title;
		alarm_title += "Digital I/O in control of robot";

		alarm_msg  = "To release digital I/O control of robot, enter the command DIO STOP.\n";
		alarm_msg += "Refer to DIO Start/Stop on page 8-66 for additional information.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_606_CDM;

		alarm_title  = title;
		alarm_title += "Serial I/O in control of robot";

		alarm_msg  = "To initiate digital I/O control of robot, enter the command DIO START.\n";
		alarm_msg += "Refer to DIO Start/Stop on page 8-66 for additional information.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_610_CDM;

		alarm_title  = title;
		alarm_title += "Emergency stop on";

		alarm_msg  = "The EMER_STOP interlock for the robot has been activated.\n";
		alarm_msg += "Refer to Operational Interlocks on page 6-66. Verify that a robot\n";
		alarm_msg += "Emergency Stop (EMS) button has been activated and deactivate\n";
		alarm_msg += "as appropriate. Check host controller software.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_611_CDM;

		alarm_title  = title;
		alarm_title += "Warning, CDM has been turned on";

		alarm_msg = "Warning, CDM has been turned on.\n";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_612_CDM;

		alarm_title  = title;
		alarm_title += "Warning, CDM has been turned off";

		alarm_msg = "Warning, CDM has been turned off.\n";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_654_COMMAND;

		alarm_title  = title;
		alarm_title += "Unknown command";

		alarm_msg = "Unknown command.\n";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_659_COMMAND;

		alarm_title  = title;
		alarm_title += "Invalid command identifier";

		alarm_msg = "Invalid command identifier.\n";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_700_COMMAND;

		alarm_title  = title;
		alarm_title += "Wafer detected on paddle";

		alarm_msg = "Wafer detected on paddle\n";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_711_COMMAND;

		alarm_title  = title;
		alarm_title += "Slot valve not open";

		alarm_msg = "Slot valve not open\n";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_712_COMMAND;

		alarm_title  = title;
		alarm_title += "No Vacuum Grip Arm A";

		alarm_msg  = "Occurs during a PICK operation if successful vacuum gripping is NOT\n";
		alarm_msg += "detected for the specified arm at the completion of the operation,\n";
		alarm_msg += "whether the arm is retracted or not.\n";
		alarm_msg += "Occurs during a PLACE operation if successful vacuum gripping is\n";
		alarm_msg += "NOT detected for the specified arm prior to placing the wafer.\n";
		alarm_msg += "Occurs during either the PICK or PLACE portion of a XFER operation.\n";
		alarm_msg += "Occurs during a GOTO operation if successful vacuum gripping is NOT\n";
		alarm_msg += "detected for the specified arm prior to the specified motion.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_714_COMMAND;

		alarm_title  = title;
		alarm_title += "Slow Vacuum Grip Arm A";

		alarm_msg  = "Occurs during a PICK operation if successful vacuum gripping is NOT\n";
		alarm_msg += "detected for the specified arm when a wafer has been picked up and the\n";
		alarm_msg += "robot arm is still extended, but IS detected at the completion of the operation.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_715_COMMAND;

		alarm_title  = title;
		alarm_title += "Possible Material on Arm A";

		alarm_msg = "Possible Material on Arm A.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_721_COMMAND;

		alarm_title  = title;
		alarm_title += "Pick failed";

		alarm_msg = "Pick failed.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_722_COMMAND;

		alarm_title  = title;
		alarm_title += "Placed failed";

		alarm_msg = "Placed failed.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_730_COMMAND;

		alarm_title  = title;
		alarm_title += "RE wafer sensor error prior to PLACE:NoWafer Sensed";

		alarm_msg = "RE wafer sensor error prior to PLACE:NoWafer Sensed.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_731_COMMAND;

		alarm_title  = title;
		alarm_title += "RE wafer sensor error after a PLACE:Wafer Sensed";

		alarm_msg = "RE wafer sensor error after a PLACE:Wafer Sensed.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_732_COMMAND;

		alarm_title  = title;
		alarm_title += "EX wafer sensor error prior to a PLACE: Wafer Sensed";

		alarm_msg = "EX wafer sensor error prior to a PLACE: Wafer Sensed.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_733_COMMAND;

		alarm_title  = title;
		alarm_title += "EX wafer sensor error after a PLACE: No Wafer Sensed";

		alarm_msg = "EX wafer sensor error after a PLACE: No Wafer Sensed.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_740_COMMAND;

		alarm_title  = title;
		alarm_title += "RE wafer sensor error prior to a PICK: Wafer Sensed";

		alarm_msg = "RE wafer sensor error prior to a PICK: Wafer Sensed.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_741_COMMAND;

		alarm_title  = title;
		alarm_title += "RE wafer sensor error after a PICK:NoWafer Sensed";

		alarm_msg = "RE wafer sensor error after a PICK:NoWafer Sensed.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_742_COMMAND;

		alarm_title  = title;
		alarm_title += "EX wafer sensor error prior to a PICK:NoWafer Sensed";

		alarm_msg = "EX wafer sensor error prior to a PICK:NoWafer Sensed.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_743_COMMAND;

		alarm_title  = title;
		alarm_title += "EX wafer sensor error after a PICK: Wafer Sensed";

		alarm_msg = "EX wafer sensor error after a PICK: Wafer Sensed.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_760_COMMAND;

		alarm_title  = title;
		alarm_title += "Possible material on Arm A";

		alarm_msg = "Prior to a PICK operation, it is not known if a wafer is on the arm.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_1900_COMMAND;

		alarm_title  = title;
		alarm_title += "Unknown serial port";

		alarm_msg = "Unknown serial port.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_1903_COMMAND;

		alarm_title  = title;
		alarm_title += "Can't allocate serial port semaphore";

		alarm_msg = "Can't allocate serial port semaphore.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_1910_COMMAND;

		alarm_title  = title;
		alarm_title += "Secondary Serial Port mode";

		alarm_msg = "Secondary Serial Port mode.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_1911_COMMAND;

		alarm_title  = title;
		alarm_title += "Secondary Serial Port is busy";

		alarm_msg = "Secondary Serial Port is busy.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_1912_COMMAND;

		alarm_title  = title;
		alarm_title += "Secondary Serial Port response timeout";

		alarm_msg = "Secondary Serial Port response timeout.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_1920_COMMAND;

		alarm_title  = title;
		alarm_title += "No serial communication with remote MCC";

		alarm_msg = "No serial communication with remote MCC.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_10000_ECC;

		alarm_title  = title;
		alarm_title += "Default debug message from the MCC";

		alarm_msg = "Default debug message from the MCC.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_10001_SYNC;

		alarm_title  = title;
		alarm_title += "Sync error, motor moving or encoder noisy";

		alarm_msg = "Sync error, motor moving or encoder noisy.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_10002_MCC;

		alarm_title  = title;
		alarm_title += "MCC board memory allocation error";

		alarm_msg = "MCC board memory allocation error.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_10003_MCC;

		alarm_title  = title;
		alarm_title += "MCC board unexpected event error";

		alarm_msg = "MCC board unexpected event error.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_10004_MCC;

		alarm_title  = title;
		alarm_title += "MCC board, bad command state";

		alarm_msg = "MCC board, bad command state.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_10005_MCC;

		alarm_title  = title;
		alarm_title += "MCC Board Sync error";

		alarm_msg  = "MCC Board Sync error, can't move motor. Possible binding.\n";
		alarm_msg += "See Z Binding Test Using the Trace Command on page 10-37.\n";
		alarm_msg += "Obstruction in way ofmotion.\n";
		alarm_msg += "Perform Verifying Robot Calibration Parameters on page 10-44.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_10006_MCC;

		alarm_title  = title;
		alarm_title += "MCC encoder VABS adjusted";

		alarm_msg = "MCC encoder VABS adjusted (small).";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_10007_MCC;

		alarm_title  = title;
		alarm_title += "MCC board position settle error";

		alarm_msg  = "MCC board position settle error/unable to obtain position The robot has\n";
		alarm_msg += "a settling window at the end of the trajectory.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_10008_MCC;

		alarm_title  = title;
		alarm_title += "MCC unable to hold position";

		alarm_msg = "MCC unable to hold position.\n";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_10009_MCC;

		alarm_title  = title;
		alarm_title += "MCC hard tracking error";

		alarm_msg  = "Verify that robot arm state is correct (arms on or arms off).\n";
		alarm_msg += "Verify robot application number is correct.\n";
		alarm_msg += "Check for physical obstruction.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_10010_MCC;

		alarm_title  = title;
		alarm_title += "MCC soft tracking error";

		alarm_msg = "MCC soft tracking error.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_10011_MOTOR;

		alarm_title  = title;
		alarm_title += "Error, motor is already moving";

		alarm_msg = "Error, motor is already moving.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_10012_MOTOR;

		alarm_title  = title;
		alarm_title += "Error, motor is not configured";

		alarm_msg = "Error, motor is not configured.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_10013_MOTOR;

		alarm_title  = title;
		alarm_title += "Error, motor is not referenced";

		alarm_msg = "Error, motor is not referenced.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_10014_MOTOR;

		alarm_title  = title;
		alarm_title += "Error, motor is already referencing";

		alarm_msg = "Error, motor is already referencing.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_10015_MOTOR;

		alarm_title  = title;
		alarm_title += "Error, motor is currently moving";

		alarm_msg = "Error, Error, motor is currently moving.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_10016_MOTOR;

		alarm_title  = title;
		alarm_title += "Error, unable to calculate trajectory";

		alarm_msg = "Error, unable to calculate trajectory.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_10017_MOTOR;

		alarm_title  = title;
		alarm_title += "Illegal number of polls calculated";

		alarm_msg = "Illegal number of polls calculated.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_10018_MOTOR;

		alarm_title  = title;
		alarm_title += "Unable to calculate absolute position";

		alarm_msg = "Unable to calculate absolute position.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_10019_MOTOR;

		alarm_title  = title;
		alarm_title += "Error, Encoder off by many sectors";

		alarm_msg  = "Verify the encoder values are in range.\n";
		alarm_msg += "See Verifying Robot Calibration Parameters on page 10-44.\n";
		alarm_msg += "An encoder value of 2048 means the encoder is saturated and\n";
		alarm_msg += "must be returned for repair.\n";
		alarm_msg += "Check the encoder collar clamp.";
	}

	// ...
	{
		alarm_id = ALID__ATMRB_10020_MOTOR;

		alarm_title  = title;
		alarm_title += "Error, Encoder failed multiple times";

		alarm_msg  = "Check if failure location is repeatable.\n";
		alarm_msg += "Record position of failure.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id	= ALID__ATMRB_10021_MOTOR;

		alarm_title  = title;
		alarm_title += "Error, Board Power Failure";

		alarm_msg  = "Error, Board Power Failure (Blown Fuse?).\n";
		alarm_msg += "Check if fuse is blown by inspecting fuses or if respective\n";
		alarm_msg += "LED is not illuminated.\n";
		alarm_msg += "Replace fuse as needed.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_10022_ERROR;

		alarm_title  = title;
		alarm_title += "Error, Z axis overtravel limit reached";

		alarm_msg  = "Error, Z axis overtravel limit reached.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_10023_ERROR;

		alarm_title  = title;
		alarm_title += "Arm actual position impossible, check sync zero";

		alarm_msg = "Arm actual position impossible, check sync zero.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_10024_ERROR;

		alarm_title  = title;
		alarm_title += "Error, MCC watchdog timed out";

		alarm_msg = "Error, MCC watchdog timed out.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_10028_ERROR;

		alarm_title  = title;
		alarm_title += "Error, obstruction encounter for axis";

		alarm_msg = "Error, obstruction encounter for axis.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_10029_EMO;

		alarm_title  = title;
		alarm_title += "Error, Emergency Stop circuit is active";

		alarm_msg = "Error, Emergency Stop circuit is active.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_10030_ERROR;

		alarm_title  = title;
		alarm_title += "Error, excessive current detected";

		alarm_msg = "Error, excessive current detected.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_10031_ERROR;

		alarm_title  = title;
		alarm_title += "Warning, Home Sensor position moved";

		alarm_msg = "Warning, Home Sensor position moved.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_10034_ERROR;

		alarm_title  = title;
		alarm_title += "Error, encoder min/max value out of range";

		alarm_msg = "Error, encoder min/max value out of range.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_10035_ERROR;

		alarm_title  = title;
		alarm_title += "Error, bad sync phase offset value";

		alarm_msg = "Error, bad sync phase offset value.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_10036_ERROR;

		alarm_title  = title;
		alarm_title += "Error, robot links are not yet defined";

		alarm_msg = "Error, robot links are not yet defined.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_10047_ERROR;

		alarm_title  = title;
		alarm_title += "Error, MCC code date/stamp mismatch";

		alarm_msg = "Error, MCC code date/stamp mismatch.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_10048_ERROR;

		alarm_title  = title;
		alarm_title += "Error, MCC stored code checksum incorrect";

		alarm_msg = "Error, MCC stored code checksum incorrect.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_10049_ERROR;

		alarm_title  = title;
		alarm_title += "Error, MCC code size too large to store";

		alarm_msg = "Error, MCC code size too large to store.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_10050_ERROR;

		alarm_title  = title;
		alarm_title += "Error, i2c bus is busy";

		alarm_msg = "Error, i2c bus is busy.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_10051_ERROR;

		alarm_title  = title;
		alarm_title += "Error, no ack on i2c send data";

		alarm_msg = "Error, no ack on i2c send data.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_10052_ERROR;

		alarm_title  = title;
		alarm_title += "Error, no ack on i2c send address";

		alarm_msg = "Error, no ack on i2c send address.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_10053_ERROR;

		alarm_title  = title;
		alarm_title += "Error, i2c bus error";

		alarm_msg = "Error, i2c bus error.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_10054_ERROR;

		alarm_title  = title;
		alarm_title += "Error, DPRAM bad MCC head/tail pointer";

		alarm_msg = "Error, DPRAM bad MCC head/tail pointer.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_10055_ERROR;

		alarm_title  = title;
		alarm_title += "Error, DPRAM bad SUP head/tail pointer";

		alarm_msg = "Error, DPRAM bad SUP head/tail pointer.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_10056_ERROR;

		alarm_title  = title;
		alarm_title += "Error, DPRAM bad SUP message site";

		alarm_msg = "Error, DPRAM bad SUP message site.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_10057_ERROR;

		alarm_title  = title;
		alarm_title += "Error, Home Sensor position moved too far";

		alarm_msg = "Error, Home Sensor position moved too far.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_10058_ERROR;

		alarm_title  = title;
		alarm_title += "Error, Set loop mode failed";

		alarm_msg = "Error, Set loop mode failed.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_10059_ERROR;

		alarm_title  = title;
		alarm_title += "Error, Motor Stop Move timed out";

		alarm_msg = "Error, Motor Stop Move timed out.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_10060_ERROR;

		alarm_title  = title;
		alarm_title += "Error, Reference Faile";

		alarm_msg = "Error, Reference Failed.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_12012_ERROR;

		alarm_title  = title;
		alarm_title += "Find Bias required";

		alarm_msg = "Find Bias required.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_13000_COMMAND;

		alarm_title  = title;
		alarm_title += "Aligner not specified";

		alarm_msg = "Aligner not specified.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_13001_COMMAND;

		alarm_title  = title;
		alarm_title += "Aligner not at station specified [ErrorID:%d]";

		alarm_msg = "Aligner not at station specified.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_13002_COMMAND;

		alarm_title  = title;
		alarm_title += "Aligner has not finished last operation";

		alarm_msg = "Aligner has not finished last operation.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_13003_COMMAND;

		alarm_title  = title;
		alarm_title += "Received bad message from Aligner";

		alarm_msg = "Received bad message from Aligner.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_13004_COMMAND;

		alarm_title  = title;
		alarm_title += "Aligner has an error";

		alarm_msg = "Aligner has an error.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_13005_COMMAND;

		alarm_title  = title;
		alarm_title += "Receive bad data from Aligner";

		alarm_msg = "Receive bad data from Aligner.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_13006_COMMAND;

		alarm_title  = title;
		alarm_title += "Wafer needs to be scanned again";

		alarm_msg = "Wafer needs to be scanned again.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_13007_COMMAND;

		alarm_title  = title;
		alarm_title += "Communication with aligner timeout";

		alarm_msg = "Communication with aligner timeout.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_13008_COMMAND;

		alarm_title  = title;
		alarm_title += "Communication with aligner not initialized";

		alarm_msg = "Communication with aligner not initialized.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_13011_COMMAND;

		alarm_title  = title;
		alarm_title += "Aligner parameter mismatch";

		alarm_msg  = "Issue the INIT ALIGNER command to upload the aligner parameters,\n";
		alarm_msg += "then request the settings using the command RQ ALIGNER 1\n";
		alarm_msg += "(WAFERID|POSTPOS 1 POS) and verify the correct parameters are set.\n";
		alarm_msg += "The wafer ID or the Post Position were SET but not STORED then the robot\n";
		alarm_msg += "was rebooted. User STORE ALIGNER command to store parameters.\n";
		alarm_msg += "The robot was reset after the wafer ID or Post Position was SET.\n";
		alarm_msg += "The aligner retained the setting, however, the robot lost the information.\n";
		alarm_msg += "Reset the robot and the aligner at the same time.\n";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_20004_ERROR;

		alarm_title  = title;
		alarm_title += "Data out of range";

		alarm_msg = "Data out of range.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_20017_ERROR;

		alarm_title  = title;
		alarm_title += "No Wafer Detected";

		alarm_msg = "No Wafer Detected.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_20030_ERROR;

		alarm_title  = title;
		alarm_title += "Vacuum Suction Error [ErrorID:%d]";

		alarm_msg = "Vacuum Suction Error.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_20031_ERROR;

		alarm_title  = title;
		alarm_title += "Vacuum Release Error";

		alarm_msg = "Vacuum Release Error.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_20037_ERROR;

		alarm_title  = title;
		alarm_title += "EEPROMChecksum Error";

		alarm_msg = "EEPROM Checksum Error.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_20041_ERROR;

		alarm_title  = title;
		alarm_title += "Out of CCD Range";

		alarm_msg = "Out of CCD Range.";
	}

	// ...
	{
		alarm_id = ALID__ATMRB_20306_ERROR;

		alarm_title  = title;
		alarm_title += "Value out of range";

		alarm_msg = "Value out of range.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_20307_ERROR;

		alarm_title  = title;
		alarm_title += "Argument not found";

		alarm_msg = "Argument not found.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_20309_ERROR;

		alarm_title  = title;
		alarm_title += "Command not available";

		alarm_msg = "Command not available.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id	= ALID__ATMRB_20706_ERROR;

		alarm_title  = title;
		alarm_title += "Z-lift mechanism is not present";

		alarm_msg = "Z-lift mechanism is not present.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id	= ALID__ATMRB_20800_ERROR;

		alarm_title  = title;
		alarm_title += "Abnormal Fiducial Mark Size (N)";

		alarm_msg = "Abnormal Fiducial Mark Size (N).";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id =  ALID__ATMRB_20801_ERROR;

		alarm_title  = title;
		alarm_title += "Abnormal Fiducial Mark Size (FD)";

		alarm_msg = "Abnormal Fiducial Mark Size (FD).";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_20802_ERROR;

		alarm_title  = title;
		alarm_title += "Too Large Average";

		alarm_msg = "Too Large Average.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_20803_ERROR;

		alarm_title  = title;
		alarm_title += "Too Small Average";

		alarm_msg = "Too Small Average.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_20804_ERROR;

		alarm_title  = title;
		alarm_title += "No Distinct Fiducial Mark Detected";

		alarm_msg = "No Distinct Fiducial Mark Detected.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_20805_ERROR;

		alarm_title  = title;
		alarm_title += "Too Large Fiducial Mark";

		alarm_msg = "Too Large Fiducial Mark.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_20806_ERROR;

		alarm_title  = title;
		alarm_title += "Too Small Wafer";

		alarm_msg = "Too Small Wafer.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_20807_ERROR;

		alarm_title  = title;
		alarm_title += "Too Large Wafer";

		alarm_msg = "Too Large Wafer.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_20809_ERROR;

		alarm_title  = title;
		alarm_title += "Out of CCD Range or Video Threshold is Low";

		alarm_msg = "Out of CCD Range or Video Threshold is Low.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_20810_ERROR;

		alarm_title  = title;
		alarm_title += "Invalid Cal Factor";

		alarm_msg = "Invalid Cal Factor.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_20811_ERROR;

		alarm_title  = title;
		alarm_title += "No Distinct Fiducial Mark Detected";

		alarm_msg = "No Distinct Fiducial Mark Detected.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_20812_ERROR;

		alarm_title  = title;
		alarm_title += "Invalid Wafer Size";

		alarm_msg = "Invalid Wafer Size.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_20813_ERROR;

		alarm_title  = title;
		alarm_title += "Out of CCD Range or Video Threshold is Low";

		alarm_msg = "Out of CCD Range or Video Threshold is Low.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_20822_ERROR;

		alarm_title  = title;
		alarm_title += "Too high Fiducial Mark (N)";

		alarm_msg = "Too high Fiducial Mark (N).";
	}

	// ...
	{
		alarm_id = ALID__ATMRB_20823_ERROR;

		alarm_title  = title;
		alarm_title += "Fiducial Differentiation Error (D)";

		alarm_msg = "Fiducial Differentiation Error (D).";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_20824_ERROR;

		alarm_title  = title;
		alarm_title += "Noisy Fiducial Mark Edge (N) [ErrorID:%d]";

		alarm_msg = "Noisy Fiducial Mark Edge (N).";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_20850_ERROR;

		alarm_title  = title;
		alarm_title += "Bad Video Data";

		alarm_msg = "Bad Video Data.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_20851_ERROR;

		alarm_title  = title;
		alarm_title += "Bad Video Data";

		alarm_msg = "Bad Video Data.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_20901_ERROR;

		alarm_title  = title;
		alarm_title += "No Results Available";

		alarm_msg = "No Results Available.";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ATMRB_MODULE_NO_EXIST_ERROR;

		alarm_title  = title;
		alarm_title += "Module No Exist Error";

		alarm_msg = "Can't Set Command, Check Config option..";

		ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	//.....
	{
		alarm_id = ALID__ATMRB_UNKNOWN_ERROR;

		alarm_title  = title;
		alarm_title += "Robot System Error";

		alarm_msg = "Unknown Error is Occured, Please Reference Manual";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}
}
