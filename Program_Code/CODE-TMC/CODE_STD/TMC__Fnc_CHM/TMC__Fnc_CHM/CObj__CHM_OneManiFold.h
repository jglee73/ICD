#pragma once

#include "Interface_Code.h"

#include "CCommon_System.h"
#include "CCommon_Error.h"


// ...
#define  CFG_LEAK_RATE__STEP_SIZE			7


class CObj__CHM_OneManiFold : public __IOBJ__STD_TYPE
{
private:
	CCommon_Error__USER_FNC mERROR__USER_FNC;

	//-------------------------------------------------------------------------
	CString sObject_Name;

	SCX__USER_LOG_CTRL xLOG_CTRL;
	SCX__USER_LOG_CTRL xLOG_CTRL__LeakCheck_TMC;

	int m_nPM_LIMIT;


	//-------------------------------------------------------------------------
	// INTERNAL PROPERTY

	// OBJ ...
	CX__VAR_STRING_CTRL  sCH__OBJ_STATUS;
	CX__VAR_STRING_CTRL  xCH__OBJ_MSG;

	CX__VAR_ANALOG_CTRL  aCH__TMC_CHM_PRESSURE_TORR;
	CX__VAR_STRING_CTRL  sCH__ROBOT_MATERIAL_TRANSFER_FLAG;

	// PARA ...
	CX__VAR_DIGITAL_CTRL dCH__PARA_BALLAST_CTRL_ACTIVE;

	// CFG ...
	CX__VAR_DIGITAL_CTRL dCH__CFG_TMC_LEAK_CHECK;

	// TMC : LEAK_CHECK PARA -----
	CX__VAR_ANALOG_CTRL  aCH__TMC_LEAK_CHECK__TIME_COUNT;
	CX__VAR_ANALOG_CTRL  aCH__CFG_TMC_LEAK_CHECK__OVER_PUMPING_TIME;
	CX__VAR_ANALOG_CTRL  aCH__CFG_TMC_LEAK_CHECK__STABLE_TIME;
	CX__VAR_ANALOG_CTRL  aCH__CFG_TMC_LEAK_CHECK__CHECK_TIME_MIN;
	CX__VAR_STRING_CTRL  sCH__TMC_LEAK_CHECK__START_TIME;
	CX__VAR_STRING_CTRL  sCH__TMC_LEAK_CHECK__END_TIME;
	CX__VAR_STRING_CTRL  sCH__TMC_LEAK_CHECK__START_PRESSURE_TORR;
	CX__VAR_STRING_CTRL  sCH__TMC_LEAK_CHECK__END_PRESSURE_TORR;
	CX__VAR_STRING_CTRL  sCH__TMC_LEAK_CHECK__STEP_PRESSURE_TORR[CFG_LEAK_RATE__STEP_SIZE];
	CX__VAR_STRING_CTRL  sCH__TMC_LEAK_CHECK__STEP_LEAK_RATE[CFG_LEAK_RATE__STEP_SIZE];
	CX__VAR_STRING_CTRL  sCH__TMC_LEAK_CHECK__TOTAL_LEAK_RATE;
	CX__VAR_ANALOG_CTRL  CFG_aCH__TMC_LEAK_CHECK__ALARM_RATE;

	// ...
	CX__VAR_STRING_CTRL  sCH__PRESSURE_STATUS;

	CX__VAR_DIGITAL_CTRL dCH__PUMPING_STS_FLAG;


	//-------------------------------------------------------------------------
	// EXTERNAL PROPERTY

	// DB OBJECT -----
	CX__VAR_DIGITAL_CTRL dEXT_CH__CFG_TRANSFER_MODE;
	CX__VAR_ANALOG_CTRL  aEXT_CH__CFG_REF_ATM_PRESSURE;
	CX__VAR_ANALOG_CTRL  aEXT_CH__CFG_REF_VAC_PRESSURE;

	CX__VAR_DIGITAL_CTRL dEXT_CH__CFG_PMx_EXIST_FLAG[CFG_PMx__SIZE];
	CX__VAR_DIGITAL_CTRL dEXT_CH__CFG_PMx_SV_USE[CFG_PMx__SIZE];

	// TMC CHM -----
	CII_EXT_OBJECT__CTRL *pTMC_CHM_PRESS__OBJ_CTRL;

	CX__VAR_DIGITAL_CTRL dEXT_CH__TMC_CHM__PARA_BALLAST_CTRL_ACTIVE;

	CX__VAR_STRING_CTRL  sEXT_CH__TMC_CHM__OBJ_STATUS;
	CX__VAR_STRING_CTRL  sEXT_CH__TMC_CHM__PUMP_VLV_OPEN_FLAG;
	CX__VAR_STRING_CTRL  sEXT_CH__TMC_CHM__PRESSURE_CTRL_FLAG;

	CX__VAR_ANALOG_CTRL  aEXT_CH__TMC_CHM__PRESSURE_TORR;
	CX__VAR_DIGITAL_CTRL dEXT_CH__TMC_CHM__PRESSURE_STATUS;

	// LLx OBJ ...
	int  iCFG__LLx_SIZE;
	int  iLLx_SLOT_SIZE;
	bool bActive__LLx_MULTI_DOOR_VALVE;
	bool bActive__LLx_MULTI_SLOT_VALVE;
	
	CX__VAR_DIGITAL_CTRL dEXT_CH__LLx__SLIT_VALVE_STATUS_A[CFG_LLx__SIZE];
	CX__VAR_DIGITAL_CTRL dEXT_CH__LLx__SLIT_VALVE_STATUS_XX[CFG_LLx__SIZE][_LLx_SLOT__SIZE];

	// PRESS VLV OBJ ...
	CII_EXT_OBJECT__CTRL *pPRESS_VLV__OBJ_CTRL;

	// Cycle Purge ...
	CX__VAR_ANALOG_CTRL  aCFG_CH__CYCLE_PURGE_COUNT;
	CX__VAR_STRING_CTRL  sCFG_CH__CYCLE_PURGE_CURRENT_COUNT;
	CX__VAR_STRING_CTRL  sCFG_CH__CYCLE_PURGE_SEQ_MSG;

	// PMC OBJ ...
	CII_EXT_OBJECT__CTRL *pPMx__OBJ_CTRL;

	CX__VAR_DIGITAL_CTRL dEXT_CH__PMx__SLIT_VALVE_STATUS[CFG_PMx__SIZE];
	//


	//------------------------------------------------------------------------------
	// TMC_CHM ---------------
	// INIT -----
	CString sMODE__INIT;
	int  Call__INIT(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	// ISOLATE -----
	CString sMODE__ISOLATE;
	int  Call__ISOLATE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	// PUMP -----
	CString sMODE__PUMP;
	int  Call__PUMP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	// VENT -----
	CString sMODE__VENT;
	int  Call__VENT(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	// LEAK_CHECK -----
	CString sMODE__LEAK_CHECK;
	int  Call__LEAK_CHECK(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	int  Fnc__LEAK_CHECK(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	// CYCLE PURGE -----
	CString sMODE__CYCLE_PURGE;
	int  Call__CYCLE_PURGE(CII_OBJECT__ALARM* p_alarm,CII_OBJECT__VARIABLE* p_variable);

	// ...
	int  Check__OBJECT_BUSY(const CString& obj_sts);

	int  Check__ALL_VLV_CLOSE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm,const CString& act_name);

	// ...
	SCI__USER_LOG_CTRL* Get_Ptr__USER_LOG_CTRL(const CString& module_name);

	int  LeakCheck__LOG_MakeFile(const CString& module_name);
	int  LeakCheck__LOG_WriteMsg(const CString& module_name,const CString& log_msg);


	//------------------------------------------------------------------------------
	void Mon__IO_MONITOR(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);
	//


public:
	CObj__CHM_OneManiFold();
	~CObj__CHM_OneManiFold();

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
