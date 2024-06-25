#include "stdafx.h"


bool Macro__Check_Active(const CString& def_data)
{
	if(def_data.CompareNoCase("NO")   == 0)			return false;
	if(def_data.CompareNoCase("NONE") == 0)			return false;
	if(def_data.CompareNoCase("NULL") == 0)			return false;

	return true;
}
