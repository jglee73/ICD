#include "StdAfx.h"
#include "CObj__PMC_STD.h"


// ...
void CObj__PMC_STD
::Mon__MODULE_STATUS(CII_OBJECT__VARIABLE* p_variable,
				     CII_OBJECT__ALARM* p_alarm)
{
	SCX__TIMER_CTRL x_timer_ctrl;
	int i_data = 0;

	CString var_data;

	while(1)
	{
		x_timer_ctrl->WAIT(1.0);

		// ...
		{
			i_data++;
			if(i_data >= 100)		i_data = 1;

			var_data.Format("%1d", i_data);
			sCH__LINK_COUNT_REPORT_SEND->Set__DATA(var_data);
		}
	}
}
