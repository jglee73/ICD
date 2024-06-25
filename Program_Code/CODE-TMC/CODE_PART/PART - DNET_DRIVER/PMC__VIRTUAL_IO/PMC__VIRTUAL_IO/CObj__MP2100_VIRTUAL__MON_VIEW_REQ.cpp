#include "StdAfx.h"
#include "CObj__MP2100_VIRTUAL.h"


#define STR__YES					"YES"
#define STR__ON						"ON"


// ...
int CObj__MP2100_VIRTUAL
::Mon__VIEW_REQ(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm)
{

	while(1)
	{
		Sleep(500);

		if(sCH__MON_IO_WARNING_VIEW_REQ->Check__DATA(STR__YES) > 0)
		{
			// ...
			{
				CString str_code = sCH__MON_IO_WARNING_DATA_HEXA->Get__STRING();
				DWORD err_code = atoi(str_code);

				CString str_msg;
				CString r_act;

				str_msg.Format("Warning [%s] \n", str_code);
				str_msg += Get__WARNING_OF_MONITORING_PARAMETER(err_code);

				CStringArray l_act;
				l_act.Add("CHECK");

				p_alarm->Popup__MESSAGE_BOX("Register Warning Check",str_msg,l_act,r_act);
			}

			sCH__MON_IO_WARNING_VIEW_REQ->Set__DATA("");
		}
		if(sCH__MON_IO_ALARM_VIEW_REQ->Check__DATA(STR__YES) > 0)
		{
			// ...
			{
				CString str_code = sCH__MON_IO_ALARM_DATA_HEXA->Get__STRING();

				DWORD err_code = atoi(str_code);

				CString str_msg;
				CString r_act;

				str_msg.Format("Alarm [%s] \n", str_code);
				str_msg += Get__ALARM_OF_MONITORING_PARAMETER(err_code);

				CStringArray l_act;
				l_act.Add("CHECK");

				p_alarm->Popup__MESSAGE_BOX("Register Alarm Check",str_msg,l_act,r_act);
			}

			sCH__MON_IO_ALARM_VIEW_REQ->Set__DATA("");
		}
		if(sCH__MON_IO_LAST_ERROR_VIEW_REQ->Check__DATA(STR__YES) > 0)
		{

			sCH__MON_IO_LAST_ERROR_VIEW_REQ->Set__DATA("");
		}

		if(sCH__CMMD_REQ_SAVE->Check__DATA("") < 0)
		{
			CString str_cmmd = sCH__CMMD_REQ_SAVE->Get__STRING();
			CString str_pos  = sCH__MON_IO_APOS_MM_GET->Get__STRING();

			int save_ok = -1;

			// ...
			{
				CString str_msg;
				CString str_bff;
				CString r_act;

				str_msg.Format("현재 위치 %s(mm)를 \n", str_pos);
				str_bff.Format(" %s에 저장 할까요?", str_cmmd);
				str_msg += str_bff;

				CStringArray l_act;
				l_act.Add(STR__YES);
				l_act.Add(STR__NO);

				p_alarm->Popup__MESSAGE_BOX("Register Alarm Check",str_msg,l_act,r_act);

				if(r_act.CompareNoCase(STR__YES) == 0)
				{
					save_ok = 1;
				}
			}

			if(save_ok > 0)
			{
				if(str_cmmd.CompareNoCase("MOVE.UP") == 0)
				{
					int i = TRANSFER_POS__UP;
					aCH__CFG_TRANSFER_POS_X_DATA_POS[i]->Set__DATA(str_pos);
				}
				else if(str_cmmd.CompareNoCase("MOVE.MIDDLE_UP") == 0)
				{
					int i = TRANSFER_POS__MIDDLE_UP;
					aCH__CFG_TRANSFER_POS_X_DATA_POS[i]->Set__DATA(str_pos);
				}
				else if(str_cmmd.CompareNoCase("MOVE.MIDDLE_DOWN") == 0)
				{
					int i = TRANSFER_POS__MIDDLE_DOWN;
					aCH__CFG_TRANSFER_POS_X_DATA_POS[i]->Set__DATA(str_pos);
				}
				else if(str_cmmd.CompareNoCase("MOVE.DOWN") == 0)
				{
					int i = TRANSFER_POS__DOWN;
					aCH__CFG_TRANSFER_POS_X_DATA_POS[i]->Set__DATA(str_pos);
				}
				else if(str_cmmd.CompareNoCase("PROC.UP") == 0)
				{
					int i = PROC_POS__UP;
					aCH__CFG_PROC_POS_X_DATA_POS[i]->Set__DATA(str_pos);
				}
				else if(str_cmmd.CompareNoCase("PROC.MIDDLE") == 0)
				{
					int i = PROC_POS__MIDDLE;
					aCH__CFG_PROC_POS_X_DATA_POS[i]->Set__DATA(str_pos);
				}
				else if(str_cmmd.CompareNoCase("PROC.DOWN") == 0)
				{
					int i = PROC_POS__DOWN;
					aCH__CFG_PROC_POS_X_DATA_POS[i]->Set__DATA(str_pos);
				}
			}

			sCH__CMMD_REQ_SAVE->Set__DATA("");
		}
	}

	return 1;
}

CString CObj__MP2100_VIRTUAL
::Get__WARNING_OF_MONITORING_PARAMETER(DWORD code_value)
{
	if(code_value == 0x00)
	{
		return "No Warning";
	}

	// ...
	CString str_msg;
	int i;

	for(i=0; i < 32; i++)
	{	
		DWORD check_bit = (code_value >> i) & 0x01;
		if(check_bit == 0x00)
		{
			continue;
		}

		if(i == 0)
		{
			str_msg += "Excessively Following Error";
			str_msg += "\n";
			str_msg += "설정 파라메타의 [Deviation Abnormal Detection Error Level]에서 워닝을 선택하고,";
			str_msg += "\n";
			str_msg += "설정 파라메타 [Deviation Abnormal Detection]을 넘은 경우.";
		}
		if(i == 1)
		{
			str_msg += "Setting Parameter Error";
			str_msg += "\n";
			str_msg += "파라메타의 범위 오버가 검출된 경우.";
			str_msg += "\n";
		}
		if(i == 2)
		{
			str_msg += "Fixed Parameter Error";
			str_msg += "\n";
			str_msg += "파라메타의 범위 오버가 검출된 경우.";
			str_msg += "\n";
		}
		if(i == 3)
		{
			str_msg += "Servo Driver Error";
			str_msg += "\n";
			str_msg += "서보에서 서보팩 측에 경고가 발생한 경우.";
			str_msg += "\n";
		}
		if(i == 4)
		{
			str_msg += "Motion Command Setting Error";
			str_msg += "\n";
			str_msg += "사용할 수 없는 모션 코맨드를 설정했습니다.";
			str_msg += "\n";
		}
		if(i == 5)
		{
			str_msg += "Reserved";
			str_msg += "\n";
		}
		if(i == 6)
		{
			str_msg += "Positive Overtravel";
			str_msg += "\n";
			str_msg += "파라메타의 설정에 의해, 정방향 오버트래블이 무효가 되어있는 상태에서 정방향 오버트래블 신호가 입력된 경우.";
			str_msg += "\n";
		}
		if(i == 7)
		{
			str_msg += "Negative Overtravel";
			str_msg += "\n";
			str_msg += "파라메타의 설정에 의해, 역방향 오버트래블이 무효가 되어있는 상태에서 역방향 오버트래블 신호가 입력된 경우.";
			str_msg += "\n";
		}
		if(i == 8)
		{
			str_msg += "Servo Not On";
			str_msg += "\n";
			str_msg += "파라메타에는 Servo On이 설정되었으나, 실제로는 Servo On이 되지 않은 경우.";
			str_msg += "\n";
		}
		if(i == 9)
		{
			str_msg += "Servo Driver Communication Warining";	
			str_msg += "\n";
			str_msg += "Mechatrolin 통신이상을 1회 검출한 경우.";
			str_msg += "\n";
		}

		return str_msg;
	}

	return "Unknown Warning";
}

CString CObj__MP2100_VIRTUAL
::Get__ALARM_OF_MONITORING_PARAMETER(DWORD code_value)
{
	if(code_value == 0x00)
	{
		return "No Alarm";
	}

	// ...
	CString str_msg;
	int i;

	for(i=0; i < 32; i++)
	{	
		DWORD check_bit = (code_value >> i) & 0x01;
		if(check_bit == 0x00)
		{
			continue;
		}

		if(i == 0)
		{
			str_msg += "Servo Driver Error";
			str_msg += "서보에서 서보팩 측에 알람이 발생한 경우.";
		}
		if(i == 1)
		{
			str_msg += "Positive Overtravel";
			str_msg += "오버트래블 신호가 입력된 상태에서 정방향으로 이동지령을 행했습니다.";
		}
		if(i == 2)
		{
			str_msg += "Negative Overtravel";
			str_msg += "역방향 오버트래블 신호가 입력된 상태에서 역방향으로 이동지령을 행했습니다.";
		}
		if(i == 3)
		{
			str_msg += "Positive Soft Limit";
			str_msg += "축 선택이 유한 길이 축에서 정방향 소프트 리밋이 유효,";
			str_msg += "동시에 원점 복귀 완료상태에 있는 경우에 정방향 소프트 리밋값을 넘는 이동지령이 이루어졌습니다.";
		}
		if(i == 4)
		{
			str_msg += "Negative Soft Limit";
			str_msg += "축 선택이 유한 길이 축에서 역방향 소프트 리밋이 유효,";
			str_msg += "동시에 원점 복귀 완료상태에 있는 경우에 역방향 소프트 리밋값을 넘는 이동지령이 이루어졌습니다.";
		}
		if(i == 5)
		{
			str_msg += "Servo Off";
			str_msg += "Servo Off상태에서 이동계의 모션 코맨드를 지령한 경우.";
		}
		if(i == 6)
		{
			str_msg += "Positioning Time Over";
			str_msg += "분배 완료가 되고 나서 설정 파라메타 [Position Complete Timeout]을 초과하여도 위치결정 완료상태가 되지 않는 경우.";
		}
		if(i == 7)
		{
			str_msg += "Excessive Positioning Moveing Amount";
			str_msg += "위치결정 이동량의 설정범위를 초과한 이동량이 지령된 경우.";
		}
		if(i == 8)
		{
			str_msg += "Excessive Speed";
			str_msg += "설정범위를 초과한 속도 지령이 이루어진 경우.";
		}
		if(i == 9)
		{
			str_msg += "Excessively Following Error";
			str_msg += "설정 파라메타의 [Deviation Abnormal Detection Error Level]에서 알람을 선택하고,";
			str_msg += "위치편차가 설정 파라메타 [Deviation Abnormal Detection]을 넘은 경우.";
		}
		if(i == 10)
		{
			str_msg += "Filter Type Change Error";
			str_msg += "분배 미완료 상태에서 필터 타입의 변경이 이루어진 경우.";
		}
		if(i == 11)
		{
			str_msg += "FilterTime Constant Change Error";
			str_msg += "분배 미완료 상태에서 필터 시정수의 변경이 이루어진 경우.";
		}
		if(i == 12)
		{
			str_msg += "Zero Point Not Set";
			str_msg += "축을 무한 길이 축으로 사용하는 경우에 원점 설정이 이루어지지 않은 상태에서 이동지령을 실행한 경우.";
		}
		if((i >= 13) && (i <= 14))
		{
			str_msg += "Reserved";
			str_msg += "시스템용";
		}
		if(i == 15)
		{
			str_msg += "Servo Driver Synchronization Communication Error";
			str_msg += "Mechatrolink 서보와의 동기통신이상을 검출한 경우.";
		}
		if(i == 16)
		{
			str_msg += "Servo Driver Communication Error";
			str_msg += "Mechatrolink 서보와의 통신이상을 2회 연속하여 검출한 경우.";
		}
		if(i == 17)
		{
			str_msg += "Servo Driver Command Timeout Error";
			str_msg += "Mechatrolink 서보에 지령한 명령이 정구시간 내에 완료하지 않은 경우.";
		}
		if(i == 18)
		{
			str_msg += "ABS Encoder Count Exceeded";
			str_msg += "절대치 인코더의 회전량이 취급할 수 있는 범위를 오버한 경우.";
		}
		if((i >= 19) && (i <= 29))
		{
			str_msg += "Reserved";
		}
		if(i == 30)
		{
			str_msg += "Servopack Motor Type Mismatch";
		}
		if(i == 31)
		{
			str_msg += "Servopack Encoder Type Mismatch";
		}

		return str_msg;
	}

	return "Unknown Alarm";
}
