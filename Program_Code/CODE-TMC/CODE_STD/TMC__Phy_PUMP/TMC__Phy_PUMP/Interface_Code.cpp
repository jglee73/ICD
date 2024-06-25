#include "StdAfx.h"
#include "Interface_Code.h"

#include "CObj__IO_OBJ.h"
#include "CObj__IO_TYPE.h"
#include "CObj__IO_SIMPLE.h"
#include "CObj__STD_TYPE.h"
#include "CObj__VIRTUAL_TYPE.h"



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
#define CLS__IO_OBJ							"IO.OBJ"
#define CLS__IO_TYPE						"IO.TYPE"
#define CLS__IO_SIMPLE						"IO.SIMPLE"
#define CLS__STD_TYPE						"STD.TYPE"
#define CLS__VIRTUAL_TYPE					"VIRTUAL.TYPE"


DECLARE_START__DLL_STD_OBJ__GET_CLASS_LIST

ADD__OBJ_CLS(CLS__IO_OBJ);
ADD__OBJ_CLS(CLS__IO_TYPE);
ADD__OBJ_CLS(CLS__IO_SIMPLE);
ADD__OBJ_CLS(CLS__STD_TYPE);
ADD__OBJ_CLS(CLS__VIRTUAL_TYPE);

DECLARE_END__DLL_STD_OBJ__GET_CLASS_LIST


DECLARE_START__DLL_STD_OBJ__CREATE_LINK

IF__OBJ_CLS(CLS__IO_OBJ)					return (new CObj__IO_OBJ);
IF__OBJ_CLS(CLS__IO_TYPE)					return (new CObj__IO_TYPE);
IF__OBJ_CLS(CLS__IO_SIMPLE)					return (new CObj__IO_SIMPLE);
IF__OBJ_CLS(CLS__STD_TYPE)					return (new CObj__STD_TYPE);
IF__OBJ_CLS(CLS__VIRTUAL_TYPE)				return (new CObj__VIRTUAL_TYPE);

DECLARE_END__DLL_STD_OBJ__CREATE_LINK
