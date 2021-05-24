#pragma once

#include <corhlpr.h>

#include "CilBasicBlockParser.h"
#include "CustomTypes.h"
#include "StackState.h"

class CilDataFlowAnalyzer
{
public:
	CilDataFlowAnalyzer(BYTE* pMethodBytes);
	CilDataFlowAnalyzer(BYTE* pMethodBytes, bool initialize);
	void Init();
	StackState GetStackStatusAtOffset(int offset);

private:
	void ParseMethodBytes();
	void ParseHeader();
	void ParseCode();
	void ParseExtraDataSections();

	bool _isInitialized = false;
	CilBasicBlockParser* _pCilBasicBlockParser = nullptr;
	
	bool _isFat = false;
	
	BYTE* _pMethodBytes = nullptr;
	
	BYTE* _pCodeBytes = nullptr;
	unsigned int _codeSize = 0;
	
	unsigned int _maxStack = 0;
	unsigned int _flags = 0;
	mdToken _localVarSigTok = 0;
	bool _hasMore = false;
	COR_ILMETHOD_SECT* _pExtraDataSections = nullptr;
	ExceptionHandlingClauseMap _clauseMap;
};
