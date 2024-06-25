// Phy_IO__DNET_HILSCHER_CIFX.h : main header file for the Phy_IO__DNET_HILSCHER_CIFX DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CPhy_IO__DNET_HILSCHER_CIFXApp
// See Phy_IO__DNET_HILSCHER_CIFX.cpp for the implementation of this class
//

class CPhy_IO__DNET_HILSCHER_CIFXApp : public CWinApp
{
public:
	CPhy_IO__DNET_HILSCHER_CIFXApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
