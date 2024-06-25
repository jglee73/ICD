#include "StdAfx.h"
#include "CObj__DRV_TEST.h"
#include "CObj__DRV_TEST__DEF.h"

#include "Macro_Function.h"


//--------------------------------------------------------------------------------
int CObj__DRV_TEST::_Init__DNET_MASTER_BY_USER_CFG()
{
	bActive__DNET_INIT = false;

	int r_flag = mDNet_Mng.Call__CHANNEL_OPEN();

	HANDLE h_drv_board = mDNet_Mng.Get__DRV_BOARD_HANDLE();
	Fnc__ENUM_BOARD(h_drv_board);

	if(r_flag < 0)		return r_flag;

	bActive__DNET_INIT = true;
	return 1;
}

