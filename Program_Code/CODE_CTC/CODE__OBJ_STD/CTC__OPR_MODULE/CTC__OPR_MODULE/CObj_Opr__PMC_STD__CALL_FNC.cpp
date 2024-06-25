#include "StdAfx.h"
#include "CObj_Opr__PMC_STD.h"


// ...
int CObj_Opr__PMC_STD::Call__PRO_START(CII_OBJECT__VARIABLE* p_variable)
{
	DECLARE__EXT_CTRL(p_variable);

	CString pm_recipe;
	p_variable->Get__VARIABLE_DATA(sVAR__RECIPE,pm_recipe);

	CStringArray l_para;
	int flag = -1;

	// ...
	{
		l_para.RemoveAll();
		l_para.Add(pm_recipe);

		flag = p_ext_mode_ctrl->Call__FNC_MODE(sEXT_MODE__PRO_READY,l_para);
	}

	if(flag > 0)
	{
		l_para.RemoveAll();
		l_para.Add(pm_recipe);

		flag = p_ext_mode_ctrl->Call__FNC_MODE(sEXT_MODE__PRO_START,l_para);
	}

	return flag;
}
