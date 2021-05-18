#pragma once

#include <map>
#include "BasicBlock.h"
#include "StackState.h"

class BasicBlockTree : public std::map<int, BasicBlock*>
{
public:
	StackState GetStackStatusAtOffset(int offset);
	BasicBlock* GetBasicBlockAtOffset(int offset);
	void GetRouteToBasicBlock(BasicBlock* pBasicBlock, BasicBlockMap& basicBlockMap);
};
