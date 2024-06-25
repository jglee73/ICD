#include "StdAfx.h"
#include "CObj_Phy__PMC_DUMMY.h"
#include "CObj_Phy__PMC_DUMMY__ALID.h"


#define  STR__CREATE				"CREATE"
#define  STR__UPDATE				"UPDATE"
#define  STR__DELETE				"DELETE"

#define  STR__BAD_UPLOAD			"BAD_UPLOAD"
#define  STR__OK_UPLOAD				"OK_UPLOAD"


// ...
void CObj_Phy__PMC_DUMMY
::Mon__FA_Step_Report(CII_OBJECT__VARIABLE* p_variable)
{
	CString pmc_name;
	pmc_name.Format("PM%1d-1",iPMx_ID);

	// ...
	CString var_data;

	int cur_id__step_start;
	int pre_id__step_start = -1;

	int cur_id__step_end;
	int pre_id__step_end = -1;

	while(1)
	{
		Sleep(3);

		if(Check__Standby_Or_CtcInUse() < 0)
		{
			Sleep(90);
			continue;
		}

		xCH__STEP_START->Get__DATA(var_data);
		cur_id__step_start = atoi(var_data);

		xCH__STEP_END->Get__DATA(var_data);
		cur_id__step_end = atoi(var_data);

		if(cur_id__step_end != pre_id__step_end)
		{
			pre_id__step_end = cur_id__step_end;

			if(cur_id__step_end > 0)
			{
				if(iFLAG__STEP_REPORT >= 0)
				{
					if(iFLAG__STEP_REPORT == 0)		iFLAG__STEP_REPORT = -1;

					xSCH_MATERIAL_CTRL->Step_End__PMC(pmc_name,iPMx_ID);
					xSCH_MATERIAL_CTRL->Step_Data_Report__PMC(pmc_name,iPMx_ID);
				}
			}
		}

		if(cur_id__step_start != pre_id__step_start)
		{
			pre_id__step_start = cur_id__step_start;

			if(cur_id__step_start > 0)
			{
				if(iFLAG__STEP_REPORT >= 0)
				{
					if(iFLAG__STEP_REPORT == 0)		iFLAG__STEP_REPORT = -1;

					xSCH_MATERIAL_CTRL->Step_Start__PMC(pmc_name,iPMx_ID);
				}
			}
		}

		// ...
	}
}
void CObj_Phy__PMC_DUMMY
::Mon__FA_RMS_Report(CII_OBJECT__VARIABLE* p_variable)
{
	int cur__rms_id;
	int pre__rms_id = -100;

	CString str__act;
	CString str__file_type;
	CString str__file_name;
	CString str__create_time;
	CString str__update_time;

	CString var_data;

	while(1)
	{
		Sleep(3);

		xCH__RMS_ID->Get__DATA(var_data);
		cur__rms_id = atoi(var_data);

		if(cur__rms_id == pre__rms_id)
		{
			continue;
		}
		if(pre__rms_id == -100)
		{
			pre__rms_id = cur__rms_id;
			continue;
		}

		pre__rms_id = cur__rms_id;
		if(cur__rms_id < 0)
		{
			continue;
		}

		xCH__RMS_ACT->Get__DATA(str__act);
		xCH__RMS_FILE_TYPE->Get__DATA(str__file_type);
		xCH__RMS_FILE_NAME->Get__DATA(str__file_name);
		xCH__RMS_CREATE_TIME->Get__DATA(str__create_time);
		xCH__RMS_UPDATE_TIME->Get__DATA(str__update_time);

		if(str__act.CompareNoCase(STR__CREATE) == 0)
		{
			xSCH_MATERIAL_CTRL->RMS__Report_PP_CREATE(str__file_name);

		}
		else if(str__act.CompareNoCase(STR__UPDATE) == 0)
		{
			xSCH_MATERIAL_CTRL->RMS__Report_PP_UPDATE(str__file_name);

		}
		else if(str__act.CompareNoCase(STR__DELETE) == 0)
		{
			xSCH_MATERIAL_CTRL->RMS__Report_PP_DELETE(str__file_name);

		}
		else if(str__act.CompareNoCase(STR__OK_UPLOAD) == 0)
		{
			xSCH_MATERIAL_CTRL->RMS__Report_SUCCESSFUL_UPLOAD();

		}
		else if(str__act.CompareNoCase(STR__BAD_UPLOAD) == 0)
		{
			xSCH_MATERIAL_CTRL->RMS__Report_BAD_UPLOAD();

		}

		// ...
	}
}
