#include "StdAfx.h"
#include "CObj__DRV_TEST.h"
#include "CObj__DRV_TEST__DEF.h"

#include "cifuser.h"
#include "rcs_user.h"
#include "dnm_user.h"


//-------------------------------------------------------------------------	
int  CObj__DRV_TEST
::Call__DEV_INFO(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm)
{
	bool active__retry_check = false;

LOOP_RETRY:

	EnterCriticalSection(&mLOCK_DNET);

	if(active__retry_check)
	{
		printf("Object Name <- [%s] \n", sObject_Name);;
		printf("  * CObj__DRV_TEST::Call__DEV_INFO() - Retry !!! \n");

		_Fnc__DEV_INFO(false);
	}
	else
	{
		printf("Object Name <- [%s] \n", sObject_Name);;
		printf("  * CObj__DRV_TEST::Call__DEV_INFO() ... \n");

		int err_count = _Fnc__DEV_INFO(true);
		if(err_count > 0)
		{
			err_count = _Fnc__DEV_INFO(false);
		}

		printf("  * err_count <- [%1d] \n", err_count);
	}

	LeaveCriticalSection(&mLOCK_DNET);

	_Check__DEV_TOTAL_MEMORY();

	// Error.Check ...
	{
		CString err_msg;

		if(Check__DEV_ERROR(err_msg) > 0)
		{
			int alm_id = ALID__DNET_NODE_ERROR;
			CString r_act;

			p_alarm->Popup__ALARM_With_MESSAGE(alm_id, err_msg, r_act);

			if(r_act.CompareNoCase(_ACT__RETRY) == 0)
			{
				active__retry_check = true;
				goto LOOP_RETRY;
			}
			if(r_act.CompareNoCase(_ACT__ABORT) == 0)
			{
				bActive__DNET_INIT = false;
				return -1;
			}
		}
	}

	if(iActive__SIM_MODE > 0)
	{
		bActive__DNET_INIT = true;
	}
	return 1;
}

int  CObj__DRV_TEST::Check__DEV_ERROR()
{
	CString err_msg;

	return Check__DEV_ERROR(err_msg);
}
int  CObj__DRV_TEST::Check__DEV_ERROR(CString& err_msg)
{
	err_msg = "";

	// ...
	bool active__error_check = false;

	CString err_bff;
	CString ch_data;

	// Total.Check ...
	{
		// In-Byte
		ch_data = sCH__DNET_CFG__TOTAL_IN_BYTE_USE->Get__STRING();
		if(sCH__DNET_INFO__TOTAL_IN_BYTE->Check__DATA(ch_data) < 0)
		{
			active__error_check = true;

			err_bff  = "Total In-Byte \n";
			err_msg += err_bff;

			err_bff.Format("  * Config <- %s \n",    sCH__DNET_CFG__TOTAL_IN_BYTE_USE->Get__STRING());
			err_msg += err_bff;
			err_bff.Format("  * Scan.Info <- %s \n", sCH__DNET_INFO__TOTAL_IN_BYTE->Get__STRING());
			err_msg += err_bff;
		}

		// Out-Byte
		ch_data = sCH__DNET_CFG__TOTAL_OUT_BYTE_USE->Get__STRING();
		if(sCH__DNET_INFO__TOTAL_OUT_BYTE->Check__DATA(ch_data) < 0)
		{
			active__error_check = true;

			err_bff  = "Total Out-Byte \n";
			err_msg += err_bff;

			err_bff.Format("  * Config <- %s \n",    sCH__DNET_CFG__TOTAL_OUT_BYTE_USE->Get__STRING());
			err_msg += err_bff;
			err_bff.Format("  * Scan.Info <- %s \n", sCH__DNET_INFO__TOTAL_OUT_BYTE->Get__STRING());
			err_msg += err_bff;
		}
	}

	// Node.Check ...
	for(int i=0; i<iSLAVE_COUNT; i++)
	{
		bool active__node_error = false;
		int node_id = i + 1;

		// In-Byte
		ch_data = sCH__DNET_CFG__SLAVE_X__IN_SIZE_USE[i]->Get__STRING();
		if(sCH__DNET_INFO__SLAVE_X__IN_SIZE_USE[i]->Check__DATA(ch_data) < 0)
		{
			active__node_error = true;
		}

		// In-Byte
		ch_data = sCH__DNET_CFG__SLAVE_X__OUT_SIZE_USE[i]->Get__STRING();
		if(sCH__DNET_INFO__SLAVE_X__OUT_SIZE_USE[i]->Check__DATA(ch_data) < 0)
		{
			active__node_error = true;
		}

		// Communication
		ch_data = sCH__DNET_INFO__SLAVE_X__ERROR_CHECK_ID[i]->Get__STRING();
		int err_id = atoi(ch_data);
		if(err_id > 0)
		{
			active__node_error = true;
		}

		if(active__node_error)
		{
			active__error_check = true;

			err_bff.Format("Node(%1d) Error ! \n", node_id);
			err_msg += err_bff;
			err_bff.Format("  * MacID <- %s \n", sCH__DNET_CFG__SLAVE_X__MACID[i]->Get__STRING());
			err_msg += err_bff;
		}
	}

	if(active__error_check)			return 1;
	return -1;
}
int  CObj__DRV_TEST::_Fnc__DEV_INFO(const bool active_update)
{
	CString log_msg;
	CString log_add;

	log_msg  = "\n";
	log_add.Format("_Fnc__DEV_INFO) - active_update(%1d) \n", active_update);
	log_msg += log_add;

	// ...
	HANDLE h_drv_board   = mDNet_Mng.Get__DRV_BOARD_HANDLE();
	HANDLE h_drv_channel = mDNet_Mng.Get__DRV_CHANNEL_HANDLE();

	if(h_drv_board   == NULL)		return -1;
	if(h_drv_channel == NULL)		return -2;

	// ...
	unsigned char rsp_data[RCS_SEGMENT_LEN];
	unsigned char bLen;

	CString ch_data;

	// ...
	int err_check__count = 0;
	CString err_sts = "???";

	for(int i=0; i<iSLAVE_COUNT; i++)
	{
		int err_check__id = 0;

		CString ref__mac_id = sCH__DNET_CFG__SLAVE_X__MACID[i]->Get__STRING();
		aCH__PARA_SLAVE_REQ_MAC_ID->Set__DATA(ref__mac_id);

		// ...
		{
			log_add.Format("%1d) Slave_Info ... \n", i);
			log_msg += log_add;

			log_add.Format("  * MacID <- [%s] \n", ref__mac_id);
			log_msg += log_add;
		}

		// Error ID ...
		if(!active_update)
		{
			CString str_id = sCH__DNET_INFO__SLAVE_X__ERROR_CHECK_ID[i]->Get__STRING();
			if(str_id.GetLength() > 0)
			{
				int err_id = atof(ch_data);
				if(err_id == 0)			continue;
			}
		}

		// Get the name of the Device from the network
		if((active_update)
		|| (sCH__DNET_INFO__SLAVE_X__NAME[i]->Check__DATA(err_sts) > 0))
		{
			CString para__slave_info = STR__PRODUCT_NAME;

			Fnc__DNET_CMD__GET_ATT_REQ(h_drv_channel, para__slave_info);

			if(dCH__PARA_SLAVE_RSP_STATE->Check__DATA(STR__OK) > 0)
			{
				CString ch_data = sCH__PARA_SLAVE_RSP_DATA_1->Get__STRING();
				sCH__DNET_INFO__SLAVE_X__NAME[i]->Set__DATA(ch_data);

				log_add.Format("  * Device Name <- [%s] \n", sCH__DNET_INFO__SLAVE_X__NAME[i]->Get__STRING());
				log_msg += log_add;
			}
			else
			{
				err_check__id = 1;

				sCH__DNET_INFO__SLAVE_X__NAME[i]->Set__DATA(err_sts);
				err_check__count++;
			}
		}
		
		// get the produced and consumed connection size
		{
			// In.Byte ...
			if((active_update)
			|| (sCH__DNET_INFO__SLAVE_X__IN_SIZE_USE[i]->Check__DATA(err_sts) > 0))
			{
				CString para__slave_info = STR__IN_BYTE;

				Fnc__DNET_CMD__GET_ATT_REQ(h_drv_channel, para__slave_info);

				if(dCH__PARA_SLAVE_RSP_STATE->Check__DATA(STR__OK) > 0)
				{
					CString ch_data = sCH__PARA_SLAVE_RSP_DATA_1->Get__STRING();
					sCH__DNET_INFO__SLAVE_X__IN_SIZE_USE[i]->Set__DATA(ch_data);

					log_add.Format("  * ProducedSize(In) <- [%s] \n", ch_data);
					log_msg += log_add;
				}
				else
				{
					err_check__id = 11;

					sCH__DNET_INFO__SLAVE_X__IN_SIZE_USE[i]->Set__DATA(err_sts);
					err_check__count++;
				}
			}

			// Out.Byte ...
			if((active_update)
			|| (sCH__DNET_INFO__SLAVE_X__OUT_SIZE_USE[i]->Check__DATA(err_sts) > 0))
			{
				CString para__slave_info = STR__OUT_BYTE;

				Fnc__DNET_CMD__GET_ATT_REQ(h_drv_channel, para__slave_info);

				if(dCH__PARA_SLAVE_RSP_STATE->Check__DATA(STR__OK) > 0)
				{
					CString ch_data = sCH__PARA_SLAVE_RSP_DATA_1->Get__STRING();
					sCH__DNET_INFO__SLAVE_X__OUT_SIZE_USE[i]->Set__DATA(ch_data);

					log_add.Format("  * ConsumedSize(Out) <- [%s] \n", ch_data);
					log_msg += log_add;
				}
				else
				{
					err_check__id = 12;

					sCH__DNET_INFO__SLAVE_X__OUT_SIZE_USE[i]->Set__DATA(err_sts);
					err_check__count++;
				}
			}
		}

		ch_data.Format("%1d", err_check__id);
		sCH__DNET_INFO__SLAVE_X__ERROR_CHECK_ID[i]->Set__DATA(ch_data);
	}

	Write__APP_LOG(log_msg);
	return err_check__count;
}

int  CObj__DRV_TEST::_Check__DEV_TOTAL_MEMORY()
{
	unsigned short total__in_byte  = 0;
	unsigned short total__out_byte = 0;

	CString ch_data;
	int cur_byte;

	if(iActive__SIM_MODE > 0)
	{
		for(int i=0; i<iSLAVE_COUNT; i++)
		{
			ch_data = sCH__DNET_CFG__SLAVE_X__NAME[i]->Get__STRING();
			sCH__DNET_INFO__SLAVE_X__NAME[i]->Set__DATA(ch_data);

			//
			ch_data = sCH__DNET_CFG__SLAVE_X__IN_SIZE_USE[i]->Get__STRING();
			sCH__DNET_INFO__SLAVE_X__IN_SIZE_USE[i]->Set__DATA(ch_data);

			ch_data = sCH__DNET_CFG__SLAVE_X__OUT_SIZE_USE[i]->Get__STRING();
			sCH__DNET_INFO__SLAVE_X__OUT_SIZE_USE[i]->Set__DATA(ch_data);

			//
			ch_data = sCH__DNET_CFG__SLAVE_X__IN_SIZE_USE[i]->Get__STRING();
			cur_byte = atoi(ch_data);
			total__in_byte += cur_byte;

			ch_data = sCH__DNET_CFG__SLAVE_X__OUT_SIZE_USE[i]->Get__STRING();
			cur_byte = atoi(ch_data);
			total__out_byte += cur_byte;
		}
	}
	else
	{
		for(int i=0; i<iSLAVE_COUNT; i++)
		{
			ch_data = sCH__DNET_INFO__SLAVE_X__IN_SIZE_USE[i]->Get__STRING();
			cur_byte = atoi(ch_data);
			total__in_byte += cur_byte;

			ch_data = sCH__DNET_INFO__SLAVE_X__OUT_SIZE_USE[i]->Get__STRING();
			cur_byte = atoi(ch_data);
			total__out_byte += cur_byte;
		}
	}

	// ...
	{
		ch_data.Format("%1d", total__in_byte);
		sCH__DNET_INFO__TOTAL_IN_BYTE->Set__DATA(ch_data);

		ch_data.Format("%1d", total__out_byte);
		sCH__DNET_INFO__TOTAL_OUT_BYTE->Set__DATA(ch_data);
	}
	return 1;
}

int  CObj__DRV_TEST::_Check__CFG_SLAVE_INFO()
{
	int total__in_use  = 0;
	int total__out_use = 0;

	CString ch_data;

	// Node.Check ...
	for(int i=0; i<iSLAVE_COUNT; i++)
	{
		ch_data = sCH__DNET_CFG__SLAVE_X__IN_SIZE_USE[i]->Get__STRING();
		total__in_use += atoi(ch_data);

		ch_data = sCH__DNET_CFG__SLAVE_X__OUT_SIZE_USE[i]->Get__STRING();
		total__out_use += atoi(ch_data);
	}

	// ...
	{
		ch_data.Format("%1d", total__in_use);
		sCH__DNET_CFG__TOTAL_IN_BYTE_USE->Set__DATA(ch_data);

		ch_data.Format("%1d", total__out_use);
		sCH__DNET_CFG__TOTAL_OUT_BYTE_USE->Set__DATA(ch_data);
	}
	return 1;
}
