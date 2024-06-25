#include "StdAfx.h"
#include "CObj__TMC_SYS_SETUP.h"


// ...
int  CObj__TMC_SYS_SETUP
::Call__SETUP_DISABLE(CII_OBJECT__VARIABLE* p_variable)
{


	return 1;
}

int  CObj__TMC_SYS_SETUP
::Call__SETUP_ENABLE(CII_OBJECT__VARIABLE* p_variable)
{
	CString str_none = "None";
	CString str_on   = "ON";
	CString str_off  = "OFF";
	CString str_atm  = "ATM";

	//
	dEXT_CH__VAC_RB__RNE->Set__DATA(str_none);
	dEXT_CH__ATM_RB__RNE->Set__DATA(str_none);

	dEXT_CH__CHM1_VAC_SNS->Set__DATA(str_none);
	dEXT_CH__CHM1_ATM_SNS->Set__DATA(str_atm);

	//
	dEXT_CH__LBA_VAC_SNS->Set__DATA(str_none);
	dEXT_CH__LBA_ATM_SNS->Set__DATA(str_atm);

	dEXT_CH__LBB_VAC_SNS->Set__DATA(str_none);
	dEXT_CH__LBB_ATM_SNS->Set__DATA(str_atm);

	//
	dEXT_CH__PM1_VAC_SNS->Set__DATA(str_off);
	dEXT_CH__PM2_VAC_SNS->Set__DATA(str_off);
	dEXT_CH__PM4_VAC_SNS->Set__DATA(str_off);
	dEXT_CH__PM4_VAC_SNS->Set__DATA(str_off);

	dEXT_CH__PM1_ATM_SNS->Set__DATA(str_on);
	dEXT_CH__PM2_ATM_SNS->Set__DATA(str_on);
	dEXT_CH__PM3_ATM_SNS->Set__DATA(str_on);
	dEXT_CH__PM4_ATM_SNS->Set__DATA(str_on);

	//
	aEXT_CH__CHM1_PRESSURE->Set__DATA("790");
	aEXT_CH__LBA_PRESSURE->Set__DATA("790");
	aEXT_CH__LBB_PRESSURE->Set__DATA("790");

	return 1;
}
