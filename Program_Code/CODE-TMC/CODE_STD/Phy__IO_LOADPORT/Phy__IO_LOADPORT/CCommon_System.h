#pragma once


// ...
#define  OFFLINE			-1
#define  ONLINE				1


// CMD
#define DOOR_UP				"DOOR_UP"
#define DOOR_DN				"DOOR_DN"
#define DOOR_EXT			"DOOR_EXT"
#define DOOR_RET			"DOOR_RET"
#define DOOR_VAC_ON			"DOOR_VAC_ON"
#define DOOR_VAC_OFF		"DOOR_VAC_OFF"
#define LATCH				"LATCH"
#define UNLATCH				"UNLATCH"
#define SHUTTLE_IN			"SHUTTLE_IN"
#define SHUTTLE_OUT			"SHUTTLE_OUT"
#define CLAMP				"CLAMP"
#define UNCLAMP				"UNCLAMP"


#define  SP					0x20
#define  CR					0x0D
#define  LF					0x0A
#define  SOH				0x01
#define  STX				0x02
#define  ETX				0x03
#define  ACK				0x06
#define  NAK				0x15
#define  EOT				0x04
#define  ENQ				0x05
#define	 SAC				0x23
#define	 ECH				0x3E
#define  PACKET_ADR_DEF		0x30

#define PACKET_SIZE			17		// 18


//통신관련
#define HEADER_SIZE			3		// SOH + LEN (2)
#define ADDRESS_SIZE		2		
#define CHECKSUM_SIZE		2

#define CMD_FAIL			0		// ("Fail")
#define CMD_PROCESS			1		// ("Process")
#define CMD_SUCCESS			2		// ("Success")
#define CMD_MAX				23

#define STATE_MAX			9
#define BASE_ERROR			2454

#define  BFF_SIZE			1024

#define  WAFER_SLOT_MAX		30

#define LAMP_OFF			0	
#define	LAMP_ON				1
#define	LAMP_BLINK			2
#define	LAMP_ALL			77

#define	LAMP_MAX			6

#define	LAMP_UNLOAD			0
#define	LAMP_LOAD			1
#define	LAMP_PAUSE			2
#define	LAMP_RESUME			3
#define	LAMP_AUTO			4
#define	LAMP_RESERVE		5


enum STATE_ENUM
{
	STS__CASSETTE = 0,
	STS__CLAMP,
	STS__LATCH,	STS__VACCUM,
	STS__DOOR,
	STS__SHUTTLE  = 5,
	STS__PADDLE,
	STS__PRESENT,
	STS__PLACEMENT,
};

enum COMMAND_ENUM
{
	CMD__INIT = 0,
	CMD__INITX,
	CMD__MAP,
	CMD__LOAD,
	CMD__UNLOAD,
	CMD__EXTEND_PADDLE = 5,
	CMD__RETRACT_PADDLE,
	CMD__LATCH,
	CMD__UNLATCH,
	CMD__CLOSE_DOOR,
	CMD__OPEN_DOOR = 10,
	CMD__VAC_ON,
	CMD__VAC_OFF,
	CMD__CLAMP,
	CMD__UNCLAMP,
	CMD__EXTEND_SHUTTLE = 15,
	CMD__RETRACT_SHUTTLE,
	CMD__RFID_READ,
	CMD__REID_WRITE,
	CMD__HALT,
	CMD__EVENT = 20,
	CMD__STATUS,
	CMD__LAMP,
};


#define CMD_TIMEOUT				20
#define NOT_FULL_AUTO			3

/*
#define STR_MODE_PREPLOAD		"PREPLOAD"
#define STR_MODE_UNLOAD 		"UNLOAD"
*/

// ...
#define	LP_SLOT_MAX				25
#define	TIMEOUT_ERROR_RET		-9999


enum ENUM_SNS
{
	SNS_INIT = 0, 
	SNS_MAP,
	SNS_LOAD,
	SNS_UNLOAD,
	SNS_EXTENDPADDLE,
	SNS_RETRACTPADDLE = 5,
	SNS_LATCH,
	SNS_UNLATCH,
	SNS_CLOSEDOOR,
	SNS_OPENDOOR,
	SNS_VACON = 10,
	SNS_VACOFF,
	SNS_CLAMP,
	SNS_UNCLAMP,
	SNS_EXTENDSHUTTLE,
	SNS_RETRACTSHUTTLE = 15,
	SNS_HALT,
	SNS_LAMP,
	SNS_INITX,
	SNS_EVON,
	SNS_EVOFF = 20,
	SNS_STATUS
};
