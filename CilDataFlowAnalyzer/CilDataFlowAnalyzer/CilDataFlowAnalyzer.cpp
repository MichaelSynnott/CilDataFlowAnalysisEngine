#include "CilDataFlowAnalyzer.h"
#include "opdef.h"

void CilDataFlowAnalyzer::ScanForBasicBlocks()
{
	_state = State::BasicBlockScan;
	
	_basicBlockMap.insert(BasicBlockMap::value_type(0, new BasicBlock(_nextBlockNumber++, 0, 0)));
	
	Parse();
}

void CilDataFlowAnalyzer::NotifyOperation(BYTE opCode, bool isTwoByteOpCode, CilOperand cilOperand, int operationOffset)
{
	_nextOperationNumber++;
	
	switch (_state)
	{
	case State::BasicBlockScan:
		if (IsJump(opCode))
		{
			// Calculate the target offset of the jump from the current offset, the length of the operation and the size of the jump
			DWordList jumpTargets;
			CalculateJumpTargetOffsets(operationOffset, opCode, isTwoByteOpCode, cilOperand, jumpTargets);
			
			for(DWORD jumpTarget : jumpTargets)
			{
				auto iterator = _basicBlockMap.find(jumpTarget);			
				if (iterator != _basicBlockMap.end())
					continue;
				
				auto basicBlock = new BasicBlock(_nextBlockNumber++, jumpTarget, _nextOperationNumber);
				_basicBlockMap.insert(BasicBlockMap::value_type(jumpTarget, basicBlock));				
			}
		}
		break;
	}
}

bool CilDataFlowAnalyzer::IsJump(BYTE opCode) const
{
	return _jumpOpCodeMap.count(opCode) != 0;
}

 void CilDataFlowAnalyzer::CalculateJumpTargetOffsets(int operationOffset, BYTE opCode, bool isTwoByteOpCode, CilOperand cilOperand, DWordList& jumpTargets)
{
	// A switch statement has multiple targets. All other jump types have a single target.
	// Calculate the target offset of the jump(s) from the current offset, the length of the operation and the size of the jump(s)
	 
	if(opCode == CEE_SWITCH)
	{
		const auto jumpBase = operationOffset
			+ 1 // 1 byte for the SWITCH opcode
			+ cilOperand.OperandByteSize;

		for(int i = 0; i < cilOperand.SwitchOperation->NumberOfTargets; i++)
		{
			jumpTargets.push_back(jumpBase + cilOperand.SwitchOperation->Targets[i]);
		}

		return;
	}

	const auto jumpBase = operationOffset
		+ 1	// 1 byte for the opcode
		+ (isTwoByteOpCode ? 1 : 0)	// 1 byte, if it's a two-byte opcode
		+ cilOperand.OperandByteSize;

	const auto operandSize = _jumpOpCodeMap.find(opCode)->second;
	DWORD jumpSize = operandSize == 4 ? cilOperand.Int32 : cilOperand.Int8;
	jumpTargets.push_back(jumpBase + jumpSize);
	return;
}

void CilDataFlowAnalyzer::InitializeJumpOpCodeMap()
{
	_jumpOpCodeMap.insert(ByteMap::value_type(CEE_BR_S, 1));
	_jumpOpCodeMap.insert(ByteMap::value_type(CEE_BRFALSE_S, 1));
	_jumpOpCodeMap.insert(ByteMap::value_type(CEE_BRTRUE_S, 1));
	_jumpOpCodeMap.insert(ByteMap::value_type(CEE_BEQ_S, 1));
	_jumpOpCodeMap.insert(ByteMap::value_type(CEE_BGE_S, 1));
	_jumpOpCodeMap.insert(ByteMap::value_type(CEE_BGT_S, 1));
	_jumpOpCodeMap.insert(ByteMap::value_type(CEE_BLE_S, 1));
	_jumpOpCodeMap.insert(ByteMap::value_type(CEE_BLT_S, 1));
	_jumpOpCodeMap.insert(ByteMap::value_type(CEE_BNE_UN_S, 1));
	_jumpOpCodeMap.insert(ByteMap::value_type(CEE_BGE_UN_S, 1));
	_jumpOpCodeMap.insert(ByteMap::value_type(CEE_BGT_UN_S, 1));
	_jumpOpCodeMap.insert(ByteMap::value_type(CEE_BLE_UN_S, 1));
	_jumpOpCodeMap.insert(ByteMap::value_type(CEE_BLT_UN_S, 1));
	_jumpOpCodeMap.insert(ByteMap::value_type(CEE_BR, 4));
	_jumpOpCodeMap.insert(ByteMap::value_type(CEE_BRFALSE, 4));
	_jumpOpCodeMap.insert(ByteMap::value_type(CEE_BRTRUE, 4));
	_jumpOpCodeMap.insert(ByteMap::value_type(CEE_BEQ, 4));
	_jumpOpCodeMap.insert(ByteMap::value_type(CEE_BGE, 4));
	_jumpOpCodeMap.insert(ByteMap::value_type(CEE_BGT, 4));
	_jumpOpCodeMap.insert(ByteMap::value_type(CEE_BLE, 4));
	_jumpOpCodeMap.insert(ByteMap::value_type(CEE_BLT, 4));
	_jumpOpCodeMap.insert(ByteMap::value_type(CEE_BNE_UN, 4));
	_jumpOpCodeMap.insert(ByteMap::value_type(CEE_BGE_UN, 4));
	_jumpOpCodeMap.insert(ByteMap::value_type(CEE_BGT_UN, 4));
	_jumpOpCodeMap.insert(ByteMap::value_type(CEE_BLE_UN, 4));
	_jumpOpCodeMap.insert(ByteMap::value_type(CEE_BLT_UN, 4));
	_jumpOpCodeMap.insert(ByteMap::value_type(CEE_LEAVE_S, 1));
	_jumpOpCodeMap.insert(ByteMap::value_type(CEE_LEAVE, 4));
	_jumpOpCodeMap.insert(ByteMap::value_type(CEE_SWITCH, 0));	
}
