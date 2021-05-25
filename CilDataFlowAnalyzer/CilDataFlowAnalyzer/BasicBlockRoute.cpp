#include "BasicBlockRoute.h"

unsigned BasicBlockRoute::GetRouteCodeSizeToOffset(int offset)
{
	unsigned retVal = 0;
	for(auto node : *this)
	{
		const auto basicBlock = node.second;
		if(offset >= basicBlock->Offset && offset <= basicBlock->OffsetOfLastOperation)
		{
			retVal += (offset - basicBlock->Offset);
			break;
		}
		retVal += basicBlock->OffsetOfLastOperation + basicBlock->LengthOfLastOperation - basicBlock->Offset;
	}

	return retVal;
}

BYTE* BasicBlockRoute::GetRouteCodeBytesToOffset(int offset)
{
	const unsigned codeLength = this->GetRouteCodeSizeToOffset(offset);
	auto retVal = new BYTE[codeLength];

	return nullptr;
}
