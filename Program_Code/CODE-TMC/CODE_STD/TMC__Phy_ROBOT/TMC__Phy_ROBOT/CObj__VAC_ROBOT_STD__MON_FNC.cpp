#include "StdAfx.h"
#include "CObj__VAC_ROBOT_STD.h"
#include "CObj__VAC_ROBOT_STD__DEF.h"
#include "CObj__VAC_ROBOT_STD__ALID.h"


// ...
void CObj__VAC_ROBOT_STD::
Mon__STATE_MONITOR(CII_OBJECT__VARIABLE* p_variable, 
				   CII_OBJECT__ALARM *p_alarm)
{
	int i;
	int k;

	while(1)
	{
		p_variable->Wait__SINGLE_OBJECT(0.5);

		if(sCH__TAS_RESET_REQ->Check__DATA("") < 0)
		{
			sCH__TAS_RESET_REQ->Set__DATA("");

			for(i=0; i<CFG_ACT__SIZE; i++)
			{
				// ROBOT ...
				{
					sCH__TAS_ACTION_TIME_NOW__RB[i]->Set__DATA("");
					sCH__TAS_ACTION_TIME_MIN__RB[i]->Set__DATA("");
					sCH__TAS_ACTION_TIME_MAX__RB[i]->Set__DATA("");
					sCH__TAS_ACTION_TIME_AVG__RB[i]->Set__DATA("");
					sCH__TAS_ACTION_TIME_AVG_F__RB[i]->Set__DATA("");
					aCH__TAS_ACTION_TIME_CNT__RB[i]->Set__VALUE(1);
				}

				// LLx ...
				if(bActive__LLx_MULTI_SLOT_VALVE)
				{
					for(k=0; k<CFG_LLx__SIZE; k++)
					{
						for(int t=0; t<CFG_LLx__SLOT_SIZE; t++)
						{
							sCH__TAS_ACTION_TIME_NOW__LLx_SLOT[i][k][t]->Set__DATA("");
							sCH__TAS_ACTION_TIME_MIN__LLx_SLOT[i][k][t]->Set__DATA("");
							sCH__TAS_ACTION_TIME_MAX__LLx_SLOT[i][k][t]->Set__DATA("");
							sCH__TAS_ACTION_TIME_AVG__LLx_SLOT[i][k][t]->Set__DATA("");
							sCH__TAS_ACTION_TIME_AVG_F__LLx_SLOT[i][k][t]->Set__DATA("");
							aCH__TAS_ACTION_TIME_CNT__LLx_SLOT[i][k][t]->Set__VALUE(1);
						}
					}
				}
				else
				{
					for(k=0; k<CFG_LLx__SIZE; k++)
					{
						sCH__TAS_ACTION_TIME_NOW__LLx_X[i][k]->Set__DATA("");
						sCH__TAS_ACTION_TIME_MIN__LLx_X[i][k]->Set__DATA("");
						sCH__TAS_ACTION_TIME_MAX__LLx_X[i][k]->Set__DATA("");
						sCH__TAS_ACTION_TIME_AVG__LLx_X[i][k]->Set__DATA("");
						sCH__TAS_ACTION_TIME_AVG_F__LLx_X[i][k]->Set__DATA("");
						aCH__TAS_ACTION_TIME_CNT__LLx_X[i][k]->Set__VALUE(1);
					}
				}

				// PMx ...
				for(k=0; k<CFG_PMx__SIZE; k++)
				{
					sCH__TAS_ACTION_TIME_NOW__PMx[i][k]->Set__DATA("");
					sCH__TAS_ACTION_TIME_MIN__PMx[i][k]->Set__DATA("");
					sCH__TAS_ACTION_TIME_MAX__PMx[i][k]->Set__DATA("");
					sCH__TAS_ACTION_TIME_AVG__PMx[i][k]->Set__DATA("");
					sCH__TAS_ACTION_TIME_AVG_F__PMx[i][k]->Set__DATA("");
					aCH__TAS_ACTION_TIME_CNT__PMx[i][k]->Set__VALUE(1);
				}
			}
		}

		// ...
	}
}
