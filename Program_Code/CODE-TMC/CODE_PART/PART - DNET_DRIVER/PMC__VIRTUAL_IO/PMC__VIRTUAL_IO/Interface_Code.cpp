#include "StdAfx.h"
#include "Interface_Code.h"


#include "CObj__MP2100_VIRTUAL.h"
#include "CObj__IO_VIRTUAL.h"


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
#define CLS__IO_VIRTUAL						"IO.VIRTUAL"
#define CLS__MP2100_VIRTUAL					"MP2100.VIRTUAL"


DECLARE_START__DLL_IO_OBJ__GET_CLASS_LIST

	ADD__OBJ_CLS(CLS__IO_VIRTUAL);
	ADD__OBJ_CLS(CLS__MP2100_VIRTUAL);

DECLARE_END__DLL_IO_OBJ__GET_CLASS_LIST


DECLARE_START__DLL_IO_OBJ__CREATE_LINK

	IF__OBJ_CLS(CLS__IO_VIRTUAL)			return (new CObj__IO_VIRTUAL);
	IF__OBJ_CLS(CLS__MP2100_VIRTUAL)		return (new CObj__MP2100_VIRTUAL);

DECLARE_END__DLL_IO_OBJ__CREATE_LINK
//----------------------------------------------------------
