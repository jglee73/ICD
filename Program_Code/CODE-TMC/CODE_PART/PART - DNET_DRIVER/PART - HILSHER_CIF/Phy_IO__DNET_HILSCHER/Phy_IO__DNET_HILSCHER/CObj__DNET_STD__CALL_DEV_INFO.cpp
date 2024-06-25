#include "StdAfx.h"
#include "CObj__DNET_STD.h"
#include "CObj__DNET_STD__DEF.h"

#include "cifuser.h"
#include "rcs_user.h"
#include "dnm_user.h"


//-------------------------------------------------------------------------	
int  CObj__DNET_STD
::Call__DEV_INFO(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm)
{
	bool active__retry_check = false;

LOOP_RETRY:

	EnterCriticalSection(&mLOCK_DNET);

	if(active__retry_check)
	{
		printf("Object Name <- [%s] \n", sObject_Name);;
		printf("  * CObj__DNET_STD::Call__DEV_INFO() - Retry !!! \n");

		_Fnc__DEV_INFO( false );
	}
	else
	{
		printf("Object Name <- [%s] \n", sObject_Name);;
		printf("  * CObj__DNET_STD::Call__DEV_INFO() ... \n");

		int err_count = _Fnc__DEV_INFO( true );
		if(err_count > 0)
		{
			err_count = _Fnc__DEV_INFO( false );
		}

		printf("  * err_count <- [%1d] \n", err_count);
	}

	LeaveCriticalSection(&mLOCK_DNET);

	_Check__DEV_TOTAL_MEMORY();

	// Error.Check ...
	{
		CString err_msg;

		if(_Check__SLAVE_ERROR(err_msg) > 0)
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
int  CObj__DNET_STD::_Fnc__DEV_INFO(const bool active_update)
{
	unsigned char abResponseData[RCS_SEGMENT_LEN];
	unsigned char bLen;

	CString log_msg;
	CString log_add;

	CString ch_data;

	// ...
	int err_check__count = 0;
	CString err_sts = "???";

	for(int i=0; i<iSLAVE_COUNT; i++)
	{
		int err_check__id = 0;

		ch_data = sCH__DNET_CFG__SLAVE_X__MACID[i]->Get__STRING();
		unsigned char mac_id = atoi(ch_data);

		// ...
		{
			log_msg.Format("%1d) Slave_Info ... \n", i);

			log_add.Format("  * MacID <- [%1d] \n", mac_id);
			log_msg += log_add;

			printf(log_msg);
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

		/*
		// Get the name of the Device from the network
		if((active_update)
		|| (sCH__DNET_INFO__SLAVE_X__NAME[i]->Check__DATA(err_sts) > 0))
		{
			bLen = DNet__ReadDeviceData((unsigned char) mac_id,
										DNM_IDENTITY_CLASS, 
										1 , 
										DNM_IDOBJ_NAME, 
										&abResponseData[0]);

			if(bLen != 0)
			{
				// terminate string with 0
				abResponseData[bLen] = 0;

				ch_data.Format("%s", &abResponseData[1]);
				sCH__DNET_INFO__SLAVE_X__NAME[i]->Set__DATA(ch_data);

				log_msg.Format("  * Device Name <- [%s] \n",sCH__DNET_INFO__SLAVE_X__NAME[i]->Get__STRING());
				printf(log_msg);
			}
			else
			{
				err_check__id = 1;

				sCH__DNET_INFO__SLAVE_X__NAME[i]->Set__DATA(err_sts);
				err_check__count++;
			}
		}
		*/

		// get the produced and consumed connection size
		if(iActive__SIM_MODE > 0)
		{

		}
		else
		{
			unsigned short usInstance = 0;
			unsigned char bProducedSize = 0;
			unsigned char bConsumedSize = 0; 

			usInstance = DNM_POLLED_CNXN_INSTANCE;
			/*
			usInstance = DNM_COS_CYCLIC_CNXN_INSTANCE;
			usInstance = DNM_COS_CYCLIC_CNXN_INSTANCE;
			usInstance = DNM_BIT_STROBED_CNXN_INSTANCE;
			usInstance = DNM_EXPLICIT_CNXN_INSTANCE;
			*/
	
			// In.Byte ...
			if((active_update)
			|| (sCH__DNET_INFO__SLAVE_X__IN_SIZE_USE[i]->Check__DATA(err_sts) > 0))
			{
				bLen = DNet__ReadDeviceData((unsigned char) mac_id,
											DNM_CNXN_CLASS, 
											usInstance, 
											DNM_CNXN_PSIZE_ATTR_ID, 
											&abResponseData[0]);
				if( bLen != 0 )
				{
					bProducedSize = abResponseData[0];

					log_msg.Format("  * ProducedSize(In) <- [%1d] \n", bProducedSize);
					printf(log_msg);

					ch_data.Format("%1d", bProducedSize);;
					sCH__DNET_INFO__SLAVE_X__IN_SIZE_USE[i]->Set__DATA(ch_data);
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
				bLen = DNet__ReadDeviceData((unsigned char) mac_id,
											DNM_CNXN_CLASS, 
											usInstance, 
											DNM_CNXN_CSIZE_ATTR_ID, 
											&abResponseData[0]);
				if(bLen != 0)
				{
					bConsumedSize = abResponseData[0];

					log_msg.Format("  * ConsumedSize(Out) <- [%1d] \n", bConsumedSize);
					printf(log_msg);

					ch_data.Format("%1d", bConsumedSize);;
					sCH__DNET_INFO__SLAVE_X__OUT_SIZE_USE[i]->Set__DATA(ch_data);
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

	return err_check__count;
}

int  CObj__DNET_STD::_Check__SLAVE_ERROR()
{
	CString err_msg;

	return _Check__SLAVE_ERROR(err_msg);
}
int  CObj__DNET_STD::_Check__SLAVE_ERROR(CString& err_msg)
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

		CString err_info;

		// In-Byte
		ch_data = sCH__DNET_CFG__SLAVE_X__IN_SIZE_USE[i]->Get__STRING();
		if(sCH__DNET_INFO__SLAVE_X__IN_SIZE_USE[i]->Check__DATA(ch_data) < 0)
		{
			active__node_error = true;

			err_bff.Format("  * Config.In Byte : %s \n", sCH__DNET_CFG__SLAVE_X__IN_SIZE_USE[i]->Get__STRING());
			err_info += err_bff;
			err_bff.Format("  * Slave.In Byte  : %s \n", sCH__DNET_INFO__SLAVE_X__IN_SIZE_USE[i]->Get__STRING());
			err_info += err_bff;
		}

		// Out-Byte
		ch_data = sCH__DNET_CFG__SLAVE_X__OUT_SIZE_USE[i]->Get__STRING();
		if(sCH__DNET_INFO__SLAVE_X__OUT_SIZE_USE[i]->Check__DATA(ch_data) < 0)
		{
			active__node_error = true;

			err_bff.Format("  * Config.Out Byte : %s \n", sCH__DNET_CFG__SLAVE_X__OUT_SIZE_USE[i]->Get__STRING());
			err_info += err_bff;
			err_bff.Format("  * Slave.Out Byte  : %s \n", sCH__DNET_INFO__SLAVE_X__OUT_SIZE_USE[i]->Get__STRING());
			err_info += err_bff;
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

			err_bff.Format("Node(%1d) - Error (%s) ! \n", node_id, sCH__DNET_INFO__SLAVE_X__ERROR_CHECK_ID[i]->Get__STRING());
			err_msg += err_bff;
			
			err_bff.Format("  * MacID <- %s \n", sCH__DNET_CFG__SLAVE_X__MACID[i]->Get__STRING());
			err_msg += err_bff;
			
			err_msg += err_info;
		}
	}

	if(active__error_check)			return 1;

	return -1;
}

int  CObj__DNET_STD::_Check__CFG_SLAVE_INFO()
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

int  CObj__DNET_STD::_Check__DEV_TOTAL_MEMORY()
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

		//
		CString log_msg;
		CString log_add;

		log_msg.Format("  * Total In_Byte  <- [%1d] \n", total__in_byte );
		log_add.Format("  * Total Out_Byte <- [%1d] \n", total__out_byte);
		log_msg += log_add;

		printf(log_msg);
	}
	return 1;
}
