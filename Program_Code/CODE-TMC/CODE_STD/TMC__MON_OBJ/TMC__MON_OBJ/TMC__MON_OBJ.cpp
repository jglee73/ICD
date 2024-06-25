// TMC__MON_OBJ.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "TMC__MON_OBJ.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

// CTMC__MON_OBJApp

BEGIN_MESSAGE_MAP(CTMC__MON_OBJApp, CWinApp)
END_MESSAGE_MAP()


// CTMC__MON_OBJApp construction

CTMC__MON_OBJApp::CTMC__MON_OBJApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CTMC__MON_OBJApp object

CTMC__MON_OBJApp theApp;


// CTMC__MON_OBJApp initialization

BOOL CTMC__MON_OBJApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
