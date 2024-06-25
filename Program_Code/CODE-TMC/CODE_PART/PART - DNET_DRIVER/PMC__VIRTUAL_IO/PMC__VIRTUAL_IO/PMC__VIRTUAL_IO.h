// PMC__VIRTUAL_IO.h : main header file for the PMC__VIRTUAL_IO DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CPMC__VIRTUAL_IOApp
// See PMC__VIRTUAL_IO.cpp for the implementation of this class
//

class CPMC__VIRTUAL_IOApp : public CWinApp
{
public:
	CPMC__VIRTUAL_IOApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
