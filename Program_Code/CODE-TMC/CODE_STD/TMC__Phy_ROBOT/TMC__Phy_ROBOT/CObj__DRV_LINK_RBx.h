#pragma once

#include "Interface_Code.h"

#include "CCommon_Error.h"
#include "CObj__DRV_LINK_RBx__DEF.h"

#include "CMng__Error_Code.h"


class CObj__DRV_LINK_RBx : public __IOBJ__STD_TYPE
{
private:
	//-------------------------------------------------------------------------
	CString	sObject_Name;

	CCommon_Error__USER_FNC mERROR__USER_FNC;

	// ..
	SCX__USER_LOG_CTRL xAPP_LOG_CTRL;

	int iActive__SIM_MODE;
	int iCFG__SIM_MODE;

	CMng__Error_Code mMNG__ERR_CODE;
	//


	//-------------------------------------------------------------------------
	//  INTERNAL PROPERTY

	// 
	CX__VAR_STRING_CTRL  sCH__CUR_CTRL_MODE;

	// PARA ...
	CX__VAR_DIGITAL_CTRL dCH__PARA_STN_NAME;
	CX__VAR_DIGITAL_CTRL dCH__PARA_STN_SLOT;
	CX__VAR_DIGITAL_CTRL dCH__PARA_ARM_TYPE;
	CX__VAR_ANALOG_CTRL  aCH__PARA_AL1_CCD_POS;

	// CFG ...
	CX__VAR_DIGITAL_CTRL dCH__CFG_ACTION_CONFIRM_FLAG;

	// CFG ...
	CX__VAR_DIGITAL_CTRL dCH__SIM_CFG__REAL_TEST;

	CX__VAR_ANALOG_CTRL  aCH__SIM_CFG__ROTATE_TIME;

	CX__VAR_ANALOG_CTRL  aCH__SIM_CFG__LPx_PICK_TIME;
	CX__VAR_ANALOG_CTRL  aCH__SIM_CFG__LPx_PLACE_TIME;

	CX__VAR_ANALOG_CTRL  aCH__SIM_CFG__ALx_PICK_TIME;
	CX__VAR_ANALOG_CTRL  aCH__SIM_CFG__ALx_PLACE_TIME;

	CX__VAR_ANALOG_CTRL  aCH__SIM_CFG__LBx_PICK_TIME;
	CX__VAR_ANALOG_CTRL  aCH__SIM_CFG__LBx_PLACE_TIME;

	CX__VAR_ANALOG_CTRL  aCH__SIM_CFG__BUFFx_PICK_TIME;
	CX__VAR_ANALOG_CTRL  aCH__SIM_CFG__BUFFx_PLACE_TIME;

	// MATERIAL ...
	CX__VAR_DIGITAL_CTRL dCH__MON__ARM_A_MATERIAL_STATUS;
	CX__VAR_DIGITAL_CTRL dCH__MON__ARM_B_MATERIAL_STATUS;

	CX__VAR_STRING_CTRL  sCH__MON__ARM_A_MATERIAL_TITLE;
	CX__VAR_STRING_CTRL  sCH__MON__ARM_B_MATERIAL_TITLE;

	// ANIMATION ...
	CX__VAR_DIGITAL_CTRL dCH__MON__ACT_ARM;
	CX__VAR_DIGITAL_CTRL dCH__MON__ARM_A_ACT;
	CX__VAR_DIGITAL_CTRL dCH__MON__ARM_B_ACT;
	CX__VAR_DIGITAL_CTRL dCH__MON__TRG_MOVE;
	CX__VAR_DIGITAL_CTRL dCH__MON__TRG_ROTATE;
	//


	//-------------------------------------------------------------------------
	//  EXTERNAL PROPERTY

	// OBJ : DB ...
	CX__VAR_DIGITAL_CTRL dEXT_CH__CFG_SIM_MODE;

	// OBJ : ATM_DRV ...
	CII_EXT_OBJECT__CTRL* pOBJ_CTRL__ATM_DRV;	

	CX__VAR_DIGITAL_CTRL dEXT_CH__ROBOT_PARA_CMMD;
	CX__VAR_STRING_CTRL  sEXT_CH__ROBOT_CMMD_STS;
	//

	
	//-------------------------------------------------------------------------
	CString sMODE__INIT;
	int  Call__INIT(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	CString sMODE__HOME;
	int  Call__HOME(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	CString sMODE__ROTATE;
	int  Call__ROTATE(CII_OBJECT__VARIABLE* p_variable,
					  CII_OBJECT__ALARM* p_alarm,
					  const CString& arm_type,
					  const CString& stn_name,
					  const CString& stn_slot);

	CString sMODE__PICK;
	int  Call__PICK(CII_OBJECT__VARIABLE* p_variable,
					CII_OBJECT__ALARM* p_alarm,
					const CString& arm_type,
					const CString& stn_name,
					const CString& stn_slot);

	CString sMODE__PLACE;
	int  Call__PLACE(CII_OBJECT__VARIABLE* p_variable,
					 CII_OBJECT__ALARM* p_alarm,
					 const CString& arm_type,
					 const CString& stn_name,
					 const CString& stn_slot);

	CString sMODE__WAFER_CHECK_ARM;
	int  Call__WAFER_CHECK_ARM(CII_OBJECT__VARIABLE* p_variable,
		                       CII_OBJECT__ALARM* p_alarm);

	int  Fnc__CMMD_WAIT(const CString& set_cmmd,
						const CString& arm_type,
						const CString& stn_name,
						const CString& stn_slot);

	int  Fnc__Check_LPx(const CString& para_stn);
	int  Fnc__Check_LBx(const CString& para_stn);
	int  Fnc__Check_ALx(const CString& para_stn);
	int  Fnc__Check_BUFFx(const CString& para_stn);

	// ...
	void Mon__IO_MONITOR(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	// ...
	void Fnc__APP_LOG(const CString& log_msg);
	//


public:
	CObj__DRV_LINK_RBx();
	~CObj__DRV_LINK_RBx();

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
