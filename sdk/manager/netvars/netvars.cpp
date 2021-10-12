#include "pch.h"
#include "includes.h"

NetVars g_NetVars;

uintptr_t NetVars::GetOffset(RecvTable* table, const char* tablename, const char* netvarname) {
	for (int i = 0; i < table->m_nProps; i++) {
		RecvProp prop = table->m_pProps[i];

		if (!_stricmp(prop.m_pVarName, netvarname)) {
			return prop.m_Offset;
		}
		
		if (prop.m_pDataTable) {
			intptr_t offset = GetOffset(prop.m_pDataTable, tablename, netvarname);

			if (offset) {
				return offset + prop.m_Offset;
			}
		}
	}

	return 0;
}

uintptr_t NetVars::GetNetvarOffset(const char* tablename, const char* netvarname, ClientClass* clientclass) {
	ClientClass* currNode = clientclass;

	for (auto currNode = clientclass; currNode; currNode = currNode->m_pNext) {
		if (!_stricmp(tablename, currNode->m_pRecvTable->m_pNetTableName)) {
			return GetOffset(currNode->m_pRecvTable, tablename, netvarname);
		}
	}

	return 0;
}