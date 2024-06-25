#pragma once


class CDNet_Mng
{
private:
	int iBD_ID;

	CString sDRIVER_VERSION;
	CString sFIRMWARE_INFO;

	unsigned char usLAST__ERR_CODE;

public:	
	CDNet_Mng()
	{
		iBD_ID = 0;

		sDRIVER_VERSION = "?.?";
		sFIRMWARE_INFO  = "???";

		usLAST__ERR_CODE = 0;
	}

	// ...
	int Set__BOARD_ID(const int bd_id);
	int Get__BOARD_ID();

	CString Get__DRIVER_VERSION()
	{
		return sDRIVER_VERSION;
	}
	CString Get__FIRMWARE_INFO()
	{
		return sFIRMWARE_INFO;
	}

	// ...
	int Call__BOARD_OPEN();
	int Call__BOARD_CLOSE();

	int Call__SCAN_START();
	int Call__SCAN_STOP();

	int Call__DEV_INFO();	

	// ...
	bool Check__LAST_ERROR_CODE(unsigned short& err_code, CString& err_msg);
	CString Get__ERR_CODE(const unsigned short err_code);
};
