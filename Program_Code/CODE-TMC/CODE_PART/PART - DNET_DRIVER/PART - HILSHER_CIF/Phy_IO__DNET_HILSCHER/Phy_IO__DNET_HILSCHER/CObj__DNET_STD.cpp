#include "StdAfx.h"
#include "CObj__DNET_STD.h"
#include "CObj__DNET_STD__DEF.h"

#include "cifuser.h"
#include "rcs_user.h"
#include "dnm_user.h"


//--------------------------------------------------------------------------------
CObj__DNET_STD::CObj__DNET_STD()
{
	InitializeCriticalSection(&mLOCK_DNET);

	iDNet_BoardNumber = -1;
	iDNet_Board_Out_Offset = 0;
	iDNet_Board_In_Offset  = 0;
}
CObj__DNET_STD::~CObj__DNET_STD()
{

	DeleteCriticalSection(&mLOCK_DNET);
}

//--------------------------------------------------------------------------------
int CObj__DNET_STD::__DEFINE__CONTROL_MODE(obj, l_mode)
{
	// ...
	{
		sObject_Name = obj;
	}

	// ...
	{
		ADD__CTRL_VAR(sMODE__INIT, "INIT");

		//
		ADD__CTRL_VAR(sMODE__LINK_IO_SET_OFF, "LINK_IO.SET_OFF");
		ADD__CTRL_VAR(sMODE__LINK_IO_SET_ON,  "LINK_IO.SET_ON" );

		//
		ADD__CTRL_VAR(sMODE__DEV_INFO, "DEV_INFO");

		// 
		ADD__CTRL_VAR(sMODE__FLOAT_TO_HEXA, "FLOAT_TO_HEXA");
		ADD__CTRL_VAR(sMODE__HEXA_TO_FLOAT, "HEXA_TO_FLOAT");

		ADD__CTRL_VAR(sMODE__NODE_OUT_BYTE_UPLOAD, "NODE.OUT.BYTE.UPLOAD");
		ADD__CTRL_VAR(sMODE__NODE_IN_BYTE_UPLOAD,  "NODE.IN.BYTE.UPLOAD" );
	}
	return 1;
}
int CObj__DNET_STD::__DEFINE__VERSION_HISTORY(version)
{
	version = "JG.Lee \n";

	return 1;
}


// ...
#define  MON_ID__MONITOR				1

#define  APP_DSP__OFF_ON				"OFF  ON"


int CObj__DNET_STD::__DEFINE__VARIABLE_STD(p_variable)
{
	DECLARE__STD_VARIABLE

	CString str_name;

	// OBJ ...
	{
		str_name = "OBJ.MSG";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__OBJ_MSG, str_name);
	}

	// Config ...
	{
		str_name = "CFG.SLAVE_INFO.CHECK";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "YES NO", "");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_SLAVE_INFO_CHECK, str_name);

		//
		str_name = "CFG.DRV_INT.STABLE.SEC";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "sec", 0, 1, 10, "");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_DRV_INT_STABLE_SEC, str_name);

		str_name = "CFG.DRV_INT.DELAY.mSEC";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "msec", 0, 10, 500, "");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_DRV_INT_DELAY_mSEC, str_name);

		str_name = "CFG.DRV_INT.RETRY.CHECK";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "count", 0, 1, 10, "");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_DRV_INT_RETRY_CHECK, str_name);

		//
		str_name = "CFG.DNET.HEARTBEAT.PERIOD";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "sec", 0, 0, 10, "");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_DNET_HEARTBEAT_PERIOD, str_name);

		str_name = "DNET.HEARTBEAT.TIME.COUNT";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__DNET_HEARTBEAT_TIME_COUNT, str_name);
	}

	// ...
	{
		str_name = "DNET.INFO.MASTER_BOARD.ID";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__DNET_INFO__MASTER_BOARD_ID, str_name);

		str_name = "DNET.INFO.MASTER_BOARD.IN_BYTE.OFFSET";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__DNET_INFO__MASTER_BOARD_IN_BYTE_OFFSET, str_name);

		str_name = "DNET.INFO.MASTER_BOARD.OUT_BYTE.OFFSET";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__DNET_INFO__MASTER_BOARD_OUT_BYTE_OFFSET, str_name);

		//
		str_name = "DNET.INFO.MASTER_BOARD_NAME";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__DNET_INFO__MASTER_BOARD_NAME, str_name);

		str_name = "DNET.INFO._MASTER_BOARD.DRIVER_VERSION";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__DNET_INFO__MASTER_BOARD_DRIVER_VERSION, str_name);

		//
		str_name = "DNET.INFO.BAUD_RATE";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__DNET_INFO__BAUD_RATE, str_name);

		// DNET.INFO ...
		{
			str_name = "DNET.INFO.TOTAL_OUT_BYTE";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__DNET_INFO__TOTAL_OUT_BYTE, str_name);

			str_name = "DNET.INFO.TOTAL_IN_BYTE";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__DNET_INFO__TOTAL_IN_BYTE, str_name);
		}

		// CFG.MAX ...
		{
			str_name = "DNET.CFG.TOTAL_OUT_BYTE.MAX";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__DNET_CFG__TOTAL_OUT_BYTE_MAX, str_name);

			str_name = "DNET.CFG.TOTAL_IN_BYTE.MAX";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__DNET_CFG__TOTAL_IN_BYTE_MAX, str_name);
		}
		// CFG.USE ...
		{
			str_name = "DNET.CFG.TOTAL_OUT_BYTE.USE";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__DNET_CFG__TOTAL_OUT_BYTE_USE, str_name);

			str_name = "DNET.CFG.TOTAL_IN_BYTE.USE";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__DNET_CFG__TOTAL_IN_BYTE_USE, str_name);
		}

		//
		{
			iSLAVE_COUNT = 0;

			str_name = "DNET.INFO.SLAVE_COUNT";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__DNET_INFO__SLAVE_COUNT, str_name);

			str_name = "DNET.CFG.SLAVE_COUNT";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__DNET_CFG__SLAVE_COUNT, str_name);
		}

		// ...
		{
			str_name = "di.Comm.Sts";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(diCH__COMM_STS, str_name);

			str_name = "APP.DRV.LOG.ENABLE";
			STD__ADD_DIGITAL(str_name, "NO YES");
			LINK__VAR_DIGITAL_CTRL(dCH__APP_DRV_LOG_ENABLE, str_name);
		}

		for(int i=0; i<CFG__SLAVE_SIZE; i++)
		{
			int id = i + 1;

			// INFO ...
			{
				str_name.Format("DNET.INFO.SLAVE.%1d.MACID", id);
				STD__ADD_STRING(str_name);
				LINK__VAR_STRING_CTRL(sCH__DNET_INFO__SLAVE_X__MACID[i], str_name);

				str_name.Format("DNET.INFO.SLAVE.%1d.NAME", id);
				STD__ADD_STRING(str_name);
				LINK__VAR_STRING_CTRL(sCH__DNET_INFO__SLAVE_X__NAME[i], str_name);

				str_name.Format("DNET.INFO.SLAVE.%1d.IN_SIZE.USE", id);
				STD__ADD_STRING(str_name);
				LINK__VAR_STRING_CTRL(sCH__DNET_INFO__SLAVE_X__IN_SIZE_USE[i], str_name);

				str_name.Format("DNET.INFO.SLAVE.%1d.OUT_SIZE.USE", id);
				STD__ADD_STRING(str_name);
				LINK__VAR_STRING_CTRL(sCH__DNET_INFO__SLAVE_X__OUT_SIZE_USE[i], str_name);

				//
				str_name.Format("DNET.INFO.SLAVE.%1d.COMM_STATE", id);
				STD__ADD_STRING(str_name);
				LINK__VAR_STRING_CTRL(sCH__DNET_INFO__SLAVE_X__COMM_STATE[i], str_name);

				str_name.Format("DNET.INFO.SLAVE.%1d.COMM_HEXA", id);
				STD__ADD_STRING(str_name);
				LINK__VAR_STRING_CTRL(sCH__DNET_INFO__SLAVE_X__COMM_HEXA[i], str_name);

				//
				str_name.Format("DNET.INFO.SLAVE.%1d.ERROR_CHECK_ID", id);
				STD__ADD_STRING(str_name);
				LINK__VAR_STRING_CTRL(sCH__DNET_INFO__SLAVE_X__ERROR_CHECK_ID[i], str_name);
			}

			// CFG ...
			{
				str_name.Format("DNET.CFG.SLAVE.%1d.MACID", id);
				STD__ADD_STRING(str_name);
				LINK__VAR_STRING_CTRL(sCH__DNET_CFG__SLAVE_X__MACID[i], str_name);

				str_name.Format("DNET.CFG.SLAVE.%1d.NAME", id);
				STD__ADD_STRING(str_name);
				LINK__VAR_STRING_CTRL(sCH__DNET_CFG__SLAVE_X__NAME[i], str_name);

				//
				str_name.Format("DNET.CFG.SLAVE.%1d.IN_OFFSET", id);
				STD__ADD_STRING(str_name);
				LINK__VAR_STRING_CTRL(sCH__DNET_CFG__SLAVE_X__IN_OFFSET[i], str_name);

				str_name.Format("DNET.CFG.SLAVE.%1d.IN_SIZE.MAX", id);
				STD__ADD_STRING(str_name);
				LINK__VAR_STRING_CTRL(sCH__DNET_CFG__SLAVE_X__IN_SIZE_MAX[i], str_name);

				str_name.Format("DNET.CFG.SLAVE.%1d.IN_SIZE.USE", id);
				STD__ADD_STRING(str_name);
				LINK__VAR_STRING_CTRL(sCH__DNET_CFG__SLAVE_X__IN_SIZE_USE[i], str_name);

				//
				str_name.Format("DNET.CFG.SLAVE.%1d.OUT_OFFSET", id);
				STD__ADD_STRING(str_name);
				LINK__VAR_STRING_CTRL(sCH__DNET_CFG__SLAVE_X__OUT_OFFSET[i], str_name);

				str_name.Format("DNET.CFG.SLAVE.%1d.OUT_SIZE.MAX", id);
				STD__ADD_STRING(str_name);
				LINK__VAR_STRING_CTRL(sCH__DNET_CFG__SLAVE_X__OUT_SIZE_MAX[i], str_name);

				str_name.Format("DNET.CFG.SLAVE.%1d.OUT_SIZE.USE", id);
				STD__ADD_STRING(str_name);
				LINK__VAR_STRING_CTRL(sCH__DNET_CFG__SLAVE_X__OUT_SIZE_USE[i], str_name);
			}
		}

		//
		{
			str_name = "DNET.INFO.COMM_STATE.CHECK.ACTIVE";
			STD__ADD_DIGITAL(str_name, "NO YES");
			LINK__VAR_DIGITAL_CTRL(dCH__DNET_INFO__COMM_STATE_CHECK_ACTIVE, str_name);
		}
	}

	// ...
	{
		int i;

		str_name = "DNET.INFO.OUT.BYTE.CHECK.ACTIVE";
		STD__ADD_DIGITAL(str_name, "NO YES");
		LINK__VAR_DIGITAL_CTRL(dCH__DNET_INFO__OUT_BYTE_CHECK_ACTIVE, str_name);

		for(i=0; i<CFG__DNET_OUT_BYTE_LIMIT; i++)
		{
			str_name.Format("DNET.INFO.OUT.BYTE.DATA.%1d", i);
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__DNET_INFO__OUT_BYTE_DATA_X[i], str_name);
		}

		//
		str_name = "DNET.INFO.IN.BYTE.CHECK.ACTIVE";
		STD__ADD_DIGITAL(str_name, "NO YES");
		LINK__VAR_DIGITAL_CTRL(dCH__DNET_INFO__IN_BYTE_CHECK_ACTIVE, str_name);

		for(i=0; i<CFG__DNET_IN_BYTE_LIMIT; i++)
		{
			str_name.Format("DNET.INFO.IN.BYTE.DATA.%1d", i);
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__DNET_INFO__IN_BYTE_DATA_X[i], str_name);
		}
	}

	// ...
	{
		str_name = "FLOAT_TO_HEXA.VALUE";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__FLOAT_TO_HEXA__VALUE, str_name);

		str_name = "FLOAT_TO_HEXA.RESULT";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__FLOAT_TO_HEXA__RESULT, str_name);

		//
		str_name = "HEXA_TO_FLOAT.VALUE";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__HEXA_TO_FLOAT__VALUE, str_name);

		str_name = "HEXA_TO_FLOAT.RESULT";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__HEXA_TO_FLOAT__RESULT, str_name);
	}

	// ...
	{
		str_name = "TEST.GAUGE.HEXA";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__TEST_GAUGE_HEXA, str_name);

		str_name = "TEST.GAUGE.VALUE";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__TEST_GAUGE_VALUE, str_name);
	}
	// ...
	{
		str_name = "TEST.FFFF.HEXA";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__TEST_FFFF_HEXA, str_name);

		str_name = "TEST.FFFF.VALUE";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__TEST_FFFF_VALUE, str_name);
	}

	// ...
	{
		str_name = "CFG.LINK_IO.WAITING.SEC";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "sec", 0, 1, 60, "");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_LINK_IO_WAITING_SEC, str_name);
	}

	// ...
	{
		p_variable->Add__MONITORING_PROC(1.0, MON_ID__MONITOR);
	}
	return 1;
}
int CObj__DNET_STD::__DEFINE__ALARM(p_alarm)
{
	DECLARE__ALARM;

	// ...
	CString obj_title;
	obj_title.Format("%s - ",sObject_Name);

	int alm_id;
	CString alm_title;
	CString alm_msg;
	CStringArray l_act;

	// ...
	{
		alm_id = ALID__OFFLINE;

		alm_title  = obj_title;
		alm_title += "Offline !";

		alm_msg = "Check the line & parameter.";

		l_act.RemoveAll();
		l_act.Add("CHECK");

		ADD__ALARM_EX(alm_id, alm_title, alm_msg, l_act);
	}
	// ...
	{
		alm_id = ALID__LAST_ERROR_CODE;

		alm_title  = obj_title;
		alm_title += "Last Error Code !";

		alm_msg = "Check the line & parameter.";

		l_act.RemoveAll();
		l_act.Add("CHECK");

		ADD__ALARM_EX(alm_id, alm_title, alm_msg, l_act);
	}
	// ...
	{
		alm_id = ALID__DNET_NODE_ERROR;

		alm_title  = obj_title;
		alm_title += "Device-Net Node Error !";

		alm_msg = "Check the device-net node.";

		l_act.RemoveAll();
		l_act.Add(_ACT__ABORT);
		l_act.Add(_ACT__RETRY);
		l_act.Add(_ACT__IGNORE);

		ADD__ALARM_EX(alm_id, alm_title, alm_msg, l_act);
	}
	// ...
	{
		alm_id = ALID__DNET_INIT_ERROR;

		alm_title  = obj_title;
		alm_title += "Device-Net Initial Error !";

		alm_msg = "Check the device-net state.";

		l_act.RemoveAll();
		l_act.Add(_ACT__CHECK);

		ADD__ALARM_EX(alm_id, alm_title, alm_msg, l_act);
	}

	return 1;
}

int CObj__DNET_STD::__DEFINE__VARIABLE_IO(p_io_variable)
{
	DECLARE__IO_VARIABLE;

	CString str_name;

	// ...
	{

	}
	return 1;
}


//--------------------------------------------------------------------------------
int CObj__DNET_STD::__INITIALIZE__OBJECT(p_variable,p_ext_obj_create)
{
	CString def_name;
	CString def_data;

	CString ch_name;
	CString obj_name;
	CString var_name;

	// ...
	{
		CString file_name;
		CString log_msg;

		// APP Log ...
		{
			file_name.Format("%s__APP.log", sObject_Name);

			log_msg  = "\n\n";
			log_msg += "//------------------------------------------------------------------------";

			xI__APP_LOG_CTRL->CREATE__SUB_DIRECTORY(sObject_Name);
			xI__APP_LOG_CTRL->SET__PROPERTY(file_name,12*1,60);

			xI__APP_LOG_CTRL->DISABLE__TIME_LOG();
			xI__APP_LOG_CTRL->WRITE__LOG(log_msg);

			xI__APP_LOG_CTRL->ENABLE__TIME_LOG();
			xI__APP_LOG_CTRL->WRITE__LOG("   START   \n");
		}

		// DRV Log ...
		{
			file_name.Format("%s__DRV.log", sObject_Name);

			log_msg  = "\n\n";
			log_msg += "//------------------------------------------------------------------------";

			xI__DRV_LOG_CTRL->CREATE__SUB_DIRECTORY(sObject_Name);
			xI__DRV_LOG_CTRL->SET__PROPERTY(file_name,12*1,60);

			xI__DRV_LOG_CTRL->DISABLE__TIME_LOG();
			xI__DRV_LOG_CTRL->WRITE__LOG(log_msg);

			xI__DRV_LOG_CTRL->ENABLE__TIME_LOG();
			xI__DRV_LOG_CTRL->WRITE__LOG("   START   \n");
		}
	}

	// SIM.MODE ...
	{
		SCX__SEQ_INFO x_seq_info;

		iActive__SIM_MODE = x_seq_info->Is__SIMULATION_MODE();
	}

	// LINK : DO.HEARTBEAT ...
	{
		def_name = "CH__DO_DNET_HEARTBEAT";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, ch_name);

		if((ch_name.CompareNoCase("NO")   == 0)
		|| (ch_name.CompareNoCase("NULL") == 0))
		{
			bActive__DO_DNET_HEARTBEAT = false;
		}
		else
		{
			bActive__DO_DNET_HEARTBEAT = true;

			p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(ch_name, obj_name,var_name);
			LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__DO_DNET_HEARTBEAT, obj_name,var_name);
		}
	}

	// LINK : IO ...
	{
		CString def_name;
		CString def_data;

		CString ch_name;
		CString obj_name;
		CString var_name;

		int i_limit;
		int i;

		def_name = "LINK.IO_SIZE";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);

		i_limit = atoi(def_data);
		if(i_limit > CFG__LINK_IO_SIZE)		i_limit = CFG__LINK_IO_SIZE;

		iSIZE__LINK_IO = i_limit;

		for(i=0; i<i_limit; i++)
		{
			def_name.Format("LINK.IO_CHANNEL.%1d", i+1);

			p_ext_obj_create->Get__DEF_CONST_DATA(def_name, ch_name);
			p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(ch_name, obj_name,var_name);

			LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__DNET_LINK_IO[i], obj_name,var_name);
		}
	}
	return 1;
}
int CObj__DNET_STD::__INITIALIZE__IO(p_io_para)
{
	// ...
	{
		CString para_name;
		CString para_data;

		int i_limit;
		int i;

		para_name = "BOARD.ID";
		p_io_para->Get__PARAMETER_DATA(para_name, para_data);
		int bd_ud = atoi(para_data);
		mDNet_Mng.Set__BOARD_ID(bd_ud);

		para_name = "BOARD.OUT_OFFSET";
		p_io_para->Get__PARAMETER_DATA(para_name, para_data);
		iDNet_Board_Out_Offset = atoi(para_data);

		para_name = "BOARD.IN_OFFSET";
		p_io_para->Get__PARAMETER_DATA(para_name, para_data);
		iDNet_Board_In_Offset = atoi(para_data);

		//
		para_name = "NODE.SIZE";
		p_io_para->Get__PARAMETER_DATA(para_name, para_data);
		i_limit = atoi(para_data);
		if(i_limit > CFG__SLAVE_SIZE)		i_limit = CFG__SLAVE_SIZE;
		iSLAVE_COUNT = i_limit;

		para_data.Format("%1d", i_limit);
		sCH__DNET_CFG__SLAVE_COUNT->Set__DATA(para_data);

		for(i=0; i<i_limit; i++)
		{
			int id = i + 1;

			para_name.Format("NODE.%1d.MACID", id);
			p_io_para->Get__PARAMETER_DATA(para_name, para_data);
			sCH__DNET_CFG__SLAVE_X__MACID[i]->Set__DATA(para_data);

			para_name.Format("NODE.%1d.NAME",  id);
			p_io_para->Get__PARAMETER_DATA(para_name, para_data);
			sCH__DNET_CFG__SLAVE_X__NAME[i]->Set__DATA(para_data);

			//
			para_name.Format("NODE.%1d.IN_OFFSET", id);
			p_io_para->Get__PARAMETER_DATA(para_name, para_data);
			sCH__DNET_CFG__SLAVE_X__IN_OFFSET[i]->Set__DATA(para_data);

			para_name.Format("NODE.%1d.IN_BYTE.MAX",  id);
			p_io_para->Get__PARAMETER_DATA(para_name, para_data);
			sCH__DNET_CFG__SLAVE_X__IN_SIZE_MAX[i]->Set__DATA(para_data);

			para_name.Format("NODE.%1d.IN_BYTE.USE",  id);
			p_io_para->Get__PARAMETER_DATA(para_name, para_data);
			sCH__DNET_CFG__SLAVE_X__IN_SIZE_USE[i]->Set__DATA(para_data);

			//
			para_name.Format("NODE.%1d.OUT_OFFSET", id);
			p_io_para->Get__PARAMETER_DATA(para_name, para_data);
			sCH__DNET_CFG__SLAVE_X__OUT_OFFSET[i]->Set__DATA(para_data);

			para_name.Format("NODE.%1d.OUT_BYTE.MAX", id);
			p_io_para->Get__PARAMETER_DATA(para_name, para_data);
			sCH__DNET_CFG__SLAVE_X__OUT_SIZE_MAX[i]->Set__DATA(para_data);

			para_name.Format("NODE.%1d.OUT_BYTE.USE", id);
			p_io_para->Get__PARAMETER_DATA(para_name, para_data);
			sCH__DNET_CFG__SLAVE_X__OUT_SIZE_USE[i]->Set__DATA(para_data);
		}
	}

	// ...
	{
		CString ch_data;
		sCH__DNET_CFG__SLAVE_COUNT->Get__DATA(ch_data);

		int i_limit = atoi(ch_data);
		int i;

		for(i=0; i<i_limit; i++)
		{
			CInfo__DNet_Node node_info;

			node_info.iNODE_ID = i + 1;

			sCH__DNET_CFG__SLAVE_X__MACID[i]->Get__DATA(ch_data);
			node_info.iMAC_ID = atoi(ch_data);

			sCH__DNET_CFG__SLAVE_X__NAME[i]->Get__DATA(ch_data);
			node_info.sNAME = ch_data;

			//
			sCH__DNET_CFG__SLAVE_X__IN_OFFSET[i]->Get__DATA(ch_data);
			node_info.iIN_OFFSET = atoi(ch_data);

			sCH__DNET_CFG__SLAVE_X__IN_SIZE_MAX[i]->Get__DATA(ch_data);
			node_info.iIN_SIZE = atoi(ch_data);

			//
			sCH__DNET_CFG__SLAVE_X__OUT_OFFSET[i]->Get__DATA(ch_data);
			node_info.iOUT_OFFSET = atoi(ch_data);

			sCH__DNET_CFG__SLAVE_X__OUT_SIZE_MAX[i]->Get__DATA(ch_data);
			node_info.iOUT_SIZE = atoi(ch_data);

			mCtrl__DNet_Node.Load__Node_Info(&node_info);
		}

		mCtrl__DNet_Node.Load__Complete();

		// ...
		{
			CString str_node_info = mCtrl__DNet_Node.Get__All_Node_Info();

			xI__APP_LOG_CTRL->WRITE__LOG(str_node_info);
			xI__DRV_LOG_CTRL->WRITE__LOG(str_node_info);
		}
	}

	printf("==================================================================== \n");
	printf("%s : Initial Starting ... \n", sObject_Name);

	if(iActive__SIM_MODE > 0)
	{
		diCH__COMM_STS->Set__DATA(STR__ONLINE);

		printf("%s : Simulation.Initial Completed !!! \n", sObject_Name);

		//
		CString ch_data;

		usOutputOffset = mCtrl__DNet_Node.Get__TOTAL_OUT_BYTE();
		usInputOffset  = mCtrl__DNet_Node.Get__TOTAL_IN_BYTE();
	}
	else
	{
		int r_ret = _Init__DNET_MASTER_BY_USER_CFG();

		if(r_ret < 0)
		{
			diCH__COMM_STS->Set__DATA(STR__OFFLINE);

			printf("%s : Initial Aborted ... \n", sObject_Name);

			//
			usInputOffset  = 0;
			usOutputOffset = 0;
		}
		else
		{
			diCH__COMM_STS->Set__DATA(STR__ONLINE);

			printf("%s : Initial Completed ... \n", sObject_Name);

			//
			CString ch_data;

			usOutputOffset = mCtrl__DNet_Node.Get__TOTAL_OUT_BYTE();
			usInputOffset  = mCtrl__DNet_Node.Get__TOTAL_IN_BYTE();
		}
	}
	printf("==================================================================== \n");

	// ...
	{
		CString ch_data;

		// ...
		{
			_Check__CFG_SLAVE_INFO();

			//
			ch_data.Format("%1d", usOutputOffset);
			sCH__DNET_CFG__TOTAL_OUT_BYTE_MAX->Set__DATA(ch_data);

			ch_data.Format("%1d", usInputOffset);
			sCH__DNET_CFG__TOTAL_IN_BYTE_MAX->Set__DATA(ch_data);
		}

		// ...
		iDNet_BoardNumber = mDNet_Mng.Get__BOARD_ID();
		ch_data.Format("%1d", iDNet_BoardNumber);
		sCH__DNET_INFO__MASTER_BOARD_ID->Set__DATA(ch_data);

		ch_data.Format("%1d", iDNet_Board_In_Offset);
		sCH__DNET_INFO__MASTER_BOARD_IN_BYTE_OFFSET->Set__DATA(ch_data);

		ch_data.Format("%1d", iDNet_Board_Out_Offset);
		sCH__DNET_INFO__MASTER_BOARD_OUT_BYTE_OFFSET->Set__DATA(ch_data);

		//
		if(iActive__SIM_MODE > 0)		ch_data = "DNM   12345 67890  ";
		else							ch_data = mDNet_Mng.Get__FIRMWARE_INFO();
		
		int s_index = ch_data.Find("DNM");
		if(s_index >= 0)		ch_data.Delete(0, s_index+3);

		ch_data.TrimLeft();
		ch_data.TrimRight();
			
		sCH__DNET_INFO__MASTER_BOARD_NAME->Set__DATA(ch_data);

		//
		if(iActive__SIM_MODE > 0)
		{
			sCH__DNET_INFO__MASTER_BOARD_DRIVER_VERSION->Set__DATA("???");
		}
		else
		{
			ch_data = mDNet_Mng.Get__DRIVER_VERSION();
			sCH__DNET_INFO__MASTER_BOARD_DRIVER_VERSION->Set__DATA(ch_data);
		}
	}

	// ...
	{
		_Fnc__DEV_INFO( true );
		_Check__DEV_TOTAL_MEMORY( );
	}
	return 1;
}


//--------------------------------------------------------------------------------
int CObj__DNET_STD
::__CALL__CONTROL_MODE(mode, p_debug, p_variable, p_alarm)
{
	CString msg;
	int flag = -1;

	// ...
	{
		msg.Format("%s Started...",mode);
		sCH__OBJ_MSG->Set__DATA(msg);

		Write__APP_LOG(msg);
	}

	// ...
	{
			 IF__CTRL_MODE(sMODE__INIT)							flag = Call__INIT(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__LINK_IO_SET_OFF)				flag = Call__LINK_IO_SET_OFF(p_variable, p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__LINK_IO_SET_ON)				flag = Call__LINK_IO_SET_ON(p_variable, p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__DEV_INFO)						flag = Call__DEV_INFO(p_variable, p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__FLOAT_TO_HEXA)				flag = Call__FLOAT_TO_HEXA(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__HEXA_TO_FLOAT)				flag = Call__HEXA_TO_FLOAT(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__NODE_OUT_BYTE_UPLOAD)			flag = Call__NODE_OUT_BYTE_UPLOAD(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__NODE_IN_BYTE_UPLOAD)			flag = Call__NODE_IN_BYTE_UPLOAD(p_variable,p_alarm);
	}

	if((flag < 0)||(p_variable->Check__CTRL_ABORT() > 0))
	{
		msg.Format("%s Aborted (%1d)", mode, flag);
	}
	else
	{
		msg.Format("%s Completed (%1d)", mode, flag);
	}	

	// ...
	{
		sCH__OBJ_MSG->Set__DATA(msg);

		Write__APP_LOG(msg);
	}
	return flag;
}

int CObj__DNET_STD::__CALL__MONITORING(id, p_variable, p_alarm)
{
	switch(id)
	{
		case MON_ID__MONITOR:
			Mon__MONITOR(p_variable,p_alarm);
			break;
	}

	return 1;
}


//--------------------------------------------------------------------------------
int CObj__DNET_STD
::__CLOSE__OBJECT()
{
	EnterCriticalSection(&mLOCK_DNET);

	if(iDNet_BoardNumber >= 0)
	{
		DevExitBoard(iDNet_BoardNumber);
		DevCloseDriver(0);

		iDNet_BoardNumber = -1;
	}

	LeaveCriticalSection(&mLOCK_DNET);
	return 1;
}
