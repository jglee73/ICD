// CTC__Fnc_VAC_ROBOT__VER_01.h : main header file for the CTC__Fnc_VAC_ROBOT__VER_01 DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CCTC__Fnc_VAC_ROBOT__VER_01App
// See CTC__Fnc_VAC_ROBOT__VER_01.cpp for the implementation of this class
//

class CCTC__Fnc_VAC_ROBOT__VER_01App : public CWinApp
{
public:
	CCTC__Fnc_VAC_ROBOT__VER_01App();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
