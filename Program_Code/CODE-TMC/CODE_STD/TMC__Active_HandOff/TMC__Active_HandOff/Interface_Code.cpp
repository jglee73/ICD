#include "StdAfx.h"
#include "CObj__PMC_EX.h"
#include "CObj__PMC_STD.h"


//----------------------------------------------------------
DECLARE_START__DLL_STD_OBJ__GET_VERSION

#ifdef _DEBUG
debug_mode =  1;
#else
debug_mode = -1;			
#endif

DECLARE_END__DLL_STD_OBJ__GET_VERSION
//----------------------------------------------------------


//----------------------------------------------------------
#define CLS__PMC_EX					"PMC.EX"
#define CLS__PMC_STD				"PMC.STD"


DECLARE_START__DLL_STD_OBJ__GET_CLASS_LIST

ADD__OBJ_CLS(CLS__PMC_EX);
ADD__OBJ_CLS(CLS__PMC_STD);

DECLARE_END__DLL_STD_OBJ__GET_CLASS_LIST


DECLARE_START__DLL_STD_OBJ__CREATE_LINK

IF__OBJ_CLS(CLS__PMC_EX)			return (new CObj__PMC_EX);
IF__OBJ_CLS(CLS__PMC_STD)			return (new CObj__PMC_STD);

DECLARE_END__DLL_STD_OBJ__CREATE_LINK
