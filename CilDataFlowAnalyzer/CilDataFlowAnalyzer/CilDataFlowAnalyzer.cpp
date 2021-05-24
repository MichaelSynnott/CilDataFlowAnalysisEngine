#include "CilDataFlowAnalyzer.h"

#include <stdexcept>

#include "CilBasicBlockParser.h"
#include "corhlpr.h"

// PUBLIC -----------------------------------------------------------------------------------------
CilDataFlowAnalyzer::CilDataFlowAnalyzer(BYTE* pMethodBytes) :
	_pMethodBytes(pMethodBytes)
{
}


CilDataFlowAnalyzer::CilDataFlowAnalyzer(BYTE* pMethodBytes, bool initialize) :
	_pMethodBytes(pMethodBytes)
{
	Init();
}

void CilDataFlowAnalyzer::Init()
{
	if (_isInitialized)
		throw std::logic_error("Calling Init() is illegal here. The CilDataFlowAnalyzer is already initialized.");
	
	ParseMethodBytes();

	_isInitialized = true;
}


StackState CilDataFlowAnalyzer::GetStackStatusAtOffset(int offset)
{
	if (! _isInitialized)
		throw std::logic_error("The CilDataFlowAnalyzer is not initialized. You must call Init() before using it");
	
	// Check if the offset is within a SEH handler block.
	// If it is, the stack will contain only the exception in question
	for(const auto clause : _clauseMap)
	{
		if (clause.second->IsOffsetInHandlerBlock(offset))
			return StackState{CorElementType::ELEMENT_TYPE_CLASS};	// The stack contains only the respective exception
	}

	return _pCilBasicBlockParser->GetStackStatusAtOffset(offset);
}

// PRIVATE ----------------------------------------------------------------------------------------
void CilDataFlowAnalyzer::ParseMethodBytes()
{
	ParseHeader();
	ParseCode();
	ParseExtraDataSections();
}

void CilDataFlowAnalyzer::ParseHeader()
{
	const auto headerAsFat = reinterpret_cast<const COR_ILMETHOD_FAT*>(_pMethodBytes);
	if (headerAsFat->IsFat())
	{
		_isFat = true;
		_pCodeBytes = headerAsFat->GetCode();
		_codeSize = headerAsFat->GetCodeSize();
		_maxStack = headerAsFat->GetMaxStack();
		_localVarSigTok = headerAsFat->GetLocalVarSigTok();
		_flags = headerAsFat->GetFlags();
		_hasMore = headerAsFat->More();
		_pExtraDataSections = const_cast<COR_ILMETHOD_SECT*>(headerAsFat->GetSect());
		return;
	}
	
	const auto headerAsTiny = reinterpret_cast<const COR_ILMETHOD_TINY*>(_pMethodBytes);
	_isFat = false;
	_pCodeBytes = headerAsTiny->GetCode();
	_codeSize = headerAsTiny->GetCodeSize();
}


void CilDataFlowAnalyzer::ParseCode()
{
	_pCilBasicBlockParser = new CilBasicBlockParser{ _pCodeBytes, static_cast<int>(_codeSize) };
	_pCilBasicBlockParser->Init();
}

void CilDataFlowAnalyzer::ParseExtraDataSections()
{
	if (!_hasMore)
		return;

	if (_pExtraDataSections->IsFat())
	{
		const auto fat = _pExtraDataSections->Fat;
		const auto numberOfClauses = (fat.DataSize - 4U) / 24U;
		auto pClause = reinterpret_cast<IMAGE_COR_ILMETHOD_SECT_EH_CLAUSE_FAT*>(reinterpret_cast<BYTE*>(_pExtraDataSections) + 4);
		for (int i = 0; i < numberOfClauses; i++)
		{
			auto pEHClause = new ExceptionHandlingClause();
			pEHClause->SetTryOffset(pClause->TryOffset);
			pEHClause->SetTryLength(pClause->TryLength);
			pEHClause->SetHandlerOffset(pClause->HandlerOffset);
			pEHClause->SetHandlerLength(pClause->HandlerLength);
			_clauseMap.insert(ExceptionHandlingClauseMap::value_type(pClause->HandlerOffset, pEHClause));

			pClause++;
		}	
	}
	else
	{
		const auto smol = _pExtraDataSections->Small;
		const auto numberOfClauses = (smol.DataSize - 4) / 12;
		auto pClause = reinterpret_cast<IMAGE_COR_ILMETHOD_SECT_EH_CLAUSE_SMALL*>(reinterpret_cast<BYTE*>(_pExtraDataSections) + 4);
		for(int i = 0; i < numberOfClauses; i++)
		{
			auto pEHClause = new ExceptionHandlingClause();
			pEHClause->SetTryOffset(pClause->TryOffset);
			pEHClause->SetTryLength(pClause->TryLength);
			pEHClause->SetHandlerOffset(pClause->HandlerOffset);
			pEHClause->SetHandlerLength(pClause->HandlerLength);
			_clauseMap.insert(ExceptionHandlingClauseMap::value_type(pClause->HandlerOffset, pEHClause));
			
			pClause++;
		}
	}
}
