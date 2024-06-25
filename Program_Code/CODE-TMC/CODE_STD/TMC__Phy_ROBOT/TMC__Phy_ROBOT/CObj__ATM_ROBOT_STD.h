#pragma once

#include "Interface_Code.h"

#include "CCommon_Error.h"
#include "CCommon_System.h"


class CObj__ATM_ROBOT_STD : public __IOBJ__STD_TYPE
{
private:
	CCommon_Error__USER_FNC mERROR__USER_FNC;

	//-------------------------------------------------------------------------
	CString	sObject_Name;

	SCX__USER_LOG_CTRL xAPP_LOG_CTRL;
	int iFlag__APP_LOG;

	int iActive_SIM;
	//

	//-------------------------------------------------------------------------
	//  INTERNAL PROPERTY

	CX__VAR_STRING_CTRL  sCH__OBJ_STATUS;

	CX__VAR_STRING_CTRL  sCH__MODULE_LINK_TARGT_LLx_NAME;
	CX__VAR_DIGITAL_CTRL dCH__MAINT_TARGT_LLx_NAME;

	// PARA CHANNEL
	CX__VAR_DIGITAL_CTRL dCH__OTR_IN_PARA__STN_NAME;
	CX__VAR_DIGITAL_CTRL dCH__OTR_IN_PARA__STN_SLOT;
	CX__VAR_DIGITAL_CTRL dCH__OTR_IN_PARA__ARM_TYPE;

	// CFG ...
	CX__VAR_DIGITAL_CTRL dCH__CFG_MAPPING_ACTION_SLIDE_OUT_CHECK;

	// MATERIAL CHANNEL
	CX__VAR_DIGITAL_CTRL dCH__OTR_OUT_MON__ARM_A_MATERIAL_STATUS;
	CX__VAR_DIGITAL_CTRL dCH__OTR_OUT_MON__ARM_B_MATERIAL_STATUS;

	CX__VAR_STRING_CTRL  sCH__OTR_OUT_MON__ARM_A_MATERIAL_TITLE;
	CX__VAR_STRING_CTRL  sCH__OTR_OUT_MON__ARM_B_MATERIAL_TITLE;

	// ANIMATION CHANNEL
	CX__VAR_DIGITAL_CTRL dCH__OTR_OUT_MON__ACT_ARM;
	CX__VAR_DIGITAL_CTRL dCH__OTR_OUT_MON__ARM_A_ACT;
	CX__VAR_DIGITAL_CTRL dCH__OTR_OUT_MON__ARM_B_ACT;
	CX__VAR_DIGITAL_CTRL dCH__OTR_OUT_MON__TRG_MOVE;
	CX__VAR_DIGITAL_CTRL dCH__OTR_OUT_MON__TRG_ROTATE;

	CX__VAR_DIGITAL_CTRL dCH__CFG__WAFER_PRESENT_CHECK;
	CX__VAR_DIGITAL_CTRL dCH__CFG__WAFER_CROSSED_CHECK_AFTER_MAPPING;
	//


	//-------------------------------------------------------------------------
	//  EXTERNAL PROPERTY

	// DB_CFG ...
	CX__VAR_DIGITAL_CTRL dEXT_CH__CFG_LPx_EXIST_FLAG[CFG_LPx__SIZE];
	CX__VAR_DIGITAL_CTRL dEXT_CH__CFG_LPx_CLOSE_MOVE_AFTER_MAPPING[CFG_LPx__SIZE];

	CX__VAR_DIGITAL_CTRL dEXT_CH__CFG_ALIGN_DEVICE;
	CX__VAR_DIGITAL_CTRL dEXT_CH__CFG_AL1_EXIST_FLAG;
	CX__VAR_DIGITAL_CTRL dEXT_CH__CFG_AL1_SLOT_MAX;

	CX__VAR_DIGITAL_CTRL dEXT_CH__CFG_LLx_EXIST_FLAG[CFG_LLx__SIZE];
	CX__VAR_DIGITAL_CTRL dEXT_CH__CFG_LLx_SLOT_MAX[CFG_LLx__SIZE];

	CX__VAR_DIGITAL_CTRL dEXT_CH__CFG_VIS1_EIXST_FLAG;
		
	CX__VAR_DIGITAL_CTRL dEXT_CH__CFG_BUFF1_EXIST_FLAG;
	CX__VAR_DIGITAL_CTRL dEXT_CH__CFG_BUFF2_EXIST_FLAG;
	CX__VAR_DIGITAL_CTRL dEXT_CH__CFG_FULL_BUFF_EXIST_FLAG;

	// LLx ...
	CX__VAR_ANALOG_CTRL  aEXT_CH__CFG_LLx_ALIGN_ANGLE[CFG_LLx__SIZE];
	CX__VAR_ANALOG_CTRL  aEXT_CH__CFG_LLx_POST_POSITION_INCREMENT[CFG_LLx__SIZE];
	CX__VAR_ANALOG_CTRL  aEXT_CH__CFG_LLx_POST_POSITION_INCREMENT_RANGE[CFG_LLx__SIZE];
	CX__VAR_ANALOG_CTRL  aEXT_CH__CFG_LLx_POST_POSITION_INCREMENT_START_ANGLE[CFG_LLx__SIZE];
	CX__VAR_DIGITAL_CTRL dEXT_CH__CFG_LLx_POST_POSITION_INCREMENT_APPLY[CFG_LLx__SIZE];
	CX__VAR_ANALOG_CTRL  aEXT_CH__CUR_LLx_POST_POSITION_INCREMENT_ANGLE[CFG_LLx__SIZE];

	// IO ...
	CX__VAR_DIGITAL_CTRL dEXT_CH__IO_EFEM_DOOR_STS;

	// ROBOT ...
	CII_EXT_OBJECT__CTRL *pROBOT__OBJ_CTRL;

	CX__VAR_DIGITAL_CTRL dEXT_CH__ROBOT_ARM_A_MATERIAL_STATUS;
	CX__VAR_DIGITAL_CTRL dEXT_CH__ROBOT_ARM_B_MATERIAL_STATUS;
	CX__VAR_DIGITAL_CTRL dEXT_CH__ROBOT_MAP_RESULT[CFG_LPx__SLOT_SIZE];

	CX__VAR_ANALOG_CTRL  aEXT_CH__ALIGNER_ANGLE_PARA;

	// IO : ARM_RNE_SNS ... 
	bool bActive__ROBOT_ARM_RNE_SNS;
	CX__VAR_DIGITAL_CTRL dEXT_CH__ROBOT_ARM_RNE_SNS;
	//

	// AL1 -----
	CX__VAR_DIGITAL_CTRL dEXT_CH__AL1_SLOT01_STATUS;
	CX__VAR_DIGITAL_CTRL dEXT_CH__AL1_SLOT02_STATUS;

	CX__VAR_STRING_CTRL  sEXT_CH__AL1_SLOT01_TITLE;
	CX__VAR_STRING_CTRL  sEXT_CH__AL1_SLOT02_TITLE;

	// LLx -----
	int iLLx_SIZE;

	CX__VAR_DIGITAL_CTRL dEXT_CH__LLx_SLOT_STATUS[CFG_LLx__SIZE][CFG_LLx__SLOT_SIZE];
	CX__VAR_STRING_CTRL  sEXT_CH__LLx_SLOT_TITLE[CFG_LLx__SIZE][CFG_LLx__SLOT_SIZE];

	CX__VAR_DIGITAL_CTRL dEXT_CH__LLx_DOOR_STATUS[CFG_LLx__SIZE];

	//
	bool bActive__LLx_ARM_RNE_SNS[CFG_LLx__SIZE];
	CX__VAR_DIGITAL_CTRL dEXT_CH__LLx_ARM_RNE_SNS[CFG_LLx__SIZE];

	CString sDATA__LLx_ARM_RNE_OFF_STS[CFG_LLx__SIZE];
	CString sDATA__LLx_ARM_RNE_ON_STS[CFG_LLx__SIZE];

	//
	bool bActive__LLx_LOAD_ENABLE_SNS[CFG_LLx__SIZE];
	CX__VAR_DIGITAL_CTRL dEXT_CH__LLx_LOAD_ENABLE_SNS[CFG_LLx__SIZE];
	
	CString sDATA__LLx_LOAD_ENABLE_OFF_STS[CFG_LLx__SIZE];
	CString sDATA__LLx_LOAD_ENABLE_ON_STS[CFG_LLx__SIZE];

	// LPx -----
	int iLPx_SIZE;

	CII_EXT_OBJECT__CTRL *pLPx__OBJ_CTRL[CFG_LPx__SIZE];

	CX__VAR_DIGITAL_CTRL dEXT_CH__LPx_SLOT_STATUS[CFG_LPx__SIZE][CFG_LPx__SLOT_SIZE];
	CX__VAR_STRING_CTRL  sEXT_CH__LPx_SLOT_TITLE[CFG_LPx__SIZE][CFG_LPx__SLOT_SIZE];

	CX__VAR_DIGITAL_CTRL dEXT_CH__LPx_DOOR_STATUS[CFG_LPx__SIZE];
	CX__VAR_DIGITAL_CTRL dEXT_CH__LPx_FOUP_EXIST_FLAG[CFG_LPx__SIZE];
	CX__VAR_DIGITAL_CTRL dEXT_CH__LPx_FOUP_CLAMP_STATUS[CFG_LPx__SIZE];
	CX__VAR_DIGITAL_CTRL dEXT_CH__LPx_FOUP_POS_STATUS[CFG_LPx__SIZE];
	CX__VAR_DIGITAL_CTRL dEXT_CH__LPx_SLOT_MAP_STATUS[CFG_LPx__SIZE][CFG_LPx__SLOT_SIZE];

	CX__VAR_STRING_CTRL  sEXT_CH__LPx_MAP_SEQ_LOCK[CFG_LPx__SIZE];

	bool bActive__LPx_WAFER_SLIDE_OUT_SNS[CFG_LPx__SIZE];
	CX__VAR_DIGITAL_CTRL dEXT_CH__LPx_WAFER_SLIDE_OUT_SNS[CFG_LPx__SIZE];
	//

	// SIDE STORAGE1 ...
	CX__VAR_DIGITAL_CTRL dEXT_CH__SIDE_STORAGE1_SLOT_STATUS[CFG_LPx__SLOT_SIZE];
	CX__VAR_STRING_CTRL  sEXT_CH__SIDE_STORAGE1_SLOT_TITLE[CFG_LPx__SLOT_SIZE];

	// SIDE STORAGE2 ...
	CX__VAR_DIGITAL_CTRL dEXT_CH__SIDE_STORAGE2_SLOT_STATUS[CFG_LPx__SLOT_SIZE];
	CX__VAR_STRING_CTRL  sEXT_CH__SIDE_STORAGE2_SLOT_TITLE[CFG_LPx__SLOT_SIZE];
	//

	//-------------------------------------------------------------------------
	// Init -----
	CString sMODE__INIT;
	int  Call__INIT(CII_OBJECT__VARIABLE* p_variable,
					CII_OBJECT__ALARM* p_alarm);

	// Pick -----
	CString sMODE__PICK;
	int  Call__PICK(CII_OBJECT__VARIABLE* p_variable,
					CII_OBJECT__ALARM* p_alarm,
					const CString& arm_type,
					const CString& stn_name,
					const CString& stn_slot);

	// Place -----
	CString sMODE__PLACE;
	int  Call__PLACE(CII_OBJECT__VARIABLE* p_variable,
					CII_OBJECT__ALARM* p_alarm,
					const CString& arm_type,
					const CString& stn_name,
					const CString& stn_slot);

	// Rotate -----
	CString sMODE__ROTATE;
	int  Call__ROTATE(CII_OBJECT__VARIABLE* p_variable,
					CII_OBJECT__ALARM* p_alarm,
					const CString& arm_type,
					const CString& stn_name,
					const CString& stn_slot);

	// MAP -----
	CString sMODE__MAP;
	int  Call__MAP(CII_OBJECT__VARIABLE* p_variable,
					CII_OBJECT__ALARM* p_alarm);

	// HOME -----
	CString sMODE__HOME;
	CString sMODE__VACOFF_HOME;
	int  Call__HOME(CII_OBJECT__VARIABLE* p_variable,
					CII_OBJECT__ALARM* p_alarm,
					const int req__vac_off);

	// VACON -----
	CString sMODE__VACON;
	int  Call__VACON(CII_OBJECT__VARIABLE* p_variable,
					CII_OBJECT__ALARM* p_alarm,
					const CString& arm_type);

	// VACOFF -----
	CString sMODE__VACOFF;
	int  Call__VACOFF(CII_OBJECT__VARIABLE* p_variable,
					CII_OBJECT__ALARM* p_alarm,
					const CString& arm_type);

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

	// ALGN ----- 
	CString sMODE__PLACE_ALIGN_PICK;
	int  Call__PLACE_ALIGN_PICK(CII_OBJECT__VARIABLE* p_variable,
								CII_OBJECT__ALARM* p_alarm,
								const CString& arm_type,
								const CString& stn_name,
								const CString& stn_slot);

	CString sMODE__ALGN;
	int  Call__ALGN(CII_OBJECT__VARIABLE* p_variable,
					CII_OBJECT__ALARM* p_alarm);

	// ALGNPICK -----
	CString sMODE__ALGNPICK;
	int  Call__ALGNPICK(CII_OBJECT__VARIABLE* p_variable,
						CII_OBJECT__ALARM* p_alarm,
						const CString& arm_type,
						const CString& stn_name,
						const CString& stn_slot);

	// ALGNPLACE -----
	CString sMODE__ALGNPLACE;
	int  Call__ALGNPLACE(CII_OBJECT__VARIABLE* p_variable,
						CII_OBJECT__ALARM* p_alarm,
						const CString& arm_type,
						const CString& stn_name,
						const CString& stn_slot);

	//  LoadPort Mapping -----
	CString sMODE__LP_MAP;
	int   Call__LP_MAP(CII_OBJECT__VARIABLE* p_variable,
						CII_OBJECT__ALARM* p_alarm,
						const CString& arm_type,
						const CString& stn_name,
						const CString& stn_slot);

	int  Check__STN_EXIST(CII_OBJECT__ALARM* p_alarm,const CString& stn_name);


	//-------------------------------------------------------------------------
	void Mon__ANI_MONITOR(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);
	void Mon__IO_MONITOR(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	// ...
	int  Interlock__CHECK_MATERIAL(CII_OBJECT__ALARM* p_alarm,
									const int place_flag,
									const CString& arm_type,
									const CString& stn_name,
									const CString& stn_slot,
									const CString& act_name);

	int  Interlock__CHECK_WAFER_LP_SLIDE(CII_OBJECT__ALARM* p_alarm,
										 const CString& stn_name,
										 const CString& act_name);

	int  Interlock__CHECK_DOOR_OPEN(CII_OBJECT__ALARM* p_alarm,
									const CString& stn_name,
									const CString& act_name);

	int  Fnc__CHANGE_MATERIAL_INFO(const int place_flag,
									const CString& arm_type,
									const CString& stn_name,
									const CString& stn_slot);

	// ...
	int  Interlock__AL1_SLOT_CHECK(const CString& stn_name,
		                           const CString& para_slot,
								   CString& trg_slot);

	int  Interlock__LLx_SLOT_CHECK(const CString& stn_name,
								   const CString& para_slot,
								   CString& trg_slot);
	//

	// ...
	void Set_ANI__ROBOT_EXTEND(const CString& arm_type,const CString& stn_name,const CString& stn_slot);

	void Set_ANI__ROBOT_ALL_RETRACT();
	void Set_ANI__ROBOT_RETRACT(const CString& arm_type,const CString& stn_name,const CString& stn_slot);

	void Set_ANI__ROBOT_ROTATE(const CString& arm_type,	const CString& stn_name);

	// ...
	void Fnc__APP_LOG(const CString& log_msg);

	int  Check__EFEM_DOORS(CII_OBJECT__ALARM* p_alarm,CII_OBJECT__VARIABLE* p_variable);
	int  Update__MAP_INFO(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm,const CString& stn_name);


public:
	CObj__ATM_ROBOT_STD();
	~CObj__ATM_ROBOT_STD();

	//-------------------------------------------------------------------------
	int __DEFINE__CONTROL_MODE(obj, l_mode);
	int __DEFINE__VERSION_HISTORY(version);

	int __DEFINE__VARIABLE_STD(p_variable);
	int __DEFINE__ALARM(p_alarm);

	int __Define__USER_FUNCTION(CII_DEFINE__FUNCTION *p_fnc_ctrl);

	//-------------------------------------------------------------------------
	int __INITIALIZE__OBJECT(p_variable,p_ext_obj_create);	

	//-------------------------------------------------------------------------
	int __CALL__CONTROL_MODE(mode, p_debug, p_variable, p_alarm);
	int __CALL__MONITORING(id, p_variable, p_alarm);
};
