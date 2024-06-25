#include "StdAfx.h"
#include "CObj__ATM_RelianceATR.h"
#include "CObj__ATM_RelianceATR__DEF.h"

#include "math.h"
#include "CCommon_DEF.h"


//------------------------------------------------------------------------------------
int  CObj__ATM_RelianceATR
::Drv__CMD_COM_INIT(const CString& var_name)
{

	return 1;
}

int CObj__ATM_RelianceATR
::Drv__RECEIVE_COMMAND(const CString& var_name, 
					   const CString& str_cmmd, 
					   CString& str__recv_data)
{
	str__recv_data = "";

	// ...
	CString str__log;

	CString str_data;
	CString str__send_data;
	CString str__for_error;

	CString msg;
	mX_Serial->CLEAR__BUFFER(&msg);

	// ...
    str__send_data.Format("%s%c",  str_cmmd,CR);
	str__for_error.Format("%s<CR>",str_cmmd);
    
	str__log.Format("[%s] SEND => [%s]\n", var_name,str__for_error);
	Fnc__DRV_LOG(str__log);

	// ...
	{
		CString str_bff;
		mX_Serial->CLEAR__BUFFER(&str_bff);
	}

	// ...
	int nRet = mX_Serial->DATA__RECV(m_Term_Str,&str__send_data,m_Rcv_Time);
	
	str__log.Format("Ret:%d", nRet);
	Fnc__DRV_LOG(str__log);

	if(nRet >= 0)
	{
		m_nCommState = ONLINE;
		str__recv_data = str__send_data;

		if(str__send_data.GetLength() < 8 )		
		{  
			// ...
			{
				str__log.Format("[%s][%s] [ERROR] => [%s]\n",
								var_name,
								str__for_error,
								str__send_data);

				Fnc__DRV_LOG(str__log);
			}
			
			m_nCommState = ONLINE;	
			return -1; 
		}

		// ...
		{
			str__log.Format("[%s] RECV <= [%s]\n", 
						    var_name,
							str__send_data);

			Fnc__DRV_LOG(str__log);
		}
		
		if((str__send_data.GetAt(0) == '_')
		&& (str__send_data.GetAt(1) == 'E'))
		{
			// _ERR 0305
			str__send_data.SetAt(0,'0');     // '_'
			str__send_data.SetAt(1,'0');     // 'E'
			str__send_data.SetAt(2,'0');     // 'R'
			str__send_data.SetAt(3,'0');     // 'R'
			str__send_data.SetAt(4,'0');     // ' '

			// str__send_data = atoi(str__send_data);
            
			m_nCommState = ONLINE;  
			return 1;
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
	return -1;
}
int CObj__ATM_RelianceATR
::Drv__SEND_COMMAND(const CString& var_name, 
			        const CString& str_cmmd, 
			        CString& str__recv_data,
				    const int timeout)
{
	str__recv_data = "";

	// ...
	CString str__log;

	CString str_data;
	CString str__send_data;
	CString str__for_error;

	// ...
	{
		str__send_data.Format("%s%c",  str_cmmd,CR);
		str__for_error.Format("%s<CR>",str_cmmd);

		str__log.Format("[%s] SEND => [%s], timeout:%d msec\n", var_name,str__for_error, timeout);

		Fnc__DRV_LOG(str__log);
	}

	// ...
	{
		CString str_bff;
		mX_Serial->CLEAR__BUFFER(&str_bff);
	}

	// ...
	int nRet = mX_Serial->DATA__RECV(m_Term_Str,&str__send_data,timeout);

	str__log.Format("After SEND && RECV => [%s], nRet [%d], RECV_DATA [%s]\n", var_name,nRet,str__send_data);
	Fnc__DRV_LOG(str__log);

	if(nRet >= 0)
	{
		m_nCommState = ONLINE;
		str__recv_data = str__send_data;

		if(str__send_data.GetLength() < 8)
		{  
			return 1;  
		}
		
		if((str__send_data.GetAt(0) == '_')
		&& (str__send_data.GetAt(1) == 'E'))
		{
			// _ERR 0305         
			str__send_data.SetAt(0,'0');		// '_'
			str__send_data.SetAt(1,'0');		// 'E'
			str__send_data.SetAt(2,'0');		// 'R'
			str__send_data.SetAt(3,'0');		// 'R'
			str__send_data.SetAt(4,'0');		// ' '

			iDRV__ERROR_ID = atoi(str__send_data); 

			// ...
			CString str_err;
			str_err.Format("%d", iDRV__ERROR_ID);

			sCH__INR__ERROR_ID->Set__DATA(str_err);
			sCH__INR__ALGINER_ERROR_ID->Set__DATA(str_err);
		}
		else if((str__send_data.GetAt(0) == 'A')
			 && (str__send_data.GetAt(1) == 'L')
			 && (str__send_data.GetAt(2) == 'G')
			 && (str__send_data.GetAt(3) == 'N'))
		{
			// ALGN _ERR 0712
			str__send_data.SetAt(0,'0');		// 'A'
			str__send_data.SetAt(1,'0');		// 'L'
			str__send_data.SetAt(2,'0');		// 'G'
			str__send_data.SetAt(3,'0');		// 'N'
			str__send_data.SetAt(4,'0');		// ' '
			str__send_data.SetAt(5,'0');		// '_'
			str__send_data.SetAt(6,'0');		// 'E'
			str__send_data.SetAt(7,'0');		// 'R'
			str__send_data.SetAt(8,'0');		// 'R'
			str__send_data.SetAt(9,'0');		// ' '
			
			iDRV__ALGNER_ERROR_ID = atoi(str__send_data); 

			// ...
			CString str_err;
			str_err.Format("%d", iDRV__ALGNER_ERROR_ID);

			sCH__INR__ALGINER_ERROR_ID->Set__DATA(str_err);
			sCH__INR__ERROR_ID->Set__DATA(str_err);
		}
		else if((str__send_data.GetAt(0) == 'M')
			 && (str__send_data.GetAt(1) == 'A')
			 && (str__send_data.GetAt(2) == 'P')
			 && (str__send_data.GetLength() > 26))  
		{
			// Map data
			int limit = str__send_data.GetLength() - 5;
			int i;
			int j = 4;

			if(limit > CFG_LPx__SLOT_SIZE)		limit = CFG_LPx__SLOT_SIZE;
			
			for(i=0;i<limit;i++)
			{
				char ch = str__send_data.GetAt(j);
				j++;

					 if(ch == '0')		dCH__LPx_SLOT_STATUS[i]->Set__DATA("ABSENT");
				else if(ch == '1')		dCH__LPx_SLOT_STATUS[i]->Set__DATA("PRESENT");
				else					dCH__LPx_SLOT_STATUS[i]->Set__DATA("UNKNOWN");

			}					
		}
		else if((str__send_data.GetAt(0) == 'A')
			 && (str__send_data.GetAt(1) == 'l')
			 && (str__send_data.GetLength() > 18))  
		{
			CString str1;
			CString str2;
			CString str3;
			CString str4;
			CString str5;

			Fnc__IndexStrTo5Word(str__send_data,' ',14,&str1,&str2,&str3,&str4,&str5);

			dDRV__RB_ANGLE_POS = atof(str1);
			dDRV__RB_ANGLE_ECC = atof(str2);
			dDRV__RB_ECC_MAG   = atof(str3);
			dDRV__RB_AVG_RD    = atof(str4);
			dDRV__RB_ECC_MAG /= 1000.0;	
			
			if(str5.Compare("N") == 0)		sDRV__RESCAN = "N";
			else							sDRV__RESCAN = "Y";
		}
		// ROM Version
		else if((str__send_data.GetAt(0) == 'V')
			 && (str__send_data.GetLength() > 3))
		{
			CString str1;
			CString str2;

			Fnc__StrTo2Word(str__send_data, CR, &str1, &str2);

			sCH__ROM_VER->Set__DATA(str1);
			sCH__ROM_DATE->Set__DATA(str2);
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

	return -1;
}

int  CObj__ATM_RelianceATR
::Fnc__StrTo2Word(const CString& data,
				  const char section,
				  CString *p_str1,
				  CString *p_str2)
{
	CString word;
	char str_ch[2];
	char ch;

	int init_flag;
	int para;
	int limit;
	int i;

	// ...
	init_flag = 0;
	para = 0;
	str_ch[1] = 0;

	limit = data.GetLength();

	for(i=0; i<=limit; i++)
	{
		if(i<limit)		ch = data.GetAt(i);
		else			ch = section;

		if(ch == section)
		{
			if(word.GetLength() > 0)
			{
					 if(para == 0)	*p_str1 = word;
				else if(para == 1)	*p_str2 = word;

				word = "";
			}

			para++;
		}
		else
		{
			str_ch[0] = ch;
			word += str_ch;
		}
	}

	return 1;
}
int  CObj__ATM_RelianceATR
::Fnc__StrTo7Word(const CString& data,
				  const char section,
				  CString *p_str1,
				  CString *p_str2,
				  CString *p_str3,
				  CString *p_str4,
				  CString *p_str5,
				  CString *p_str6,
				  CString *p_str7)
{
	CString word;
	char str_ch[2];
	char ch;

	int init_flag;
	int para;
	int limit;
	int i;

	// ...
	init_flag = 0;
	para = 0;
	str_ch[1] = '\0';

	limit = data.GetLength();

	for(i=0;i<=limit;i++)
	{
		if(i<limit)		ch = data.GetAt(i);
		else			ch = section;

		if(ch == section)
		{
			if(word.GetLength() > 0)
			{
					 if(para == 0)	*p_str1 = word;
				else if(para == 1)	*p_str2 = word;
				else if(para == 2)  *p_str3 = word;
				else if(para == 3)  *p_str4 = word;
				else if(para == 4)  *p_str5 = word;
				else if(para == 5)  *p_str6 = word;
				else if(para == 6)  *p_str7 = word;

				word = "";
			}

			para++;
		}
		else
		{
			str_ch[0] = ch;
			word += str_ch;
		}
	}

	return 1;
}

//.....
int  CObj__ATM_RelianceATR::
Fnc__IndexStrTo1Word(const CString& data,
					 const char section,
					 int index,
					 CString *p_str1)
{
	//.....
	CString word;
	char str_ch[2];
	char ch;
	
	int init_flag;
	int para;
	int limit;
	int i;

	//.....
	init_flag = 0;
	para = 0;
	str_ch[1] = '\0';
	
	limit = data.GetLength();
	index--;

	for(i=index;i<=limit;i++)
	{
		if(i<limit)		ch = data.GetAt(i);
		else			ch = section;

		if(ch == section)
		{
			if(word.GetLength() > 0)
			{
				if(para == 0)		*p_str1 = word;

				word = "";
			}

			para++;
		}
		else
		{
			str_ch[0] = ch;
			word += str_ch;
		}
	}

	//
	return 1;
}
int  CObj__ATM_RelianceATR::
Fnc__IndexStrTo4Word(const CString& data,
					 const char section,
					 int index,
					 CString *p_str1,
					 CString *p_str2,
					 CString *p_str3,
					 CString *p_str4)
{
	//.....
	CString word;
	char str_ch[2];
	char ch;

	int  init_flag;
	int  para;
	int  limit;
	int  i;

	//.....
	init_flag = 0;
	para = 0;
	str_ch[1] = 0;

	limit = data.GetLength();
	index--;

	for(i=index;i<=limit;i++)
	{
		if(i<limit)			ch = data.GetAt(i);
		else				ch = section;

		if(ch == section)
		{
			if(word.GetLength() > 0)
			{
				     if(para == 0)	*p_str1 = word;
				else if(para == 1)	*p_str2 = word;
				else if(para == 2)  *p_str3 = word;
				else if(para == 3)  *p_str4 = word;

				word = "";
			}

			para++;
		}
		else
		{
			str_ch[0] = ch;
			word += str_ch;
		}
	}

	//
	return 1;
}
int  CObj__ATM_RelianceATR::
Fnc__IndexStrTo5Word(const CString& data,
					 const char section,
					 int index,
					 CString *p_str1,
					 CString *p_str2,
					 CString *p_str3,
					 CString *p_str4,
					 CString *p_str5)
{
	//.....
	CString word;
	char str_ch[2];
	char ch;

	int  init_flag;
	int  para;
	int  limit;
	int  i;

	//.....
	init_flag = 0;
	para = 0;
	str_ch[1] = 0;

	limit = data.GetLength();
	index--;

	for(i=index;i<=limit;i++)
	{
		if(i<limit)			ch = data.GetAt(i);
		else				ch = section;

		if(ch == section)
		{
			if(word.GetLength() > 0)
			{
					 if(para == 0)	*p_str1 = word;
				else if(para == 1)	*p_str2 = word;
				else if(para == 2)  *p_str3 = word;
				else if(para == 3)  *p_str4 = word;
				else if(para == 4)  *p_str5 = word;

				word = "";
			}

			para++;
		}
		else
		{
			str_ch[0] = ch;
			word += str_ch;
		}
	}

	//
	return 1;
}

//.....
int  CObj__ATM_RelianceATR::
Fnc__HexToDec(const CString& str_dec)
{
	//.....
	CString string;
	char str_ch[2];
	char ch;

	int  value;
	int  limit;
	int  i;

	//.....
	string = str_dec;
	string.MakeUpper();

	value = 0;
	str_ch[1] = 0;

	limit = string.GetLength();

	for(i=0;i<limit;i++)
	{
		if(i > 0)	value *= 16;

		//.....
		ch = string.GetAt(i);
		str_ch[0] = ch;

		     if(((ch >= '0')&&(ch <= '9')))		value += atoi(str_ch);
		else if(ch == 'A')						value += 10;
		else if(ch == 'B')						value += 11;
		else if(ch == 'C')						value += 12;
		else if(ch == 'D')						value += 13;
		else if(ch == 'E')						value += 14;
		else if(ch == 'F')						value += 15;		
	}

	//
	return value;
}
int  CObj__ATM_RelianceATR::
Fnc__DecToBin(int dec,
			  CString *p_str)
{
	//.....
	CString str_result;
	CString word;
	int value;
	int bit;

	//.....
	int flag = 1;

	while(flag > 0)
	{
		value = dec / 2;
		bit   = dec % 2;
		dec   = value;

		if(value >= 0)
		{
			word.Format("%1d",bit);
			str_result += word;
		}

		if(value == 0)
		{
			flag = 0;
		}
	}

	//.....
	char str_ch[2];
	int limit;
	int i;

	str_ch[1] = 0;
	word = "";

	limit = str_result.GetLength();

	for(i=limit-1;i>=0;i--)
	{
		str_ch[0] = str_result.GetAt(i); 
		word += str_ch;
	}

	//....
	
	*p_str = word;
	return 1;
}
