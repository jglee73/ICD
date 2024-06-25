// TMC__Phy_PMC.h : main header file for the TMC__Phy_PMC DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CTMC__Phy_PMCApp
// See TMC__Phy_PMC.cpp for the implementation of this class
//

class CTMC__Phy_PMCApp : public CWinApp
{
public:
	CTMC__Phy_PMCApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
