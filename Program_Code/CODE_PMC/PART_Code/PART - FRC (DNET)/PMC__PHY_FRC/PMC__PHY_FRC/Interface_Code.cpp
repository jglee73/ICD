#include "StdAfx.h"
#include "Interface_Code.h"

#include "CObj__DELTA_II_IO.h"
#include "CObj__MADEE_IO.h"



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
#define CLS__DELTA_II_IO							"DELTA_II.IO"
#define CLS__MADDEE_IO								"MADEE.IO"


DECLARE_START__DLL_STD_OBJ__GET_CLASS_LIST

ADD__OBJ_CLS(CLS__DELTA_II_IO);
ADD__OBJ_CLS(CLS__MADDEE_IO);

DECLARE_END__DLL_STD_OBJ__GET_CLASS_LIST


DECLARE_START__DLL_STD_OBJ__CREATE_LINK

IF__OBJ_CLS(CLS__DELTA_II_IO)						return (new CObj__DELTA_II_IO);
IF__OBJ_CLS(CLS__MADDEE_IO)							return (new CObj__MADEE_IO);

DECLARE_END__DLL_STD_OBJ__CREATE_LINK

