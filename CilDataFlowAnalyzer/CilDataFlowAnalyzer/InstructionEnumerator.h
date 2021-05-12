#pragma once
#include "CustomTypes.h"
#include "CilParser.h"
#include <set>

class InstructionEnumerator : public CilParser
{

public:
    InstructionEnumerator(BYTE* code, int codeLength)
        : CilParser(code, codeLength)
    {
    }

    InstructionEnumerator(WordList& cilBytes)
        : CilParser(cilBytes)
    {

    }

    std::vector<int> FindInstructionOffsets(BYTE* targetOpCodes, int opCodeCount, mdToken targetOperand);
    std::vector<int> FindInstructionOffsets(BYTE* targetOpCodes, int opCodeCount);
    std::vector<int> FindInstructionOffsets(BYTE targetOpCode);
    std::vector<int> FindInstructionOffsets(BYTE targetOpCode, mdToken targetOperand);
    std::set<mdToken> FindTokens(BYTE* targetOpCodes, int opCodeCount, CorTokenType tokenType);


private:
    void CheckForOpCodeMatch(BYTE opCode, int offset);
    void CheckForOperandMatch(mdToken token);
    bool IsTargetOpCode(BYTE opCode) const;

    BYTE* _targetOpCodes = nullptr;
    int _opCodeCount;

    mdToken _targetOperand;

    std::vector<int> _locations;
    std::set<mdToken> _tokens;
    bool _isMatchingOpCode = false;
    int _matchingOpCodeOffset = -1;
    bool _hasOperand = false;
    bool _IsParsingForTokens = false;

protected:

    void NotifyOpCode(BYTE opCode, int offset) override;
    void NotifyTwoByteOpCode(BYTE opCode, int offset) override;
    void NotifyToken(mdToken, int offset) override;
    void NotifyInt8(BYTE, int offset) override;
    void NotifyInt16(WORD, int offset) override;
    void NotifyInt32(DWORD, int offset) override;
    void NotifyInt64(UINT64, int offset) override;
    void NotifyFloat32(FLOAT, int offset) override;
    void NotifyFloat64(DOUBLE, int offset) override;
    void NotifyUnrecognizedOpCode(BYTE opCode, int offset) override;
    void NotifyUnrecognizedTwoByteOpCode(BYTE opCode, int offset) override;
    void NotifyTwoByteOpCodePrefix(int offset) override;


};
