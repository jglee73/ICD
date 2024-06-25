// Phy__IO_LOADPORT.h : main header file for the Phy__IO_LOADPORT DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CPhy__IO_LOADPORTApp
// See Phy__IO_LOADPORT.cpp for the implementation of this class
//

class CPhy__IO_LOADPORTApp : public CWinApp
{
public:
	CPhy__IO_LOADPORTApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
