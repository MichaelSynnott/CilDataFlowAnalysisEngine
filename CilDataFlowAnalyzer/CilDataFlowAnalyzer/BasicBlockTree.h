#pragma once

#include <map>
#include "BasicBlock.h"
#include "BasicBlockRoute.h"
#include "StackState.h"

class BasicBlockTree : public std::map<int, BasicBlock*>
{
	
public:
	
	static bool GetRouteToBasicBlock(BasicBlock* pTarget, BasicBlock* pRoot, BasicBlockRoute& route);

	StackState GetStackStatusAtOffset(int offset);
	BasicBlock* GetBasicBlockAtOffset(int offset);
};
