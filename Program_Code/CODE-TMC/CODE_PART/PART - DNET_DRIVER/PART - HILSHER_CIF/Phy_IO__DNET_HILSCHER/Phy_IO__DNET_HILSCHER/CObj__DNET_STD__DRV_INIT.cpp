#include "StdAfx.h"
#include "CObj__DNET_STD.h"
#include "CObj__DNET_STD__DEF.h"

#include "cifuser.h"
#include "rcs_user.h"
#include "dnm_user.h"


#define SERVER_CONSUMED_CONNECTION_SIZE			8
#define SERVER_PRODUCED_CONNECTION_SIZE			8


//--------------------------------------------------------------------------------
int CObj__DNET_STD::_Init__DNET_MASTER_BY_USER_CFG()
{
	bActive__DNET_INIT = false;

	// ...
	{
		CString ch_data = "0";

		sCH__DNET_INFO__TOTAL_OUT_BYTE->Set__DATA(ch_data);
		sCH__DNET_INFO__TOTAL_IN_BYTE->Set__DATA(ch_data);
	}

	if(mDNet_Mng.Call__BOARD_OPEN() < 0)
	{
		return -1;
	}
	if(mDNet_Mng.Call__SCAN_START() < 0)
	{
		return -1;
	}

	bActive__DNET_INIT = true;
	return 1;
}

int CObj__DNET_STD::_Init__DNET_MASTER_BY_AUTO_CFG()
{
	int dev_id = 0;

	// ...
	BOARD_INFO      tBoardInfo;
	FIRMWAREINFO    tFirmwareInfo;
	DRIVERINFO	    tDriverInfo;
	RCS_MESSAGE     tMsg;
	RCS_MESSAGE     tMsgDownload;

	DNM_DOWNLOAD_REQUEST          *ptDownloadRequest;
	DNM_DEV_PRM_HEADER            *ptDevPrmHeader;
	DNM_PRED_MSTSL_CFG_DATA       *ptPredMstslCfgData;
	DNM_PRED_MSTSL_CONNOBJ        *ptPredMstslConnobj;
	DNM_PRED_MSTSL_IO_OBJ_HEADER  *ptPredMstslIoObjHeader;
	DNM_IO_MODULE                 *ptIoModule;
	DNM_PRED_MSTSL_ADD_TAB        *ptPredMstslAddTab;
	DNM_EXPL_SET_ATTR_DATA        *ptExplSetAttrData;
	DNM_UCMM_CONN_OBJ_CFG_DATA    *ptUcmmConnObjCfgData;
	DNM_UCMM_CONN_OBJ_ADD_TAB     *ptUcmmConnObjAddTab;

	BUS_DNM *ptBusDnm;

	DNM_AUTO_CONFIG_REQUEST   *ptAutoConfigReq;
	DNM_AUTO_CONFIG_CONFIRM   *ptAutoConfigCnf;

	// reserve response buffer for message 
	unsigned char   abResponseData[RCS_SEGMENT_LEN];
	unsigned char   bProducedSize   = 0;		// produced size of connection
	unsigned char   bConsumedSize   = 0;		// consumed size of connection
	unsigned short  usVendorID      = 0;		// Vendor ID
	unsigned short  usDeviceType    = 0;		// Device Type
	unsigned short  usProductCode   = 0;		// Product code
	unsigned char   bMajorRevision  = 0;		// Revision major digit
	unsigned char   bMinorRevision  = 0;		// Revision minor digit
	unsigned char   bConnectionType = 0;		// type of I/O connection that is established

	unsigned short  usInstance      = 0;
	unsigned char   bLen            = 0;

	unsigned char   bOwnMacId = 0;
	unsigned char   bBaudrate = DNM_BAUD_500;

	// ...
	CString log_add;
	CString log_msg;

	CString ch_data;
	short s_ret;


	// ...
	{
		ch_data = "0";

		sCH__DNET_INFO__TOTAL_OUT_BYTE->Set__DATA(ch_data);
		sCH__DNET_INFO__TOTAL_IN_BYTE->Set__DATA(ch_data);
	}

	// open driver first
	if((s_ret = DevOpenDriver(dev_id)) != DRV_NO_ERROR) 
	{
		printf("DevOpenDriver(0): %d\n", s_ret);
	} 
	else if((s_ret = DevGetBoardInfo(0, sizeof(tBoardInfo), &tBoardInfo)) != DRV_NO_ERROR ) 
	{
		log_msg.Format("DevGetBoardInfo(0, sizeof(BOARD_INFO), %p): %d\n", tBoardInfo, s_ret);
		log_add += log_msg;

		printf(log_msg);
		DevCloseDriver(0);
	} 
	else 
	{
		// scan all available boards for DNM-firmware 
		int i;

		for(i=0; i<MAX_DEV_BOARDS; i++) 
		{
			if(tBoardInfo.tBoard[i].usAvailable == 1) 
			{
				// found an available board, check if it has the DNM firmware
				if((s_ret = DevInitBoard(tBoardInfo.tBoard[i].usBoardNumber, NULL)) != DRV_NO_ERROR) 
				{
					// printf("DevInitBoard(%d, NULL): %d, board skipped\n", tBoardInfo.tBoard[i].usBoardNumber, sRet);
				} 
				else if((s_ret = DevGetInfo(tBoardInfo.tBoard[i].usBoardNumber,
					GET_FIRMWARE_INFO,
					sizeof(tFirmwareInfo),
					&tFirmwareInfo)) != DRV_NO_ERROR) 
				{
					DevExitBoard(tBoardInfo.tBoard[i].usBoardNumber);
				} 
				else if(strncmp((char*)&tFirmwareInfo.abFirmwareName[0], "DNM     ", 8) ) 
				{
					DevExitBoard(tBoardInfo.tBoard[i].usBoardNumber);
				} 
				else 
				{
					break;
				} 
			}
		}

		if(i >= MAX_DEV_BOARDS) 
		{
			log_msg.Format("No DeviceNet master board found.\n");
			log_add += log_msg;

			printf(log_add);
			return -1;
		}

		// found a board with DNM firmware
		{
			iDNet_BoardNumber = tBoardInfo.tBoard[i].usBoardNumber;

			// ...
			{
				printf("Board %d: ", iDNet_BoardNumber);

				ch_data.Format("%1d", iDNet_BoardNumber);
				sCH__DNET_INFO__MASTER_BOARD_ID->Set__DATA(ch_data);
			}

			// print firmware information 
			{
				ch_data = "";

				for(i = 0; i < 16; i++)
				{
					char ch = tFirmwareInfo.abFirmwareName[i];

					putchar(ch);
					ch_data += ch;
				}

				putchar(' ');
				ch_data += ' ';

				for(i = 0; i < 16; i++)
				{
					char ch = tFirmwareInfo.abFirmwareVersion[i];

					putchar(ch);
					ch_data += ch;
				}

				sCH__DNET_INFO__MASTER_BOARD_NAME->Set__DATA(ch_data);
				printf("\n");
			}
		}


		// reset board first to check if it is configured by SyCon
		do
		{
			if((s_ret = DevReset(iDNet_BoardNumber, COLDSTART, 5000L)) != DRV_NO_ERROR ) 
			{
				printf("DevReset(%d, COLDSTART, 5000): %d\n", iDNet_BoardNumber, s_ret);

				__CLOSE__OBJECT();
				return -1;
			}        

			if((s_ret = DevGetInfo(iDNet_BoardNumber, GET_DRIVER_INFO, sizeof(DRIVERINFO), &tDriverInfo)) != DRV_NO_ERROR ) 
			{
				printf("DevGetInfo(...): %d\n", s_ret); 

				__CLOSE__OBJECT();
				return -1;
			}

			// check if RUN bit of device is set, then card is configured with SyCon 

			if((tDriverInfo.bHostFlags & 0x40) != 0)
			{
				// clear data base in DEVICE 
				tMsg.rx = 0;		// task = OS-Task
				tMsg.tx = 16;
				tMsg.ln = 2;
				tMsg.nr = 0;
				tMsg.a  = 0;
				tMsg.f  = 0;
				tMsg.b  = 6;		// command data base access
				tMsg.e  = 0;
				tMsg.d[0] = 4;		// mode clear data base
				tMsg.d[1] = 8;		// startsegment = 8

				if((s_ret = DevPutMessage(iDNet_BoardNumber, (MSG_STRUC *)&tMsg, 5000L)) == DRV_NO_ERROR ) 
				{
					// message successfully sent
					// card will jump into bootloader mode after sending this message and will be not RDY for a short moment

					while( (s_ret = DevGetMessage(iDNet_BoardNumber, sizeof(tMsg), (MSG_STRUC *)&tMsg, 0)) == DRV_DEV_GET_NO_MESSAGE )
					{
						continue;
					}

					if(s_ret == DRV_NO_ERROR) 
					{
						// message successfully received, data base should be deleted now
						continue;
					}
					else 
					{
						printf("\nDevGetMessage(%d, sizeof(tMsg), &tMsg, 500): %d\n", iDNet_BoardNumber, (int)s_ret);

						__CLOSE__OBJECT();
						return -1;
					}
				}
				else 
				{
					// an error occcurred
					printf("\nDevPutMessage(%d, &tMsg, 5000): %d\n", iDNet_BoardNumber, (int) s_ret);

					__CLOSE__OBJECT();
					return -1;
				} 
			}
			else
			{
				break;
			}
		} 
		while(1);

		// perform automatic network scan first to get which devices are connected 
		bOwnMacId = 0;

		do
		{
			tMsg.rx = 3;		// task = DNM-Task
			tMsg.tx = 16;
			tMsg.ln = sizeof( DNM_AUTO_CONFIG_REQUEST );
			tMsg.nr = 0;
			tMsg.a  = 0;
			tMsg.f  = 0;
			tMsg.b  = DNM_Auto_Config; 
			tMsg.e  = 0;

			ptAutoConfigReq = (DNM_AUTO_CONFIG_REQUEST*) &tMsg.d[0];

			// ...
			{
				sCH__DNET_INFO__BAUD_RATE->Set__DATA("500K");

				ptAutoConfigReq->bBaudrate = DNM_BAUD_500;
				bBaudrate = DNM_BAUD_500;
				printf("\nscanning the network at 500kBaud");

				// ptAutoConfigReq->bBaudrate = DNM_BAUD_250;
				// bBaudrate = DNM_BAUD_250;
				// printf("\nscanning the network at 250kBaud");

				// ptAutoConfigReq->bBaudrate = DNM_BAUD_125;
				// bBaudrate = DNM_BAUD_125;
				// printf("\nscanning the network at 125kBaud");
			}

			// ...
			ptAutoConfigReq->bOwnMacId = bOwnMacId;

			if((s_ret = DevPutMessage(iDNet_BoardNumber, (MSG_STRUC *)&tMsg, 5000L)) == DRV_NO_ERROR) 
			{
				if((s_ret = DevGetMessage(iDNet_BoardNumber, sizeof(tMsg), (MSG_STRUC *)&tMsg, 50000L)) == DRV_NO_ERROR) 
				{
					if(tMsg.f == TASK_F_OK)
					{
						break;
					}
					else if( tMsg.f == TASK_F_DUPLICATE_MAC_ID )
					{
						// try another own MAC ID

						if(bOwnMacId++ >= DNM_MAX_NUM_DEVICES )
						{
							printf("\nEvery MAC-ID occupied, can't scan the network\n");

							__CLOSE__OBJECT();
							return -1;
						}

						printf("\nDuplicate MAC-ID detected, try value: %d now\n", bOwnMacId);
						i = (unsigned short)(i - 1);
					}
					else if(tMsg.f == TASK_F_NO_CAN)
					{
						printf("\nused baud rate indicates no device at the same baud rate\n");
					}
				}
				else 
				{
					printf("\nDevGetMessage(%d, sizeof(tMsg), &tMsg, 500): %d\n", iDNet_BoardNumber, (int)s_ret);

					__CLOSE__OBJECT();
					return -1;
				} 
			}
			else 
			{
				printf("\nDevPutMessage(%d, &tMsg, 5000): %d\n", iDNet_BoardNumber, (int)s_ret);

				__CLOSE__OBJECT();
				return -1;
			} 
		}
		while(false);

		// ...		
		usInputOffset  = 0;
		usOutputOffset = 0;

		ptAutoConfigCnf = ( DNM_AUTO_CONFIG_CONFIRM* ) &tMsg.d[0];

		for(i=0; i<DNM_MAX_NUM_DEVICES; i++)
		{
			if(ptAutoConfigCnf->abSuppFnc[i] != 0)
			{
				int index = iSLAVE_COUNT++;

				// ...
				{
					ch_data.Format("%1d", iSLAVE_COUNT);
					sCH__DNET_INFO__SLAVE_COUNT->Set__DATA(ch_data);
				}

				// ...
				{
					log_msg.Format("\nDevice ID %d detected, named : ", i);
					log_add += log_msg;

					printf(log_msg);

					if(index < CFG__SLAVE_SIZE)
					{
						ch_data.Format("%1d", i);
						sCH__DNET_INFO__SLAVE_X__MACID[index]->Set__DATA(ch_data);
					}
				}

				// get the name of the Device from the network
				bLen = DNet__ReadDeviceData((unsigned char) i,
											DNM_IDENTITY_CLASS, 
											1 , 
											DNM_IDOBJ_NAME, 
											&abResponseData[0]);

				if(bLen != 0)
				{
					// terminate string with 0
					abResponseData[bLen] = 0;

					log_msg.Format("%s",&abResponseData[1]);
					log_add += log_msg;

					printf(log_msg);

					if(index < CFG__SLAVE_SIZE)
					{
						ch_data = log_msg;
						sCH__DNET_INFO__SLAVE_X__NAME[index]->Set__DATA(ch_data);
					}
				}

				bLen = DNet__ReadDeviceData((unsigned char) i,
											DNM_IDENTITY_CLASS, 
											1 , 
											DNM_IDOBJ_VENDOR_ID, 
											&abResponseData[0]);
				if(bLen != 0)
				{
					usVendorID = *((unsigned short*)&abResponseData[0]);
				}

				bLen = DNet__ReadDeviceData((unsigned char) i,
											DNM_IDENTITY_CLASS, 
											1 , 
											DNM_IDOBJ_DEV_TYPE, 
											&abResponseData[0]);
				if(bLen != 0)
				{
					usDeviceType = *((unsigned short*)&abResponseData[0]);
				}

				bLen = DNet__ReadDeviceData((unsigned char) i,
											DNM_IDENTITY_CLASS, 
											1 , 
											DNM_IDOBJ_PROD_CODE, 
											&abResponseData[0]);
				if(bLen != 0)
				{
					usProductCode = *((unsigned short*)&abResponseData[0]);
				}

				bLen = DNet__ReadDeviceData((unsigned char) i,
											DNM_IDENTITY_CLASS, 
											1 , 
											DNM_IDOBJ_REV, 
											&abResponseData[0]);
				if(bLen != 0)
				{
					bMajorRevision  = abResponseData[0];
					bMinorRevision  = abResponseData[1];
				}

				// get the produced and consumed connection size
				if((ptAutoConfigCnf->abSuppFnc[i] & DNM_SUPP_FNC_POLLED) != 0)
				{
					usInstance = DNM_POLLED_CNXN_INSTANCE;
					bConnectionType = DNM_TYPE_POLLED;
				}
				else if((ptAutoConfigCnf->abSuppFnc[i] & DNM_SUPP_FNC_CHANGEOFSTATE) != 0)
				{
					usInstance = DNM_COS_CYCLIC_CNXN_INSTANCE;
					bConnectionType = DNM_TYPE_CYCLIC;
				}
				else if((ptAutoConfigCnf->abSuppFnc[i] & DNM_SUPP_FNC_CYCLIC) != 0)
				{
					usInstance = DNM_COS_CYCLIC_CNXN_INSTANCE;
					bConnectionType = DNM_TYPE_CHG_OF_STATE;            
				}
				else if( (ptAutoConfigCnf->abSuppFnc[i] & DNM_SUPP_FNC_BITSTROBED) != 0 )
				{
					usInstance = DNM_BIT_STROBED_CNXN_INSTANCE;
					bConnectionType = DNM_TYPE_BIT_STROBED;
				}
				else
				{
					usInstance = DNM_EXPLICIT_CNXN_INSTANCE;
					bConnectionType = 0;
				}

				bLen = DNet__ReadDeviceData((unsigned char) i,
											DNM_CNXN_CLASS, 
											2,							// usInstance, 
											DNM_CNXN_PSIZE_ATTR_ID, 
											&abResponseData[0]);
				if( bLen != 0 )
				{
					bProducedSize = abResponseData[0];
				}
				else
				{
					bProducedSize = 0;
				}

				bLen = DNet__ReadDeviceData((unsigned char) i,
											DNM_CNXN_CLASS, 
											2,							// usInstance, 
											DNM_CNXN_CSIZE_ATTR_ID, 
											&abResponseData[0]);
				if(bLen != 0)
				{
					bConsumedSize = abResponseData[0];
				}
				else
				{
					bConsumedSize = 0;
				}

				// ...
				{
					log_msg.Format("\n *** bConsumedSize : %1d", bConsumedSize);
					log_add += log_msg;

					printf(log_msg);

					//
					log_msg.Format("\n *** bProducedSize : %1d", bProducedSize);
					log_add += log_msg;

					printf(log_msg);
				}

				if(index < CFG__SLAVE_SIZE)
				{
					ch_data.Format("%1d", bConsumedSize);
					sCH__DNET_INFO__SLAVE_X__OUT_SIZE_USE[index]->Set__DATA(ch_data);

					ch_data.Format("%1d", bProducedSize);
					sCH__DNET_INFO__SLAVE_X__IN_SIZE_USE[index]->Set__DATA(ch_data);
				}

				// load device data set now         
				tMsgDownload.rx = 3;  
				tMsgDownload.tx = 16;
				tMsgDownload.nr = 0;
				tMsgDownload.a  = 0;
				tMsgDownload.f  = 0;
				tMsgDownload.b  = DNM_Download; 
				tMsgDownload.e  = 0;

				ptDownloadRequest = (DNM_DOWNLOAD_REQUEST *) &tMsgDownload.d[0];

				// load the download header area first
				ptDownloadRequest->bReq_Add     = 0;
				ptDownloadRequest->bArea_Code   = (unsigned char ) i;	// MAC address
				ptDownloadRequest->usAdd_Offset = 0;					// no offset start from the beginning

				ptDevPrmHeader = (DNM_DEV_PRM_HEADER *) &ptDownloadRequest->abData[0];

				// enable device key check
				ptDevPrmHeader->bDvFlag.bActive       = 1;
				ptDevPrmHeader->bDvFlag.bDeviceType   = 1;
				ptDevPrmHeader->bDvFlag.bProductCode  = 1;
				ptDevPrmHeader->bDvFlag.bVendorID     = 1;
				ptDevPrmHeader->bDvFlag.bRevision     = 1;

				ptDevPrmHeader->bDvFlag.bReserved1    = 0;
				ptDevPrmHeader->bDvFlag.bReserved2    = 0;

				ptDevPrmHeader->bOctetString[0] = 0;
				ptDevPrmHeader->bOctetString[1] = 0;

				// is device supporting UCMM ?
				if( (ptAutoConfigCnf->abSuppFnc[i] & DNM_SUPP_FNC_UCMM) != 0)
				{
					ptDevPrmHeader->bDvFlag.bUcmmSup = 1;
					ptDevPrmHeader->bUcmmGroup = ptAutoConfigCnf->abUcmmGroup[i];
				}
				else
				{
					ptDevPrmHeader->bDvFlag.bUcmmSup = 0;
					ptDevPrmHeader->bUcmmGroup = 0;
				}

				// set up device specific key parameter
				ptDevPrmHeader->usVendorID      = usVendorID;
				ptDevPrmHeader->usDeviceType    = usDeviceType;
				ptDevPrmHeader->usProductCode   = usProductCode;
				ptDevPrmHeader->bMajorRevision  = bMajorRevision;
				ptDevPrmHeader->bMinorRevision  = bMinorRevision;

				ptDevPrmHeader->usRecFragTimer  = 1000;
				ptDevPrmHeader->usDevParaLen    = sizeof( DNM_DEV_PRM_HEADER );

				ptPredMstslCfgData = (DNM_PRED_MSTSL_CFG_DATA *) ((unsigned char*)ptDevPrmHeader +ptDevPrmHeader->usDevParaLen);

				ptPredMstslCfgData->usPredMstslCfgDataLen = 2;
				ptPredMstslConnobj      = (DNM_PRED_MSTSL_CONNOBJ *)&ptPredMstslCfgData->atConnObjInst;
				ptPredMstslIoObjHeader  = (DNM_PRED_MSTSL_IO_OBJ_HEADER *)&ptPredMstslConnobj->tPredMstSlObjHeader;

				if(bConnectionType != 0)
				{
					ptPredMstslCfgData->usPredMstslCfgDataLen += sizeof( DNM_PRED_MSTSL_IO_OBJ_HEADER );

					ptPredMstslIoObjHeader->bConnectionType         = bConnectionType;
					ptPredMstslIoObjHeader->bWatchdogTimeoutAction  = DNM_TRANSMISSION_TO_TIMEOUT;
					ptPredMstslIoObjHeader->usExpPacketRate         = 10;
					ptPredMstslIoObjHeader->usProdInhibitTime       = 0;
					ptPredMstslIoObjHeader->bNumOfIOModules         = 0;

					ptIoModule = (DNM_IO_MODULE *) ((unsigned char *) ptPredMstslIoObjHeader +sizeof( DNM_PRED_MSTSL_IO_OBJ_HEADER));

					if(bConsumedSize)		// check for outputs in the view of the DEVICE
					{
						ptPredMstslIoObjHeader->bNumOfIOModules++;
						ptIoModule->bDataType = TASK_TDT_STRING|DNM_OUTPUT;
						ptIoModule->bDataSize = bConsumedSize;
						ptIoModule++;
						ptPredMstslCfgData->usPredMstslCfgDataLen += sizeof( DNM_IO_MODULE);
					}

					if(bProducedSize)		// check for inputs in the view of the DEVICE
					{
						ptPredMstslIoObjHeader->bNumOfIOModules++;
						ptIoModule->bDataType = TASK_TDT_STRING;
						ptIoModule->bDataSize = bProducedSize;
						ptIoModule++;
						ptPredMstslCfgData->usPredMstslCfgDataLen += sizeof( DNM_IO_MODULE);
					}

					ptPredMstslAddTab = (DNM_PRED_MSTSL_ADD_TAB *) ((unsigned char *)ptPredMstslCfgData +ptPredMstslCfgData->usPredMstslCfgDataLen);

					ptPredMstslAddTab->usAddTabLen  = 4;
					ptPredMstslAddTab->bInputCount  = 0;
					ptPredMstslAddTab->bOutputCount = 0;

					if( bProducedSize )		// check for input data 
					{
						ptPredMstslAddTab->ausIOOffsets[ptPredMstslAddTab->bInputCount] = usInputOffset;
						usInputOffset = (unsigned short)(usInputOffset + bProducedSize);
						ptPredMstslAddTab->bInputCount++;
						ptPredMstslAddTab->usAddTabLen += sizeof( unsigned short );
					}

					if(bConsumedSize)		// check for output data
					{
						ptPredMstslAddTab->ausIOOffsets[ptPredMstslAddTab->bOutputCount+ptPredMstslAddTab->bInputCount] = usOutputOffset;
						usOutputOffset = (unsigned short)(usOutputOffset + bConsumedSize);
						ptPredMstslAddTab->bOutputCount++;
						ptPredMstslAddTab->usAddTabLen += sizeof( unsigned short );
					}

					ptExplSetAttrData = (DNM_EXPL_SET_ATTR_DATA *) ((unsigned char *)ptPredMstslAddTab +ptPredMstslAddTab->usAddTabLen);

					ptExplSetAttrData->usAttrDataLen = 2;
					ptUcmmConnObjCfgData = (DNM_UCMM_CONN_OBJ_CFG_DATA *) ((unsigned char *)ptExplSetAttrData +ptExplSetAttrData->usAttrDataLen);

					ptUcmmConnObjCfgData->usCfgDataLen = 2;
					ptUcmmConnObjAddTab   = (DNM_UCMM_CONN_OBJ_ADD_TAB *) ((unsigned char *)ptUcmmConnObjCfgData +ptUcmmConnObjCfgData->usCfgDataLen);

					ptUcmmConnObjAddTab->usAddTabLen  = 4;
					ptUcmmConnObjAddTab->bInputCount  = 0;
					ptUcmmConnObjAddTab->bOutputCount = 0;

					// sum up whole length	            
					ptDevPrmHeader->usDevParaLen = (unsigned short)(ptDevPrmHeader->usDevParaLen + ptPredMstslCfgData->usPredMstslCfgDataLen);
					ptDevPrmHeader->usDevParaLen = (unsigned short)(ptDevPrmHeader->usDevParaLen + ptPredMstslAddTab->usAddTabLen);
					ptDevPrmHeader->usDevParaLen = (unsigned short)(ptDevPrmHeader->usDevParaLen + ptExplSetAttrData->usAttrDataLen);
					ptDevPrmHeader->usDevParaLen = (unsigned short)(ptDevPrmHeader->usDevParaLen + ptUcmmConnObjCfgData->usCfgDataLen);
					ptDevPrmHeader->usDevParaLen = (unsigned short)(ptDevPrmHeader->usDevParaLen + ptUcmmConnObjAddTab->usAddTabLen);

					// subtract defined data area
					tMsgDownload.ln = (unsigned char)( ptDevPrmHeader->usDevParaLen +
						sizeof( DNM_DOWNLOAD_REQUEST ) 
						- MAX_LEN_DATA_UNIT);		

					if((s_ret = DevPutMessage(iDNet_BoardNumber, (MSG_STRUC *)&tMsgDownload, 500L)) == DRV_NO_ERROR ) 
					{
						if((s_ret = DevGetMessage(iDNet_BoardNumber, sizeof(tMsgDownload), (MSG_STRUC *)&tMsgDownload, 500L)) == DRV_NO_ERROR ) 
						{
							if(tMsgDownload.f != TASK_F_OK)
							{
								printf("Error in loading data set. DEVICE does not accept data set");

								__CLOSE__OBJECT();
								return -1;
							}		                
						}
						else 
						{
							printf("\nDevGetMessage(%d, sizeof(tMsgDownload), &tMsgDownload, 500): %d\n", iDNet_BoardNumber, (int) s_ret);

							__CLOSE__OBJECT();
							return -1;
						}
					}
					else 
					{
						printf("\nDevPutMessage(%d, &tMsgDownload, 500): %d\n", iDNet_BoardNumber, (int) s_ret);

						__CLOSE__OBJECT();
						return -1;
					}
				}
			}
		}


		// every device data set is loaded now, load the master parameter data set as next
		tMsgDownload.rx = 3;  
		tMsgDownload.tx = 16;
		tMsgDownload.nr = 0;
		tMsgDownload.a  = 0;
		tMsgDownload.f  = 0;
		tMsgDownload.b  = DNM_Download; 
		tMsgDownload.e  = 0;

		ptDownloadRequest = (DNM_DOWNLOAD_REQUEST *) &tMsgDownload.d[0];

		// load the download header area first
		ptDownloadRequest->bReq_Add     = 0;
		ptDownloadRequest->bArea_Code   = DNM_DEVICE_PRM;
		ptDownloadRequest->usAdd_Offset = 0; 

		ptBusDnm = (BUS_DNM *) &ptDownloadRequest->abData[0];

		ptBusDnm->bBaudrate   = bBaudrate;
		ptBusDnm->bOwnMacId   = bOwnMacId;

		ptBusDnm->bReserved   = 0;
		ptBusDnm->usReserved  = 0;

		ptBusDnm->bSrvConsConnSize = SERVER_CONSUMED_CONNECTION_SIZE; 
		ptBusDnm->usConsOffset = usInputOffset;
		usInputOffset += SERVER_CONSUMED_CONNECTION_SIZE;

		ptBusDnm->bSrvProdConnSize = SERVER_PRODUCED_CONNECTION_SIZE; 
		ptBusDnm->usProdOffset = usOutputOffset;
		usOutputOffset += SERVER_PRODUCED_CONNECTION_SIZE;

		ptBusDnm->bAutoClear = DNM_ACLR_INACTIVE;
		ptBusDnm->usVendorId = DNM_HILSCHER_VENDOR;

		tMsgDownload.ln = sizeof(BUS_DNM) + sizeof(DNM_DOWNLOAD_REQUEST) - MAX_LEN_DATA_UNIT;         

		if((s_ret = DevPutMessage(iDNet_BoardNumber, (MSG_STRUC *)&tMsgDownload, 500L)) == DRV_NO_ERROR) 
		{
			if((s_ret = DevGetMessage(iDNet_BoardNumber, sizeof(tMsgDownload), (MSG_STRUC *)&tMsgDownload, 3000L)) == DRV_NO_ERROR ) 
			{
				if(tMsgDownload.f != TASK_F_OK)
				{
					printf("Error in loading data set. DEVICE does not accept bus parameter data set");

					__CLOSE__OBJECT();
					return -1;
				}
			}
			else 
			{
				printf("\nDevGetMessage(%d, sizeof(tMsgDownload), &tMsgDownload, 3000): %d\n", iDNet_BoardNumber, (int) s_ret);

				__CLOSE__OBJECT();
				return -1;
			}
		}
		else 
		{
			printf("\nDevPutMessage(%d, &tMsgDownload, 500): %d\n", iDNet_BoardNumber, (int) s_ret);

			__CLOSE__OBJECT();
			return -1;
		}
	} 

	// ...
	{
		ch_data.Format("%1d", usOutputOffset);
		sCH__DNET_INFO__TOTAL_OUT_BYTE->Set__DATA(ch_data);

		ch_data.Format("%1d", usInputOffset);
		sCH__DNET_INFO__TOTAL_IN_BYTE->Set__DATA(ch_data);
	}

	// ...
	{
		FILE *pf = fopen("D:\\DNet_Info.txt", "w");

		if(pf != NULL)
		{
			fputs(log_add, pf);
			fclose(pf);
		}
	}
	return 1;
}

unsigned char CObj__DNET_STD
::DNet__ReadDeviceData(unsigned char  bDeviceAdr,
					   unsigned char  bClass,
					   unsigned short usInstance,
					   unsigned char  bAttribute,
					   unsigned char *ptRespData)
{
	int ref_count = (int) aCH__CFG_DRV_INT_RETRY_CHECK->Get__VALUE();
	int cur_count = 0;

	while(cur_count < ref_count)
	{
		int r_flag = _DNet__ReadDeviceData(bDeviceAdr,bClass,usInstance, bAttribute, ptRespData);
		if(r_flag > 0)			return r_flag;

		cur_count++;
	}

	return 0;
}
unsigned char CObj__DNET_STD
::_DNet__ReadDeviceData(unsigned char  bDeviceAdr,
					    unsigned char  bClass,
					    unsigned short usInstance,
					    unsigned char  bAttribute,
					    unsigned char *ptRespData)
{
	double cfg_delay__msec = (int) aCH__CFG_DRV_INT_DELAY_mSEC->Get__VALUE();
	Sleep(cfg_delay__msec);

	// ...
	RCS_MESSAGETELEGRAM_10 tMsgExt;
	short s_ret;

	tMsgExt.rx = 3;  
	tMsgExt.tx = 16;
	tMsgExt.ln = sizeof( RCS_TELEGRAMHEADER_10 );
	tMsgExt.nr = 0;
	tMsgExt.a  = 0;
	tMsgExt.f  = 0;
	tMsgExt.b  = DNM_Get_Set_Attribute; 
	tMsgExt.e  = 0;

	tMsgExt.device_adr = bDeviceAdr;
	tMsgExt.data_area  = bClass;
	tMsgExt.data_adr   = usInstance;
	tMsgExt.data_idx   = bAttribute;
	tMsgExt.data_cnt   = 0;
	tMsgExt.data_type  = 0;
	tMsgExt.function   = TASK_TFC_READ;

	if((s_ret = DevPutMessage(iDNet_BoardNumber, (MSG_STRUC *)&tMsgExt, 10000L)) == DRV_NO_ERROR) 
	{
		if((s_ret = DevGetMessage(iDNet_BoardNumber, sizeof(tMsgExt), (MSG_STRUC *)&tMsgExt, 10000L)) == DRV_NO_ERROR) 
		{
			if(tMsgExt.f == TASK_F_OK)
			{
				memcpy( ptRespData, &tMsgExt.d[0], tMsgExt.data_cnt);
				return ( tMsgExt.data_cnt );
			}
			else
			{
				printf("\n ERROR - tMsgExt.f (%1d) \n", tMsgExt.f);
				return ( 0 );
			}
		}
		else 
		{
			printf("\nDevGetMessage(%d, sizeof(tMsgExt), &tMsgExt, 500): %d\n", iDNet_BoardNumber, (int) s_ret);
			return ( 0 );
		}
	}
	else 
	{
		printf("\nDevPutMessage(%d, &tMsgExt, 500): %d\n", iDNet_BoardNumber, (int) s_ret);
		return ( 0 );
	}

	return( 0 );
}

