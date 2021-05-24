#pragma once
#include "cor.h"

class ExceptionHandlingClause
{
public:
    void SetIsException(bool isException) { _isException = isException; }
    bool GetIsException() const { return _isException; }

    void SetIsFilter(bool isFilter) { _isFilter = isFilter; }
    bool GetIsFilter() const { return _isFilter; }

    void SetIsFinally(bool isFinally) { _isFinally = isFinally; }
    bool GetIsFinally() const { return _isFinally; }

    void SetIsFault(bool isFault) { _isFault = isFault; }
    bool GetIsFault() const { return _isFault; }

    void SetTryOffset(DWORD tryOffset) { _tryOffset = tryOffset; }
    DWORD GetTryOffset() const { return _tryOffset; }

    void SetTryLength(DWORD tryLength) { _tryLength = tryLength; }
    DWORD GetTryLength() const { return _tryLength; }

    void SetHandlerOffset(DWORD handlerOffset) { _handlerOffset = handlerOffset; }
    DWORD GetHandlerOffset() const { return _handlerOffset; }

    void SetHandlerLength(DWORD handlerLength) { _handlerLength = handlerLength; }
    DWORD GetHandlerLength() const { return _handlerLength; }

    void SetClassTokenOrFilterOffset(DWORD classTokenOrFilterOffset) { _classTokenOrFilterOffset = classTokenOrFilterOffset; }
    DWORD GetClassTokenOrFilterOffset() const { return _classTokenOrFilterOffset; }

    bool IsOffsetInHandlerBlock(const DWORD offset) const
    {
        return offset >= _handlerOffset && offset < _handlerOffset + _handlerLength;
    }

private:
    bool _isException = false;
    bool _isFilter = false;
    bool _isFinally = false;
    bool _isFault = false;

    DWORD _tryOffset = 0;
    DWORD _tryLength = 0;
    DWORD _handlerOffset = 0;
    DWORD _handlerLength = 0;
    DWORD _classTokenOrFilterOffset = 0;
};

