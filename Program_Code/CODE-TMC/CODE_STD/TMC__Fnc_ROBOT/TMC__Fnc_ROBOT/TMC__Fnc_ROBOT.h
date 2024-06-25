// TMC__Fnc_ROBOT.h : main header file for the TMC__Fnc_ROBOT DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CTMC__Fnc_ROBOTApp
// See TMC__Fnc_ROBOT.cpp for the implementation of this class
//

class CTMC__Fnc_ROBOTApp : public CWinApp
{
public:
	CTMC__Fnc_ROBOTApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
