#include "StdAfx.h"
#include "Interface_Code.h"

#include "CObj__ATM_ROBOT_FUSION.h"
#include "CObj__ATM_ROBOT_STD.h"

#include "CObj__VAC_ROBOT_STD.h"
#include "CObj__SIDE_STORAGE.h"

#include "CObj__DRV_LINK_ALx.h"
#include "CObj__DRV_LINK_RBx.h"


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
#define CLS__ATM_ROBOT_FUSION			"ATM_ROBOT.FUSION"
#define CLS__ATM_ROBOT_STD				"ATM_ROBOT.STD"

#define CLS__VAC_ROBOT_STD				"VAC_ROBOT.STD"
#define CLS__SIDE_STORAGE				"SIDE.STORAGE"

#define CLS__DRV_LINK_ALx				"DRV_LINK.ALx"
#define CLS__DRV_LINK_RBx				"DRV_LINK.RBx"


DECLARE_START__DLL_STD_OBJ__GET_CLASS_LIST

ADD__OBJ_CLS(CLS__ATM_ROBOT_FUSION);
ADD__OBJ_CLS(CLS__ATM_ROBOT_STD);

ADD__OBJ_CLS(CLS__VAC_ROBOT_STD);
ADD__OBJ_CLS(CLS__SIDE_STORAGE);

ADD__OBJ_CLS(CLS__DRV_LINK_ALx);
ADD__OBJ_CLS(CLS__DRV_LINK_RBx);

DECLARE_END__DLL_STD_OBJ__GET_CLASS_LIST


DECLARE_START__DLL_STD_OBJ__CREATE_LINK

IF__OBJ_CLS(CLS__ATM_ROBOT_FUSION)		return (new CObj__ATM_ROBOT_FUSION);
IF__OBJ_CLS(CLS__ATM_ROBOT_STD)			return (new CObj__ATM_ROBOT_STD);

IF__OBJ_CLS(CLS__VAC_ROBOT_STD)			return (new CObj__VAC_ROBOT_STD);
IF__OBJ_CLS(CLS__SIDE_STORAGE)			return (new CObj__SIDE_STORAGE);

IF__OBJ_CLS(CLS__DRV_LINK_ALx)			return (new CObj__DRV_LINK_ALx);
IF__OBJ_CLS(CLS__DRV_LINK_RBx)			return (new CObj__DRV_LINK_RBx);

DECLARE_END__DLL_STD_OBJ__CREATE_LINK
