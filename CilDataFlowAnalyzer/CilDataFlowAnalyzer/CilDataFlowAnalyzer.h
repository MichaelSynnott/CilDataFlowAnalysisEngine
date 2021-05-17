#pragma once
#include "BasicBlock.h"
#include "cor.h"
#include "InstructionEnumerator.h"

using namespace std;

typedef map<int, BasicBlock*> BasicBlockMap;
typedef list<DWORD> DWordList;
typedef map<BYTE, BYTE> ByteMap;

enum class State
{
	ScanningForBasicBlocks,
	PopulatingBasicBlocks
};

class CilDataFlowAnalyzer : public CilParser
{
public:
	CilDataFlowAnalyzer(BYTE* pCode, int codeLength)
		: CilParser(pCode, codeLength)
	{
		InitializeJumpOpCodeMap();
	}

	CilDataFlowAnalyzer(WordList& cilBytes)
		: CilParser(cilBytes)
	{
		InitializeJumpOpCodeMap();
	}

	void ScanForBasicBlocks();

protected:
	bool IsJump(BYTE opCode) const;
	void CalculateJumpTargetOffsets(int operationOffset, BYTE opCode, bool isTwoByteOpCode, CilOperand cilOperand, DWordList& jumpTargets);

	void InitializeJumpOpCodeMap();

	
	void NotifyOperation(BYTE opCode, bool isTwoByteOpCode, CilOperand cilOperand, int operationOffset) override;

	BasicBlock* _pCurrentBasicBlock = 0;
	int _previousOperationOffset = 0;
	ByteMap _jumpOpCodeMap;
	BasicBlockMap _basicBlockMap;
	int _nextBlockNumber = 0;
	int _nextOperationNumber = -1;
	State _state;
};
