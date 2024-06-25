// TMC__DB_OBJ.h : main header file for the TMC__DB_OBJ DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CTMC__DB_OBJApp
// See TMC__DB_OBJ.cpp for the implementation of this class
//

class CTMC__DB_OBJApp : public CWinApp
{
public:
	CTMC__DB_OBJApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
