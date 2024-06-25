#include "StdAfx.h"
#include "CObj__VAC_MAGATRAN.h"
#include "CObj__VAC_MAGATRAN__DEF.h"
#include "CObj__VAC_MAGATRAN__ALID.h"

#include "CCommon_DEF.h"


//--------------------------------------------------------------------------------
#define  REG_ACT__ABORT							\
l_act.RemoveAll();								\
l_act.Add("ABORT");

#define  REG_ACT__ABORT_SENSORSYNC				\
l_act.RemoveAll();								\
l_act.Add("ABORT");								\
l_act.Add("SENSOR_SYNC");


void CObj__VAC_MAGATRAN
::Register__ALARM_INF(CII_DEFINE__ALARM* p_alarm)
{
	DECLARE__ALARM;

	// ...
	CString title;
	title.Format("%s - ",sObject_Name);

	CString alarm_title;
	CString alarm_msg;
	CStringArray l_act;
	int alarm_id;

	// Success Code -----
	{
		alarm_id = ALID__VACRB1_ERR_00001;

		alarm_title  = title;
		alarm_title += "VACRB: Command Failed";

		alarm_msg.Format("Error No. => [%s]\n", "00001");
		alarm_msg += "This is a generic command that requires the operator to have a knowledge of the\n";
		alarm_msg += "events leading to the failure. For example, the operator must know if the robot\n";
		alarm_msg += "failed during a theta motion, or during Z homing, or other. Normally, the last\n";
		alarm_msg += "command issued to the robot will provide this information. Next, the operator\n";
		alarm_msg += "is to refer to the appropriate failure mode in Symptoms of\n";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00210;

		alarm_title = title;
		alarm_title += "Not at Station";

		alarm_msg.Format("Error No. => [%s]\n","00210");
		alarm_msg += "Station based command issued while robot was not at a station";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00220;

		alarm_title = title;
		alarm_title += "Radial axis not at retract position";

		alarm_msg.Format("Error No. => [%s]\n","00220");
		alarm_msg += "Robot must be retracted prior to executing desired move";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00221;

		alarm_title = title;
		alarm_title += "Invalid arm selection ";

		alarm_msg.Format("Error No. => [%s]\n","00221");
		alarm_msg += "Verify that arm A is being selected for single arm robot. Arm B is available for dual arm.";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// Station Errors -----
	{
		alarm_id = ALID__VACRB1_ERR_00233;

		alarm_title  = title;
		alarm_title += "VACRB: Extend to station not enabled";

		alarm_msg.Format("Error No. => [%s]\n", "00233");
		alarm_msg += "Extend to station not enabled";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00234;

		alarm_title = title;
		alarm_title += "Valve not closed";

		alarm_msg.Format("Error No. => [%s]\n","00234");

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00301;

		alarm_title = title;
		alarm_title += "Mnemonic already used";

		alarm_msg.Format("Error No. => [%s]\n","00301");
		alarm_msg += "Part of a command has been duplicated. This command argument must";
		alarm_msg += " be unique. Input a different command argument.";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00305;

		alarm_title = title;
		alarm_title += "Unrecognized command; expecting a mnemonic";

		alarm_msg.Format("Error No. => [%s]\n","00305");
		alarm_msg += "An optional parameter where at least one parameter is required is missing. Enter a valid command";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00306;

		alarm_title = title;
		alarm_title += "Value out of range";

		alarm_msg.Format("Error No. => [%s]\n","00306");
		alarm_msg += "Enter a value within range for the desired function";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00309;

		alarm_title = title;
		alarm_title += "Command not supported";

		alarm_msg.Format("Error No. => [%s]\n","00309");
		alarm_msg += "Enter the command EEPROM RESET";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00350;

		alarm_title = title;
		alarm_title += "Parser error, bad node in parse tree.";

		alarm_msg.Format("Error No. => [%s]\n","00350");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00351;

		alarm_title = title;
		alarm_title += "Parser error, stack overflow";

		alarm_msg.Format("Error No. => [%s]\n","00351");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00352;

		alarm_title = title;
		alarm_title += "Parse error, no memory available.";

		alarm_msg.Format("Error No. => [%s]\n","00352");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00353;

		alarm_title = title;
		alarm_title += "Unexpected mail to UIO task";

		alarm_msg.Format("Error No. => [%s]\n","00353");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00390;

		alarm_title = title;
		alarm_title += "Checksum is invalid.";

		alarm_msg.Format("Error No. => [%s]\n","00390");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00402;

		alarm_title = title;
		alarm_title += "Bad slot number";

		alarm_msg.Format("Error No. => [%s]\n","00402");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00405;

		alarm_title = title;
		alarm_title += "Bad Lower Position";

		alarm_msg.Format("Error No. => [%s]\n","00405");
		alarm_msg += "The entered Lower value yields an invalid value when subtracted from the station's BTO";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00406;

		alarm_title = title;
		alarm_title += "Bad Pitch";

		alarm_msg.Format("Error No. => [%s]\n","00406");
		alarm_msg += "The entered Pitch value yields an invalid value when multiplied by the number of slots.";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00407;

		alarm_title = title;
		alarm_title += "Bad T Position";

		alarm_msg.Format("Error No. => [%s]\n","00407");
		alarm_msg += "Invalid Theta value (T > 360¡Æ or T < 0¡Æ). Enter a theta position within the range of 0 to 360000,\n";
		alarm_msg += " where 360000 represents 360 degrees. Robot theta positions are represented in millidegrees.\n";
		alarm_msg += " Thus, 1 degree of robot travel equals 1000 units.";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00408;

		alarm_title = title;
		alarm_title += "Bad R value";

		alarm_msg.Format("Error No. => [%s]\n","00408");
		alarm_msg += "Value is either too small or too large for the arm to reach or an attempt was made to drive arm";
		alarm_msg += " past its limits";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00409;

		alarm_title = title;
		alarm_title += "Bad Z value";

		alarm_msg.Format("Error No. => [%s]\n","00409");
		alarm_msg += "Value is either too small or too large for the arm to reach or an attempt was made to drive arm";
		alarm_msg += " past its limits";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00414;

		alarm_title = title;
		alarm_title += "Push value must be Positive";

		alarm_msg.Format("Error No. => [%s]\n","00414");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00415;

		alarm_title = title;
		alarm_title += "Station R+PUSH value is Invalid.";

		alarm_msg.Format("Error No. => [%s]\n","00415");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00416;

		alarm_title = title;
		alarm_title += "Station not initialized";

		alarm_msg.Format("Error No. => [%s]\n","00416");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00417;

		alarm_title = title;
		alarm_title += "Offset too large";

		alarm_msg.Format("Error No. => [%s]\n","00417");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00418;

		alarm_title = title;
		alarm_title += "Bad retract position";

		alarm_msg.Format("Error No. => [%s]\n","00418");
		alarm_msg += "Invalid RTRCT2";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00508;

		alarm_title = title;
		alarm_title += "Wafer sensor not defined";

		alarm_msg.Format("Error No. => [%s]\n","00508");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00509;

		alarm_title = title;
		alarm_title += "No Z axis on robot.";

		alarm_msg.Format("Error No. => [%s]\n","00509");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00527;

		alarm_title = title;
		alarm_title += "MCC communication error";

		alarm_msg.Format("Error No. => [%s]\n","00527");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00528;

		alarm_title = title;
		alarm_title += "MCC Queue Full.";

		alarm_msg.Format("Error No. => [%s]\n","00528");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00550;

		alarm_title = title;
		alarm_title += "Station parameter out of range";

		alarm_msg.Format("Error No. => [%s]\n","00550");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00551;

		alarm_title = title;
		alarm_title += "Servo parameter out of range";

		alarm_msg.Format("Error No. => [%s]\n","00551");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00552;

		alarm_title = title;
		alarm_title += "Sensor out of range";

		alarm_msg.Format("Error No. => [%s]\n","00552");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00554;

		alarm_title = title;
		alarm_title += "Cannot resume due to unsuccessful HALT";

		alarm_msg.Format("Error No. => [%s]\n","00554");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00555;

		alarm_title = title;
		alarm_title += "Cannot resume because robot not HALTED";

		alarm_msg.Format("Error No. => [%s]\n","00555");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00557;

		alarm_title = title;
		alarm_title += "Robot did not respond";

		alarm_msg.Format("Error No. => [%s]\n","00557");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00558;

		alarm_title = title;
		alarm_title += "Robot unknown";

		alarm_msg.Format("Error No. => [%s]\n","00558");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00602;

		alarm_title = title;
		alarm_title += "Command sequencer busy";

		alarm_msg.Format("Error No. => [%s]\n","00602");
		alarm_msg += "Wait for the robot to complete its last operation. If the error persists, reset the robot by issuing";
		alarm_msg += "the RESET command or toggling power";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00603;

		alarm_title = title;
		alarm_title += "Command halted";

		alarm_msg.Format("Error No. => [%s]\n","00603");
		alarm_msg += "Wait for the robot to complete its last operation. If the error persists, reset the robot by issuing";
		alarm_msg += "the RESET command or toggling power.";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00604;

		alarm_title = title;
		alarm_title += "CDM in control of the robot";

		alarm_msg.Format("Error No. => [%s]\n","00604");
		alarm_msg += "The command issued requires control of the robot. Turn off the CDM prior to continuing.";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00605;

		alarm_title = title;
		alarm_title += "Digital I/O in control of robot";

		alarm_msg.Format("Error No. => [%s]\n","00605");
		alarm_msg += "To release digital I/O control of robot, enter the command DIO STOP.";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00606;

		alarm_title = title;
		alarm_title += "Serial I/O in control of robot";

		alarm_msg.Format("Error No. => [%s]\n","00606");
		alarm_msg += "To initiate digital I/O control of robot, enter the command DIO START";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00607;

		alarm_title = title;
		alarm_title += "MCC processor not alive";

		alarm_msg.Format("Error No. => [%s]\n","00607");
		alarm_msg += "Verify that no FETs of the theta driver board have shorted-out. For each FET, ";
		alarm_msg += "apply an ohmmeter between pins 1 and 3 to verify resistivity.";
		alarm_msg += " If a FET has shorted-out, replace theta driver board. ";
		alarm_msg += "If FETs are not shorted, replace PC104 Card. ";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00608;

		alarm_title = title;
		alarm_title += "Robot halting";

		alarm_msg.Format("Error No. => [%s]\n","00608");
		alarm_msg += "A HALT command has been issued to the robot which stops robot motion. If ";
		alarm_msg += "the robot is operating ";
		alarm_msg += "in the compatibility mode: COMPATIBILITY HALT VT5, then the robot must be homed ";
		alarm_msg += "prior to its next move. If the robot is operating in the compatibility mode: ";
		alarm_msg += "COMPATIBILITY ";
		alarm_msg += "HALT MAG6, then the robot will remain referenced and is ready for the next move.";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00610;

		alarm_title = title;
		alarm_title += "Emergency stop on";

		alarm_msg.Format("Error No. => [%s]\n","00610");
		alarm_msg += "The EMER_STOP interlock for the robot has been activated.\n";
		alarm_msg += "- The PowerPak accessory has been programmed using the EMER_STOP interlock";
		alarm_msg += " function but the PowerPak is not properly connected to the robot. Check ";
		alarm_msg += "PowerPak cables for proper connection and continuity.\n";
		alarm_msg += "- Verify that a robot emergency off (EMO) button has been activated and ";
		alarm_msg += "deactivate as appropriate.\n";
		alarm_msg += "- Check host controller software.";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00611;

		alarm_title = title;
		alarm_title += "Warning, CDM has been turned on.";

		alarm_msg.Format("Error No. => [%s]\n","00611");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00612;

		alarm_title = title;
		alarm_title += "Warning, CDM has been turned off.";

		alarm_msg.Format("Error No. => [%s]\n","00612");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00613;

		alarm_title = title;
		alarm_title += "UPS Battery is low";

		alarm_msg.Format("Error No. => [%s]\n","00613");
		alarm_msg += "The UPS_BATTERY_SEN interlock for the robot has been activated.";
		alarm_msg += "accessory is programmed using the UPS_BATTERY_SEN interlock function,";
		alarm_msg += "and the internal PowerPak battery voltage is less than 23.5 volts. Recharge or ";
		alarm_msg += "replace PowerPak as appropriate.\n";
		alarm_msg += "- Check host controller software.";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00652;

		alarm_title = title;
		alarm_title += "Unable to create command dispatcher";

		alarm_msg.Format("Error No. => [%s]\n","00652");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00653;

		alarm_title = title;
		alarm_title += "Unexpected mail received by dispatcher";

		alarm_msg.Format("Error No. => [%s]\n","00653");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00654;

		alarm_title = title;
		alarm_title += "Unknown command";

		alarm_msg.Format("Error No. => [%s]\n","00654");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00655;

		alarm_title = title;
		alarm_title += "Bad parameter passed to dispatcher";

		alarm_msg.Format("Error No. => [%s]\n","00655");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00656;

		alarm_title = title;
		alarm_title += "Command processing has finished";

		alarm_msg.Format("Error No. => [%s]\n","00656");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00700;

		alarm_title = title;
		alarm_title += "Wafer detected";

		alarm_msg.Format("Error No. => [%s]\n","00700");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00701;

		alarm_title = title;
		alarm_title += "No Wafer detected";

		alarm_msg.Format("Error No. => [%s]\n","00701");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00705;

		alarm_title = title;
		alarm_title += "Wafer missing";

		alarm_msg.Format("Error No. => [%s]\n","00705");
		alarm_msg += "Ensure wafer is present in VCE or process module prior to issuing ";
		alarm_msg += "PICK/PLACE/GOTO/XFER command.";
		alarm_msg += "Check host controller software.";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00706;

		alarm_title = title;
		alarm_title += "Wafer sensed";

		alarm_msg.Format("Error No. => [%s]\n","00706");
		alarm_msg += "Ensure slot in VCE or process module is empty prior to issuing ";
		alarm_msg += "PICK/PLACE/GOTO/XFER command. Check host controller software";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00710;

		alarm_title = title;
		alarm_title += "Slot valve closed prior PICK/PLACE/GOTO/XFER";

		alarm_msg.Format("Error No. => [%s]\n","00710");
		alarm_msg += "Ensure slot valve is open prior to issuing PICK/PLACE/GOTO/XFER ";
		alarm_msg += "command. Check slot valve for proper operation.";
		alarm_msg += "Check host controller software.";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00711;

		alarm_title = title;
		alarm_title += "Slot valve not open.";

		alarm_msg.Format("Error No. => [%s]\n","00711");
		alarm_msg += "Ensure slot valve is open prior to issuing PICK/PLACE/GOTO/XFER ";
		alarm_msg += "command. Check slot valve for proper operation.";
		alarm_msg += "Check host controller software.";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00715;

		alarm_title = title;
		alarm_title += "Possible material on arm";

		alarm_msg.Format("Error No. => [%s]\n","00715");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00721;

		alarm_title = title;
		alarm_title += "Pick failed";

		alarm_msg.Format("Error No. => [%s]\n","00721");
		alarm_msg += "Ensure wafer sensors are operating properly.";
		alarm_msg += "Ensure slot valve is operating properly.";
		alarm_msg += "Ensure slot valve is open prior to issuing PICK/PLACE/GOTO/XFER ";
		alarm_msg += "command. Check host controller software.";
	
		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00722;

		alarm_title = title;
		alarm_title += "Placed failed";

		alarm_msg.Format("Error No. => [%s]\n","00722");
		alarm_msg += "Ensure wafer sensors are operating properly.";
		alarm_msg += "Ensure slot valve is open prior to issuing PICK/PLACE/GOTO/XFER ";
		alarm_msg += "command. Check host controller software.";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00730;

		alarm_title = title;
		alarm_title += "RE wafer sensor error prior to PLACE: No Wafer Sensed";

		alarm_msg.Format("Error No. => [%s]\n","00730");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00731;

		alarm_title = title;
		alarm_title += "RE wafer sensor error after a PLACE: Wafer Sensed";

		alarm_msg.Format("Error No. => [%s]\n","00731");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00732;

		alarm_title = title;
		alarm_title += "EX wafer sensor error prior to a PLACE: Wafer Sensed";

		alarm_msg.Format("Error No. => [%s]\n","00732");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00733;

		alarm_title = title;
		alarm_title += "EX wafer sensor error after a PLACE: No Wafer Sensed";

		alarm_msg.Format("Error No. => [%s]\n","00733");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00734;

		alarm_title = title;
		alarm_title += "R_MT wafer sensor error on a PLACE: No Wafer Sensed during EXtend.";

		alarm_msg.Format("Error No. => [%s]\n","00734");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00735;

		alarm_title = title;
		alarm_title += "R_MT wafer sensor failure";

		alarm_msg.Format("Error No. => [%s]\n","00735");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00736;

		alarm_title = title;
		alarm_title += "R_MT wafer sensor error on a PLACE: Wafer Sensed during REtract";

		alarm_msg.Format("Error No. => [%s]\n","00736");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00738;

		alarm_title = title;
		alarm_title += "Active option in GOTO supported for R_MT wafer sensor only";

		alarm_msg.Format("Error No. => [%s]\n","00738");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00739;

		alarm_title = title;
		alarm_title += "R_MT wafer sensor error: Wafer Sensed on MAT_OFF move";

		alarm_msg.Format("Error No. => [%s]\n","00739");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00740;

		alarm_title = title;
		alarm_title += "RE wafer sensor error prior to a PICK: Wafer Sensed.";

		alarm_msg.Format("Error No. => [%s]\n","00740");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00741;

		alarm_title = title;
		alarm_title += "RE wafer sensor error after a PICK: No Wafer Sensed";

		alarm_msg.Format("Error No. => [%s]\n","00741");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00742;

		alarm_title = title;
		alarm_title += "EX wafer sensor error prior to a PICK: No Wafer Sensed";

		alarm_msg.Format("Error No. => [%s]\n","00742");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00743;

		alarm_title = title;
		alarm_title += "EX wafer sensor error after a PICK: Wafer Sensed";

		alarm_msg.Format("Error No. => [%s]\n","00743");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00744;

		alarm_title = title;
		alarm_title += "R_MT wafer sensor error on a PICK: Wafer Sensed during EXtend";

		alarm_msg.Format("Error No. => [%s]\n","00744");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00745;

		alarm_title = title;
		alarm_title += "R_MT wafer sensor error on a PICK: No Wafer Sensed during REtract.";

		alarm_msg.Format("Error No. => [%s]\n","00745");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00749;

		alarm_title = title;
		alarm_title += "R_MT wafer sensor error: No Wafer Sensed on MAT_ON move";

		alarm_msg.Format("Error No. => [%s]\n","00749");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00750;

		alarm_title = title;
		alarm_title += "No station with R_MT wafer sensor found for Arm A";

		alarm_msg.Format("Error No. => [%s]\n","00750");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00751;

		alarm_title = title;
		alarm_title += "No station with R_MT wafer sensor found for Arm B";

		alarm_msg.Format("Error No. => [%s]\n","00751");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00800;

		alarm_title = title;
		alarm_title += "Bad configuration name";

		alarm_msg.Format("Error No. => [%s]\n","00800");
		alarm_msg += "Verify robot application number is valid";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00801;

		alarm_title = title;
		alarm_title += "Database checksum error";

		alarm_msg.Format("Error No. => [%s]\n","00801");
		alarm_msg += "Issue an EEPROM RESET command.";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00802;

		alarm_title = title;
		alarm_title += "Arm not configured";

		alarm_msg.Format("Error No. => [%s]\n","00802");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00803;

		alarm_title = title;
		alarm_title += "Servo not configured";

		alarm_msg.Format("Error No. => [%s]\n","00803");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00804;

		alarm_title = title;
		alarm_title += "Motor not configured";

		alarm_msg.Format("Error No. => [%s]\n","00804");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00805;

		alarm_title = title;
		alarm_title += "Illegal configuration for this command";

		alarm_msg.Format("Error No. => [%s]\n","00805");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00810;

		alarm_title = title;
		alarm_title += "Cannot open Master configuration file";

		alarm_msg.Format("Error No. => [%s]\n","00810");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00811;

		alarm_title = title;
		alarm_title += "Cannot read from Master configuration file";

		alarm_msg.Format("Error No. => [%s]\n","00811");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00812;

		alarm_title = title;
		alarm_title += "Cannot open Object data file";

		alarm_msg.Format("Error No. => [%s]\n","00812");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00813;

		alarm_title = title;
		alarm_title += "Cannot read Object data file";

		alarm_msg.Format("Error No. => [%s]\n","00813");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00814;

		alarm_title = title;
		alarm_title += "Cannot open Object master file";

		alarm_msg.Format("Error No. => [%s]\n","00814");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00815;

		alarm_title = title;
		alarm_title += "Cannot read Object master file";

		alarm_msg.Format("Error No. => [%s]\n","00815");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00816;

		alarm_title = title;
		alarm_title += "Cannot open Current configuration file";

		alarm_msg.Format("Error No. => [%s]\n","00816");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00817;

		alarm_title = title;
		alarm_title += "Cannot read from Current configuration file";

		alarm_msg.Format("Error No. => [%s]\n","00817");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00818;

		alarm_title = title;
		alarm_title += "Cannot write to Current Configuration file";

		alarm_msg.Format("Error No. => [%s]\n","00818");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00819;

		alarm_title = title;
		alarm_title += "Object checksum error";

		alarm_msg.Format("Error No. => [%s]\n","00819");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00820;

		alarm_title = title;
		alarm_title += "Could not send generic object to MCC";

		alarm_msg.Format("Error No. => [%s]\n","00820");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00821;

		alarm_title = title;
		alarm_title += "Object not found";

		alarm_msg.Format("Error No. => [%s]\n","00821");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00822;

		alarm_title = title;
		alarm_title += "Object not valid for current configuration";

		alarm_msg.Format("Error No. => [%s]\n","00822");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00823;

		alarm_title = title;
		alarm_title += "Bad group type";

		alarm_msg.Format("Error No. => [%s]\n","00823");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00824;

		alarm_title = title;
		alarm_title += "Bad group name";

		alarm_msg.Format("Error No. => [%s]\n","00824");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00825;

		alarm_title = title;
		alarm_title += "Group not found";

		alarm_msg.Format("Error No. => [%s]\n","00825");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00826;

		alarm_title = title;
		alarm_title += "Group not valid for current configuration";

		alarm_msg.Format("Error No. => [%s]\n","00826");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00827;

		alarm_title = title;
		alarm_title += "Configuration message to MCC timed out";

		alarm_msg.Format("Error No. => [%s]\n","00827");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00850;

		alarm_title = title;
		alarm_title += "End of database found";

		alarm_msg.Format("Error No. => [%s]\n","00850");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00851;

		alarm_title = title;
		alarm_title += "Unable to read from database";

		alarm_msg.Format("Error No. => [%s]\n","00851");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00852;

		alarm_title = title;
		alarm_title += "Unable to write to database";

		alarm_msg.Format("Error No. => [%s]\n","00852");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00853;

		alarm_title = title;
		alarm_title += "Bad database handle found";

		alarm_msg.Format("Error No. => [%s]\n","00853");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00854;

		alarm_title = title;
		alarm_title += "Database full";

		alarm_msg.Format("Error No. => [%s]\n","00854");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00855;

		alarm_title = title;
		alarm_title += "Database not initialized";

		alarm_msg.Format("Error No. => [%s]\n","00855");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00857;

		alarm_title = title;
		alarm_title += "Configuration files have different stam";

		alarm_msg.Format("Error No. => [%s]\n","00857");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00860;

		alarm_title = title;
		alarm_title += "Bad parameter passes to memory system";

		alarm_msg.Format("Error No. => [%s]\n","00860");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00861;

		alarm_title = title;
		alarm_title += "No memory available for memory system";

		alarm_msg.Format("Error No. => [%s]\n","00861");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00862;

		alarm_title = title;
		alarm_title += "Partition currently in use";

		alarm_msg.Format("Error No. => [%s]\n","00862");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00950;

		alarm_title = title;
		alarm_title += "Unexpected mail received by monitor";

		alarm_msg.Format("Error No. => [%s]\n","00950");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00951;

		alarm_title = title;
		alarm_title += "No monitor resources available";

		alarm_msg.Format("Error No. => [%s]\n","00951");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00952;

		alarm_title = title;
		alarm_title += "Unknown monitor event type";

		alarm_msg.Format("Error No. => [%s]\n","00952");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00953;

		alarm_title = title;
		alarm_title += "Monitor event canceled";

		alarm_msg.Format("Error No. => [%s]\n","00953");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00954;

		alarm_title = title;
		alarm_title += "Event time-out occurred";

		alarm_msg.Format("Error No. => [%s]\n","00954");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00955;

		alarm_title = title;
		alarm_title += "Monitored event occurred";

		alarm_msg.Format("Error No. => [%s]\n","00955");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_00956;

		alarm_title = title;
		alarm_title += "Bad monitor function received";

		alarm_msg.Format("Error No. => [%s]\n","00956");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_01001;

		alarm_title = title;
		alarm_title += "Unknown I/O State type";

		alarm_msg.Format("Error No. => [%s]\n","01001");
		alarm_msg += "Check host controller software for proper I/O state type. ";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_01002;

		alarm_title = title;
		alarm_title += "Unknown I/O name";

		alarm_msg.Format("Error No. => [%s]\n","01002");
		alarm_msg += "Choose a correct I/O name. A list of existing I/O names can be obtained by ";
		alarm_msg += "issuing the command RQ IO MAP ALL";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_01003;

		alarm_title = title;
		alarm_title += "I/O name already in use";

		alarm_msg.Format("Error No. => [%s]\n","01003");
		alarm_msg += "If appropriate, delete existing I/O name using the REMOVE IO command. Choose a different I/O name.";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_01004;

		alarm_title = title;
		alarm_title += "I/O system out of memory";

		alarm_msg.Format("Error No. => [%s]\n","01004");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_01005;

		alarm_title = title;
		alarm_title += "Name reserved by I/O system";

		alarm_msg.Format("Error No. => [%s]\n","01005");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_01006;

		alarm_title = title;
		alarm_title += "Illegal number of bits for I/O type";

		alarm_msg.Format("Error No. => [%s]\n","01006");
		alarm_msg += "Assign the proper number of bits for the I/O type specified";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_01007;

		alarm_title = title;
		alarm_title += "Unknown I/O block name";

		alarm_msg.Format("Error No. => [%s]\n","01007");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_01008;

		alarm_title = title;
		alarm_title += "Bad I/O bitmask";

		alarm_msg.Format("Error No. => [%s]\n","01008");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_01009;

		alarm_title = title;
		alarm_title += "Unknown I/O type";

		alarm_msg.Format("Error No. => [%s]\n","01009");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_01010;

		alarm_title = title;
		alarm_title += "I/O type mismatch";

		alarm_msg.Format("Error No. => [%s]\n","01010");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_01011;

		alarm_title = title;
		alarm_title += "Incorrect I/O channel specified";

		alarm_msg.Format("Error No. => [%s]\n","01011");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_01012;

		alarm_title = title;
		alarm_title += "Bad I/O handle";

		alarm_msg.Format("Error No. => [%s]\n","01012");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_01013;

		alarm_title = title;
		alarm_title += "Unknown I/O state";

		alarm_msg.Format("Error No. => [%s]\n","01013");
		alarm_msg += "Check host controller software for proper I/O state. ";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_01014;

		alarm_title = title;
		alarm_title += "I/O is write only";

		alarm_msg.Format("Error No. => [%s]\n","01014");
		alarm_msg += "An attempt was made to write to an output.";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_01015;

		alarm_title = title;
		alarm_title += "I/O is read only.";

		alarm_msg.Format("Error No. => [%s]\n","01015");
		alarm_msg += "An attempt was made to read from an input.";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_01100;

		alarm_title = title;
		alarm_title += "Current position not within work space.";

		alarm_msg.Format("Error No. => [%s]\n","01100");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_01101;

		alarm_title = title;
		alarm_title += "Destination position not within work space";

		alarm_msg.Format("Error No. => [%s]\n","01101");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_01102;

		alarm_title = title;
		alarm_title += "Work spaces do not overlap";

		alarm_msg.Format("Error No. => [%s]\n","01102");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_01103;

		alarm_title = title;
		alarm_title += "Work space interlock occurred";

		alarm_msg.Format("Error No. => [%s]\n","01103");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_01104;

		alarm_title = title;
		alarm_title += "No more work spaces available";

		alarm_msg.Format("Error No. => [%s]\n","01104");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_01105;

		alarm_title = title;
		alarm_title += "The work space volume must be specified";

		alarm_msg.Format("Error No. => [%s]\n","01105");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_01106;

		alarm_title = title;
		alarm_title += "Radial maximum is less than radial minimum";

		alarm_msg.Format("Error No. => [%s]\n","01106");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_01108;

		alarm_title = title;
		alarm_title += "Z maximum is less than Z minimum";

		alarm_msg.Format("Error No. => [%s]\n","01108");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_01109;

		alarm_title = title;
		alarm_title += "Radial minimum is greater than stored radial max";

		alarm_msg.Format("Error No. => [%s]\n","01109");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_01110;

		alarm_title = title;
		alarm_title += "Radial maximum is less than stored radial min";

		alarm_msg.Format("Error No. => [%s]\n","01110");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_01113;

		alarm_title = title;
		alarm_title += "Z minimum is greater than stored Z maximum";

		alarm_msg.Format("Error No. => [%s]\n","01113");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_01114;

		alarm_title = title;
		alarm_title += "Z maximum is less than stored Z minimum";

		alarm_msg.Format("Error No. => [%s]\n","01114");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_01115;

		alarm_title = title;
		alarm_title += "Work space name does not exist";

		alarm_msg.Format("Error No. => [%s]\n","01115");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_01118;

		alarm_title = title;
		alarm_title += "Invalid station number";

		alarm_msg.Format("Error No. => [%s]\n","01118");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_01119;

		alarm_title = title;
		alarm_title += "Reserved work space name used";

		alarm_msg.Format("Error No. => [%s]\n","01119");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_01300;

		alarm_title = title;
		alarm_title += "Bad mail message received by MCC";

		alarm_msg.Format("Error No. => [%s]\n","01300");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_01302;

		alarm_title = title;
		alarm_title += "Command halted";

		alarm_msg.Format("Error No. => [%s]\n","01302");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_01307;

		alarm_title = title;
		alarm_title += "MCC Queue Full";

		alarm_msg.Format("Error No. => [%s]\n","01307");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_01308;

		alarm_title = title;
		alarm_title += "Could not calculate MCC command ID";

		alarm_msg.Format("Error No. => [%s]\n","01308");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_01309;

		alarm_title = title;
		alarm_title += "Dual Ported RAM lock fail";

		alarm_msg.Format("Error No. => [%s]\n","01309");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_01310;

		alarm_title = title;
		alarm_title += "Unable to send to MCC";

		alarm_msg.Format("Error No. => [%s]\n","01310");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_01311;

		alarm_title = title;
		alarm_title += "Error opening MCC code";

		alarm_msg.Format("Error No. => [%s]\n","01311");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_01312;

		alarm_title = title;
		alarm_title += "Error reading MCC code";

		alarm_msg.Format("Error No. => [%s]\n","01312");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_01313;

		alarm_title = title;
		alarm_title += "MCC task can't access DP RAM";

		alarm_msg.Format("Error No. => [%s]\n","01313");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_01314;

		alarm_title = title;
		alarm_title += "MCC DP RAM memory size is too small";

		alarm_msg.Format("Error No. => [%s]\n","01314");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_01600;

		alarm_title = title;
		alarm_title += "Bad date format";

		alarm_msg.Format("Error No. => [%s]\n","01600");
		alarm_msg += "Enter the date using a two digit number to represent the month, day, and year. ";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_01601;

		alarm_title = title;
		alarm_title += "Bad time format";

		alarm_msg.Format("Error No. => [%s]\n","01601");
		alarm_msg += "Enter the time using a two digit number to represent the hour, minute, and ";
		alarm_msg += "second. Separate the hour, minute, and second using the : character";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_01602;

		alarm_title = title;
		alarm_title += "Year out of range";

		alarm_msg.Format("Error No. => [%s]\n","01602");
		alarm_msg += "Enter the year using a two digit number in the range of 00 through 99.";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_01603;

		alarm_title = title;
		alarm_title += "Month out of range";

		alarm_msg.Format("Error No. => [%s]\n","01603");
		alarm_msg += "Enter the month using a two digit number in the range of 01 through 12.";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_01604;

		alarm_title = title;
		alarm_title += "Day out of range";

		alarm_msg.Format("Error No. => [%s]\n","01604");
		alarm_msg += "Enter the day using a two digit number in the range of 01 through 31.";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_01605;

		alarm_title = title;
		alarm_title += "Hour out of range";

		alarm_msg.Format("Error No. => [%s]\n","01605");
		alarm_msg += "Enter the hour using a two digit number in the range of 01 through 23.";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_01606;

		alarm_title = title;
		alarm_title += "Minute out of range";

		alarm_msg.Format("Error No. => [%s]\n","01606");
		alarm_msg += "Enter the minute using a two digit number in the range of 01 through 59.";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_01607;

		alarm_title = title;
		alarm_title += "Second out of range";

		alarm_msg.Format("Error No. => [%s]\n","01607");
		alarm_msg += "Enter the second using a two digit number in the range of 01 through 59.";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_01800;

		alarm_title = title;
		alarm_title += "CDM already initialized";

		alarm_msg.Format("Error No. => [%s]\n","01800");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_01801;

		alarm_title = title;
		alarm_title += "CDM escape key entered";

		alarm_msg.Format("Error No. => [%s]\n","01801");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_01802;

		alarm_title = title;
		alarm_title += "CDM quit key entered";

		alarm_msg.Format("Error No. => [%s]\n","01802");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_01803;

		alarm_title = title;
		alarm_title += "CDM bad parameter";

		alarm_msg.Format("Error No. => [%s]\n","01803");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_01804;

		alarm_title = title;
		alarm_title += "CDM move aborted";

		alarm_msg.Format("Error No. => [%s]\n","01804");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_01805;

		alarm_title = title;
		alarm_title += "CDM Has Control of Robot";

		alarm_msg.Format("Error No. => [%s]\n","01805");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_01900;

		alarm_title = title;
		alarm_title += "Unknown serial port";

		alarm_msg.Format("Error No. => [%s]\n","01900");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_01901;

		alarm_title = title;
		alarm_title += "Unable to open serial port";

		alarm_msg.Format("Error No. => [%s]\n","01901");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_01902;

		alarm_title = title;
		alarm_title += "Unable to close serial port";

		alarm_msg.Format("Error No. => [%s]\n","01902");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_01903;

		alarm_title = title;
		alarm_title += "Can't allocate serial port semaphore";

		alarm_msg.Format("Error No. => [%s]\n","01903");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_01904;

		alarm_title = title;
		alarm_title += "Serial port overflow";

		alarm_msg.Format("Error No. => [%s]\n","01904");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_01905;

		alarm_title = title;
		alarm_title += "Serial port empty";

		alarm_msg.Format("Error No. => [%s]\n","01905");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_01910;

		alarm_title = title;
		alarm_title += "Secondary Serial Port mode";

		alarm_msg.Format("Error No. => [%s]\n","01910");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_01911;

		alarm_title = title;
		alarm_title += "Secondary Serial Port is busy";

		alarm_msg.Format("Error No. => [%s]\n","01911");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_01912;

		alarm_title = title;
		alarm_title += "Secondary Serial Port response timeout.";

		alarm_msg.Format("Error No. => [%s]\n","01912");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_01920;

		alarm_title = title;
		alarm_title += "No serial communication with remote MCC";

		alarm_msg.Format("Error No. => [%s]\n","01920");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_02000;

		alarm_title = title;
		alarm_title += "No memory available for multi-tasker";

		alarm_msg.Format("Error No. => [%s]\n","02000");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_02001;

		alarm_title = title;
		alarm_title += "Multi-tasking kernel error";

		alarm_msg.Format("Error No. => [%s]\n","02001");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_02002;

		alarm_title = title;
		alarm_title += "Bad parameter passed to multi-tasker";

		alarm_msg.Format("Error No. => [%s]\n","02002");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_02003;

		alarm_title = title;
		alarm_title += "Timeout occurred";

		alarm_msg.Format("Error No. => [%s]\n","02003");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_02004;

		alarm_title = title;
		alarm_title += "Illegal task block requested";

		alarm_msg.Format("Error No. => [%s]\n","02004");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_02005;

		alarm_title = title;
		alarm_title += "No resources available";

		alarm_msg.Format("Error No. => [%s]\n","02005");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_02100;

		alarm_title = title;
		alarm_title += "Unable to read from NonVolatile RAM";

		alarm_msg.Format("Error No. => [%s]\n","02100");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_02101;

		alarm_title = title;
		alarm_title += "Unable to write to NonVolatile RAM";

		alarm_msg.Format("Error No. => [%s]\n","02101");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_02102;

		alarm_title = title;
		alarm_title += "NonVolatile RAM overflow";

		alarm_msg.Format("Error No. => [%s]\n","02102");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_02200;

		alarm_title = title;
		alarm_title += "No memory available for mail system";

		alarm_msg.Format("Error No. => [%s]\n","02200");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_02202;

		alarm_title = title;
		alarm_title += "Error initializing mail system";

		alarm_msg.Format("Error No. => [%s]\n","02202");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_02203;

		alarm_title = title;
		alarm_title += "Unknown task ID passed to mail system";

		alarm_msg.Format("Error No. => [%s]\n","02203");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_03000;

		alarm_title = title;
		alarm_title += "Trace currently running";

		alarm_msg.Format("Error No. => [%s]\n","03000");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_03001;

		alarm_title = title;
		alarm_title += "Trace variable already set";

		alarm_msg.Format("Error No. => [%s]\n","03001");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_03002;

		alarm_title = title;
		alarm_title += "Trace variable not set";

		alarm_msg.Format("Error No. => [%s]\n","03002");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_03003;

		alarm_title = title;
		alarm_title += "Bad trace variable name";

		alarm_msg.Format("Error No. => [%s]\n","03003");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_03004;

		alarm_title = title;
		alarm_title += "Bad trace trigger name";

		alarm_msg.Format("Error No. => [%s]\n","03004");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_03005;

		alarm_title = title;
		alarm_title += "No trace variables set";

		alarm_msg.Format("Error No. => [%s]\n","03005");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_03011;

		alarm_title = title;
		alarm_title += "Bad trace period";

		alarm_msg.Format("Error No. => [%s]\n","03011");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_04001;

		alarm_title = title;
		alarm_title += "Serial number not set";

		alarm_msg.Format("Error No. => [%s]\n","04001");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_04002;

		alarm_title = title;
		alarm_title += "System not configured";

		alarm_msg.Format("Error No. => [%s]\n","04002");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_04003;

		alarm_title = title;
		alarm_title += "System already born";

		alarm_msg.Format("Error No. => [%s]\n","04003");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_04004;

		alarm_title = title;
		alarm_title += "Operator name not set";

		alarm_msg.Format("Error No. => [%s]\n","04004");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_04005;

		alarm_title = title;
		alarm_title += "Message log bad record";

		alarm_msg.Format("Error No. => [%s]\n","04005");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_04006;

		alarm_title = title;
		alarm_title += "Message log not found";

		alarm_msg.Format("Error No. => [%s]\n","04006");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_04007;

		alarm_title = title;
		alarm_title += "Message log write error";

		alarm_msg.Format("Error No. => [%s]\n","04007");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_04008;

		alarm_title = title;
		alarm_title += "Message log seek error";

		alarm_msg.Format("Error No. => [%s]\n","04008");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_04009;

		alarm_title = title;
		alarm_title += "Message log read error";

		alarm_msg.Format("Error No. => [%s]\n","04009");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_04010;

		alarm_title = title;
		alarm_title += "Checksum error in message log";

		alarm_msg.Format("Error No. => [%s]\n","04010");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_04011;

		alarm_title = title;
		alarm_title += "Beginning of message log encountered";

		alarm_msg.Format("Error No. => [%s]\n","04011");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_04012;

		alarm_title = title;
		alarm_title += "Error log not initialized";

		alarm_msg.Format("Error No. => [%s]\n","04012");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_10000;

		alarm_title = title;
		alarm_title += "Default debug message from the MCC";

		alarm_msg.Format("Error No. => [%s]\n","10000");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_10001;

		alarm_title = title;
		alarm_title += "Sync error, motor moving or encoder noisy";

		alarm_msg.Format("Error No. => [%s]\n","10001");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_10002;

		alarm_title = title;
		alarm_title += "MCC board memory allocation error";

		alarm_msg.Format("Error No. => [%s]\n","10002");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_10003;

		alarm_title = title;
		alarm_title += "MCC board unexpected event error";

		alarm_msg.Format("Error No. => [%s]\n","10003");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_10004;

		alarm_title = title;
		alarm_title += "MCC board, bad command state";

		alarm_msg.Format("Error No. => [%s]\n","10004");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_10005;

		alarm_title = title;
		alarm_title += "MCC Board Sync error, can't move motor";

		alarm_msg.Format("Error No. => [%s]\n","10005");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_10006;

		alarm_title = title;
		alarm_title += "MCC encoder VABS adjusted (small)";

		alarm_msg.Format("Error No. => [%s]\n","10006");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_10007;

		alarm_title = title;
		alarm_title += "Warning, unable to obtain position";

		alarm_msg.Format("Error No. => [%s]\n","10007");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_10008;

		alarm_title = title;
		alarm_title += "MCC unable to hold position";

		alarm_msg.Format("Error No. => [%s]\n","10008");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_10009;

		alarm_title = title;
		alarm_title += "MCC hard tracking error";

		alarm_msg.Format("Error No. => [%s]\n","10009");
		alarm_msg += "Verify that robot arm state is correct (arms on or arms off).";
		alarm_msg += "Verify robot application number is correct.";
		alarm_msg += "Check for physical obstruction. Remove or adjust physical obstruction to ";
		alarm_msg += "prevent interference.";
		alarm_msg += "While attempting theta or radial motion, verify that all 3 phase LEDs of the T1 ";
		alarm_msg += "motor (DS 1, DS 2, DS3) and all 3 phase LEDs of the T2 motor (DS2, DS 4, DS6) are ";
		alarm_msg += "illuminated on the theta driver board.";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_10010;

		alarm_title = title;
		alarm_title += "MCC soft tracking error";

		alarm_msg.Format("Error No. => [%s]\n","10010");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_10011;

		alarm_title = title;
		alarm_title += "Error, motor is already moving";

		alarm_msg.Format("Error No. => [%s]\n","10011");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_10012;

		alarm_title = title;
		alarm_title += "Error, motor is not configured";

		alarm_msg.Format("Error No. => [%s]\n","10012");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_10013;

		alarm_title = title;
		alarm_title += "Error, motor is not referenced";

		alarm_msg.Format("Error No. => [%s]\n","10013");
		alarm_msg += "Home the robot by issuing the command HOME ALL";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_10014;

		alarm_title = title;
		alarm_title += "Error, motor is already referencing";

		alarm_msg.Format("Error No. => [%s]\n","10014");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_10015;

		alarm_title = title;
		alarm_title += "Error, motor is currently moving";

		alarm_msg.Format("Error No. => [%s]\n","10015");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_10016;

		alarm_title = title;
		alarm_title += "Error, unable to calculate trajectory";

		alarm_msg.Format("Error No. => [%s]\n","10016");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_10017;

		alarm_title = title;
		alarm_title += " Illegal number of polls calculated";

		alarm_msg.Format("Error No. => [%s]\n","10017");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_10018;

		alarm_title = title;
		alarm_title += "Unable to calculate absolute position";

		alarm_msg.Format("Error No. => [%s]\n","10018");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_10019;

		alarm_title = title;
		alarm_title += "Error, Encoder off by many sectors";

		alarm_msg.Format("Error No. => [%s]\n","10019");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_10020;

		alarm_title = title;
		alarm_title += "Error, Encoder failed multiple times";

		alarm_msg.Format("Error No. => [%s]\n","10020");
		alarm_msg += "Check if failure location is repeatable. Record position of failure.";
		alarm_msg += "For Z encoder failure:\n";
		alarm_msg += "Verify Z encoder is secured to leadscrew via two 4-40 SHCS and Loctite.\n";
		alarm_msg += "Verify Z encoder is tightly secured to robot chassis via three M3 SHCS.\n";
		alarm_msg += "Replace Z encoder. Call Brooks Technical Support.\n";
		alarm_msg += "For T1 or T2 encoder failures:\n";
		alarm_msg += "Record which encoder (T1 or T2) failure occurred.";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_10021;

		alarm_title = title;
		alarm_title += "Error, Board Power Failure (Blown Fuse)";

		alarm_msg.Format("Error No. => [%s]\n","10021");
		alarm_msg += "Check if fuse is blown by inspecting fuses or if respective LED is not illuminated";
		alarm_msg += "Check if robot cables and/or board set have a good ground to the chassis.";
		alarm_msg += "Verify that power to robot is 24 ¡¾2 VDC.";
		alarm_msg += "Inspect robot power cable for proper connection and continuity.";
		alarm_msg += "Check all FET's for short circuits. ";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_10022;

		alarm_title = title;
		alarm_title += "Error, Z axis overtravel limit reached";

		alarm_msg.Format("Error No. => [%s]\n","10022");
		alarm_msg += "Reissue motion command.";
		alarm_msg += "Inspect appropriate Z overtravel limit switch for possible obstruction resulting ";
		alarm_msg += "in switch activation.";
		alarm_msg += "Adjust the Z axis overtravel limit switch";
		alarm_msg += "Move robot so that Z overtravel limit switch is not activated. Manually toggle ";
		alarm_msg += "switch to verify";
		alarm_msg += "operation. If switch does not toggle, replace Z axis overtravel limit switch.";
		alarm_msg += "Replace Z driver board";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_10023;

		alarm_title = title;
		alarm_title += "Arm actual position impossible, check sync zero";

		alarm_msg.Format("Error No. => [%s]\n","10023");
		alarm_msg += "Determine the sync zero values presently stored in the robot by issuing the ";
		alarm_msg += "command RQ SYNC ZERO ALL. Record the values.";
		alarm_msg += "All robot stations must be retaught after completing the next step: Redefine the ";
		alarm_msg += "home position to the desired location. ";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_10024;

		alarm_title = title;
		alarm_title += "Error, MCC watchdog timed out";

		alarm_msg.Format("Error No. => [%s]\n","10024");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_10025;

		alarm_title = title;
		alarm_title += "Error, defective R_MT type wafer sensor";

		alarm_msg.Format("Error No. => [%s]\n","10025");
		alarm_msg += "Verify that the radial and theta positions taught for the R_MT type wafer ";
		alarm_msg += "sensor are accurate. Check host controller software.";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_10026;

		alarm_title = title;
		alarm_title += "Error, arm load not what expected";

		alarm_msg.Format("Error No. => [%s]\n","10026");
		alarm_msg += "Verify robot application number is correct by issuing the command RQ ";
		alarm_msg += "CONFIG. The correct";
		alarm_msg += "application number can be obtained from the Quality Report (QR) that ";
		alarm_msg += "shipped with the robot or by contacting Brooks Technical Support.";
		alarm_msg += "Remove wafer and repeat robot move. If the robot armset moves properly, the ";
		alarm_msg += "wafer is too large for the robot application number servos.";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_10028;

		alarm_title = title;
		alarm_title += "Error, obstruction encounter for axis";

		alarm_msg.Format("Error No. => [%s]\n","10028");
		alarm_msg += "Inspect for physical obstruction.";
		alarm_msg += "Reteach the station to ensure the end effector is not scraping a surface, ";
		alarm_msg += "particularly VCE cassette slots.";
		alarm_msg += "Inspect the Z-Driver board. If the part number is 002-4234-01, then verify that ";
		alarm_msg += "a resistor has been placed on the back of the board. Request TSB-259 from Brooks ";
		alarm_msg += "Automation Technical Support.";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_10029;

		alarm_title = title;
		alarm_title += "Error, Emergency Stop circuit is active";

		alarm_msg.Format("Error No. => [%s]\n","10029");
		alarm_msg += "Verify that one of the following Motor Interlock Bypass mechanisms is in \n";
		alarm_msg += "place:\n";
		alarm_msg += "Motor Interlock Bypass Jumper Block located at designation J7 on the I/O ";
		alarm_msg += "board part number 002-3758-01. MISC I/O connector pins 23 and 24.";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_10030;

		alarm_title = title;
		alarm_title += "Error, excessive current detected";

		alarm_msg.Format("Error No. => [%s]\n","10030");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_10031;

		alarm_title = title;
		alarm_title += "Warning: Z Home Sensor position moved";

		alarm_msg.Format("Error No. => [%s]\n","10031");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_10032;

		alarm_title = title;
		alarm_title += "MCC MAP failed";

		alarm_msg.Format("Error No. => [%s]\n","10032");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_10034;

		alarm_title = title;
		alarm_title += "Error, encoder min/max value out of range";

		alarm_msg.Format("Error No. => [%s]\n","10034");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_10035;

		alarm_title = title;
		alarm_title += "Error, bad sync phase offset value";

		alarm_msg.Format("Error No. => [%s]\n","10035");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_10036;

		alarm_title = title;
		alarm_title += "Error, robot links are not yet defined";

		alarm_msg.Format("Error No. => [%s]\n","10036");
		alarm_msg += "";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_ERR_Unknown;

		alarm_title  = title;
		alarm_title += "VACRB: Undefined Error";

		alarm_msg.Format("Error No. => [%s]\n", "Unknown");
		alarm_msg += "Please Reference MagnaTran Hardware Manual";

		REG_ACT__ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VACRB1_MATERIAL_DIFFERENT_INFO;
	
		alarm_title  = title;
		alarm_title += "VACRB: Mapping Information Error";

		alarm_msg.Format("After Map Complete, Wafer Status is different from Mapping Sensor.\n");
		//	alarm_msg += "Abort [MAINT], Yes [Updated... from Sensor Status]";

		REG_ACT__ABORT_SENSORSYNC;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}
}
