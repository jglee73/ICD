#include "StdAfx.h"
#include "CObj__DB_GUI.h"


//-------------------------------------------------------------------------
CObj__DB_GUI::CObj__DB_GUI()
{

}
CObj__DB_GUI::~CObj__DB_GUI()
{

}

//-------------------------------------------------------------------------
int CObj__DB_GUI::__DEFINE__CONTROL_MODE(obj,l_mode)
{
	sObject_Name = obj;

	return 1;
}
int CObj__DB_GUI::__DEFINE__VERSION_HISTORY(version)
{

	return 1;
}

int CObj__DB_GUI::__DEFINE__VARIABLE_STD(p_variable)
{
	DECLARE__STD_VARIABLE;

	// ...
	CString str_name;

	// ...
	{
		str_name = "sPOPUP.MNT.LBA";
		STD__ADD_STRING(str_name);

		str_name = "LBA.APC.POPUP";
		STD__ADD_STRING(str_name);

		//
		str_name = "sPOPUP.MNT.LBB";
		STD__ADD_STRING(str_name);

		str_name = "LBB.APC.POPUP";
		STD__ADD_STRING(str_name);

		//
		str_name = "sPOPUP.MNT.VACCHM";
		STD__ADD_STRING(str_name);

		str_name = "sPOPUP.MNT.VACROBOT";
		STD__ADD_STRING(str_name);

		str_name = "sPOPUP.MNT.LP1";
		STD__ADD_STRING(str_name);

		str_name = "sPOPUP.MNT.LP2";
		STD__ADD_STRING(str_name);

		str_name = "sPOPUP.MNT.LP3";
		STD__ADD_STRING(str_name);

		str_name = "sPOPUP.MNT.VAL";
		STD__ADD_STRING(str_name);

		str_name = "sPOPUP.MNT.FFU";
		STD__ADD_STRING(str_name);

		str_name = "sPOPUP.MNT.RF1";
		STD__ADD_STRING(str_name);

		str_name = "sPOPUP.MNT.RF2";
		STD__ADD_STRING(str_name);

		str_name = "sPOPUP.MNT.RF3";
		STD__ADD_STRING(str_name);

		str_name = "sPOPUP.MNT.E1";
		STD__ADD_STRING(str_name);

		str_name = "sPOPUP.MNT.E2";
		STD__ADD_STRING(str_name);

		str_name = "sPOPUP.MNT.E3";
		STD__ADD_STRING(str_name);

		str_name = "sPOPUP.MNT.ARB";
		STD__ADD_STRING(str_name);

		str_name = "sPOPUP.MNT.AL";
		STD__ADD_STRING(str_name);

		str_name = "sPOPUP.MNT.E84.OHT";
		STD__ADD_STRING(str_name);

		str_name = "sPOPUP.MNT.DRYPUMP";
		STD__ADD_STRING(str_name);

		str_name = "sPOPUP.MNT.INITIAL";
		STD__ADD_STRING(str_name);
	}
	return 1;
}


//-------------------------------------------------------------------------
int CObj__DB_GUI::__INITIALIZE__OBJECT(p_variable,p_ext_obj_create)
{
	CString def_name;
	CString def_data;

	return 1;
}


//-------------------------------------------------------------------------
int CObj__DB_GUI::__CALL__CONTROL_MODE(mode,p_debug,p_variable,p_alarm)
{

	return 1;
}

int CObj__DB_GUI::__CALL__MONITORING(id,p_variable,p_alarm)
{

	return 1;
}
