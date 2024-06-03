// PMC__PHY_FRC.h : main header file for the PMC__PHY_FRC DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CPMC__PHY_FRCApp
// See PMC__PHY_FRC.cpp for the implementation of this class
//

class CPMC__PHY_FRCApp : public CWinApp
{
public:
	CPMC__PHY_FRCApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
