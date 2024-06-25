#include "StdAfx.h"
#include "Interface_Code.h"

#include "CObj__PMC_STD.h"
#include "CObj__PMC_SIMPLE.h"


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
#define CLS__PMC_STD				"PMC.STD"
#define CLS__PMC_SIMPLE				"PMC.SIMPLE"


DECLARE_START__DLL_STD_OBJ__GET_CLASS_LIST

ADD__OBJ_CLS(CLS__PMC_STD);
ADD__OBJ_CLS(CLS__PMC_SIMPLE);

DECLARE_END__DLL_STD_OBJ__GET_CLASS_LIST


DECLARE_START__DLL_STD_OBJ__CREATE_LINK

IF__OBJ_CLS(CLS__PMC_STD)			return (new CObj__PMC_STD);
IF__OBJ_CLS(CLS__PMC_SIMPLE)		return (new CObj__PMC_SIMPLE);

DECLARE_END__DLL_STD_OBJ__CREATE_LINK
