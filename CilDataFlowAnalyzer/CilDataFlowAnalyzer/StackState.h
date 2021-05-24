#pragma once
#include <stack>

class StackState : public std::stack<CorElementType>
{
public:
	StackState() = default;
	
	StackState(CorElementType corElementType)
	{
		this->push(corElementType);
	}	
};
