#include "OpCodeHelper.h"
#include "opdef.h"

bool OpCodeHelper::IsConditionalBranch(BYTE opCode, bool isTwoByteOpCode)
{
	if (isTwoByteOpCode)
		return false;
	
	if (opCode >= CEE_BRFALSE_S && opCode <= CEE_BLT_UN_S)
		return true;

	if (opCode >= CEE_BRFALSE && opCode <= CEE_SWITCH)
		return true;

	return false;
}

bool OpCodeHelper::IsNonConditionalBranch(BYTE opCode, bool isTwoByteOpCode)
{
	if (isTwoByteOpCode)
		return false;

	return opCode == CEE_BR_S || opCode == CEE_BR || opCode == CEE_LEAVE_S || opCode == CEE_LEAVE;
}
