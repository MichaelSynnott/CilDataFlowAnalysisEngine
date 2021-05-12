#include "InstructionEnumerator.h"

std::vector<int> InstructionEnumerator::FindInstructionOffsets(BYTE* targetOpCodes, int opCodeCount, mdToken targetOperand)
{
    _locations.clear();
    _hasOperand = true;
    _IsParsingForTokens = false;
    _targetOpCodes = targetOpCodes;
    _opCodeCount = opCodeCount;
    _targetOperand = targetOperand;
    Parse();
    return _locations;
}

std::vector<int> InstructionEnumerator::FindInstructionOffsets(BYTE* targetOpCodes, int opCodeCount)
{
    _locations.clear();
    _hasOperand = false;
    _IsParsingForTokens = false;
    _targetOpCodes = targetOpCodes;
    _opCodeCount = opCodeCount;
    Parse();
    return _locations;
}

std::vector<int> InstructionEnumerator::FindInstructionOffsets(BYTE targetOpCode)
{
    BYTE targetOpCodes[]{ targetOpCode };
    return FindInstructionOffsets(targetOpCodes, 1);
}

std::vector<int> InstructionEnumerator::FindInstructionOffsets(BYTE targetOpCode, mdToken targetOperand)
{
    BYTE targetOpCodes[]{ targetOpCode };
    return FindInstructionOffsets(targetOpCodes, 1, targetOperand);
}

std::set<mdToken> InstructionEnumerator::FindTokens(BYTE* targetOpCodes, int opCodeCount, CorTokenType tokenType)
{
    _locations.clear();
    _tokens.clear();
    _hasOperand = true;
    _IsParsingForTokens = true;
    _targetOpCodes = targetOpCodes;
    _opCodeCount = opCodeCount;
    _targetOperand = tokenType;
    Parse();
    return _tokens;
}


void InstructionEnumerator::NotifyOpCode(BYTE opCode, int offset)
{
    CheckForOpCodeMatch(opCode, offset);
}

void InstructionEnumerator::NotifyTwoByteOpCode(BYTE opCode, int offset)
{
    CheckForOpCodeMatch(opCode, offset - 1);
}

void InstructionEnumerator::NotifyToken(mdToken token, int offset)
{
    CheckForOperandMatch(token);
}

void InstructionEnumerator::NotifyInt8(BYTE byte, int offset)
{
    CheckForOperandMatch(byte);
}

void InstructionEnumerator::NotifyInt16(WORD word, int offset)
{
    CheckForOperandMatch(word);
}

void InstructionEnumerator::NotifyInt32(DWORD dWord, int offset)
{
    CheckForOperandMatch(dWord);
}

void InstructionEnumerator::NotifyInt64(UINT64 qWord, int offset)
{
    // TODO when actually needed
}

void InstructionEnumerator::NotifyFloat32(FLOAT float32, int offset)
{
    // TODO when actually needed
}

void InstructionEnumerator::NotifyFloat64(DOUBLE float64, int offset)
{
    // TODO when actually needed
}

void InstructionEnumerator::NotifyUnrecognizedOpCode(BYTE opCode, int offset)
{
    // TODO when actually needed
}

void InstructionEnumerator::NotifyUnrecognizedTwoByteOpCode(BYTE opCode, int offset)
{
    // TODO when actually needed
}

void InstructionEnumerator::NotifyTwoByteOpCodePrefix(int offset)
{
    // TODO when actually needed
}

void InstructionEnumerator::CheckForOpCodeMatch(BYTE opCode, int offset)
{
    _isMatchingOpCode = IsTargetOpCode(opCode);
    if (!_isMatchingOpCode) return;

    _matchingOpCodeOffset = offset;

    if (!_hasOperand)
    {
        _locations.push_back(_matchingOpCodeOffset);
    }
}

void InstructionEnumerator::CheckForOperandMatch(mdToken token)
{
    if (_isMatchingOpCode && _hasOperand && _IsParsingForTokens && token >> 24 == _targetOperand >> 24)
    {
        _locations.push_back(_matchingOpCodeOffset);
        _tokens.insert(token);
        return;
    }

    if (_isMatchingOpCode && _hasOperand && !_IsParsingForTokens && token == _targetOperand)
    {
        _locations.push_back(_matchingOpCodeOffset);
    }
}

bool InstructionEnumerator::IsTargetOpCode(BYTE opCode) const
{
    for (int i = 0; i < _opCodeCount; i++)
    {
        if (opCode == _targetOpCodes[i])
        {
            return true;
        }
    }

    return false;
}
