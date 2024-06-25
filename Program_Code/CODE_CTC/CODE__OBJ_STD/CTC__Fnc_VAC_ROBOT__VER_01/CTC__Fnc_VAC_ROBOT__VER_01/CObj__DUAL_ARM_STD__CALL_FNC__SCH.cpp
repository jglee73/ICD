#include "StdAfx.h"
#include "CObj__DUAL_ARM_STD.h"


// ...
int  CObj__DUAL_ARM_STD
::Call__SCH_DB_CHECK(CII_OBJECT__VARIABLE *p_variable)
{
	CString str_msg;
	CString str_bff;

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

	xSCH_MATERIAL_CTRL->Check__STS_DB();
	xSCH_MATERIAL_CTRL->Check__RCP_DB();
	return 1;
}

int  CObj__DUAL_ARM_STD
::Call__USER_INFO_CHECK(CII_OBJECT__VARIABLE *p_variable)
{
	CString log_msg;
	CString log_bff;

	CString def_name;
	CString def_data;

	int port_id = 1;

	log_msg.Format("LP%1d - Report ... \n", port_id);

	for(int i=0; i < 25; i++)
	{
		int slot_id = i + 1;

		log_bff.Format("%02d) \n", slot_id);
		log_msg += log_bff;

		// 1.
		{
			def_name = USER_DEF__TRG_MDx;

			int r_flag = xSCH_MATERIAL_CTRL->Get__MATERIAL_USER_INFO(port_id,slot_id, def_name,def_data);
			if(r_flag > 0)
			{
				log_bff.Format("   * %s <- %s \n", def_name,def_data);
				log_msg += log_bff;
			}
		}
		// 2.
		{
			def_name = USER_DEF__TRG_SLOT;

			int r_flag = xSCH_MATERIAL_CTRL->Get__MATERIAL_USER_INFO(port_id,slot_id, def_name,def_data);
			if(r_flag > 0)
			{
				log_bff.Format("   * %s <- %s \n", def_name,def_data);
				log_msg += log_bff;
			}
		}
		// 3.
		{
			def_name = USER_DEF__TRG_ANGLE;

			int r_flag = xSCH_MATERIAL_CTRL->Get__MATERIAL_USER_INFO(port_id,slot_id, def_name,def_data);
			if(r_flag > 0)
			{
				log_bff.Format("   * %s <- %s \n", def_name,def_data);
				log_msg += log_bff;
			}
		}
		// 4.
		{
			def_name = USER_DEF__GOTO_PM;

			int r_flag = xSCH_MATERIAL_CTRL->Get__MATERIAL_USER_INFO(port_id,slot_id, def_name,def_data);
			if(r_flag > 0)
			{
				log_bff.Format("   * %s <- %s \n", def_name,def_data);
				log_msg += log_bff;
			}
		}
	}

	printf(log_msg);

	return 1;
}

