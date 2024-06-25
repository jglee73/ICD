#include "StdAfx.h"
#include "Interface_Code.h"

#include "CObj__SYSTEM.h"
#include "CObj__CDA.h"



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
#define CLS__SYSTEM					"SYSTEM"
#define CLS__CDA					"CDA"


DECLARE_START__DLL_STD_OBJ__GET_CLASS_LIST

	ADD__OBJ_CLS(CLS__SYSTEM);
	ADD__OBJ_CLS(CLS__CDA);

DECLARE_END__DLL_STD_OBJ__GET_CLASS_LIST


DECLARE_START__DLL_STD_OBJ__CREATE_LINK

	IF__OBJ_CLS(CLS__SYSTEM)			return (new CObj__SYSTEM);
	IF__OBJ_CLS(CLS__CDA)				return (new CObj__CDA);

DECLARE_END__DLL_STD_OBJ__CREATE_LINK
