#pragma once


union UNION_2_BYTE__UINT
{
	unsigned short int uiDATA;
	char		 cBYTE[2];
};
union UNION_3_BYTE__UINT
{
	unsigned int uiDATA;
	char		 cBYTE[3];
};
union UNION_4_BYTE__UINT
{
	unsigned long uiDATA;
	char		 cBYTE[4];
};
union UNION_4_BYTE__FLOAT
{
	float fDATA;
	unsigned long uiDATA;
	char  cBYTE[4];
};


class CCommon_Utility
{
public:
	CCommon_Utility();
	~CCommon_Utility();

	// ...
	bool Check__Link(const CString& str_link);

	// ...
	int Get__StringArrray_From_String(const CString& str_data,const CString& str_sep,CStringArray& l_data);
	int Get__Hexa_From_String(const CString& str_data);

	int Check__Same_String(const CString& str_data, const CStringArray& l_data);
	int Check__Hexa_Type(const CString& str_data);
};
