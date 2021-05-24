#include "BasicBlockTree.h"

StackState BasicBlockTree::GetStackStatusAtOffset(int offset)
{
	// 0. Find out which basic block the offset is in. => BBO
	// 1. Find a code path from basic block 0 to BBO
	// 2. Walk the code path, maintaining the stack state

	const auto pTargetBasicBlock = GetBasicBlockAtOffset(offset);
	
	if (pTargetBasicBlock == nullptr)
		return StackState{};

	const auto pRootBasicBlock = this->at(0);
	BasicBlockMap route;
	GetRouteToBasicBlock(pTargetBasicBlock, pRootBasicBlock, route);
	
	// Now walk the calculated route to the operation at the specified offset,
	// calculating the stack state as we go.

	//auto x = GetCodePathBytes(route, offset);
	
	// TODO: Walk the route, calculating the stack state
	
	return StackState{};
}

BasicBlock* BasicBlockTree::GetBasicBlockAtOffset(int offset)
{
	for (auto& iBasicBlock : *this)
	{
		if (iBasicBlock.second->Offset > offset)
			return nullptr;

		if (offset >= iBasicBlock.second->Offset && offset <= iBasicBlock.second->OffsetOfLastOperation)
			return iBasicBlock.second;
	}
	return nullptr;
}

bool BasicBlockTree::GetRouteToBasicBlock(BasicBlock* pTarget, BasicBlock* pRoot, BasicBlockMap& route)
{
	route.insert(BasicBlockMap::value_type(pRoot->Offset, pRoot));
	if (pRoot->Offset == pTarget->Offset)
		return true;
	
	for(const auto bb : pRoot->Children)
	{
		if (GetRouteToBasicBlock(pTarget, bb.second, route))
			return true;
		
		route.erase(bb.second->Offset);
	}
	return false;
}
