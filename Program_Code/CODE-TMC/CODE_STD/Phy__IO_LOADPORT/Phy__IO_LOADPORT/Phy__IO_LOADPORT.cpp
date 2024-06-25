// Phy__IO_LOADPORT.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "Phy__IO_LOADPORT.h"

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

// CPhy__IO_LOADPORTApp

BEGIN_MESSAGE_MAP(CPhy__IO_LOADPORTApp, CWinApp)
END_MESSAGE_MAP()


// CPhy__IO_LOADPORTApp construction

CPhy__IO_LOADPORTApp::CPhy__IO_LOADPORTApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CPhy__IO_LOADPORTApp object

CPhy__IO_LOADPORTApp theApp;


// CPhy__IO_LOADPORTApp initialization

BOOL CPhy__IO_LOADPORTApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
