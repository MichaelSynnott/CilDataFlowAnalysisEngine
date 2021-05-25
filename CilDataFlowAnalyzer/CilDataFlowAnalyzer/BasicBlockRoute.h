#pragma once

#include <map>
#include "BasicBlock.h"

class BasicBlockRoute : public std::map<int, BasicBlock*>
{
public:
	unsigned GetRouteCodeSizeToOffset(int offset);
	BYTE* GetRouteCodeBytesToOffset(int offset);
};
