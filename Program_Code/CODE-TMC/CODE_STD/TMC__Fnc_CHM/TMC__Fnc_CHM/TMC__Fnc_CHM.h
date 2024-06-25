// TMC__Fnc_CHM.h : main header file for the TMC__Fnc_CHM DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CTMC__Fnc_CHMApp
// See TMC__Fnc_CHM.cpp for the implementation of this class
//

class CTMC__Fnc_CHMApp : public CWinApp
{
public:
	CTMC__Fnc_CHMApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
