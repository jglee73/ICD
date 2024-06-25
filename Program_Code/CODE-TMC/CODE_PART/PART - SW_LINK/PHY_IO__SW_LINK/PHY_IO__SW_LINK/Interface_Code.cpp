#include "StdAfx.h"
#include "Interface_Code.h"

#include "CObj__PART_SERVER.h"
#include "CObj__PART_CLIENT.h"


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
#define CLS__PART_SERVER						"PART.SERVER"
#define CLS__PART_CLIENT						"PART.CLIENT"


DECLARE_START__DLL_IO_OBJ__GET_CLASS_LIST

ADD__OBJ_CLS(CLS__PART_SERVER);
ADD__OBJ_CLS(CLS__PART_CLIENT);

DECLARE_END__DLL_IO_OBJ__GET_CLASS_LIST


DECLARE_START__DLL_IO_OBJ__CREATE_LINK

IF__OBJ_CLS(CLS__PART_SERVER)					return (new CObj__PART_SERVER);
IF__OBJ_CLS(CLS__PART_CLIENT)					return (new CObj__PART_CLIENT);

DECLARE_END__DLL_IO_OBJ__CREATE_LINK
