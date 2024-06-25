#include "StdAfx.h"
#include "Interface_Code.h"

#include "CObj__ATM_ROBOT_STD.h"

#include "CObj__VAC_ROBOT_STD.h"
#include "CObj__VAC_ROBOT_EX.h"


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
#define CLS__ATM_ROBOT_STD					"ATM_ROBOT.STD"

#define CLS__VAC_ROBOT_STD					"VAC_ROBOT.STD"
#define CLS__VAC_ROBOT_EX					"VAC_ROBOT.EX"


DECLARE_START__DLL_STD_OBJ__GET_CLASS_LIST

ADD__OBJ_CLS(CLS__ATM_ROBOT_STD);

ADD__OBJ_CLS(CLS__VAC_ROBOT_STD);
ADD__OBJ_CLS(CLS__VAC_ROBOT_EX);

DECLARE_END__DLL_STD_OBJ__GET_CLASS_LIST


DECLARE_START__DLL_STD_OBJ__CREATE_LINK

IF__OBJ_CLS(CLS__ATM_ROBOT_STD)				return (new CObj__ATM_ROBOT_STD);

IF__OBJ_CLS(CLS__VAC_ROBOT_STD)				return (new CObj__VAC_ROBOT_STD);
IF__OBJ_CLS(CLS__VAC_ROBOT_EX)				return (new CObj__VAC_ROBOT_EX);

DECLARE_END__DLL_STD_OBJ__CREATE_LINK
