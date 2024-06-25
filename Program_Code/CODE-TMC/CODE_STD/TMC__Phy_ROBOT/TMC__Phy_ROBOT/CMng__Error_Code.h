#pragma once


class CMng__Error_Code
{
private:
	CRITICAL_SECTION mLOCK;

	CStringArray sList__ERR_CODE;
	CStringArray sList__ERR_MSG;

public:
	CMng__Error_Code()
	{
		InitializeCriticalSection(&mLOCK);

	}
	~CMng__Error_Code()
	{

		DeleteCriticalSection(&mLOCK);
	}

	void Clear__Error_Code()
	{
		EnterCriticalSection(&mLOCK);

		sList__ERR_CODE.RemoveAll();
		sList__ERR_MSG.RemoveAll();

		LeaveCriticalSection(&mLOCK);
	}
	void Load__Error_Code(const CString& err_code,const CString& err_msg)
	{
		EnterCriticalSection(&mLOCK);

		sList__ERR_CODE.Add(err_code);
		sList__ERR_MSG.Add(err_msg);

		if(sList__ERR_CODE.GetSize() > 10)
		{
			sList__ERR_CODE.RemoveAt(0);
			sList__ERR_MSG.RemoveAt(0);
		}

		LeaveCriticalSection(&mLOCK);
	}
	int  Get__Error_Code(CString& err_code,CString& err_msg)
	{
		EnterCriticalSection(&mLOCK);

		int r_res = -1;

		if(sList__ERR_CODE.GetSize() > 0)
		{
			r_res = 1;

			err_code = sList__ERR_CODE[0];
			err_msg  = sList__ERR_MSG[0];

			sList__ERR_CODE.RemoveAt(0);
			sList__ERR_MSG.RemoveAt(0);
		}

		LeaveCriticalSection(&mLOCK);
		return r_res;
	}
};
