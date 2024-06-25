#pragma once

#include "Interface_Code.h"

#include "CCommon_System.h"
#include "CCommon_Error.h"

#include "CObj__LPx_STD__DEF.h"


class CObj__LPx_STD : public __IOBJ__STD_TYPE
{
private:
	CCommon_Error__USER_FNC mERROR__USER_FNC;

	//-------------------------------------------------------------------------
	CString sObject_Name;

	SCX__USER_LOG_CTRL xLOG_CTRL;
	
	int iActive_SIM;
	//

	//-------------------------------------------------------------------------
	// INTERNAL PROPERTY

	CX__VAR_DIGITAL_CTRL dCH__SLOT_STATUS[CFG_LPx__SLOT_MAX];
	CX__VAR_STRING_CTRL  sCH__SLOT_TITLE[CFG_LPx__SLOT_MAX];

	CX__VAR_STRING_CTRL  sCH__CLAMP_STS;
	CX__VAR_STRING_CTRL  sCH__FOUP_STS;
	CX__VAR_STRING_CTRL  sCH__FOUP_POS;
	CX__VAR_STRING_CTRL  sCH__DOOR_STS;
	CX__VAR_STRING_CTRL  sCH__TRANSFER_PIO_STATUS;

	CX__VAR_STRING_CTRL  sCH__CFG_BYPASS_READ_ID_FLAG;

	CX__VAR_STRING_CTRL  sCH__CFG_FA_MODE;
	CX__VAR_STRING_CTRL  sCH__CFG_FA_SERVICE_MODE;
	CX__VAR_STRING_CTRL  sCH__MON_ACTIVE_FA_AUTO;

	CX__VAR_STRING_CTRL  sCH__CFG_RELOAD_FLAG;
	CX__VAR_STRING_CTRL  sCH__MAP_SEQ_LOCK;

	// ...
	CX__VAR_DIGITAL_CTRL dCH__CFG_RF_EXIST_FLAG;	
	CX__VAR_STRING_CTRL  sCH__FOUP_CID_STRING;

	// ...
	CX__VAR_DIGITAL_CTRL dCH__CFG_PREPLOAD_CID_MODE;
	CX__VAR_DIGITAL_CTRL dCH__CFG_LOAD_MAP_MODE;
	CX__VAR_DIGITAL_CTRL dCH__CFG_LOAD_ACT_AUTO_MAP_MODE;
	//


	//-------------------------------------------------------------------------
	// EXTERNAL PROPERTY

	bool bOBJ_TYPE__DUAL;

	// LINK_CH : OBJ_STATE ...
	CX__VAR_DIGITAL_CTRL dEXT_CH__OBJ_STATE;

	// OBJ : LPx ...
	CII_EXT_OBJECT__CTRL *pLPx__OBJ_CTRL;

	CX__VAR_STRING_CTRL  sEXT_CH__LPx__CLAMP_STS;
	CX__VAR_STRING_CTRL  sEXT_CH__LPx__DOOR_STS;

	CX__VAR_STRING_CTRL  sEXT_CH__LPx__FOUP_STS;
	CX__VAR_STRING_CTRL  sEXT_CH__LPx__FOUP_POS;

	CX__VAR_STRING_CTRL  sEXT_CH__LPx__SLOT_MAP_RESULT[CFG_LPx__SLOT_MAX];

	CX__VAR_STRING_CTRL  sEXT_CH__TRANSFER_PIO_STATUS;
	CX__VAR_STRING_CTRL  sEXT_CH__RELOAD_FLAG;

	CX__VAR_STRING_CTRL  sEXT_CH__LPx__MON_ACTIVE_FA_AUTO;

	// OBJ : RFx ...
	bool bActive__RFx_OBJ;

	CII_EXT_OBJECT__CTRL *pRFx__OBJ_CTRL;

	CX__VAR_STRING_CTRL  sEXT_CH__RFx__CID_STRING;
	CX__VAR_STRING_CTRL  sEXT_CH__RFx__MON_ACTIVE_FA_AUTO;

	// ARM_RNE.INFO ...
	bool bActive__ROBOT_ARM_RNE_SNS;
	CX__VAR_DIGITAL_CTRL dEXT_CH__ROBOT_ARM_RNE_SNS;

	CString sDATA__RNE_ON;
	CString sDATA__RNE_OFF;
	//


	//------------------------------------------------------------------------------
	// INIT ----- 
	CString sMODE__INIT;
	int  Call__INIT(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	CString sMODE__HOME;
	int  Call__HOME(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	// DOOR OPEN -----
	CString sMODE__DOOR_OPEN;
	int  Call__DOOR_OPEN(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	// CHECK DOOR OPEN -----
	CString sMODE__CHECK_DOOR_OPEN;
	int  Call__CHECK_DOOR_OPEN(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	// LOAD -----
	CString sMODE__LOAD;
	int  Call__LOAD(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	// UNLOAD -----
	CString sMODE__UNLOAD;
	int  Call__UNLOAD(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	// PREPLOAD -----
	CString sMODE__PREPLOAD;
	int  Call__PREPLOAD(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	// RLSUNLOAD ----- 
	CString sMODE__RLSUNLOAD;
	int  Call__RLSUNLOAD(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	// CYCLE ...
	CString sMODE__CYCLE;
	int  Call__CYCLE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	// RF Reader/Writer ...
	CString sMODE__CID_READ;
	int  Call__CID_READ(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);
	int  Fnc__CID_READ(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	CString sMODE__PAGE_READ;
	int  Call__PAGE_READ(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	CString sMODE__CID_WRITE;
	int  Call__CID_WRITE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	CString sMODE__PAGE_WRITE;
	int  Call__PAGE_WRITE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	// MAP -----
	CString sMODE__MAP;
	int  Call__MAP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);
	int  Fnc__MAP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	int  Update__MAP_INFO(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	// CLAMP -----
	CString sMODE__CLAMP;
	int  Call__CLAMP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	// UNCLAMP -----
	CString sMODE__UNCLAMP;
	int  Call__UNCLAMP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	// EXTENDPADDLE -----
	CString sMODE__EXTENDPADDLE;
	int  Call__EXTENDPADDLE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	// RETRACTPADDLE -----
	CString sMODE__RETRACTPADDLE;
	int  Call__RETRACTPADDLE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	CString sMODE__PADDLEDOWN;
	int  Call__PADDLEDOWN(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	CString sMODE__PADDLEUP;
	int  Call__PADDLEUP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	// LATCH -----
	CString sMODE__LATCH;
	int  Call__LATCH(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	// UNLATCH -----
	CString sMODE__UNLATCH;
	int  Call__UNLATCH(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	// CLOSEDOOR -----
	CString sMODE__CLOSEDOOR;
	int  Call__CLOSEDOOR(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	// OPENDOOR -----
	CString sMODE__OPENDOOR;
	int  Call__OPENDOOR(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	CString sMODE__OPENDOOR_WITH_NO_UNLATCH;
	int  Call__OPENDOOR_WITH_NO_UNLATCH(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	// VACON -----
	CString sMODE__VACON;
	int  Call__VACON(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	// VACOFF -----
	CString sMODE__VACOFF;
	int  Call__VACOFF(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	// EXTENDSHUTTLE -----
	CString sMODE__EXTENDSHUTTLE;
	int  Call__EXTENDSHUTTLE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	// RETRACTSHUTTLE -----
	CString sMODE__RETRACTSHUTTLE;
	int  Call__RETRACTSHUTTLE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	// HALT -----
	CString sMODE__HALT;
	int  Call__HALT(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	// ...
	void Mon__ANI_MONITOR(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);
	void Mon__IO_MONITOR(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	// ...
	int  Alarm_Check__ARM_NOT_RETRACTED(CII_OBJECT__ALARM* p_alarm);
	//


public:
	CObj__LPx_STD();
	~CObj__LPx_STD();

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
