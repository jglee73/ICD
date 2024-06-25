#include "stdafx.h"
#include "CCtrl__DNet_Node.h"


// ...
CCtrl__DNet_Node::CCtrl__DNet_Node()
{
	InitializeCriticalSection(&mLOCK);

	iSize__IN_BYTE  = 0;
	iSize__OUT_BYTE = 0;
}
CCtrl__DNet_Node::~CCtrl__DNet_Node()
{

	DeleteCriticalSection(&mLOCK);
}

// ...
int CCtrl__DNet_Node::Load__Node_Info(CHeader__DNet_Node* p_info)
{
	EnterCriticalSection(&mLOCK);

	// ...
	{
		int check_node = -1;

		int i_limit = pList_Node.GetSize();
		int i;

		for(i=0; i<i_limit; i++)
		{
			CInfo__DNet_Node *p_node = (CInfo__DNet_Node*) pList_Node[i];

			if(p_node->iMAC_ID != p_info->iMAC_ID)
			{
				continue;
			}

			check_node = 1;
			break;
		}

		if(check_node < 0)
		{
			CInfo__DNet_Node *p_node = new CInfo__DNet_Node;
			pList_Node.Add(p_node);

			p_node->Copy_From(p_info);

			// ...
			int mac_id = p_node->iMAC_ID;

			p_node->iDATA__IN_INDEX  = _Get__IN_BYTE_INDEX_OF_MACID(mac_id);
			p_node->iDATA__OUT_INDEX = _Get__OUT_BYTE_INDEX_OF_MACID(mac_id);
		}
	}

	LeaveCriticalSection(&mLOCK);
	return 1;
}
int CCtrl__DNet_Node::Load__Complete()
{
	EnterCriticalSection(&mLOCK);

	// ...
	{
		iSize__IN_BYTE  = 0;
		iSize__OUT_BYTE = 0;
	}

	// ...
	{
		int i_limit = pList_Node.GetSize();
		int i;

		for(i=0; i<i_limit; i++)
		{
			CInfo__DNet_Node *p_node = (CInfo__DNet_Node*) pList_Node[i];

			p_node->iDATA__PRE_IN_BYTE  = iSize__IN_BYTE;
			p_node->iDATA__PRE_OUT_BYTE = iSize__OUT_BYTE;

			iSize__IN_BYTE  += p_node->iIN_SIZE;
			iSize__OUT_BYTE += p_node->iOUT_SIZE;
		}
	}

	LeaveCriticalSection(&mLOCK);
	return 1;
}

// ...
int CCtrl__DNet_Node::Get__Node_Size()
{
	EnterCriticalSection(&mLOCK);

	int node_size = pList_Node.GetSize();

	LeaveCriticalSection(&mLOCK);
	return node_size;
}
CInfo__DNet_Node* CCtrl__DNet_Node::Get__Node_Address(const int node_index)
{
	EnterCriticalSection(&mLOCK);

	CInfo__DNet_Node *p_node = NULL;

	if((node_index >= 0) && (node_index < pList_Node.GetSize()))
	{
		p_node = (CInfo__DNet_Node*) pList_Node[node_index];
	}

	LeaveCriticalSection(&mLOCK);
	return p_node;
}

// ...
int CCtrl__DNet_Node::Get_MacID_Of_Node_ID(const int node_id)
{
	EnterCriticalSection(&mLOCK);

	int mac_id = _Get_MacID_Of_Node_ID(node_id);

	LeaveCriticalSection(&mLOCK);
	return mac_id;
}
int CCtrl__DNet_Node::_Get_MacID_Of_Node_ID(const int node_id)
{
	int i_limit = pList_Node.GetSize();
	int i;

	for(i=0; i<i_limit; i++)
	{
		CInfo__DNet_Node *p_node = (CInfo__DNet_Node*) pList_Node[i];

		if(node_id != p_node->iNODE_ID)
		{
			continue;
		}

		return p_node->iMAC_ID;
	}
	return -1;
}

CString CCtrl__DNet_Node::Get__All_Node_Info()
{
	EnterCriticalSection(&mLOCK);

	CString str_info = "\n";
	CUIntArray l_macid;

	// ...
	{
		CString str_bff;

		int i_limit = pList_Node.GetSize();
		int i;

		str_info += "All Node Info .. \n";

		for(i=0; i<i_limit; i++)
		{
			CInfo__DNet_Node *p_node = (CInfo__DNet_Node*) pList_Node[i];

			l_macid.Add(p_node->iMAC_ID);

			//
			str_bff.Format("%02d) ... \n", i+1);
			str_info += str_bff;
			
			str_bff.Format("     MAC_ID : %1d \n", p_node->iMAC_ID);
			str_info += str_bff;

			str_bff.Format("     NAME : %s \n", p_node->sNAME);
			str_info += str_bff;

			str_bff.Format("     IN_OFFSET : %1d \n", p_node->iIN_OFFSET);
			str_info += str_bff;

			str_bff.Format("     IN_SIZE : %1d \n", p_node->iIN_SIZE);
			str_info += str_bff;

			str_bff.Format("     OUT_OFFSET : %1d \n", p_node->iOUT_OFFSET);
			str_info += str_bff;

			str_bff.Format("     OUT_SIZE : %1d \n", p_node->iOUT_SIZE);
			str_info += str_bff;

			str_bff.Format("     DATA__IN_INDEX  : %1d \n", p_node->iDATA__IN_INDEX);
			str_info += str_bff;

			str_bff.Format("     DATA__OUT_INDEX : %1d \n", p_node->iDATA__OUT_INDEX);
			str_info += str_bff;

			str_bff.Format("     DATA__PRE_IN_BYTE : %1d \n", p_node->iDATA__PRE_IN_BYTE);
			str_info += str_bff;

			str_bff.Format("     DATA__PRE_OUT_BYTE : %1d \n", p_node->iDATA__PRE_OUT_BYTE);
			str_info += str_bff;
		}

		str_info += "\n";
	}

	// ...
	{
		CString str_bff;

		str_info += "MacID Offset ... \n";

		int i_limit = l_macid.GetSize();;
		int i;

		for(i=0; i<i_limit; i++)
		{
			int mac_id = l_macid[i];

			int in_index  = _Get__IN_BYTE_INDEX_OF_MACID(mac_id);
			int out_index = _Get__OUT_BYTE_INDEX_OF_MACID(mac_id);

			//
			str_bff.Format("%02d)  MacID(%3d)  IN_Index(%3d)  OUT_Index(%3d) \n", 
				           i+1, mac_id, in_index, out_index);
			str_info += str_bff;
		}

		// ...
		{
			str_bff.Format("  *** Total IN_SIZE  : %1d \n", iSize__IN_BYTE);
			str_info += str_bff;
			
			str_bff.Format("  *** Total OUT_SIZE : %1d \n", iSize__OUT_BYTE);
			str_info += str_bff;
		}

		str_info += "\n";
	}

	LeaveCriticalSection(&mLOCK);
	return str_info;
}

// ...
int CCtrl__DNet_Node::Get__IN_BYTE_INDEX_OF_MACID(const int mac_id)
{
	EnterCriticalSection(&mLOCK);

	int s_index = _Get__IN_BYTE_INDEX_OF_MACID(mac_id);

	LeaveCriticalSection(&mLOCK);
	return s_index;
}
int CCtrl__DNet_Node::_Get__IN_BYTE_INDEX_OF_MACID(const int mac_id)
{
	int s_index = -1;

	// ...
	{
		int s_offset = 0;

		int i_limit = pList_Node.GetSize();
		int i;

		for(i=0; i<i_limit; i++)
		{
			CInfo__DNet_Node *p_node = (CInfo__DNet_Node*) pList_Node[i];

			if(p_node->iMAC_ID != mac_id)
			{
				s_offset += p_node->iIN_SIZE;
				continue;
			}

			s_index = s_offset + p_node->iIN_OFFSET;
			break;
		}
	}

	return s_index;
}

// ...
int CCtrl__DNet_Node::Get__OUT_BYTE_INDEX_OF_MACID(const int mac_id)
{
	EnterCriticalSection(&mLOCK);

	int s_index = _Get__OUT_BYTE_INDEX_OF_MACID(mac_id);

	LeaveCriticalSection(&mLOCK);
	return s_index;
}
int CCtrl__DNet_Node::_Get__OUT_BYTE_INDEX_OF_MACID(const int mac_id)
{
	int s_index = -1;

	// ...
	{
		int s_offset = 0;

		int i_limit = pList_Node.GetSize();
		int i;

		for(i=0; i<i_limit; i++)
		{
			CInfo__DNet_Node *p_node = (CInfo__DNet_Node*) pList_Node[i];

			if(p_node->iMAC_ID != mac_id)
			{
				s_offset += p_node->iOUT_SIZE;
				continue;
			}

			s_index = s_offset + p_node->iOUT_OFFSET;
			break;
		}
	}

	return s_index;
}

CInfo__DNet_Node* CCtrl__DNet_Node::_Get__NODE_INFO(const int mac_id)
{
	CInfo__DNet_Node* p_addr = NULL;

	// ...
	{
		int i_limit = pList_Node.GetSize();
		int i;

		for(i=0; i<i_limit; i++)
		{
			CInfo__DNet_Node *p_node = (CInfo__DNet_Node*) pList_Node[i];

			if(p_node->iMAC_ID != mac_id)
			{
				continue;
			}

			p_addr = p_node;
			break;
		}
	}

	return p_addr;
}

// ...
int CCtrl__DNet_Node::Set__OUT_BYTE__IO_BIT(const int md_id,const int ch_id, const int s_data)
{
	EnterCriticalSection(&mLOCK);

	CInfo__DNet_Node* p_node = _Get__NODE_INFO(md_id);
	int r_ret = -1;

	if(p_node != NULL)
	{
		int io_index = (ch_id / 8) + p_node->iOUT_OFFSET;
		int io_bit   = ch_id % 8;

		if((io_index >= 0) && (io_index < CFG__DNET_NODE_DATA_OUT_BYTE_MAX))
		{
			unsigned char io_data = p_node->usDATA__OUT_BYTE[io_index];
			unsigned char io_set  = 0x01;

			io_set <<= io_bit;

			if(s_data > 0)
			{
				io_data |= io_set;
			}
			else
			{
				unsigned char cur_bit = 0x0ff & (io_data & io_set);

				if(cur_bit != 0)		io_data ^= io_set;
			}

			p_node->usDATA__OUT_BYTE[io_index] = io_data;

			r_ret = 1;
		}
	}

	LeaveCriticalSection(&mLOCK);
	return r_ret;
}
int CCtrl__DNet_Node::Get__IN_BYTE__IO_BIT(const int md_id,const int ch_id, int& r_data)
{
	EnterCriticalSection(&mLOCK);

	CInfo__DNet_Node* p_node = _Get__NODE_INFO(md_id);
	int r_ret = -1;

	if(p_node != NULL)
	{
		int io_index = (ch_id / 8) + p_node->iIN_OFFSET;
		int io_bit   = ch_id % 8;

		if((io_index >= 0) && (io_index < CFG__DNET_NODE_DATA_IN_BYTE_MAX))
		{
			unsigned char io_data = p_node->usDATA__IN_BYTE[io_index];
			unsigned char io_mask = 0x01;

			io_mask <<= io_bit;

			int check_bit = 0x0ff & (io_data & io_mask);

			if(check_bit > 0)		r_data = 1;
			else					r_data = 0;

			r_ret = 1;
		}
	}

	LeaveCriticalSection(&mLOCK);
	return r_ret;
}

// ...
int CCtrl__DNet_Node
::Set__OUT_BYTE__IO_BYTE(const int md_id,const int ch_id,const int byte_offset,const int byte_size,
						 unsigned char *p_uch_data)
{
	EnterCriticalSection(&mLOCK);

	CInfo__DNet_Node* p_node = _Get__NODE_INFO(md_id);
	int r_ret = -1;

	if(p_node != NULL)
	{
		int io_index = byte_offset + p_node->iOUT_OFFSET;

		if((io_index >= 0) && (io_index < (CFG__DNET_NODE_DATA_OUT_BYTE_MAX - byte_size)))
		{
			int i_limit = byte_size;
			int i;

			for(i=0; i<i_limit; i++)
			{
				p_node->usDATA__OUT_BYTE[io_index + i] = 0x0ff & p_uch_data[i];
			}

			r_ret = 1;
		}
	}

	LeaveCriticalSection(&mLOCK);
	return r_ret;
}
int CCtrl__DNet_Node
::Get__IN_BYTE__IO_BYTE(const int md_id,const int ch_id,const int byte_offset,const int byte_size, 
						unsigned char *p_uch_data,const int bff_size)
{
	EnterCriticalSection(&mLOCK);

	CInfo__DNet_Node* p_node = _Get__NODE_INFO(md_id);
	int r_ret = -1;

	if(p_node != NULL)
	{
		int io_index = byte_offset + p_node->iIN_OFFSET;

		if((io_index >= 0) && (io_index < (CFG__DNET_NODE_DATA_IN_BYTE_MAX - byte_size)))
		{
			int i_limit = byte_size;
			int i;

			if(i_limit > bff_size)		i_limit = bff_size;

			for(i=0; i<i_limit; i++)
			{
				p_uch_data[i] = 0x0ff & p_node->usDATA__IN_BYTE[io_index + i];
			}

			r_ret = 1;
		}
	}

	LeaveCriticalSection(&mLOCK);
	return r_ret;
}

// ...
int CCtrl__DNet_Node::Get__TOTAL_OUT_BYTE()
{
	return iSize__OUT_BYTE;
}
int CCtrl__DNet_Node::Get__TOTAL_IN_BYTE()
{
	return iSize__IN_BYTE;
}

int CCtrl__DNet_Node::Update__ALL_OUT_BYTE(unsigned char* p_data,const unsigned int data_size)
{
	EnterCriticalSection(&mLOCK);

	int out_size = iSize__OUT_BYTE;

	// ...
	{
		int n_limit = pList_Node.GetSize();
		int n;

		for(n=0; n<n_limit; n++)
		{
			CInfo__DNet_Node *p_node = (CInfo__DNet_Node*) pList_Node[n];

			// ...
			{
				int io_index = p_node->iDATA__OUT_INDEX;
				int i_limit = p_node->iOUT_SIZE;
				int s_i = p_node->iOUT_OFFSET;

				for( ; s_i<i_limit; s_i++)
				{	
					if(io_index < data_size)
					{
						p_data[io_index++] = 0x0ff & p_node->usDATA__OUT_BYTE[s_i];
					}
				}
			}
		}
	}

	LeaveCriticalSection(&mLOCK);
	return out_size;
}
int CCtrl__DNet_Node::Update__ALL_IN_BYTE(unsigned char* p_data,const unsigned int data_size)
{
	EnterCriticalSection(&mLOCK);

	int in_size = iSize__IN_BYTE;

	// ...
	{
		int n_limit = pList_Node.GetSize();
		int n;

		for(n=0; n<n_limit; n++)
		{
			CInfo__DNet_Node *p_node = (CInfo__DNet_Node*) pList_Node[n];

			int io_index = p_node->iDATA__PRE_IN_BYTE;
			int i_limit = p_node->iIN_SIZE;

			for(int s_i=0; s_i<i_limit; s_i++)
			{	
				if(io_index < data_size)
				{
					p_node->usDATA__IN_BYTE[s_i] = 0x0ff & p_data[io_index++];
				}
			}
		}
	}

	LeaveCriticalSection(&mLOCK);
	return in_size;
}
