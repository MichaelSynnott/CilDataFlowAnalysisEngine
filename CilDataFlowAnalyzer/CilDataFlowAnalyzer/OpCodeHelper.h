#pragma once

#include "cor.h"

class OpCodeHelper
{
public:
	static bool IsConditionalBranch(BYTE opCode, bool isTwoByteOpCode);
	static bool IsNonConditionalBranch(BYTE opCode, bool isTwoByteOpCode);
};