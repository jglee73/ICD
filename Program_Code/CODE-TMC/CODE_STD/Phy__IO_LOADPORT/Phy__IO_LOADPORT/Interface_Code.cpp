#include "StdAfx.h"
#include "Interface_Code.h"

#include "CObj__BROOKS_IO.h"
#include "CObj__OHT_E84_IO.h"

#include "CObj__BROOKS_RS232.h"
#include "CObj__STD_TYPE.h"


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
#define CLS__BROOKS_IO				"BROOKS.IO"
#define CLS__OHT_E84_IO				"OHT_E84.IO"

#define CLS__BROOKS_RS232			"BROOKS.RS232"

#define CLS__STD_TYPE			    "STD.TYPE"


DECLARE_START__DLL_STD_OBJ__GET_CLASS_LIST

ADD__OBJ_CLS(CLS__BROOKS_IO);
ADD__OBJ_CLS(CLS__OHT_E84_IO);

ADD__OBJ_CLS(CLS__BROOKS_RS232);

ADD__OBJ_CLS(CLS__STD_TYPE);

DECLARE_END__DLL_STD_OBJ__GET_CLASS_LIST


DECLARE_START__DLL_STD_OBJ__CREATE_LINK

IF__OBJ_CLS(CLS__BROOKS_IO)			return (new CObj__BROOKS_IO);
IF__OBJ_CLS(CLS__OHT_E84_IO)		return (new CObj__OHT_E84_IO);

IF__OBJ_CLS(CLS__BROOKS_RS232)		return (new CObj__BROOKS_RS232);

IF__OBJ_CLS(CLS__STD_TYPE)			return (new CObj__STD_TYPE);

DECLARE_END__DLL_STD_OBJ__CREATE_LINK
