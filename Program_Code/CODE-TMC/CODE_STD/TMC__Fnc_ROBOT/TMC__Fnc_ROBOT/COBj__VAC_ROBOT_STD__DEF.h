#pragma once


// ...
enum {NORMAL, ACTIVE_HANDOFF};

enum {NOT__X_PICK_PLACE = -1, X_PICK_PLACE = 1};

enum {ACT_HOFF__SWAP__PICK_X__PLACE = -1, ACT_HOFF__SWAP__PLACE_X__PICK = 1};


// ...
#define  CMMD__INIT				"INIT"
#define  CMMD__ROTATE			"ROTATE"
#define  CMMD__PICK				"PICK"
#define  CMMD__XPICK			"XPICK"
#define  CMMD__PLACE			"PLACE"
#define  CMMD__XPLACE			"XPLACE"

#define  CMMD__HOME				"HOME"
#define  CMMD__MAP				"MAP"

#define  CMMD__RETRACT			"RETRACT"
#define  CMMD__EXTEND			"EXTEND"
#define  CMMD__GOUP				"GOUP"
#define  CMMD__GODOWN			"GODOWN"

// ...
#define  CMMD__PICK_READY       "PICK_READY"
#define  CMMD__PICK_EXTEND      "PICK_EXTEND"
#define  CMMD__PICK_COMPLETE    "PICK_COMPLETE"

#define  CMMD__PLACE_READY      "PLACE_READY"
#define  CMMD__PLACE_EXTEND     "PLACE_EXTEND"
#define  CMMD__PLACE_COMPLETE   "PLACE_COMPLETE"

// ...
#define  CMMD__PUMP				"PUMP"
#define  CMMD__TRANSFER_PUMP	"TRANSFER_PUMP"

#define  STR__NONE				"NONE"
#define  STR__EXIST				"EXIST"
#define  STR__UNKNOWN			"UNKNOWN"

#define  ACT__RETRY				"RETRY"
#define  CHECK__RETRY			"CHECK_RETRY"

//.....
#define  ARM_A					"A"
#define  ARM_B					"B"

//.....
#define  STR__ENABLE			"ENABLE"
#define  STR__DISABLE			"DISABLE"

#define  STR__OPEN				"OPEN"
#define  STR__CLOSE				"CLOSE"

//.....
#define  STR__VAC				"VAC"

#define  STR__UP				"UP"
#define  STR__DOWN				"DOWN"

#define  STR__IDLE				"IDLE"
#define  STR__MAINT				"MAINT"

// ...
#define  STR__YES               "YES"
#define  STR__NO				"NO"

// ...
#define  STR__PASSIVE			"PASSIVE"
#define  STR__ACTIVE			"ACTIVE"

