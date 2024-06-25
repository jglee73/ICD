#include "StdAfx.h"
#include "CObj__DB_CFG.h"


//-------------------------------------------------------------------------
CObj__DB_CFG::CObj__DB_CFG()
{

}
CObj__DB_CFG::~CObj__DB_CFG()
{

}

//-------------------------------------------------------------------------
int CObj__DB_CFG::__DEFINE__CONTROL_MODE(obj,l_mode)
{
	sObject_Name = obj;

	return 1;
}

int CObj__DB_CFG::__DEFINE__VERSION_HISTORY(version)
{

	return 1;
}


// ...
#define APP_DSP__CFG_STS					"Unchange  Change"

#define APP_DSP__DISABLE_ENABLE				"DISABLE  ENABLE"
#define APP_DSP__TRANSFER_MODE				"ATM  VAC"
#define APP_DSP__YES_NO						"YES  NO"

#define APP_DSP__MATERIAL_TYPE									\
"300mmRnd  3inRnd  82.5mmRnd  100mmRnd  125mmRnd  150mmRnd  200mmRnd  Other"	

#define APP_DSP__UP_DOWN					"UP  DOWN"
#define APP_DSP__FDCL						"None  Flat  Ntch"
#define APP_DSP__LP_TYPE					"Normal  6M"
#define APP_DSP__OCR_TYPE					"None  SIMEMS  OTHER"

#define APP_DSP__SYS_PUMP_TYPE				"ONE_MANIFOLD  TWO_MANIFOLD"


// ...
#define LINK__VAR(VAR_NAME,CH_NAME)								\
str_name = CH_NAME;												\
VAR_NAME = CH_NAME;


// ...
#define MON_ID__PM_LINK_TEST				1


int CObj__DB_CFG::__DEFINE__VARIABLE_STD(p_variable)
{
	DECLARE__STD_VARIABLE;

	// ...
	CString str_name;
	CString def_name;
	CString def_data;
	int i;

	// LLx ...
	{
		CString def_name;
		CString def_data;

		def_name = "LLx.MULTI_SLOT_VALVE";
		p_variable->Get__DEF_CONST_DATA(def_name, def_data);

		if(def_data.CompareNoCase("YES") == 0)			bActive__LLx_MULTI_SLOT_VALVE = true;
		else											bActive__LLx_MULTI_SLOT_VALVE = false;
	}

	for(i=0;i<CFG_PAGE__SIZE;i++)
	{
		str_name.Format("CFG.PAGE%1d.SAVE",i+1);
		LINK__VAR(dVAR__CFG_PAGEx_SAVE[i],str_name);
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,APP_DSP__CFG_STS,"");
	}

	// PAGE.LPX ...
	{
		str_name = "CFG.PAGE.ROBOT.SAVE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, APP_DSP__CFG_STS, "");

		str_name = "CFG.PAGE.PRESSURE.SAVE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, APP_DSP__CFG_STS, "");

		str_name = "CFG.PAGE.SLOT_VALVE.SAVE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, APP_DSP__CFG_STS, "");

		str_name = "CFG.PAGE.FA_INTERFACE.SAVE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, APP_DSP__CFG_STS, "");

		str_name = "CFG.PAGE.SYSTEM.SAVE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, APP_DSP__CFG_STS, "");

		str_name = "CFG.PAGE.LLx.SAVE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, APP_DSP__CFG_STS, "");

		str_name = "CFG.PAGE.DA.SAVE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, APP_DSP__CFG_STS, "");

		str_name = "CFG.PAGE.ACTIVE_HANDOFF.SAVE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, APP_DSP__CFG_STS, "");

		//
		str_name = "CFG.PAGE.LPx_COMMON.SAVE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, APP_DSP__CFG_STS, "");

		str_name = "CFG.PAGE.LPx_PURGE.SAVE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, APP_DSP__CFG_STS, "");
	}

	// ...
	{
		str_name = "CFG.HEATER.SAVE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, APP_DSP__CFG_STS, "");
	}

	// ...
	{
		str_name = "CFG.DA_LLx.SAVE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, APP_DSP__CFG_STS, "");

		str_name = "CFG.DA_PMx.SAVE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, APP_DSP__CFG_STS, "");
	}

	// ...
	{
		str_name = "CUR.UTILITY.CATEGORY";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__CUR_UTILITY_CATEGORY, str_name);

		str_name = "CUR.UTILITY.FNC.NAME";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__CUR_UTILITY_FNC_NAME, str_name);
	}

	// ...
	{
		str_name = "SCH_TEST.CFG.PMC_DUMMY_BY_CTC";
		STD__ADD_DIGITAL(str_name, "NO YES");
		LINK__VAR_DIGITAL_CTRL(dCH__SCH_TEST_CFG_PMC_DUMMY_BY_CTC, str_name);

		//
		str_name = "CFG.SETUP.TEST.MODE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "DISABLE ENABLE", "");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_SETUP_TEST_MODE, str_name);

		str_name = "CFG.SETUP.PRESSURE.CHECK";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "ENABLE DISABLE", "");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_SETUP_PRESSURE_CHECK, str_name);

		str_name = "CFG.SETUP.AUTO.VENTING_PUMPING";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "DISABLE ENABLE", "");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_SETUP_AUTO_VENTING_PUMPING, str_name);

		str_name = "CFG.SETUP.LLx.DOOR_SLOT_VLV.CONTROL";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "DISABLE ENABLE", "");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_SETUP_LLx_DOOR_SLOT_VLV_CONTROL, str_name);
	}

	// ...
	{
		LINK__VAR(aVAR__CFG_SIM_TMx_VENT_TIME,"CFG.SIM.TMx.VENT.TIME");
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",1,0,99,"");

		LINK__VAR(aVAR__CFG_SIM_TMx_PUMP_TIME,"CFG.SIM.TMx.PUMP.TIME");
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",1,0,99,"");

		LINK__VAR(aVAR__CFG_SIM_SLOT_VLV_CLOSE_TIME,"CFG.SIM.SLOT.VLV.CLOSE.TIME");
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",1,0,99,"");

		LINK__VAR(aVAR__CFG_SIM_SLOT_VLV_OPEN_TIME,"CFG.SIM.SLOT.VLV.OPEN.TIME");
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",1,0,99,"");

		LINK__VAR(aVAR__CFG_SIM_DOOR_VLV_CLOSE_TIME,"CFG.SIM.DOOR.VLV.CLOSE.TIME");
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",1,0,99,"");

		LINK__VAR(aVAR__CFG_SIM_DOOR_VLV_OPEN_TIME,"CFG.SIM.DOOR.VLV.OPEN.TIME");
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",1,0,99,"");

		LINK__VAR(aVAR__CFG_SIM_LLx_SLOW_VENT_TIME,"CFG.SIM.LLx.SLOW.VENT.TIME");
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",1,0,99,"");

		LINK__VAR(aVAR__CFG_SIM_LLx_FAST_VENT_TIME,"CFG.SIM.LLx.FAST.VENT.TIME");
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",1,0,99,"");

		LINK__VAR(aVAR__CFG_SIM_LLx_SLOT_PUMP_TIME,"CFG.SIM.LLx.SLOW.PUMP.TIME");
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",1,0,99,"");

		LINK__VAR(aVAR__CFG_SIM_LLx_FAST_PUMP_TIME,"CFG.SIM.LLx.FAST.PUMP.TIME");
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",1,0,99,"");
	}

	// ...
	{
		LINK__VAR(dVAR__CFG_TRANSFER_MODE,"CFG.TRANSFER.MODE");
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,APP_DSP__TRANSFER_MODE,"");

		LINK__VAR(aVAR__CFG_REF_ATM_PRESSURE,"CFG.REF.ATM.PRESSURE");
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"torr",3,0,1000,"");

		LINK__VAR(aVAR__CFG_REF_VAC_PRESSURE,"CFG.REF.VAC.PRESSURE");
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"torr",3,0,1000,"");

		LINK__VAR(sVAR__CTC_CUR_MODE, "CTC.CUR_MODE");
		STD__ADD_STRING(str_name);
	}

	// LPx -----
	{
		for(i=0;i<CFG_LP_LIMIT;i++)
		{
			str_name.Format("LP%1d.STx.USE.N2_PURGE", i+1);
			STD__ADD_STRING(str_name);

			//
			str_name.Format("CFG.LP%1d.EXIST.FLAG",i+1);
			LINK__VAR(dVAR__CFG_LPx_EXIST_FLAG[i],str_name);
			STD__ADD_DIGITAL_WITH_X_OPTION(str_name,APP_DSP__YES_NO,"");

			str_name.Format("CFG.LP%1d.TYPE",i+1);
			LINK__VAR(dVAR__CFG_LPx_TYPE[i],str_name);
			STD__ADD_DIGITAL_WITH_X_OPTION(str_name,APP_DSP__YES_NO,"");
		}

		str_name.Format("CFG.LT.CURTN.USE");
		LINK__VAR(dVAR__CFG_LT_CURTN_USE,str_name);
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,APP_DSP__YES_NO,"");

		str_name.Format("CFG.PIO.TP1");
		LINK__VAR(aVAR__CFG_PIO_TP1,str_name);
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "sec", 0, 0, 180,"");

		str_name.Format("CFG.PIO.TP2");
		LINK__VAR(aVAR__CFG_PIO_TP2,str_name);
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "sec", 0, 0, 180,"");

		str_name.Format("CFG.PIO.TP3");
		LINK__VAR(aVAR__CFG_PIO_TP3,str_name);
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "sec", 0, 0, 180,"");

		str_name.Format("CFG.PIO.TP4");
		LINK__VAR(aVAR__CFG_PIO_TP4,str_name);
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "sec", 0, 0, 180,"");

		str_name.Format("CFG.PIO.TP5");
		LINK__VAR(aVAR__CFG_PIO_TP5,str_name);
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "sec", 0, 0, 180,"");

		str_name.Format("CFG.PIO.TP6");
		LINK__VAR(aVAR__CFG_PIO_TP6,str_name);
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "sec", 0, 0, 180,"");

		str_name.Format("CFG.PIO.TD1");
		LINK__VAR(aVAR__CFG_PIO_TD1,str_name);
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "sec", 0, 0, 180,"");
	}

	// ALx ...
	{
		str_name.Format("CFG.dALIGN.DEVICE");
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"ATM_RB ALIGNER","");

		for(i=0; i<CFG_ALx__LIMIT; i++)
		{
			int id = i + 1;

			str_name.Format("CFG.AL%1d.SLOT.MAX", id);
			STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"1 2","");

			str_name.Format("CFG.AL%1d.EXIST.FLAG", id);
			STD__ADD_DIGITAL_WITH_X_OPTION(str_name,APP_DSP__YES_NO,"");

			str_name.Format("CFG.AL%1d.CCD.POS", id);
			STD__ADD_ANALOG_WITH_X_OPTION(str_name,"deg",1,0.0,360.0,"");

			str_name.Format("CFG.AL%1d.OCR.CCD.POS", id);
			STD__ADD_ANALOG_WITH_X_OPTION(str_name,"deg",1,0.0,360.0,"");

			str_name.Format("CFG.AL%1d.MATERIAL.TYPE", id);
			STD__ADD_DIGITAL_WITH_X_OPTION(str_name,APP_DSP__MATERIAL_TYPE,"");

			str_name.Format("CFG.AL%1d.FDCL", id);
			STD__ADD_DIGITAL_WITH_X_OPTION(str_name,APP_DSP__FDCL,"");

			/*
			str_name.Format("CFG.AL%1d.OCR.TYPE", id);
			STD__ADD_DIGITAL_WITH_X_OPTION(str_name,APP_DSP__OCR_TYPE,"");
			*/
		}
	}

	// VAT ...
	{
		str_name = "CFG.PROCESS.MANOMETER.MAX.PRESSURE.mTORR";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "mtorr", 0, 0, 10000, "");

		str_name = "SIM.PRESSURE.TORR";
		STD__ADD_STRING(str_name);
	}

	// PMx
	{
		for(i=0; i<CFG_PM_LIMIT; i++)
		{
			int id = i + 1;

			str_name.Format("CFG.PM%1d.ALIGN.POS", id);
			STD__ADD_ANALOG_WITH_X_OPTION(str_name,"deg",1,0.0,360.0,"");

			str_name.Format("CFG.PM%1d.EXIST.FLAG", id);
			STD__ADD_DIGITAL_WITH_X_OPTION(str_name,APP_DSP__YES_NO,"");

			str_name.Format("CFG.PM%1d.SV.USE", id);
			STD__ADD_DIGITAL(str_name,APP_DSP__YES_NO);

			str_name.Format("CFG.PM%1d.CHM.TYPE", id);
			STD__ADD_DIGITAL(str_name, "REAL DUMMY");
		}
	}

	// LLx ...
	{
		for(i=0; i<CFG_LLx__LIMIT; i++)
		{
			CString ll_name = _Get__LLx_NAME(i);
			int id = i + 1;

			if(bActive__LLx_MULTI_SLOT_VALVE)
			{
				str_name.Format("CFG.LL%1d.1.ALIGN.POS", id);
				STD__ADD_ANALOG_WITH_X_OPTION(str_name,"deg",1,0.0,360.0,"");

				str_name.Format("CFG.LL%1d.2.ALIGN.POS", id);
				STD__ADD_ANALOG_WITH_X_OPTION(str_name,"deg",1,0.0,360.0,"");

				//
				str_name.Format("CFG.LL%1d.1.EXIST.FLAG", id);
				STD__ADD_DIGITAL_WITH_X_OPTION(str_name,APP_DSP__YES_NO,"");

				str_name.Format("CFG.LL%1d.2.EXIST.FLAG", id);
				STD__ADD_DIGITAL_WITH_X_OPTION(str_name,APP_DSP__YES_NO,"");
			}
			else
			{
				str_name.Format("CFG.LL%1d.ALIGN.POS", id);
				STD__ADD_ANALOG_WITH_X_OPTION(str_name,"deg",1,0.0,360.0,"");

				//
				str_name.Format("CFG.%s.EXIST.FLAG", ll_name);
				STD__ADD_DIGITAL_WITH_X_OPTION(str_name,APP_DSP__YES_NO,"");
			}

			// ...
			{
				str_name.Format("CFG.%s.SV.EXIST.FLAG", ll_name);
				STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"FALSE TRUE","");

				str_name.Format("CFG.%s.SLOT.MAX", ll_name);
				STD__ADD_ANALOG_WITH_X_OPTION(str_name, "slot", 0,0,2, "");
		
				//
				str_name.Format("CFG.%s.LIFT_PIN.EXIST.FLAG", ll_name);
				STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"FALSE TRUE","");

				str_name.Format("CFG.%s.LIFT_PIN.DOWN.MODE.AFTER_DOOR_CLOSE", ll_name);
				STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"ENABLE DISABLE","");

				str_name.Format("CFG.%s.LIFT_PIN.UP.MODE.AFTER_HEATING", ll_name);
				STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"DISABLE ENABLE","");

				str_name.Format("CFG.%s.LIFT_PIN.UP.TIMEOUT", ll_name);
				STD__ADD_ANALOG_WITH_X_OPTION(str_name, "sec", 0, 3, 100, "");

				str_name.Format("CFG.%s.LIFT_PIN.DOWN.TIMEOUT", ll_name);
				STD__ADD_ANALOG_WITH_X_OPTION(str_name, "sec", 0, 3, 100, "");
			}

			// LLx.PARA ...
			{
				str_name.Format("LL%1d.PARA.VENT.COOLING_SEC", id);
				STD__ADD_ANALOG(str_name, "sec", 0, 0, 9999);

				str_name.Format("LL%1d.COUNT.VENT.COOLING_SEC", id);
				STD__ADD_STRING(str_name);
			}
		}
	}

	// VIS1 ...
	{
		str_name.Format("CFG.VIS1.EXIST.FLAG");
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,APP_DSP__YES_NO,"");
	}

	// BUFF1 ...
	{
		str_name.Format("CFG.BUFF1.EXIST.FLAG");
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,APP_DSP__YES_NO,"");
	}
	// BUFF2 ...
	{
		str_name.Format("CFG.BUFF2.EXIST.FLAG");
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,APP_DSP__YES_NO,"");
	}
	// FULL BUFF ...
	{
		str_name.Format("CFG.FULL.BUFF.EXIST.FLAG");
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,APP_DSP__YES_NO,"");
	}

	// Cool Station ...
	{
		str_name.Format("CFG.dCOOL.STATION.SLOT1.USE");
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"NOTUSE AVAILABLE","");

		str_name.Format("CFG.dCOOL.STATION.SLOT2.USE");
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"NOTUSE AVAILABLE","");

		str_name.Format("CFG.dCOOL.STATION.SLOT3.USE");
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"NOTUSE AVAILABLE","");

		str_name.Format("CFG.dCOOL.STATION.SLOT4.USE");
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"NOTUSE AVAILABLE","");

		str_name.Format("CFG.dCOOL.DEFAULT.PROCESS.TIME");
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",0,0,180,"def:60");
	}

	// OCR ...
	{
		str_name.Format("CFG.dOCR.ENABLED");	
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "FALSE TRUE", "");

		str_name.Format("CFG.aOCR.ANGLE.ROTATE.BACK");	
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",1,0.0,3600,"def:0");

		str_name.Format("CFG.dOCR.DEBUG.MODE");	
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"FALSE TRUE", "");

		str_name.Format("CFG.aOCR.ERROR.BYPASS");
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",0,0,2,"def:1");

		str_name.Format("CFG.aOCR.NUM.RETRY.SEMI.M14");
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",0,0,999,"def:5");

		str_name.Format("CFG.aOCR.NUM.RETRY.SEMI.M17");
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",0,0,999,"def:8");

		str_name.Format("CFG.aOCR.RPLACE.CENTER.ALIGN");
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",0,0,1,"def:0");

		str_name.Format("CFG.aOCR.SCORE.LIMIT.SEMI.M14");
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",0,50,999,"def:70");

		str_name.Format("CFG.aOCR.SELECT.READ.OPT.TYPE");
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",0,0,2,"def:0");

		str_name.Format("CFG.dVERIFY.CYCLE.TEST.ENABLED");
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"FALSE TRUE", "");

		str_name.Format("CFG.aOCR.WAFER.ROTATE.ANGLE");	
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",1,0.0,3600,"def:900");
	}

	// ...
	{
		str_name.Format("CFG.dOHT1.CLAMP.TURN.OFF.HO");	
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"FALSE TRUE","def:FALSE");

		str_name.Format("CFG.dOHT2.CLAMP.TURN.OFF.HO");	
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"FALSE TRUE","def:FALSE");

		str_name.Format("CFG.dOHT3.CLAMP.TURN.OFF.HO");	
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"FALSE TRUE","def:FALSE");

		str_name.Format("CFG.aOHT1.COMPLETE.ON.TIMEOUTE23_T5");	
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",0,1,999,"def:60");

		str_name.Format("CFG.aOHT2.COMPLETE.ON.TIMEOUTE23_T5");	
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",0,1,999,"def:60");

		str_name.Format("CFG.aOHT3.COMPLETE.ON.TIMEOUTE23_T5");	
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",0,1,999,"def:60");

		str_name.Format("CFG.dOHT1.ENABLE.DEBUG");
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"FALSE TRUE","def:FALSE");

		str_name.Format("CFG.dOHT2.ENABLE.DEBUG");
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"FALSE TRUE","def:FALSE");

		str_name.Format("CFG.dOHT3.ENABLE.DEBUG");
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"FALSE TRUE","def:FALSE");

		str_name.Format("CFG.dOHT1.ENABLED.DISPLAY.ON.UI");	
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"FALSE TRUE","def:FALSE");

		str_name.Format("CFG.dOHT2.ENABLED.DISPLAY.ON.UI");	
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"FALSE TRUE","def:FALSE");

		str_name.Format("CFG.dOHT3.ENABLED.DISPLAY.ON.UI");	
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"FALSE TRUE","def:FALSE");

		str_name.Format("CFG.dOHT1.ERR.RECOVERY.ONE.OPTION");	
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"UNKNOWN NONE","def:NONE");

		str_name.Format("CFG.dOHT2.ERR.RECOVERY.ONE.OPTION");
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"UNKNOWN NONE","def:NONE");

		str_name.Format("CFG.dOHT3.ERR.RECOVERY.ONE.OPTION");
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"UNKNOWN NONE","def:NONE");

		str_name.Format("CFG.dOHT1.ERR.RECOVERY.TYPE1");
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"FALSE TRUE","def:FALSE");

		str_name.Format("CFG.dOHT2.ERR.RECOVERY.TYPE1");
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"FALSE TRUE","def:FALSE");

		str_name.Format("CFG.dOHT3.ERR.RECOVERY.TYPE1");
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"FALSE TRUE","def:FALSE");

		str_name.Format("CFG.dOHT1.SELECT.E23.E84");
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"0 1","def:0");

		str_name.Format("CFG.dOHT2.SELECT.E23.E84");
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"0 1","def:0");

		str_name.Format("CFG.dOHT3.SELECT.E23.E84");
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"0 1","def:0");

		str_name.Format("CFG.aOHT1.SHORT.DELAY");
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"msec",0,50,1000,"def:500");

		str_name.Format("CFG.aOHT2.SHORT.DELAY");
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"msec",0,50,1000,"def:500");

		str_name.Format("CFG.aOHT3.SHORT.DELAY");
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"msec",0,50,1000,"def:500");
	}

	// ...
	{
		str_name.Format("CFG.dLP1.DISPLAY.READ.TAG.PAGE1and2.ONLY");
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"FALSE TRUE","def:FALSE");

		str_name.Format("CFG.dLP2.DISPLAY.READ.TAG.PAGE1and2.ONLY");
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"FALSE TRUE","def:FALSE");

		str_name.Format("CFG.dLP3.DISPLAY.READ.TAG.PAGE1and2.ONLY");
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"FALSE TRUE","def:FALSE");

		str_name.Format("CFG.dLP1.ACC.MODE.VIOLATION.TO.POST.ERR.ALARM");
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"FALSE TRUE","def:FALSE");

		str_name.Format("CFG.dLP2.ACC.MODE.VIOLATION.TO.POST.ERR.ALARM");
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"FALSE TRUE","def:FALSE");

		str_name.Format("CFG.dLP3.ACC.MODE.VIOLATION.TO.POST.ERR.ALARM");
		LINK__VAR(dVAR__CFG_LP3_ACC_MODE_VIOLATION_TO_POST_ERR_ALARM,str_name);
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"FALSE TRUE","def:FALSE");

		str_name.Format("CFG.dLP1.IS.DUAL.LIGHT.INSTALLED");
		LINK__VAR(dVAR__CFG_LP1_IS_DUAL_LIGHT_INSTALLED,str_name);
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"FALSE TRUE","def:FALSE");

		str_name.Format("CFG.dLP2.IS.DUAL.LIGHT.INSTALLED"); 
		LINK__VAR(dVAR__CFG_LP2_IS_DUAL_LIGHT_INSTALLED,str_name);
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"FALSE TRUE","def:FALSE");

		str_name.Format("CFG.dLP3.IS.DUAL.LIGHT.INSTALLED");
		LINK__VAR(dVAR__CFG_LP3_IS_DUAL_LIGHT_INSTALLED,str_name);
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"FALSE TRUE","def:FALSE");

		str_name.Format("CFG.dLP1.ENABLE.DEBUG");
		LINK__VAR(dVAR__CFG_LP1_ENABLE_DEBUG,str_name);
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"0 1","def:0");

		str_name.Format("CFG.dLP2.ENABLE.DEBUG");
		LINK__VAR(dVAR__CFG_LP2_ENABLE_DEBUG,str_name);
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"0 1","def:0");

		str_name.Format("CFG.dLP3.ENABLE.DEBUG");
		LINK__VAR(dVAR__CFG_LP3_ENABLE_DEBUG,str_name);
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"0 1","def:0");

		str_name.Format("CFG.aLP1.WAFER.NUMBER");
		LINK__VAR(aVAR__CFG_LP1_WAFER_NUMBER,str_name);
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"-",0,13,26,"def:25");

		str_name.Format("CFG.aLP2.WAFER.NUMBER");
		LINK__VAR(aVAR__CFG_LP2_WAFER_NUMBER,str_name);
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"-",0,13,26,"def:25");

		str_name.Format("CFG.aLP3.WAFER.NUMBER");
		LINK__VAR(aVAR__CFG_LP3_WAFER_NUMBER,str_name);
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"-",0,13,26,"def:25");
	}

	// ...
	{
		str_name.Format("CFG.aLP1.PDO.LIGHT.BTN.DOCK.UNDOCK.OPT");
		LINK__VAR(dVAR__CFG_LP1_PDO_LIGHT_BTN_DOCK_UNDOCK_OPT,str_name);
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"-",0,0,2,"def:0");

		str_name.Format("CFG.aLP2.PDO.LIGHT.BTN.DOCK.UNDOCK.OPT");
		LINK__VAR(dVAR__CFG_LP2_PDO_LIGHT_BTN_DOCK_UNDOCK_OPT,str_name);
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"-",0,0,2,"def:0");

		str_name.Format("CFG.aLP3.PDO.LIGHT.BTN.DOCK.UNDOCK.OPT");
		LINK__VAR(dVAR__CFG_LP3_PDO_LIGHT_BTN_DOCK_UNDOCK_OPT,str_name);
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"-",0,0,2,"def:0");
	}

	// ...
	{
		str_name.Format("CFG.dLP1.READ.8BYTES.FOR.TAG.READ");
		LINK__VAR(dVAR__CFG_LP1_READ_8BYTES_FOR_TAG_READ,str_name);
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"UNKNOWN RETURN8","def:RETURN8");

		str_name.Format("CFG.dLP2.READ.8BYTES.FOR.TAG.READ");
		LINK__VAR(dVAR__CFG_LP2_READ_8BYTES_FOR_TAG_READ,str_name);
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"UNKNOWN RETURN8","def:RETURN8");

		str_name.Format("CFG.dLP3.READ.8BYTES.FOR.TAG.READ");
		LINK__VAR(dVAR__CFG_LP3_READ_8BYTES_FOR_TAG_READ,str_name);
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"UNKNOWN RETURN8","def:RETURN8");

		str_name.Format("CFG.dLP1.TRIM.LEADING.TRAILING.SPACES");
		LINK__VAR(dVAR__CFG_LP1_TRIM_LEADING_TRAILING_SPACES,str_name);
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"FALSE TRUE","def:FALSE");

		str_name.Format("CFG.dLP2.TRIM.LEADING.TRAILING.SPACES");
		LINK__VAR(dVAR__CFG_LP2_TRIM_LEADING_TRAILING_SPACES,str_name);
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"FALSE TRUE","def:FALSE");

		str_name.Format("CFG.dLP3.TRIM.LEADING.TRAILING.SPACES");
		LINK__VAR(dVAR__CFG_LP3_TRIM_LEADING_TRAILING_SPACES,str_name);
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"FALSE TRUE","def:FALSE");

		str_name.Format("CFG.dLP1.TRIM.TRAILING.UNDER.SCORES");	
		LINK__VAR(dVAR__CFG_LP1_TRIM_TRAILING_UNDER_SCORES,str_name);
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"FALSE TRUE","def:TRUE");

		str_name.Format("CFG.dLP2.TRIM.TRAILING.UNDER.SCORES");	
		LINK__VAR(dVAR__CFG_LP2_TRIM_TRAILING_UNDER_SCORES,str_name);
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"FALSE TRUE","def:TRUE");

		str_name.Format("CFG.dLP3.TRIM.TRAILING.UNDER.SCORES");	
		LINK__VAR(dVAR__CFG_LP3_TRIM_TRAILING_UNDER_SCORES,str_name);
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"FALSE TRUE","def:TRUE");
	}

	// ...
	{
		str_name.Format("CFG.dLP1.TAG.ID1.ENABLED.DISPLAY.ON.UI");
		LINK__VAR(dVAR__CFG_LP1_TAG_ID1_ENABLED_DISPLAY_ON_UI,str_name);
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"FALSE TRUE","def:TRUE");

		str_name.Format("CFG.dLP2.TAG.ID1.ENABLED.DISPLAY.ON.UI");
		LINK__VAR(dVAR__CFG_LP2_TAG_ID1_ENABLED_DISPLAY_ON_UI,str_name);
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"FALSE TRUE","def:TRUE");

		str_name.Format("CFG.dLP3.TAG.ID1.ENABLED.DISPLAY.ON.UI");
		LINK__VAR(dVAR__CFG_LP3_TAG_ID1_ENABLED_DISPLAY_ON_UI,str_name);
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"FALSE TRUE","def:TRUE");
	}

	// CFG : LLx SLOT ...
	{
		for(i=0; i<CFG_LLx__LIMIT; i++)
		{
			int id = i + 1;

			str_name.Format("CFG.LL%1d.SLOT01.USE", id);
			STD__ADD_DIGITAL(str_name,"NO  YES");

			str_name.Format("CFG.LL%1d.SLOT02.USE", id);
			STD__ADD_DIGITAL(str_name,"NO  YES");
		}
	}

	// LLx ALIGN ...
	{
		// ...
		{
			sVAR_NAME__MAINT_TARGET_LLx_NAME = "MAINT.TARGET.LLx.NAME";

			if(bActive__LLx_MULTI_SLOT_VALVE)
			{
				str_name = sVAR_NAME__MAINT_TARGET_LLx_NAME;
				STD__ADD_DIGITAL(str_name,"LL1 LL2  PM1 PM2 PM3 PM4 PM5 PM6");

				str_name = "MAINT.TARGET.LLx.SLOT";
				STD__ADD_DIGITAL(str_name, "1 2");
			}
			else
			{
				str_name = sVAR_NAME__MAINT_TARGET_LLx_NAME;
				STD__ADD_DIGITAL(str_name,"LBA LBB LBC LBD  PM1 PM2 PM3 PM4 PM5 PM6");

				str_name = "MAINT.TARGET.LLx.SLOT";
				STD__ADD_DIGITAL(str_name, "1");
			}

			//
			str_name = "MODULE.LINK.sTARGET.LLx.NAME";
			STD__ADD_STRING(str_name);

			str_name = "MODULE.LINK.sTARGET.LLx.SLOT";
			STD__ADD_STRING(str_name);
		}

		// ...
		{
			str_name = "CUR.AL1.TARGET.LLx.NAME";
			STD__ADD_STRING(str_name);

			str_name = "CUR.AL1.TARGET.LLx.SLOT";
			STD__ADD_STRING(str_name);

			//
			str_name = "CUR.AL2.TARGET.LLx.NAME";
			STD__ADD_STRING(str_name);

			str_name = "CUR.AL2.TARGET.LLx.SLOT";
			STD__ADD_STRING(str_name);
		}

		// ...
		{
			str_name = "CUR.AL1.CCD.POS";
			STD__ADD_STRING(str_name);

			str_name = "CUR.AL2.CCD.POS";
			STD__ADD_STRING(str_name);
		}
	}

	// LLx - Align ...
	for(i=0; i<CFG_LLx__LIMIT; i++)
	{
		int id = i + 1;

		if(bActive__LLx_MULTI_SLOT_VALVE)
		{
			str_name.Format("CFG.LL%1d.1.POST.POSITION.INCREMENT", id);
			STD__ADD_ANALOG_WITH_X_OPTION(str_name, "degree", 0,0,3, "");

			str_name.Format("CFG.LL%1d.2.POST.POSITION.INCREMENT", id);
			STD__ADD_ANALOG_WITH_X_OPTION(str_name, "degree", 0,0,3, "");

			//
			str_name.Format("CFG.LL%1d.1.POST.POSITION.INCREMENT.RANGE", id);
			STD__ADD_ANALOG_WITH_X_OPTION(str_name, "degree", 0,0,360, "");

			str_name.Format("CFG.LL%1d.2.POST.POSITION.INCREMENT.RANGE", id);
			STD__ADD_ANALOG_WITH_X_OPTION(str_name, "degree", 0,0,360, "");

			//
			str_name.Format("CFG.LL%1d.1.POST.POSITION.INCREMENT.START.ANGLE", id); 
			STD__ADD_ANALOG_WITH_X_OPTION(str_name, "degree", 0,0,360, "");

			str_name.Format("CFG.LL%1d.2.POST.POSITION.INCREMENT.START.ANGLE", id); 
			STD__ADD_ANALOG_WITH_X_OPTION(str_name, "degree", 0,0,360, "");

			//
			str_name.Format("CFG.LL%1d.1.POST.POSITION.INCREMENT.APPLY", id);
			STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"DISABLE ENABLE","");

			str_name.Format("CFG.LL%1d.2.POST.POSITION.INCREMENT.APPLY", id);
			STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"DISABLE ENABLE","");

			//
			str_name.Format("CUR.LL%1d.1.POST.POSITION.INCREMENT.ANGLE", id);
			STD__ADD_ANALOG(str_name,"degree", 0,0,360);

			str_name.Format("CUR.LL%1d.2.POST.POSITION.INCREMENT.ANGLE", id);
			STD__ADD_ANALOG(str_name,"degree", 0,0,360);
		}
		else
		{
			str_name.Format("CFG.aLL%1d.POST.POSITION.INCREMENT", id);
			STD__ADD_ANALOG_WITH_X_OPTION(str_name, "degree", 0,0,3, "");

			str_name.Format("CFG.aLL%1d.POST.POSITION.INCREMENT.RANGE", id);
			STD__ADD_ANALOG_WITH_X_OPTION(str_name, "degree", 0,0,360, "");

			str_name.Format("CFG.aLL%1d.POST.POSITION.INCREMENT.START.ANGLE", id); 
			STD__ADD_ANALOG_WITH_X_OPTION(str_name, "degree", 0,0,360, "");

			str_name.Format("CFG.LL%1d.POST.POSITION.INCREMENT.APPLY", id);
			STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"DISABLE ENABLE","");

			str_name.Format("CUR.aLL%1d.POST.POSITION.INCREMENT.ANGLE", id);
			STD__ADD_ANALOG(str_name, "degree", 0,0,360);
		}
	}

	// PMx - Align ...
	for(i=0; i<CFG_PM_LIMIT; i++)
	{
		int id = i + 1;

		str_name.Format("CFG.PM%1d.POST.POSITION.INCREMENT", id);
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "degree", 0,0,3, "");

		str_name.Format("CFG.PM%1d.POST.POSITION.INCREMENT.RANGE", id);
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "degree", 0,0,360, "");

		str_name.Format("CFG.PM%1d.POST.POSITION.INCREMENT.START.ANGLE", id); 
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "degree", 0,0,360, "");

		str_name.Format("CFG.PM%1d.POST.POSITION.INCREMENT.APPLY", id);
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"DISABLE ENABLE","");

		str_name.Format("CUR.PM%1d.POST.POSITION.INCREMENT.ANGLE", id);
		STD__ADD_ANALOG(str_name, "degree", 0,0,360);
	}

	// ...
	{
		str_name.Format("CFG.dALLOW.INIT.APM.WHEN.INIT.ALL");
		LINK__VAR(dVAR__CFG_ALLOW_INIT_APM_WHEN_INIT_ALL,str_name);
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"FALSE TRUE","def:TRUE");

		str_name.Format("CFG.dALLOW.INIT.METROLOGY.WHEN.INIT.ALL");	
		LINK__VAR(dVAR__CFG_ALLOW_INIT_METROLOGY_WHEN_INIT_ALL,str_name);
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"FALSE TRUE","def:TRUE");

		str_name.Format("CFG.dALLOW.LOAD.UNLOAD.ERR.TO.CONTINUE.OHT.OPERATION");
		LINK__VAR(dVAR__CFG_ALLOW_LOAD_UNLOAD_ERR_TO_CONTINUE_OHT_OPERATION,str_name);
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"0 1","def:TRUE");
	}

	for(i=0;i<CFG_LP_LIMIT;i++)
	{
		str_name.Format("CFG.dLP%1d.DOOR_CLOSE_MODE.AFTER_MAPPING", i+1);
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"DISABLE ENABLE","");
	}

	// PMC LINK TEST ...
	{
		for(i=0;i<CFG_PM_LIMIT;i++)
		{
			str_name.Format("PM%1d.LINK_TEST.FLAG", i+1);
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__PMC_LINK_TEST_FLAG[i],str_name);
		}
	}

	// ...
	{
		str_name = "CFG.SIM.MODE";
		STD__ADD_DIGITAL(str_name,APP_DSP__DISABLE_ENABLE);
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_SIM_MODE,str_name);
	}

	// ANI Channel ...
	{
		str_name = "ANI.PRESSURE_STATUS.ATM";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__ANI_PRESSURE_STATUS_ATM, str_name);
	}

	// DA.LLx .. 
	for(i=0; i<CFG_LLx__LIMIT; i++)
	{
		int id = i + 1;

		// DA Use ...
		{
			str_name.Format("CFG.DA.PICK.USE.LL%1d", id);
			STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "YES NO", "");

			str_name.Format("CFG.DA.PLACE.USE.LL%1d", id);
			STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "YES NO", "");
		}
	}
	// DA.PMx ...
	for(i=0; i<CFG_PM_LIMIT; i++)
	{
		int id = i + 1;

		// DA Use ...
		{
			str_name.Format("CFG.DA.PICK.USE.PM%1d", id);
			STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "YES NO", "");

			str_name.Format("CFG.DA.PLACE.USE.PM%1d", id);
			STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "YES NO", "");
		}
	}

	// ...
	{
		p_variable->Add__MONITORING_PROC(1.0, MON_ID__PM_LINK_TEST);
	}
	return 1;
}


// ...
#define  ACT__CHECK								\
l_act.RemoveAll();								\
l_act.Add("CHECK");


int CObj__DB_CFG::__DEFINE__ALARM(p_alarm)
{
	DECLARE__ALARM;

	// ...
	CString title;
	title.Format("%s - ",sObject_Name);

	CString alm_title;
	CString alm_msg;
	CStringArray l_act;
	int alm_id;

	// ...
	{
		alm_id = ALID__SETUP_MODE_ENABLE;

		alm_title  = title;
		alm_title += "The config setting of \"Setup Mode\" is \"ENABLE\" !";

		alm_msg = "Please, check config page.\n";

		ACT__CHECK;
		ADD__ALARM_EX(alm_id,alm_title,alm_msg, l_act);
	}

	// ...
	{
		alm_id = ALID__SETUP_MODE_PRESSURE_CHECK_DISABLE;

		alm_title  = title;
		alm_title += "The config setting of \"Setup Mode Pressure Check\" is \"DISABLE\" !";

		alm_msg = "Please, check config page.\n";

		ACT__CHECK;
		ADD__ALARM_EX(alm_id,alm_title,alm_msg, l_act);
	}

	// ...
	{
		alm_id = ALID__SIMULATION_MODE_ACTIVE;

		alm_title  = title;
		alm_title += "Simulation mode is active !";

		alm_msg = "";

		l_act.RemoveAll();
		l_act.Add("CHECK");

		ADD__ALARM_EX(alm_id,alm_title,alm_msg,l_act);
	}

	// ...
	{
		alm_id = ALID__SIMULATION_PMC_DUMMY_MODE_ACTIVE;

		alm_title  = title;
		alm_title += "PMC-Dummy mode is active !";

		alm_msg = "";

		l_act.RemoveAll();
		l_act.Add("CHECK");

		ADD__ALARM_EX(alm_id,alm_title,alm_msg,l_act);
	}

	return 1;
}


//-------------------------------------------------------------------------
int CObj__DB_CFG::__INITIALIZE__OBJECT(p_variable,p_ext_obj_create)
{
	CString def_name;
	CString def_data;

	// Target Align ...
	{
		CString str_list;

		def_name = "DATA_LIST.TRG_ALIGN";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);

		int k_limit = atoi(def_data);
		for(int k=0; k<k_limit; k++)
		{
			int id = k + 1;

			def_name.Format("TRG_ALIGN.MODULE.%1d", id);
			p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);

			str_list += def_data;
			str_list += " ";
		}

		if(k_limit > 0)
		{
			CString var_name = sVAR_NAME__MAINT_TARGET_LLx_NAME;

			p_ext_obj_create->Change__LOCAL_DIGITAL_DESCRIPTOR_INFO(var_name, str_list);
		}
	}

	// ...
	{
		sCH__ANI_PRESSURE_STATUS_ATM->Set__DATA("ATM");

		sCH__CUR_UTILITY_CATEGORY->Set__DATA("IDLE");
		sCH__CUR_UTILITY_FNC_NAME->Set__DATA("IDLE");
	}
	return 1;
}


//-------------------------------------------------------------------------
int CObj__DB_CFG::__CALL__CONTROL_MODE(mode,p_debug,p_variable,p_alarm)
{

	return 1;
}

int CObj__DB_CFG::__CALL__MONITORING(id,p_variable,p_alarm)
{
	switch(id)
	{
		case MON_ID__PM_LINK_TEST:	
			Mon__PM_LINK_TEST(p_variable,p_alarm);
			break;
	}

	return 1;
}
