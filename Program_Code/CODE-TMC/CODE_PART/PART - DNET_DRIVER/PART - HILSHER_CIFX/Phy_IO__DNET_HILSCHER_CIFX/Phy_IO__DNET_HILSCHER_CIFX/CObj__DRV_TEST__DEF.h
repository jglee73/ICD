#pragma once

#include "cifXUser.h"


// ...
#define DEVNET_FAL_CMD_GET_ATT_REQ                        0x0000380A
#define DEVNET_FAL_CMD_GET_ATT_CNF                        0x0000380B

#define DN_FAL_SDU_GETSET_ATT_MAX_DATA                    512


typedef struct CIFX_PACKET__GET_ATT_REQtag
{
	CIFX_PACKET_HEADER  tHeader;                 

	uint8_t  bDevMacId;                                   // MAC ID of the slave
	uint8_t  abReserved[3];                               // Reserved
	uint16_t usClass;                                     // Class ID
	uint16_t usInstance;                                  // Instance ID
	uint16_t usAttribute;                                 // Attribute ID
	uint16_t usReserved;                                  // Reserved

} CIFX_PACKET__GET_ATT_REQ;

typedef struct CIFX_PACKET__GET_ATT_CNFtag
{
	CIFX_PACKET_HEADER  tHeader;                 

	uint8_t  bDevMacId;                                   // MAC ID of the slave
	uint8_t  abReserved[3];                               // Reserved
	uint16_t usClass;                                     // Class ID
	uint16_t usInstance;                                  // Instance ID
	uint16_t usAttribute;                                 // Attribute ID
	uint8_t  bGenErr;                                     // General Error. @see CIP_General_Error_Code
	uint8_t  bAddErr;                                     // Additional error
	uint8_t  abAttData[DN_FAL_SDU_GETSET_ATT_MAX_DATA];   // Attribute data
} CIFX_PACKET__GET_ATT_CNF;


// ...
#define DEVNET_FAL_CMD_DEV_DIAG_REQ						 0x00003820
#define DEVNET_FAL_CMD_DEV_DIAG_CNF                      0x00003821

#define DN_FAL_MAX_DATA_SIZE                             1556


typedef struct CIFX_PACKET__DIAG_REQtag
{
	CIFX_PACKET_HEADER	tHeader;                 

	uint8_t  ubDevMacId;								  // MAC ID of the slave.Max 63 
	uint32_t fGetOnly;									  // 1 = do not reset corresponding bit in abDv_diag
	uint8_t  ubReserved;								  // Reserved
} CIFX_PACKET__DIAG_REQ;


typedef struct CIFX_DIAG_DATAttag
{
#define DN_FAL_NODE_DIAG_NO_RES            0x80
#define DN_FAL_NODE_DIAG_PRM_FAULT         0x20
#define DN_FAL_NODE_DIAG_CFG_FAULT         0x10
#define DN_FAL_NODE_DIAG_UCMM_SUPPORT      0x08
#define DN_FAL_NODE_DIAG_NOT_CONFIGURED    0x01

	uint8_t bNodeExtraDiag;                               // Extra diagnosis

	uint8_t  bDevMainState;                               // DV_ ... state
	uint8_t  bOnlineError;
	uint8_t  bGeneralErrorCode;							  // @see CIP_General_Error_Code ...
	uint8_t  bAdditionalCode;                             // Additional Error Code 
	uint16_t usHrtBeatTimeout;                            // Heartbeat timeout in milliseconds
} CIFX_DIAG_DATA;

typedef union CIFX_DIAG_DATALOADtag
{
	CIFX_DIAG_DATA tDiagData;                             // Diagnosis

	uint8_t abData[DN_FAL_MAX_DATA_SIZE-6];				  // Size of ubDevMacId+udDataLen+ubDiagType = 6
} CIFX_DIAG_DATALOAD;

typedef struct CIFX_PACKET__DIAG_CNFtag
{
	CIFX_PACKET_HEADER tHeader;

	uint8_t  ubDevMacId;								  // Device MAC ID. Max 64
	uint32_t ulDatLen;                                    // Data Length
	uint8_t  ubDiagType;                                  // Preserved. Do no used

	CIFX_DIAG_DATALOAD tData;
} CIFX_PACKET__DIAG_CNF;



// ...
#define CFG__LINK_IO_SIZE				10


// ...
#define STR__ON							"ON"
#define STR__OFF						"OFF"

#define STR__YES						"YES"
#define STR__NO	  					    "NO"

#define STR__ONLINE						"ONLINE"
#define STR__OFFLINE					"OFFLINE"

// ...
#define STR__PRODUCT_NAME				"PRODUCT.NAME"
#define STR__IN_BYTE					"IN.BYTE"
#define STR__OUT_BYTE					"OUT.BYTE"

// ...
#define STR__UNKNOWN					"UNKNOWN"
#define STR__OK							"OK"
#define STR__ERROR						"ERROR"
