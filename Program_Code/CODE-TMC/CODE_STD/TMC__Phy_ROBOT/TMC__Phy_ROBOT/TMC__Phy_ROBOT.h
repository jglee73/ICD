// TMC__Phy_ROBOT.h : main header file for the TMC__Phy_ROBOT DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CTMC__Phy_ROBOTApp
// See TMC__Phy_ROBOT.cpp for the implementation of this class
//

class CTMC__Phy_ROBOTApp : public CWinApp
{
public:
	CTMC__Phy_ROBOTApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
