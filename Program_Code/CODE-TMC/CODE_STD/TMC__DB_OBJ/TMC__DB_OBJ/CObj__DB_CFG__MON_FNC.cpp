#include "StdAfx.h"
#include "CObj__DB_CFG.h"


// ...
void CObj__DB_CFG
::Mon__PM_LINK_TEST(CII_OBJECT__VARIABLE* p_variable,
					CII_OBJECT__ALARM* p_alarm)
{
	SCX__SEQ_INFO x_seq_info;
	int sim_mode = x_seq_info->Is__SIMULATION_MODE();

	// ...
	CString ch_data;
	int link_count = 0;


	while(1)
	{
		p_variable->Wait__SINGLE_OBJECT(0.9);


		// ...
		{
			if(sCH__CUR_UTILITY_CATEGORY->Check__DATA("") > 0)
				sCH__CUR_UTILITY_CATEGORY->Set__DATA("IDLE");

			if(sCH__CUR_UTILITY_FNC_NAME->Check__DATA("") > 0)
				sCH__CUR_UTILITY_FNC_NAME->Set__DATA("IDLE");
		}

		if(dCH__CFG_SETUP_TEST_MODE->Check__DATA("ENABLE") > 0)
		{
			// ...
			{
				int alm_id = ALID__SETUP_MODE_ENABLE;
				CString alm_msg;
				CString alm_bff;
				CString r_act;

				alm_bff.Format(" * %s <- %s \n", 
								dCH__CFG_SETUP_TEST_MODE->Get__CHANNEL_NAME(),
								dCH__CFG_SETUP_TEST_MODE->Get__STRING());
				alm_msg += alm_bff;
				
				alm_bff.Format(" * %s <- %s \n",
								dCH__CFG_SETUP_AUTO_VENTING_PUMPING->Get__CHANNEL_NAME(),
								dCH__CFG_SETUP_AUTO_VENTING_PUMPING->Get__STRING());
				alm_msg += alm_bff;

				alm_bff.Format(" * %s <- %s \n",
								dCH__CFG_SETUP_LLx_DOOR_SLOT_VLV_CONTROL->Get__CHANNEL_NAME(),
								dCH__CFG_SETUP_LLx_DOOR_SLOT_VLV_CONTROL->Get__STRING());
				alm_msg += alm_bff;

				p_alarm->Check__ALARM(alm_id, r_act);
				p_alarm->Post__ALARM_With_MESSAGE(alm_id, alm_msg);
			}

			if(dCH__CFG_SETUP_PRESSURE_CHECK->Check__DATA("DISABLE") > 0)
			{
				int alm_id = ALID__SETUP_MODE_PRESSURE_CHECK_DISABLE;
				CString alm_msg;
				CString alm_bff;
				CString r_act;

				alm_bff.Format(" * %s <- %s \n", 
							   dCH__CFG_SETUP_TEST_MODE->Get__CHANNEL_NAME(),
							   dCH__CFG_SETUP_TEST_MODE->Get__STRING());
				alm_msg += alm_bff;

				alm_bff.Format(" * %s <- %s \n", 
							   dCH__CFG_SETUP_PRESSURE_CHECK->Get__CHANNEL_NAME(),
							   dCH__CFG_SETUP_PRESSURE_CHECK->Get__STRING());
				alm_msg += alm_bff;

				p_alarm->Check__ALARM(alm_id, r_act);
				p_alarm->Post__ALARM_With_MESSAGE(alm_id, alm_msg);
			}
		}

		// ...
		{
			if(link_count > 100)
			{
				link_count = 0;
			}
			link_count++;

			for(int i=0; i<CFG_PM_LIMIT; i++)
			{
				ch_data.Format("%1d", link_count);
				sCH__PMC_LINK_TEST_FLAG[i]->Set__DATA(ch_data);
			}
		}

		if(sim_mode > 0)
		{
			int alarm_id;
			CString alm_msg;
			CString r_act;

			// ...
			{
				alarm_id = ALID__SIMULATION_MODE_ACTIVE;
				alm_msg = "";

				p_alarm->Check__ALARM(alarm_id, r_act);
				p_alarm->Post__ALARM_With_MESSAGE(alarm_id, alm_msg);
			}

			if(dCH__SCH_TEST_CFG_PMC_DUMMY_BY_CTC->Check__DATA("YES") > 0)
			{
				alarm_id = ALID__SIMULATION_PMC_DUMMY_MODE_ACTIVE;

				alm_msg.Format(" * \"%s\" <- \"%s\" \n",
							   dCH__SCH_TEST_CFG_PMC_DUMMY_BY_CTC->Get__CHANNEL_NAME(),
							   dCH__SCH_TEST_CFG_PMC_DUMMY_BY_CTC->Get__STRING());

				p_alarm->Check__ALARM(alarm_id, r_act);
				p_alarm->Post__ALARM_With_MESSAGE(alarm_id, alm_msg);
			}
		}
	}
}

