#pragma once
#include "BasicBlock.h"
#include "BasicBlockTree.h"
#include "InstructionEnumerator.h"

using namespace std;

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

	void Init();
	StackState GetStackStatusAtOffset(int offset);

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
