#pragma once

#include "CustomTypes.h"
#include "cor.h"
#include "corprof.h"
#include <vector>

#include "SwitchOperation.h"

enum class CilOperandType
{
    None,
    Token,
    Int8,
    Int16,
    Int32,
    Int64,
    Float32,
    Float64,
	SwitchTargetList
};

class CilOperand
{
public:
	CilOperand() : OperandType(CilOperandType::None) {}
	CilOperand(mdToken token, bool isToken) : OperandType(CilOperandType::Token), Token(token), OperandByteSize(4) {}   // Second argument to differentiate between mdToken and DWORD
    CilOperand(BYTE int8) : OperandType(CilOperandType::Int8), Int8(int8), OperandByteSize(1) {}
    CilOperand(WORD int16) : OperandType(CilOperandType::Int16), Int16(int16), OperandByteSize(2) {}
    CilOperand(DWORD int32) : OperandType(CilOperandType::Int32), Int32(int32), OperandByteSize(4) {}
    CilOperand(UINT64 uint64) : OperandType(CilOperandType::Int64), Int64(uint64), OperandByteSize(8) {}
    CilOperand(FLOAT float32) : OperandType(CilOperandType::Float32), Float32(float32), OperandByteSize(4) {}
    CilOperand(DOUBLE float64) : OperandType(CilOperandType::Float64), Float64(float64), OperandByteSize(8) {}
	
    CilOperand(SwitchOperation* switchOperation)
		: OperandType(CilOperandType::SwitchTargetList), SwitchOperation(switchOperation)
	{
        OperandByteSize = 4 + switchOperation->NumberOfTargets * 4;
	}
	
    CilOperandType OperandType = CilOperandType::None;
    mdToken Token = 0;
    BYTE Int8 = 0;
    WORD Int16 = 0;
    DWORD Int32 = 0;
    UINT64 Int64 = 0;
    FLOAT Float32 = 0;
    DOUBLE Float64 = 0;
    SwitchOperation* SwitchOperation;
    BYTE OperandByteSize = 0;
};

class CilParser
{

public:
    CilParser(BYTE* code, int codeLength)
        : _code(code), _codeLength(codeLength) {}

    CilParser(WordList& cilBytes)
    {
        _codeLength = cilBytes.size();
        _newCodeArrayCreated = true;
        _code = new BYTE[_codeLength];
        int i = 0;
        for (auto byte : cilBytes)
        {
            *(_code + i++) = byte;
        }
    }

    virtual ~CilParser()
    {
        if (_newCodeArrayCreated)
        {
            delete _code;
        }
    }
    virtual void Parse();

protected:

    const CilOperand CilOperandNone{};
	
    // subtype these methods to create your parser side-effects
    virtual void NotifyBeginParse() {}
    virtual void NotifyEndParse() {}
    virtual void NotifyOpCode(BYTE opCode, int offset) {}
    virtual void NotifyTwoByteOpCode(BYTE opCode, int offset) {}
    virtual void NotifyToken(mdToken, int offset) {}
    virtual void NotifyInt8(BYTE, int offset) {}
    virtual void NotifyInt16(WORD, int offset) {}
    virtual void NotifyInt32(DWORD, int offset) {}
    virtual void NotifyInt64(UINT64, int offset) {}
    virtual void NotifyFloat32(FLOAT, int offset) {}
    virtual void NotifyFloat64(DOUBLE, int offset) {}
    virtual void NotifyUnrecognizedOpCode(BYTE opCode, int offset) {}
    virtual void NotifyUnrecognizedTwoByteOpCode(BYTE opCode, int offset) {}
    virtual void NotifyTwoByteOpCodePrefix(int offset) {}
    virtual void NotifyOperation(BYTE opCode, bool isTwoByteOpCode, CilOperand cilOperand, int operationOffset) {};
    virtual BYTE ParseInt8();
    virtual WORD ParseInt16();
    virtual DWORD ParseInt32();
    virtual UINT64 ParseInt64();
    virtual DOUBLE ParseFloat64();
    virtual FLOAT ParseFloat32();

    virtual mdToken ParseToken();

    virtual DWORD ConsumeInt32();
    virtual UINT64 ConsumeInt64();
    virtual DOUBLE ConsumeFloat64();
    virtual FLOAT ConsumeFloat32();

    bool _newCodeArrayCreated = false;
    BYTE* _code = nullptr;
    int _codeLength;
    int _ip = 0;

    BYTE _currentOpCode;
    int _currentOperationOffset;
    bool _isCurrentTwoByteOpCode;

    bool _isParsingSwitchOperation = false;
    int _currentSwitchTarget = 0;
    DWORD* _switchTargets;
};


