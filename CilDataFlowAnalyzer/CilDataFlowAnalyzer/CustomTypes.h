#pragma once

#include "cor.h"
#include "corprof.h"
#include "PlatformHeaders.h"
#include <vector>
#include <list>
#include <string>
#include <map>

#if !defined(_CUSTOM_TYPES_DEFINED)
typedef std::vector<WORD> WordList;
typedef std::list<WSTRING> WStringList;
typedef std::list<BYTE> ByteList;
typedef std::map<ModuleID, WSTRING&> WStringByModuleIdMap;
typedef std::map<WSTRING, WORD> LabelToLabelAliasMap;
typedef std::map<WSTRING, DWORD> LabelToLocationMap;
typedef std::map<WORD, WSTRING> LabelAliasToLabelMap;
#define _CUSTOM_TYPES_DEFINED
#endif
