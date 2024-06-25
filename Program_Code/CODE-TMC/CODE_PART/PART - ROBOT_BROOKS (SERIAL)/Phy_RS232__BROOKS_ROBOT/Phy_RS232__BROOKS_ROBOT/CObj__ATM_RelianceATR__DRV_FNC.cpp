#include "StdAfx.h"
#include "CObj__ATM_RelianceATR.h"
#include "CObj__ATM_RelianceATR__DEF.h"

#include "CCommon_DEF.h"


//------------------------------------------------------------------------------------
int CObj__ATM_RelianceATR
::__Read__ANALOG(const CString& var_name, const CDS_IO__CHANNEL_INFO& io_info, double& read_data)
{
	if(aiCH__ANG_POS->Check__VARIABLE_NAME(var_name) > 0)
	{
		read_data = dDRV__RB_ANGLE_POS;
		return 1;
	}
	if(aiCH__ANG_ECC->Check__VARIABLE_NAME(var_name) > 0)
	{
		read_data = dDRV__RB_ANGLE_ECC;
		return 1;
	}
	if(aiCH__ECC_MAG->Check__VARIABLE_NAME(var_name) > 0)
	{
		read_data = dDRV__RB_ECC_MAG;
		return 1;
	}
	if(aiCH__AVRG_RD->Check__VARIABLE_NAME(var_name) > 0)
	{
		read_data = dDRV__RB_AVG_RD;
		return 1;
	}
	if(aiCH__ERROR_ID->Check__VARIABLE_NAME(var_name) > 0)
	{
		read_data = (double) iDRV__ERROR_ID;			//
		return 1;
	}

	return -1;
}

int CObj__ATM_RelianceATR
::__Read__DIGITAL(const CString& var_name,const CDS_IO__CHANNEL_INFO& io_info, CString& read_data,int& item_index)
{
	CString str__recv_data;
	CString str__cmmd;
	int flag;

	if(diCH__COMM_STS->Check__VARIABLE_NAME(var_name) > 0)
	{
		if(iActive_SIM > 0)
		{
			read_data = STR__ONLINE;
		}
		else
		{
			if(m_nCommState == OFFLINE)		read_data = STR__OFFLINE;
			else							read_data = STR__ONLINE;
		}

		return 1;
	}
	
	// eRB1.ArmAWfr   (Unknown Absent Present)
	if(diCH__ARM_A_WFR_MANUAL->Check__VARIABLE_NAME(var_name) > 0)
	{
		str__cmmd = "RQ LOAD A";
		flag = Drv__RECEIVE_COMMAND(var_name, str__cmmd,str__recv_data);
		
		if(flag < 0)		return -1;

		if(str__recv_data.GetAt(5) == '?')
		{
			read_data = STR__UNKNOWN;
			return 1;
		}
		if((str__recv_data.GetAt(5) == 'O')&&(str__recv_data.GetAt(6) == 'F'))
		{
			read_data = STR__Absent;
			return 1;
		}
			
		read_data = STR__Present;
		return 1;
	}

	// eRB1.VacSnsA   (ON OFF)
	if(diCH__VAC_SNS_A->Check__VARIABLE_NAME(var_name) > 0)
	{
		str__cmmd = "RQ VAC GRIP A";
		flag = Drv__RECEIVE_COMMAND(var_name, str__cmmd,str__recv_data);
	
		if(flag < 0)		return -1;

		if(str__recv_data.GetLength() > 11)  
		{
			char ch1;
			char ch2;

			ch1 = str__recv_data.GetAt(9);    // 'O'
			ch2 = str__recv_data.GetAt(10);   // 'N' or 'F'

			if((ch1 == 'O') && (ch2 == 'F'))
			{
				sDRV__RB_VAC_SNS_A = "OFF";
			}
			else
			{
				sDRV__RB_VAC_SNS_A = "ON";
			}

			read_data = sDRV__RB_VAC_SNS_A;
		}
		return 1;
	}

	// eRB1.Rescan  (D/I) N/Y
	if(diCH__RESCAN->Check__VARIABLE_NAME(var_name) > 0)
	{
		str__cmmd = "RQ ALIGNER RSLT";
		flag = Drv__RECEIVE_COMMAND(var_name, str__cmmd,str__recv_data);
	
		if(flag < 0)		return -1;

		if(str__recv_data.GetLength() > 18)  
		{
			CString str1;
			CString str2;
			CString str3;
			CString str4;

			Fnc__IndexStrTo4Word(str__recv_data,' ',14,&str1,&str2,&str3,&str4);

			dDRV__RB_ANGLE_POS = atof(str1);
			dDRV__RB_ANGLE_ECC = atof(str2);
			dDRV__RB_ECC_MAG   = atof(str3);
			dDRV__RB_AVG_RD    = atof(str4);
			dDRV__RB_ECC_MAG /= 1000.0;
									
			Fnc__IndexStrTo1Word(str__recv_data,CR,36,&str1);
			
			if(str1.CompareNoCase("N") == 0)	sDRV__RESCAN = "N";
			else								sDRV__RESCAN = "Y";

			str__recv_data = str1;
		}
		return 1;
	}

	// eRB1.CurrStn (1 --- 25)
	if(diCH__CURR_STN->Check__VARIABLE_NAME(var_name) > 0)
	{
		str__cmmd = "RQ POS STN T";
		flag = Drv__RECEIVE_COMMAND(var_name, str__cmmd,str__recv_data);
	
		if(flag < 0)		return -1;
		
		if(str__recv_data.GetLength() > 7)		// OK
		{
			str__recv_data.SetAt(0,'0');  // 'P'
			str__recv_data.SetAt(1,'0');  // 'O'
			str__recv_data.SetAt(2,'0');  // 'S'
			str__recv_data.SetAt(3,'0');  // ' '
			str__recv_data.SetAt(4,'0');  // 'S'
			str__recv_data.SetAt(5,'0');  // 'T'
			str__recv_data.SetAt(6,'0');  // 'N'
			str__recv_data.SetAt(7,'0');  // ' '

			read_data.Format("%d", atoi(str__recv_data));
		}
		return 1;		
	}
	
	// eRB1.VacStsA (ON OFF)
	if(diCH__VAC_STS_A->Check__VARIABLE_NAME(var_name) > 0)
	{
		str__cmmd = "RQ VAC GRIP A";
		flag = Drv__RECEIVE_COMMAND(var_name, str__cmmd,str__recv_data);
	
		if(flag < 0)		return -1;
		
		if(str__recv_data.GetLength() > 11)		// OK
		{
			char ch1;
			char ch2;

			ch1 = str__recv_data.GetAt(9);	  // 'O'
			ch2 = str__recv_data.GetAt(10);   // 'N' or 'F'

			if((ch1 == 'O')&&(ch2 == 'F'))     sDRV__RB_VAC_SNS_A = "OFF";
			else							   sDRV__RB_VAC_SNS_A = "ON";

			read_data = sDRV__RB_VAC_SNS_A;
		}
		return 1;		
	}
	
	// eRB1.ALWfr ( ? No Yes )
	if(diCH__AL_WFR->Check__VARIABLE_NAME(var_name) > 0)
	{
		str__cmmd = "RQ ALIGNER RQWPRS";
		flag = Drv__RECEIVE_COMMAND(var_name, str__cmmd,str__recv_data);
	
		if(flag < 0)		return -1;
		
		if(str__recv_data.GetLength() > 14)  
		{
			CString str1;
			Fnc__IndexStrTo1Word(str__recv_data,CR,17,&str1);

			str1.TrimLeft();
			read_data = str1;
		}
		return 1;
	}

	//
	if(diCH__ROBOT_CURR_POS->Check__VARIABLE_NAME(var_name) > 0)
	{
		str__cmmd = "RQ POS STN ALL";
		flag = Drv__RECEIVE_COMMAND(var_name, str__cmmd,str__recv_data);
	
		if(flag < 0)		return -1;
		
		if(str__recv_data.GetLength() > 7)  
		{
			CString str1;
			CString str2;
			CString str3;
			CString str4;

			Fnc__IndexStrTo4Word(str__recv_data,' ',9,&str1,&str2,&str3,&str4);

			// (R)		
			     if(str1.Compare("RE") == 0)		sDRV__RB_AR_POS = "Retract";
			else if(str1.Compare("EX") == 0)		sDRV__RB_AR_POS = "Extend";
			else                            		sDRV__RB_AR_POS = "Unknown";

			// rotation (T)
			int b_str2 = atoi(str2);

			if((b_str2 >= 1)&&(b_str2 <= 25))
			{
				sDRV__RB_T_POS.Format("%1d",b_str2);
			}
			else
			{
				sDRV__RB_T_POS = "Unknown";
			}

			// (Z)		
				 if(str4.Compare("DN") == 0)		sDRV__RB_Z_POS = "Down";
			else if(str4.Compare("UP") == 0)		sDRV__RB_Z_POS = "Up";
			else                            		sDRV__RB_Z_POS = "Unknown";

			read_data = "On";
		}
		return 1;
	}

	if(diCH__ROBOT_Z_POS->Check__VARIABLE_NAME(var_name) > 0)
	{
		read_data = sDRV__RB_Z_POS;
		return 1;  
	}
	if(diCH__ROBOT_AR_POS->Check__VARIABLE_NAME(var_name) > 0)
	{
		read_data = sDRV__RB_AR_POS;
		return 1;
	}
	if(diCH__ROBOT_T_POS->Check__VARIABLE_NAME(var_name) > 0)
	{
		read_data = sDRV__RB_T_POS;
		return 1;  
	}

	return -1;
}

int CObj__ATM_RelianceATR
::__Read__STRING(const CString& var_name, const CDS_IO__CHANNEL_INFO& io_info, CString& read_data)
{

	return -1;
}


//------------------------------------------------------------------------------------
int CObj__ATM_RelianceATR
::__Write__ANALOG(const CString& var_name, const CDS_IO__CHANNEL_INFO& io_info, const double set_data)
{
	iDRV__ERROR_ID = 0;
	sCH__INR__ERROR_ID->Set__DATA("0");
	iDRV__ALGNER_ERROR_ID = 0;
	sCH__INR__ALGINER_ERROR_ID->Set__DATA("0");

	if(aoCH__ANGLE->Check__VARIABLE_NAME(var_name) > 0)
	{
		dDRV__RB_ANGLE = set_data;
	 	return 1;  
	}
	
	return 1;
}

int CObj__ATM_RelianceATR
::__Write__DIGITAL(const CString& var_name,const CDS_IO__CHANNEL_INFO& io_info, const CString& set_data,const int item_index)
{
	// ...
	{
		CString log_msg;
		CString log_bff;

		log_msg = "__Write__DIGITAL() ... \n";

		log_bff.Format(" *** var_name : %s \n", var_name);
		log_msg += log_bff;

		log_bff.Format(" *** set_data : %s \n", set_data);
		log_msg += log_bff;

		Fnc__DRV_LOG(log_msg);
	}

	// ...
	CString str__recv_data;
	CString str_data;
	CString str__cmmd;
    int flag;

	iDRV__ERROR_ID = 0;
	sCH__INR__ERROR_ID->Set__DATA("0");
	iDRV__ALGNER_ERROR_ID = 0;
	sCH__INR__ALGINER_ERROR_ID->Set__DATA("0");

	// eRB2.Command
	//    Goto Pick ALPick AlgnPick Place  CheckLoad SetVac Home Reset MapStn GetMap SetALStn StoALStn InitAlgn ALSetComm ALStoComm ALAlign ALScan ALHome ALReset ALResult ALLoadPP ALSetPP ALSetWfr Reinit
	if(doCH__COMMAND->Check__VARIABLE_NAME(var_name) > 0)
	{
		if(set_data.CompareNoCase("Goto") == 0) 
		{
			str__cmmd.Format("GOTO N %d ARM %s R %s Z %s SLOT %d",
							 iDRV__STN_NO,
							 sDRV__ARM_NO,
							 sDRV__R_POS,
							 sDRV__Z_POS,
							 iDRV__SLOT_NO);

			flag = Drv__SEND_COMMAND(var_name, str__cmmd,str__recv_data, m_Out_Time);
		}
		else if(set_data.CompareNoCase("Pick") == 0) 
		{
			/*
			str__cmmd.Format("PICK %d SLOT %d ARM %s STRT %s ENRT %s",
							 iDRV__STN_NO,
							 iDRV__SLOT_NO,
							 sDRV__ARM_NO,
							 sDRV__RB_STRT,
							 sDRV__RB_ENRT);
		    */
			str__cmmd.Format("PICK %d SLOT %d",
							 iDRV__STN_NO,
							 iDRV__SLOT_NO);
			
			flag = Drv__SEND_COMMAND(var_name, str__cmmd,str__recv_data, m_Out_Time);  
		}
		else if(set_data.CompareNoCase("AlgnPick") == 0)	  // Align + Pick
		{
			str__cmmd.Format("PICK %d SLOT %d ARM %s STRT NR ALGN",
							 iDRV__STN_NO,
							 iDRV__SLOT_NO,
							 sDRV__ARM_NO);

			// ...
			int nAlgn_Pick_Time;
			CString str_drv_log;

			aCH__CFG__ALIGN_AND_PICK_TIMEOUT->Get__DATA(str_data);
			nAlgn_Pick_Time = atof(str_data)*1000;

			str_drv_log.Format("--> Config Value:%d", nAlgn_Pick_Time);
			Fnc__DRV_LOG(str_drv_log);
			
			flag = Drv__SEND_COMMAND(var_name, str__cmmd,str__recv_data, nAlgn_Pick_Time); 
		}
		else if(set_data.CompareNoCase("ALPick") == 0)        // delta Pick only
		{
			str__cmmd.Format("PICK %d PALGN", iDRV__STN_NO);

			flag = Drv__SEND_COMMAND(var_name, str__cmmd,str__recv_data, m_Out_Time); 
		}
		else if(set_data.CompareNoCase("Place") == 0) 
		{
			/*
			str__cmmd.Format("PLACE %d SLOT %d ARM %s STRT %s ENRT %s",
							 iDRV__STN_NO,
							 iDRV__SLOT_NO,
							 sDRV__ARM_NO,
							 sDRV__RB_STRT,
							 sDRV__RB_ENRT);
		    */
			str__cmmd.Format("PLACE %d SLOT %d",
							 iDRV__STN_NO,
							 iDRV__SLOT_NO);

			flag = Drv__SEND_COMMAND(var_name, str__cmmd,str__recv_data, m_Out_Time);
		}
		else if(set_data.CompareNoCase("PlaceAlgn") == 0) 
		{
			str__cmmd.Format("PLACE %d ENRT NR", iDRV__STN_NO);

			flag = Drv__SEND_COMMAND(var_name, str__cmmd,str__recv_data, m_Out_Time);
		}
		else if(set_data.CompareNoCase("CheckLoad") == 0)
		{
			flag = 1;
		}
		else if(set_data.CompareNoCase("SetVac") == 0) 
		{
			str__cmmd.Format("VAC %s", sDRV__RB_VACUUM);
			
			flag = Drv__SEND_COMMAND(var_name, str__cmmd,str__recv_data, m_Out_Time);
		}
		else if(set_data.CompareNoCase("Home") == 0) 
		{
			str__cmmd.Format("HOME %s", sDRV__RB_HOME_AXIS);
				
			flag = Drv__SEND_COMMAND(var_name, str__cmmd,str__recv_data, m_Out_Time);
		}
		else if(set_data.CompareNoCase("Reset") == 0)
		{
			str__cmmd = "RESET";

			flag = Drv__SEND_COMMAND(var_name, str__cmmd,str__recv_data, m_Out_Time); 
		}
		else if(set_data.CompareNoCase("MapStn") == 0)	// LoadPort 에서 Mapping 한다.
		{
			str__cmmd.Format("MAP %d", iDRV__STN_NO);

			flag = Drv__SEND_COMMAND(var_name, str__cmmd,str__recv_data, m_Out_Time);
		}
		else if(set_data.CompareNoCase("GetMap") == 0) // LoadPort 에서 Mapping한 결과를 가져온다.
		{
			str__cmmd.Format("RSR %d", iDRV__STN_NO);

			flag = Drv__SEND_COMMAND(var_name, str__cmmd,str__recv_data, m_Out_Time);
		}
		else if(set_data.CompareNoCase("MapClear") == 0)	// 해당 station의 현재 map 정보를 clear 한다.
		{
			str__cmmd.Format("SET MP CLEAR %d", iDRV__STN_NO);
				
			flag = Drv__SEND_COMMAND(var_name, str__cmmd,str__recv_data, m_Out_Time);
		}
		else if(set_data.CompareNoCase("SetALStn") == 0) 
		{
			str__cmmd.Format("SET STN ALGN %d", iDRV__STN_NO);
				
			flag = Drv__SEND_COMMAND(var_name, str__cmmd,str__recv_data, m_Out_Time);
		}
		else if(set_data.CompareNoCase("StoALStn") == 0) 
		{
			str__cmmd = "STORE STN ALGN";

			flag = Drv__SEND_COMMAND(var_name, str__cmmd,str__recv_data, m_Out_Time);
		}
		else if(set_data.CompareNoCase("InitAlgn") == 0) 
		{
			str__cmmd.Format("INIT ALIGNER WAFER SIZE %s", sDRV__RB_MATERIAL);

			flag = Drv__SEND_COMMAND(var_name, str__cmmd,str__recv_data, m_Out_Time);
		}
		else if(set_data.CompareNoCase("ALSetComm") == 0)
		{
			str__cmmd = "HLLO";

			flag = Drv__SEND_COMMAND(var_name, str__cmmd,str__recv_data, m_Out_Time);
		}
		else if(set_data.CompareNoCase("ALStoComm") == 0)
		{
			str__cmmd = "HLLO";

			flag = Drv__SEND_COMMAND(var_name, str__cmmd,str__recv_data, m_Out_Time);
		}
		else if(set_data.CompareNoCase("ALAlign") == 0)
		{
			str__cmmd = "ALIGNER ALGN";

			flag = Drv__SEND_COMMAND(var_name, str__cmmd,str__recv_data, m_Out_Time);
		}
		else if(set_data.CompareNoCase("ALScan") == 0)
		{
			str__cmmd = "ALIGNER SCAN";

			flag = Drv__SEND_COMMAND(var_name, str__cmmd,str__recv_data, m_Out_Time);
		}
		else if(set_data.CompareNoCase("ALHome") == 0)
		{
			str__cmmd = "ALIGNER HOME";

			flag = Drv__SEND_COMMAND(var_name, str__cmmd,str__recv_data, m_Out_Time);
		}
		else if(set_data.CompareNoCase("ALReset") == 0)
		{
			str__cmmd = "ALIGNER RESET";

			flag = Drv__SEND_COMMAND(var_name, str__cmmd,str__recv_data, m_Out_Time);
		}
		else if(set_data.CompareNoCase("ALResult") == 0) 
		{
 			str__cmmd = "RQ ALIGNER RSLT";

			flag = Drv__SEND_COMMAND(var_name, str__cmmd,str__recv_data, m_Out_Time);
		}
		else if(set_data.CompareNoCase("ALLoadPP") == 0) 
		{
			iDRV__RB_WFR_CCD = 1;
			str__cmmd.Format("ALIGNER LDPS %d %06.0f", 
							 iDRV__RB_WFR_CCD, 
							 (dDRV__RB_ANGLE*1000.0));

			/*
			// jglee : Manual에는 10를 곱하라고 하는데 1000이 맞는것 같음
			iDRV__RB_WFR_CCD = 1;
			str__cmmd.Format("ALIGNER LDPS %d %04.0f", 
							 iDRV__RB_WFR_CCD, 
							 (dDRV__RB_ANGLE*10.0));
			*/

			flag = Drv__SEND_COMMAND(var_name, str__cmmd,str__recv_data, m_Out_Time);
		}
		else if(set_data.CompareNoCase("ALSetPP") == 0) 
		{
			iDRV__RB_WFR_CCD = 1;
			str__cmmd.Format("ALIGNER SLPS %d", iDRV__RB_WFR_CCD);

			flag = Drv__SEND_COMMAND(var_name, str__cmmd,str__recv_data, m_Out_Time);
		}
		else if(set_data.CompareNoCase("ALSetWfr") == 0) 
		{
			str__cmmd.Format("ALIGNER SLWF SHAPE %s SIZE %s FDCL %s",
							 sDRV__RB_WFR_SHAPE,
							 sDRV__RB_WFR_SIZE,
							 sDRV__RB_WFR_FDCL);

			flag = Drv__SEND_COMMAND(var_name, str__cmmd,str__recv_data, m_Out_Time);
		}	
		else if(set_data.CompareNoCase("Reinit") == 0)  
		{
			int ntimeout = 5000;		// msec

			str__cmmd = "SET COMM M/B PKT FLOW SEQ LF OFF";
			flag = Drv__SEND_COMMAND(var_name, str__cmmd,str__recv_data, ntimeout);	
			if(flag < 0)	return -1;
			
			str__cmmd = "SET IO ECHO N";
			flag = Drv__SEND_COMMAND(var_name, str__cmmd,str__recv_data, ntimeout);
			if(flag < 0)	return -1;

			str__cmmd = "STORE COMM ALL";
			flag = Drv__SEND_COMMAND(var_name, str__cmmd,str__recv_data, ntimeout);
			if(flag < 0)	return -1;
			
			str__cmmd = "STORE IO ECHO";
			flag = Drv__SEND_COMMAND(var_name, str__cmmd,str__recv_data, ntimeout);
			if(flag < 0)	return -1;
		
			str__cmmd = "RQ VERSION";
			flag = Drv__SEND_COMMAND(var_name, str__cmmd,str__recv_data, ntimeout);
			if(flag < 0)	return -1;
		}
		
		return flag;   
	}
	
	// eRB1.Stn (1- 25)
	if(doCH__STN->Check__VARIABLE_NAME(var_name) > 0)
	{
		iDRV__STN_NO = atoi(set_data);
		return 1;  
	}
	// eRB1.Arm (A B)
	if(doCH__ARM->Check__VARIABLE_NAME(var_name) > 0)
	{
		if(set_data.CompareNoCase("B") == 0)	sDRV__ARM_NO = "B";   
		else                                	sDRV__ARM_NO = "A";

		return 1;   
	}
	// eRB1.Slot (1 - 30)
	if(doCH__SLOT->Check__VARIABLE_NAME(var_name) > 0)
	{
		iDRV__SLOT_NO = atoi(set_data);
		return 1;  
	}
	// eRB1.ZPos (DN  UP)
	if(doCH__ZPOS->Check__VARIABLE_NAME(var_name) > 0)
	{
		if(set_data.CompareNoCase("UP") == 0)	sDRV__Z_POS = "UP";
		else                                    sDRV__Z_POS = "DN";

		return 1;  
	}
	// eRB1.Vacuum (Off On)
	if(doCH__VACUUM->Check__VARIABLE_NAME(var_name) > 0)
	{
		if(set_data.CompareNoCase("OFF") == 0)	sDRV__RB_VACUUM = "OFF";
		else                                    sDRV__RB_VACUUM = "ON";

	 	return 1;  
	}

	// eRB1.STRT (Normal Retract2 NoRetract)
	// NR: No retract
	// R1: Normal retract
	// R2: Second retract location
	// STRT: Start retract location
	if(doCH__STRT->Check__VARIABLE_NAME(var_name) > 0)
	{
			 if(set_data.CompareNoCase("Retract2")  == 0)		sDRV__RB_STRT = "R2";
		else if(set_data.CompareNoCase("NoRetract") == 0)		sDRV__RB_STRT = "NR";
		else													sDRV__RB_STRT = "R1";

		return 1;  
	}
	// eRB1.ENRT (Normal Retract2 NoRetract)
	// ENRT: End retract location
	if(doCH__ENRT->Check__VARIABLE_NAME(var_name) > 0)
	{
			 if(set_data.CompareNoCase("Retract2")  == 0)		sDRV__RB_ENRT = "R2";
		else if(set_data.CompareNoCase("NoRetract") == 0)		sDRV__RB_ENRT = "NR";
		else													sDRV__RB_ENRT = "R1";

	 	return 1;  
	}
	// eRB1.HomeAxis (ALL  R  T  Z  W  S)
	if(doCH__HOME_AXIS->Check__VARIABLE_NAME(var_name) > 0)
	{
			 if(set_data.CompareNoCase("R") == 0)		sDRV__RB_HOME_AXIS = "R";
		else if(set_data.CompareNoCase("T") == 0)		sDRV__RB_HOME_AXIS = "T";
		else if(set_data.CompareNoCase("Z") == 0)		sDRV__RB_HOME_AXIS = "Z";
		else if(set_data.CompareNoCase("W") == 0)	 	sDRV__RB_HOME_AXIS = "W";
		else if(set_data.CompareNoCase("S") == 0)		sDRV__RB_HOME_AXIS = "S";
		else											sDRV__RB_HOME_AXIS = "ALL";

		return 1;  
	}
	// eRB1.RPos (RE  EX)
	if(doCH__RPOS->Check__VARIABLE_NAME(var_name) > 0)
	{
			 if(set_data.CompareNoCase("EX") == 0)		sDRV__R_POS = "EX";
		else                                			sDRV__R_POS = "RE";

		return 1;  
	}
	// eRB1.ArmAWfr (Unknown Absent Present)
	if(doCH__ARM_A_WFR->Check__VARIABLE_NAME(var_name) > 0)
	{
			 if(set_data.CompareNoCase("Unknown") == 0)		sDRV__RB_ARM_A_WFR_SET = "?";
		else if(set_data.CompareNoCase("Absent")  == 0)		sDRV__RB_ARM_A_WFR_SET = "OFF";
		else                                        	sDRV__RB_ARM_A_WFR_SET = "ON";

		str__cmmd.Format("SET LOAD A %s", sDRV__RB_ARM_A_WFR_SET);
		
		return Drv__SEND_COMMAND(var_name,str__cmmd,str__recv_data, m_Out_Time);
	}
	// eRB1.WfrSize (1 2 3 4 5 6 7 20 21 22 23)
	if(doCH__WFR_SIZE->Check__VARIABLE_NAME(var_name) > 0)
	{
			 if(set_data.CompareNoCase("1")  == 0)		sDRV__RB_WFR_SIZE = "1";
		else if(set_data.CompareNoCase("2")  == 0)		sDRV__RB_WFR_SIZE = "2";
		else if(set_data.CompareNoCase("3")  == 0)		sDRV__RB_WFR_SIZE = "3";
		else if(set_data.CompareNoCase("4")  == 0)		sDRV__RB_WFR_SIZE = "4";
		else if(set_data.CompareNoCase("5")  == 0)		sDRV__RB_WFR_SIZE = "5";
		else if(set_data.CompareNoCase("6")  == 0)		sDRV__RB_WFR_SIZE = "6";
		else if(set_data.CompareNoCase("7")  == 0)		sDRV__RB_WFR_SIZE = "7";
		else if(set_data.CompareNoCase("20") == 0)		sDRV__RB_WFR_SIZE = "20";
		else if(set_data.CompareNoCase("21") == 0)		sDRV__RB_WFR_SIZE = "21";
		else if(set_data.CompareNoCase("22") == 0)		sDRV__RB_WFR_SIZE = "22";
		else                                			sDRV__RB_WFR_SIZE = "23";

		return 1;  
	}
	// eRB1.WfrFDCL (None  Flat  Ntch)
	if(doCH__WFR_FDCL->Check__VARIABLE_NAME(var_name) > 0)
	{
			 if(set_data.CompareNoCase("None") == 0)	sDRV__RB_WFR_FDCL = "NONE";
		else if(set_data.CompareNoCase("Flat") == 0)	sDRV__RB_WFR_FDCL = "FLAT";
		else                                			sDRV__RB_WFR_FDCL = "NTCH";

		return 1;  
	}
	// eRB1.WfrShape (Round Square)
	if(doCH__WFR_SHAPE->Check__VARIABLE_NAME(var_name) > 0)
	{
		     if(set_data.CompareNoCase("Round")  == 0)	sDRV__RB_WFR_SHAPE = "ROUND";
		else if(set_data.CompareNoCase("Square") == 0)	sDRV__RB_WFR_SHAPE = "SQUARE";
		else                                			sDRV__RB_WFR_SHAPE = "ROUND";

	 	return 1;  
	}
	// eATMRB1.WfrCCD
	if(doCH__WFR_CCD->Check__VARIABLE_NAME(var_name) > 0)
	{
			 if(set_data.CompareNoCase("1") == 0)		iDRV__RB_WFR_CCD = 1;
		else if(set_data.CompareNoCase("2") == 0)		iDRV__RB_WFR_CCD = 2;
		else                                			iDRV__RB_WFR_CCD = 3;

		return 1;  
	}

	// eRB1.Material (? 0 1 2 3 4 5 6 7 20 21 22 23 FP)
	/*
	3 = 100mm
	4 = 125mm
	5 = 150mm
	6 = 200mm
	7 = 300mm
	*/
	if(doCH__MATERIAL->Check__VARIABLE_NAME(var_name) > 0)
	{
			 if(set_data.CompareNoCase("0")  == 0)		sDRV__RB_MATERIAL = "0";
		else if(set_data.CompareNoCase("1")  == 0)		sDRV__RB_MATERIAL = "1";
		else if(set_data.CompareNoCase("2")  == 0)		sDRV__RB_MATERIAL = "2";
		else if(set_data.CompareNoCase("3")  == 0)		sDRV__RB_MATERIAL = "3";
		else if(set_data.CompareNoCase("4")  == 0)		sDRV__RB_MATERIAL = "4";
		else if(set_data.CompareNoCase("5")  == 0)		sDRV__RB_MATERIAL = "5";
		else if(set_data.CompareNoCase("6")  == 0)		sDRV__RB_MATERIAL = "6";
		else if(set_data.CompareNoCase("7")  == 0)		sDRV__RB_MATERIAL = "7";
		else if(set_data.CompareNoCase("20") == 0)		sDRV__RB_MATERIAL = "20";
		else if(set_data.CompareNoCase("21") == 0)		sDRV__RB_MATERIAL = "21";
		else if(set_data.CompareNoCase("22") == 0)		sDRV__RB_MATERIAL = "22";
		else if(set_data.CompareNoCase("23") == 0)		sDRV__RB_MATERIAL = "23";
		else if(set_data.CompareNoCase("FP") == 0)		sDRV__RB_MATERIAL = "FP";
		else                                			sDRV__RB_MATERIAL = "?";

		return 1;  
	}

	return 1;
}

int CObj__ATM_RelianceATR
::__Write__STRING(const CString& var_name, const CDS_IO__CHANNEL_INFO& io_info, const CString& set_data)
{
	SCX__SEQ_INFO x_seq_info;
	CString str_time;

	CString str__send_data;
	CString str__for_error;
	CString str__log;
	CString str__temp;
	CString str__timeout;
	int cmd_timeout = 0;

	aCH__HYPER_TERMINAL_CMD_TIMEOUT->Get__DATA(str__timeout);

	if(soCH__HYPER_TERMINAL_CMD->Check__VARIABLE_NAME(var_name) > 0)
	{
		str__send_data.Format("%s%c", set_data, CR);
		str__for_error.Format("%s",   set_data);
		
		cmd_timeout = atoi(str__timeout)*1000;	//
		str__log.Format("Hyper Terminal [%s] SEND => [%s], timeout:%d msec", var_name,str__for_error,cmd_timeout);
		Fnc__DRV_LOG(str__log);

		CString msg;
		mX_Serial->CLEAR__BUFFER(&msg);

		int nRet;
		nRet = mX_Serial->DATA__RECV(m_Term_Str,&str__send_data, cmd_timeout);
		str__log.Format("After Hyper Terminal, Revc.. Ret:%d, data[%s]",  nRet,str__send_data);
		Fnc__DRV_LOG(str__log);

		sCH__HYPER_TERMINAL_RECV_MSG->Set__DATA("");
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

	return -1;
}
