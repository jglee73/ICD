// Phy_IO__DNET_HILSCHER.h : main header file for the Phy_IO__DNET_HILSCHER DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CPhy_IO__DNET_HILSCHERApp
// See Phy_IO__DNET_HILSCHER.cpp for the implementation of this class
//

class CPhy_IO__DNET_HILSCHERApp : public CWinApp
{
public:
	CPhy_IO__DNET_HILSCHERApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
