#pragma once
#include "BasicBlock.h"
#include "BasicBlockTree.h"
#include "CilParser.h"

using namespace std;

enum class State
{
	ScanningForBranchTargets,
	PopulatingBasicBlocks
};

class CilBasicBlockParser : public CilParser
{
public:
	CilBasicBlockParser(BYTE* pCode, int codeLength)
		: CilParser(pCode, codeLength)
	{
		InitializeBranchOpCodeMap();
	}

	CilBasicBlockParser(WordList& cilBytes)
		: CilParser(cilBytes)
	{
		InitializeBranchOpCodeMap();
	}

	void Init();
	StackState GetStackStatusAtOffset(int offset);
	BasicBlockTree GetBasicBlockTree() const { return _basicBlockTree; }

protected:
	bool IsBranch(BYTE opCode) const;
	void CalculateBranchTargetOffsets(int operationOffset, BYTE opCode, bool isTwoByteOpCode, CilOperand cilOperand, DWordList& branchTargets);
	void InitializeBranchOpCodeMap();
	void NotifyOperation(BYTE opCode, bool isTwoByteOpCode, CilOperand cilOperand, int operationOffset) override;

	BasicBlock* _pCurrentBasicBlock = 0;
	ByteMap _branchOpCodeMap;
	BasicBlockTree _basicBlockTree;
	int _nextBlockNumber = 0;
	int _nextOperationNumber = -1;
	State _state;
	bool _isPreviousOpCodeAFlowStopper = false;
};
