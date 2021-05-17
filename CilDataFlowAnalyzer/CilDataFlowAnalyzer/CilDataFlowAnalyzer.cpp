#include "CilDataFlowAnalyzer.h"

#include "OpCodeHelper.h"
#include "opdef.h"

void CilDataFlowAnalyzer::ScanForBasicBlocks()
{
	_state = State::ScanningForBasicBlocks;
	_basicBlockMap.insert(BasicBlockMap::value_type(0, new BasicBlock(_nextBlockNumber++, 0, 0)));
	Parse();

	_nextOperationNumber = 0;
	_pCurrentBasicBlock = nullptr;
	_previousOperationOffset = 0;
	_state = State::PopulatingBasicBlocks;
	Parse();
	
}

void CilDataFlowAnalyzer::NotifyOperation(BYTE opCode, bool isTwoByteOpCode, CilOperand cilOperand, int operationOffset)
{
	_nextOperationNumber++;
	
	switch (_state)
	{
		case State::ScanningForBasicBlocks:
		{
			if (!IsJump(opCode))
				break;

			// Calculate the target offset of the jump from the current offset, the length of the operation and the size of the jump
			DWordList jumpTargets;
			CalculateJumpTargetOffsets(operationOffset, opCode, isTwoByteOpCode, cilOperand, jumpTargets);

			for (DWORD jumpTarget : jumpTargets)
			{
				auto iterator = _basicBlockMap.find(jumpTarget);
				if (iterator != _basicBlockMap.end())
					continue;

				auto basicBlock = new BasicBlock(_nextBlockNumber++, jumpTarget, _nextOperationNumber);
				_basicBlockMap.insert(BasicBlockMap::value_type(jumpTarget, basicBlock));
			}
		}
		break;

		case State::PopulatingBasicBlocks:
		{
			// Cases:
			// 0: The operation is operation 0
			// 1: The operation is the start of a basic block and is not operation 0
			// 2: The operation is a conditional basic block terminator
			// 3: The operation is a non-conditional basic block terminator
			// 4: The operation is a method exit or return (RET, THROW, ENDFINALLY, ENDFILTER, RETHROW)
			// 5: The operation is neither at the start of a basic block, nor a basic block terminator			
			

			const auto iterator = _basicBlockMap.find(operationOffset);
				
			// case 0: The operation is operation 0
			if (iterator != _basicBlockMap.end() && operationOffset == 0)
			{
				// This is the very first operation, so just set the current basic block.
				_pCurrentBasicBlock = iterator->second;
			}

			// case 1: The operation is the start of a basic block and is not operation 0
			if (iterator != _basicBlockMap.end() && operationOffset != 0)
			{
				// We have reached the start of a new basic block
				const auto newBasicBlock = iterator->second;
				
				// a. set the offset of the last operation in the previous basic block (pointed to by _pCurrentBasicBlock)
				_pCurrentBasicBlock->OffsetOfLastOperation = _previousOperationOffset;
				
				// b. Add the new basic block as a child of the previous basic block (pointed to by _pCurrentBasicBlock) because the code flows from the previous basic block to the new one
				_pCurrentBasicBlock->Children.push_back(newBasicBlock);
				
				// c. Point _pCurrentBasicBlock to the new basic block
				_pCurrentBasicBlock = newBasicBlock;
			}

			// case 2: The operation is a conditional basic block terminator
			if(OpCodeHelper::IsConditionalBranch(opCode, isTwoByteOpCode))
			{
				// We have reached the end of the current basic block
				// a. set the offset of the last operation in the current basic block to the current operation offset
				_pCurrentBasicBlock->OffsetOfLastOperation = operationOffset;

				// b. Iterate over the target(s) of the conditional jump, find the basic block for each target and make the basic block a child of the current basic block
				DWordList jumpTargets;
				CalculateJumpTargetOffsets(operationOffset, opCode, isTwoByteOpCode, cilOperand, jumpTargets);
				for (DWORD jumpTarget : jumpTargets)
				{
					const auto targetBasicBlock = _basicBlockMap.find(jumpTarget);
					_pCurrentBasicBlock->Children.push_back(targetBasicBlock->second);
				}

				// The operation after a branch is the beginning of a new basic block
				// Calculate the offset of the next operation
				const auto nextOperationOffset = operationOffset
					+ 1	// 1 byte for the opcode
					+ (isTwoByteOpCode ? 1 : 0)	// 1 byte, if it's a two-byte opcode
					+ cilOperand.OperandByteSize;

				if(nextOperationOffset < _codeLength)
				{
					BasicBlock* subsequentBasicBlock;
					const auto subsequentBasicBlockIter = _basicBlockMap.find(nextOperationOffset);
					if(subsequentBasicBlockIter == _basicBlockMap.end())
					{
						subsequentBasicBlock = new BasicBlock(_nextBlockNumber++, nextOperationOffset, _nextOperationNumber);
						_basicBlockMap.insert(BasicBlockMap::value_type(nextOperationOffset, subsequentBasicBlock));
					}
					else
					{
						subsequentBasicBlock = subsequentBasicBlockIter->second;
					}
					_pCurrentBasicBlock->Children.push_back(subsequentBasicBlock);				
				}
			}

			_previousOperationOffset = _currentOperationOffset;
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
