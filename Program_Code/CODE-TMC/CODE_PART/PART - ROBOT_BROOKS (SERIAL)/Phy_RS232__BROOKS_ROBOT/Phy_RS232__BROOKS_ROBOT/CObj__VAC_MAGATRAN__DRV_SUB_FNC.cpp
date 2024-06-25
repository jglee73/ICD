#include "StdAfx.h"
#include "CObj__VAC_MAGATRAN.h"
#include "CObj__VAC_MAGATRAN__DEF.h"

#include "math.h"
#include "Macro_Function.h"
#include "CCommon_DEF.h"


//------------------------------------------------------------------------------------
int  CObj__VAC_MAGATRAN
::Drv__CMD_COM_INIT(const CString& var_name)
{
	int flag;

	CString str__send_data;
	CString str__for_error;

	// Init 1 : SET COMM M/B PKT FLOW SEQ LF OFF 
	{
		str__send_data.Format("SET COMM M/B PKT FLOW SEQ LF OFF%c",CR);
		str__for_error.Format("SET COMM M/B PKT FLOW SEQ LF OFF<CR>");

		flag = Drv__SEND_MESSAGE_RETURN(var_name, str__send_data, str__for_error);
		if(flag < 0) return -1;
	}

	// Init 2 : SET IO ECHO N 
	{
		str__send_data.Format("SET IO ECHO N%c",CR);
		str__for_error.Format("SET IO ECHO N<CR>");

		flag = Drv__SEND_MESSAGE_RETURN(var_name, str__send_data, str__for_error);
		if(flag < 0) return -1;
	}

	// Init 3 : STORE IO ECHO 
	{
		str__send_data.Format("STORE IO ECHO%c",CR);		
		str__for_error.Format("STORE IO ECHO<CR>");	

		flag = Drv__SEND_MESSAGE_RETURN(var_name, str__send_data, str__for_error);
		if(flag < 0) return -1;
	}

	// Init 4 : STORE COMM ALL 
	{
		str__send_data.Format("STORE COMM ALL%c",CR);
		str__for_error.Format("STORE COMM ALL<CR>");

		flag = Drv__SEND_MESSAGE_RETURN(var_name, str__send_data, str__for_error);
		if(flag < 0) return -1;
	}

	// Init 5 : SET LOAD MODE TRI 
	{
		str__send_data.Format("SET LOAD MODE TRI%c",CR);
		str__for_error.Format("SET LOAD MODE TRI<CR>");

		flag = Drv__SEND_MESSAGE_RETURN(var_name, str__send_data, str__for_error);
		if(flag < 0) return -1;
	}

	// Init 6 : STORE LOAD MODE 
	{
		str__send_data.Format("STORE LOAD MODE%c",CR);
		str__for_error.Format("STORE LOAD MODE<CR>");

		flag = Drv__SEND_MESSAGE_RETURN(var_name, str__send_data, str__for_error);
		if(flag < 0) return -1;
	}

	return 1;
}

int CObj__VAC_MAGATRAN
::Drv__SEND_MESSAGE_RETURN(const CString& var_name, 
						   CString str__send_data, 
						   const CString& str__for_error)
{
	CString str__log;
	int index = 0;

	char check_char[10];

	// ...
	{
		str__log.Format("[%s] SEND => [%s]\n", var_name,str__for_error);
		Fnc__DRV_LOG(str__log);
	}

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

				return -1;
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

	return -1;
}
