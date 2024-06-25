#include "StdAfx.h"
#include "CObj_Phy__PMC_STD.h"
#include "CObj_Phy__PMC_STD__ALID.h"


#include "CMacro_FA.h"
extern CMacro_FA mFA_Link;

// ...
#define  STR__CREATE				"CREATE"
#define  STR__UPDATE				"UPDATE"
#define  STR__DELETE				"DELETE"

#define  STR__BAD_UPLOAD			"BAD_UPLOAD"
#define  STR__OK_UPLOAD				"OK_UPLOAD"


// ...
void CObj_Phy__PMC_STD
::Mon__FA_Report(CII_OBJECT__VARIABLE* p_variable)
{
	int cur__rms_id;
	int pre__rms_id = -100;

	CString str__act;
	CString str__file_type;
	CString str__file_name;
	CString str__create_time;
	CString str__update_time;

	// ...
	CString pmc_name;
	pmc_name.Format("PM%1d-1",iPMx_ID);

	// ...
	int cur_id__step_start;
	int pre_id__step_start = -1;

	int cur_id__step_end;
	int pre_id__step_end = -1;

	CString var_data;
	int i_data;

	// ...
	CI_FA_300mm__E30_CTRL* p_e30_ctrl = mFA_Link.Get__FA_E30_CTRL();
	CI_FA_300mm__E40_CTRL *p_fa_e40_ctrl = mFA_Link.Get__FA_E40_CTRL();
	CI_FA_300mm__E94_CTRL *p_fa_e94_ctrl = mFA_Link.Get__FA_E94_CTRL();


	while(1)
	{
		p_variable->Wait__SINGLE_OBJECT(0.01);


		if(p_e30_ctrl == NULL)
		{
			p_e30_ctrl = mFA_Link.Get__FA_E30_CTRL();
			continue;
		}
		if(p_fa_e40_ctrl == NULL)
		{
			p_fa_e40_ctrl = mFA_Link.Get__FA_E40_CTRL();
			continue;
		}
		if(p_fa_e94_ctrl == NULL)
		{
			p_fa_e94_ctrl = mFA_Link.Get__FA_E94_CTRL();
			continue;
		}

	
		// FA RMS Report ...
		while(1)		// Break 처리를 위한 While 문 ...
		{
			xCH__RMS_ID->Get__DATA(var_data);
			cur__rms_id = atoi(var_data);

			if(cur__rms_id == pre__rms_id)		break;
			if(pre__rms_id == -100)
			{
				pre__rms_id = cur__rms_id;
				break;
			}
			pre__rms_id = cur__rms_id;

			if(cur__rms_id < 0)					break;

			// ...
			xCH__RMS_ACT->Get__DATA(str__act);
			xCH__RMS_FILE_TYPE->Get__DATA(str__file_type);
			xCH__RMS_FILE_NAME->Get__DATA(str__file_name);
			xCH__RMS_CREATE_TIME->Get__DATA(str__create_time);
			xCH__RMS_UPDATE_TIME->Get__DATA(str__update_time);

			if(str__act.CompareNoCase(STR__CREATE) == 0)
			{
				i_data = iRMS_SVID__PP_CREATE;
				p_e30_ctrl->Event__PROCESS_PROGRAM_CHANGE__WITH_PARA(str__file_name, i_data);
			}
			else if(str__act.CompareNoCase(STR__UPDATE) == 0)
			{
				i_data = iRMS_SVID__PP_UPDATE;
				p_e30_ctrl->Event__PROCESS_PROGRAM_CHANGE__WITH_PARA(str__file_name, i_data);
			}
			else if(str__act.CompareNoCase(STR__DELETE) == 0)
			{
				i_data = iRMS_SVID__PP_DELETE;
				p_e30_ctrl->Event__PROCESS_PROGRAM_CHANGE__WITH_PARA(str__file_name, i_data);
			}
			else if(str__act.CompareNoCase(STR__OK_UPLOAD) == 0)
			{
				xSCH_MATERIAL_CTRL->RMS__Report_SUCCESSFUL_UPLOAD();

			}
			else if(str__act.CompareNoCase(STR__BAD_UPLOAD) == 0)
			{
				xSCH_MATERIAL_CTRL->RMS__Report_BAD_UPLOAD();
			}

			break;
		}

		// FA Step_Report ...
		while(1)		// Break 처리를 위한 While 문 ...
		{
			if(Check__Standby_Or_CtcInUse() < 0)
			{
				Sleep(90);
				break;
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

						// ...
						{
							IDS__SCH_MATERIAL_STATUS ds_sts;
							xSCH_MATERIAL_CTRL->Get__MATERIAL_STATUS(pmc_name,ds_sts);

							int port_id = ds_sts.iPORT_ID;
							int slot_id = ds_sts.iSLOT_ID;

							CString cur_cjobid;
							CString cur_pjobid;

							if(p_fa_e40_ctrl != NULL)
							{
								p_fa_e40_ctrl->Get__PJOB_FROM_PTN_And_SLOT(port_id,slot_id,cur_pjobid);
							}
							if(p_fa_e94_ctrl != NULL)
							{
								p_fa_e94_ctrl->Get__CJob_From_PJob(cur_pjobid,cur_cjobid);
							}

							xCH__CJOBID->Set__DATA(cur_cjobid);
							xCH__PJOBID->Set__DATA(cur_pjobid);
						}

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
		
			break;
		}

		// ...
	}
}
