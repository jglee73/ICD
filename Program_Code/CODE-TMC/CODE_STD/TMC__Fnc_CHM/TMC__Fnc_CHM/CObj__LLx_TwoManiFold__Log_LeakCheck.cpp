#include "StdAfx.h"
#include "CObj__LLx_TwoManiFold.h"
#include "CObj__LLx_TwoManiFold__DEF.h"

#include "Macro_Function.h"


// ...
SCI__USER_LOG_CTRL* CObj__LLx_TwoManiFold
::Get_Ptr__USER_LOG_CTRL(const CString& module_name)
{
	if(module_name.CompareNoCase(_LLx_NAME__LEFT) == 0)
	{
		return xLOG_CTRL__LeakCheck_LBA.Get__SCI_PTR();
	}
	if(module_name.CompareNoCase(_LLx_NAME__RIGHT) == 0)
	{
		return xLOG_CTRL__LeakCheck_LBB.Get__SCI_PTR();
	}

	return NULL;
}

int  CObj__LLx_TwoManiFold
::LeakCheck__LOG_MakeFile__LLx_Left()
{
	CString llx_name = _LLx_NAME__LEFT;
	CString log_name = sLOG_NAME__LLx_LEFT;

	return _LeakCheck__LOG_MakeFile(llx_name, log_name);
}
int  CObj__LLx_TwoManiFold
::LeakCheck__LOG_MakeFile__LLx_Right()
{
	CString llx_name = _LLx_NAME__RIGHT;
	CString log_name = sLOG_NAME__LLx_RIGHT;

	return _LeakCheck__LOG_MakeFile(llx_name, log_name);
}
int  CObj__LLx_TwoManiFold
::_LeakCheck__LOG_MakeFile(const CString& llx_name, 
						   const CString& log_name)
{
	SCI__USER_LOG_CTRL *p_log_ctrl = Get_Ptr__USER_LOG_CTRL(llx_name);
	if(p_log_ctrl == NULL)		return -1;

	// ...
	CString dir_sub;
	dir_sub.Format("..\\LeakCheck\\%s\\%s", Macro__Get_SystemTimeDir(), log_name);

	// ...
	int		i;
	int		nLen;
	CString szDir = "";
	char    cCh;

	nLen = dir_sub.GetLength();

	for(i=0; i<nLen; i++)
	{
		cCh = dir_sub.GetAt(i);
		szDir += cCh;
		if(cCh == '\\')	p_log_ctrl->CREATE__SUB_DIRECTORY(szDir);
	}
	p_log_ctrl->CREATE__SUB_DIRECTORY(szDir);	

	// ...
	CString file_name;
	file_name.Format("%s.Lek", Macro__Get_SystemTimeFile());

	p_log_ctrl->SET__PROPERTY(file_name, 31,1440);
	p_log_ctrl->DISABLE__TIME_LOG();

	p_log_ctrl->WRITE__LOG("No,Time,Pressure,Leak Rate"); 
	return 1;
}

int  CObj__LLx_TwoManiFold
::LeakCheck__LOG_WriteMsg__LLx_Left(const CString& log_msg)
{
	CString llx_name = _LLx_NAME__LEFT;

	return _LeakCheck__LOG_WriteMsg(llx_name, log_msg);
}
int  CObj__LLx_TwoManiFold
::LeakCheck__LOG_WriteMsg__LLx_Right(const CString& log_msg)
{
	CString llx_name = _LLx_NAME__RIGHT;

	return _LeakCheck__LOG_WriteMsg(llx_name, log_msg);
}
int  CObj__LLx_TwoManiFold
::_LeakCheck__LOG_WriteMsg(const CString& llx_name,
						   const CString& log_msg)
{
	SCI__USER_LOG_CTRL *p_log_ctrl = Get_Ptr__USER_LOG_CTRL(llx_name);
	if(p_log_ctrl == NULL)		return -1;

	p_log_ctrl->WRITE__LOG(log_msg);
	return 1;
}
