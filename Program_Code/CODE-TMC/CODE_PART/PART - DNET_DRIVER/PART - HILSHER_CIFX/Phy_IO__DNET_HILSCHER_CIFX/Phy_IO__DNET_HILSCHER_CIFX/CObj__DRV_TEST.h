#pragma once

#include "Interface_Code.h"

#include "CObj__DRV_TEST__ALID.h"
#include "CObj__DRV_TEST__DEF.h"

#include "CCtrl__DNet_Node.h"
#include "DNet_Mng.h"


// ...
#define CFG__SLAVE_SIZE						30


class CObj__DRV_TEST : public __IOBJ__IO_TYPE
{
private:
	//-------------------------------------------------------------------------
	CString	sObject_Name;

	// ...
	CRITICAL_SECTION mLOCK_DNET;

	int iActive__SIM_MODE;

	// ...
	int iDNet_BoardNumber;
	int iDNet_ChannelNumber;

	int iDNet_Board_Out_Offset;
	int iDNet_Board_In_Offset;

	CDNet_Mng mDNet_Mng;

	// ...
	unsigned char cDNet_OutputData[3584];
	unsigned char cDNet_InputData[3584];

	unsigned short uDNet_OutputSize;
	unsigned short uDNet_InputSize;

	// ...
	CCtrl__DNet_Node mCtrl__DNet_Node;

	HANDLE hRes__DRV_BOARD;
	HANDLE hRes__DRV_CHANNEL;

	bool bActive__COMM_STATE;

	bool bActive__DNET_START;
	bool bActive__POLL_LOOP;
	bool bActive__EXT_MSG;
	//
	
	//-------------------------------------------------------------------------
	SCX__USER_LOG_CTRL xI__APP_LOG_CTRL;
	SCX__USER_LOG_CTRL xI__DRV_LOG_CTRL;

	void Write__APP_LOG(const CString& log_msg);

	void Write__DRV_LOG(const CString& log_msg);
	void Write__DRV_LOG(const CString& fnc_type,
						const CString& var_name, 
						const CDS_IO__CHANNEL_INFO& io_info,
						const CString& io_data = "",
						const int io_index = -1);
	void Write__DRV_LOG(const CString& fnc_type,
						const CString& var_name, 
						const CDS_IO__CHANNEL_INFO& io_info,
						const double set_data);


	//-------------------------------------------------------------------------
	// VIRTUAL CHANNEL ...

	// OBJ ...
	CX__VAR_STRING_CTRL  sCH__OBJ_MSG;

	// MON  ...
	CX__VAR_STRING_CTRL  dCH__MON_COMM_STATE;
	CX__VAR_DIGITAL_CTRL dCH__MON_ACTIVE_DNET_START;

	// PARA ...
	CX__VAR_ANALOG_CTRL  aCH__PARA_SLAVE_REQ_MAC_ID;
	CX__VAR_DIGITAL_CTRL dCH__PARA_SLAVE_REQ_INFO;

	CX__VAR_DIGITAL_CTRL dCH__PARA_SLAVE_RSP_STATE;
	CX__VAR_STRING_CTRL  sCH__PARA_SLAVE_RSP_DATA_HEXA;
	CX__VAR_STRING_CTRL  sCH__PARA_SLAVE_RSP_DATA_1;
	CX__VAR_STRING_CTRL  sCH__PARA_SLAVE_RSP_DATA_2;

	// Test.Para ...
	CX__VAR_STRING_CTRL  sCH__TEST_PARA_BOARD_NAME;
	CX__VAR_ANALOG_CTRL  aCH__TEST_PARA_CHANNEL_ID;

	CX__VAR_ANALOG_CTRL  aCH__TEST_PARA_MAC_ID;
	CX__VAR_ANALOG_CTRL  aCH__TEST_PARA_CLASS_ID;
	CX__VAR_ANALOG_CTRL  aCH__TEST_PARA_INSTANCE_ID;
	CX__VAR_ANALOG_CTRL  aCH__TEST_PARA_ATTRIBUTE_ID;

	// Config ...
	CX__VAR_DIGITAL_CTRL dCH__CFG_DRV_INT_ERROR_CHECK;
	CX__VAR_ANALOG_CTRL  aCH__CFG_DRV_INT_STABLE_SEC;
	CX__VAR_ANALOG_CTRL  aCH__CFG_DRV_INT_DELAY_mSEC;
	CX__VAR_ANALOG_CTRL  aCH__CFG_DRV_INT_RETRY_CHECK;

	//
	CX__VAR_ANALOG_CTRL  aCH__CFG_DNET_HEARTBEAT_PERIOD;
	CX__VAR_STRING_CTRL  sCH__DNET_HEARTBEAT_TIME_COUNT;

	//
	CX__VAR_STRING_CTRL  sCH__DNET_INFO__MASTER_BOARD_ID;
	CX__VAR_STRING_CTRL  sCH__DNET_INFO__MASTER_BOARD_IN_BYTE_OFFSET;
	CX__VAR_STRING_CTRL  sCH__DNET_INFO__MASTER_BOARD_OUT_BYTE_OFFSET;

	CX__VAR_STRING_CTRL  sCH__DNET_INFO__MASTER_BOARD_NAME;
	CX__VAR_STRING_CTRL  sCH__DNET_INFO__MASTER_BOARD_DRIVER_VERSION;
	CX__VAR_STRING_CTRL  sCH__DNET_INFO__BAUD_RATE;

	//
	CX__VAR_STRING_CTRL  sCH__DNET_INFO__MASTER_CHANNEL_ID;

	//
	CX__VAR_STRING_CTRL  sCH__DNET_INFO__TOTAL_OUT_BYTE;
	CX__VAR_STRING_CTRL  sCH__DNET_INFO__TOTAL_IN_BYTE;

	// CFG.MAX ...
	CX__VAR_STRING_CTRL  sCH__DNET_CFG__TOTAL_OUT_BYTE_MAX;
	CX__VAR_STRING_CTRL  sCH__DNET_CFG__TOTAL_IN_BYTE_MAX;

	// CFG.USE ...
	CX__VAR_STRING_CTRL  sCH__DNET_CFG__TOTAL_OUT_BYTE_USE;
	CX__VAR_STRING_CTRL  sCH__DNET_CFG__TOTAL_IN_BYTE_USE;

	// Slave.Info ...
	int iSLAVE_COUNT;

	CX__VAR_DIGITAL_CTRL dCH__APP_DRV_LOG_ENABLE;

	//
	CX__VAR_STRING_CTRL  sCH__DNET_INFO__SLAVE_COUNT;
	CX__VAR_STRING_CTRL  sCH__DNET_INFO__SLAVE_X__MACID[CFG__SLAVE_SIZE];
	CX__VAR_STRING_CTRL  sCH__DNET_INFO__SLAVE_X__NAME[CFG__SLAVE_SIZE];
	CX__VAR_STRING_CTRL  sCH__DNET_INFO__SLAVE_X__IN_SIZE_USE[CFG__SLAVE_SIZE];
	CX__VAR_STRING_CTRL  sCH__DNET_INFO__SLAVE_X__OUT_SIZE_USE[CFG__SLAVE_SIZE];

	CX__VAR_DIGITAL_CTRL dCH__DNET_INFO__COMM_STATE_CHECK_ACTIVE;
	CX__VAR_STRING_CTRL  sCH__DNET_INFO__SLAVE_X__COMM_STATE[CFG__SLAVE_SIZE];
	CX__VAR_STRING_CTRL  sCH__DNET_INFO__SLAVE_X__COMM_HEXA[CFG__SLAVE_SIZE];

	CX__VAR_STRING_CTRL  sCH__DNET_INFO__SLAVE_X__ERROR_CHECK_ID[CFG__SLAVE_SIZE];

	//
	CX__VAR_STRING_CTRL  sCH__DNET_CFG__SLAVE_COUNT;
	CX__VAR_STRING_CTRL  sCH__DNET_CFG__SLAVE_X__MACID[CFG__SLAVE_SIZE];
	CX__VAR_STRING_CTRL  sCH__DNET_CFG__SLAVE_X__NAME[CFG__SLAVE_SIZE];

	CX__VAR_STRING_CTRL  sCH__DNET_CFG__SLAVE_X__IN_OFFSET[CFG__SLAVE_SIZE];
	CX__VAR_STRING_CTRL  sCH__DNET_CFG__SLAVE_X__IN_SIZE_MAX[CFG__SLAVE_SIZE];
	CX__VAR_STRING_CTRL  sCH__DNET_CFG__SLAVE_X__IN_SIZE_USE[CFG__SLAVE_SIZE];

	CX__VAR_STRING_CTRL  sCH__DNET_CFG__SLAVE_X__OUT_OFFSET[CFG__SLAVE_SIZE];
	CX__VAR_STRING_CTRL  sCH__DNET_CFG__SLAVE_X__OUT_SIZE_MAX[CFG__SLAVE_SIZE];
	CX__VAR_STRING_CTRL  sCH__DNET_CFG__SLAVE_X__OUT_SIZE_USE[CFG__SLAVE_SIZE];

	//
	CX__VAR_DIGITAL_CTRL dCH__DNET_INFO__OUT_BYTE_CHECK_ACTIVE;
	CX__VAR_DIGITAL_CTRL dCH__DNET_INFO__IN_BYTE_CHECK_ACTIVE;

	//
	CX__VAR_STRING_CTRL  sCH__TEST_GAUGE_HEXA;
	CX__VAR_STRING_CTRL  sCH__TEST_GAUGE_VALUE;

	CX__VAR_STRING_CTRL  sCH__TEST_FFFF_HEXA;
	CX__VAR_STRING_CTRL  sCH__TEST_FFFF_VALUE;

	// LINK.IO ...
	CX__VAR_ANALOG_CTRL  aCH__CFG_LINK_IO_WAITING_SEC;

	int iSIZE__LINK_IO;
	CX__VAR_DIGITAL_CTRL dEXT_CH__DNET_LINK_IO[CFG__LINK_IO_SIZE];

	// HEARTBEAT.IO ...
	bool bActive__DO_DNET_HEARTBEAT;
	CX__VAR_DIGITAL_CTRL dEXT_CH__DO_DNET_HEARTBEAT;
	//


	//-------------------------------------------------------------------------
	CString sMODE__DRIVER_OPEN;
	int  Call__DRIVER_OPEN(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	//
	CString sMODE__ENUM_BOARD;
	int  Call__ENUM_BOARD(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	int  Fnc__ENUM_BOARD(const HANDLE h_drv_board);

	//
	CString sMODE__SYS_DEVICE;
	int  Call__SYS_DEVICE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	CString sMODE__CHANNEL_INFO;
	int  Call__CHANNEL_INFO(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	CString sMODE__DEVICE_RESET;
	int  Call__DEVICE_RESET(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	CString sMODE__DRIVER_CLOSE;
	int  Call__DRIVER_CLOSE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	//
	CString sMODE__EXPLICIT_MESSAGE;
	int  Call__EXPLICIT_MESSAGE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	int  Fnc__DNET_CMD__GET_ATT_REQ(const HANDLE h_drv_channel,const CString& para__slave_info);

	//
	CString Get__Dump_Data(unsigned char* p_data, unsigned long ch_len);
	CString Get__Dump_Packet(CIFX_PACKET* p_packet);

	// ...
	CString sMODE__LINK_IO_SET_OFF;
	int  Call__LINK_IO_SET_OFF(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	CString sMODE__LINK_IO_SET_ON;
	int  Call__LINK_IO_SET_ON(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	// ...
	CString sMODE__DEV_INFO;
	int  Call__DEV_INFO(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	int  _Check__CFG_SLAVE_INFO();

	int  Check__DEV_ERROR();
	int  Check__DEV_ERROR(CString& err_msg);

	int  _Fnc__DEV_INFO(const bool active_update);
	int  _Check__DEV_TOTAL_MEMORY();
	//

	// ...
	void Mon__MONITOR(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	void _DNet__UPDATE_NODE_X(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);
	void _DNet__UPDATE_NODE_ALL(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	// ...
	bool bActive__DNET_INIT;

	int _Init__DNET_MASTER_BY_USER_CFG();
	//


public:
	CObj__DRV_TEST();
	~CObj__DRV_TEST();

	//-------------------------------------------------------------------------
	int __DEFINE__CONTROL_MODE(obj,l_mode);
	int __DEFINE__VERSION_HISTORY(version);

	int __DEFINE__VARIABLE_STD(p_variable);
	int __DEFINE__VARIABLE_IO(p_variable);

	int __DEFINE__ALARM(p_alarm);

	//-------------------------------------------------------------------------
	int __INITIALIZE__OBJECT(p_variable,p_ext_obj_create);
	int __INITIALIZE__IO(p_io_para);

	//-------------------------------------------------------------------------
	int __CALL__CONTROL_MODE(mode,p_debug,p_variable,p_alarm);
	int __CALL__MONITORING(id, p_variable, p_alarm);

	//-------------------------------------------------------------------------
	int __Read__ANALOG(const CString& var_name, const CDS_IO__CHANNEL_INFO& io_info, double& read_data);
	int __Read__DIGITAL(const CString& var_name,const CDS_IO__CHANNEL_INFO& io_info, CString& read_data,int& item_index);
	int __Read__STRING(const CString& var_name, const CDS_IO__CHANNEL_INFO& io_info, CString& read_data);

	int __Write__ANALOG(const CString& var_name, const CDS_IO__CHANNEL_INFO& io_info, const double set_data);
	int __Write__DIGITAL(const CString& var_name,const CDS_IO__CHANNEL_INFO& io_info, const CString& set_data,const int item_index);
	int __Write__STRING(const CString& var_name, const CDS_IO__CHANNEL_INFO& io_info, const CString& set_data);

	//-------------------------------------------------------------------------
	int __CLOSE__OBJECT();
};
