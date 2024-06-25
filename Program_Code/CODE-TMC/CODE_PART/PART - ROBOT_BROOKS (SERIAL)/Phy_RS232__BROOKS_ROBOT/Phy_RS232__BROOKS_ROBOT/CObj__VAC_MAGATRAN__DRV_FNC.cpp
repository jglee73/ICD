#include "StdAfx.h"
#include "CObj__VAC_MAGATRAN.h"
#include "CObj__VAC_MAGATRAN__DEF.h"

#include "Macro_Function.h"
#include "CCommon_DEF.h"


//------------------------------------------------------------------------------------
int CObj__VAC_MAGATRAN
::__Read__ANALOG(const CString& var_name, const CDS_IO__CHANNEL_INFO& io_info, double& read_data)
{

	return -1;
}

int CObj__VAC_MAGATRAN
::__Read__DIGITAL(const CString& var_name,const CDS_IO__CHANNEL_INFO& io_info, CString& read_data,int& item_index)
{
	CString str__read_req_data;
	CString str__for_error;
	CString str__log;
	CString str__digital_data;
	int index = 0;

	char check_char1;
	char check_char2;

	if(diCH__ARM_A_READ->Check__VARIABLE_NAME(var_name) > 0)
	{
		str__read_req_data.Format("RQ POS STN A R%c",CR);
		str__for_error.Format("RQ POS STN A R<CR>");

		str__log.Format("[%s] SEND => [%s]\n", var_name,str__for_error);
		Fnc__DRV_LOG(str__log);

		// ...
		CString msg;
		mX_Serial->CLEAR__BUFFER(&msg);

		// ...
		int nRet = mX_Serial->DATA__RECV(m_Term_Str, &str__read_req_data, m_Rcv_Time);

		str__log.Format("Ret:%d", nRet);
		Fnc__DRV_LOG(str__log);

		if(nRet >= 0)
		{
			str__log.Format("[%s] DIRECT_RECV <= [%s]\n", var_name,str__read_req_data);
			Fnc__DRV_LOG(str__log);

			m_nCommState = ONLINE;

			// ...
			str__read_req_data.TrimLeft();

			index = str__read_req_data.GetLength();
			if(index > 11)
			{
				check_char1 = str__read_req_data.GetAt(10);
				check_char2 = str__read_req_data.GetAt(11);

				str__digital_data.Format("%c%c",check_char1,check_char2);

				     if(str__digital_data == "EX")		read_data = "Extended";
				else if(str__digital_data == "RE")		read_data = "Retracted";
				else if(str__digital_data == "--")		read_data = "Unknown";
				else									read_data = "Unknown";

				// ...
				{
					str__log.Format("[%s] RECV <= [%s]\n", var_name,str__read_req_data);
					Fnc__DRV_LOG(str__log);
				}
			}
			else
			{
				read_data = "Unknown";
			}
		}
		else
		{
			m_nCommState = OFFLINE;

			// ...
			{
				str__log.Format("[%s] [Error] => No response Send:(%s) Receive:(%s)\n", 
								var_name,
								str__for_error,
								str__read_req_data);

				Fnc__DRV_LOG(str__log);
			}

			read_data = "Unknown";
		}

		return 1;
	}

	if(diCH__ARM_B_READ->Check__VARIABLE_NAME(var_name) > 0)
	{
		str__read_req_data.Format("RQ POS STN B R%c",CR);
		str__for_error.Format("RQ POS STN B R<CR>");

		str__log.Format("[%s] SEND => [%s]\n", var_name,str__for_error);
		Fnc__DRV_LOG(str__log);

		// ...
		CString msg;
		mX_Serial->CLEAR__BUFFER(&msg);
		
		// ...
		int nRet = mX_Serial->DATA__RECV(m_Term_Str,&str__read_req_data,m_Rcv_Time);

		str__log.Format("Ret:%d", nRet);
		Fnc__DRV_LOG(str__log);
		
		if(nRet >= 0)
		{
			str__log.Format("[%s] DIRECT_RECV <= [%s]\n", var_name,str__read_req_data);
			Fnc__DRV_LOG(str__log);

			m_nCommState = ONLINE;

			// ...
			str__read_req_data.TrimLeft();

			index = str__read_req_data.GetLength();
			if(index > 11)
			{
				check_char1 = str__read_req_data.GetAt(10);
				check_char2 = str__read_req_data.GetAt(11);

				str__digital_data.Format("%c%c", check_char1,check_char2);

				     if(str__digital_data == "EX")		read_data = "Extended";
				else if(str__digital_data == "RE")		read_data = "Retracted";
				else if(str__digital_data == "--")		read_data = "Unknown";
				else									read_data = "Unknown";

				// ...
				{
					str__log.Format("[%s] RECV <= [%s]\n", var_name,str__read_req_data);
					Fnc__DRV_LOG(str__log);
				}
			}
			else
			{
				read_data = "Unknown";
			}
		}
		else
		{
			m_nCommState = OFFLINE;

			// ...
			{
				str__log.Format("[%s] [Error] => No response Send:(%s) Receive:(%s)\n", var_name,str__for_error,str__read_req_data);

				Fnc__DRV_LOG(str__log);
			}

			read_data = "Unknown";
		}

		return 1;
	}

	if(diCH__ARM_A_MAT_READ->Check__VARIABLE_NAME(var_name) > 0)
	{
		str__read_req_data.Format("RQ LOAD A%c",CR);
		str__for_error.Format("RQ LOAD A<CR>");

		str__log.Format("[%s] SEND => [%s]\n", var_name,str__for_error);
		Fnc__DRV_LOG(str__log);

		// ...
		CString msg;
		mX_Serial->CLEAR__BUFFER(&msg);

		// ...
		int nRet = mX_Serial->DATA__RECV(m_Term_Str, &str__read_req_data, m_Rcv_Time);
		
		str__log.Format("Ret:%d", nRet);
		Fnc__DRV_LOG(str__log);
		
		if(nRet >= 0)
		{
			str__log.Format("[%s] DIRECT_RECV <= [%s]\n", var_name,str__read_req_data);
			Fnc__DRV_LOG(str__log);

			m_nCommState = ONLINE;

			// ...
			str__read_req_data.TrimLeft();

			index = str__read_req_data.GetLength();
			if(index > 8)
			{
				check_char1 = str__read_req_data.GetAt(7);
				check_char2 = str__read_req_data.GetAt(8);

				str__digital_data.Format("%c%c",check_char1,check_char2);

				     if(str__digital_data == "ON")		read_data = "Present";
				else if(str__digital_data == "OF")		read_data = "Absent";
				else if(str__digital_data == "?")		read_data = "Unknown";
				else									read_data = "Unknown";

				// ...
				{
					str__log.Format("[%s] RECV <= [%s]\n", var_name,str__read_req_data);
					Fnc__DRV_LOG(str__log);
				}
			}
			else
			{
				read_data = "Unknown";
			}
		}
		else
		{
			m_nCommState = OFFLINE;

			// ...
			{
				str__log.Format("[%s] [Error] => No response Send:(%s) Receive:(%s)\n",
								var_name,
								str__for_error,
								str__read_req_data);

				Fnc__DRV_LOG(str__log);
			}

			read_data = "Unknown";
		}

		return 1;
	}

	if(diCH__ARM_B_MAT_READ->Check__VARIABLE_NAME(var_name) > 0)
	{
		str__read_req_data.Format("RQ LOAD B%c",CR);
		str__for_error.Format("RQ LOAD B<CR>");

		str__log.Format("[%s] SEND => [%s]\n", var_name,str__for_error);
		Fnc__DRV_LOG(str__log);

		// ...
		CString msg;
		mX_Serial->CLEAR__BUFFER(&msg);

		// ...
		int nRet = mX_Serial->DATA__RECV(m_Term_Str,&str__read_req_data,m_Rcv_Time);
		
		str__log.Format("Ret:%d", nRet);
		Fnc__DRV_LOG(str__log);

		if(nRet >= 0)
		{
			str__log.Format("[%s] DIRECT_RECV <= [%s]\n", var_name,str__read_req_data);
			Fnc__DRV_LOG(str__log);

			m_nCommState = ONLINE;

			// ...
			str__read_req_data.TrimLeft();

			index = str__read_req_data.GetLength();
			if(index > 8)
			{
				check_char1 = str__read_req_data.GetAt(7);
				check_char2 = str__read_req_data.GetAt(8);

				str__digital_data.Format("%c%c",check_char1,check_char2);

				     if(str__digital_data == "ON")		read_data = "Present";
				else if(str__digital_data == "OF")		read_data = "Absent";
				else if(str__digital_data == "?")		read_data = "Unknown";
				else									read_data = "Unknown";

				// ...
				{
					str__log.Format("[%s] RECV <= [%s]\n", var_name,str__read_req_data);
					Fnc__DRV_LOG(str__log);
				}
			}
			else
			{
				read_data = "Unknown";
			}
		}
		else
		{
			m_nCommState = OFFLINE;

			// ...
			{
				str__log.Format("[%s] [Error] => No response Send:(%s) Receive:(%s)\n",
								var_name,
								str__for_error,
								str__read_req_data);

				Fnc__DRV_LOG(str__log);
			}

			read_data = "Unknown";
		}

		return 1;
	}

	if(diCH__COMM_STS->Check__VARIABLE_NAME(var_name) > 0)
	{
		if(m_nCommState == OFFLINE)		read_data = STR__OFFLINE;
		else							read_data = STR__ONLINE;

		return 1;
	}

	return -1;
}

int CObj__VAC_MAGATRAN
::__Read__STRING(const CString& var_name, const CDS_IO__CHANNEL_INFO& io_info, CString& read_data)
{
	CString str__read_req_data;
	CString str__for_error;
	CString str__log;
	int index = 0;

	if(siCH__VERSION->Check__VARIABLE_NAME(var_name) > 0)
	{
		str__read_req_data.Format("RQ VERSION%c",CR);
		str__for_error.Format("RQ VERSION<CR>");

		str__log.Format("[%s] SEND => [%s]\n", var_name,str__for_error);
		Fnc__DRV_LOG(str__log);

		// ...
		CString msg;
		mX_Serial->CLEAR__BUFFER(&msg);

		// ...
		int nRet = mX_Serial->DATA__RECV(m_Term_Str, &str__read_req_data, m_Rcv_Time);

		str__log.Format("Ret:%d", nRet);
		Fnc__DRV_LOG(str__log);
		
		if(nRet >= 0)
		{
			str__log.Format("[%s] DIRECT_RECV <= [%s]\n", var_name,str__read_req_data);
			Fnc__DRV_LOG(str__log);

			m_nCommState = ONLINE;

			// ...
			str__read_req_data.TrimLeft();

			index = str__read_req_data.GetLength();
			if(index > 5)
			{
				str__read_req_data.SetAt(5, 0);
				
				read_data.Format("%s", str__read_req_data);

				// ...
				{
					str__log.Format("[%s] RECV <= [%s]\n", var_name,str__read_req_data);
					Fnc__DRV_LOG(str__log);
				}
			}
			else
			{
				read_data = "Unknown";
			}
		}
		else
		{
			m_nCommState = OFFLINE;

			// ...
			{
				str__log.Format("[%s] [Error] => No response Send:(%s) Receive:(%s)\n", 
								var_name,
								str__for_error,
								str__read_req_data);

				Fnc__DRV_LOG(str__log);
			}

			read_data = "Unknown";
		}

		return 1;
	}

	return -1;
}


//------------------------------------------------------------------------------------
int CObj__VAC_MAGATRAN
::__Write__ANALOG(const CString& var_name, const CDS_IO__CHANNEL_INFO& io_info, const double set_data)
{
	if(aoCH_RO_OFFSET->Check__VARIABLE_NAME(var_name) > 0)
	{
		m_sRO_OFFSET.Format("%.f", set_data);
		return 1;
	}

	if(aoCH_TO_OFFSET->Check__VARIABLE_NAME(var_name) > 0)
	{
		m_sTO_OFFSET.Format("%.f", set_data);
		return 1;
	}

	if(aoCH_STN_R_VAL->Check__VARIABLE_NAME(var_name) > 0)
	{
		m_sSTN_R_SET_VAL.Format("%.f", set_data);
		return 1;
	}

	if(aoCH_STN_T_VAL->Check__VARIABLE_NAME(var_name) > 0)
	{
		m_sSTN_T_SET_VAL.Format("%.f", set_data);
		return -1;
	}

	return -1;
}

int CObj__VAC_MAGATRAN
::__Write__DIGITAL(const CString& var_name,const CDS_IO__CHANNEL_INFO& io_info, const CString& set_data,const int item_index)
{
	CString str__send_data;
	CString str__log;
	CString str__for_error;
	int     index = 0;
	int     nEach_action_timeout = 10000;	// 10 sec
	CString str_cfg_data;
	CString str_action;
	CString para__stn_name;

	char check_char[10] = {0,};


	if(doCH__COMMAND->Check__VARIABLE_NAME(var_name) > 0)
	{
		// AGoto BGoto APick BPick APlace BPlace ASwap BSwap ComInit HomeAll HomeR Reset LoadA LoadB UnloadA UnloadB ChkLoad ChkLoadA ChkLoadB APickAL APlaceAL BPickAL BPlaceAL WfrSnsIntk

		// MX , GX Robot Commands -----
		sCH__ACTION_STS->Get__DATA(str_action);
		dCH__OTR_IN_PARA__STN_NAME->Get__DATA(para__stn_name);

		// ----- Get Action Timeout From config page...
		if(str_action.CompareNoCase(sMODE__ROTATE) == 0)
		{
			if(para__stn_name.CompareNoCase("PM1") == 0)
			{
				aCH__CFG_PM1_ROTATE_TIME->Get__DATA(str_cfg_data);
				nEach_action_timeout = atoi(str_cfg_data)*1000;
			}
			else if(para__stn_name.CompareNoCase("PM2") == 0)
			{
				aCH__CFG_PM2_ROTATE_TIME->Get__DATA(str_cfg_data);
				nEach_action_timeout = atoi(str_cfg_data)*1000;
			}
			else if(para__stn_name.CompareNoCase("PM3") == 0)
			{
				aCH__CFG_PM3_ROTATE_TIME->Get__DATA(str_cfg_data);
				nEach_action_timeout = atoi(str_cfg_data)*1000;
			}
			else if(para__stn_name.CompareNoCase("PM4") == 0)
			{
				aCH__CFG_PM4_ROTATE_TIME->Get__DATA(str_cfg_data);
				nEach_action_timeout = atoi(str_cfg_data)*1000;
			}
			else
			{
				aCH__CFG_ACTION_TIMEOUT_ROTATE_TIME->Get__DATA(str_cfg_data);
				nEach_action_timeout = atoi(str_cfg_data)*1000;
			}
		}
		else if(str_action.CompareNoCase(sMODE__GODOWN) == 0)
		{
			aCH__CFG_ACTION_TIMEOUT_DOWN->Get__DATA(str_cfg_data);
			nEach_action_timeout = atoi(str_cfg_data)*1000;
		}
		else if(str_action.CompareNoCase(sMODE__GOUP) == 0)
		{
			aCH__CFG_ACTION_TIMEOUT_UP->Get__DATA(str_cfg_data);
			nEach_action_timeout = atoi(str_cfg_data)*1000;
		}
		else if(str_action.CompareNoCase(sMODE__RETRACT) == 0)
		{
			aCH__CFG_ACTION_TIMEOUT_RETRACT_TIME->Get__DATA(str_cfg_data);
			nEach_action_timeout = atoi(str_cfg_data)*1000;
		}
		else if( (str_action.CompareNoCase("EXTEND_DYNAMIC_ALIGN") == 0) ||
			(str_action.CompareNoCase("RETRACT_DYNAMIC_ALIGN") == 0) )
		{
			aCH__CFG_ACTION_TIMEOUT_GOTO_DA_TIME->Get__DATA(str_cfg_data);
			nEach_action_timeout = atoi(str_cfg_data)*1000;
		}
		else if(str_action.CompareNoCase(sMODE__PICK) == 0)
		{
			aCH__CFG_ACTION_TIMEOUT_PICK_TIME->Get__DATA(str_cfg_data);
			nEach_action_timeout = atoi(str_cfg_data)*1000;
		}
		else if(str_action.CompareNoCase(sMODE__PLACE) == 0)
		{
			aCH__CFG_ACTION_TIMEOUT_PLACE_TIME->Get__DATA(str_cfg_data);
			nEach_action_timeout = atoi(str_cfg_data)*1000;
		}

		// --------- Command Format...
		if(set_data.CompareNoCase("AGoto") == 0)
		{
			// AGoto : GOTO Station RPosition ZPostion Slot Arm
			str__send_data.Format("GOTO N %s R %s Z %s SLOT %s ARM A%c",   m_sStationA, m_sRPos, m_sZPos, m_sSlot, CR);
			str__for_error.Format("GOTO N %s R %s Z %s SLOT %s ARM A<CR>", m_sStationA, m_sRPos, m_sZPos, m_sSlot);
		}
		else if(set_data.CompareNoCase("AGoto_Offset") == 0)
		{
			// AGoto_Offset : GOTO Station RPosition ZPostion Slot Arm
			str__send_data.Format("GOTO N %s R %s RO %s TO %s Z %s SLOT %s ARM A%c",   m_sStationA, m_sRPos, m_sRO_OFFSET, m_sTO_OFFSET, m_sZPos, m_sSlot, CR);
			str__for_error.Format("GOTO N %s R %s RO %s TO %s Z %s SLOT %s ARM A<CR>", m_sStationA, m_sRPos, m_sRO_OFFSET, m_sTO_OFFSET, m_sZPos, m_sSlot);
		}
		else if(set_data.CompareNoCase("BGoto") == 0)
		{
			str__send_data.Format("GOTO N %s R %s Z %s SLOT %s ARM B%c",   m_sStationB, m_sRPos, m_sZPos, m_sSlot, CR);
			str__for_error.Format("GOTO N %s R %s Z %s SLOT %s ARM B<CR>", m_sStationB, m_sRPos, m_sZPos, m_sSlot);
		}
		else if(set_data.CompareNoCase("BGoto_Offset") == 0)
		{
			// BGoto_Offset : GOTO Station RPosition ZPostion Slot Arm
			str__send_data.Format("GOTO N %s R %s RO %s TO %s Z %s SLOT %s ARM B%c",   m_sStationB, m_sRPos, m_sRO_OFFSET, m_sTO_OFFSET, m_sZPos, m_sSlot, CR);
			str__for_error.Format("GOTO N %s R %s RO %s TO %s Z %s SLOT %s ARM B<CR>", m_sStationB, m_sRPos, m_sRO_OFFSET, m_sTO_OFFSET, m_sZPos, m_sSlot);
		}
		else if(set_data.CompareNoCase("APick") == 0)
		{
			// APick : PICK Station Slot Arm
			str__send_data.Format("PICK %s SLOT %s ARM A%c",   m_sStationA, m_sSlot, CR);
			str__for_error.Format("PICK %s SLOT %s ARM A<CR>", m_sStationA, m_sSlot);
		}
		else if(set_data.CompareNoCase("BPick") == 0)
		{
			// BPick : PICK Station Slot Arm
			str__send_data.Format("PICK %s SLOT %s ARM B%c",   m_sStationB, m_sSlot, CR);
			str__for_error.Format("PICK %s SLOT %s ARM B<CR>", m_sStationB, m_sSlot);
		}
		else if(set_data.CompareNoCase("APlace") == 0)
		{
			// APlace : PLACE Station Slot Arm
			str__send_data.Format("PLACE %s SLOT %s ARM A%c",   m_sStationA, m_sSlot, CR);
			str__for_error.Format("PLACE %s SLOT %s ARM A<CR>", m_sStationA, m_sSlot);
		}
		else if(set_data.CompareNoCase("BPlace") == 0)
		{
			// BPlace : PLACE Station Slot Arm
			str__send_data.Format("PLACE %s SLOT %s ARM B%c",   m_sStationB, m_sSlot, CR);
			str__for_error.Format("PLACE %s SLOT %s ARM B<CR>", m_sStationB, m_sSlot);
		}
		else if(set_data.CompareNoCase("APlace_Offset") == 0)
		{
			// APlace_Offset : PLACE Station with an offset
			str__send_data.Format("PLACE %s SLOT %s ARM A RO %s TO %s%c",   m_sStationA, m_sSlot, m_sRO_OFFSET, m_sTO_OFFSET, CR);
			str__for_error.Format("PLACE %s SLOT %s ARM A RO %s TO %s<CR>", m_sStationA, m_sSlot, m_sRO_OFFSET, m_sTO_OFFSET);
		}
		else if(set_data.CompareNoCase("BPlace_Offset") == 0)
		{
			// BPlace_Offset : PLACE Station with an offset
			str__send_data.Format("PLACE %s SLOT %s ARM B RO %s TO %s%c",   m_sStationB, m_sSlot, m_sRO_OFFSET, m_sTO_OFFSET, CR);
			str__for_error.Format("PLACE %s SLOT %s ARM B RO %s TO %s<CR>", m_sStationB, m_sSlot, m_sRO_OFFSET, m_sTO_OFFSET);
		}

		// Common Robot Commands -----
		else if(set_data.CompareNoCase("ComInit") == 0)
		{
			if(Drv__CMD_COM_INIT(var_name) < 0)		return -1;

			return  1;
		}
		else if(set_data.CompareNoCase("HomeAll") == 0)
		{
			//..
			str__send_data.Format("HOME ALL%c", CR);
			str__for_error.Format("HOME ALL<CR>");
		}
		else if(set_data.CompareNoCase("HomeR") == 0)
		{
			//..
			str__send_data.Format("HOME R%c",CR);
			str__for_error.Format("HOME R<CR>");
		}
		else if(set_data.CompareNoCase("Reset") == 0)
		{
			//..
			str__send_data.Format("RESET%c", CR);
			str__for_error.Format("RESET<CR>");
		}
		else if(set_data.CompareNoCase("LoadA") == 0)
		{
			//..
			str__send_data.Format("SET LOAD ARM A ON%c",CR);
			str__for_error.Format("SET LOAD ARM A ON<CR>");
		}
		else if(set_data.CompareNoCase("LoadB") == 0)
		{
			//.. 
			str__send_data.Format("SET LOAD ARM B ON%c",CR);
			str__for_error.Format("SET LOAD ARM B ON<CR>");
		}
		else if(set_data.CompareNoCase("UnloadA") == 0)
		{
			//.. 
			str__send_data.Format("SET LOAD ARM A OFF%c",CR);
			str__for_error.Format("SET LOAD ARM A OFF<CR>");
		}
		else if(set_data.CompareNoCase("UnloadB") == 0)
		{
			//.. 
			str__send_data.Format("SET LOAD ARM B OFF%c",CR);
			str__for_error.Format("SET LOAD ARM B OFF<CR>");
		}
		else if(set_data.CompareNoCase("ChkLoad") == 0)
		{
			//.. 
			str__send_data.Format("CHECK LOAD INTLCK ALL DIS%c",CR);
			str__for_error.Format("CHECK LOAD INTLCK ALL DIS<CR>");
		}
		else if(set_data.CompareNoCase("ChkLoadA") == 0)
		{
			//.. 
			str__send_data.Format("CHECK LOAD A INTLCK ALL DIS%c",CR);
			str__for_error.Format("CHECK LOAD A INTLCK ALL DIS<CR>");
		}
		else if(set_data.CompareNoCase("ChkLoadB") == 0)
		{
			//.. 
			str__send_data.Format("CHECK LOAD B INTLCK ALL DIS%c",CR);
			str__for_error.Format("CHECK LOAD B INTLCK ALL DIS<CR>");
		}

		// Pick & Place with Alginer -----
		else if(set_data.CompareNoCase("APickAL") == 0)
		{
			//.. 
			str__send_data.Format("PICK %s ARM A STRT NR%c",   m_sStationA,CR);
			str__for_error.Format("PICK %s ARM A STRT NR<CR>", m_sStationA);
		}
		else if(set_data.CompareNoCase("APlaceAL") == 0)
		{
			//.. 
			str__send_data.Format("PLACE %s ARM A STRT NR%c",   m_sStationA,CR);
			str__for_error.Format("PLACE %s ARM A STRT NR<CR>", m_sStationA);
		}
		else if(set_data.CompareNoCase("BPickAL") == 0)
		{
			//.. 
			str__send_data.Format("PICK %s ARM B STRT NR%c",   m_sStationB,CR);
			str__for_error.Format("PICK %s ARM B STRT NR<CR>", m_sStationB);
		}
		else if(set_data.CompareNoCase("BPlaceAL") == 0)
		{
			//.. 
			str__send_data.Format("PLACE %s ARM B STRT NR%c",   m_sStationB,CR);
			str__for_error.Format("PLACE %s ARM B STRT NR<CR>", m_sStationB);
		}
		else if(set_data.CompareNoCase("WfrSnsIntk") == 0)
		{
			//.. 
			str__send_data.Format("SET INTLCK WAF_SEN %s%c",    m_sWfrSnsIntlk,CR);
			str__for_error.Format("SET INTLCK WAF_SEN %s<CR>",  m_sWfrSnsIntlk);
		}
		else if(set_data.CompareNoCase("ASet_Cptr_On") == 0)
		{
			// SET_CPTR_ON : 각 Station앞에 있는 센서를 ON 시킨다.
			str__send_data.Format("SET CPTR %s ON%c",   m_sCptr_StationA, CR);
			str__for_error.Format("SET CPTR %s ON<CR>, timeout : %d", m_sCptr_StationA, m_Out_Time);
		}
		else if(set_data.CompareNoCase("ASet_Cptr_Off") == 0)
		{
			// SET_CPTR_ON : 각 Station앞에 있는 센서를 OFF 시킨다.
			str__send_data.Format("SET CPTR %s OFF%c",   m_sCptr_StationA, CR);
			str__for_error.Format("SET CPTR %s OFF<CR>, timeout : %d", m_sCptr_StationA, m_Out_Time);
		}
		else if(set_data.CompareNoCase("BSet_Cptr_On") == 0)
		{
			// SET_CPTR_ON : 각 Station앞에 있는 센서를 ON 시킨다.
			str__send_data.Format("SET CPTR %s ON%c",   m_sCptr_StationB, CR);
			str__for_error.Format("SET CPTR %s ON<CR>, timeout : %d", m_sCptr_StationB, m_Out_Time);
		}
		else if(set_data.CompareNoCase("BSet_Cptr_Off") == 0)
		{
			// SET_CPTR_ON : 각 Station앞에 있는 센서를 OFF 시킨다.
			str__send_data.Format("SET CPTR %s OFF%c",   m_sCptr_StationB, CR);
			str__for_error.Format("SET CPTR %s OFF<CR>, timeout : %d", m_sCptr_StationB, m_Out_Time);
		}
		else if(set_data.CompareNoCase("SET_CPTR_21_ON") == 0)
		{
			str__send_data.Format("SET CPTR 21 ON%c", CR);
			str__for_error.Format("SET CPTR 21 ON<CR>, timeout : %d", m_Out_Time);
		}
		else if(set_data.CompareNoCase("SET_CPTR_21_OFF") == 0)
		{
			str__send_data.Format("SET CPTR 21 OFF%c", CR);
			str__for_error.Format("SET CPTR 21 OFF<CR>, timeout : %d", m_Out_Time);
		}
		else if(set_data.CompareNoCase("AGet_Cptr") == 0)
		{
			// RQ CPTR station : 각 Station앞에 있는 센서값을 가져온다.
			str__send_data.Format("RQ CPTR %s%c",   m_sCptr_StationA, CR);
			str__for_error.Format("RQ CPTR %s<CR>, timeout : %d", m_sCptr_StationA, m_Out_Time);
		}
		else if(set_data.CompareNoCase("BGet_Cptr") == 0)
		{
			// RQ CPTR station : 각 Station앞에 있는 센서값을 가져온다.
			str__send_data.Format("RQ CPTR %s%c",   m_sCptr_StationB, CR);
			str__for_error.Format("RQ CPTR %s<CR>, timeout : %d", m_sCptr_StationB, m_Out_Time);
		}
		else if(set_data.CompareNoCase("GET_CPTR_21") == 0)
		{
			str__send_data.Format("RQ CPTR 21%c", CR);
			str__for_error.Format("RQ CPTR 21<CR>, timeout : %d", m_Out_Time);
		}
		else if(set_data.CompareNoCase("AGet_Current_Stn") == 0)
		{
			str__send_data.Format("RQ STN %s A ALL%c",   m_sStationA, CR);
			str__for_error.Format("RQ STN %s A ALL<CR>", m_sStationA);
		}	
		else if(set_data.CompareNoCase("BGet_Current_Stn") == 0)
		{
			str__send_data.Format("RQ STN %s B ALL%c",   m_sStationB, CR);
			str__for_error.Format("RQ STN %s B ALL<CR>", m_sStationB);
		}
		else if(set_data.CompareNoCase("ASet_Stn") == 0)
		{
			str__send_data.Format("SET STN %s ARM A R %s T %s%c",   m_sStationA, m_sSTN_R_SET_VAL, m_sSTN_T_SET_VAL, CR);
			str__for_error.Format("SET STN %s ARM A R %s T %s<CR>", m_sStationA, m_sSTN_R_SET_VAL, m_sSTN_T_SET_VAL);
		}
		else if(set_data.CompareNoCase("BSet_Stn") == 0)
		{
			str__send_data.Format("SET STN %s ARM B R %s T %s%c",   m_sStationB, m_sSTN_R_SET_VAL, m_sSTN_T_SET_VAL, CR);
			str__for_error.Format("SET STN %s ARM B R %s T %s<CR>", m_sStationB, m_sSTN_R_SET_VAL, m_sSTN_T_SET_VAL);
		}
		else
		{
			str__log.Format("[%s] Unknown DATA Error\n", var_name);
			Fnc__DRV_LOG(str__log);
			return -1;
		}

		m_Out_Time = nEach_action_timeout;

		str__log.Format("[%s] SEND => [%s], EndChar:%s, timeout:%d\n", 
						var_name,
						str__send_data, 
						m_Term_Str,
						m_Out_Time);
		Fnc__DRV_LOG(str__log);

		// ...
		CString msg;
		mX_Serial->CLEAR__BUFFER(&msg);

		// ...
		int nRet = mX_Serial->DATA__RECV(m_Term_Str, &str__send_data, m_Out_Time);
		
		str__log.Format("Ret:%d", nRet);
		Fnc__DRV_LOG(str__log);
		
		if(nRet >= 0)
		{
			m_nCommState = ONLINE;

			// ...
			str__send_data.TrimLeft();

			index = str__send_data.GetLength();
			if(index >= 2)
			{
				if(str__send_data.GetAt(1) == 'E')
				{
					for(int i=5; i<index; i++)
					{
						check_char[i-5] = str__send_data.GetAt(i);
					}
					m_sErrorID.Format("%s",&check_char);
					sCH__ERRID->Set__DATA(m_sErrorID);

					// ...
					{
						str__log.Format("[%s][%s] [ERROR] [ID:%s] => Problem of Command[%s]\n",
										var_name,
										str__for_error,
										m_sErrorID,
										str__send_data);

						Fnc__DRV_LOG(str__log);
					}
				}
				else
				{
					// Error 없슴..
					m_sErrorID.Format("0");
					sCH__ERRID->Set__DATA(m_sErrorID);

					// ...
					{
						str__log.Format("[%s] RECV <= [%s]\n", var_name,str__send_data);

						Fnc__DRV_LOG(str__log);
					}

					if((set_data.CompareNoCase("AGet_Cptr")   == 0) 
					|| (set_data.CompareNoCase("BGet_Cptr")   == 0) 
					|| (set_data.CompareNoCase("GET_CPTR_21") == 0))
					{
						// CPTR NOT TRIGGERED 가 올수도 있다.
						sCH__CPTR_SNS_FULL_VAL->Set__DATA(str__send_data);
					}

					if((set_data.CompareNoCase("AGet_Current_Stn") == 0) 
					|| (set_data.CompareNoCase("BGet_Current_Stn") == 0))
					{
						sCH__RQ_STN_FULL_VAL->Set__DATA(str__send_data);
					}
				}
			}
			return 1;
		}

		m_nCommState = OFFLINE;

		// ...
		{
			str__log.Format("[%s] [Error] => No response Send:(%s) Receive:(%s)\n",
							var_name, 
							str__for_error,
							str__send_data);

			Fnc__DRV_LOG(str__log);
		}

		return 1;
	}

	// edo.RB1.StnA 
	if(doCH__STN_A->Check__VARIABLE_NAME(var_name) > 0)
	{
		m_sStationA = set_data;

		// ...
		{
			str__log.Format("[%s] RECV <= [%s]\n", var_name,set_data);
			Fnc__DRV_LOG(str__log);
		}
		return 1;
	}

	// edo.RB1.StnB 
	if(doCH__STN_B->Check__VARIABLE_NAME(var_name) > 0)
	{
		m_sStationB = set_data;

		// ...
		{
			str__log.Format("[%s] RECV <= [%s]\n", var_name,set_data);
			Fnc__DRV_LOG(str__log);
		}
		return 1;
	}

	// edo.RB1.CPTR.StnA
	if(doCH__CPTR_STN_A->Check__VARIABLE_NAME(var_name) > 0)
	{
		m_sCptr_StationA = set_data;

		// ...
		{
			str__log.Format("[%s] RECV <= [%s]\n", var_name,set_data);
			Fnc__DRV_LOG(str__log);
		}
		return 1;
	}

	// edo.RB1.CPTR.StnB
	if(doCH__CPTR_STN_B->Check__VARIABLE_NAME(var_name) > 0)
	{
		m_sCptr_StationB = set_data;

		// ...
		{
			str__log.Format("[%s] RECV <= [%s]\n", var_name,set_data);
			Fnc__DRV_LOG(str__log);
		}
		return 1;
	}

	// edo.RB1.RPos
	if(doCH__RPOS->Check__VARIABLE_NAME(var_name) > 0)
	{
		if(set_data.CompareNoCase("Retract") == 0)		m_sRPos = "RE";
		else											m_sRPos = "EX";

		// ...
		{
			str__log.Format("[%s] RECV <= [%s]\n", var_name,set_data);
			Fnc__DRV_LOG(str__log);
		}
		return 1;
	}

	// edo.RB1.ArmAMat
	// Unknown Absent Present;
	if(doCH__ARM_A_MAT->Check__VARIABLE_NAME(var_name) > 0)
	{
		if(set_data.CompareNoCase("Absent") == 0)
		{
			str__send_data.Format("SET LOAD A OFF%c", CR);
			str__for_error.Format("SET LOAD A OFF<CR>");
		}
		else if(set_data.CompareNoCase("Present") == 0)
		{
			str__send_data.Format("SET LOAD A ON%c", CR);
			str__for_error.Format("SET LOAD A ON<CR>");
		}
		else
		{
			str__send_data.Format("SET LOAD A ?%c", CR);
			str__for_error.Format("SET LOAD A ?<CR>");
		}

		str__log.Format("[%s] SEND => [%s]\n", var_name,str__for_error);
		Fnc__DRV_LOG(str__log);

		// ...
		CString msg;
		mX_Serial->CLEAR__BUFFER(&msg);

		// ...
		int nRet = mX_Serial->DATA__RECV(m_Term_Str,&str__send_data,m_Out_Time);
		
		str__log.Format("Ret:%d", nRet);
		Fnc__DRV_LOG(str__log);

		if(nRet >= 0)
		{
			m_nCommState = ONLINE;

			// ...
			str__send_data.TrimLeft();

			index = str__send_data.GetLength();
			if(index >= 2)
			{
				if(str__send_data.GetAt(1) == 'E')
				{
					for(int i=5; i<index; i++)
					{
						check_char[i-5] = str__send_data.GetAt(i);
					}

					m_sErrorID.Format("%s",&check_char);
					sCH__ERRID->Set__DATA(m_sErrorID);

					// ...
					{
						str__log.Format("[%s][%s] [ERROR] [ID:%s] => Problem of Command[%s]\n", 
										var_name,
										str__for_error,
										m_sErrorID,
										str__send_data);

						Fnc__DRV_LOG(str__log);
					}

					return 1;
				}
				else
				{
					m_sErrorID.Format("0");
					sCH__ERRID->Set__DATA(m_sErrorID);

					// ...
					{
						str__log.Format("[%s] RECV <= [%s]\n", var_name,str__send_data);

						Fnc__DRV_LOG(str__log);
					}					
				}
			}

			return 1;
		}

		m_nCommState = OFFLINE;

		// ...
		{
			str__log.Format("[%s] [Error] => No response Send:(%s) Receive:(%s)\n",
							var_name, 
							str__for_error,
							str__send_data);

			Fnc__DRV_LOG(str__log);
		}

		return 1;
	}

	// edo.RB1.ArmBMat
	// Unknown Absent Present;
	if(doCH__ARM_B_MAT->Check__VARIABLE_NAME(var_name) > 0)
	{
		if(set_data.CompareNoCase("Absent") == 0)
		{
			str__send_data.Format("SET LOAD B OFF%c", CR);
			str__for_error.Format("SET LOAD B OFF<CR>");
		}
		else if(set_data.CompareNoCase("Present") == 0)
		{
			str__send_data.Format("SET LOAD B ON%c", CR);
			str__for_error.Format("SET LOAD B ON<CR>");
		}
		else
		{
			str__send_data.Format("SET LOAD B ?%c", CR);
			str__for_error.Format("SET LOAD B ?<CR>");
		}

		str__log.Format("[%s] SEND => [%s]\n", var_name,str__for_error);
		Fnc__DRV_LOG(str__log);

		// ...
		CString msg;
		mX_Serial->CLEAR__BUFFER(&msg);

		// ...
		int nRet = mX_Serial->DATA__RECV(m_Term_Str,&str__send_data,m_Out_Time);

		str__log.Format("Ret:%d", nRet);
		Fnc__DRV_LOG(str__log);

		if(nRet >= 0)
		{
			m_nCommState = ONLINE;

			// ...
			str__send_data.TrimLeft();

			index = str__send_data.GetLength();
			if(index >= 2)
			{
				if(str__send_data.GetAt(1) == 'E')
				{
					for(int i=5; i<index; i++)
					{
						check_char[i-5] = str__send_data.GetAt(i);
					}
					m_sErrorID.Format("%s",&check_char);
					sCH__ERRID->Set__DATA(m_sErrorID);

					// ...
					{
						str__log.Format("[%s][%s] [ERROR] [ID:%s] => Problem of Command[%s]\n",
										var_name,
										str__for_error,
										m_sErrorID,
										str__send_data);

						Fnc__DRV_LOG(str__log);
					}

					return 1;
				}
				else
				{
					m_sErrorID.Format("0");
					sCH__ERRID->Set__DATA(m_sErrorID);

					// ...
					{
						str__log.Format("[%s] RECV <= [%s]\n", var_name,str__send_data);

						Fnc__DRV_LOG(str__log);
					}					
				}
			}

			return 1;
		}

		m_nCommState = OFFLINE;

		// ...
		{
			str__log.Format("[%s] [Error] => No response Send:(%s) Receive:(%s)\n",
							var_name, 
							str__for_error,
							str__send_data);

			Fnc__DRV_LOG(str__log);
		}

		return 1;
	}

	// edo.RB1.ZPos
	if(doCH__ZPOS->Check__VARIABLE_NAME(var_name) > 0)
	{
		if(set_data.CompareNoCase("Down") == 0)		m_sZPos = "DN";
		else										m_sZPos = "UP";

		// ...
		{
			str__log.Format("[%s] RECV <= [%s]\n", var_name,set_data);

			Fnc__DRV_LOG(str__log);
		}

		return 1;
	}

	// edo.RB1.Slot
	if(doCH__SLOT->Check__VARIABLE_NAME(var_name) > 0)
	{
		m_sSlot = set_data;

		// ...
		{
			str__log.Format("[%s] RECV <= [%s]\n", var_name,set_data);

			Fnc__DRV_LOG(str__log);
		}

		return 1;
	}

	// edo.RB1.WfrSns
	if(doCH__ARM_WFR_SNS->Check__VARIABLE_NAME(var_name) > 0)
	{
		if(set_data.CompareNoCase("Yes") == 0)	m_sWfrSnsIntlk = "Y";
		else									m_sWfrSnsIntlk = "N";

		// ...
		{
			str__log.Format("[%s] RECV <= [%s]\n", var_name,set_data);

			Fnc__DRV_LOG(str__log);
		}

		return 1;
	}

	return 1;
}

int CObj__VAC_MAGATRAN
::__Write__STRING(const CString& var_name, const CDS_IO__CHANNEL_INFO& io_info, const CString& set_data)
{
	SCX__SEQ_INFO x_seq_info;
	CString str_time;

	CString str__send_data;
	CString str__for_error;
	CString str__log;
	CString str__timeout;
	CString str__temp;
	int cmd_timeout = 0;

	// ...
	aCH__HYPER_TERMINAL_CMD_TIMEOUT->Get__DATA(str__timeout);

	if(soCH__HYPER_TERMINAL_CMD->Check__VARIABLE_NAME(var_name) > 0)
	{
		str__send_data.Format("%s%c", set_data, CR);
		str__for_error.Format("%s",   set_data);

		cmd_timeout = atoi(str__timeout)*1000;	//
		
		str__log.Format("Hyper Terminal [%s] SEND => [%s], timeout:%d msec", var_name,str__for_error,cmd_timeout);
		Fnc__DRV_LOG(str__log);

		// ...
		CString msg;
		mX_Serial->CLEAR__BUFFER(&msg);

		// ...
		int nRet = mX_Serial->DATA__RECV(m_Term_Str, &str__send_data, cmd_timeout);

		str__log.Format("After Hyper Terminal, Revc.. Ret:%d, data[%s]", nRet, str__send_data);
		Fnc__DRV_LOG(str__log);

		sCH__HYPER_TERMINAL_RECV_MSG->Set__DATA("");

		str__send_data.TrimLeft();
		
		if(nRet < 0)
		{
			str__log.Format("[%s] Timeout Error. \n\n", str__for_error);
			sCH__HYPER_TERMINAL_RECV_MSG->Set__DATA(str__log);
		}
		else if(nRet == 0)	// waiting 문자 들어옴..
		{
			str__log = m_Term_Str;
			str__log += "\n";
			sCH__HYPER_TERMINAL_RECV_MSG->Set__DATA(str__log);
		}
		else
		{
			str__log = str__send_data;
			str__log += "\n";
			sCH__HYPER_TERMINAL_RECV_MSG->Set__DATA(str__log);
		}

		// ...
		{
			xCH__UPDATE_FLAG->Get__DATA(str__log);
			str__temp.Format("%1d", atoi(str__log)+1);
			xCH__UPDATE_FLAG->Set__DATA(str__temp);
			_sleep(500);

			str_time = x_seq_info->Get__DATE_TIME_STRING();
			str__log.Format("[%s] : <----- RECV -----> \n", str_time);
			sCH__HYPER_TERMINAL_RECV_MSG->Set__DATA("");
			sCH__HYPER_TERMINAL_RECV_MSG->Set__DATA(str__log);

			xCH__UPDATE_FLAG->Get__DATA(str__log);
			str__temp.Format("%1d", atoi(str__log)+1);
			xCH__UPDATE_FLAG->Set__DATA(str__temp);
			_sleep(500);
		}

		// ...
		{
			str__log.Format("[%s] RECV <= [%s]\n", var_name,str__send_data);

			Fnc__DRV_LOG(str__log);
		}					

		return 1;
	}

	return 1;
}
