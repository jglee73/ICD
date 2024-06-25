// TMC__Phy_PUMP.h : main header file for the TMC__Phy_PUMP DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CTMC__Phy_PUMPApp
// See TMC__Phy_PUMP.cpp for the implementation of this class
//

class CTMC__Phy_PUMPApp : public CWinApp
{
public:
	CTMC__Phy_PUMPApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
