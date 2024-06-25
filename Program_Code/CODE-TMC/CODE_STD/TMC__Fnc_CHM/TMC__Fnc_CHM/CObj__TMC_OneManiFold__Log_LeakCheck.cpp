#include "StdAfx.h"
#include "CObj__TMC_OneManiFold.h"
#include "CObj__TMC_OneManiFold__DEF.h"

#include "Macro_Function.h"


// ...
SCI__USER_LOG_CTRL* CObj__TMC_OneManiFold
::Get_Ptr__USER_LOG_CTRL(const CString& module_name)
{
	if(module_name.CompareNoCase(MODULE_NAME__TMC) == 0)
	{
		return xLOG_CTRL__LeakCheck_TMC.Get__SCI_PTR();
	}
	if(module_name.CompareNoCase(MODULE_NAME__LL1) == 0)
	{
		return xLOG_CTRL__LeakCheck_LBA.Get__SCI_PTR();
	}
	if(module_name.CompareNoCase(MODULE_NAME__LL2) == 0)
	{
		return xLOG_CTRL__LeakCheck_LBB.Get__SCI_PTR();
	}

	return NULL;
}

int  CObj__TMC_OneManiFold
::LeakCheck__LOG_MakeFile(const CString& module_name)
{
	SCI__USER_LOG_CTRL *p_log_ctrl = Get_Ptr__USER_LOG_CTRL(module_name);
	if(p_log_ctrl == NULL)		return -1;

	// ...
	CString dir_sub;
	dir_sub.Format("..\\LeakCheck\\%s\\%s", Macro__Get_SystemTimeDir(),module_name);

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
int  CObj__TMC_OneManiFold
::LeakCheck__LOG_WriteMsg(const CString& module_name,const CString& log_msg)
{
	SCI__USER_LOG_CTRL *p_log_ctrl = Get_Ptr__USER_LOG_CTRL(module_name);
	if(p_log_ctrl == NULL)		return -1;

	p_log_ctrl->WRITE__LOG(log_msg);
	return 1;
}
