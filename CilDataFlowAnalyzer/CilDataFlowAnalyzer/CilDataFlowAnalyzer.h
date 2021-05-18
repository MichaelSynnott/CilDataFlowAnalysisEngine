#pragma once
#include "BasicBlock.h"
#include "InstructionEnumerator.h"

using namespace std;


typedef list<DWORD> DWordList;
typedef map<BYTE, BYTE> ByteMap;

enum class State
{
	ScanningForBranchTargets,
	PopulatingBasicBlocks
};

class CilDataFlowAnalyzer : public CilParser
{
public:
	CilDataFlowAnalyzer(BYTE* pCode, int codeLength)
		: CilParser(pCode, codeLength)
	{
		InitializeBranchOpCodeMap();
	}

	CilDataFlowAnalyzer(WordList& cilBytes)
		: CilParser(cilBytes)
	{
		InitializeBranchOpCodeMap();
	}

	void ScanForBasicBlocks();

protected:
	bool IsBranch(BYTE opCode) const;
	void CalculateBranchTargetOffsets(int operationOffset, BYTE opCode, bool isTwoByteOpCode, CilOperand cilOperand, DWordList& branchTargets);

	void InitializeBranchOpCodeMap();

	
	void NotifyOperation(BYTE opCode, bool isTwoByteOpCode, CilOperand cilOperand, int operationOffset) override;

	BasicBlock* _pCurrentBasicBlock = 0;
	ByteMap _branchOpCodeMap;
	BasicBlockMap _basicBlockMap;
	int _nextBlockNumber = 0;
	int _nextOperationNumber = -1;
	State _state;
	bool _isPreviousOpCodeAFlowStopper = false;
};
