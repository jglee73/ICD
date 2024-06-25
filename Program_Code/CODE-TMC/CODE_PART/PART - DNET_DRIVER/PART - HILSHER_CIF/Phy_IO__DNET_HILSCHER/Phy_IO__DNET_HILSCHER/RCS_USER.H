/* <St> *******************************************************************

  FILENAME    : RCS_USER.H

  -------------------------------------------------------------------------
  CREATED BY  : R.Mayer,  Hilscher GmbH
  CREATED AT  : 10.05.96
  PROJECT     : global
  =========================================================================

  FUNCTION :
    General RCS definitions

  =========================================================================

  CHANGES OF REVISIONS :

  Version Name      Date      Changes
  -------------------------------------------------------------------------
  V1.103  hp        26.03.04  - Missed Mode MODE_SET_DEVICE_DATA added again.

  V1.102  Mayer     22.03.04  - Review with internal definitions from RC100700.H
                              - Comments translated to english

  V1.101  hp        27.01.04  Added stuff which was lost by the review on 
                              26.01.2004
                              (V1.009  Platz     06.01.04  New modes for B_SYSFKT defined
                              Everything around RCS_B_IO_WATCH inserted)
  V1.100  Mayer     26.01.04  Review added from other versions
                              - (V1.007 HP Added RCS_TID... RCS_ERR)
                              - (V1.006 HP Added General RCS Errors

  V1.006  Mayer     24.09.02  Review
                              GNU-C compiler support included
                              RCS_DEF.H now included

  V1.005  Wagner    29.05.01  include guard added/__cplusplus (double 
	                            underscore) corrected
  
  V1.004  Mayer     20.09.99  RCS message command definitions included

  V1.003  Mayer     10.03.99  fault in #ifdef __cplusplus fixed

  V1.002  Mayer     14.05.98  RCS_TELEGRAMHEADERDATA_10 structure included

  V1.001  Mayer     29.05.96  Task errors included

  V1.000  Mayer     10.05.96  created from the file rc090203.h

  ******************************************************************** <En> */
/* prevent multiple inclusion */
#ifndef __RCS_USER_H
#define __RCS_USER_H

#if _MSC_VER >= 1000
#pragma once
#endif /* _MSC_VER >= 1000 */

/* support for GNU compiler */
#ifdef __GNUC__
  #define GNUC_PACKED  __attribute__((__packed__))
#else
  #define GNUC_PACKED
#endif

#ifdef __cplusplus
  extern "C" {
#endif  /* __cplusplus */

#if defined( _MSC_VER)          /* Microsoft C */
  #pragma pack(1)               /* Byte Alignment   */
#endif

/* ======================================================================== */
/* General task commands                                                    */
/* ======================================================================== */

#define TASK_B_INITIALIZE                   1
#define TASK_B_10                           16     /* RCS_TELEGRAMHEADER_10 */
#define TASK_B_11                           17     /* Task specific         */
                                                   /* do not use (old):     */
#define TASK_B_12                           18     /* RCS_TELEGRAMHEADER    */


/* ======================================================================== */
/* General task errors                                                      */
/* ======================================================================== */

#define TASK_F_OK                              0
#define TASK_F_NO_COMMUNICATION                1
#define TASK_F_IDLE                            2

#define TASK_F_INIT_BASE                       50

#define TASK_F_PARITY                          100
#define TASK_F_FRAMING                         101
#define TASK_F_OVERRUN                         102
#define TASK_F_DATACOUNT                       103
#define TASK_F_CHECKSUM                        104
#define TASK_F_TIMEOUT                         105
#define TASK_F_PROTOCOL                        106
#define TASK_F_DATA                            107
#define TASK_F_NACK                            108

#define TASK_F_PROTOCOL_BASE                   110

#define TASK_F_MESSAGEHEADER                   150
#define TASK_F_MESSAGESIZE                     151
#define TASK_F_MESSAGECOMMAND                  152
#define TASK_F_MESSAGESTRUCTURE                153
#define TASK_F_MESSAGEERROR                    154
#define TASK_F_MESSAGETIMEOUT                  155
#define TASK_F_MESSAGESEQUENCE                 156
#define TASK_F_MESSAGENUMBER                   157
#define TASK_F_MESSAGECOMMANDRUNNING           158


#define TASK_F_TELEGRAMHEADER                  160
#define TASK_F_DEVICE_ADR                      161
#define TASK_F_DATA_AREA                       162
#define TASK_F_DATA_ADR                        163
#define TASK_F_DATA_IDX                        164
#define TASK_F_DATA_CNT                        165
#define TASK_F_DATA_TYPE                       166
#define TASK_F_FUNCTION                        167

#define TASK_F_MESSAGE_BASE                    170

#define TASK_F_NOT_INITIALIZED                 200
#define TASK_F_BUSY                            201
#define TASK_F_SEGMENT                         202
#define TASK_F_USER                            203

#define TASK_F_DATABASE                        210
#define TASK_F_DATABASE_WRITE                  211
#define TASK_F_DATABASE_READ                   212
#define TASK_F_STRUCTURE                       213
#define TASK_F_PARAMETER                       214
#define TASK_F_CONFIGURATION                   215
#define TASK_F_FUNCTIONLIST                    216
#define TASK_F_SYSTEM                          217
#define TASK_F_MEM_ALLOC                       218
#define TASK_F_NO_DUALPORT_MEM_POINTER         219


#define TASK_F_SYSTEM_BASE                     220


/* ======================================================================== */
/* General RCS command                                                      */
/* ======================================================================== */

/* ----------------------------------------------- */
/* RCS message commands                            */
/* ----------------------------------------------- */

  #define RCS_B_MAX                   11
  #define RCS_B_SYSFKT                1     /* System function                          */
  #define RCS_B_TASKFKT               2     /* Task function                            */
  #define RCS_B_DIAGNOSE              3     /* Diagnostic function                      */
  #define RCS_B_STRUCTFNC             4     /* Task structure function                  */
  #define RCS_B_TRACE                 5     /* Trace function                           */
  #define RCS_B_LOADFKT               6     /* Upload and download function             */
  #define RCS_B_DBM                   7     /* Database function                        */
  #define RCS_B_IO_WATCH              8     /* Input and output watch function          */
  #define RCS_B_SELFTEST              9     /* Selftest function                        */
  #define RCS_B_DEBUG                 10    /* Special debug function                   */
  #define RCS_B_UPLOAD_MULTIPLEX      11    /* Multiplex upload and download function   */





/* ----------------------------------------------- */
/* RCS message modes                               */
/* ----------------------------------------------- */

  /* B_SYSFKT */
  #define MODE_KALTSTART              1     /* RCS_B_SYSFKT force a cold start */                          
  #define MODE_WARMSTART              2     /* RCS_B_SYSFKT force a warm start */                          
  #define MODE_ZYKL_STATUS_STOP       3     /* RCS_B_SYSFKT start/stop cyclic task state */                
  #define MODE_FWVERSION              4     /* RCS_B_SYSFKT get the firmware, version and checksum */      
  #define MODE_GET_PROJ_WERTE_HW      5     /* RCS_B_SYSFKT get the hardware configuration */              
  #define MODE_GET_PROJ_WERTE_SW      6     /* RCS_B_SYSFKT get the software configuration */              
  #define MODE_SHOW_DYN_SYSSTAT       7     /* RCS_B_SYSFKT dynamic System state */                        
  #define MODE_RUN_RDY_COM            8     /* RCS_B_SYSFKT dynamic System state only once */              
  #define MODE_GET_RT_CLOCK           9     /* RCS_B_SYSFKT get real time clock NOT IMPLEMENTED */         
  #define MODE_SET_RT_CLOCK           10    /* RCS_B_SYSFKT set real time clock NOT IMPLEMENTED */         
  #define MODE_MODUL_RCS              11    /* RCS_B_SYSFKT get configured RCS module*/                    
  #define MODE_MODUL_LIB              12    /* RCS_B_SYSFKT get configured LIB module*/                    
  #define MODE_MODUL_MCL              13    /* RCS_B_SYSFKT get configured MCL module*/                    
  #define MODE_DISTRIBUTOR_DRIVER     14    /* RCS_B_SYSFKT ->function 1 == insert, else get information */
  #define MODE_PRINT_DRIVER           15    /* RCS_B_SYSFKT get driver information*/                       
  #define MODE_GET_RCS_ERROR          16    /* RCS_B_SYSFKT read the RCS Error (if RCS can answer)*/       
  #define MODE_CHANGE_MCL_STATE       17    /* RCS_B_SYSFKT */                                             
  #define MODE_GET_PROJECT_INFO       18    /* RCS_B_SYSFKT */                                             
  #define MODE_CHANGE_BAUDRATE        19    /* RCS_B_SYSFKT implemented in RCS_COM.P86 V1.???*/            
  #define MODE_RW_TASK_PARAM_STATUS   20    /* RCS_B_SYSFKT implemented in RCS_COM.P86 V1.???*/            
  #define MODE_RW_IOEXCHANGE          21    /* RCS_B_SYSFKT implemented in RCS_COM.P86 V1.???*/            
  #define MODE_IO_TRIGGER             22    /* RCS_B_SYSFKT implemented in RCS_COM.P86 V1.???*/            
  #define MODE_WD_TRIGGER             23    /* RCS_B_SYSFKT implemented in RCS_COM.P86 V1.???*/            

  /* B_TASKFKT */
  #define MODE_START_STOP_STAT        0     /* RCS_B_TASKFKT */
  #define MODE_START_STOP             1     /* RCS_B_TASKFKT */
  #define MODE_TASK_VERSION           2     /* RCS_B_TASKFKT */
  #define MODE_ZYKL_STATUS            3     /* RCS_B_TASKFKT */
  #define MODE_SHOW_FUELLST           4     /* RCS_B_TASKFKT */
  #define MODE_SHOW_TIMER             5     /* RCS_B_TASKFKT */

  /* B_DIAGNOSE */
  #define MODE_DIAG_MEM_READ_SINGLE   0x0   /* RCS_B_DIAGNOSE */
  #define MODE_DIAG_MEM_WRITE_SINGLE  0x1   /* RCS_B_DIAGNOSE */
  #define MODE_DIAG_IO_READ_SINGLE    0x2   /* RCS_B_DIAGNOSE */
  #define MODE_DIAG_IO_WRITE_SINGLE   0x3   /* RCS_B_DIAGNOSE */
  #define MODE_DIAG_ZYKL              0x10  /* RCS_B_DIAGNOSE */
  #define MODE_DIAG_MEM_READ_ZYKL     0x10  /* RCS_B_DIAGNOSE */
  #define MODE_DIAG_MEM_WRITE_ZYKL    0x11  /* RCS_B_DIAGNOSE */
  #define MODE_DIAG_IO_READ_ZYKL      0x12  /* RCS_B_DIAGNOSE */
  #define MODE_DIAG_IO_WRITE_ZYKL     0x13  /* RCS_B_DIAGNOSE */

  /* B_STRUCTFNC */
  #define MODE_ZYKL_TASK_STRUK        0     /* RCS_B_STRUCTFNC */
  #define MODE_INIT_INFO_STRUK        1     /* RCS_B_STRUCTFNC */
  #define MODE_WRITE_STRUK            2     /* RCS_B_STRUCTFNC */
  #define MODE_START_TASK_STRUC_ONCE  3     /* RCS_B_STRUCTFNC */

  /* B_TRACE */
  #define MODE_START_TRACE            0     /* RCS_B_TRACE */
  #define MODE_DEL_TRACE_PUF          1     /* RCS_B_TRACE */

  /* B_LOADFKT */
  #define MODE_UPLOAD_BINAER          0     /* RCS_B_LOADFKT */
  #define MODE_DOWNLOAD_BINAER        1     /* RCS_B_LOADFKT not implemented */
  #define MODE_UPLOAD_DBM             2     /* RCS_B_LOADFKT */
  #define MODE_DOWNLOAD_DBM           3     /* RCS_B_LOADFKT not implemented */
  #define MODE_DEL_FLASH              4     /* RCS_B_LOADFKT */
  #define MODE_GET_FLASH_DIR          5     /* RCS_B_LOADFKT */
  #define MODE_URLADEN                6     /* RCS_B_LOADFKT firmware download implemented in BSL */
  #define MODE_LONG_BINLOAD           7     /* RCS_B_LOADFKT */
  #define MODE_FREE_DRIVER            8     /* RCS_B_LOADFKT */
  #define MODE_GET_SEG_DESCRIPTION    9     /* !!!!  RCS_B_LOADFKT !!!!! */
  #define MODE_SET_DEVICE_DATA        9     /* !!!!! Functionality will switch depending on the Message length !!!! */
  #define MODE_DELETE_FLASH_ONL       10    /* RCS_B_LOADFKT */
  #define MODE_WRITE_FLASH_ONL        11    /* RCS_B_LOADFKT */
  #define MODE_CLR_OEM_CODE           12    /* RCS_B_LOADFKT, serial flash only */

  /* B_IO_WATCH */
  #define MAX_WATCH                   3     /* RCS_B_IO_WATCH */
  #define MODE_IO_WATCH_INIT          0     /* RCS_B_IO_WATCH */
  #define MODE_IO_WATCH_READ          1     /* RCS_B_IO_WATCH */
  #define MODE_IO_WATCH_WRITE         2     /* RCS_B_IO_WATCH */

  /* B_SELFTEST */
  #define MODE_TEST                   0     /* RCS_B_SELFTEST */

  /* Special definitions */
  #define MODE_PRINT_DEVICE           1     /* NO mode, only for MENU.H */
  #define MODE_PRINT_DEVICE_DRIVER    2     /* NO mode, only for MENU.H */

/************************************************************************** */
/* General RCS Errors in answer message MSG.F                                              */
/************************************************************************** */

#define RCS_F_OK                               0
#define RCS_F_TASK_MISSING                     4
#define RCS_F_NOT_INITIALIZED                  5
#define RCS_F_MCL_DISABLE                      6
#define RCS_F_MCL_ERROR                        7

#define RCS_F_DOWNLOAD_NAME                    20
#define RCS_F_DOWNLOAD_INDEX                   21
#define RCS_F_DOWNLOAD_NR                      22
#define RCS_F_DOWNLOAD_CNT                     23
#define RCS_F_DOWNLOAD_SEQ                     24
#define RCS_F_DOWNLOAD_CHECKSUM                25
#define RCS_F_DOWNLOAD_GUARDED                 26
#define RCS_F_DOWNLOAD_TYPE                    27
#define RCS_F_UPLOAD_GUARDED                   26
#define RCS_F_UPLOAD_STATE                     28
#define RCS_F_UPLOAD_TYPE                      27
#define RCS_F_ERASE_TYPE                       27
#define RCS_F_BSL_MISSING                      29
#define RCS_F_DBM_MISSING                      39

#define RCS_F_TRACE_OVERFLOW                   30
#define RCS_F_TRACE_LEN                        31

#define RCS_F_COMMAND_FULL                     40
#define RCS_F_COMMAND_MISSING                  41
#define RCS_F_COMMAND_MODE                     42
#define RCS_F_COMMAND_PARAMETER                43
#define RCS_F_COMMAND_MESSAGE_LEN              44
#define RCS_F_COMMAND_TX_NOT_MCL               45

#define RCS_F_FLASH_BUSY                       50
#define RCS_F_FLASH_ERASE                      51
#define RCS_F_FLASH_WRITE                      52
#define RCS_F_FLASH_MISSING                    53
#define RCS_F_FLASH_TIMEOUT                    54
#define RCS_F_FLASH_GUARDED                    55
#define RCS_F_FLASH_SIZE                       56

#define RCS_F_STRUCTURE_TYPE                   60
#define RCS_F_STRUCTURE_SIZE                   61
#define RCS_F_STRUCTURE_MISSING                62

#define RCS_F_REALTIMECLOCK                    70

#define RCS_F_DBM_INVALID_HANDLE               80
#define RCS_F_DBM_INVALID_DATA_LENGTH          81
#define RCS_F_DBM_INVALID_SETNO                82
#define RCS_F_DBM_INVALID_NAME                 83
#define RCS_F_DBM_TABLE_FULL                   84
#define RCS_F_DBM_OTHER                        85

#define RCS_F_DEVDAT_PRESENT                   90
#define RCS_F_DEVDAT_CODE                      91
#define RCS_F_DEVDAT_DRIVER_DOUBLE             92
#define RCS_F_DEVDAT_DRIVER_FULL               93

/************************************************************************** */
/* System error RCS - stored in RCS_ERROR                                   */
/************************************************************************** */

#define RCS_ERR_PRIORITY_MAX                   1
#define RCS_ERR_PRIORITY_NULL                  2
#define RCS_ERR_PRIORITY_DOUBLE                3
#define RCS_ERR_STACKSIZE                      4
#define RCS_ERR_EPROMSIZE                      5
#define RCS_ERR_RAMSIZE                        6
#define RCS_ERR_SEGMENTCOUNT                   7
#define RCS_ERR_SEGMENTSIZE                    8
#define RCS_ERR_CYCLETIME                      9
#define RCS_ERR_FREQUENCY                      10
#define RCS_ERR_TRACEBUFFERSIZE                11
#define RCS_ERR_RAM_MIN                        12
#define RCS_ERR_DEVICEADDRESS                  13
#define RCS_ERR_MCL_TOKEN                      14
#define RCS_ERR_DRIVERTYPE                     15
#define RCS_ERR_SCC                            16
#define RCS_ERR_FLASHTYPE_OPT                  17
#define RCS_ERR_FLASHTYPE_BSL                  18
#define RCS_ERR_FLASHDIR_NAME                  19
#define RCS_ERR_FUNCTION_TAB                   20
#define RCS_ERR_RAMTYPE                        21
#define RCS_ERR_FLASHDIR_TYPE                  22

#define RCS_ERR_RAMTEST                        50
#define RCS_ERR_DATASEGMENT                    51
#define RCS_ERR_RAM                            52
#define RCS_ERR_EPROM                          53
#define RCS_ERR_DONGLE                         54
#define RCS_ERR_WRONG_RCSIDENTIFIER            55
#define RCS_ERR_MEM_ALLOC                      56



#define RCS_ERR_RCSTASK_NOT_READY              100
#define RCS_ERR_TASK_1_NOT_READY               101
#define RCS_ERR_TASK_2_NOT_READY               102
#define RCS_ERR_TASK_3_NOT_READY               103
#define RCS_ERR_TASK_4_NOT_READY               104
#define RCS_ERR_TASK_5_NOT_READY               105
#define RCS_ERR_TASK_6_NOT_READY               106
#define RCS_ERR_TASK_7_NOT_READY               107
#define RCS_ERR_TASK_8_NOT_READY               108
#define RCS_ERR_TASK_9_NOT_READY               109
#define RCS_ERR_TASK_10_NOT_READY              110
#define RCS_ERR_TASK_11_NOT_READY              111
#define RCS_ERR_TASK_12_NOT_READY              112
#define RCS_ERR_TASK_13_NOT_READY              113
#define RCS_ERR_TASK_14_NOT_READY              114
#define RCS_ERR_TASK_15_NOT_READY              115

#define RCS_ERR_MCL_0_MISSING                  120
#define RCS_ERR_MCL_1_MISSING                  121
#define RCS_ERR_MCL_2_MISSING                  122
#define RCS_ERR_MCL_DOUBLE                     128
#define RCS_ERR_MCL_STARTADDRESS               129
#define RCS_ERR_MCL_0_ERROR                    130
#define RCS_ERR_MCL_1_ERROR                    131
#define RCS_ERR_MCL_2_ERROR                    132
#define RCS_ERR_MCL_MODE                       138

#define RCS_ERR_RCS_0_MISSING                  140
#define RCS_ERR_RCS_1_MISSING                  141
#define RCS_ERR_RCS_2_MISSING                  142
#define RCS_ERR_RCS_3_MISSING                  143
#define RCS_ERR_RCS_4_MISSING                  144
#define RCS_ERR_RCS_5_MISSING                  145
#define RCS_ERR_RCS_6_MISSING                  146
#define RCS_ERR_RCS_7_MISSING                  147
#define RCS_ERR_RCS_DOUBLE                     148
#define RCS_ERR_RCS_STARTADDRESS               149
#define RCS_ERR_RCS_0_ERROR                    150
#define RCS_ERR_RCS_1_ERROR                    151
#define RCS_ERR_RCS_2_ERROR                    152
#define RCS_ERR_RCS_3_ERROR                    153
#define RCS_ERR_RCS_4_ERROR                    154
#define RCS_ERR_RCS_5_ERROR                    155
#define RCS_ERR_RCS_6_ERROR                    156
#define RCS_ERR_RCS_7_ERROR                    157

#define RCS_ERR_LIB_0_MISSING                  160
#define RCS_ERR_LIB_1_MISSING                  161
#define RCS_ERR_LIB_2_MISSING                  162
#define RCS_ERR_LIB_3_MISSING                  163
#define RCS_ERR_LIB_4_MISSING                  164
#define RCS_ERR_LIB_5_MISSING                  165
#define RCS_ERR_LIB_6_MISSING                  166
#define RCS_ERR_LIB_7_MISSING                  167
#define RCS_ERR_LIB_DOUBLE                     168
#define RCS_ERR_LIB_STARTADDRESS               169
#define RCS_ERR_LIB_0_ERROR                    170
#define RCS_ERR_LIB_1_ERROR                    171
#define RCS_ERR_LIB_2_ERROR                    172
#define RCS_ERR_LIB_3_ERROR                    173
#define RCS_ERR_LIB_4_ERROR                    174
#define RCS_ERR_LIB_5_ERROR                    175
#define RCS_ERR_LIB_6_ERROR                    176
#define RCS_ERR_LIB_7_ERROR                    177

#define RCS_ERR_UNKNOWN_IRQ                    200
#define RCS_ERR_WATCHDOG                       201
#define RCS_ERR_SCC_TX_IRQ                     202
#define RCS_ERR_SCC_RX_IRQ                     203
#define RCS_ERR_TASKSTATE                      204

#define RCS_ERR_TASK_0                         230
#define RCS_ERR_TASK_1                         231
#define RCS_ERR_TASK_2                         232
#define RCS_ERR_TASK_3                         233
#define RCS_ERR_TASK_4                         234
#define RCS_ERR_TASK_5                         235
#define RCS_ERR_TASK_6                         236
#define RCS_ERR_TASK_7                         237

#define RCS_ERR_DBG_TASK_0_SEGMENT             240
#define RCS_ERR_DBG_TASK_1_SEGMENT             241
#define RCS_ERR_DBG_TASK_2_SEGMENT             242
#define RCS_ERR_DBG_TASK_3_SEGMENT             243
#define RCS_ERR_DBG_TASK_4_SEGMENT             244
#define RCS_ERR_DBG_TASK_5_SEGMENT             245
#define RCS_ERR_DBG_TASK_6_SEGMENT             246
#define RCS_ERR_DBG_TASK_7_SEGMENT             247


/************************************************************************** */
/* Miscellaneous definitions for RCS system functions                       */
/************************************************************************** */

/* Identifier for traces entries - TraceIDendifier */ 

#define RCS_TID_NONE                           0
#define RCS_TID_TELEGRAMHEADER                 1
#define RCS_TID_TELEGRAMDATA                   2
#define RCS_TID_TELEGRAM                       3
#define RCS_TID_MESSAGEHEADER                  4
#define RCS_TID_MESSAGEDATA                    5
#define RCS_TID_MESSAGE                        6
#define RCS_TID_DATA                           7


/* ======================================================================== */
/* Message definitions                                                      */
/* ======================================================================== */

/* ----------------- Definition of the standard telegram header ----------- */
/* Keyword: MESSAGE    ---------------------------------------------------- */
/* TelegramFunction */

#define TASK_TFC_UNUSED                        0
#define TASK_TFC_READ                          1
#define TASK_TFC_WRITE                         2
#define TASK_TFC_QUERRY                        3

/* TelegramDataArea */
/* 'data_area'      */

#define TASK_TDA_UNUSED                        0
#define TASK_TDA_BIT                           1
#define TASK_TDA_BYTE                          2
#define TASK_TDA_WORD                          3
#define TASK_TDA_DWORD                         4
#define TASK_TDA_FLOAT                         5

/* TelegramDataType */
/* 'data_type' for MOTOROLA data types !                                    */
/* For INTEL data types, the 'RCS_TDT_IDF_MASK' flag must be set            */

#define TASK_TDT_UNUSED                        0
#define TASK_TDT_BOOLEAN                       1
#define TASK_TDT_INT8                          2
#define TASK_TDT_INT16                         3
#define TASK_TDT_INT32                         4
#define TASK_TDT_UINT8                         5
#define TASK_TDT_UINT16                        6
#define TASK_TDT_UINT32                        7
#define TASK_TDT_FLOAT                         8
#define TASK_TDT_ASCII                         9
#define TASK_TDT_STRING                        10
#define TASK_TDT_BIT                           14

#define TASK_TDT_IDF_MSK                     0X80


/* ------------------------------------------------------------------------ */
/* --- Evaluation Masks from protocol configuration file RCS_CNF.P86    --- */
/* ------------------------------------------------------------------------ */

/* Reference: Structur RCS_TASK_DIR */

#define TASK_MTS_IDF_MSK                    0X01   /* 0: User data in Motorola format (MSB/LSB) */
                                                   /* 1: User data in Intel format    (LSB/MSB) */
#define TASK_MTS_STH_MSK                    0X02   /* 0: Protocol specific telegram header      */
                                                   /* 1: Standard telegram header               */

#define TASK_MTS_B10_MSK                    0x02   /* 1: Standard telegram header               */
#define TASK_MTS_B11_MSK                    0x04   /* 1: Protocol telegram header               */

#define TASK_MTS_PD4_MSK                    0x10
#define TASK_MTS_PD5_MSK                    0x20
#define TASK_MTS_PD6_MSK                    0x40
#define TASK_MTS_PD7_MSK                    0x80
  

/* ======================================================================== */
/* Message structure definitions                                            */
/* ======================================================================== */

/* ------------------------------------------------------------------------ */
/* -------------------- General  RCS definitions -------------------------- */
/* Keyword: MESSAGE  ------------------------------------------------------ */

#define RCS_TOKEN                             0
#define RCS_SEGMENT_LEN                       288
#define RCS_MESSAGEHEADER_LEN                 8
#define RCS_TELEGRAMHEADER_LEN                8
#define RCS_FOREVER                           0
#define RCS_NEWSTART                          0
#define RCS_COLDSTART                         1
#define RCS_WARMSTART                         2
#define RCS_SYSTEMSTART                       3
#define RCS_BOOTSTART                         4
#define RCS_DBMSTART                          5     /* only RCS internal */
#define RCS_CHECKSTART                        6     /* Urstart with firwarere start*/

#define RCS_TASK                              0x00  /* Number of RCS	*/
#define PLC_TASK                              0x02  /* Number for PLC_TASK	*/

#define MSG_SYSTEM_TX                         0xFF  /* Transmitter for system functions number	*/

#define RCS_B_TASK_STATE                      4     /* Task state */

#define FIRMWARE_DOWNLOAD                     1
#define CONFIGURATION_DOWNLOAD                2
  

/* Evaluation masks for the message extension byte */
/* Keyword: RCS_E_ msg.b -------------------------------------------------- */

#define RCS_FIFO_MSK                          0
#define RCS_LIFO_MSK                          1
#define RCS_NAK_MSK                           2

#define RCS_NORM_MSK                          0
#define RCS_FIRST_MSK                         4
#define RCS_CONT_MSK                          8
#define RCS_LAST_MSK                        0x0C
#define RCS_SEQ_MSK                         0x0C

#define RCS_NO_TASK                           0
#define RCS_TASKINIT_PENDING                  1
#define RCS_TASKINIT_RUNNING                  2
#define RCS_TASKINIT_ERROR                    3
#define RCS_TASKINIT_END                      4


#define RCS_CLR_ERROR_LED                     0
#define RCS_SET_ERROR_LED                     1

                                    
#define RCS_ERROR_LED_MODE_ON_OFF             0
#define RCS_ERROR_LED_MODE_ON                 1

/* ======================================================================== */
/* Globale Type Definition                                                  */
/* ======================================================================== */
typedef struct RCS_TASK_ERRORtag {
  unsigned char bTask;
  unsigned char bError;
} GNUC_PACKED RCS_TASK_ERROR;


/* ------------------------ RCS message definition ------------------------ */

typedef struct RCS_MESSAGEHEADERtag {
   unsigned char        rx;             /* receiver                         */
   unsigned char        tx;             /* transmitter                      */
   unsigned char        ln;             /* lenght                           */
   unsigned char        nr;             /* number                           */
   unsigned char        a;              /* answer                           */
   unsigned char        f;              /* fault                            */
   unsigned char        b;              /* command                          */
   unsigned char        e;              /* extension                        */
} GNUC_PACKED RCS_MESSAGEHEADER;

typedef struct RCS_MESSAGEtag {
   unsigned char        rx;             /* receiver                         */
   unsigned char        tx;             /* transmitter                      */
   unsigned char        ln;             /* lenght                           */
   unsigned char        nr;             /* number                           */
   unsigned char        a;              /* answer                           */
   unsigned char        f;              /* fault                            */
   unsigned char        b;              /* command                          */
   unsigned char        e;              /* extension                        */
   unsigned char        d[ RCS_SEGMENT_LEN-RCS_MESSAGEHEADER_LEN ]; /* data */
} GNUC_PACKED RCS_MESSAGE;


/* ----------------- Standard telegram header ----------------------------- */
/* Keyword: MESSAGE, TASK_B_10  --------------------------------------------*/

typedef struct RCS_TELEGRAMHEADER_10tag {
   unsigned char        device_adr;     /* device address                   */
   unsigned char        data_area;      /* data area                        */
   unsigned short       data_adr;       /* data address                     */
   unsigned char        data_idx;       /* data index                       */
   unsigned char        data_cnt;       /* data count                       */
   unsigned char        data_type;      /* data type                        */
   unsigned char        function;       /* function                         */
} GNUC_PACKED RCS_TELEGRAMHEADER_10;

typedef struct RCS_TELEGRAMHEADERDATA_10tag {
   unsigned char        device_adr;
   unsigned char        data_area;
   unsigned short       data_adr;
   unsigned char        data_idx;
   unsigned char        data_cnt;
   unsigned char        data_type;
   unsigned char        function;
   unsigned char        d[ RCS_SEGMENT_LEN-RCS_MESSAGEHEADER_LEN-RCS_TELEGRAMHEADER_LEN];
} GNUC_PACKED RCS_TELEGRAMHEADERDATA_10;

typedef struct RCS_MESSAGETELEGRAMHEADER_10_tag {
   unsigned char        rx;             /* receiver                         */
   unsigned char        tx;             /* transmitter                      */
   unsigned char        ln;             /* lenght                           */
   unsigned char        nr;             /* number                           */
   unsigned char        a;              /* answer                           */
   unsigned char        f;              /* fault                            */
   unsigned char        b;              /* command                          */
   unsigned char        e;              /* extension                        */
   unsigned char        device_adr;     /* device address                   */
   unsigned char        data_area;      /* data area                        */
   unsigned short       data_adr;       /* data address                     */
   unsigned char        data_idx;       /* data index                       */
   unsigned char        data_cnt;       /* data count                       */
   unsigned char        data_type;      /* data type                        */
   unsigned char        function;       /* function                         */
} GNUC_PACKED RCS_MESSAGETELEGRAMHEADER_10;

typedef struct RCS_MESSAGETELEGRAM_10tag {
   unsigned char        rx;             /* receiver                         */
   unsigned char        tx;             /* transmitter                      */
   unsigned char        ln;             /* lenght                           */
   unsigned char        nr;             /* number                           */
   unsigned char        a;              /* answer                           */
   unsigned char        f;              /* fault                            */
   unsigned char        b;              /* command                          */
   unsigned char        e;              /* extension                        */
   unsigned char        device_adr;     /* device address                   */
   unsigned char        data_area;      /* data area                        */
   unsigned short       data_adr;       /* data address                     */
   unsigned char        data_idx;       /* data index                       */
   unsigned char        data_cnt;       /* data count                       */
   unsigned char        data_type;      /* data type                        */
   unsigned char        function;       /* function                         */
   unsigned char        d[ RCS_SEGMENT_LEN-RCS_MESSAGEHEADER_LEN-RCS_TELEGRAMHEADER_LEN];
} GNUC_PACKED RCS_MESSAGETELEGRAM_10;


#if defined( _MSC_VER)          /* Microsoft C */
  #pragma pack()                /* Byte Alignment   */
#endif

#ifdef __cplusplus
  }
#endif  /* __cplusplus */

#endif /* __RCS_USER_H */

/* === eof 'RCS_USER.H' === */

