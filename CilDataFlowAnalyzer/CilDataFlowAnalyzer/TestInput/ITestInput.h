#pragma once
#include "cor.h"

class ITestInput
{
public:
	virtual ~ITestInput() = default;
	virtual BYTE* GetTestBytes() = 0;
	virtual int GetTestLength() = 0;
};
