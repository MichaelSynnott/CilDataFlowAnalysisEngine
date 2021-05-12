#pragma once

class BasicBlock
{
public:
	
	BasicBlock(int blockNumber, int offset, int operationNumber)
		: BlockNumber(blockNumber), Offset(offset), OperationNumber(operationNumber) {}

	int BlockNumber;
	int Offset;
	int OperationNumber;
};