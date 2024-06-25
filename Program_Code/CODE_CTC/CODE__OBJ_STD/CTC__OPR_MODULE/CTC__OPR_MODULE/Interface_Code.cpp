#include "StdAfx.h"
#include "Interface_Code.h"

#include "CObj_Opr__AUTO_MODE.h"
#include "CObj_Opr__MAINT_MODE.h"
#include "CObj_Opr__PMC_STD.h"



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
#define CLS__AUTO_MODE					"AUTO.MODE"
#define CLS__MAINT_MODE					"MAINT.MODE"
#define CLS__PMC_STD					"PMC.STD"


DECLARE_START__DLL_STD_OBJ__GET_CLASS_LIST

ADD__OBJ_CLS(CLS__AUTO_MODE);
ADD__OBJ_CLS(CLS__MAINT_MODE);
ADD__OBJ_CLS(CLS__PMC_STD);

DECLARE_END__DLL_STD_OBJ__GET_CLASS_LIST


DECLARE_START__DLL_STD_OBJ__CREATE_LINK

IF__OBJ_CLS(CLS__AUTO_MODE)				return (new CObj_Opr__AUTO_MODE);
IF__OBJ_CLS(CLS__MAINT_MODE)			return (new CObj_Opr__MAINT_MODE);
IF__OBJ_CLS(CLS__PMC_STD)				return (new CObj_Opr__PMC_STD);

DECLARE_END__DLL_STD_OBJ__CREATE_LINK
