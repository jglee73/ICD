#pragma once

#include "Interface_Code.h"

#include "CCommon_System.h"
#include "CCommon_Error.h"


// ...
#define  CFG_LEAK_RATE__STEP_SIZE			7


class CObj__LLx_TwoManiFold : public __IOBJ__STD_TYPE
{
private:
	CCommon_Error__USER_FNC mERROR__USER_FNC;

	//-------------------------------------------------------------------------
	CString sObject_Name;

	SCX__USER_LOG_CTRL xLOG_CTRL;
	SCX__USER_LOG_CTRL xLOG_CTRL__LeakCheck_LBA;
	SCX__USER_LOG_CTRL xLOG_CTRL__LeakCheck_LBB;


	//-------------------------------------------------------------------------
	// INTERNAL PROPERTY

	CX__VAR_STRING_CTRL  sCH__OBJ_STATUS;
	CX__VAR_STRING_CTRL  xCH__OBJ_MSG;

	//.....
	CX__VAR_STRING_CTRL  sCH__LBA_ROBOT_TRANSFER_FLAG;
	CX__VAR_STRING_CTRL  sCH__LBB_ROBOT_TRANSFER_FLAG;

	//.....
	CX__VAR_DIGITAL_CTRL dCH__CFG_LBA_LEAK_CHECK;
	CX__VAR_DIGITAL_CTRL dCH__CFG_LBB_LEAK_CHECK;

	// LBA : LEAK_CHECK PARA -----
	CX__VAR_ANALOG_CTRL  aCH__LBA_LEAK_CHECK__TIME_COUNT;
	CX__VAR_ANALOG_CTRL  aCH__CFG_LBA_LEAK_CHECK__OVER_PUMPING_TIME;
	CX__VAR_ANALOG_CTRL  aCH__CFG_LBA_LEAK_CHECK__STABLE_TIME;
	CX__VAR_ANALOG_CTRL  aCH__CFG_LBA_LEAK_CHECK__CHECK_TIME_MIN;
	CX__VAR_STRING_CTRL  sCH__LBA_LEAK_CHECK__START_TIME;
	CX__VAR_STRING_CTRL  sCH__LBA_LEAK_CHECK__END_TIME;
	CX__VAR_STRING_CTRL  sCH__LBA_LEAK_CHECK__START_PRESSURE_TORR;
	CX__VAR_STRING_CTRL  sCH__LBA_LEAK_CHECK__END_PRESSURE_TORR;
	CX__VAR_STRING_CTRL  sCH__LBA_LEAK_CHECK__STEP_PRESSURE_TORR[CFG_LEAK_RATE__STEP_SIZE];
	CX__VAR_STRING_CTRL  sCH__LBA_LEAK_CHECK__STEP_LEAK_RATE[CFG_LEAK_RATE__STEP_SIZE];
	CX__VAR_STRING_CTRL  sCH__LBA_LEAK_CHECK__TOTAL_LEAK_RATE;
	CX__VAR_ANALOG_CTRL  CFG_aCH__LBA_LEAK_CHECK__ALARM_RATE;

	// LBB : LEAK_CHECK PARA -----
	CX__VAR_ANALOG_CTRL  aCH__LBB_LEAK_CHECK__TIME_COUNT;
	CX__VAR_ANALOG_CTRL  aCH__CFG_LBB_LEAK_CHECK__OVER_PUMPING_TIME;
	CX__VAR_ANALOG_CTRL  aCH__CFG_LBB_LEAK_CHECK__STABLE_TIME;
	CX__VAR_ANALOG_CTRL  aCH__CFG_LBB_LEAK_CHECK__CHECK_TIME_MIN;
	CX__VAR_STRING_CTRL  sCH__LBB_LEAK_CHECK__START_TIME;
	CX__VAR_STRING_CTRL  sCH__LBB_LEAK_CHECK__END_TIME;
	CX__VAR_STRING_CTRL  sCH__LBB_LEAK_CHECK__START_PRESSURE_TORR;
	CX__VAR_STRING_CTRL  sCH__LBB_LEAK_CHECK__END_PRESSURE_TORR;
	CX__VAR_STRING_CTRL  sCH__LBB_LEAK_CHECK__STEP_PRESSURE_TORR[CFG_LEAK_RATE__STEP_SIZE];
	CX__VAR_STRING_CTRL  sCH__LBB_LEAK_CHECK__STEP_LEAK_RATE[CFG_LEAK_RATE__STEP_SIZE];
	CX__VAR_STRING_CTRL  sCH__LBB_LEAK_CHECK__TOTAL_LEAK_RATE;
	CX__VAR_ANALOG_CTRL  CFG_aCH__LBB_LEAK_CHECK__ALARM_RATE;

	// ...
	CX__VAR_DIGITAL_CTRL dCH__PUMPING_STS_FLAG;


	//-------------------------------------------------------------------------
	// EXTERNAL PROPERTY

	// DB OBJ ...
	CX__VAR_DIGITAL_CTRL dEXT_CH__CFG_TRANSFER_MODE;
	CX__VAR_ANALOG_CTRL  aEXT_CH__CFG_REF_ATM_PRESSURE;
	CX__VAR_ANALOG_CTRL  aEXT_CH__CFG_REF_VAC_PRESSURE;

	// OBJ LLx_LEFT ...
	CII_EXT_OBJECT__CTRL *pLBA_PRESS__OBJ_CTRL;
	CString sLOG_NAME__LLx_LEFT;

	CX__VAR_STRING_CTRL  sEXT_CH__LBA__OBJ_STATUS;

	CX__VAR_STRING_CTRL  sEXT_CH__LBA__PUMP_VLV_OPEN_FLAG;
	CX__VAR_STRING_CTRL  sEXT_CH__LBA__PRESSURE_CTRL_FLAG;

	CX__VAR_ANALOG_CTRL  aEXT_CH__LBA__PRESSURE_TORR;
	CX__VAR_DIGITAL_CTRL dEXT_CH__LBA__PRESSURE_STATUS;
	CX__VAR_DIGITAL_CTRL dEXT_CH__LBA__SLIT_VALVE_STATUS;

	// OBJ LLx_RIGHT ...
	CII_EXT_OBJECT__CTRL *pLBB_PRESS__OBJ_CTRL;
	CString sLOG_NAME__LLx_RIGHT;

	CX__VAR_STRING_CTRL  sEXT_CH__LBB__OBJ_STATUS;

	CX__VAR_STRING_CTRL  sEXT_CH__LBB__PUMP_VLV_OPEN_FLAG;
	CX__VAR_STRING_CTRL  sEXT_CH__LBB__PRESSURE_CTRL_FLAG;

	CX__VAR_ANALOG_CTRL  aEXT_CH__LBB__PRESSURE_TORR;
	CX__VAR_DIGITAL_CTRL dEXT_CH__LBB__PRESSURE_STATUS;
	CX__VAR_DIGITAL_CTRL dEXT_CH__LBB__SLIT_VALVE_STATUS;

	// PRESS VLV OBJECT -----
	CII_EXT_OBJECT__CTRL *pPRESS_VLV__OBJ_CTRL;
	//

	//------------------------------------------------------------------------------
	// LBA & LEFT ...
	CString sMODE__LBA_PUMP;
	CString sMODE__LEFT_PUMP;
	int  Call__LEFT_PUMP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	CString sMODE__LBA_LEAK_CHECK;
	CString sMODE__LEFT_LEAK_CHECK;
	int  Call__LEFT_LEAK_CHECK(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	int  Fnc__LEFT_LEAK_CHECK(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	// LBB & RIGHT ...
	CString sMODE__LBB_PUMP;
	CString sMODE__RIGHT_PUMP;
	int  Call__RIGHT_PUMP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	CString sMODE__LBB_LEAK_CHECK;
	CString sMODE__RIGHT_LEAK_CHECK;
	int  Call__RIGHT_LEAK_CHECK(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	int  Fnc__RIGHT_LEAK_CHECK(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	// ...
	int  Check__OBJECT_BUSY(const CString& obj_sts);

	int  Check__ALL_VLV_CLOSE(CII_OBJECT__VARIABLE* p_variable,
								CII_OBJECT__ALARM* p_alarm,
								const CString& act_name);

	int  Check__SYSTEM_VAC_PRESSURE(CII_OBJECT__VARIABLE* p_variable,
									CII_OBJECT__ALARM* p_alarm,
									const int lba_pump_flag,
									const int lbb_pump_flag,
									int& retry_flag);

	int  Check__SYSTEM_VAC_PRESSURE_WITHOUT_ALARM(CII_OBJECT__VARIABLE* p_variable,
													CII_OBJECT__ALARM* p_alarm,
													const CString& module_name,
													CII__VAR_ANALOG_CTRL* pch_module_pressure,
													CString& alarm_msg);

	// ...
	SCI__USER_LOG_CTRL* Get_Ptr__USER_LOG_CTRL(const CString& module_name);

	int LeakCheck__LOG_MakeFile__LLx_Left();
	int LeakCheck__LOG_MakeFile__LLx_Right();
	int _LeakCheck__LOG_MakeFile(const CString& llx_name,const CString& log_name);
	
	int LeakCheck__LOG_WriteMsg__LLx_Left(const CString& log_msg);
	int LeakCheck__LOG_WriteMsg__LLx_Right(const CString& log_msg);
	int _LeakCheck__LOG_WriteMsg(const CString& llx_name,const CString& log_msg);


	//------------------------------------------------------------------------------
	void Mon__IO_MONITOR(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);
	//


public:
	CObj__LLx_TwoManiFold();
	~CObj__LLx_TwoManiFold();

	//-------------------------------------------------------------------------
	int __DEFINE__CONTROL_MODE(obj,l_mode);
	int __DEFINE__VERSION_HISTORY(version);

	int __DEFINE__VARIABLE_STD(p_variable);
	int __DEFINE__ALARM(p_alarm);

	//-------------------------------------------------------------------------
	int __INITIALIZE__OBJECT(p_variable,p_ext_obj_create);

	//-------------------------------------------------------------------------
	int __CALL__CONTROL_MODE(mode,p_debug,p_variable,p_alarm);
	int __CALL__MONITORING(id,p_variable,p_alarm);
};
