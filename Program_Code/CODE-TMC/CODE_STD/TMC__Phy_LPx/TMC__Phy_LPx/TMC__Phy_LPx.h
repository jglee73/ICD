// TMC__Phy_LPx.h : main header file for the TMC__Phy_LPx DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CTMC__Phy_LPxApp
// See TMC__Phy_LPx.cpp for the implementation of this class
//

class CTMC__Phy_LPxApp : public CWinApp
{
public:
	CTMC__Phy_LPxApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
