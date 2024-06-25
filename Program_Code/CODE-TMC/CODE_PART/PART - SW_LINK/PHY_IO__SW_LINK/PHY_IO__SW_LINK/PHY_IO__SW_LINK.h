// PHY_IO__SW_LINK.h : main header file for the PHY_IO__SW_LINK DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CPHY_IO__SW_LINKApp
// See PHY_IO__SW_LINK.cpp for the implementation of this class
//

class CPHY_IO__SW_LINKApp : public CWinApp
{
public:
	CPHY_IO__SW_LINKApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
