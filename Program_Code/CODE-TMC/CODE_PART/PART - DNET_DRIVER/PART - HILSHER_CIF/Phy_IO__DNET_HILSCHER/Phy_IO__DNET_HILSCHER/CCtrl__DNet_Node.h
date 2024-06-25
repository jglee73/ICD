#pragma once


#define CFG__DNET_NODE_DATA_IN_BYTE_MAX				300
#define CFG__DNET_NODE_DATA_OUT_BYTE_MAX			300


// ...
/*
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
*/

// ...
class CHeader__DNet_Node
{
public:
	int iNODE_ID;

	int iMAC_ID;
	CString sNAME;

	int iIN_OFFSET;
	int iIN_SIZE;

	int iOUT_OFFSET;
	int iOUT_SIZE;

public:
	CHeader__DNet_Node()
	{
		iNODE_ID = -1;

		iMAC_ID = -1;
		sNAME   = "";

		iIN_OFFSET  = 0;
		iIN_SIZE    = 0;

		iOUT_OFFSET = 0;
		iOUT_SIZE   = 0;
	}

	void Copy_From(CHeader__DNet_Node* p_info)
	{
		iNODE_ID = p_info->iNODE_ID;

		iMAC_ID = p_info->iMAC_ID;
		sNAME   = p_info->sNAME;

		iIN_OFFSET  = p_info->iIN_OFFSET;
		iIN_SIZE    = p_info->iIN_SIZE;

		iOUT_OFFSET = p_info->iOUT_OFFSET;
		iOUT_SIZE   = p_info->iOUT_SIZE;
	}
};

class CInfo__DNet_Node : public CHeader__DNet_Node
{
public:
	int iDATA__PRE_IN_BYTE;
	int iDATA__IN_INDEX;
	
	int iDATA__PRE_OUT_BYTE;
	int iDATA__OUT_INDEX;

	unsigned char usDATA__IN_BYTE[CFG__DNET_NODE_DATA_IN_BYTE_MAX+1];
	unsigned char usDATA__OUT_BYTE[CFG__DNET_NODE_DATA_OUT_BYTE_MAX+1];

public:
	CInfo__DNet_Node()
	{
		iDATA__PRE_IN_BYTE  = 0;
		iDATA__PRE_OUT_BYTE = 0;

		iDATA__IN_INDEX  = -1;
		iDATA__OUT_INDEX = -1;

		memset(usDATA__IN_BYTE,  0, CFG__DNET_NODE_DATA_IN_BYTE_MAX+1);
		memset(usDATA__OUT_BYTE, 0, CFG__DNET_NODE_DATA_OUT_BYTE_MAX+1);
	}
};


class CCtrl__DNet_Node
{
private:
	CRITICAL_SECTION mLOCK;

	CPtrArray pList_Node;
	int iSize__OUT_BYTE;
	int iSize__IN_BYTE;

	int _Get_MacID_Of_Node_ID(const int node_id);

	int _Get__IN_BYTE_INDEX_OF_MACID(const int mac_id);
	int _Get__OUT_BYTE_INDEX_OF_MACID(const int mac_id);

	CInfo__DNet_Node* _Get__NODE_INFO(const int mac_id);


public:
	CCtrl__DNet_Node();
	~CCtrl__DNet_Node();

	// ...
	int Load__Node_Info(CHeader__DNet_Node* p_info);
	int Load__Complete();

	CString Get__All_Node_Info();

	// ...
	int Get_MacID_Of_Node_ID(const int node_id);

	int Get__IN_BYTE_INDEX_OF_MACID(const int mac_id);
	int Get__OUT_BYTE_INDEX_OF_MACID(const int mac_id);

	// ...
	int Set__OUT_BYTE__IO_BIT(const int md_id,const int ch_id, const int s_data);
	int Get__IN_BYTE__IO_BIT(const int md_id,const int ch_id, int& s_data);

	// ...
	int Set__OUT_BYTE__IO_BYTE(const int md_id,const int ch_id,const int byte_offset,const int byte_size, 
						       unsigned char *p_uch_data);
	int Get__IN_BYTE__IO_BYTE(const int md_id,const int ch_id,const int byte_offset,const int byte_size, 
							  unsigned char *p_uch_data,const int bff_size);

	// ...
	int Get__TOTAL_OUT_BYTE();
	int Get__TOTAL_IN_BYTE();

	int Update__ALL_OUT_BYTE(unsigned char* p_data,const unsigned int data_size);
	int Update__ALL_IN_BYTE(unsigned char* p_data,const unsigned int data_size);
};
