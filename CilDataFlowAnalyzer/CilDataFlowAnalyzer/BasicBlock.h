#pragma once
#include <map>

class BasicBlock;
typedef std::map<int, BasicBlock*> BasicBlockMap;

class BasicBlock
{
public:
	
	BasicBlock(int blockNumber, int offset, int operationNumber)
		: BlockNumber(blockNumber), Offset(offset), OperationNumber(operationNumber), OffsetOfLastOperation(0) {}

	int BlockNumber;
	int Offset;
	int OperationNumber;
	int OffsetOfLastOperation;
	BasicBlockMap Children;
};
