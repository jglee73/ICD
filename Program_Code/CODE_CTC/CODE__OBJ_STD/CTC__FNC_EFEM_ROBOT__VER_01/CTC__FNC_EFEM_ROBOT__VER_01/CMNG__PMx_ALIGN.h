#pragma once

#include "Interface_Code.h"

#include "CCommon_System.h"
#include "CObj__DUAL_ARM_STD__DEF.h"


#define _ALIGN_STS__START				1
#define _ALIGN_STS__END					2
#define _ALIGN_STS__ABORT				3


class CMNG__PMx_ALIGN
{
public:
	SCI__CTC__SCH_MATERIAL_CTRL* pSCH_MATERIAL_CTRL;

	int iINFO__ALIGN_COUNT[CFG_PM_LIMIT];

	CStringArray sList__WAFER_TITLE;
	CStringArray sList__TRG_NAME;
	CStringArray sList__TRG_SLOT;
	CStringArray sList__AL_ANGLE;
	CUIntArray  iList__ALIGN_STATE;

	// PMx ...
	int iPMx_SIZE;
	
	CII__VAR_DIGITAL_CTRL* pCH__CFG_PMx_USE[CFG_PM_LIMIT];
	CII__VAR_STRING_CTRL*  pCH__PMx_OBJ_VIRTUAL_STATUS[CFG_PM_LIMIT];
	CII__VAR_STRING_CTRL*  pCH__PMx_PROC_INFO_PROCESS_STATUS[CFG_PM_LIMIT];
	CII__VAR_DIGITAL_CTRL* pCH__PMx_PROCESS_TYPE[CFG_PM_LIMIT];

	// LPx ...
	int iLP_SIZE;
	CII__VAR_DIGITAL_CTRL* pCH__LPx_CFG_PROCESS_TYPE[CFG_LP_LIMIT];

	// ...
	CII__VAR_DIGITAL_CTRL* pCH__CFG_TARGET_PMx_MODE;

	CII__VAR_STRING_CTRL*  pCH__WAFER_TITLE_OF_AL;

	CII__VAR_STRING_CTRL*  pCH__TRG_NAME_OF_AL;
	CII__VAR_STRING_CTRL*  pCH__TRG_SLOT_OF_AL;
	CII__VAR_STRING_CTRL*  pCH__RESULT_ALIGN_ANGLE;

	CII__VAR_STRING_CTRL*  pCH__WAFER_TITLE_OF_ARM_A;
	CII__VAR_STRING_CTRL*  pCH__TRG_NAME_OF_ARM_A;

	CII__VAR_STRING_CTRL*  pCH__WAFER_TITLE_OF_ARM_B;	
	CII__VAR_STRING_CTRL*  pCH__TRG_NAME_OF_ARM_B;

	// ...
	int  _Get__DB_INDEX(const CString& str__wfr_title);
	int  _Get__START_INDEX();
	int  _Get__MAX_ALIGN_COUNT();

	int  _Fnc__PICK_AL(const CString& str__wfr_title, 
					   const CString& str__trg_name,
					   const CString& str__trg_slot,
					   const int act_state);

	int  _Report__PICK_AL(const int act_state,
						  const CString& str__al_angle);
	//

public:
	CMNG__PMx_ALIGN();
	~CMNG__PMx_ALIGN();

	// ...
	void Reset__ALL_INFO();

	CString Get__NEXT_ALIGN(const CString& para_module, const CString& para_slot = "");
	CString Get__CURRENT_ALIGN_INFO(const CString& arm_type);

	int  Start__PICK_AL();
	int  End__PICK_AL(const CString& str__al_anble);
	int  Abort__PICK_AL();

	int  Check__ALIGN_INFO(const CString& arm_type);
};
