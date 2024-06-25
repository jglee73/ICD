#pragma once

#include "Interface_Code.h"


class CObj__PART_SERVER : public __IOBJ__IO_TYPE
{
private:
	//-------------------------------------------------------------------------
	CString	sObject_Name;

	// ...
	SCX__USER_LOG_CTRL  mX__Log_Ctrl;

	int iActive__SIM_MODE;

	SCX__ETHERNET_JGLEE mX__Net_Server;
	//


	//-------------------------------------------------------------------------
	//  INTERNAL PROPERTY

	CX__VAR_STRING_CTRL sCH__COMM_STATE;

	// CFG ...
	CX__VAR_ANALOG_CTRL aCH__CFG_CHM_PUMP_TIME;
	CX__VAR_ANALOG_CTRL aCH__CFG_CHM_VENT_TIME;
	CX__VAR_STRING_CTRL sCH__MON_CHM_TIME_COUNT;

	CX__VAR_ANALOG_CTRL aCH__CFG_PROCESS_TIME;
	CX__VAR_ANALOG_CTRL aCH__CFG_CLEAN_TIME;
	CX__VAR_STRING_CTRL sCH__MON_PRC_TIME_COUNT;
	//

	//-------------------------------------------------------------------------
	// ...
	CString sMODE__INIT;
	int Call__INIT(CII_OBJECT__VARIABLE *p_variable, CII_OBJECT__ALARM *p_alarm);
	//

	// ...
	void Mon__DRV_PROC(CII_OBJECT__VARIABLE *p_variable, CII_OBJECT__ALARM *p_alarm);

	// ...
	int  SEND__COMMAND(const CString& s_data);
	//

public:
	CObj__PART_SERVER();
	~CObj__PART_SERVER();

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
