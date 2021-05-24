#include "CilBasicBlockParser.h"
#include "OpCodeHelper.h"
#include "opdef.h"

void CilBasicBlockParser::Init()
{
	_state = State::ScanningForBranchTargets;
	_basicBlockTree.insert(BasicBlockTree::value_type(0, new BasicBlock(_nextBlockNumber++, 0, 0)));
	Parse();

	_nextOperationNumber = -1;
	_pCurrentBasicBlock = nullptr;
	_state = State::PopulatingBasicBlocks;
	Parse();
}


StackState CilBasicBlockParser::GetStackStatusAtOffset(int offset)
{
	return _basicBlockTree.GetStackStatusAtOffset(offset);
}

void CilBasicBlockParser::NotifyOperation(BYTE opCode, bool isTwoByteOpCode, CilOperand cilOperand, int operationOffset)
{
	_nextOperationNumber++;
	
	switch (_state)
	{
		case State::ScanningForBranchTargets:
		{
			// The target of a branch is the start of a basic block
			if (!IsBranch(opCode))
				break;

			// Calculate the target offset of the branch from the current offset, the length of the operation and the size of the jump
			DWordList branchTargets;
			CalculateBranchTargetOffsets(operationOffset, opCode, isTwoByteOpCode, cilOperand, branchTargets);

			for (DWORD branchTarget : branchTargets)
			{
				auto iterator = _basicBlockTree.find(branchTarget);
				if (iterator != _basicBlockTree.end())
					continue;

				auto basicBlock = new BasicBlock(_nextBlockNumber++, branchTarget, _nextOperationNumber);
				_basicBlockTree.insert(BasicBlockTree::value_type(branchTarget, basicBlock));
			}
		}
		break;

		case State::PopulatingBasicBlocks:
		{
			// Cases:
			// 0: The operation is operation 0
			// 1: The operation is the start of a basic block and is not operation 0
			// 2: The operation is a conditional or non-conditional basic block terminator
			// 3: The operation is a method exit or return (RET, THROW, ENDFINALLY, ENDFILTER, RETHROW)
			// 4: The operation is neither at the start of a basic block, nor a basic block terminator (We can just ignore this - there is no processing to do.)
			
			const auto basicBlockIter = _basicBlockTree.find(operationOffset);
				
			// case 0: The operation is operation 0
			if (basicBlockIter != _basicBlockTree.end() && operationOffset == 0)
			{
				// This is the very first operation, so just set the current basic block.
				_pCurrentBasicBlock = basicBlockIter->second;
			}

			// case 1: The operation is the start of a basic block and is not operation 0
			if (basicBlockIter != _basicBlockTree.end() && operationOffset != 0)
			{
				// We have reached the start of a new basic block
				const auto newBasicBlock = basicBlockIter->second;
				
				// If the previous operation was not a 'flow stopper',
				// ... add the new basic block as a child of the previous basic block (pointed to by _pCurrentBasicBlock)
				// because the code flows from the previous basic block to the new one.
				// (If the previous opcode *was* a non-conditional branch or method exit - a flow stopper - code does not flow to the new basic block.)
				if (!_isPreviousOpCodeAFlowStopper)
				{
					_pCurrentBasicBlock->Children.insert(BasicBlockTree::value_type(operationOffset, newBasicBlock));
				}
				
				// Point _pCurrentBasicBlock to the new basic block
				_pCurrentBasicBlock = newBasicBlock;
			}

			// case 2: The operation is a branch or method return - a basic block terminator
			if(OpCodeHelper::IsConditionalBranch(opCode, isTwoByteOpCode) || OpCodeHelper::IsNonConditionalBranch(opCode, isTwoByteOpCode))
			{
				// We have reached the end of the current basic block
				// a. set the offset of the last operation in the current basic block to the current operation offset
				_pCurrentBasicBlock->OffsetOfLastOperation = operationOffset;

				// b. Iterate over the target(s) of the conditional branch, find the basic block for each target and make the basic block a child of the current basic block
				DWordList branchTargets;
				CalculateBranchTargetOffsets(operationOffset, opCode, isTwoByteOpCode, cilOperand, branchTargets);
				for (DWORD branchTarget : branchTargets)
				{
					const auto targetBasicBlock = _basicBlockTree.find(branchTarget);
					_pCurrentBasicBlock->Children.insert(BasicBlockTree::value_type(branchTarget, targetBasicBlock->second));
				}

				// The operation after a branch is the beginning of a new basic block
				// We need to check if there is already a basic block defined there
				// Calculate the offset of the next operation
				const auto nextOperationOffset = operationOffset
					+ 1	// 1 byte for the opcode
					+ (isTwoByteOpCode ? 1 : 0)	// 1 byte, if it's a two-byte opcode
					+ cilOperand.OperandByteSize;

				// If we've not reached the end of the code ...
				if(nextOperationOffset < _codeLength)
				{
					const auto subsequentBasicBlockIter = _basicBlockTree.find(nextOperationOffset);
					// If there's no basic block at that offset, define one
					if(subsequentBasicBlockIter == _basicBlockTree.end())
					{
						auto subsequentBasicBlock = new BasicBlock(_nextBlockNumber++, nextOperationOffset, _nextOperationNumber + 1);
						_basicBlockTree.insert(BasicBlockTree::value_type(nextOperationOffset, subsequentBasicBlock));
					}
				}

				_isPreviousOpCodeAFlowStopper = OpCodeHelper::IsNonConditionalBranch(opCode, isTwoByteOpCode);
			}

			// case 3: The operation is a method exit or return (RET, THROW, ENDFINALLY, ENDFILTER, RETHROW)
			if(OpCodeHelper::IsMethodExit(opCode, isTwoByteOpCode))
			{
				// We have reached the end of the current basic block
				// a. set the offset of the last operation in the current basic block to the current operation offset
				_pCurrentBasicBlock->OffsetOfLastOperation = operationOffset;

				// The operation after a method exit is the beginning of a new basic block
				// We need to check if there is already a basic block defined there
				// Calculate the offset of the next operation
				const auto nextOperationOffset = operationOffset
					+ 1	// 1 byte for the opcode
					+ (isTwoByteOpCode ? 1 : 0)	// 1 byte, if it's a two-byte opcode
					+ cilOperand.OperandByteSize;

				// If we've not reached the end of the code ...
				if (nextOperationOffset < _codeLength)
				{
					const auto subsequentBasicBlockIter = _basicBlockTree.find(nextOperationOffset);
					// If there's no basic block at that offset, define one
					if (subsequentBasicBlockIter == _basicBlockTree.end())
					{
						auto subsequentBasicBlock = new BasicBlock(_nextBlockNumber++, nextOperationOffset, _nextOperationNumber + 1);
						_basicBlockTree.insert(BasicBlockTree::value_type(nextOperationOffset, subsequentBasicBlock));
					}
				}

				_isPreviousOpCodeAFlowStopper = true;
			}
		}
		break;
	}
}

bool CilBasicBlockParser::IsBranch(BYTE opCode) const
{
	return _branchOpCodeMap.count(opCode) != 0;
}

 void CilBasicBlockParser::CalculateBranchTargetOffsets(int operationOffset, BYTE opCode, bool isTwoByteOpCode, CilOperand cilOperand, DWordList& branchTargets)
{
	// A switch statement has multiple targets. All other branch types have a single target.
	// Calculate the target offset of the branch(s) from the current offset, the length of the operation and the size of the jump(s)
	 
	if(opCode == CEE_SWITCH)
	{
		const auto jumpBase = operationOffset
			+ 1 // 1 byte for the SWITCH opcode
			+ cilOperand.OperandByteSize;

		for(int i = 0; i < cilOperand.SwitchOperation->NumberOfTargets; i++)
		{
			branchTargets.push_back(jumpBase + cilOperand.SwitchOperation->Targets[i]);
		}

		return;
	}

	const auto jumpBase = operationOffset
		+ 1	// 1 byte for the opcode
		+ (isTwoByteOpCode ? 1 : 0)	// 1 byte, if it's a two-byte opcode
		+ cilOperand.OperandByteSize;

	const auto operandSize = _branchOpCodeMap.find(opCode)->second;
	const DWORD jumpSize = operandSize == 4 ? cilOperand.Int32 : cilOperand.Int8;
	branchTargets.push_back(jumpBase + jumpSize);
	return;
}

void CilBasicBlockParser::InitializeBranchOpCodeMap()
{
	_branchOpCodeMap.insert(ByteMap::value_type(CEE_BR_S, 1));
	_branchOpCodeMap.insert(ByteMap::value_type(CEE_BRFALSE_S, 1));
	_branchOpCodeMap.insert(ByteMap::value_type(CEE_BRTRUE_S, 1));
	_branchOpCodeMap.insert(ByteMap::value_type(CEE_BEQ_S, 1));
	_branchOpCodeMap.insert(ByteMap::value_type(CEE_BGE_S, 1));
	_branchOpCodeMap.insert(ByteMap::value_type(CEE_BGT_S, 1));
	_branchOpCodeMap.insert(ByteMap::value_type(CEE_BLE_S, 1));
	_branchOpCodeMap.insert(ByteMap::value_type(CEE_BLT_S, 1));
	_branchOpCodeMap.insert(ByteMap::value_type(CEE_BNE_UN_S, 1));
	_branchOpCodeMap.insert(ByteMap::value_type(CEE_BGE_UN_S, 1));
	_branchOpCodeMap.insert(ByteMap::value_type(CEE_BGT_UN_S, 1));
	_branchOpCodeMap.insert(ByteMap::value_type(CEE_BLE_UN_S, 1));
	_branchOpCodeMap.insert(ByteMap::value_type(CEE_BLT_UN_S, 1));
	_branchOpCodeMap.insert(ByteMap::value_type(CEE_BR, 4));
	_branchOpCodeMap.insert(ByteMap::value_type(CEE_BRFALSE, 4));
	_branchOpCodeMap.insert(ByteMap::value_type(CEE_BRTRUE, 4));
	_branchOpCodeMap.insert(ByteMap::value_type(CEE_BEQ, 4));
	_branchOpCodeMap.insert(ByteMap::value_type(CEE_BGE, 4));
	_branchOpCodeMap.insert(ByteMap::value_type(CEE_BGT, 4));
	_branchOpCodeMap.insert(ByteMap::value_type(CEE_BLE, 4));
	_branchOpCodeMap.insert(ByteMap::value_type(CEE_BLT, 4));
	_branchOpCodeMap.insert(ByteMap::value_type(CEE_BNE_UN, 4));
	_branchOpCodeMap.insert(ByteMap::value_type(CEE_BGE_UN, 4));
	_branchOpCodeMap.insert(ByteMap::value_type(CEE_BGT_UN, 4));
	_branchOpCodeMap.insert(ByteMap::value_type(CEE_BLE_UN, 4));
	_branchOpCodeMap.insert(ByteMap::value_type(CEE_BLT_UN, 4));
	_branchOpCodeMap.insert(ByteMap::value_type(CEE_LEAVE_S, 1));
	_branchOpCodeMap.insert(ByteMap::value_type(CEE_LEAVE, 4));
	_branchOpCodeMap.insert(ByteMap::value_type(CEE_SWITCH, 0));	
}
