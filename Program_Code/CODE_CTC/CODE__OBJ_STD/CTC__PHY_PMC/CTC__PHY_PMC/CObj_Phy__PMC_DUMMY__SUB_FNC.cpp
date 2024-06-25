#include "StdAfx.h"
#include "CObj_Phy__PMC_DUMMY.h"
#include "CObj_Phy__PMC_DUMMY__ALID.h"

#include "Macro_Function.h"


// ...
void CObj_Phy__PMC_DUMMY
::Datalog__Write_Lot_Slot(const int slot_log_count,
						  const int pm_slot_id,
						  const IDS__SCH_MATERIAL_INFO& db_info,
						  CString& pm_log_path)
{
	CString pm_name;
	pm_name.Format("PM%1d-%1d",iPMx_ID,pm_slot_id);

	CString lotid = db_info.sLOT_ID;

	CString str_portid;
	str_portid.Format("PORT%1d",db_info.iSRC__PTN);

	int     lp_slotid = db_info.iSRC__STN;
	CString recipe = db_info.sCUR_RECIPE;

	// ...
	CString log_file;
	CString sub_dir;
	CString date;
	CString time;
	CString file;
	CString bff;

	Macro__Get_Date_Time(date, time);

	sub_dir  = date;
	sub_dir += "\\";
	sub_dir += lotid;
	sub_dir += "\\";

	if((iLOG_SLOT_MAX > 1)||(slot_log_count == 0))
	{
		file  = lotid;
		file += "_";	
		bff.Format("%1d",lp_slotid);
		file += bff;
		file += "-";
		file += time;
	}
	else 
	{
		xCH__LOG_FILE_NAME->Get__DATA(file);		
	}

	log_file  = sub_dir;
	log_file += file;

	if(iLOG_ID < 1)
	{
		log_file += ".d";
	}
	else
	{
		CString bff_log;
		bff_log.Format(".d%1d",iLOG_ID);

		log_file += bff_log;
	}

	if(slot_log_count == 0)
	{
		xCH__LOG_SUB_DIRECTORY->Set__DATA(sub_dir);
		xCH__LOG_FILE_NAME->Set__DATA(file);
	}

	// ...
	int pm_slot_index = pm_slot_id - 1;

	if((pm_slot_index >= 0)&&(pm_slot_index < CFG__PMx_MAX_SLOT))
	{
		xCH__SLOT_LOG_SUB_DIRECTORY[pm_slot_index]->Set__DATA(sub_dir);
		xCH__SLOT_LOG_FILE_NAME[pm_slot_index]->Set__DATA(file);
	}

	pm_log_path = log_file;
}
void CObj_Phy__PMC_DUMMY
::Datalog__Write_Lot_Slot_End(const int pm_slot_id,
								const CString& lotid,
								const CString& portid,
								const int slotid)
{
	xCH__LOG_SUB_DIRECTORY->Set__DATA("");
	xCH__LOG_FILE_NAME->Set__DATA("");

	// ...
	int pm_slot_index = pm_slot_id - 1;

	if((pm_slot_index >= 0)&&(pm_slot_index < CFG__PMx_MAX_SLOT))
	{
		xCH__SLOT_LOG_SUB_DIRECTORY[pm_slot_index]->Set__DATA("");
		xCH__SLOT_LOG_FILE_NAME[pm_slot_index]->Set__DATA("");
	}
}
