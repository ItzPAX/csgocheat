#pragma once

// forward declaration
class RecvTable;

class RecvProp {
public:
	const char* m_pVarName;
	void* m_RecvType;
	int			m_Flags;
	int			m_StringBufferSize;
	bool		m_bInsideArray;		// Set to true by the engine if this property sits inside an array.

	// Extra data that certain special property types bind to the property here.
	const void* m_pExtraData;

	// If this is an array (DPT_Array).
	RecvProp* m_pArrayProp;
	void* m_ArrayLengthProxy;

	void* m_ProxyFn;
	void* m_DataTableProxyFn;	// For RDT_DataTable.

	RecvTable* m_pDataTable;		// For RDT_DataTable.
	int			m_Offset;

	int			m_ElementStride;
	int			m_nElements;

	// If it's one of the numbered "000", "001", etc properties in an array, then
	// these can be used to get its array property name for debugging.
	const char* m_pParentArrayPropName;
};

class RecvTable {
public:
	// Properties described in a table.
	RecvProp* m_pProps;
	int			m_nProps;

	// The decoder. NOTE: this covers each RecvTable AND all its children (ie: its children
	// will have their own decoders that include props for all their children).
	void* m_pDecoder;

	const char* m_pNetTableName;	// The name matched between client and server.
	bool		m_bInitialized;
	bool		m_bInMainList;
};

class ClientClass {
public:
	void*			m_pCreateFn;
	void*			m_pCreateEventFn;	// Only called for event objects.
	const char*		m_pNetworkName;
	RecvTable*		m_pRecvTable;
	ClientClass*	m_pNext;
	int				m_ClassID;	// Managed by the engine.
};

class NetVars {
private:
	intptr_t GetOffset(RecvTable* table, const char* tablename, const char* netvarname);
	intptr_t GetNetvarOffset(const char* tablename, const char* netvarname, ClientClass* clientclass); // wrapper for GetOffsetFunction
public:
	template<typename T, typename U>
	T GetNetvar(const char* tablename, const char* netvarname, U base); // function to call to get netvar
};

template<typename T, typename U>
inline T NetVars::GetNetvar(const char* tablename, const char* netvarname, U base) {
	ClientClass* clientclass = g_Interface.pClient->GetAllClasses();
	auto offset = GetNetvarOffset(tablename, netvarname, clientclass);

	T* ret = (T*)(offset + base);
	return *ret;
}

extern NetVars g_NetVars;