#include "StdAfx.h"
#include "CObj__VAC_MAGATRAN.h"
#include "CObj__VAC_MAGATRAN__DEF.h"
#include "CObj__VAC_MAGATRAN__ALID.h"

#include "math.h"
#include "Macro_Function.h"
#include "CCommon_DEF.h"


//------------------------------------------------------------------------------------
int  CObj__VAC_MAGATRAN
::Is__LOCAL(CII_OBJECT__ALARM* p_alarm)
{
	return 1;
}
int  CObj__VAC_MAGATRAN
::Is__OFFLINE(CII_OBJECT__ALARM* p_alarm)
{
LOOP_RETRY:

	if(diCH__COMM_STS->Check__DATA(STR__OFFLINE) > 0)
	{
		int alarm_id = ALID__OFFLINE_ALARM_ACT;
		CString r_act;

		p_alarm->Popup__ALARM(alarm_id,r_act);

		if(r_act.CompareNoCase("RETRY") == 0)		goto LOOP_RETRY;
		else										return -1;
	}

	return 1;
}

int CObj__VAC_MAGATRAN
::Fnc__WAFER_CHECK_ON_ARM(const CString& para__arm_type, 
						  const bool active__act_pick,
						  const bool active__act_retract)
{
	if((iActive_SIM > 0)
	|| (dCH__CFG_WAFER_DRIVER_CHECK->Check__DATA(STR__YES) < 0))
	{
		if(para__arm_type.CompareNoCase(ARM_A) == 0)
		{
			if(active__act_retract)
			{
				CString str__arm_wfr = dCH__MON_ARM_A_MATERIAL_STATUS->Get__STRING();

				if(str__arm_wfr.CompareNoCase(STR__NONE) != 0)		str__arm_wfr = STR__EXIST;
				else												str__arm_wfr = STR__NONE;

				dCH__MON_ARM_A_MATERIAL_STATUS->Set__DATA(str__arm_wfr);
			}
			else
			{
				if(active__act_pick)		dCH__MON_ARM_A_MATERIAL_STATUS->Set__DATA(STR__EXIST);
				else						dCH__MON_ARM_A_MATERIAL_STATUS->Set__DATA(STR__NONE);
			}
		}
		else if(para__arm_type.CompareNoCase(ARM_B) == 0)
		{
			if(active__act_retract)
			{
				CString str__arm_wfr = dCH__MON_ARM_B_MATERIAL_STATUS->Get__STRING();

				if(str__arm_wfr.CompareNoCase(STR__NONE) != 0)		str__arm_wfr = STR__EXIST;
				else												str__arm_wfr = STR__NONE;

				dCH__MON_ARM_B_MATERIAL_STATUS->Set__DATA(str__arm_wfr);
			}
			else
			{
				if(active__act_pick)		dCH__MON_ARM_B_MATERIAL_STATUS->Set__DATA(STR__EXIST);
				else						dCH__MON_ARM_B_MATERIAL_STATUS->Set__DATA(STR__NONE);
			}
		}
	}
	else
	{
		if(para__arm_type.CompareNoCase(ARM_A) == 0)
		{
			CString str__arm_wfr = diCH__ARM_A_MAT_READ->Get__STRING();
			CString ch_data;

				 if(str__arm_wfr.CompareNoCase(STR__Absent) == 0)		ch_data = STR__NONE;
			else if(str__arm_wfr.CompareNoCase(STR__Present) == 0)		ch_data = STR__EXIST;
			else														ch_data = STR__UNKNOWN;

			dCH__MON_ARM_A_MATERIAL_STATUS->Set__DATA(ch_data);
		}
		else if(para__arm_type.CompareNoCase(ARM_B) == 0)
		{
			CString str__arm_wfr = diCH__ARM_B_MAT_READ->Get__STRING();
			CString ch_data;

				 if(str__arm_wfr.CompareNoCase(STR__Absent) == 0)		ch_data = STR__NONE;
			else if(str__arm_wfr.CompareNoCase(STR__Present) == 0)		ch_data = STR__EXIST;
			else														ch_data = STR__UNKNOWN;

			dCH__MON_ARM_B_MATERIAL_STATUS->Set__DATA(ch_data);
		}
	}

	return 1;
}

int CObj__VAC_MAGATRAN
::Fnc__calculation_CPTR_SNS_VAL(const CString& stn_name,
							    const CString& arm_type,
							    CPTR_TUNED_VAL& ro_to_tune_val)
{
	CString str_val;
	CString str_log;
	
	double caled_r_hi, caled_t_hi, caled_z_hi;
	double caled_r_lo, caled_t_lo, caled_z_lo;

	double r_hi, t_hi, z_hi;
	double r_lo, t_lo, z_lo;

	double r_tune_val, t_tune_val;

	// 1. PICK 할 때 읽어온 Data를 가져온다.
	{
		if(arm_type.CompareNoCase(ARM_A) == 0)
		{
			sCH__CPTR_ARM_A_SNS_R_RE_VALUE_HIGH->Get__DATA(str_val);	r_hi = atof(str_val);
			sCH__CPTR_ARM_A_SNS_T_RE_VALUE_HIGH->Get__DATA(str_val);	t_hi = atof(str_val);
			sCH__CPTR_ARM_A_SNS_Z_RE_VALUE_HIGH->Get__DATA(str_val);	z_hi = atof(str_val);

			// RETRACT... LOW Sns
			sCH__CPTR_ARM_A_SNS_R_RE_VALUE_LOW->Get__DATA(str_val);		r_lo = atof(str_val);
			sCH__CPTR_ARM_A_SNS_T_RE_VALUE_LOW->Get__DATA(str_val);		t_lo = atof(str_val);
			sCH__CPTR_ARM_A_SNS_Z_RE_VALUE_LOW->Get__DATA(str_val);		z_lo = atof(str_val);
		}
		else
		{
			sCH__CPTR_ARM_B_SNS_R_RE_VALUE_HIGH->Get__DATA(str_val);	r_hi = atof(str_val);
			sCH__CPTR_ARM_B_SNS_T_RE_VALUE_HIGH->Get__DATA(str_val);	t_hi = atof(str_val);
			sCH__CPTR_ARM_B_SNS_Z_RE_VALUE_HIGH->Get__DATA(str_val);	z_hi = atof(str_val);

			// RETRACT... LOW Sns
			sCH__CPTR_ARM_B_SNS_R_RE_VALUE_LOW->Get__DATA(str_val);		r_lo = atof(str_val);
			sCH__CPTR_ARM_B_SNS_T_RE_VALUE_LOW->Get__DATA(str_val);		t_lo = atof(str_val);
			sCH__CPTR_ARM_B_SNS_Z_RE_VALUE_LOW->Get__DATA(str_val);		z_lo = atof(str_val);
		}

		str_log.Format("After Picked, Result Data....   R_HI:%f, T_HI:%f, Z_HI:%f, R_LO:%f, T_LO:%f, Z_LO:%f", 
					   r_hi, t_hi, z_hi, r_lo, t_lo, z_lo);
		
		Fnc__APP_LOG(str_log);
	}

	// 2. 해당 Station의 CALIBRATION 된 R, T, Z 값들을 가져온다.
	{
		Get__CALED_CPTR_SNS_VAL(stn_name,arm_type,
								caled_r_hi, caled_t_hi, caled_z_hi,
								caled_r_lo, caled_t_lo, caled_z_lo);

		str_log.Format("Teached, Cur Stn[%s], ARM [%s], R_HI:%f, T_HI:%f, Z_HI:%f, R_LO:%f, T_LO:%f, Z_LO:%f", 
					   stn_name, arm_type, 
					   caled_r_hi, caled_t_hi, caled_z_hi, 
					   caled_r_lo, caled_t_lo, caled_z_lo);

		Fnc__APP_LOG(str_log);
	}

	// 3. PICK 후에 얻어온 R, T, Z 값과 CALIBRATION 된 R, T, Z 값들을 비교 계산한다.
	{
		Calculation_Sns_Val(caled_r_hi, caled_t_hi, caled_z_hi,
							caled_r_lo, caled_t_lo, caled_z_lo,
							r_hi, t_hi, z_hi,
							r_lo, t_lo, z_lo,
							r_tune_val, t_tune_val,
							stn_name);

		ro_to_tune_val.ro_tuned_val = r_tune_val;
		ro_to_tune_val.to_tuned_val = t_tune_val;
	}

	return 1;
}

int CObj__VAC_MAGATRAN
::Fnc__calculation_CPTR_SNS_VAL_N(const CString& stn_name,
								  const CString& arm_type,
								  CPTR_TUNED_VAL& ro_to_tune_val)
{
	CString str_val;
	CString str_log;
	double caled_r_hi, caled_t_hi, caled_z_hi;
	double caled_r_lo, caled_t_lo, caled_z_lo;

	double r_hi, t_hi, z_hi;
	double r_lo, t_lo, z_lo;

	// 1. PICK 할 때 읽어온 Data를 가져온다.
	{
		if(arm_type.CompareNoCase(ARM_A) == 0)
		{
			sCH__CPTR_ARM_A_SNS_R_RE_VALUE_HIGH->Get__DATA(str_val);	r_hi = atof(str_val);
			sCH__CPTR_ARM_A_SNS_T_RE_VALUE_HIGH->Get__DATA(str_val);	t_hi = atof(str_val);
			sCH__CPTR_ARM_A_SNS_Z_RE_VALUE_HIGH->Get__DATA(str_val);	z_hi = atof(str_val);

			// RETRACT... LOW Sns
			sCH__CPTR_ARM_A_SNS_R_RE_VALUE_LOW->Get__DATA(str_val);		r_lo = atof(str_val);
			sCH__CPTR_ARM_A_SNS_T_RE_VALUE_LOW->Get__DATA(str_val);		t_lo = atof(str_val);
			sCH__CPTR_ARM_A_SNS_Z_RE_VALUE_LOW->Get__DATA(str_val);		z_lo = atof(str_val);
		}
		else
		{
			sCH__CPTR_ARM_B_SNS_R_RE_VALUE_HIGH->Get__DATA(str_val);	r_hi = atof(str_val);
			sCH__CPTR_ARM_B_SNS_T_RE_VALUE_HIGH->Get__DATA(str_val);	t_hi = atof(str_val);
			sCH__CPTR_ARM_B_SNS_Z_RE_VALUE_HIGH->Get__DATA(str_val);	z_hi = atof(str_val);

			// RETRACT... LOW Sns
			sCH__CPTR_ARM_B_SNS_R_RE_VALUE_LOW->Get__DATA(str_val);		r_lo = atof(str_val);
			sCH__CPTR_ARM_B_SNS_T_RE_VALUE_LOW->Get__DATA(str_val);		t_lo = atof(str_val);
			sCH__CPTR_ARM_B_SNS_Z_RE_VALUE_LOW->Get__DATA(str_val);		z_lo = atof(str_val);
		}

		str_log.Format("After Picked, Result Data....   R_HI:%f, T_HI:%f, Z_HI:%f, R_LO:%f, T_LO:%f, Z_LO:%f", 
			           r_hi, t_hi, z_hi, r_lo, t_lo, z_lo);
		
		Fnc__APP_LOG(str_log);
	}

	// 2. 해당 Station의 CALIBRATION 된 R, T, Z 값들을 가져온다.
	{
		Get__CALED_CPTR_SNS_VAL(stn_name,arm_type,
								caled_r_hi, caled_t_hi, caled_z_hi,
								caled_r_lo, caled_t_lo, caled_z_lo);

		str_log.Format("Teached, Cur Stn[%s], ARM [%s], R_HI:%f, T_HI:%f, Z_HI:%f, R_LO:%f, T_LO:%f, Z_LO:%f", 
			           stn_name, arm_type, 
					   caled_r_hi, caled_t_hi, caled_z_hi, 
					   caled_r_lo, caled_t_lo, caled_z_lo);
		
		Fnc__APP_LOG(str_log);
	}

	// 3. PICK 후에 얻어온 R, T, Z 값과 CALIBRATION 된 R, T, Z 값들을 비교 계산한다.
	{
		double r_tune_val, t_tune_val;

		Calculation_Sns_Val_N(caled_r_hi, caled_t_hi, caled_z_hi,
							  caled_r_lo, caled_t_lo, caled_z_lo,
							  r_hi, t_hi, z_hi,
							  r_lo, t_lo, z_lo,
							  r_tune_val, t_tune_val,
							  stn_name);

		// Below values mean X & Y Offset, don't mean Theta Offset
		ro_to_tune_val.ro_tuned_val = r_tune_val;
		ro_to_tune_val.to_tuned_val = t_tune_val;
	}

	return OBJ_AVAILABLE;
}

int CObj__VAC_MAGATRAN
::Get__CALED_CPTR_SNS_VAL(const CString& stn_name,
						  const CString& arm_type,
						  double& r_hi, double& t_hi, double& z_hi,
						  double& r_lo, double& t_lo, double& z_lo)
{
	CString cfg_cptr_sns_val;

	if(stn_name.CompareNoCase(STR__LBA) == 0)
	{
		if(arm_type.CompareNoCase(ARM_A) == 0)
		{
			// HIGH
			aCH__OTR_IN_ARM_A_CALED_LL1_CPTR_R_HIGH_SNS_VAL->Get__DATA(cfg_cptr_sns_val);	r_hi = atof(cfg_cptr_sns_val);
			aCH__OTR_IN_ARM_A_CALED_LL1_CPTR_T_HIGH_SNS_VAL->Get__DATA(cfg_cptr_sns_val);	t_hi = atof(cfg_cptr_sns_val);
			aCH__OTR_IN_ARM_A_CALED_LL1_CPTR_Z_HIGH_SNS_VAL->Get__DATA(cfg_cptr_sns_val);	z_hi = atof(cfg_cptr_sns_val);

			// LOW
			aCH__OTR_IN_ARM_A_CALED_LL1_CPTR_R_LOW_SNS_VAL->Get__DATA(cfg_cptr_sns_val);	r_lo = atof(cfg_cptr_sns_val);
			aCH__OTR_IN_ARM_A_CALED_LL1_CPTR_T_LOW_SNS_VAL->Get__DATA(cfg_cptr_sns_val);	t_lo = atof(cfg_cptr_sns_val);
			aCH__OTR_IN_ARM_A_CALED_LL1_CPTR_Z_LOW_SNS_VAL->Get__DATA(cfg_cptr_sns_val);	z_lo = atof(cfg_cptr_sns_val);
		}
		else
		{
			// HIGH
			aCH__OTR_IN_ARM_B_CALED_LL1_CPTR_R_HIGH_SNS_VAL->Get__DATA(cfg_cptr_sns_val);	r_hi = atof(cfg_cptr_sns_val);
			aCH__OTR_IN_ARM_B_CALED_LL1_CPTR_T_HIGH_SNS_VAL->Get__DATA(cfg_cptr_sns_val);	t_hi = atof(cfg_cptr_sns_val);
			aCH__OTR_IN_ARM_B_CALED_LL1_CPTR_Z_HIGH_SNS_VAL->Get__DATA(cfg_cptr_sns_val);	z_hi = atof(cfg_cptr_sns_val);

			// LOW
			aCH__OTR_IN_ARM_B_CALED_LL1_CPTR_R_LOW_SNS_VAL->Get__DATA(cfg_cptr_sns_val);	r_lo = atof(cfg_cptr_sns_val);
			aCH__OTR_IN_ARM_B_CALED_LL1_CPTR_T_LOW_SNS_VAL->Get__DATA(cfg_cptr_sns_val);	t_lo = atof(cfg_cptr_sns_val);
			aCH__OTR_IN_ARM_B_CALED_LL1_CPTR_Z_LOW_SNS_VAL->Get__DATA(cfg_cptr_sns_val);	z_lo = atof(cfg_cptr_sns_val);
		}
	}
	else if(stn_name.CompareNoCase(STR__LBB) == 0)
	{
		if(arm_type.CompareNoCase(ARM_A) == 0)
		{
			// HIGH
			aCH__OTR_IN_ARM_A_CALED_LL2_CPTR_R_HIGH_SNS_VAL->Get__DATA(cfg_cptr_sns_val);	r_hi = atof(cfg_cptr_sns_val);
			aCH__OTR_IN_ARM_A_CALED_LL2_CPTR_T_HIGH_SNS_VAL->Get__DATA(cfg_cptr_sns_val);	t_hi = atof(cfg_cptr_sns_val);
			aCH__OTR_IN_ARM_A_CALED_LL2_CPTR_Z_HIGH_SNS_VAL->Get__DATA(cfg_cptr_sns_val);	z_hi = atof(cfg_cptr_sns_val);

			// LOW
			aCH__OTR_IN_ARM_A_CALED_LL2_CPTR_R_LOW_SNS_VAL->Get__DATA(cfg_cptr_sns_val);	r_lo = atof(cfg_cptr_sns_val);
			aCH__OTR_IN_ARM_A_CALED_LL2_CPTR_T_LOW_SNS_VAL->Get__DATA(cfg_cptr_sns_val);	t_lo = atof(cfg_cptr_sns_val);
			aCH__OTR_IN_ARM_A_CALED_LL2_CPTR_Z_LOW_SNS_VAL->Get__DATA(cfg_cptr_sns_val);	z_lo = atof(cfg_cptr_sns_val);
		}
		else
		{
			// HIGH
			aCH__OTR_IN_ARM_B_CALED_LL2_CPTR_R_HIGH_SNS_VAL->Get__DATA(cfg_cptr_sns_val);	r_hi = atof(cfg_cptr_sns_val);
			aCH__OTR_IN_ARM_B_CALED_LL2_CPTR_T_HIGH_SNS_VAL->Get__DATA(cfg_cptr_sns_val);	t_hi = atof(cfg_cptr_sns_val);
			aCH__OTR_IN_ARM_B_CALED_LL2_CPTR_Z_HIGH_SNS_VAL->Get__DATA(cfg_cptr_sns_val);	z_hi = atof(cfg_cptr_sns_val);

			// LOW
			aCH__OTR_IN_ARM_B_CALED_LL2_CPTR_R_LOW_SNS_VAL->Get__DATA(cfg_cptr_sns_val);	r_lo = atof(cfg_cptr_sns_val);
			aCH__OTR_IN_ARM_B_CALED_LL2_CPTR_T_LOW_SNS_VAL->Get__DATA(cfg_cptr_sns_val);	t_lo = atof(cfg_cptr_sns_val);
			aCH__OTR_IN_ARM_B_CALED_LL2_CPTR_Z_LOW_SNS_VAL->Get__DATA(cfg_cptr_sns_val);	z_lo = atof(cfg_cptr_sns_val);
		}
	}
	else
	{
		int pm_index = Macro__CHECK_PMx_INDEX(stn_name);

		if((pm_index >= 0)
		&& (pm_index <  CFG_PMx__SIZE))
		{
			if(arm_type.CompareNoCase(ARM_A) == 0)
			{
				// HIGH
				aCH__OTR_IN_ARM_A_CALED_PMx_CPTR_R_HIGH_SNS_VAL[pm_index]->Get__DATA(cfg_cptr_sns_val);	r_hi = atof(cfg_cptr_sns_val);
				aCH__OTR_IN_ARM_A_CALED_PMx_CPTR_T_HIGH_SNS_VAL[pm_index]->Get__DATA(cfg_cptr_sns_val);	t_hi = atof(cfg_cptr_sns_val);
				aCH__OTR_IN_ARM_A_CALED_PMx_CPTR_Z_HIGH_SNS_VAL[pm_index]->Get__DATA(cfg_cptr_sns_val);	z_hi = atof(cfg_cptr_sns_val);

				// LOW
				aCH__OTR_IN_ARM_A_CALED_PMx_CPTR_R_LOW_SNS_VAL[pm_index]->Get__DATA(cfg_cptr_sns_val);	r_lo = atof(cfg_cptr_sns_val);
				aCH__OTR_IN_ARM_A_CALED_PMx_CPTR_T_LOW_SNS_VAL[pm_index]->Get__DATA(cfg_cptr_sns_val);	t_lo = atof(cfg_cptr_sns_val);
				aCH__OTR_IN_ARM_A_CALED_PMx_CPTR_Z_LOW_SNS_VAL[pm_index]->Get__DATA(cfg_cptr_sns_val);	z_lo = atof(cfg_cptr_sns_val);
			}
			else
			{
				// HIGH
				aCH__OTR_IN_ARM_B_CALED_PMx_CPTR_R_HIGH_SNS_VAL[pm_index]->Get__DATA(cfg_cptr_sns_val);	r_hi = atof(cfg_cptr_sns_val);
				aCH__OTR_IN_ARM_B_CALED_PMx_CPTR_T_HIGH_SNS_VAL[pm_index]->Get__DATA(cfg_cptr_sns_val);	t_hi = atof(cfg_cptr_sns_val);
				aCH__OTR_IN_ARM_B_CALED_PMx_CPTR_Z_HIGH_SNS_VAL[pm_index]->Get__DATA(cfg_cptr_sns_val);	z_hi = atof(cfg_cptr_sns_val);

				// LOW
				aCH__OTR_IN_ARM_B_CALED_PMx_CPTR_R_LOW_SNS_VAL[pm_index]->Get__DATA(cfg_cptr_sns_val);	r_lo = atof(cfg_cptr_sns_val);
				aCH__OTR_IN_ARM_B_CALED_PMx_CPTR_T_LOW_SNS_VAL[pm_index]->Get__DATA(cfg_cptr_sns_val);	t_lo = atof(cfg_cptr_sns_val);
				aCH__OTR_IN_ARM_B_CALED_PMx_CPTR_Z_LOW_SNS_VAL[pm_index]->Get__DATA(cfg_cptr_sns_val);	z_lo = atof(cfg_cptr_sns_val);
			}
		}
	}

	return 1;
}

int CObj__VAC_MAGATRAN
::Init__CALED_CPTR_SNS_VAL_CH(const CString& stn_name, const CString& arm_type)
{
	CString cfg_cptr_sns_val = "UNKNOWN";

	if(stn_name.CompareNoCase(STR__LBA) == 0)
	{
		if(arm_type.CompareNoCase(ARM_A) == 0)
		{
			// HIGH
			aCH__OTR_IN_ARM_A_CALED_LL1_CPTR_R_HIGH_SNS_VAL->Set__DATA(cfg_cptr_sns_val);	
			aCH__OTR_IN_ARM_A_CALED_LL1_CPTR_T_HIGH_SNS_VAL->Set__DATA(cfg_cptr_sns_val);
			aCH__OTR_IN_ARM_A_CALED_LL1_CPTR_Z_HIGH_SNS_VAL->Set__DATA(cfg_cptr_sns_val);

			// LOW
			aCH__OTR_IN_ARM_A_CALED_LL1_CPTR_R_LOW_SNS_VAL->Set__DATA(cfg_cptr_sns_val);
			aCH__OTR_IN_ARM_A_CALED_LL1_CPTR_T_LOW_SNS_VAL->Set__DATA(cfg_cptr_sns_val);
			aCH__OTR_IN_ARM_A_CALED_LL1_CPTR_Z_LOW_SNS_VAL->Set__DATA(cfg_cptr_sns_val);
		}
		else
		{
			// HIGH
			aCH__OTR_IN_ARM_B_CALED_LL1_CPTR_R_HIGH_SNS_VAL->Set__DATA(cfg_cptr_sns_val);
			aCH__OTR_IN_ARM_B_CALED_LL1_CPTR_T_HIGH_SNS_VAL->Set__DATA(cfg_cptr_sns_val);
			aCH__OTR_IN_ARM_B_CALED_LL1_CPTR_Z_HIGH_SNS_VAL->Set__DATA(cfg_cptr_sns_val);

			// LOW
			aCH__OTR_IN_ARM_B_CALED_LL1_CPTR_R_LOW_SNS_VAL->Set__DATA(cfg_cptr_sns_val);
			aCH__OTR_IN_ARM_B_CALED_LL1_CPTR_T_LOW_SNS_VAL->Set__DATA(cfg_cptr_sns_val);
			aCH__OTR_IN_ARM_B_CALED_LL1_CPTR_Z_LOW_SNS_VAL->Set__DATA(cfg_cptr_sns_val);
		}
	}
	else if(stn_name.CompareNoCase(STR__LBB) == 0)
	{
		if(arm_type.CompareNoCase(ARM_A) == 0)
		{
			// HIGH
			aCH__OTR_IN_ARM_A_CALED_LL2_CPTR_R_HIGH_SNS_VAL->Set__DATA(cfg_cptr_sns_val);
			aCH__OTR_IN_ARM_A_CALED_LL2_CPTR_T_HIGH_SNS_VAL->Set__DATA(cfg_cptr_sns_val);
			aCH__OTR_IN_ARM_A_CALED_LL2_CPTR_Z_HIGH_SNS_VAL->Set__DATA(cfg_cptr_sns_val);

			// LOW
			aCH__OTR_IN_ARM_A_CALED_LL2_CPTR_R_LOW_SNS_VAL->Set__DATA(cfg_cptr_sns_val);
			aCH__OTR_IN_ARM_A_CALED_LL2_CPTR_T_LOW_SNS_VAL->Set__DATA(cfg_cptr_sns_val);
			aCH__OTR_IN_ARM_A_CALED_LL2_CPTR_Z_LOW_SNS_VAL->Set__DATA(cfg_cptr_sns_val);
		}
		else
		{
			// HIGH
			aCH__OTR_IN_ARM_B_CALED_LL2_CPTR_R_HIGH_SNS_VAL->Set__DATA(cfg_cptr_sns_val);
			aCH__OTR_IN_ARM_B_CALED_LL2_CPTR_T_HIGH_SNS_VAL->Set__DATA(cfg_cptr_sns_val);
			aCH__OTR_IN_ARM_B_CALED_LL2_CPTR_Z_HIGH_SNS_VAL->Set__DATA(cfg_cptr_sns_val);

			// LOW
			aCH__OTR_IN_ARM_B_CALED_LL2_CPTR_R_LOW_SNS_VAL->Set__DATA(cfg_cptr_sns_val);
			aCH__OTR_IN_ARM_B_CALED_LL2_CPTR_T_LOW_SNS_VAL->Set__DATA(cfg_cptr_sns_val);
			aCH__OTR_IN_ARM_B_CALED_LL2_CPTR_Z_LOW_SNS_VAL->Set__DATA(cfg_cptr_sns_val);
		}
	}
	else
	{
		int pm_index = Macro__CHECK_PMx_INDEX(stn_name);

		if((pm_index >= 0)
		&& (pm_index <  CFG_PMx__SIZE))
		{
			if(arm_type.CompareNoCase(ARM_A) == 0)
			{
				// HIGH
				aCH__OTR_IN_ARM_A_CALED_PMx_CPTR_R_HIGH_SNS_VAL[pm_index]->Set__DATA(cfg_cptr_sns_val);
				aCH__OTR_IN_ARM_A_CALED_PMx_CPTR_T_HIGH_SNS_VAL[pm_index]->Set__DATA(cfg_cptr_sns_val);
				aCH__OTR_IN_ARM_A_CALED_PMx_CPTR_Z_HIGH_SNS_VAL[pm_index]->Set__DATA(cfg_cptr_sns_val);

				// LOW
				aCH__OTR_IN_ARM_A_CALED_PMx_CPTR_R_LOW_SNS_VAL[pm_index]->Set__DATA(cfg_cptr_sns_val);
				aCH__OTR_IN_ARM_A_CALED_PMx_CPTR_T_LOW_SNS_VAL[pm_index]->Set__DATA(cfg_cptr_sns_val);
				aCH__OTR_IN_ARM_A_CALED_PMx_CPTR_Z_LOW_SNS_VAL[pm_index]->Set__DATA(cfg_cptr_sns_val);
			}
			else
			{
				// HIGH
				aCH__OTR_IN_ARM_B_CALED_PMx_CPTR_R_HIGH_SNS_VAL[pm_index]->Set__DATA(cfg_cptr_sns_val);
				aCH__OTR_IN_ARM_B_CALED_PMx_CPTR_T_HIGH_SNS_VAL[pm_index]->Set__DATA(cfg_cptr_sns_val);
				aCH__OTR_IN_ARM_B_CALED_PMx_CPTR_Z_HIGH_SNS_VAL[pm_index]->Set__DATA(cfg_cptr_sns_val);

				// LOW
				aCH__OTR_IN_ARM_B_CALED_PMx_CPTR_R_LOW_SNS_VAL[pm_index]->Set__DATA(cfg_cptr_sns_val);
				aCH__OTR_IN_ARM_B_CALED_PMx_CPTR_T_LOW_SNS_VAL[pm_index]->Set__DATA(cfg_cptr_sns_val);
				aCH__OTR_IN_ARM_B_CALED_PMx_CPTR_Z_LOW_SNS_VAL[pm_index]->Set__DATA(cfg_cptr_sns_val);
			}
		}
	}

	return 1;
}

int CObj__VAC_MAGATRAN
::Save__CALED_CPTR_SNS_VAL(const CString& stn_name,
						   const CString& arm_type,
						   double& r_hi, double& t_hi, double& z_hi,
						   double& r_lo, double& t_lo, double& z_lo)
{
	CString cfg_cptr_sns_val;

	if(stn_name.CompareNoCase(STR__LBA) == 0)
	{
		if(arm_type.CompareNoCase(ARM_A) == 0)
		{
			// HIGH
			cfg_cptr_sns_val.Format("%.f", r_hi); aCH__OTR_IN_ARM_A_CALED_LL1_CPTR_R_HIGH_SNS_VAL->Set__DATA(cfg_cptr_sns_val);	
			cfg_cptr_sns_val.Format("%.f", t_hi); aCH__OTR_IN_ARM_A_CALED_LL1_CPTR_T_HIGH_SNS_VAL->Set__DATA(cfg_cptr_sns_val);
			cfg_cptr_sns_val.Format("%.f", z_hi); aCH__OTR_IN_ARM_A_CALED_LL1_CPTR_Z_HIGH_SNS_VAL->Set__DATA(cfg_cptr_sns_val);

			// LOW
			cfg_cptr_sns_val.Format("%.f", r_lo); aCH__OTR_IN_ARM_A_CALED_LL1_CPTR_R_LOW_SNS_VAL->Set__DATA(cfg_cptr_sns_val);
			cfg_cptr_sns_val.Format("%.f", t_lo); aCH__OTR_IN_ARM_A_CALED_LL1_CPTR_T_LOW_SNS_VAL->Set__DATA(cfg_cptr_sns_val);
			cfg_cptr_sns_val.Format("%.f", z_lo); aCH__OTR_IN_ARM_A_CALED_LL1_CPTR_Z_LOW_SNS_VAL->Set__DATA(cfg_cptr_sns_val);
		}
		else
		{
			// HIGH
			cfg_cptr_sns_val.Format("%.f", r_hi); aCH__OTR_IN_ARM_B_CALED_LL1_CPTR_R_HIGH_SNS_VAL->Set__DATA(cfg_cptr_sns_val);
			cfg_cptr_sns_val.Format("%.f", t_hi); aCH__OTR_IN_ARM_B_CALED_LL1_CPTR_T_HIGH_SNS_VAL->Set__DATA(cfg_cptr_sns_val);
			cfg_cptr_sns_val.Format("%.f", z_hi); aCH__OTR_IN_ARM_B_CALED_LL1_CPTR_Z_HIGH_SNS_VAL->Set__DATA(cfg_cptr_sns_val);

			// LOW
			cfg_cptr_sns_val.Format("%.f", r_lo); aCH__OTR_IN_ARM_B_CALED_LL1_CPTR_R_LOW_SNS_VAL->Set__DATA(cfg_cptr_sns_val);
			cfg_cptr_sns_val.Format("%.f", t_lo); aCH__OTR_IN_ARM_B_CALED_LL1_CPTR_T_LOW_SNS_VAL->Set__DATA(cfg_cptr_sns_val);
			cfg_cptr_sns_val.Format("%.f", z_lo); aCH__OTR_IN_ARM_B_CALED_LL1_CPTR_Z_LOW_SNS_VAL->Set__DATA(cfg_cptr_sns_val);
		}
	}
	else if(stn_name.CompareNoCase(STR__LBB) == 0)
	{
		if(arm_type.CompareNoCase(ARM_A) == 0)
		{
			// HIGH
			cfg_cptr_sns_val.Format("%.f", r_hi); aCH__OTR_IN_ARM_A_CALED_LL2_CPTR_R_HIGH_SNS_VAL->Set__DATA(cfg_cptr_sns_val);
			cfg_cptr_sns_val.Format("%.f", t_hi); aCH__OTR_IN_ARM_A_CALED_LL2_CPTR_T_HIGH_SNS_VAL->Set__DATA(cfg_cptr_sns_val);
			cfg_cptr_sns_val.Format("%.f", z_hi); aCH__OTR_IN_ARM_A_CALED_LL2_CPTR_Z_HIGH_SNS_VAL->Set__DATA(cfg_cptr_sns_val);

			// LOW
			cfg_cptr_sns_val.Format("%.f", r_lo); aCH__OTR_IN_ARM_A_CALED_LL2_CPTR_R_LOW_SNS_VAL->Set__DATA(cfg_cptr_sns_val);
			cfg_cptr_sns_val.Format("%.f", t_lo); aCH__OTR_IN_ARM_A_CALED_LL2_CPTR_T_LOW_SNS_VAL->Set__DATA(cfg_cptr_sns_val);
			cfg_cptr_sns_val.Format("%.f", z_lo); aCH__OTR_IN_ARM_A_CALED_LL2_CPTR_Z_LOW_SNS_VAL->Set__DATA(cfg_cptr_sns_val);
		}
		else
		{
			// HIGH
			cfg_cptr_sns_val.Format("%.f", r_hi); aCH__OTR_IN_ARM_B_CALED_LL2_CPTR_R_HIGH_SNS_VAL->Set__DATA(cfg_cptr_sns_val);
			cfg_cptr_sns_val.Format("%.f", t_hi); aCH__OTR_IN_ARM_B_CALED_LL2_CPTR_T_HIGH_SNS_VAL->Set__DATA(cfg_cptr_sns_val);
			cfg_cptr_sns_val.Format("%.f", z_hi); aCH__OTR_IN_ARM_B_CALED_LL2_CPTR_Z_HIGH_SNS_VAL->Set__DATA(cfg_cptr_sns_val);

			// LOW
			cfg_cptr_sns_val.Format("%.f", r_lo); aCH__OTR_IN_ARM_B_CALED_LL2_CPTR_R_LOW_SNS_VAL->Set__DATA(cfg_cptr_sns_val);
			cfg_cptr_sns_val.Format("%.f", t_lo); aCH__OTR_IN_ARM_B_CALED_LL2_CPTR_T_LOW_SNS_VAL->Set__DATA(cfg_cptr_sns_val);
			cfg_cptr_sns_val.Format("%.f", z_lo); aCH__OTR_IN_ARM_B_CALED_LL2_CPTR_Z_LOW_SNS_VAL->Set__DATA(cfg_cptr_sns_val);
		}
	}
	else
	{
		int pm_index = Macro__CHECK_PMx_INDEX(stn_name);

		if((pm_index >= 0)
		&& (pm_index <  CFG_PMx__SIZE))
		{
			if(arm_type.CompareNoCase(ARM_A) == 0)
			{
				// HIGH
				cfg_cptr_sns_val.Format("%.f", r_hi); aCH__OTR_IN_ARM_A_CALED_PMx_CPTR_R_HIGH_SNS_VAL[pm_index]->Set__DATA(cfg_cptr_sns_val);
				cfg_cptr_sns_val.Format("%.f", t_hi); aCH__OTR_IN_ARM_A_CALED_PMx_CPTR_T_HIGH_SNS_VAL[pm_index]->Set__DATA(cfg_cptr_sns_val);
				cfg_cptr_sns_val.Format("%.f", z_hi); aCH__OTR_IN_ARM_A_CALED_PMx_CPTR_Z_HIGH_SNS_VAL[pm_index]->Set__DATA(cfg_cptr_sns_val);

				// LOW
				cfg_cptr_sns_val.Format("%.f", r_lo); aCH__OTR_IN_ARM_A_CALED_PMx_CPTR_R_LOW_SNS_VAL[pm_index]->Set__DATA(cfg_cptr_sns_val);
				cfg_cptr_sns_val.Format("%.f", t_lo); aCH__OTR_IN_ARM_A_CALED_PMx_CPTR_T_LOW_SNS_VAL[pm_index]->Set__DATA(cfg_cptr_sns_val);
				cfg_cptr_sns_val.Format("%.f", z_lo); aCH__OTR_IN_ARM_A_CALED_PMx_CPTR_Z_LOW_SNS_VAL[pm_index]->Set__DATA(cfg_cptr_sns_val);
			}
			else
			{
				// HIGH
				cfg_cptr_sns_val.Format("%.f", r_hi); aCH__OTR_IN_ARM_B_CALED_PMx_CPTR_R_HIGH_SNS_VAL[pm_index]->Set__DATA(cfg_cptr_sns_val);
				cfg_cptr_sns_val.Format("%.f", t_hi); aCH__OTR_IN_ARM_B_CALED_PMx_CPTR_T_HIGH_SNS_VAL[pm_index]->Set__DATA(cfg_cptr_sns_val);
				cfg_cptr_sns_val.Format("%.f", z_hi); aCH__OTR_IN_ARM_B_CALED_PMx_CPTR_Z_HIGH_SNS_VAL[pm_index]->Set__DATA(cfg_cptr_sns_val);

				// LOW
				cfg_cptr_sns_val.Format("%.f", r_lo); aCH__OTR_IN_ARM_B_CALED_PMx_CPTR_R_LOW_SNS_VAL[pm_index]->Set__DATA(cfg_cptr_sns_val);
				cfg_cptr_sns_val.Format("%.f", t_lo); aCH__OTR_IN_ARM_B_CALED_PMx_CPTR_T_LOW_SNS_VAL[pm_index]->Set__DATA(cfg_cptr_sns_val);
				cfg_cptr_sns_val.Format("%.f", z_lo); aCH__OTR_IN_ARM_B_CALED_PMx_CPTR_Z_LOW_SNS_VAL[pm_index]->Set__DATA(cfg_cptr_sns_val);
			}
		}
	}

	return 1;
}

CString  CObj__VAC_MAGATRAN
::Get__CFG_STN_NUM(const CString& stn_name)
{
	CString cfg_stn_num;

	     if(stn_name.CompareNoCase(STR__LBA) == 0)		aCH__OTR_IN_CFG__LBA_STN_NUM->Get__DATA(cfg_stn_num);
	else if(stn_name.CompareNoCase(STR__LBB) == 0)		aCH__OTR_IN_CFG__LBB_STN_NUM->Get__DATA(cfg_stn_num);

	// ...
	int pm_index = Macro__CHECK_PMx_INDEX(stn_name);

	if((pm_index >= 0)
	&& (pm_index <  CFG_PMx__SIZE))
	{
		aCH__OTR_IN_CFG__PMx_STN_NUM[pm_index]->Get__DATA(cfg_stn_num);
	}

	return cfg_stn_num;
}

CString  CObj__VAC_MAGATRAN
::Get__CFG_DYNAMIC_ALGN_USE(const CString& stn_name)
{
	CString cfg_val;

	     if(stn_name.CompareNoCase(STR__LBA) == 0)		dCH__CFG_LL1_DYNAMIC_ALIGN_USE->Get__DATA(cfg_val);
	else if(stn_name.CompareNoCase(STR__LBB) == 0)		dCH__CFG_LL2_DYNAMIC_ALIGN_USE->Get__DATA(cfg_val);

	// ...
	int pm_index = Macro__CHECK_PMx_INDEX(stn_name);

	if((pm_index >= 0)
	&& (pm_index <  CFG_PMx__SIZE))
	{
		dCH__CFG_PMx_DYNAMIC_ALIGN_USE[pm_index]->Get__DATA(cfg_val);
	}

	return cfg_val;
}

CString  CObj__VAC_MAGATRAN
::Get__CFG_STN_CPTR_NUM(const CString& stn_name)
{
	CString cfg_val;

	     if(stn_name.CompareNoCase(STR__LBA) == 0)		aCH__OTR_IN_CFG_LL1_CPTR_SNS_NUM->Get__DATA(cfg_val);
	else if(stn_name.CompareNoCase(STR__LBB) == 0)		aCH__OTR_IN_CFG_LL2_CPTR_SNS_NUM->Get__DATA(cfg_val);

	// ...
	int pm_index = Macro__CHECK_PMx_INDEX(stn_name);

	if((pm_index >= 0)
	&& (pm_index <  CFG_PMx__SIZE))
	{
		aCH__OTR_IN_CFG_PMx_CPTR_SNS_NUM[pm_index]->Get__DATA(cfg_val);
	}

	return cfg_val;
}

int  CObj__VAC_MAGATRAN
::Get__CFG_STN_OFFSET_RANGE_RET(const CString& stn_name, 
							    CString& str_r_soft_tol, CString& str_r_hard_tol,
							    CString& str_t_soft_tol, CString& str_t_hard_tol)
{
	if(stn_name.CompareNoCase(STR__LBA) == 0)
	{
		aCH__OTR_IN_CFG__R_OFFSET_SOFT_TOLERANCE_LBA_RET->Get__DATA(str_r_soft_tol);
		aCH__OTR_IN_CFG__R_OFFSET_HARD_TOLERANCE_LBA_RET->Get__DATA(str_r_hard_tol);

		aCH__OTR_IN_CFG__T_OFFSET_SOFT_TOLERANCE_LBA_RET->Get__DATA(str_t_soft_tol);
		aCH__OTR_IN_CFG__T_OFFSET_HARD_TOLERANCE_LBA_RET->Get__DATA(str_t_hard_tol);
	}
	else if(stn_name.CompareNoCase(STR__LBB) == 0)
	{
		aCH__OTR_IN_CFG__R_OFFSET_SOFT_TOLERANCE_LBB_RET->Get__DATA(str_r_soft_tol);
		aCH__OTR_IN_CFG__R_OFFSET_HARD_TOLERANCE_LBB_RET->Get__DATA(str_r_hard_tol);

		aCH__OTR_IN_CFG__T_OFFSET_SOFT_TOLERANCE_LBB_RET->Get__DATA(str_t_soft_tol);
		aCH__OTR_IN_CFG__T_OFFSET_HARD_TOLERANCE_LBB_RET->Get__DATA(str_t_hard_tol);
	}

	// ...
	int pm_index = Macro__CHECK_PMx_INDEX(stn_name);

	if((pm_index >= 0)
	&& (pm_index <  CFG_PMx__SIZE))
	{
		aCH__OTR_IN_CFG__R_OFFSET_SOFT_TOLERANCE_RET[pm_index]->Get__DATA(str_r_soft_tol);
		aCH__OTR_IN_CFG__R_OFFSET_HARD_TOLERANCE_RET[pm_index]->Get__DATA(str_r_hard_tol);

		aCH__OTR_IN_CFG__T_OFFSET_SOFT_TOLERANCE_RET[pm_index]->Get__DATA(str_t_soft_tol);
		aCH__OTR_IN_CFG__T_OFFSET_HARD_TOLERANCE_RET[pm_index]->Get__DATA(str_t_hard_tol);
	}

	return 1;
}

int  CObj__VAC_MAGATRAN
::Is__ERROR_EXIST(CII_OBJECT__ALARM* p_alarm)
{
	CString data;
	CString title;
	CString msg;
	CString r_act;

	int error;
	int result;
	int	alarm_id;

	// Read ErrorID
	{
		sCH__ERRID->Get__DATA(data);
		error = atoi(data);

		msg.Format("APP_GET_ERR_ID : [%d]", error);
		Fnc__APP_LOG(msg);
	}

	// Switch Error List
	switch(error)
	{
		case 0:
			result = OBJ_AVAILABLE;
			break;

			// Success Code
		case 1:
			alarm_id = ALID__VACRB1_ERR_00001;
			break;

		case 210:
			alarm_id = ALID__VACRB1_ERR_00210;
			break;

		case 220:
			alarm_id = ALID__VACRB1_ERR_00220;
			break;

		case 221:
			alarm_id = ALID__VACRB1_ERR_00221;
			break;

		case 233:
			alarm_id = ALID__VACRB1_ERR_00233;
			break;

		case 234:
			alarm_id = ALID__VACRB1_ERR_00234;
			break;

		case 301:
			alarm_id = ALID__VACRB1_ERR_00301;
			break;

		case 305:
			alarm_id = ALID__VACRB1_ERR_00305;
			break;

		case 306:
			alarm_id = ALID__VACRB1_ERR_00306;
			break;

		case 309:
			alarm_id = ALID__VACRB1_ERR_00309;
			break;

		case 350:
			alarm_id = ALID__VACRB1_ERR_00350;
			break;

		case 351:
			alarm_id = ALID__VACRB1_ERR_00351;
			break;

		case 352:
			alarm_id = ALID__VACRB1_ERR_00352;
			break;

		case 353:
			alarm_id = ALID__VACRB1_ERR_00353;
			break;

		case 390:
			alarm_id = ALID__VACRB1_ERR_00390;
			break;

		case 402:
			alarm_id = ALID__VACRB1_ERR_00402;
			break;

		case 405:
			alarm_id = ALID__VACRB1_ERR_00405;
			break;

		case 406:
			alarm_id = ALID__VACRB1_ERR_00406;
			break;

		case 407:
			alarm_id = ALID__VACRB1_ERR_00407;
			break;

		case 408:
			alarm_id = ALID__VACRB1_ERR_00408;
			break;

		case 409:
			alarm_id = ALID__VACRB1_ERR_00409;
			break;

		case 414:
			alarm_id = ALID__VACRB1_ERR_00414;
			break;

		case 415:
			alarm_id = ALID__VACRB1_ERR_00415;
			break;

		case 416:
			alarm_id = ALID__VACRB1_ERR_00416;
			break;

		case 417:
			alarm_id = ALID__VACRB1_ERR_00417;
			break;

		case 418:
			alarm_id = ALID__VACRB1_ERR_00418;
			break;

		case 508:
			alarm_id = ALID__VACRB1_ERR_00508;
			break;

		case 509:
			alarm_id = ALID__VACRB1_ERR_00509;
			break;

		case 527:
			alarm_id = ALID__VACRB1_ERR_00527;
			break;

		case 528:
			alarm_id = ALID__VACRB1_ERR_00528;
			break;

		case 550:
			alarm_id = ALID__VACRB1_ERR_00550;
			break;

		case 551:
			alarm_id = ALID__VACRB1_ERR_00551;
			break;

		case 552:
			alarm_id = ALID__VACRB1_ERR_00552;
			break;

		case 554:
			alarm_id = ALID__VACRB1_ERR_00554;
			break;

		case 555:
			alarm_id = ALID__VACRB1_ERR_00555;
			break;

		case 557:
			alarm_id = ALID__VACRB1_ERR_00557;
			break;

		case 558:
			alarm_id = ALID__VACRB1_ERR_00558;
			break;

		case 602:
			alarm_id = ALID__VACRB1_ERR_00602;
			break;

		case 603:
			alarm_id = ALID__VACRB1_ERR_00603;
			break;

		case 604:
			alarm_id = ALID__VACRB1_ERR_00604;
			break;

		case 605:
			alarm_id = ALID__VACRB1_ERR_00605;
			break;

		case 606:
			alarm_id = ALID__VACRB1_ERR_00606;
			break;

		case 607:
			alarm_id = ALID__VACRB1_ERR_00607;
			break;

		case 608:
			alarm_id = ALID__VACRB1_ERR_00608;
			break;

		case 610:
			alarm_id = ALID__VACRB1_ERR_00610;
			break;

		case 611:
			alarm_id = ALID__VACRB1_ERR_00611;
			break;

		case 612:
			alarm_id = ALID__VACRB1_ERR_00612;
			break;

		case 613:
			alarm_id = ALID__VACRB1_ERR_00613;
			break;

		case 652:
			alarm_id = ALID__VACRB1_ERR_00652;
			break;		

		case 653:
			alarm_id = ALID__VACRB1_ERR_00653;
			break;

		case 654:
			alarm_id = ALID__VACRB1_ERR_00654;
			break;

		case 655:
			alarm_id = ALID__VACRB1_ERR_00655;
			break;

		case 656:
			alarm_id = ALID__VACRB1_ERR_00656;
			break;

		case 700:
			alarm_id = ALID__VACRB1_ERR_00700;
			break;

		case 701:
			alarm_id = ALID__VACRB1_ERR_00701;
			break;

		case 705:
			alarm_id = ALID__VACRB1_ERR_00705;
			break;

		case 706:
			alarm_id = ALID__VACRB1_ERR_00706;
			break;

		case 710:
			alarm_id = ALID__VACRB1_ERR_00710;
			break;

		case 711:
			alarm_id = ALID__VACRB1_ERR_00711;
			break;

		case 715:
			alarm_id = ALID__VACRB1_ERR_00715;
			break;

		case 722:
			alarm_id = ALID__VACRB1_ERR_00722;
			break;		

		case 721:
			alarm_id = ALID__VACRB1_ERR_00721;
			break;

		case 730:
			alarm_id = ALID__VACRB1_ERR_00730;
			break;

		case 731:
			alarm_id = ALID__VACRB1_ERR_00731;
			break;

		case 732:
			alarm_id = ALID__VACRB1_ERR_00732;
			break;

		case 733:
			alarm_id = ALID__VACRB1_ERR_00733;
			break;

		case 734:
			alarm_id = ALID__VACRB1_ERR_00734;
			break;

		case 735:
			alarm_id = ALID__VACRB1_ERR_00735;
			break;

		case 736:
			alarm_id = ALID__VACRB1_ERR_00736;
			break;

		case 738:
			alarm_id = ALID__VACRB1_ERR_00738;
			break;

		case 739:
			alarm_id = ALID__VACRB1_ERR_00739;
			break;

		case 740:
			alarm_id = ALID__VACRB1_ERR_00740;
			break;

		case 741:
			alarm_id = ALID__VACRB1_ERR_00741;
			break;

		case 742:
			alarm_id = ALID__VACRB1_ERR_00742;
			break;

		case 743:
			alarm_id = ALID__VACRB1_ERR_00743;
			break;

		case 745:
			alarm_id = ALID__VACRB1_ERR_00745;
			break;

		case 749:
			alarm_id = ALID__VACRB1_ERR_00749;
			break;

		case 750:
			alarm_id = ALID__VACRB1_ERR_00750;
			break;

		case 751:
			alarm_id = ALID__VACRB1_ERR_00751;
			break;

		case 800:
			alarm_id = ALID__VACRB1_ERR_00800;
			break;

		case 801:
			alarm_id = ALID__VACRB1_ERR_00801;
			break;

		case 802:
			alarm_id = ALID__VACRB1_ERR_00802;
			break;

		case 803:
			alarm_id = ALID__VACRB1_ERR_00803;
			break;

		case 804:
			alarm_id = ALID__VACRB1_ERR_00804;
			break;

		case 805:
			alarm_id = ALID__VACRB1_ERR_00805;
			break;

		case 810:
			alarm_id = ALID__VACRB1_ERR_00810;
			break;

		case 811:
			alarm_id = ALID__VACRB1_ERR_00811;
			break;

		case 812:
			alarm_id = ALID__VACRB1_ERR_00812;
			break;

		case 813:
			alarm_id = ALID__VACRB1_ERR_00813;
			break;

		case 814:
			alarm_id = ALID__VACRB1_ERR_00814;
			break;

		case 815:
			alarm_id = ALID__VACRB1_ERR_00815;
			break;

		case 816:
			alarm_id = ALID__VACRB1_ERR_00816;
			break;

		case 817:
			alarm_id = ALID__VACRB1_ERR_00817;
			break;

		case 818:
			alarm_id = ALID__VACRB1_ERR_00818;
			break;

		case 819:
			alarm_id = ALID__VACRB1_ERR_00819;
			break;

		case 820:
			alarm_id = ALID__VACRB1_ERR_00820;
			break;

		case 821:
			alarm_id = ALID__VACRB1_ERR_00821;
			break;

		case 822:
			alarm_id = ALID__VACRB1_ERR_00822;
			break;

		case 823:
			alarm_id = ALID__VACRB1_ERR_00823;
			break;

		case 824:
			alarm_id = ALID__VACRB1_ERR_00824;
			break;

		case 825:
			alarm_id = ALID__VACRB1_ERR_00825;
			break;

		case 826:
			alarm_id = ALID__VACRB1_ERR_00826;
			break;

		case 827:
			alarm_id = ALID__VACRB1_ERR_00827;
			break;		

		case 850:
			alarm_id = ALID__VACRB1_ERR_00850;
			break;

		case 851:
			alarm_id = ALID__VACRB1_ERR_00851;
			break;

		case 852:
			alarm_id = ALID__VACRB1_ERR_00852;
			break;

		case 853:
			alarm_id = ALID__VACRB1_ERR_00853;
			break;

		case 854:
			alarm_id = ALID__VACRB1_ERR_00854;
			break;

		case 855:
			alarm_id = ALID__VACRB1_ERR_00855;
			break;

		case 857:
			alarm_id = ALID__VACRB1_ERR_00857;
			break;

		case 860:
			alarm_id = ALID__VACRB1_ERR_00860;
			break;

		case 861:
			alarm_id = ALID__VACRB1_ERR_00861;
			break;

		case 862:
			alarm_id = ALID__VACRB1_ERR_00862;
			break;

		case 950:
			alarm_id = ALID__VACRB1_ERR_00950;
			break;

		case 951:
			alarm_id = ALID__VACRB1_ERR_00951;
			break;

		case 952:
			alarm_id = ALID__VACRB1_ERR_00952;
			break;

		case 953:
			alarm_id = ALID__VACRB1_ERR_00953;
			break;

		case 954:
			alarm_id = ALID__VACRB1_ERR_00954;
			break;

		case 955:
			alarm_id = ALID__VACRB1_ERR_00955;
			break;

		case 956:
			alarm_id = ALID__VACRB1_ERR_00956;
			break;

		case 1001:
			alarm_id = ALID__VACRB1_ERR_01001;
			break;

		case 1002:
			alarm_id = ALID__VACRB1_ERR_01002;
			break;

		case 1003:
			alarm_id = ALID__VACRB1_ERR_01003;
			break;

		case 1004:
			alarm_id = ALID__VACRB1_ERR_01004;
			break;

		case 1005:
			alarm_id = ALID__VACRB1_ERR_01005;
			break;

		case 1006:
			alarm_id = ALID__VACRB1_ERR_01006;
			break;

		case 1007:
			alarm_id = ALID__VACRB1_ERR_01007;
			break;

		case 1008:
			alarm_id = ALID__VACRB1_ERR_01008;
			break;

		case 1009:
			alarm_id = ALID__VACRB1_ERR_01009;
			break;

		case 1010:
			alarm_id = ALID__VACRB1_ERR_01010;
			break;

		case 1011:
			alarm_id = ALID__VACRB1_ERR_01011;
			break;

		case 1012:
			alarm_id = ALID__VACRB1_ERR_01012;
			break;

		case 1013:
			alarm_id = ALID__VACRB1_ERR_01013;
			break;

		case 1014:
			alarm_id = ALID__VACRB1_ERR_01014;
			break;

		case 1015:
			alarm_id = ALID__VACRB1_ERR_01015;
			break;

		case 1100:
			alarm_id = ALID__VACRB1_ERR_01100;
			break;

		case 1101:
			alarm_id = ALID__VACRB1_ERR_01101;
			break;

		case 1102:
			alarm_id = ALID__VACRB1_ERR_01102;
			break;

		case 1103:
			alarm_id = ALID__VACRB1_ERR_01103;
			break;

		case 1104:
			alarm_id = ALID__VACRB1_ERR_01104;
			break;

		case 1105:
			alarm_id = ALID__VACRB1_ERR_01105;
			break;

		case 1106:
			alarm_id = ALID__VACRB1_ERR_01106;
			break;

		case 1108:
			alarm_id = ALID__VACRB1_ERR_01108;
			break;

		case 1109:
			alarm_id = ALID__VACRB1_ERR_01109;
			break;

		case 1110:
			alarm_id = ALID__VACRB1_ERR_01110;
			break;

		case 1113:
			alarm_id = ALID__VACRB1_ERR_01113;
			break;

		case 1114:
			alarm_id = ALID__VACRB1_ERR_01114;
			break;

		case 1115:
			alarm_id = ALID__VACRB1_ERR_01115;
			break;

		case 1118:
			alarm_id = ALID__VACRB1_ERR_01118;
			break;

		case 1119:
			alarm_id = ALID__VACRB1_ERR_01119;
			break;

		case 1300:
			alarm_id = ALID__VACRB1_ERR_01300;
			break;

		case 1302:
			alarm_id = ALID__VACRB1_ERR_01302;
			break;

		case 1307:
			alarm_id = ALID__VACRB1_ERR_01307;
			break;

		case 1308:
			alarm_id = ALID__VACRB1_ERR_01308;
			break;

		case 1309:
			alarm_id = ALID__VACRB1_ERR_01309;
			break;

		case 1310:
			alarm_id = ALID__VACRB1_ERR_01310;
			break;

		case 1311:
			alarm_id = ALID__VACRB1_ERR_01311;
			break;

		case 1312:
			alarm_id = ALID__VACRB1_ERR_01312;
			break;

		case 1313:
			alarm_id = ALID__VACRB1_ERR_01313;
			break;

		case 1314:
			alarm_id = ALID__VACRB1_ERR_01314;
			break;

		case 1600:
			alarm_id = ALID__VACRB1_ERR_01600;
			break;

		case 1601:
			alarm_id = ALID__VACRB1_ERR_01601;
			break;

		case 1602:
			alarm_id = ALID__VACRB1_ERR_01602;
			break;

		case 1603:
			alarm_id = ALID__VACRB1_ERR_01603;
			break;

		case 1604:
			alarm_id = ALID__VACRB1_ERR_01604;
			break;

		case 1605:
			alarm_id = ALID__VACRB1_ERR_01605;
			break;

		case 1606:
			alarm_id = ALID__VACRB1_ERR_01606;
			break;

		case 1607:
			alarm_id = ALID__VACRB1_ERR_01607;
			break;

		case 1801:
			alarm_id = ALID__VACRB1_ERR_01801;
			break;

		case 1802:
			alarm_id = ALID__VACRB1_ERR_01802;
			break;

		case 1803:
			alarm_id = ALID__VACRB1_ERR_01803;
			break;

		case 1804:
			alarm_id = ALID__VACRB1_ERR_01804;
			break;

		case 1805:
			alarm_id = ALID__VACRB1_ERR_01805;
			break;

		case 1900:
			alarm_id = ALID__VACRB1_ERR_01900;
			break;

		case 1901:
			alarm_id = ALID__VACRB1_ERR_01901;
			break;	

		case 1902:
			alarm_id = ALID__VACRB1_ERR_01902;
			break;

		case 1903:
			alarm_id = ALID__VACRB1_ERR_01903;
			break;

		case 1904:
			alarm_id = ALID__VACRB1_ERR_01904;
			break;

		case 1905:
			alarm_id = ALID__VACRB1_ERR_01905;
			break;

		case 1910:
			alarm_id = ALID__VACRB1_ERR_01910;
			break;

		case 1911:
			alarm_id = ALID__VACRB1_ERR_01911;
			break;		

		case 1912:
			alarm_id = ALID__VACRB1_ERR_01912;
			break;

		case 1920:
			alarm_id = ALID__VACRB1_ERR_01920;
			break;

		case 2000:
			alarm_id = ALID__VACRB1_ERR_02000;
			break;

		case 2001:
			alarm_id = ALID__VACRB1_ERR_02001;
			break;

		case 2002:
			alarm_id = ALID__VACRB1_ERR_02002;
			break;

		case 2003:
			alarm_id = ALID__VACRB1_ERR_02003;
			break;

		case 2004:
			alarm_id = ALID__VACRB1_ERR_02004;
			break;

		case 2005:
			alarm_id = ALID__VACRB1_ERR_02005;
			break;

		case 2100:
			alarm_id = ALID__VACRB1_ERR_02100;
			break;

		case 2101:
			alarm_id = ALID__VACRB1_ERR_02101;
			break;

		case 2102:
			alarm_id = ALID__VACRB1_ERR_02102;
			break;

		case 2200:
			alarm_id = ALID__VACRB1_ERR_02200;
			break;

		case 2202:
			alarm_id = ALID__VACRB1_ERR_02202;
			break;

		case 2203:
			alarm_id = ALID__VACRB1_ERR_02203;
			break;

		case 3000:
			alarm_id = ALID__VACRB1_ERR_03000;
			break;

		case 3001:
			alarm_id = ALID__VACRB1_ERR_03001;
			break;

		case 3002:
			alarm_id = ALID__VACRB1_ERR_03002;
			break;

		case 3003:
			alarm_id = ALID__VACRB1_ERR_03003;
			break;

		case 3004:
			alarm_id = ALID__VACRB1_ERR_03004;
			break;

		case 3005:
			alarm_id = ALID__VACRB1_ERR_03005;
			break;

		case 3011:
			alarm_id = ALID__VACRB1_ERR_03011;
			break;

		case 4001:
			alarm_id = ALID__VACRB1_ERR_04001;
			break;

		case 4002:
			alarm_id = ALID__VACRB1_ERR_04002;
			break;	

		// Station Errors
		case 4003:
			alarm_id = ALID__VACRB1_ERR_04003;
			break;

		case 4004:
			alarm_id = ALID__VACRB1_ERR_04004;
			break;

		case 4005:
			alarm_id = ALID__VACRB1_ERR_04005;
			break;

		case 4006:
			alarm_id = ALID__VACRB1_ERR_04006;
			break;

		case 4007:
			alarm_id = ALID__VACRB1_ERR_04007;
			break;

		case 4008:
			alarm_id = ALID__VACRB1_ERR_04008;
			break;

		case 4009:
			alarm_id = ALID__VACRB1_ERR_04009;
			break;

		case 4010:
			alarm_id = ALID__VACRB1_ERR_04010;
			break;

		case 4011:
			alarm_id = ALID__VACRB1_ERR_04011;
			break;

		case 4012:
			alarm_id = ALID__VACRB1_ERR_04012;
			break;

		case 10000:
			alarm_id = ALID__VACRB1_ERR_10000;
			break;

		case 10001:
			alarm_id = ALID__VACRB1_ERR_10001;
			break;

		case 10002:
			alarm_id = ALID__VACRB1_ERR_10002;
			break;

		case 10003:
			alarm_id = ALID__VACRB1_ERR_10003;
			break;

		case 10004:
			alarm_id = ALID__VACRB1_ERR_10004;
			break;

		case 10005:
			alarm_id = ALID__VACRB1_ERR_10005;
			break;

		case 10006:
			alarm_id = ALID__VACRB1_ERR_10006;
			break;

		case 10007:
			alarm_id = ALID__VACRB1_ERR_10007;
			break;

		case 10008:
			alarm_id = ALID__VACRB1_ERR_10008;
			break;

		case 10009:
			alarm_id = ALID__VACRB1_ERR_10009;
			break;

		case 10010:
			alarm_id = ALID__VACRB1_ERR_10010;
			break;

		case 10011:
			alarm_id = ALID__VACRB1_ERR_10011;
			break;

		case 10012:
			alarm_id = ALID__VACRB1_ERR_10012;
			break;

		case 10013:
			alarm_id = ALID__VACRB1_ERR_10013;
			break;

		case 10014:
			alarm_id = ALID__VACRB1_ERR_10014;
			break;

		case 10015:
			alarm_id = ALID__VACRB1_ERR_10015;
			break;

		case 10016:
			alarm_id = ALID__VACRB1_ERR_10016;
			break;

		case 10017:
			alarm_id = ALID__VACRB1_ERR_10017;
			break;

		case 10018:
			alarm_id = ALID__VACRB1_ERR_10018;
			break;

		case 10019:
			alarm_id = ALID__VACRB1_ERR_10019;
			break;

		case 10020:
			alarm_id = ALID__VACRB1_ERR_10020;
			break;

		case 10021:
			alarm_id = ALID__VACRB1_ERR_10021;
			break;

		case 10022:
			alarm_id = ALID__VACRB1_ERR_10022;
			break;

		case 10023:
			alarm_id = ALID__VACRB1_ERR_10023;
			break;

		case 10024:
			alarm_id = ALID__VACRB1_ERR_10024;
			break;

		case 10025:
			alarm_id = ALID__VACRB1_ERR_10025;
			break;

		case 10026:
			alarm_id = ALID__VACRB1_ERR_10026;
			break;

		case 10028:
			alarm_id = ALID__VACRB1_ERR_10028;
			break;

		case 10029:
			alarm_id = ALID__VACRB1_ERR_10029;
			break;

		case 10030:
			alarm_id = ALID__VACRB1_ERR_10030;
			break;

		case 10031:
			alarm_id = ALID__VACRB1_ERR_10031;
			break;

		case 10032:
			alarm_id = ALID__VACRB1_ERR_10032;
			break;

		case 10034:
			alarm_id = ALID__VACRB1_ERR_10034;
			break;

		case 10035:
			alarm_id = ALID__VACRB1_ERR_10035;
			break;
		case 10036:
			alarm_id = ALID__VACRB1_ERR_10036;
			break;

		// Unknown Errors
		default:			    
			alarm_id = ALID__VACRB1_ERR_Unknown;
			break;
	}

	if(error != 0) 
	{
		p_alarm->Popup__ALARM(alarm_id,r_act);

		if(r_act.CompareNoCase("RETRY") == 0)	result = OBJ_RETRY;
		else									result = OBJ_ABORT;

	}

	// --- Clear !!
	return result;
}
