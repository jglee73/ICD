#pragma once

#include "Interface_Code.h"
#include "CObj__PART_CLIENT__DEF.h"


class CObj__PART_CLIENT : public __IOBJ__IO_TYPE
{
private:
	//-------------------------------------------------------------------------
	CString	sObject_Name;

	// ...
	SCX__USER_LOG_CTRL mX__Log_Ctrl;

	int iActive__SIM_MODE;

	// ...
	SCX__ETHERNET_JGLEE mX__Net_Client;

	bool bActive__COMM_ONLINE;
	//


	//-------------------------------------------------------------------------
	//  INTERNAL PROPERTY

	// OBJ ...
	CX__VAR_STRING_CTRL  sCH__OBJ_MSG;

	// MON ...
	CX__VAR_STRING_CTRL  sCH__MON_COMM_STATE;

	CX__VAR_STRING_CTRL  sCH__MON_CONTROL_MODE;
	CX__VAR_STRING_CTRL  sCH__MON_CHM_PRESSURE;
	CX__VAR_STRING_CTRL  sCH__MON_LIFT_PIN_STATE;
	CX__VAR_STRING_CTRL  sCH__MON_SHUTTER_STATE;
	CX__VAR_STRING_CTRL  sCH__MON_PROCESS_STATE;
	CX__VAR_STRING_CTRL  sCH__MON_CHAMBER_STATE;

	CX__VAR_STRING_CTRL  sCH__MON_CHM_TIME_COUNT;
	CX__VAR_STRING_CTRL  sCH__MON_PRC_TIME_COUNT;

	// INFO.DRV ...
	CX__VAR_STRING_CTRL  sCH__INFO_DRV_NET_IP;
	CX__VAR_STRING_CTRL  sCH__INFO_DRV_NET_PORT;

	CX__VAR_STRING_CTRL  sCH__INFO_DRV_ACT_NAME;
	CX__VAR_STRING_CTRL  sCH__INFO_DRV_ACT_RSP;

	// IO ...
	CX__VAR_DIGITAL_CTRL doCH__Control_SET;

	CX__VAR_STRING_CTRL  siCH__CONTROL_MODE;
	CX__VAR_STRING_CTRL  siCH__CHM_PRESSURE;
	CX__VAR_STRING_CTRL  siCH__PIN_STATE;
	CX__VAR_STRING_CTRL  siCH__SHUTTER_STATE;
	CX__VAR_STRING_CTRL  siCH__PROCESS_STATE;
	CX__VAR_STRING_CTRL  siCH__CHAMBER_STATE;
	//

	//-------------------------------------------------------------------------
	CString sMODE__INIT;
	int Call__INIT(CII_OBJECT__VARIABLE *p_variable, CII_OBJECT__ALARM *p_alarm);

	//
	CString sMODE__TRANSFER_START;
	int Call__TRANSFER_START(CII_OBJECT__VARIABLE *p_variable, CII_OBJECT__ALARM *p_alarm);

	CString sMODE__TRANSFER_END;
	int Call__TRANSFER_END(CII_OBJECT__VARIABLE *p_variable, CII_OBJECT__ALARM *p_alarm);

	//
	CString sMODE__PIN_UP;
	int Call__PIN_UP(CII_OBJECT__VARIABLE *p_variable, CII_OBJECT__ALARM *p_alarm);

	CString sMODE__PIN_DOWN;
	int Call__PIN_DOWN(CII_OBJECT__VARIABLE *p_variable, CII_OBJECT__ALARM *p_alarm);

	//
	CString sMODE__CHAMBER_PUMP;
	int Call__CHAMBER_PUMP(CII_OBJECT__VARIABLE *p_variable, CII_OBJECT__ALARM *p_alarm);

	CString sMODE__CHAMBER_VENT;
	int Call__CHAMBER_VENT(CII_OBJECT__VARIABLE *p_variable, CII_OBJECT__ALARM *p_alarm);

	CString sMODE__CHAMBER_ABORT;
	int Call__CHAMBER_ABORT(CII_OBJECT__VARIABLE *p_variable, CII_OBJECT__ALARM *p_alarm);

	//
	CString sMODE__PROCESS_START;
	int Call__PROCESS_START(CII_OBJECT__VARIABLE *p_variable, CII_OBJECT__ALARM *p_alarm);

	CString sMODE__CLEAN_START;
	int Call__CLEAN_START(CII_OBJECT__VARIABLE *p_variable, CII_OBJECT__ALARM *p_alarm);

	CString sMODE__PROCESS_ABORT;
	int Call__PROCESS_ABORT(CII_OBJECT__VARIABLE *p_variable, CII_OBJECT__ALARM *p_alarm);

	// ...
	int Fnc__COMMAND_SEND(CII_OBJECT__ALARM *p_alarm, const CString& str_cmd);
	//

	// ...
	void Mon__IO_MONITORING(CII_OBJECT__VARIABLE *p_variable, CII_OBJECT__ALARM *p_alarm);
	void Mon__DRV_PROC(CII_OBJECT__VARIABLE *p_variable, CII_OBJECT__ALARM *p_alarm);

	int  SEND__COMMAND(const CString& var_name,const CString& s_data, CString& r_data);
	//

public:
	CObj__PART_CLIENT();
	~CObj__PART_CLIENT();

	//-------------------------------------------------------------------------
	int __DEFINE__CONTROL_MODE(obj,l_mode);
	int __DEFINE__VERSION_HISTORY(version);

	int __DEFINE__VARIABLE_STD(p_variable);
	int __DEFINE__VARIABLE_IO(p_io_variable);

	int __DEFINE__ALARM(p_alarm);

	//-------------------------------------------------------------------------
	int __INITIALIZE__OBJECT(p_variable,p_ext_obj_create);
	int __INITIALIZE__IO(p_io_para);

	//-------------------------------------------------------------------------
	int __CALL__CONTROL_MODE(mode,p_debug,p_variable,p_alarm);
	int __CALL__MONITORING(id,p_variable,p_alarm);

	//-------------------------------------------------------------------------
	int __Read__ANALOG(const CString& var_name, const CDS_IO__CHANNEL_INFO& io_info, double& read_data);
	int __Read__DIGITAL(const CString& var_name,const CDS_IO__CHANNEL_INFO& io_info, CString& read_data,int& item_index);
	int __Read__STRING(const CString& var_name, const CDS_IO__CHANNEL_INFO& io_info, CString& read_data);

	int __Write__ANALOG(const CString& var_name, const CDS_IO__CHANNEL_INFO& io_info, const double set_data);
	int __Write__DIGITAL(const CString& var_name,const CDS_IO__CHANNEL_INFO& io_info, const CString& set_data,const int item_index);
	int __Write__STRING(const CString& var_name, const CDS_IO__CHANNEL_INFO& io_info, const CString& set_data);

};
