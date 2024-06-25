#include "StdAfx.h"
#include "CObj_Opr__MAINT_MODE.h"
#include "CObj_Opr__MAINT_MODE__DEF.h"

#include "Macro_Function.h"


// ...
int  CObj_Opr__MAINT_MODE::Call__CHECK_TARGET_INFO(CII_OBJECT__VARIABLE *p_variable)
{
	CString request_trg = sCH__TEST_TARGET_TITLE->Get__STRING();

	CDB__TARGET_INFO* p_db = &mDB__TRG_INFO;
	CString result_trg = p_db->Get__TARGET_INFO(request_trg);

	printf(" Call__CHECK_TARGET_INFO() - Report ... \n");
	printf(" * request_trg <- [%s] \n", request_trg);
	printf(" * result_trg  <- [%s] \n", result_trg );
	printf("\n");

	return 1;
}
