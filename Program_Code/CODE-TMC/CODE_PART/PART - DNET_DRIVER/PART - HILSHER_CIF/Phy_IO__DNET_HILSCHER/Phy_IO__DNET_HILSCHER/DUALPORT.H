/* <St> *******************************************************************

  FILENAME      : DUALPORT.H

  -------------------------------------------------------------------------
  CREATETED     : R. Mayer,  Hilscher GmbH
  DATE          : 17.10.97
  =========================================================================

  DISCRIPTION

    Standard dual port memory structure for CIF, CIF104 and COM boards.
    Special definition for CIF40-IBSM, CIF104-IBSM and COM-IBSM

  =========================================================================

  CHANGES

  version name        date        Discription
  -------------------------------------------------------------------------

  V1.001  WG          29.05.01    - include guard added
  
	V1.000  MY          17.10.97    - created

  ******************************************************************** <En> */

#ifndef __DUALPORT_H
#define __DUALPORT_H

#if _MSC_VER >= 1000
#pragma once
#endif /* _MSC_VER >= 1000 */

#ifdef __cplusplus
  extern "C" {
#endif

#pragma pack(1)

/* ------------------------------------------------------------------------------------ */
/*   DPM flag definition                                                                */
/* ------------------------------------------------------------------------------------ */

/* Host flags (HOST_FLAGS) written by the device */
#define HOSTCOM_FLAG            0x01
#define DEVACK_FLAG             0x02
#define PDACK_FLAG              0x04
#define STATECOM_FLAG           0x08
#define TESTACK_FLAG            0x10
#define COM_FLAG                0x20
#define RUN_FLAG                0x40
#define READY_FLAG              0x80

#define READY_RUN_FLAG          0xC0  /* Check if RUN_FLAG and READY_FLAG are set */

/* Device flags (DEV_FLAGS) written by the host */
#define HOSTACK_FLAG            0x01
#define DEVCOM_FLAG             0x02
#define PDCOM_FLAG              0x04
#define STATEACK_FLAG           0x08
#define TESTCOM_FLAG            0x10
#define NOTREADY_FLAG           0x20
#define INIT_FLAG               0x40
#define RESET_FLAG              0x80

/* ------------------------------------------------------------------------------------ */
/*   Reset mode definition                                                              */
/* ------------------------------------------------------------------------------------ */

#ifndef _CIFUSER_H__
#define COLDSTART           0x80
#define WARMSTART           0x40
#define BOOTSTART           0xC0

/* ==================================================================================== */
/* STANDARD DPM structure definition for                                                */
/*                                                                                      */
/* CIF, CIF 104 and COM boards                                                          */
/*                                                                                      */
/*                                                                                      */
/*             CCCCC    II    FFFFFFF      II        CCCCC    OOOOOO     MM      MM     */
/*           CC         II    FF          II       CC       OO      OO   MMMM  MMMM     */
/*           CC         II    FFFF       II        CC       OO      OO   MM MMMM MM     */
/*           CC         II    FF        II         CC       OO      OO   MM  MM  MM     */
/*           CC         II    FF       II          CC       OO      OO   MM      MM     */
/*             CCCCC    II    FF      II             CCCCC    OOOOOO     MM      MM     */
/*                                                                                      */
/*                                                                                      */
/* ==================================================================================== */

/*----------------------------*/
/* Device version information */
/*----------------------------*/
typedef struct tagVERSIONINFO {           /* DEV serial number and OS versions          */
  unsigned long   ulDate;
  unsigned long   ulDeviceNo;
  unsigned long   ulSerialNo;
  unsigned long   ulReserved;
  unsigned char   abPcOsName0[4];
  unsigned char   abPcOsName1[4];
  unsigned char   abPcOsName2[4];
  unsigned char   abOemIdentifier[4];
} VERSIONINFO;

/*-----------------------------*/
/* Device firmware information */
/*-----------------------------*/
typedef struct tagFIRMWAREINFO {
  unsigned char   abFirmwareName[16];     /* Firmware name                              */
  unsigned char   abFirmwareVersion[16];  /* Firmware version                           */
} FIRMWAREINFO;

/*-------------------------------*/
/* Device task state information */
/*-------------------------------*/
typedef struct tagTASKSTATE {
  unsigned char   abTaskState[64];        /* Task state field                           */
} TASKSTATE;

/*----------------------------*/
/* Device task paramater data */
/*----------------------------*/
typedef struct tagTASKPARAM {
  unsigned char   abTaskParameter[64];    /* Task parameter field                       */
} TASKPARAM;

/*-------------------------*/
/* Device task information */
/*-------------------------*/
typedef struct tagTASKINFO {
  struct  {
    unsigned char  abTaskName[8];         /* Task name                                  */
    unsigned short usTaskVersion;         /* Task version                               */
    unsigned char  bTaskCondition;        /* Actual task condition                      */
    unsigned char  abreserved[5];         /* n.c.                                       */
  } tTaskInfo [7];
} TASKINFO;

/*-------------------------------------------*/
/* Device operating system (RCS) information */
/*-------------------------------------------*/
typedef struct tagRCSINFO {
  unsigned short usRcsVersion;            /* Device operating system (RCS) version      */
  unsigned char  bRcsError;               /* Operating system errors                    */
  unsigned char  bHostWatchDog;           /* Host watchdog value                        */
  unsigned char  bDevWatchDog;            /* Device watchdog value                      */
  unsigned char  bSegmentCount;           /* RCS segment free counter                   */
  unsigned char  bDeviceAdress;           /* RCS device base address                    */
  unsigned char  bDriverType;             /* RCS driver type                            */
} RCSINFO;

/*--------------------*/
/* Device description */
/*--------------------*/
typedef struct tagDEVINFO {
  unsigned char  bDpmSize;                /* Device dpm size (2,8...)                   */
  unsigned char  bDevType;                /* Device type  (manufactor code)             */
  unsigned char  bDevModel;               /* Device model (manufactor code)             */
  unsigned char  abDevIdentifier[3];      /* Device identification characters           */
} DEVINFO;
#endif //_CIFUSER_H__

/* ------------------------------------------------------------------------------------ */
/* STANDARD DPM layout                                                                  */
/* ------------------------------------------------------------------------------------ */
/* Standard definition for the last 1 Kbyte */
typedef struct tagDPM_MEMORY {

  unsigned char abDeviceMailbox[288];  /* Mailbox HOST --> DEVICE            (288 Bytes)*/

  VERSIONINFO   tDevVersion;           /* DEV verion information             ( 32 Bytes)*/

  unsigned char abHostMailbox[288];    /* Mailbox DEVICE --> HOST            (288 Bytes)*/

  FIRMWAREINFO  tFirmwareInfo;         /* Firmware info                      ( 32 Bytes)*/

  TASKPARAM     tTask1Parameter;       /* Task 1 parameter                   ( 64 Bytes)*/

  TASKPARAM     tTask2Parameter;       /* Task 2 parameter                   ( 64 Bytes)*/

  TASKSTATE     tTask1State;           /* Task 1 state                       ( 64 Bytes)*/

  TASKSTATE     tTask2State;           /* Task 2 state                       ( 64 Bytes)*/

  TASKINFO      tTaskInfo;             /* Task 1 to 7 info field             (112 Bytes)*/

  RCSINFO       tRcsInfo;              /* Operating system information       (  8 Bytes)*/

  DEVINFO       tDeviceInfo;           /* DEVICE information                 (  6 Bytes)*/

  volatile unsigned char bHostFlags;   /* DPM communication DEVICE->HOST     (  1 Byte )*/
  volatile unsigned char bDevFlags;    /* DPM communication HOST->DEVICE     (  1 Byte )*/

} DPM_MEMORY;


/*------------------------------*/
/* Definition for a 2 Kbyte DPM */
/*------------------------------*/
typedef struct tagDPM_MEMORY_2KB {

  unsigned char abSendProcessData[512];         /* Send process data image              */
  unsigned char abReceiveProcessData[512];      /* Receive process data image           */
  DPM_MEMORY    tDpm;                           /* Definition of the standard DPM       */

} DPM_MEMORY_2KB;


/*------------------------------*/
/* Definition for a 8 Kbyte DPM */
/*------------------------------*/
typedef struct tagDPM_MEMORY_8KB {

  unsigned char abSendProcessData[3584];        /* Send process data image              */
  unsigned char abReceiveProcessData[3584];     /* Receive process data image           */
  DPM_MEMORY    tDpm;                           /* Definition of the standard DPM       */

} DPM_MEMORY_8KB;


/*------------------------------*/
/* Definition for a 16 Kbyte DPM */
/*------------------------------*/
typedef struct tagDPM_MEMORY_16KB {

  unsigned char abSendProcessData[7680];        /* Send process data image              */
  unsigned char abReceiveProcessData[7680];     /* Receive process data image           */
  DPM_MEMORY    tDpm;                           /* Definition of the standard DPM       */

} DPM_MEMORY_16KB;


#define DPM_SIZE_1K   1
#define DPM_SIZE_2K   2
#define DPM_SIZE_8K   8
#define DPM_SIZE_16K 16


/* ==================================================================================== */
/* SPECIAL DPM structure definition for                                                 */
/*                                                                                      */
/* CIF40-IBSM, CIF 104-IBSM and COM-IBSM                                                */
/*                                                                                      */
/*                                                                                      */
/*             CCCCC    OOOOOO     MM      MM                                           */
/*           CC       OO      OO   MMMM  MMMM                                           */
/*           CC       OO      OO   MM MMMM MM                                           */
/*           CC       OO      OO   MM  MM  MM                                           */
/*           CC       OO      OO   MM      MM                                           */
/*             CCCCC    OOOOOO     MM      MM                                           */
/*                                                                                      */
/*                                                                                      */
/* ==================================================================================== */

/* ------------------------------------------------------------------------------------ */
/* SPECIAL DPM layout                                                                   */
/* ------------------------------------------------------------------------------------ */
/* Definition for the last 1 Kbyte */
typedef struct tagDPM_CIF40_COM_MEMORY {

  unsigned char abDeviceMailbox[288];     /* Mailbox HOST --> DEVICE         (288 Bytes)*/

  unsigned char reserved[32];             /* Reserved                        ( 32 Bytes)*/

  unsigned char abHostMailbox[288];       /* Mailbox DEVICE --> HOST         (288 Bytes)*/

  FIRMWAREINFO  tFirmwareInfo;            /* Firmware info                   ( 32 Bytes)*/

  unsigned char abTaskParameter[16];      /* Task parameter                  ( 16 Bytes)*/

  unsigned char abConfigurationList[336]; /* Configuration list              (336 Bytes)*/

  unsigned char abInfoField[16];          /* Information field               ( 16 Bytes)*/

  RCSINFO       tRcsComInfo;              /* Operating system information    (  8 Bytes)*/

  DEVINFO       tDeviceInfo;              /* DEVICE information              (  6 Bytes)*/

  volatile unsigned char bHostFlags;   /* DPM communication DEVICE->HOST     (  1 Byte )*/
  volatile unsigned char bDevFlags;    /* DPM communication HOST->DEVICE     (  1 Byte )*/

} DPM_CIF40_COM_MEMORY;


/*------------------------------*/
/* Definition for a 2 Kbyte DPM */
/*------------------------------*/
typedef struct tagDPM_CIF40_COM_MEMORY_2KB {

  unsigned char        abSendProcessData[512];    /* Send process data image            */
  unsigned char        abReceiveProcessData[512]; /* Receive process data image         */
  DPM_CIF40_COM_MEMORY tDpm;                      /* Definition of the standard DPM     */

} DPM_CIF40_COM_MEMORY_2KB;

#pragma pack()

#ifdef __cplusplus
  }
#endif

#endif