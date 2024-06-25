#include "StdAfx.h"
#include "CObj__DNET_STD.h"


//------------------------------------------------------------------------------------
void CObj__DNET_STD
::Write__APP_LOG(const CString& log_msg)
{
	xI__APP_LOG_CTRL->WRITE__LOG(log_msg);
}

void CObj__DNET_STD
::Write__DRV_LOG(const CString& log_msg)
{
	xI__DRV_LOG_CTRL->WRITE__LOG(log_msg);
}
void CObj__DNET_STD
::Write__DRV_LOG(const CString& fnc_type,
				 const CString& var_name, 
				 const CDS_IO__CHANNEL_INFO& io_info,
				 const CString& io_data,
				 const int io_index)
{
	if(dCH__APP_DRV_LOG_ENABLE->Check__DATA(STR__YES) < 0)
	{
		return;
	}

	// ...
	CString log_msg;
	CString log_bff;

	log_msg.Format("DRV_Type : %s \n", fnc_type);

	log_bff.Format("  Var_Name <- %s \n", var_name);
	log_msg += log_bff;

	log_bff.Format("  MD_NO <- %1d \n", io_info.iMD_NO);
	log_msg += log_bff;
	log_bff.Format("  CH_NO <- %1d \n", io_info.iCH_NO);
	log_msg += log_bff;

	log_bff.Format("  CMMD1 <- %s \n", io_info.sCOMMAND1);
	log_msg += log_bff;
	log_bff.Format("  CMMD2 <- %s \n", io_info.sCOMMAND2);
	log_msg += log_bff;
	log_bff.Format("  CMMD3 <- %s \n", io_info.sCOMMAND3);
	log_msg += log_bff;

	log_bff.Format("  MIN_VOLT <- %.3f \n", io_info.dMIN_VOLT);
	log_msg += log_bff;
	log_bff.Format("  MAX_VOLT <- %.3f \n", io_info.dMAX_VOLT);
	log_msg += log_bff;

	log_bff.Format("  MIN_VALUE <- %.3f \n", io_info.dMIN_VALUE);
	log_msg += log_bff;
	log_bff.Format("  MAX_VALUE <- %.3f \n", io_info.dMAX_VALUE);
	log_msg += log_bff;

	if(io_index >= 0)
	{
		log_bff.Format("  IO_DATA  <- %s \n", io_data);
		log_msg += log_bff;
		log_bff.Format("  IO_INDEX <- %1d \n", io_index);
		log_msg += log_bff;
	}

	xI__DRV_LOG_CTRL->WRITE__LOG(log_msg);
}
void CObj__DNET_STD
::Write__DRV_LOG(const CString& fnc_type,
				 const CString& var_name, 
				 const CDS_IO__CHANNEL_INFO& io_info,
				 const double set_data)
{
	if(dCH__APP_DRV_LOG_ENABLE->Check__DATA(STR__YES) < 0)
	{
		return;
	}

	// ...
	CString log_msg;
	CString log_bff;

	log_msg.Format("DRV_Type : %s \n", fnc_type);

	log_bff.Format("  Var_Name <- %s \n", var_name);
	log_msg += log_bff;

	log_bff.Format("  MD_NO <- %1d \n", io_info.iMD_NO);
	log_msg += log_bff;
	log_bff.Format("  CH_NO <- %1d \n", io_info.iCH_NO);
	log_msg += log_bff;

	log_bff.Format("  CMMD1 <- %s \n", io_info.sCOMMAND1);
	log_msg += log_bff;
	log_bff.Format("  CMMD2 <- %s \n", io_info.sCOMMAND2);
	log_msg += log_bff;
	log_bff.Format("  CMMD3 <- %s \n", io_info.sCOMMAND3);
	log_msg += log_bff;

	log_bff.Format("  MIN_VOLT <- %.3f \n", io_info.dMIN_VOLT);
	log_msg += log_bff;
	log_bff.Format("  MAX_VOLT <- %.3f \n", io_info.dMAX_VOLT);
	log_msg += log_bff;

	log_bff.Format("  MIN_VALUE <- %.3f \n", io_info.dMIN_VALUE);
	log_msg += log_bff;
	log_bff.Format("  MAX_VALUE <- %.3f \n", io_info.dMAX_VALUE);
	log_msg += log_bff;

	log_bff.Format("  SET_DATA  <- %.3f \n", set_data);
	log_msg += log_bff;

	xI__DRV_LOG_CTRL->WRITE__LOG(log_msg);
}
