#pragma once

#include "Interface_Code.h"

#include "CObj__DNET_STD__ALID.h"
#include "CObj__DNET_STD__DEF.h"

#include "CCtrl__DNet_Node.h"
#include "DNet_Mng.h"


// ...
#define CFG__SLAVE_SIZE					30

#define CFG__DNET_OUT_BYTE_LIMIT		300
#define CFG__DNET_IN_BYTE_LIMIT			300


class CObj__DNET_STD : public __IOBJ__IO_TYPE
{
private:
	//-------------------------------------------------------------------------
	CString	sObject_Name;

	// ...
	CRITICAL_SECTION mLOCK_DNET;

	int iActive__SIM_MODE;

	int iDNet_BoardNumber;
	int iDNet_Board_Out_Offset;
	int iDNet_Board_In_Offset;
	CDNet_Mng mDNet_Mng;

	unsigned char  abOutputData[3584];
	unsigned char  abInputData[3584];

	unsigned short usInputOffset;
	unsigned short usOutputOffset;

	CCtrl__DNet_Node mCtrl__DNet_Node;

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
	// VIRTUAL CHANNEL

	// OBJ ...
	CX__VAR_STRING_CTRL  sCH__OBJ_MSG;

	// Config ...
	CX__VAR_DIGITAL_CTRL dCH__CFG_SLAVE_INFO_CHECK;

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
	CX__VAR_STRING_CTRL  sCH__DNET_INFO__TOTAL_OUT_BYTE;
	CX__VAR_STRING_CTRL  sCH__DNET_INFO__TOTAL_IN_BYTE;

	CX__VAR_STRING_CTRL  sCH__DNET_CFG__TOTAL_OUT_BYTE_MAX;
	CX__VAR_STRING_CTRL  sCH__DNET_CFG__TOTAL_IN_BYTE_MAX;

	CX__VAR_STRING_CTRL  sCH__DNET_CFG__TOTAL_OUT_BYTE_USE;
	CX__VAR_STRING_CTRL  sCH__DNET_CFG__TOTAL_IN_BYTE_USE;

	//
	int iSLAVE_COUNT;

	//
	CX__VAR_STRING_CTRL  diCH__COMM_STS;
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
	CX__VAR_STRING_CTRL  sCH__DNET_INFO__OUT_BYTE_DATA_X[CFG__DNET_OUT_BYTE_LIMIT];

	CX__VAR_DIGITAL_CTRL dCH__DNET_INFO__IN_BYTE_CHECK_ACTIVE;
	CX__VAR_STRING_CTRL  sCH__DNET_INFO__IN_BYTE_DATA_X[CFG__DNET_IN_BYTE_LIMIT];

	//
	CX__VAR_STRING_CTRL  sCH__FLOAT_TO_HEXA__VALUE;
	CX__VAR_STRING_CTRL  sCH__FLOAT_TO_HEXA__RESULT;

	CX__VAR_STRING_CTRL  sCH__HEXA_TO_FLOAT__VALUE;
	CX__VAR_STRING_CTRL  sCH__HEXA_TO_FLOAT__RESULT;

	//
	CX__VAR_STRING_CTRL  sCH__TEST_GAUGE_HEXA;
	CX__VAR_STRING_CTRL  sCH__TEST_GAUGE_VALUE;

	CX__VAR_STRING_CTRL  sCH__TEST_FFFF_HEXA;
	CX__VAR_STRING_CTRL  sCH__TEST_FFFF_VALUE;

	//
	CX__VAR_ANALOG_CTRL  aCH__CFG_LINK_IO_WAITING_SEC;

	int iSIZE__LINK_IO;
	CX__VAR_DIGITAL_CTRL dEXT_CH__DNET_LINK_IO[CFG__LINK_IO_SIZE];

	//
	bool bActive__DO_DNET_HEARTBEAT;
	CX__VAR_DIGITAL_CTRL dEXT_CH__DO_DNET_HEARTBEAT;
	//


	//-------------------------------------------------------------------------
	CString sMODE__INIT;
	int  Call__INIT(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	//
	CString sMODE__LINK_IO_SET_OFF;
	int  Call__LINK_IO_SET_OFF(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	CString sMODE__LINK_IO_SET_ON;
	int  Call__LINK_IO_SET_ON(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	//
	CString sMODE__DEV_INFO;
	int  Call__DEV_INFO(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);
	int  _Fnc__DEV_INFO(const bool active_update);

	int  _Check__CFG_SLAVE_INFO();

	int  _Check__SLAVE_ERROR();
	int  _Check__SLAVE_ERROR(CString& err_msg);

	int  _Check__DEV_TOTAL_MEMORY();

	//
	CString sMODE__FLOAT_TO_HEXA;
	int  Call__FLOAT_TO_HEXA(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	CString sMODE__HEXA_TO_FLOAT;
	int  Call__HEXA_TO_FLOAT(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);
	CString Get__HEXA_TO_FLOAT(const CString str_hexa);

	//
	CString sMODE__NODE_OUT_BYTE_UPLOAD;
	int  Call__NODE_OUT_BYTE_UPLOAD(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	CString sMODE__NODE_IN_BYTE_UPLOAD;
	int  Call__NODE_IN_BYTE_UPLOAD(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	// ...
	void Mon__MONITOR(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);
	
	void _DNet__UPDATE_INFO();

	// ...
	unsigned char DNet__ReadDeviceData(unsigned char  bDeviceAdr,
										unsigned char  bClass,
										unsigned short usInstance,
										unsigned char  bAttribute,
										unsigned char *ptRespData);
	unsigned char _DNet__ReadDeviceData(unsigned char  bDeviceAdr,
										unsigned char  bClass,
										unsigned short usInstance,
										unsigned char  bAttribute,
										unsigned char *ptRespData);

	// ...
	bool bActive__DNET_INIT;

	int _Init__DNET_MASTER_BY_USER_CFG();
	int _Init__DNET_MASTER_BY_AUTO_CFG();
	//


public:
	CObj__DNET_STD();
	~CObj__DNET_STD();

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
