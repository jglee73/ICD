// CTC__PHY_PMC.h : main header file for the CTC__PHY_PMC DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CCTC__PHY_PMCApp
// See CTC__PHY_PMC.cpp for the implementation of this class
//

class CCTC__PHY_PMCApp : public CWinApp
{
public:
	CCTC__PHY_PMCApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
