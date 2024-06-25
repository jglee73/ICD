#pragma once

#include "Interface_Code.h"

#include "CCommon_Error.h"
#include "CCommon_System.h"

#include "CObj_Opr__MAINT_MODE__DEF.h"


// ...
#define  ALID__OBJECT_MODE_UNKNOWN					11

// ...
#define  _CMMD__MACRO_MOVE__MANUAL_MOVE				1
#define  _CMMD__MACRO_MOVE__TOOL_CLEAR				2
#define  _CMMD__MACRO_MOVE__PROCESS					3
#define  _CMMD__MACRO_MOVE__LOOP					4
#define  _CMMD__MACRO_MOVE__ERROR					101


class CDB__MACRO_LOG
{
private:
	CRITICAL_SECTION mCS_LOCK;

	CUIntArray   iList__CMMD_TYPE;
	CStringArray sList__PARA_1;
	CStringArray sList__PARA_2;
	CStringArray sList__PARA_3;
	CStringArray sList__PARA_4;

public:
	CDB__MACRO_LOG()
	{
		InitializeCriticalSection(&mCS_LOCK);
	}
	~CDB__MACRO_LOG()
	{
		DeleteCriticalSection(&mCS_LOCK);
	}

	void Clear__ALL_HISTORY()
	{
		EnterCriticalSection(&mCS_LOCK);

		// ...
		{
			iList__CMMD_TYPE.RemoveAll();
			
			sList__PARA_1.RemoveAll();
			sList__PARA_2.RemoveAll();
			sList__PARA_3.RemoveAll();
			sList__PARA_4.RemoveAll();
		}

		LeaveCriticalSection(&mCS_LOCK);
	}
	int  Get__HISTORY_COUNT()
	{
		EnterCriticalSection(&mCS_LOCK);

		int count = iList__CMMD_TYPE.GetSize();

		LeaveCriticalSection(&mCS_LOCK);
		return count;
	}
	int Load__MOVE_INFO(const int cmmd_type,
					    const CString& str__para_1,
					    const CString& str__para_2,
					    const CString& str__para_3,
					    const CString& str__para_4,
						CString& strr__cmmd_line)   
	{
		EnterCriticalSection(&mCS_LOCK);

		int db_index = iList__CMMD_TYPE.GetSize();

		// ...
		{
			iList__CMMD_TYPE.Add(cmmd_type);

			sList__PARA_1.Add(str__para_1);
			sList__PARA_2.Add(str__para_2);
			sList__PARA_3.Add(str__para_3);
			sList__PARA_4.Add(str__para_4);
		}

		if(cmmd_type == _CMMD__MACRO_MOVE__MANUAL_MOVE)
			strr__cmmd_line.Format("Move : [%s][%s] -> [%s][%s]", str__para_1,str__para_2, str__para_3,str__para_4);
		else if(cmmd_type == _CMMD__MACRO_MOVE__TOOL_CLEAR)
			strr__cmmd_line = "TOOL_CLEAR";
		else if(cmmd_type == _CMMD__MACRO_MOVE__PROCESS)
			strr__cmmd_line.Format("Process : [%s]", str__para_1);
		else if(cmmd_type == _CMMD__MACRO_MOVE__LOOP)
			strr__cmmd_line.Format("Repeat[%s] : [%s] -> [%s]", str__para_1, str__para_2,str__para_3);

		LeaveCriticalSection(&mCS_LOCK);
		return db_index;
	}
	void Get__ALL_MOVE_HISTORY(CUIntArray&   l_cmmd_type,
							   CStringArray& l_para_1,
							   CStringArray& l_para_2,
							   CStringArray& l_para_3,
							   CStringArray& l_para_4)
	{
		EnterCriticalSection(&mCS_LOCK);

		// ...
		{
			l_cmmd_type.Copy(iList__CMMD_TYPE);

			l_para_1.Copy(sList__PARA_1);
			l_para_2.Copy(sList__PARA_2);
			l_para_3.Copy(sList__PARA_3);
			l_para_4.Copy(sList__PARA_4);
		}

		LeaveCriticalSection(&mCS_LOCK);
	}
};

class CDB__TARGET_INFO
{
public:
	int iALx_SIZE;
	CStringArray sLIST__ALx_TITLE;
	CStringArray sLIST__ALx_NAME;
	CStringArray sLIST__ALx_SLOT;
	CUIntArray   iLIST__ALx_PROC;

	int iLLx_SIZE;
	CStringArray sLIST__LLx_TITLE;
	CStringArray sLIST__LLx_NAME;
	CStringArray sLIST__LLx_SLOT;
	CUIntArray   iLIST__LLx_PROC;

	int iPMx_SIZE;
	CStringArray sLIST__PMx_TITLE;
	CStringArray sLIST__PMx_NAME;
	CStringArray sLIST__PMx_SLOT;
	CUIntArray   iLIST__PMx_PROC;

	int iSTx_SIZE;
	CStringArray sLIST__STx_TITLE;
	CStringArray sLIST__STx_NAME;
	CStringArray sLIST__STx_SLOT;
	CUIntArray   iLIST__STx_PROC;

public:
	CDB__TARGET_INFO()
	{
		iALx_SIZE = 0;
		iLLx_SIZE = 0;
		iPMx_SIZE = 0;
		iSTx_SIZE = 0;
	}

	void Get__TARGET_ITEM_LIST(CStringArray& l_var_item)
	{
		l_var_item.RemoveAll();

		// ...
		CString trg_info;
		int i;

		// ALx ...
		for(i=0; i<iALx_SIZE; i++)
		{
			trg_info = sLIST__ALx_TITLE[i];
			l_var_item.Add(trg_info);
		}
		// LLx ...
		for(i=0; i<iLLx_SIZE; i++)
		{
			trg_info = sLIST__LLx_TITLE[i];
			l_var_item.Add(trg_info);
		}
		// PMx ...
		for(i=0; i<iPMx_SIZE; i++)
		{
			trg_info = sLIST__PMx_TITLE[i];
			l_var_item.Add(trg_info);
		}
		// STx ...
		for(i=0; i<iSTx_SIZE; i++)
		{
			trg_info = sLIST__STx_TITLE[i];
			l_var_item.Add(trg_info);
		}
	}

	CString Get__TARGET_INFO(const CString& str_title)
	{
		CString trg_info;
		int i;

		// ALx ...
		for(i=0; i<iALx_SIZE; i++)
		{
			if(str_title.CompareNoCase(sLIST__ALx_TITLE[i]) != 0)			continue;

			trg_info.Format("%s-%s", sLIST__ALx_NAME[i],sLIST__ALx_SLOT[i]);
			return trg_info;
		}
		// LLx ...
		for(i=0; i<iLLx_SIZE; i++)
		{
			if(str_title.CompareNoCase(sLIST__LLx_TITLE[i]) != 0)			continue;

			trg_info.Format("%s-%s", sLIST__LLx_NAME[i],sLIST__LLx_SLOT[i]);
			return trg_info;
		}
		// PMx ...
		for(i=0; i<iPMx_SIZE; i++)
		{
			if(str_title.CompareNoCase(sLIST__PMx_TITLE[i]) != 0)			continue;

			trg_info.Format("%s-%s", sLIST__PMx_NAME[i],sLIST__PMx_SLOT[i]);
			return trg_info;
		}
		// STx ...
		for(i=0; i<iSTx_SIZE; i++)
		{
			if(str_title.CompareNoCase(sLIST__STx_TITLE[i]) != 0)			continue;

			trg_info.Format("%s-%s", sLIST__STx_NAME[i],sLIST__STx_SLOT[i]);
			return trg_info;
		}

		return str_title;
	}

	int Get__TARGET_INFO(const CString& str_title, CString & trg_name,CString& trg_slot)
	{
		bool active_proc;
		int i;

		// ALx ...
		for(i=0; i<iALx_SIZE; i++)
		{
			if(str_title.CompareNoCase(sLIST__ALx_TITLE[i]) != 0)			continue;

			trg_name = sLIST__ALx_NAME[i];
			trg_slot = sLIST__ALx_SLOT[i];
			return 1;
		}

		// LLx ...
		if(Get__LLx_INFO(str_title, trg_name,trg_slot, active_proc) > 0)
		{
			return 1;
		}

		// PMx ...
		if(Get__PMx_INFO(str_title, trg_name,trg_slot, active_proc) > 0)
		{
			return 1;
		}

		// STx ...
		for(i=0; i<iSTx_SIZE; i++)
		{
			if(str_title.CompareNoCase(sLIST__STx_TITLE[i]) != 0)			continue;

			trg_name = sLIST__STx_NAME[i];
			trg_slot = sLIST__STx_SLOT[i];
			return 1;
		}

		return -1;
	}
	int Get__PMx_INFO(const CString& str_title, CString & trg_name,CString& trg_slot, bool& active_proc)
	{
		int i;

		// PMx ...
		for(i=0; i<iPMx_SIZE; i++)
		{
			if(str_title.CompareNoCase(sLIST__PMx_TITLE[i]) != 0)			continue;

			trg_name = sLIST__PMx_NAME[i];
			trg_slot = sLIST__PMx_SLOT[i];

			if(iLIST__PMx_PROC[i] > 0)			active_proc = true;
			else								active_proc = false;

			return i;
		}

		return -1;
	}
	int Get__LLx_INFO(const CString& str_title, CString & trg_name,CString& trg_slot, bool& active_proc)
	{
		int i;

		// LLx ...
		for(i=0; i<iLLx_SIZE; i++)
		{
			if(str_title.CompareNoCase(sLIST__LLx_TITLE[i]) != 0)			continue;

			trg_name = sLIST__LLx_NAME[i];
			trg_slot = sLIST__LLx_SLOT[i];

			if(iLIST__LLx_PROC[i] > 0)			active_proc = true;
			else								active_proc = false;

			return i;
		}

		return -1;
	}
};


// ...
#define CFG_SIZE__MOVE_HISTORY						20

// ...
#define  _ACT__TRANSFER_SIZE						10
#define  _ACT_ID__MOVE_TO_LPx						0
#define  _ACT_ID__MOVE_TO_LLx_IN_ATM				1
#define  _ACT_ID__LLx_DOOR_VLV_OP_CL				2
#define  _ACT_ID__LLx_PUMP_VENT						3
#define  _ACT_ID__LLx_COOLING						4
#define  _ACT_ID__LLx_SLOT_VLV_OP_CL				5
#define  _ACT_ID__MOVE_TO_LLx_IN_VAC				6
#define  _ACT_ID__MOVE_TO_AL1						7
#define  _ACT_ID__MOVE_TO_ST1						8
#define  _ACT_ID__MOVE_TO_ST2						9

#define  _ACT_MOVE__MOVE_TO_LPx						"MOVE_TO_FOUP"
#define  _ACT_MOVE__MOVE_TO_LLx_IN_ATM				"MOVE_TO_LLx_IN_ATM"
#define  _ACT_MOVE__LLx_DOOR_VLV_OP_CL				"LLx_DOOR_VLV_OP_CL"
#define  _ACT_MOVE__LLx_PUMP_VENT					"LLx_PUMP_VENT"
#define  _ACT_MOVE__LLx_COOLING						"LLx_COOLING"
#define  _ACT_MOVE__LLx_SLOT_VLV_OP_CL				"LLx_SLOT_VLV_OP_CL"
#define  _ACT_MOVE__MOVE_TO_LLx_IN_VAC				"MOVE_TO_LLx_IN_VAC"
#define  _ACT_MOVE__MOVE_TO_AL1						"MOVE_TO_AL1"
#define  _ACT_MOVE__MOVE_TO_ST1						"MOVE_TO_ST1"
#define  _ACT_MOVE__MOVE_TO_ST2						"MOVE_TO_ST2"

// ...
#define  _ACT__PROCESS_SIZE							4
#define  _ACT_ID__MOVE_TO_PMx						0
#define  _ACT_ID__PMx_SLOT_VLV_OP_CL				1
#define  _ACT_ID__PMx_PIN_UP_TO_DOWN				2
#define  _ACT_ID__PMx_PROCESS						3

#define  _ACT_PROC__MOVE_TO_PMx						"MOVE_TO_PMx"
#define  _ACT_PROC__PMx_SLOT_VLV_OP_CL				"PMx_SLOT_VLV_OP_CL"
#define  _ACT_PROC__PMx_PIN_UP_TO_DOWN				"PMx_PIN_UP_TO_DOWN"
#define  _ACT_PROC__PMx_PROCESS						"PMx_PROCESS"


class CObj_Opr__MAINT_MODE : public __IOBJ__STD_TYPE
{
private:
	//-------------------------------------------------------------------------
	CString sObject_Name;

	CCommon_Error__USER_FNC mERROR__USER_FNC;
	SCX__USER_LOG_CTRL xAPP_LOG_CTRL;

	CDB__TARGET_INFO mDB__TRG_INFO;


	//-------------------------------------------------------------------------
	//  INTERNAL PROPERTY

	// OBJ ...
	CX__VAR_STRING_CTRL xCH__OBJ_MSG;
	CX__VAR_STRING_CTRL xCH__ACTIVE_MODE;

	// ...
	CStringArray sList__ATM_AREA;
	CStringArray sList__ATM_RB_AREA;
	CStringArray sList__LPx_AREA;
	CStringArray sList__ALx_AREA;
	CStringArray sList__BUFFx_AREA;
	CStringArray sList__VISx_AREA;
	CStringArray sList__LBx_AREA;

	CStringArray sList__VAC_AREA;
	CStringArray sList__VAC_RB_AREA;
	CStringArray sList__PMx_AREA;

	// CFG ...
	CX__VAR_DIGITAL_CTRL dCH__CFG_MANUAL_MOVE_ALIGN_TARGET_PMx;

	// PARTICLE.COMMON ...
	CX__VAR_DIGITAL_CTRL dCH__PARTICLE_PARA_SEL_AREA;
	CX__VAR_STRING_CTRL  sCH__PARTICLE_PARA_CUR_AREA;

	//
	CX__VAR_DIGITAL_CTRL dCH__PARTICLE_PARA_LPx;
	CX__VAR_STRING_CTRL  sCH__PARTICLE_CUR_LPx;

	CX__VAR_DIGITAL_CTRL dCH__PARTICLE_PARA_LLx;
	CX__VAR_STRING_CTRL  sCH__PARTICLE_CUR_LLx;

	CX__VAR_ANALOG_CTRL  aCH__PARTICLE_PARA_LLx_COOLING_SEC;
	CX__VAR_STRING_CTRL  sCH__PARTICLE_CUR_LLx_COOLING_SEC;

	CX__VAR_DIGITAL_CTRL dCH__PARTICLE_PARA_LLx_COOL_TYPE;

	CX__VAR_ANALOG_CTRL  aCH__PARTICLE_PARA_STx_SLOT[CFG_ST_LIMIT];
	CX__VAR_STRING_CTRL  sCH__PARTICLE_CUR_STx_SLOT[CFG_ST_LIMIT];

	//
	CX__VAR_DIGITAL_CTRL dCH__PARTICLE_PARA_EFEM_ROBOT;
	CX__VAR_STRING_CTRL  sCH__PARTICLE_CUR_EFEM_ROBOT;

	CX__VAR_DIGITAL_CTRL dCH__PARTICLE_PARA_TM_ROBOT;
	CX__VAR_STRING_CTRL  sCH__PARTICLE_CUR_TM_ROBOT;

	// TRANSFER.PART ...
	CX__VAR_STRING_CTRL  sCH__PARA_MANUAL_MOVE_LLx_DOOR_CLOSE_SKIP;
	CX__VAR_STRING_CTRL  sCH__PARA_MANUAL_MOVE_LLx_SLOT_CLOSE_SKIP;

	CX__VAR_STRING_CTRL  sCH__PARA_TRANSFER_ACT_CHECK_X[_ACT__TRANSFER_SIZE];
	CX__VAR_STRING_CTRL  sCH__PARA_TRANSFER_CTRL_STATE_X[_ACT__TRANSFER_SIZE];

	CX__VAR_STRING_CTRL  sCH__PARA_TRANSFER_STN_SRC_X[_ACT__TRANSFER_SIZE];
	CX__VAR_STRING_CTRL  sCH__PARA_TRANSFER_STN_TRG_X[_ACT__TRANSFER_SIZE];
	CX__VAR_STRING_CTRL  sCH__PARA_TRANSFER_CFG_COUNT_X[_ACT__TRANSFER_SIZE];
	CX__VAR_STRING_CTRL  sCH__PARA_TRANSFER_CUR_COUNT_X[_ACT__TRANSFER_SIZE];

	// PROCESS.PART ...
	CX__VAR_STRING_CTRL  sCH__PARA_MANUAL_MOVE_PMx_SLOT_CLOSE_SKIP;
	CX__VAR_STRING_CTRL  sCH__PARA_MANUAL_MOVE_PMx_LIFT_PIN_SKIP;

	CX__VAR_STRING_CTRL  sCH__PARA_PROCESS_ACT_CHECK_X[CFG_PM_LIMIT][_ACT__PROCESS_SIZE];
	CX__VAR_STRING_CTRL  sCH__PARA_PROCESS_CTRL_STATE_X[CFG_PM_LIMIT][_ACT__PROCESS_SIZE];

	CX__VAR_STRING_CTRL  sCH__PARA_PROCESS_STN_SRC_X[CFG_PM_LIMIT][_ACT__PROCESS_SIZE];
	CX__VAR_STRING_CTRL  sCH__PARA_PROCESS_STN_TRG_X[CFG_PM_LIMIT][_ACT__PROCESS_SIZE];
	CX__VAR_STRING_CTRL  sCH__PARA_PROCESS_CFG_COUNT_X[CFG_PM_LIMIT][_ACT__PROCESS_SIZE];
	CX__VAR_STRING_CTRL  sCH__PARA_PROCESS_CUR_COUNT_X[CFG_PM_LIMIT][_ACT__PROCESS_SIZE];

	CX__VAR_STRING_CTRL  sCH__PARA_PROCESS_RECIPE_NAME_X[CFG_PM_LIMIT];

	// ...
	CX__VAR_STRING_CTRL  sCH__PARTICLE_PARA_SELECT_MODE;

	CX__VAR_STRING_CTRL  sCH__PARTICLE_PARA_SRC_MODULE;
	CX__VAR_STRING_CTRL  sCH__PARTICLE_PARA_SRC_SLOT;
	CX__VAR_DIGITAL_CTRL dCH__PARTICLE_PARA_TRG_MODULE;

	CX__VAR_ANALOG_CTRL  aCH__PARTICLE_PARA_DELAY_TIME;
	CX__VAR_ANALOG_CTRL  aCH__PARTICLE_PARA_DELAY_COUNT;

	//
	CX__VAR_DIGITAL_CTRL dCH__PARTICLE_PARA_PRC_APPLY;
	CX__VAR_STRING_CTRL  sCH__PARTICLE_DATA_PRC_APPLY;

	CX__VAR_DIGITAL_CTRL dCH__PARTICLE__PROCESS_PMx;
	CX__VAR_STRING_CTRL  sCH__PARTICLE_PARA_PMx_RECIPE[CFG_PM_LIMIT];
	CX__VAR_STRING_CTRL  sCH__PARTICLE_PARA_PMx_RCP_CLN[CFG_PM_LIMIT];
	CX__VAR_STRING_CTRL  sCH__PARTICLE_PARA_PMx_RCP_PRE[CFG_PM_LIMIT];
	CX__VAR_STRING_CTRL  sCH__PARTICLE_PARA_LLx_RECIPE[CFG_LLx_LIMIT];
	CX__VAR_STRING_CTRL  sCH__PARTICLE_PARA_LLx_RCP_CLN[CFG_LLx_LIMIT];
	CX__VAR_STRING_CTRL  sCH__PARTICLE_PARA_LLx_RCP_PRE[CFG_LLx_LIMIT];

	// CX__VAR_DIGITAL_CTRL dCH__PARTICLE_PARA_CYCLE_MODULE;
	CX__VAR_STRING_CTRL  sCH__PARTICLE_DATA_CYCLE_MODULE;

	CX__VAR_ANALOG_CTRL  aCH__PARTICLE_PARA_PICK_PLACE_CYCLE_COUNT;
	CX__VAR_STRING_CTRL  sCH__PARTICLE_DATA_PICK_PLACE_CYCLE_COUNT;

	CX__VAR_ANALOG_CTRL  aCH__PARTICLE_PARA_MOVE_CYCLE_COUNT;
	CX__VAR_STRING_CTRL  sCH__PARTICLE_DATA_MOVE_CYCLE_COUNT;

	CX__VAR_ANALOG_CTRL  aCH__PARTICLE_PARA_PROC_CYCLE_COUNT;
	CX__VAR_STRING_CTRL  sCH__PARTICLE_DATA_PROC_CYCLE_COUNT;

	CX__VAR_ANALOG_CTRL  aCH__PARTICLE_PARA_ATM_ROBOT_CFG_TIME_SEC;
	CX__VAR_STRING_CTRL  sCH__PARTICLE_PARA_ATM_ROBOT_CUR_TIME_COUNT;

	CX__VAR_ANALOG_CTRL  aCH__PARTICLE_PARA_VAC_ROBOT_CFG_TIME_SEC;
	CX__VAR_STRING_CTRL  sCH__PARTICLE_PARA_VAC_ROBOT_CUR_TIME_COUNT;

	// ...
	CDB__MACRO_LOG mMacro_Log;

	CX__VAR_STRING_CTRL  sCH__MACRO_MOVE__LOG_STATUS;		// START STOP	
	CX__VAR_ANALOG_CTRL  aCH__MACRO_MOVE__CYCLE_COUNT;
	CX__VAR_STRING_CTRL  sCH__MACRO_MOVE__CUR_CYCLE_COUNT;
	CX__VAR_STRING_CTRL  sCH__MACRO_MOVE__LOG_VIEW_REQ;
	CX__VAR_STRING_CTRL  sCH__MACRO_MOVE__LOG_SAVE_REQ;
	CX__VAR_STRING_CTRL  sCH__MACRO_MOVE__LOG_FILE_NAME;

	CX__VAR_STRING_CTRL  sCH__MACRO_MOVE__HISTORY[CFG_SIZE__MOVE_HISTORY];
	CX__VAR_STRING_CTRL  sCH__MACRO_MOVE__LOG_COUNT;
	CX__VAR_STRING_CTRL  sCH__MACRO_MOVE__SCR_NAME;

	CX__VAR_ANALOG_CTRL  aCH__MACRO_MOVE__CFG_FOUP_WAIT_SEC;
	CX__VAR_STRING_CTRL  sCH__MACRO_MOVE__FOUP_WAIT_CUR_SEC;
	CX__VAR_ANALOG_CTRL  aCH__MACRO_MOVE__CFG_PMC_WAIT_SEC;
	CX__VAR_STRING_CTRL  sCH__MACRO_MOVE__PMC_WAIT_CUR_SEC;
	CX__VAR_ANALOG_CTRL  aCH__MACRO_MOVE__CFG_BUFFER_WAIT_SEC;
	CX__VAR_STRING_CTRL  sCH__MACRO_MOVE__BUFFER_WAIT_CUR_SEC;

	CX__VAR_STRING_CTRL  sCH__MACRO_MOVE__ATM_RB_MOVE_POS__CENTER;
	CX__VAR_STRING_CTRL  sCH__MACRO_MOVE__ATM_RB_ARM_ACT__RETRACT;
	CX__VAR_STRING_CTRL  sCH__MACRO_MOVE__VAC_RB_ARM_TYPE__A;
	CX__VAR_STRING_CTRL  sCH__MACRO_MOVE__VAC_RB_ROT_POS__CENTER;
	CX__VAR_STRING_CTRL  sCH__MACRO_MOVE__VAC_RB_ARM_ACT__RETRACT;

	CX__VAR_STRING_CTRL  sCH__MACRO_MOVE__EDITOR_ACTION_NAME[CFG_SIZE__MOVE_HISTORY];
	CX__VAR_STRING_CTRL  sCH__MACRO_MOVE__EDITOR_CUR_INDEX;
	CX__VAR_STRING_CTRL  sCH__MACRO_MOVE__EDITOR_CHANGE_FLAG;

	//
	CX__VAR_STRING_CTRL  sCH__MACRO_MOVE__MOVE_SELECT;
	CX__VAR_STRING_CTRL  sCH__MACRO_MOVE__SRC_SLOT;
	CX__VAR_STRING_CTRL  sCH__MACRO_MOVE__SRC_MODULE;
	CX__VAR_STRING_CTRL  sCH__MACRO_MOVE__TRG_SLOT;
	CX__VAR_STRING_CTRL  sCH__MACRO_MOVE__TRG_MODULE;

	CX__VAR_ANALOG_CTRL  aCH__MACRO_MOVE__LOOP_START;
	CX__VAR_ANALOG_CTRL  aCH__MACRO_MOVE__LOOP_END;
	CX__VAR_ANALOG_CTRL  aCH__MACRO_MOVE__LOOP_REPEAT;

	CX__VAR_STRING_CTRL  sCH__MACRO_MOVE__STEP_REPEAT_START;
	CX__VAR_STRING_CTRL  sCH__MACRO_MOVE__STEP_REPEAT_END;
	CX__VAR_STRING_CTRL  sCH__MACRO_MOVE__STEP_REPEAT_COUNT;
	CX__VAR_STRING_CTRL  sCH__MACRO_MOVE__STEP_REPEAT_INDEX;

	CX__VAR_STRING_CTRL  sCH__MACRO_MOVE__PROCESS_ROUTE;
	CX__VAR_DIGITAL_CTRL dCH__MACRO_MOVE__PROCESS_PMx;
	CX__VAR_STRING_CTRL  sCH__MACRO_MOVE__PROCESS_PRE_RECIPE_PMx[CFG_PM_LIMIT];
	CX__VAR_STRING_CTRL  sCH__MACRO_MOVE__PROCESS_MAIN_RECIPE_PMx[CFG_PM_LIMIT];
	CX__VAR_STRING_CTRL  sCH__MACRO_MOVE__PROCESS_POST_RECIPE_PMx[CFG_PM_LIMIT];
	CX__VAR_STRING_CTRL  sCH__MACRO_MOVE__PROCESS_PRE_RECIPE_LLx[CFG_LLx_LIMIT];
	CX__VAR_STRING_CTRL  sCH__MACRO_MOVE__PROCESS_MAIN_RECIPE_LLx[CFG_LLx_LIMIT];
	CX__VAR_STRING_CTRL  sCH__MACRO_MOVE__PROCESS_POST_RECIPE_LLx[CFG_LLx_LIMIT];

	CX__VAR_STRING_CTRL  sCH__MACRO_MOVE__OBJ_STATUS__STANDBY;
	CX__VAR_STRING_CTRL  sCH__MACRO_MOVE__PRESSURE_STATUS__VAC;
	CX__VAR_STRING_CTRL  sCH__MACRO_MOVE__DOOR_STATUS__CLOSE;
	CX__VAR_STRING_CTRL  sCH__MACRO_MOVE__DOOR_STATUS__OPEN;
	CX__VAR_STRING_CTRL  sCH__MACRO_MOVE__CST_STATUS__EXIST;
	CX__VAR_STRING_CTRL  sCH__MACRO_MOVE__CST_POS__LOAD;

	CX__VAR_STRING_CTRL  sCH__MACRO_MOVE__EDITOR_ACT_CMMD;
	CX__VAR_STRING_CTRL  sCH__MACRO_MOVE__FILE_ACT_CMMD;
	CX__VAR_STRING_CTRL  sCH__MACRO_MOVE__FILE_NAME;

	CX__VAR_STRING_CTRL  sCH__MACRO_MOVE__ACTION_SELECT;

	CX__VAR_STRING_CTRL  sCH__MACRO_MOVE__PMC_SLOT_STATUS[CFG_PM_LIMIT];
	CX__VAR_STRING_CTRL  sCH__MACRO_MOVE__PMC_SLOT_TITLE[CFG_PM_LIMIT];

	CX__VAR_STRING_CTRL  sCH__MACRO_MOVE__VAC_RB__A_ARM_SLOT_STATUS;
	CX__VAR_STRING_CTRL  sCH__MACRO_MOVE__VAC_RB__A_ARM_SLOT_TITLE;
	CX__VAR_STRING_CTRL  sCH__MACRO_MOVE__VAC_RB__B_ARM_SLOT_STATUS;
	CX__VAR_STRING_CTRL  sCH__MACRO_MOVE__VAC_RB__B_ARM_SLOT_TITLE;

	CX__VAR_STRING_CTRL  sCH__MACRO_MOVE__ATM_RB__A_ARM_SLOT_STATUS;
	CX__VAR_STRING_CTRL  sCH__MACRO_MOVE__ATM_RB__A_ARM_SLOT_TITLE;

	CX__VAR_STRING_CTRL  sCH__MACRO_MOVE__ATM_RB__B_ARM_SLOT_STATUS;
	CX__VAR_STRING_CTRL  sCH__MACRO_MOVE__ATM_RB__B_ARM_SLOT_TITLE;

	CX__VAR_STRING_CTRL  sCH__MACRO_MOVE__LLx_SLOT_STATUS[CFG_LLx_LIMIT][CFG_LLx__SLOT_MAX];
	CX__VAR_STRING_CTRL  sCH__MACRO_MOVE__LLx_SLOT_TITLE[CFG_LLx_LIMIT][CFG_LLx__SLOT_MAX];

	CX__VAR_STRING_CTRL  sCH__MACRO_MOVE__AL_SLOT_STATUS[CFG_AL__SLOT_MAX];
	CX__VAR_STRING_CTRL  sCH__MACRO_MOVE__AL_SLOT_TITLE[CFG_AL__SLOT_MAX];

	CX__VAR_STRING_CTRL  sCH__MACRO_MOVE__BUFFER1_SLOT_STATUS[CFG_BUFFER__SLOT_MAX];
	CX__VAR_STRING_CTRL  sCH__MACRO_MOVE__BUFFER1_SLOT_TITLE[CFG_BUFFER__SLOT_MAX];

	CX__VAR_STRING_CTRL  sCH__MACRO_MOVE__BUFFER2_SLOT_STATUS[CFG_BUFFER__SLOT_MAX];
	CX__VAR_STRING_CTRL  sCH__MACRO_MOVE__BUFFER2_SLOT_TITLE[CFG_BUFFER__SLOT_MAX];

	CX__VAR_STRING_CTRL  sCH__MACRO_MOVE__LPx_SLOT_STATUS[CFG_LP_LIMIT][CFG_LP__SLOT_MAX];

	// ...
	CString sVAR_PARA__MOVE_SELECT;

	CX__VAR_STRING_CTRL  sCH__PARA_SRC_MODULE_BY_USER;
	CX__VAR_STRING_CTRL  sCH__PARA_SRC_SLOT_BY_USER;

	CX__VAR_STRING_CTRL  sCH__PARA_TRG_MODULE_BY_USER;
	CX__VAR_STRING_CTRL  sCH__PARA_TRG_SLOT_BY_USER;

	//
	CX__VAR_STRING_CTRL  sCH__MOVE_SRC_MODULE;
	CX__VAR_STRING_CTRL  sCH__MOVE_SRC_SLOT;

	CX__VAR_STRING_CTRL  sCH__MOVE_TRG_MODULE;
	CX__VAR_STRING_CTRL  sCH__MOVE_TRG_SLOT;

	// ...
	CX__VAR_STRING_CTRL  sCH__TEST_TARGET_TITLE;
	//

	//-------------------------------------------------------------------------
	//  EXTERNAL PROPERTY

	// DB.CFG ...
	CX__VAR_DIGITAL_CTRL dEXT_CH__PNx_USE[CFG_PM_LIMIT];

	// LPx - OBJ ...
	int iLPx_SIZE;
	CX__VAR_STRING_CTRL sEXT_CH__LPx_LOCK_FLAG[CFG_LP_LIMIT];

	// ATM RB - OBJ ...
	CII_EXT_OBJECT__CTRL *pATM_RB__OBJ_CTRL;

	CX__VAR_STRING_CTRL  sEXT_CH__PARA_MANUAL_MOVE_ALIGN_TARGET_PMx;
	CX__VAR_STRING_CTRL  sEXT_CH__PARA_MANUAL_MOVE_ALIGN_SKIP;

	CX__VAR_STRING_CTRL  sEXT_CH__TARGET_LLx_NAME;
	CX__VAR_STRING_CTRL  sEXT_CH__TARGET_LLx_SLOT;

	CX__VAR_STRING_CTRL  sEXT_CH__ATM_RB__CHECK_MODULE;
	CX__VAR_DIGITAL_CTRL dEXT_CH__ATM_RB__CFG_BUFFER_USE_DURING_MAINT_MODE;

	// VAC_RB - OBJ ...
	CII_EXT_OBJECT__CTRL *pVAC_RB__OBJ_CTRL;

	CX__VAR_STRING_CTRL  sEXT_CH__VAC_RB__CHECK_MODULE;	

	// PMx - OBJ ...
	int iPMx_SIZE;
	CII_EXT_OBJECT__CTRL *pPMx__OBJ_CTRL[CFG_PM_LIMIT];

	// LLx - OBJ ...
	int iLLx_SIZE;
	CII_EXT_OBJECT__CTRL *pLLx__OBJ_CTRL[CFG_LLx_LIMIT];

	CX__VAR_DIGITAL_CTRL dEXT_CH__LLx_PARA_SLOT_ID[CFG_LLx_LIMIT];
	CX__VAR_ANALOG_CTRL  aEXT_CH__LLx_PARA_VENT_COOLING_SEC[CFG_LLx_LIMIT];

	CX__VAR_STRING_CTRL  sEXT_CH__LLx_PARA_PROC_RECIPE_NAME[CFG_LLx_LIMIT];
	CX__VAR_STRING_CTRL  sEXT_CH__LLx_PARA_PROC_SCH_NAME[CFG_LLx_LIMIT];

	CX__VAR_DIGITAL_CTRL dEXT_CH__LLx_PRESSURE_STATUS[CFG_LLx_LIMIT];

	// OBJ - TMC_CHM ...
	CII_EXT_OBJECT__CTRL *pOBJ_CTRL__TMC_CHM;

	CX__VAR_DIGITAL_CTRL dEXT_CH__TMC_CHM__PARA_PMx_ID;
	//


	//-------------------------------------------------------------------------
	CString sMODE__MANUAL_MOVE;
	int  Call__MANUAL_MOVE(CII_OBJECT__VARIABLE *p_variable);

	CString sMODE__TOOL_CLEAR;
	int  Call__TOOL_CLEAR(CII_OBJECT__VARIABLE *p_variable);

	CString sMODE__MACRO_MOVE;
	int  Call__MACRO_MOVE(CII_OBJECT__VARIABLE *p_variable);

	// ...
	CString sMODE__PARTICLE_PART_CHECK;
	int  Call__PARTICLE_PART_CHECK(CII_OBJECT__VARIABLE *p_variable, CII_OBJECT__ALARM *p_alarm);

	CString sMODE__PARTICLE_TRANSFER_PART;
	int  Call__PARTICLE_TRANSFER_PART(CII_OBJECT__VARIABLE *p_variable, CII_OBJECT__ALARM *p_alarm);
	int  _Reserve__PARTICLE_TRANSFER_PART(CII_OBJECT__VARIABLE *p_variable, CII_OBJECT__ALARM *p_alarm);
	int  _Release__PARTICLE_TRANSFER_PART(CII_OBJECT__VARIABLE *p_variable, CII_OBJECT__ALARM *p_alarm);
	int  _Run__PARTICLE_TRANSFER_PART(CII_OBJECT__VARIABLE *p_variable, CII_OBJECT__ALARM *p_alarm);

	CString sMODE__PARTICLE_PROCESS_PART;
	int  Call__PARTICLE_PROCESS_PART(CII_OBJECT__VARIABLE *p_variable, CII_OBJECT__ALARM *p_alarm);
	int  _Reserve__PARTICLE_PROCESS_PART(CII_OBJECT__VARIABLE *p_variable, CII_OBJECT__ALARM *p_alarm);
	int  _Release__PARTICLE_PROCESS_PART(CII_OBJECT__VARIABLE *p_variable, CII_OBJECT__ALARM *p_alarm);
	int  _Run__PARTICLE_PROCESS_PART(CII_OBJECT__VARIABLE *p_variable, CII_OBJECT__ALARM *p_alarm);

	CString _Get__Transfer_Action_Name(const int act_id);
	CString _Get__Process_Action_Name(const int act_id);

	// ...
	CString sMODE__PARTICLE_CHECK;
	int  Call__PARTICLE_CHECK(CII_OBJECT__VARIABLE *p_variable);
	int  _Fnc__PARTICLE_CHECK__WAITING(CII_OBJECT__VARIABLE *p_variable, 
									   const CString& para__trg_module,
									   const CString& para__trg_slot,
		                               const bool active__pm,
									   const bool active__ll,
									   const int prc_check__pm_index,
									   const int prc_check__ll_index);

	// ...
	CString sMODE__CHECK_TARGET_INFO;
	int  Call__CHECK_TARGET_INFO(CII_OBJECT__VARIABLE *p_variable);

	// ...
	void Mon__MACRO_MOVE_CTRL(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);
	void Fnc__MACRO_MOVE_EDITOR(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);

	// ...
	void Fnc__MOVE_HISTORY_TO_SCR();

	void Fnc__MACRO_MOVE_CHANGE_INFO();
	void Fnc__MACRO_MOVE_INIT_INFO();
	void Fnc__MACRO_MOVE_EDITOR_CLEAR();
	void Fnc__MACRO_MOVE_WAFER_RESET();

	// ...
	int  Fnc__ATM_RB__Manual_Move_To(CII_OBJECT__VARIABLE *p_variable,
									 const CString& s_module,
									 const CString& s_slot,
									 CString& t_module,CString& t_slot);

	int  ATM_RB__Pick_Module_With_Arm(CII_OBJECT__VARIABLE *p_variable,
									  CString& arm_type,
									  const CString& s_module,
									  const CString& s_slot);

	int  ATM_RB__Pick_Module(CII_OBJECT__VARIABLE *p_variable,
		                     const CString& str_module,
							 const CString& str_slot,
							 CString& arm_type);

	int  ATM_RB__Place_Module(CII_OBJECT__VARIABLE *p_variable,
		                      const CString& arm_type,
							  CString& str_module,
							  CString& str_slot,
							  const bool active__al_retry = true);

	// ...
	int  Fnc__VAC_RB__Manual_Move_To(CII_OBJECT__VARIABLE *p_variable,
									 const CString& s_module,const CString& s_slot,
									 CString& t_module,CString& t_slot);

	int  VAC_RB__Pick_Module_With_Arm(CII_OBJECT__VARIABLE *p_variable,
									  CString& arm_type,const CString& s_module,const CString& s_slot);

	int  VAC_RB__Pick_Module(CII_OBJECT__VARIABLE *p_variable,
							 const CString& str_module,const CString& str_slot,CString& arm_type);

	int  VAC_RB__Place_Module(CII_OBJECT__VARIABLE *p_variable,
							  const CString& arm_type,CString& str_module,CString& str_slot);

	int  Seq__VAC_RB__Place(CII_OBJECT__VARIABLE* p_variable,
							const CString& arm_type,
							const CString& str_module,
							const CString& str_slot);
	int  Seq__VAC_RB__Pick(CII_OBJECT__VARIABLE* p_variable,
							const CString& arm_type,
							const CString& str_module,
							const CString& str_slot);

	// ...
	int  Seq__ATM_RB__Place(CII_OBJECT__VARIABLE* p_variable,
							const CString& arm_type,
							const CString& str_module,
							const CString& str_slot);
	int  Seq__ATM_RB__Pick(CII_OBJECT__VARIABLE* p_variable,
							const CString& arm_type,
							const CString& str_module,
							const CString& str_slot);

	// ...
	CString Get__ATM_RB1__ARM_TYPE(const CString& str_arm);
	CString Get__VAC_RB1__ARM_TYPE(const CString& str_arm);

	int  Get__LLx_INDEX(const CString str_module);
	CString Get__LLx_NAME(const int ll_i);
	CString Get__LLx_NAME(const CString& ll_name);

	bool Check__LLx_ISLOT(const CString str_module, CString& ll_name,CString& ll_slot);
	//

public:
	CObj_Opr__MAINT_MODE();
	~CObj_Opr__MAINT_MODE();

	//-------------------------------------------------------------------------
	int __DEFINE__CONTROL_MODE(obj,l_mode);
	int __DEFINE__VERSION_HISTORY(version);

	int __DEFINE__VARIABLE_STD(p_variable);
	int __DEFINE__ALARM(p_alarm);
	
	int __Define__USER_FUNCTION(CII_DEFINE__FUNCTION *p_def_fnc);

	//-------------------------------------------------------------------------
	int __INITIALIZE__OBJECT(p_variable,p_ext_obj_create);

	//-------------------------------------------------------------------------
	int __CALL__CONTROL_MODE(mode,p_debug,p_variable,p_alarm);
	int __CALL__MONITORING(id,p_variable,p_alarm);
};
