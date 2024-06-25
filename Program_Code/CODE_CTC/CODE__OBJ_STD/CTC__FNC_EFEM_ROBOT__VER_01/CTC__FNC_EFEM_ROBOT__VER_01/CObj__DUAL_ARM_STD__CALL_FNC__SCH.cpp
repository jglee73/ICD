#include "StdAfx.h"
#include "CObj__DUAL_ARM_STD.h"


// ...
int  CObj__DUAL_ARM_STD
::Call__SCH_DB_CHECK(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm)
{
	CString str_msg;
	CString str_bff;

	// ...
	{
		str_msg = "SCH_DB Check ... \n";

		str_bff.Format("ATM_RB__Is_Available() : %1d \n", ATM_RB__Is_Available());
		str_msg += str_bff;

		str_bff.Format("xSCH_MATERIAL_CTRL->Check__SYSTEM_PAUSE()     : %1d \n", xSCH_MATERIAL_CTRL->Check__SYSTEM_PAUSE());
		str_msg += str_bff;

		str_bff.Format("xSCH_MATERIAL_CTRL->Get__MATERIAL_DB_SIZE()   : %1d \n", xSCH_MATERIAL_CTRL->Get__MATERIAL_DB_SIZE());
		str_msg += str_bff;

		str_bff.Format("xCH__ATM_RB_PAUSE_FLAG->Check__DATA(STR__YES) : %1d \n", xCH__ATM_RB_PAUSE_FLAG->Check__DATA(STR__YES));
		str_msg += str_bff;

		MessageBox(NULL, str_msg, sObject_Name, MB_OK);
	}

	// ...
	{
		str_msg = "SCH_DB Check ... \n";

		// Arm_A
		{
			CString arm_type = "A";

			IDS__SCH_MATERIAL_STATUS ds_info;
			int flag = xSCH_MATERIAL_CTRL->Get__MATERIAL_STATUS(arm_type,ds_info);

			str_bff.Format("Arm_Type : [%s] \n", arm_type);
			str_msg += str_bff;

			str_bff.Format(" * flag [%1d] \n", flag);
			str_msg += str_bff;

			str_bff.Format(" * Position [%s] \n", ds_info.sMATERIAL_POS);
			str_msg += str_bff;

			str_bff.Format(" * Status   [%s] \n", ds_info.sMATERIAL_STS);
			str_msg += str_bff;
		}
		// Arm_B
		{
			CString arm_type = "B";

			IDS__SCH_MATERIAL_STATUS ds_info;
			int flag = xSCH_MATERIAL_CTRL->Get__MATERIAL_STATUS(arm_type,ds_info);

			str_bff.Format("Arm_Type : [%s] \n", arm_type);
			str_msg += str_bff;

			str_bff.Format(" * flag [%1d] \n", flag);
			str_msg += str_bff;

			str_bff.Format(" * Position [%s] \n", ds_info.sMATERIAL_POS);
			str_msg += str_bff;

			str_bff.Format(" * Status   [%s] \n", ds_info.sMATERIAL_STS);
			str_msg += str_bff;
		}

		MessageBox(NULL, str_msg, sObject_Name, MB_OK);
	}

	xSCH_MATERIAL_CTRL->Check__STS_DB();
	xSCH_MATERIAL_CTRL->Check__RCP_DB();
	return 1;
}

