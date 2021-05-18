#include "BasicBlockTree.h"

StackState BasicBlockTree::GetStackStatusAtOffset(int offset)
{
	// 0. Find out which basic block the offset is in. => BBO
	// 1. Find a code path from basic block 0 to BBO
	// 2. Walk the code path, maintaining the stack state

	auto basicBlock = GetBasicBlockAtOffset(offset);
	
	BasicBlockMap route;
	GetRouteToBasicBlock(basicBlock, route);
	
	return StackState{};
}

BasicBlock* BasicBlockTree::GetBasicBlockAtOffset(int offset)
{
	const auto iterator = this->lower_bound(offset);
	if (iterator == this->end())
		return nullptr;

	if (offset > iterator->second->OffsetOfLastOperation)
		return nullptr;
	
	return iterator->second;
}

void BasicBlockTree::GetRouteToBasicBlock(BasicBlock* pBasicBlock, BasicBlockMap& basicBlockMap)
{
	
}
