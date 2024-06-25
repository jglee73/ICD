#include "StdAfx.h"
#include "Interface_Code.h"

#include "CObj__ATM_RelianceATR.h"
#include "CObj__VAC_MAGATRAN.h"



//----------------------------------------------------------
DECLARE_START__DLL_IO_OBJ__GET_VERSION

#ifdef _DEBUG
debug_mode =  1;
#else
debug_mode = -1;			
#endif

DECLARE_END__DLL_IO_OBJ__GET_VERSION
//----------------------------------------------------------


//----------------------------------------------------------
#define CLS__ATM_RelianceATR				"ATM.RelianceATR"
#define CLS__VAC_MAGATRAN					"VAC.MAGATRAN"


DECLARE_START__DLL_IO_OBJ__GET_CLASS_LIST

ADD__OBJ_CLS(CLS__ATM_RelianceATR);
ADD__OBJ_CLS(CLS__VAC_MAGATRAN);

DECLARE_END__DLL_IO_OBJ__GET_CLASS_LIST


DECLARE_START__DLL_IO_OBJ__CREATE_LINK

IF__OBJ_CLS(CLS__ATM_RelianceATR)		return (new CObj__ATM_RelianceATR);
IF__OBJ_CLS(CLS__VAC_MAGATRAN)			return (new CObj__VAC_MAGATRAN);

DECLARE_END__DLL_IO_OBJ__CREATE_LINK
