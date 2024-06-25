#pragma once

#include "Interface_Code.h"

#include "CCommon_System.h"
#include "CCommon_Error.h"


class CObj__VAC_ROBOT_EX : public __IOBJ__STD_TYPE
{
private:
	CCommon_Error__USER_FNC mERROR__USER_FNC;

	//-------------------------------------------------------------------------
	CString sObject_Name;

	SCX__USER_LOG_CTRL xLOG_CTRL;
	CString m_sBEFORE_MODE;

	int iSIM_MODE;
	int iSIM__PMC_DUMMY_MODE;


	//-------------------------------------------------------------------------
	// INTERNAL CHANNEL

	CX__VAR_STRING_CTRL  sCH__OBJ_MESSAGE;
	CX__VAR_DIGITAL_CTRL dCH__OBJ_STATUS;

	CX__VAR_DIGITAL_CTRL dCH__PARA_ARM;
	CX__VAR_DIGITAL_CTRL dCH__PARA_MODULE;
	CX__VAR_DIGITAL_CTRL dCH__PARA_SLOT;

	CX__VAR_STRING_CTRL  sCH__PARA_MANUAL_MOVE_LLx_SLOT_CLOSE_SKIP;
	CX__VAR_STRING_CTRL  sCH__PARA_MANUAL_MOVE_PMx_SLOT_CLOSE_SKIP;

	//
	CX__VAR_STRING_CTRL  sCH__FROM_CTC_WAC_PM_USE_X[CFG_PM_LIMIT];
	CX__VAR_STRING_CTRL  sCH__FROM_CTC_WAC_PM_PICK_CHECK_X[CFG_PM_LIMIT];
	CX__VAR_STRING_CTRL  sCH__FROM_CTC_WAC_PM_PLACE_CHECK_X[CFG_PM_LIMIT];
	CX__VAR_STRING_CTRL  sCH__FROM_CTC_WAC_PM_POS_ID_X[CFG_PM_LIMIT];
	CX__VAR_STRING_CTRL  sCH__FROM_CTC_WAC_PM_POS_SLOT_X[CFG_PM_LIMIT];
	CX__VAR_STRING_CTRL  sCH__FROM_CTC_WAC_PM_DELAY_SEC_X[CFG_PM_LIMIT];
	CX__VAR_STRING_CTRL  sCH__FROM_CTC_WAC_PM_WAFER_TYPE;

	CX__VAR_STRING_CTRL  sCH__INFO_WAC_PMC_ACTIVE;
	CX__VAR_STRING_CTRL  sCH__INFO_WAC_PMC_STATE;
	CX__VAR_STRING_CTRL  sCH__INFO_WAC_PMC_ARM_TYPE;
	CX__VAR_STRING_CTRL  sCH__INFO_WAC_PMC_WFR_INFO;
	CX__VAR_STRING_CTRL  sCH__INFO_WAC_PMC_POS_ID;
	CX__VAR_STRING_CTRL  sCH__INFO_WAC_PMC_POS_SLOT;
	CX__VAR_STRING_CTRL  sCH__INFO_WAC_PMC_DELAY_COUNT;

	//
	CX__VAR_DIGITAL_CTRL dCH__CFG_LLx_CLOSE_SEQUENCE_MODE;
	CX__VAR_DIGITAL_CTRL dCH__CFG_PMx_CLOSE_SEQUENCE_MODE;

	// CFG
	CX__VAR_DIGITAL_CTRL dCH__CFG_LLx_HANDSHAKE_MODE[CFG_LLx_LIMIT];
	CX__VAR_DIGITAL_CTRL dCH__CFG_LLx_ROBOT_Z_MOVE[CFG_LLx_LIMIT];
	//


	//-------------------------------------------------------------------------
	// EXTERNAL CHANNEL

	// DB_CFG -----
	CX__VAR_DIGITAL_CTRL dEXT_CH__SCH_TEST_CFG_PMC_DUMMY_BY_CTC;

	CX__VAR_DIGITAL_CTRL dEXT_CH__CFG_SETUP_TEST_MODE;
	CX__VAR_DIGITAL_CTRL dEXT_CH__CFG_SETUP_AUTO_VENTING_PUMPING;
	CX__VAR_DIGITAL_CTRL dEXT_CH__CFG_SETUP_LLx_DOOR_SLOT_VLV_CONTROL;

	CX__VAR_DIGITAL_CTRL dEXT_CH__CFG_TRANSFER_MODE;
	CX__VAR_ANALOG_CTRL  aEXT_CH__CFG_REF_ATM_PRESSURE;
	CX__VAR_ANALOG_CTRL  aEXT_CH__CFG_REF_VAC_PRESSURE;

	CX__VAR_STRING_CTRL  sEXT_CH__CTC_CUR_MODE;

	CX__VAR_DIGITAL_CTRL dEXT_CH__CFG_PMx_CHM_TYPE[CFG_PM_LIMIT];

	// VAC_ROBOT -----
	CII_EXT_OBJECT__CTRL *pROBOT__OBJ_CTRL;

	CX__VAR_DIGITAL_CTRL dEXT_CH__PHY_ROBOT__MON_ACT_ARM;
	CX__VAR_DIGITAL_CTRL dEXT_CH__PHY_ROBOT__MON_TRG_ROT;

	CX__VAR_DIGITAL_CTRL dEXT_CH__PHY_ROBOT__ARM_A_MATERIAL_STATUS;
	CX__VAR_DIGITAL_CTRL dEXT_CH__PHY_ROBOT__ARM_B_MATERIAL_STATUS;
	CX__VAR_STRING_CTRL  sEXT_CH__PHY_ROBOT__ARM_A_MATERIAL_TITLE;
	CX__VAR_STRING_CTRL  sEXT_CH__PHY_ROBOT__ARM_B_MATERIAL_TITLE;

	CX__VAR_DIGITAL_CTRL dEXT_CH__PHY_ROBOT__PARA_ACTIVE_HANDOFF_REQ;
	CX__VAR_DIGITAL_CTRL dEXT_CH__PHY_ROBOT__PARA_ACTIVE_HANDOFF_ACT;

	// PMx ...
	int iSIZE__PMx;

	bool bActive__Single_Handoff;
	bool bActive__PMx_Passive_Use;

	CX__VAR_ANALOG_CTRL  aEXT_CH__PARA_PMC_ID;
	CII_EXT_OBJECT__CTRL *pOPR_PMx_HANDOFF_CTRL;
	CII_EXT_OBJECT__CTRL *pOPR_PMC_HANDOFF_X[CFG_PM_LIMIT];

	CX__VAR_DIGITAL_CTRL dEXT_CH__PMx_CFG_HANDSHAKE_MODE[CFG_PM_LIMIT];

	CX__VAR_DIGITAL_CTRL dEXT_CH__PMx_CFG_USE_SHUTTER[CFG_PM_LIMIT];
	CX__VAR_DIGITAL_CTRL dEXT_CH__PMx_CFG_USE_LIFT_PIN[CFG_PM_LIMIT];
	CX__VAR_DIGITAL_CTRL dEXT_CH__PMx_CFG_USE_CR_POSITION[CFG_PM_LIMIT];

	CX__VAR_STRING_CTRL  sEXT_CH__PMx_SHUTTER_STS[CFG_PM_LIMIT];
	CX__VAR_STRING_CTRL  sEXT_CH__PMx_LIFT_PIN_POS_STS[CFG_PM_LIMIT];
	CX__VAR_STRING_CTRL  sEXT_CH__PMx_CR_POSITION_STS[CFG_PM_LIMIT];

	CX__VAR_DIGITAL_CTRL dEXT_CH__PHY_PMx_SLOT01_STATUS[CFG_PM_LIMIT];
	CX__VAR_STRING_CTRL  sEXT_CH__PHY_PMx_SLOT01_TITLE[CFG_PM_LIMIT];

	// VAC_CHM ...
	CII_EXT_OBJECT__CTRL *pVAC_CHM__OBJ_CTRL;

	CX__VAR_DIGITAL_CTRL dEXT_CH__PARA_BALLAST_CTRL_ACTIVE;

	CX__VAR_STRING_CTRL  sEXT_CH__VAC_CHM__PRESSURE_STATUS;
	CX__VAR_STRING_CTRL  sEXT_CH__ROBOT_MATERIAL_TRANSFER_FLAG;

	// LLx ...
	bool bActive__LLx_MANIFOLD_X;

	int iSIZE__LLx;
	CII_EXT_OBJECT__CTRL *pLLx__OBJ_CTRL[CFG_LLx_LIMIT];

	CX__VAR_ANALOG_CTRL  aEXT_CH__LLx__PARA_SLOT_ID[CFG_LLx_LIMIT];
	CX__VAR_ANALOG_CTRL  aEXT_CH__LLx__PARA_HANDSHAKE_SLOT[CFG_LLx_LIMIT];

	CX__VAR_STRING_CTRL  sEXT_CH__LLx__PRESSURE_STATUS[CFG_LLx_LIMIT];

	CX__VAR_STRING_CTRL  sEXT_CH__LLx__ROBOT_TRANSFER_FLAG[CFG_LLx_LIMIT];

	CX__VAR_DIGITAL_CTRL dEXT_CH__LLx__SLOTx_STATUS[CFG_LLx_LIMIT][CFG_LLx_SLOT_LIMIT];
	CX__VAR_STRING_CTRL  sEXT_CH__LLx__SLOTx_TITLE[CFG_LLx_LIMIT][CFG_LLx_SLOT_LIMIT];

	// TMC_VLV ...
	CII_EXT_OBJECT__CTRL *pTMC_VLV__OBJ_CTRL;

	CX__VAR_ANALOG_CTRL  aEXT_CH__TMC_VLV__PMC_ID;
	//


	//------------------------------------------------------------------------------
	// Home -----
	CString sMODE__HOME;
	int  Call__HOME(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm);

	// Map -----
	CString sMODE__MAP;
	int  Call__MAP(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm);

	// ...
	CString sMODE__INIT;
	int  Call__INIT(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm);

	CString sMODE__SET_PARA;
	int  Call__SET_PARA(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm);

	//
	CString sMODE__PICK;
	int  Call__PICK(CII_OBJECT__VARIABLE* p_variable,
					CII_OBJECT__ALARM* p_alarm,
					const CString& para_arm,
					const CString& para_module,
					const CString& para_slot,
					const int handoff_mode);

	CString sMODE__XPICK;
	int  Call__XPICK(CII_OBJECT__VARIABLE* p_variable,
					CII_OBJECT__ALARM* p_alarm,
					const CString& para_arm,
					const CString& para_module,
					const CString& para_slot,
					const int handoff_mode);

	int  Seq__PICK(CII_OBJECT__VARIABLE* p_variable,
					CII_OBJECT__ALARM* p_alarm,
					const CString& para_arm,
					const CString& para_module,
					const CString& para_slot,
					const int ex_flag,
					const int handoff_mode);

	int  Fnc__LBx_PICK(CII_OBJECT__VARIABLE* p_variable,
						CII_OBJECT__ALARM* p_alarm,
						const CString& para_arm,
						const CString& para_module,
						const CString& para_slot,
						const int ex_flag);
	int  Fnc__PMx_PICK(CII_OBJECT__VARIABLE* p_variable,
						CII_OBJECT__ALARM* p_alarm,
						const CString& para_arm,
						const CString& para_module,
						const CString& para_slot,
						const int ex_flag,
						const int handoff_mode);

	//
	CString sMODE__PLACE;
	int  Call__PLACE(CII_OBJECT__VARIABLE* p_variable,
					 CII_OBJECT__ALARM* p_alarm,
					 const CString& para_arm,
					 const CString& para_module,
					 const CString& para_slot,
					 const int handoff_mode);

	CString sMODE__XPLACE;
	int  Call__XPLACE(CII_OBJECT__VARIABLE* p_variable,
					  CII_OBJECT__ALARM* p_alarm,
					  const CString& para_arm,
					  const CString& para_module,
					  const CString& para_slot,
					  const int handoff_mode);

	int  Seq__PLACE(CII_OBJECT__VARIABLE* p_variable,
				    CII_OBJECT__ALARM* p_alarm,
				    const CString& para_arm,
				    const CString& para_module,
				    const CString& para_slot,
				    const int ex_flag,
				    const int handoff_mode);

	int  Fnc__LBx_PLACE(CII_OBJECT__VARIABLE* p_variable,
						CII_OBJECT__ALARM* p_alarm,
						const CString& para_arm,
						const CString& para_module,
						const CString& para_slot,
						const int ex_flag);
	int  Fnc__PMx_PLACE(CII_OBJECT__VARIABLE* p_variable,
						CII_OBJECT__ALARM* p_alarm,
						const CString& para_arm,
						const CString& para_module,
						const CString& para_slot,
						const int ex_flag,
						const int handoff_mode);

	//
	int  Fnc__WAC_ACT(CII_OBJECT__VARIABLE* p_variable,
					  CII_OBJECT__ALARM* p_alarm,
					  const CString& wac_arm,
					  const CString& para_arm,
					  const CString& para_module,
					  const CString& para_slot);

	int  _Fnc__WAC_ACT(CII_OBJECT__VARIABLE* p_variable,
					   CII_OBJECT__ALARM* p_alarm,
					   const CString& wac_arm,
					   const CString& para_arm,
					   const CString& para_module,
					   const CString& para_slot);

	//
	CString sMODE__ROTATE;
	int  Call__ROTATE(CII_OBJECT__VARIABLE* p_variable,
					  CII_OBJECT__ALARM* p_alarm,
					  const CString& para_arm,
					  const CString& para_module,
					  const CString& para_slot);

	// RETRACT -----
	CString sMODE__RETRACT;
	int  Call__RETRACT(CII_OBJECT__VARIABLE* p_variable,
					   CII_OBJECT__ALARM* p_alarm,
					   const CString& arm_type,
					   const CString& stn_name, 
					   const CString& stn_slot);

	// EXTEND -----
	CString sMODE__EXTEND;
	int  Call__EXTEND(CII_OBJECT__VARIABLE* p_variable,
					  CII_OBJECT__ALARM* p_alarm,
					  const CString& arm_type,
				      const CString& stn_name, 
					  const CString& stn_slot);

	// GOUP -----
	CString sMODE__GOUP;
	int  Call__GOUP(CII_OBJECT__VARIABLE* p_variable,
					CII_OBJECT__ALARM* p_alarm,
					const CString& arm_type,
					const CString& stn_name, 
					const CString& stn_slot);

	// GODOWN -----
	CString sMODE__GODOWN;
	int  Call__GODOWN(CII_OBJECT__VARIABLE* p_variable,
					  CII_OBJECT__ALARM* p_alarm,
					  const CString& arm_type,
					  const CString& stn_name, 
					  const CString& stn_slot);

	// -----
	CString sMODE__RQ_STN;
	int Call__RQ_STN(CII_OBJECT__VARIABLE* p_variable,
					 CII_OBJECT__ALARM* p_alarm,
					 const CString& arm_type,
					 const CString& stn_name);

	// -----
	CString sMODE__SET_STN;
	int Call__SET_STN(CII_OBJECT__VARIABLE* p_variable,
					  CII_OBJECT__ALARM* p_alarm,
					  const CString& arm_type,
					  const CString& stn_name);

	CString sMODE__TEACHED_CPTR_SAVE;
	int Call__TEACHED_CPTR_SAVE(CII_OBJECT__VARIABLE* p_variable,
								CII_OBJECT__ALARM* p_alarm,
								const CString& arm_type,
								const CString& stn_name);

	// ...
	int  Fnc__CALL_ROTATE(CII_OBJECT__VARIABLE* p_variable,
						  CII_OBJECT__ALARM* p_alarm,
						  const CString& para_arm,
						  const CString& para_module,
						  const CString& para_slot);
	int  Fnc__RUN_ROTATE(CII_OBJECT__VARIABLE* p_variable,
						 CII_OBJECT__ALARM* p_alarm,
						 const CString& para_arm,
						 const CString& para_module,
						 const CString& para_slot);

	int Fnc__ACT_HOFF_PICK(CII_OBJECT__VARIABLE* p_variable,
							CII_OBJECT__ALARM* p_alarm,
							const CString& para_arm,
							const CString& para_module,
							const CString& para_slot);

	int Fnc__ACT_HOFF_PLACE(CII_OBJECT__VARIABLE* p_variable,
							CII_OBJECT__ALARM* p_alarm,
							const CString& para_arm,
							const CString& para_module,
							const CString& para_slot);

	int Is__TRANSFER_READY_TO_PICK(CII_OBJECT__ALARM* p_alarm, 
									const CString str_pick_place,
									const CString str_ext_ret,
									const int pm_index);

	int Is__PIN_UP_STS(CII_OBJECT__ALARM* p_alarm, 
						const CString str_pick_place,
						const CString str_ext_ret,
						const int pm_index);

	int Is__TRANSFER_READY_TO_PLACE(CII_OBJECT__ALARM* p_alarm, 
									const bool active__active_handoff,
									const CString str_pick_place,
									const CString str_ext_ret,
									const int pm_index);

	int Is__PIN_DOWN_OR_MIDDLE_STS(CII_OBJECT__ALARM* p_alarm, 
								   const CString str_pick_place,
								   const CString str_ext_ret,
								   const int pm_index);

	int  Fnc__CALL_PICK(CII_OBJECT__VARIABLE* p_variable,
						CII_OBJECT__ALARM* p_alarm,
						const CString& para_arm,
						const CString& para_module,
						const CString& para_slot);

	int  Fnc__CALL_PLACE(CII_OBJECT__VARIABLE* p_variable,
						 CII_OBJECT__ALARM* p_alarm,
						 const CString& para_arm,
						 const CString& para_module,
						 const CString& para_slot);

	//
	int  Fnc__PASSIVE_TRANSFER_READY(const int pm_index);
	int  Fnc__PASSIVE_TRANSFER_END(const int pm_index);

	//
	int  Sub_OBJCALL_PMC_ACT_HANDOFF(const int pmc_id, const CString obj_mode);
	int  Sub_OBJRUN_PMC_ACT_HANDOFF(const int pmc_id, const CString obj_mode);

	int  Interlock__CHECK_MATERIAL(CII_OBJECT__ALARM* p_alarm,
									const int place_flag,
									const CString& arm_type,
									const CString& stn_name,
									const CString& stn_slot);
	int  _Interlock__CHECK_MATERIAL(CII_OBJECT__ALARM* p_alarm,
									const int place_flag,
									const CString& arm_type,
									const CString& stn_name,
									const CString& stn_slot);

	int Fnc__CHANGE_MATERIAL_INFO(const int place_flag,
									const CString& arm_type,
									const CString& stn_name,
									const CString& stn_slot);
	int _Fnc__CHANGE_MATERIAL_INFO(const int place_flag,
									const CString& arm_type,
									const CString& stn_name,
									const CString& stn_slot);

	//
	int  Fnc__PMx_SV_CLOSE(const int pmc_id);
	int  Run__PMx_SV_CLOSE(const int pmc_id);

	int  Fnc__PMx_SV_TRANSFER_OPEN(const int pmc_id);
	int  Fnc__Run_PMx_SV_TRANSFER_OPEN(const int pmc_id);
	int  Fnc__Wait_PMx_SV_TRANSFER_OPEN(const int pmc_id);
	int  Fnc__PMx_SV_CLOSE_CHECK(CII_OBJECT__ALARM* p_alarm);

	//
	int  Fnc__TRANSFER_AUTO_PUMPING(const CString& arm_type, const CString& stn_name, const CString& stn_slot);

	int  Fnc__RUN_SV_OPEN( const CString& arm_type, const CString& stn_name, const CString& stn_slot);
	int  Fnc__WAIT_SV_OPEN(const CString& arm_type, const CString& stn_name, const CString& stn_slot);
	int  Fnc__CALL_SV_OPEN(const CString& arm_type, const CString& stn_name, const CString& stn_slot);

	//
	void Fnc__APP_LOG(const CString& log_msg);

	void _Get__ARM_INFO(const CString& arm_type, 
						const CString& stn_name, 
						const CString& stn_slot, 
					    CStringArray& l__arm_type, 
						CStringArray& l__stn_name, 
						CStringArray& l__stn_slot);
	//

public:
	CObj__VAC_ROBOT_EX();
	~CObj__VAC_ROBOT_EX();

	//-------------------------------------------------------------------------
	int __DEFINE__CONTROL_MODE(obj,l_mode);
	int __DEFINE__VERSION_HISTORY(version);

	int __DEFINE__VARIABLE_STD(p_variable);
	int __DEFINE__ALARM(p_alarm);

	int __Define__USER_FUNCTION(CII_DEFINE__FUNCTION *p_fnc_ctrl);

	//-------------------------------------------------------------------------
	int __INITIALIZE__OBJECT(p_variable,p_ext_obj_create);

	//-------------------------------------------------------------------------
	int __CALL__CONTROL_MODE(mode,p_debug,p_variable,p_alarm);
	int __CALL__MONITORING(id,p_variable,p_alarm);
};
