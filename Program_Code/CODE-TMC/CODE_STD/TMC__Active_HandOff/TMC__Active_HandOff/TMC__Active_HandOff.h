// TMC__Active_HandOff.h : main header file for the TMC__Active_HandOff DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CTMC__Active_HandOffApp
// See TMC__Active_HandOff.cpp for the implementation of this class
//

class CTMC__Active_HandOffApp : public CWinApp
{
public:
	CTMC__Active_HandOffApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
