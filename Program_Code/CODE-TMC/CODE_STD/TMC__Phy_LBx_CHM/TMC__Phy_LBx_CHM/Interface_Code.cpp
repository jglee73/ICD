#include "StdAfx.h"
#include "Interface_Code.h"

#include "CObj__LBx_CHM_SIMPLE.h"
#include "CObj__LBx_CHM_SLOT.h"
#include "CObj__LBx_CHM_STD.h"


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
#define CLS__LBx_CHM_SIMPLE				"LBx_CHM.SIMPLE"
#define CLS__LBx_CHM_SLOT				"LBx_CHM.SLOT"
#define CLS__LBx_CHM_STD				"LBx_CHM.STD"


DECLARE_START__DLL_STD_OBJ__GET_CLASS_LIST

ADD__OBJ_CLS(CLS__LBx_CHM_SIMPLE);
ADD__OBJ_CLS(CLS__LBx_CHM_SLOT);
ADD__OBJ_CLS(CLS__LBx_CHM_STD);

DECLARE_END__DLL_STD_OBJ__GET_CLASS_LIST


DECLARE_START__DLL_STD_OBJ__CREATE_LINK

IF__OBJ_CLS(CLS__LBx_CHM_SIMPLE)		return (new CObj__LBx_CHM_SIMPLE);
IF__OBJ_CLS(CLS__LBx_CHM_SLOT)			return (new CObj__LBx_CHM_SLOT);
IF__OBJ_CLS(CLS__LBx_CHM_STD)			return (new CObj__LBx_CHM_STD);

DECLARE_END__DLL_STD_OBJ__CREATE_LINK
