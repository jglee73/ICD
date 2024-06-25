#include "StdAfx.h"
#include "Interface_Code.h"

#include "CObj__CHM_ManiFold_X.h"
#include "CObj__CHM_TwoManiFold.h"
#include "CObj__CHM_OneManiFold.h"

#include "CObj__LLx_TwoManiFold.h"
#include "CObj__LLx_OneManiFold.h"

#include "CObj__TMC_OneManiFold.h"


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
#define CLS__CHM_ManiFold_X				    	"CHM.ManiFold_X"
#define CLS__CHM_TwoManiFold					"CHM.TwoManiFold"
#define CLS__CHM_OneManiFold					"CHM.OneManiFold"

#define CLS__LLx_TwoManiFold					"LLx.TwoManiFold"
#define CLS__LLx_OneManiFold					"LLx.OneManiFold"

#define CLS__TMC_OneManiFold					"TMC.OneManiFold"


DECLARE_START__DLL_STD_OBJ__GET_CLASS_LIST

ADD__OBJ_CLS(CLS__CHM_ManiFold_X);
ADD__OBJ_CLS(CLS__CHM_TwoManiFold);
ADD__OBJ_CLS(CLS__CHM_OneManiFold);

ADD__OBJ_CLS(CLS__LLx_TwoManiFold);
ADD__OBJ_CLS(CLS__LLx_OneManiFold);

ADD__OBJ_CLS(CLS__TMC_OneManiFold);

DECLARE_END__DLL_STD_OBJ__GET_CLASS_LIST


DECLARE_START__DLL_STD_OBJ__CREATE_LINK

IF__OBJ_CLS(CLS__CHM_ManiFold_X)				return (new CObj__CHM_ManiFold_X);
IF__OBJ_CLS(CLS__CHM_TwoManiFold)				return (new CObj__CHM_TwoManiFold);
IF__OBJ_CLS(CLS__CHM_OneManiFold)				return (new CObj__CHM_OneManiFold);

IF__OBJ_CLS(CLS__LLx_TwoManiFold)				return (new CObj__LLx_TwoManiFold);
IF__OBJ_CLS(CLS__LLx_OneManiFold)				return (new CObj__LLx_OneManiFold);

IF__OBJ_CLS(CLS__TMC_OneManiFold)				return (new CObj__TMC_OneManiFold);

DECLARE_END__DLL_STD_OBJ__CREATE_LINK
