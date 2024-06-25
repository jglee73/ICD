#include "StdAfx.h"
#include "CObj__DRV_LINK_RBx.h"
#include "CObj__DRV_LINK_RBx__DEF.h"
#include "CObj__DRV_LINK_RBx__ALID.h"


//--------------------------------------------------------------------------------
CObj__DRV_LINK_RBx::CObj__DRV_LINK_RBx()
{

}
CObj__DRV_LINK_RBx::~CObj__DRV_LINK_RBx()
{

}

//--------------------------------------------------------------------------------
int CObj__DRV_LINK_RBx::__DEFINE__CONTROL_MODE(obj,l_mode)
{
	sObject_Name = obj;

	// ...
	{
		ADD__CTRL_VAR(sMODE__INIT,   "INIT");	
		ADD__CTRL_VAR(sMODE__HOME,   "HOME");	

		ADD__CTRL_VAR(sMODE__ROTATE, "ROTATE");	
		ADD__CTRL_VAR(sMODE__PICK,	 "PICK");	
		ADD__CTRL_VAR(sMODE__PLACE,	 "PLACE");	

		ADD__CTRL_VAR(sMODE__WAFER_CHECK_ARM, "WAFER_CHECK.ARM");
	}
	return 1;
}
int CObj__DRV_LINK_RBx::__DEFINE__VERSION_HISTORY(version)
{
	version = "JG.Lee \n";

	return 1;
}


// ...
#define  MON_ID__IO_MONITOR									1

// ...
#define APP_DSP__RB_TARGET_MOVE								\
"UNKNOWN HOME READY											\
 LBA LBB													\
 AL1														\
 LP1 LP2 LP3 LP4 LP5										\
 BUFF1 BUFF2"

#define APP_DSP__STN_NAME									\
"LP1 LP2 LP3 LP4 LP5										\
 LBA LBB													\
 AL1 														\
 BUFF1 BUFF2"

#define APP_DSP__ARM_TYPE									\
"A B"

#define APP_DSP__RET_EXT									\
"Retract Extend"

#define APP_DSP__ARM_STS_ANI								\
"UNKNOWN RETRACT EXTEND"

#define APP_DSP__DSlot										\
"1  2  3  4  5  6  7  8  9  10								\
11 12 13 14 15 16 17 18 19 20								\
21 22 23 24 25 26 27 28 29 30"


int CObj__DRV_LINK_RBx::__DEFINE__VARIABLE_STD(p_variable)
{
	DECLARE__STD_VARIABLE

	CCommon_Error__DEF_VARIABLE m_err_def_variable;
	p_variable->Link__DEF_VARIABLE__ERROR_FEEDBACK(&m_err_def_variable);

	// ...
	CString str_name;
	int i;

	// PARA ...
	{
		str_name = "CUR.CTRL.MODE";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__CUR_CTRL_MODE, str_name);

		//
		str_name = "PARA.STN.NAME";
		STD__ADD_DIGITAL(str_name, APP_DSP__STN_NAME);
		LINK__VAR_DIGITAL_CTRL(dCH__PARA_STN_NAME, str_name);

		str_name = "PARA.STN.SLOT";
		STD__ADD_DIGITAL(str_name, APP_DSP__DSlot);
		LINK__VAR_DIGITAL_CTRL(dCH__PARA_STN_SLOT, str_name);

		str_name = "PARA.ARM.TYPE";
		STD__ADD_DIGITAL(str_name, APP_DSP__ARM_TYPE);
		LINK__VAR_DIGITAL_CTRL(dCH__PARA_ARM_TYPE, str_name);

		str_name = "PARA.AL1.CCD.POS";
		STD__ADD_ANALOG(str_name, "deg", 1, 0.0, 360.0);
		LINK__VAR_ANALOG_CTRL(aCH__PARA_AL1_CCD_POS, str_name);
	}

	// CFG ...
	{
		str_name = "CFG.ACTION.CONFIRM.FLAG";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"DISABLE ENABLE","");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_ACTION_CONFIRM_FLAG,str_name);
	}

	// CFG : SIMULATION ...
	{
		str_name = "SIM.CFG.REAL.TEST";
		STD__ADD_DIGITAL(str_name, "NO YES");
		LINK__VAR_DIGITAL_CTRL(dCH__SIM_CFG__REAL_TEST, str_name);

		//
		str_name = "SIM.CFG.ROTATE.TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "sec", 1, 0, 9999, "");
		LINK__VAR_ANALOG_CTRL(aCH__SIM_CFG__ROTATE_TIME, str_name);

		//
		str_name = "SIM.CFG.LPx.PICK.TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "sec", 1, 0, 9999, "");
		LINK__VAR_ANALOG_CTRL(aCH__SIM_CFG__LPx_PICK_TIME, str_name);

		str_name = "SIM.CFG.LPx.PLACE.TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "sec", 1, 0, 9999, "");
		LINK__VAR_ANALOG_CTRL(aCH__SIM_CFG__LPx_PLACE_TIME, str_name);

		//
		str_name = "SIM.CFG.ALx.PICK.TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "sec", 1, 0, 9999, "");
		LINK__VAR_ANALOG_CTRL(aCH__SIM_CFG__ALx_PICK_TIME, str_name);

		str_name = "SIM.CFG.ALx.PLACE.TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "sec", 1, 0, 9999, "");
		LINK__VAR_ANALOG_CTRL(aCH__SIM_CFG__ALx_PLACE_TIME, str_name);

		//
		str_name = "SIM.CFG.LBx.PICK.TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "sec", 1, 0, 9999, "");
		LINK__VAR_ANALOG_CTRL(aCH__SIM_CFG__LBx_PICK_TIME, str_name);

		str_name = "SIM.CFG.LBx.PLACE.TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "sec", 1, 0, 9999, "");
		LINK__VAR_ANALOG_CTRL(aCH__SIM_CFG__LBx_PLACE_TIME, str_name);

		//
		str_name = "SIM.CFG.BUFFx.PICK.TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "sec", 1, 0, 9999, "");
		LINK__VAR_ANALOG_CTRL(aCH__SIM_CFG__BUFFx_PICK_TIME, str_name);

		str_name = "SIM.CFG.BUFFx.PLACE.TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "sec", 1, 0, 9999, "");
		LINK__VAR_ANALOG_CTRL(aCH__SIM_CFG__BUFFx_PLACE_TIME, str_name);
	}

	// MATERIAL CHANNEL ...
	{
		CString dsp_slot_sts;
		p_variable->Get__STD_DESCRIPTION("STD_SLOT_STATUS", dsp_slot_sts);

		// Material Status
		str_name = "MON.ARM_A.MATERIAL.STATUS";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, dsp_slot_sts, "");
		LINK__VAR_DIGITAL_CTRL(dCH__MON__ARM_A_MATERIAL_STATUS, str_name);

		str_name = "MON.ARM_B.MATERIAL.STATUS";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, dsp_slot_sts, "");
		LINK__VAR_DIGITAL_CTRL(dCH__MON__ARM_B_MATERIAL_STATUS, str_name);

		// Material Title
		str_name = "MON.ARM_A.MATERIAL.TITLE";
		STD__ADD_STRING_WITH_X_OPTION(str_name, "");
		LINK__VAR_STRING_CTRL(sCH__MON__ARM_A_MATERIAL_TITLE, str_name);

		str_name = "MON.ARM_B.MATERIAL.TITLE";
		STD__ADD_STRING_WITH_X_OPTION(str_name,"");
		LINK__VAR_STRING_CTRL(sCH__MON__ARM_B_MATERIAL_TITLE, str_name);
	}

	// ANIMATION CHANNEL ...
	{
		str_name = "MON.ACT.ARM";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, APP_DSP__ARM_TYPE, "");
		LINK__VAR_DIGITAL_CTRL(dCH__MON__ACT_ARM, str_name);

		str_name = "MON.ARM_A.ACT";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, APP_DSP__ARM_STS_ANI, "");
		LINK__VAR_DIGITAL_CTRL(dCH__MON__ARM_A_ACT, str_name);

		str_name = "MON.ARM_B.ACT";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, APP_DSP__ARM_STS_ANI, "");
		LINK__VAR_DIGITAL_CTRL(dCH__MON__ARM_B_ACT, str_name);

		str_name = "MON.TRG.MOVE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, APP_DSP__RB_TARGET_MOVE, "");
		LINK__VAR_DIGITAL_CTRL(dCH__MON__TRG_MOVE, str_name);

		str_name = "MON.TRG.ROTATE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, APP_DSP__RB_TARGET_MOVE, "");
		LINK__VAR_DIGITAL_CTRL(dCH__MON__TRG_ROTATE, str_name);
	}

	// ...
	{
		p_variable->Add__MONITORING_PROC(1.0, MON_ID__IO_MONITOR);
	}
	return 1;
}


// ...
#define  ACT__RETRY_ABORT						\
l_act.RemoveAll();								\
l_act.Add("RETRY");								\
l_act.Add("ABORT");

#define  ACT__ABORT_ALARM						\
l_act.RemoveAll();								\
l_act.Add("ABORT");

#define  ACT__NO_YES_ALARM						\
l_act.RemoveAll();								\
l_act.Add("NO");								\
l_act.Add("YES");


int CObj__DRV_LINK_RBx::__DEFINE__ALARM(p_alarm)
{
	DECLARE__ALARM;

	// ...
	CString title;
	title.Format("%s - ",sObject_Name);

	CString alarm_title;
	CString alarm_msg;
	CString alarm_bff;
	CStringArray l_act;
	int alarm_id;
	int i;

	// ...
	{
		alarm_id = ALID__OFFLINE_ALARM;

		alarm_title  = title;
		alarm_title += "Offline.";

		alarm_msg  = "Controller is Offline.\n";
		alarm_msg += "Please, Check Communication Status !\n";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}
	// ...
	{
		alarm_id = ALID__ERROR_CODE;

		alarm_title  = title;
		alarm_title += "Error Code !";

		alarm_msg = "Please, check HW status !\n";

		l_act.RemoveAll();
		l_act.Add(ACT__ABORT);
		l_act.Add(ACT__RETRY);
		l_act.Add(ACT__IGNORE);

		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);	
	}

	return 1;
}


//--------------------------------------------------------------------------------
int CObj__DRV_LINK_RBx::__INITIALIZE__OBJECT(p_variable,p_ext_obj_create)
{
	CString def_name;
	CString ch_name;
	CString obj_name;
	CString var_name;

	// OBJ : DB ...
	{
		def_name = "OBJ__DB";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, obj_name);

		var_name = "CFG.SIM.MODE";
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__CFG_SIM_MODE, obj_name,var_name);
	}

	// OBJ : ATM_DRV ...
	{
		def_name = "OBJ__ATM_DRV";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, obj_name);

		pOBJ_CTRL__ATM_DRV = p_ext_obj_create->Create__OBJECT_CTRL(obj_name);

		//
		var_name = "PARA.ROBOT.CMMD";
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__ROBOT_PARA_CMMD, obj_name,var_name);

		var_name = "RBx.CMMD.STATE";
		LINK__EXT_VAR_STRING_CTRL(sEXT_CH__ROBOT_CMMD_STS, obj_name,var_name);
	}

	// ...
	{
		CString file_name;
		CString log_msg;

		file_name.Format("%s_App.log", sObject_Name);

		log_msg  = "\n\n";
		log_msg += "//------------------------------------------------------------------------";

		xAPP_LOG_CTRL->CREATE__SUB_DIRECTORY(sObject_Name);
		xAPP_LOG_CTRL->SET__PROPERTY(file_name,24*5,60);

		xAPP_LOG_CTRL->DISABLE__TIME_LOG();
		xAPP_LOG_CTRL->WRITE__LOG(log_msg);

		xAPP_LOG_CTRL->ENABLE__TIME_LOG();
		xAPP_LOG_CTRL->WRITE__LOG("   START   \n");
	}

	// ...
	{
		SCX__SEQ_INFO x_seq;

		iActive__SIM_MODE = x_seq->Is__SIMULATION_MODE();
	}
	return 1;
}


//--------------------------------------------------------------------------------
int CObj__DRV_LINK_RBx::__CALL__CONTROL_MODE(mode, p_debug, p_variable, p_alarm)
{
LOOP_RETRY:

	// ...
	mMNG__ERR_CODE.Clear__Error_Code();

	// ...
	CString para__arm_type;
	CString para__stn_name;
	CString para__stn_slot;

	CStringArray l_para;
	p_variable->Get__FNC_PARAMETER(l_para);

	if(l_para.GetSize() > 0)
	{
		CString para_data;
		int limit = l_para.GetSize();
		int i;

		for(i=0;i<limit;i++)
		{
			para_data = l_para[i];

			switch(i)
			{
				case 0:		
					dCH__PARA_ARM_TYPE->Set__DATA(para_data);
					break;

				case 1:
					dCH__PARA_STN_NAME->Set__DATA(para_data);
					break;

				case 2:
					dCH__PARA_STN_SLOT->Set__DATA(para_data);
					break;
			}	
		}
	} 


	dCH__PARA_ARM_TYPE->Get__DATA(para__arm_type);
	dCH__PARA_STN_NAME->Get__DATA(para__stn_name);
	dCH__PARA_STN_SLOT->Get__DATA(para__stn_slot);

	// ...
	{
		CString log_msg;

		log_msg.Format("Start ... :  [%s] (%s,%s,%s)",
						mode,
						para__arm_type,
						para__stn_name,
						para__stn_slot);

		Fnc__APP_LOG(log_msg);
	}

	// ...
	int seq_flag = 1;
	int flag = -1;

	if(dCH__CFG_ACTION_CONFIRM_FLAG->Check__DATA(STR__ENABLE) > 0)
	{
		CString log_msg;

		CString str_title;
		CString str_msg;
		CString str_bff;
		CStringArray l_act;
		CString r_act;

		str_title.Format("%s - Action Confirm Flag", sObject_Name);

		str_bff.Format("Object Mode : [%s] \n", mode);
		str_msg += str_bff;
		str_bff.Format("Arm Type : [%s] \n", para__arm_type);
		str_msg += str_bff;
		str_bff.Format("Station Name : [%s] \n", para__stn_name);
		str_msg += str_bff;
		str_bff.Format("Station Slot : [%s] \n", para__stn_slot);
		str_msg += str_bff;

		l_act.RemoveAll();
		l_act.Add(STR__YES);
		l_act.Add(STR__NO);

		p_alarm->Popup__MESSAGE_BOX(str_title,str_msg,l_act,r_act);

		if(r_act.CompareNoCase(STR__NO) == 0)
		{
			seq_flag = -1;

			CString log_msg = "Confirm : Action No Clicked !";
			Fnc__APP_LOG(log_msg);
		}
	}

	if(seq_flag > 0)
	{
		sCH__CUR_CTRL_MODE->Set__DATA(mode);

			 IF__CTRL_MODE(sMODE__INIT)			flag = Call__INIT(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__HOME)			flag = Call__HOME(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__ROTATE)		flag = Call__ROTATE(p_variable,p_alarm, para__arm_type,para__stn_name,para__stn_slot);
		ELSE_IF__CTRL_MODE(sMODE__PICK)			flag = Call__PICK(p_variable,p_alarm,   para__arm_type,para__stn_name,para__stn_slot);
		ELSE_IF__CTRL_MODE(sMODE__PLACE)		flag = Call__PLACE(p_variable,p_alarm,  para__arm_type,para__stn_name,para__stn_slot);

		ELSE_IF__CTRL_MODE(sMODE__WAFER_CHECK_ARM)		flag = Call__WAFER_CHECK_ARM(p_variable,p_alarm);
	}
	else
	{
		flag = -1;
	}

	// ...
	{
		CString err_code;
		CString err_msg;

		while(mMNG__ERR_CODE.Get__Error_Code(err_code,err_msg) > 0)
		{
			int i_error = atoi(err_code);
			if(i_error == 0)		continue;

			// ...
			{
				CString log_msg;
				CString log_bff;

				log_msg = "Error_Code Occurred !!!";

				log_bff.Format("Error_Code : [%s] \n", err_code);
				log_msg += log_bff;
				log_bff.Format("Error_Msg  : [%s] \n", err_msg);
				log_msg += log_bff;

				Fnc__APP_LOG(log_msg);
			}

			// ...
			int alm_id = ALID__ERROR_CODE;
			CString alm_msg;
			CString alm_bff;
			CString r_act;

			alm_msg.Format("Control_Mode : %s \n", mode);

			alm_bff.Format("Error_Code : %s \n", err_code);
			alm_msg += alm_bff;
			alm_bff.Format("Error_Message \n");
			alm_msg += alm_bff;
			alm_msg += "  * ";
			alm_msg += err_msg;

			// ...
			{
				CString log_msg;

				log_msg.Format("Alarm Post : %1d \n", alm_id);
				log_msg += alm_msg;

				Fnc__APP_LOG(log_msg);
			}

			p_alarm->Popup__ALARM_With_MESSAGE(alm_id,alm_msg,r_act);			

			if(r_act.CompareNoCase(ACT__RETRY) == 0)
			{
				// ...
				{
					CString log_msg = "Alarm Selection <- Retry";
					Fnc__APP_LOG(log_msg);
				}

				goto LOOP_RETRY;
			}
			if(r_act.CompareNoCase(ACT__ABORT) == 0)
			{
				// ...
				{
					CString log_msg = "Alarm Selection <- Abort";
					Fnc__APP_LOG(log_msg);
				}

				flag = -1;
			}

			if(r_act.CompareNoCase(ACT__IGNORE) == 0)
			{
				// ...
			}
		}
	}

	if((flag < 0)||(p_variable->Check__CTRL_ABORT() > 0))
	{
		CString log_msg;	
		log_msg.Format("Aborted ... :  [%s]\n", mode);

		Fnc__APP_LOG(log_msg);
	}
	else
	{
		CString log_msg;
		log_msg.Format("Completed ... :  [%s]\n", mode);

		Fnc__APP_LOG(log_msg);
	}

	return flag;
}

int CObj__DRV_LINK_RBx::__CALL__MONITORING(id, p_variable, p_alarm)
{
	switch(id)
	{
		case MON_ID__IO_MONITOR:
			Mon__IO_MONITOR(p_variable,p_alarm);
			break;
	}

	return 1;
}
