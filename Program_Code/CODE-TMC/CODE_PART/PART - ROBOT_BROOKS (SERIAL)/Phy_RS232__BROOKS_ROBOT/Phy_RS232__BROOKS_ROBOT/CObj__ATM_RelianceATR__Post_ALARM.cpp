#include "StdAfx.h"
#include "CObj__ATM_RelianceATR.h"
#include "CObj__ATM_RelianceATR__DEF.h"
#include "CObj__ATM_RelianceATR__ALID.h"


//------------------------------------------------------------------------------------
int  CObj__ATM_RelianceATR
::Fnc__ROBOT_ALARM_POST(CII_OBJECT__ALARM* p_alarm,
					    const int err_id)
{
	int alarm_id;
	CString err_msg;

	if(err_id != 0)    
	{
		err_msg.Format("Error ID  :  %1d\n",err_id);
		Fnc__APP_LOG(err_msg);

		switch(err_id)
		{
			case 1:
				alarm_id = ALID__ATMRB_1_COMMAND_FAILED;
				break;

			case 2:
				alarm_id = ALID__ATMRB_2_MCC_ERROR;
				break;

			case 210:
				alarm_id =  ALID__ATMRB_210_STATION_ERROR;
				break;

			case 220:
				alarm_id = ALID__ATMRB_220_ERROR;
				break;

			case 221:
				alarm_id = ALID__ATMRB_221_ARM_ERROR;
				break;

			case 222:
				alarm_id = ALID__ATMRB_222_ERROR;
				break;

			case 223:
				alarm_id = ALID__ATMRB_223_ERROR;
				break;

			case 224:
				alarm_id = ALID__ATMRB_224_ERROR;
				break;

			case 230:
				alarm_id = ALID__ATMRB_230_EXTEND_ERROR;
				break;

			case 231:
				alarm_id = ALID__ATMRB_231_EXTEND_ERROR;
				break;

			case 232:
				alarm_id = ALID__ATMRB_232_PNEUMATIC_ERROR;
				break;

			case 233:
				alarm_id = ALID__ATMRB_233_EXTEND_ERROR;
				break;

			case 234:
				alarm_id = ALID__ATMRB_234_VALVE_ERROR;
				break;

			case 301:
				alarm_id = ALID__ATMRB_301_ERROR;
				break;

			case 305:
				alarm_id = ALID__ATMRB_305_UNKNOWN_COMMAND;
				break;

			case 309:
				alarm_id = ALID__ATMRB_309_ERROR;
				break;

			case 350:
				alarm_id = ALID__ATMRB_350_ERROR;
				break;

			case 351:
				alarm_id = ALID__ATMRB_351_ERROR;
				break;

			case 352:
				alarm_id = ALID__ATMRB_352_ERROR;
				break;

			case 353:
				alarm_id = ALID__ATMRB_353_ERROR;
				break;

			case 390:
				alarm_id = ALID__ATMRB_390_ERROR;
				break;

			case 402:
				alarm_id = ALID__ATMRB_402_STATION;
				break;

			case 405:
				alarm_id = ALID__ATMRB_405_STATION;
				break;

			case 416:
				alarm_id = ALID__ATMRB_416_POSITION;
				break;

			case 509:
				alarm_id = ALID__ATMRB_509_ERROR;
				break;

			case 602:
				alarm_id = ALID__ATMRB_602_CDM;
				break;

			case 603:
				alarm_id = ALID__ATMRB_603_CDM;
				break;

			case 604:
				alarm_id = ALID__ATMRB_604_CDM;
				break;

			case 605:
				alarm_id = ALID__ATMRB_605_CDM;
				break;

			case 606:
				alarm_id = ALID__ATMRB_606_CDM;
				break;

			case 610:
				alarm_id = ALID__ATMRB_610_CDM;
				break;

			case 611:
				alarm_id = ALID__ATMRB_611_CDM;
				break;

			case 612:
				alarm_id = ALID__ATMRB_612_CDM;
				break;

			case 654:
				alarm_id = ALID__ATMRB_654_COMMAND;
				break;

			case 659:
				alarm_id = ALID__ATMRB_659_COMMAND;
				break;

			case 700:
				alarm_id = ALID__ATMRB_700_COMMAND;
				break;

			case 711:
				alarm_id = ALID__ATMRB_711_COMMAND;
				break;

			case 712:
				alarm_id = ALID__ATMRB_712_COMMAND;
				break;

			case 714:
				alarm_id = ALID__ATMRB_714_COMMAND;
				break;

			case 715:
				alarm_id = ALID__ATMRB_715_COMMAND;
				break;

			case 721:
				alarm_id = ALID__ATMRB_721_COMMAND;
				break;

			case 722:
				alarm_id = ALID__ATMRB_722_COMMAND;
				break;

			case 730:
				alarm_id = ALID__ATMRB_730_COMMAND;
				break;

			case 731:
				alarm_id = ALID__ATMRB_731_COMMAND;
				break;

			case 732:
				alarm_id = ALID__ATMRB_732_COMMAND;
				break;

			case 733:
				alarm_id = ALID__ATMRB_733_COMMAND;
				break;

			case 740:
				alarm_id = ALID__ATMRB_740_COMMAND;
				break;

			case 741:
				alarm_id = ALID__ATMRB_741_COMMAND;
				break;

			case 742:
				alarm_id = ALID__ATMRB_742_COMMAND;
				break;

			case 743:
				alarm_id = ALID__ATMRB_743_COMMAND;
				break;

			case 760:
				alarm_id = ALID__ATMRB_760_COMMAND;
				break;

			case 1900:
				alarm_id = ALID__ATMRB_1900_COMMAND;
				break;

			case 1903:
				alarm_id = ALID__ATMRB_1903_COMMAND;
				break;

			case 1910:
				alarm_id = ALID__ATMRB_1910_COMMAND;
				break;

			case 1911:
				alarm_id = ALID__ATMRB_1911_COMMAND;
				break;

			case 1912:
				alarm_id = ALID__ATMRB_1912_COMMAND;
				break;

			case 1920:
				alarm_id = ALID__ATMRB_1920_COMMAND;
				break;

			case 10000:
				alarm_id = ALID__ATMRB_10000_ECC;
				break;

			case 10001:
				alarm_id = ALID__ATMRB_10001_SYNC;
				break;

			case 10002:
				alarm_id = ALID__ATMRB_10002_MCC;
				break;

			case 10003: 
				alarm_id = ALID__ATMRB_10003_MCC;
				break;

			case 10004:
				alarm_id = ALID__ATMRB_10004_MCC;
				break;

			case 10005:
				alarm_id = ALID__ATMRB_10005_MCC;
				break;

			case 10006:
				alarm_id = ALID__ATMRB_10006_MCC;
				break;

			case 10007:
				alarm_id = ALID__ATMRB_10007_MCC;
				break;

			case 10008:
				alarm_id = ALID__ATMRB_10008_MCC;
				break;

			case 10009:
				alarm_id = ALID__ATMRB_10009_MCC;
				break;

			case 10010:
				alarm_id = ALID__ATMRB_10010_MCC;
				break;

			case 10011:
				alarm_id = ALID__ATMRB_10011_MOTOR;
				break;

			case 10012:
				alarm_id = ALID__ATMRB_10012_MOTOR;
				break;

			case 10013:
				alarm_id = ALID__ATMRB_10013_MOTOR;
				break;

			case 10014:
				alarm_id = ALID__ATMRB_10014_MOTOR;
				break;

			case 10015:
				alarm_id = ALID__ATMRB_10015_MOTOR;
				break;

			case 10016:
				alarm_id = ALID__ATMRB_10016_MOTOR;
				break;

			case 10017:
				alarm_id = ALID__ATMRB_10017_MOTOR;
				break;

			case 10018:
				alarm_id = ALID__ATMRB_10018_MOTOR;
				break;

			case 10019:
				alarm_id = ALID__ATMRB_10019_MOTOR;
				break;

			case 10020:
				alarm_id = ALID__ATMRB_10020_MOTOR;
				break;

			case 10021:
				alarm_id = ALID__ATMRB_10021_MOTOR;
				break;

			case 10022:
				alarm_id = ALID__ATMRB_10022_ERROR;
				break;

			case 10023:
				alarm_id = ALID__ATMRB_10023_ERROR;
				break;

			case 10024:
				alarm_id = ALID__ATMRB_10024_ERROR;
				break;

			case 10028:
				alarm_id = ALID__ATMRB_10028_ERROR;
				break;

			case 10029:
				alarm_id = ALID__ATMRB_10029_EMO;
				break;

			case 10030:
				alarm_id = ALID__ATMRB_10030_ERROR;
				break;

			case 10031:
				alarm_id = ALID__ATMRB_10031_ERROR;
				break;

			case 10034:
				alarm_id = ALID__ATMRB_10034_ERROR;
				break;

			case 10035:
				alarm_id = ALID__ATMRB_10035_ERROR;
				break;

			case 10036:
				alarm_id = ALID__ATMRB_10036_ERROR;
				break;

			case 10047:
				alarm_id = ALID__ATMRB_10047_ERROR;
				break;

			case 10048:
				alarm_id = ALID__ATMRB_10048_ERROR;
				break;

			case 10049:
				alarm_id = ALID__ATMRB_10049_ERROR;
				break;

			case 10050:
				alarm_id = ALID__ATMRB_10050_ERROR;
				break;

			case 10051:
				alarm_id = ALID__ATMRB_10051_ERROR;
				break;

			case 10052:
				alarm_id = ALID__ATMRB_10052_ERROR;
				break;

			case 10053:
				alarm_id = ALID__ATMRB_10053_ERROR;
				break;

			case 10054:
				alarm_id = ALID__ATMRB_10054_ERROR;
				break;

			case 10055:
				alarm_id = ALID__ATMRB_10055_ERROR;
				break;

			case 10056:
				alarm_id = ALID__ATMRB_10056_ERROR;
				break;

			case 10057:
				alarm_id = ALID__ATMRB_10057_ERROR;
				break;

			case 10058:
				alarm_id = ALID__ATMRB_10058_ERROR;
				break;

			case 10059:
				alarm_id = ALID__ATMRB_10059_ERROR;
				break;

			case 10060:
				alarm_id = ALID__ATMRB_10060_ERROR;
				break;

			case 12012:
				alarm_id = ALID__ATMRB_12012_ERROR;
				break;

			case 13000: 
				alarm_id = ALID__ATMRB_13000_COMMAND;
				break;

			case 13001:
				alarm_id = ALID__ATMRB_13001_COMMAND;
				break;

			case 13002:
				alarm_id = ALID__ATMRB_13002_COMMAND;
				break;

			case 13003:
				alarm_id = ALID__ATMRB_13003_COMMAND;
				break;

			case 13004:
				alarm_id = ALID__ATMRB_13004_COMMAND;
				break;

			case 13005:
				alarm_id = ALID__ATMRB_13005_COMMAND;
				break;

			case 13006:
				alarm_id = ALID__ATMRB_13006_COMMAND;
				break;

			case 13007:
				alarm_id = ALID__ATMRB_13007_COMMAND;
				break;

			case 13008:
				alarm_id = ALID__ATMRB_13008_COMMAND;
				break;

			case 13011:
				alarm_id = ALID__ATMRB_13011_COMMAND;
				break;

			case 20004:
				alarm_id = ALID__ATMRB_20004_ERROR;
				break;

			case 20017:
				alarm_id = ALID__ATMRB_20017_ERROR;
				break;

			case 20030:
				alarm_id = ALID__ATMRB_20030_ERROR;
				break;

			case 20031:
				alarm_id = ALID__ATMRB_20031_ERROR;
				break;

			case 20037:
				alarm_id = ALID__ATMRB_20037_ERROR;
				break;

			case 20041:
				alarm_id = ALID__ATMRB_20041_ERROR;
				break;

			case 20306:
				alarm_id = ALID__ATMRB_20306_ERROR;
				break;

			case 20307:
				alarm_id = ALID__ATMRB_20307_ERROR;
				break;

			case 20309:
				alarm_id = ALID__ATMRB_20309_ERROR;
				break;

			case 20706:
				alarm_id = ALID__ATMRB_20706_ERROR;
				break;

			case 20800:
				alarm_id = ALID__ATMRB_20800_ERROR;
				break;

			case 20801:
				alarm_id =  ALID__ATMRB_20801_ERROR;
				break;

			case 20802:
				alarm_id = ALID__ATMRB_20802_ERROR;
				break;

			case 20803:
				alarm_id = ALID__ATMRB_20803_ERROR;
				break;

			case 20804:
				alarm_id = ALID__ATMRB_20804_ERROR;
				break;

			case 20805:
				alarm_id = ALID__ATMRB_20805_ERROR;
				break;

			case 20806:
				alarm_id = ALID__ATMRB_20806_ERROR;
				break;

			case 20807:
				alarm_id = ALID__ATMRB_20807_ERROR;
				break;

			case 20809: 
				alarm_id = ALID__ATMRB_20809_ERROR;
				break;

			case 20810:
				alarm_id = ALID__ATMRB_20810_ERROR;
				break;

			case 20811:
				alarm_id = ALID__ATMRB_20811_ERROR;
				break;

			case 20812:
				alarm_id = ALID__ATMRB_20812_ERROR;
				break;

			case 20813:
				alarm_id = ALID__ATMRB_20813_ERROR;
				break;

			case 20822:
				alarm_id = ALID__ATMRB_20822_ERROR;
				break;

			case 20823:
				alarm_id = ALID__ATMRB_20823_ERROR;
				break;

			case 20824:
				alarm_id = ALID__ATMRB_20824_ERROR;
				break;

			case 20850:
				alarm_id = ALID__ATMRB_20850_ERROR;
				break;

			case 20851:
				alarm_id = ALID__ATMRB_20851_ERROR;
				break;

			case 20901:
				alarm_id = ALID__ATMRB_20901_ERROR;
				break; 

			default :
				alarm_id = ALID__ATMRB_UNKNOWN_ERROR;
				break;
		}  

		// ...
		{
			CString r_act;

			p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,err_msg,r_act);

			if(r_act.CompareNoCase("ABORT") == 0)		return -1;
			if(r_act.CompareNoCase("RETRY") == 0)		return  0;
		}
		return -1;
	} 

	return 1;
}
