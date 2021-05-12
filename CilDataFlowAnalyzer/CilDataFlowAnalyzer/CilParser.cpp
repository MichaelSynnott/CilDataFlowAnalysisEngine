#include "CilParser.h"
#include "opdef.h"

void CilParser::Parse()
{
    _ip = 0;

    NotifyBeginParse();

    while (_ip < _codeLength)
    {
        BYTE opCode = *(_code + _ip);
    	
        _currentOpCode = opCode;
        _currentOperationOffset = _ip;
        _isCurrentTwoByteOpCode = false;
    	
        NotifyOpCode(opCode, _ip);

        switch (opCode)
        {
        case CEE_ADD:
        case CEE_ADD_OVF:
        case CEE_ADD_OVF_UN:
        case CEE_AND:
        case CEE_BREAK:
        case CEE_CONV_I1:
        case CEE_CONV_I2:
        case CEE_CONV_I4:
        case CEE_CONV_I8:
        case CEE_CONV_R4:
        case CEE_CONV_R8:
        case CEE_CONV_U1:
        case CEE_CONV_U2:
        case CEE_CONV_U4:
        case CEE_CONV_U8:
        case CEE_CONV_I:
        case CEE_CONV_U:
        case CEE_CONV_R_UN:
        case CEE_CONV_OVF_I1:
        case CEE_CONV_OVF_I2:
        case CEE_CONV_OVF_I4:
        case CEE_CONV_OVF_I8:
        case CEE_CONV_OVF_U1:
        case CEE_CONV_OVF_U2:
        case CEE_CONV_OVF_U4:
        case CEE_CONV_OVF_U8:
        case CEE_CONV_OVF_I:
        case CEE_CONV_OVF_U:
        case CEE_CONV_OVF_I1_UN:
        case CEE_CONV_OVF_I2_UN:
        case CEE_CONV_OVF_I4_UN:
        case CEE_CONV_OVF_I8_UN:
        case CEE_CONV_OVF_U1_UN:
        case CEE_CONV_OVF_U2_UN:
        case CEE_CONV_OVF_U4_UN:
        case CEE_CONV_OVF_U8_UN:
        case CEE_CONV_OVF_I_UN:
        case CEE_CONV_OVF_U_UN:
        case CEE_DIV:
        case CEE_DIV_UN:
        case CEE_DUP:
        case CEE_ENDFINALLY:
        case CEE_LDARG_0:
        case CEE_LDARG_1:
        case CEE_LDARG_2:
        case CEE_LDARG_3:
        case CEE_LDC_I4_0:
        case CEE_LDC_I4_1:
        case CEE_LDC_I4_2:
        case CEE_LDC_I4_3:
        case CEE_LDC_I4_4:
        case CEE_LDC_I4_5:
        case CEE_LDC_I4_6:
        case CEE_LDC_I4_7:
        case CEE_LDC_I4_8:
        case CEE_LDC_I4_M1:
        case CEE_LDIND_I1:
        case CEE_LDIND_I2:
        case CEE_LDIND_I4:
        case CEE_LDIND_I8:
        case CEE_LDIND_U1:
        case CEE_LDIND_U2:
        case CEE_LDIND_U4:
        case CEE_LDIND_R4:
        case CEE_LDIND_R8:
        case CEE_LDIND_I:
        case CEE_LDIND_REF:
        case CEE_LDLOC_0:
        case CEE_LDLOC_1:
        case CEE_LDLOC_2:
        case CEE_LDLOC_3:
        case CEE_LDNULL:
        case CEE_MUL:
        case CEE_MUL_OVF:
        case CEE_MUL_OVF_UN:
        case CEE_NEG:
        case CEE_NOP:
        case CEE_NOT:
        case CEE_OR:
        case CEE_POP:
        case CEE_REM:
        case CEE_REM_UN:
        case CEE_RET:
        case CEE_SHL:
        case CEE_SHR:
        case CEE_SHR_UN:
        case CEE_STIND_I1:
        case CEE_STIND_I2:
        case CEE_STIND_I4:
        case CEE_STIND_I8:
        case CEE_STIND_R4:
        case CEE_STIND_R8:
        case CEE_STIND_I:
        case CEE_STIND_REF:
        case CEE_STLOC_0:
        case CEE_STLOC_1:
        case CEE_STLOC_2:
        case CEE_STLOC_3:
        case CEE_SUB:
        case CEE_SUB_OVF:
        case CEE_SUB_OVF_UN:
        case CEE_XOR:
        case CEE_LDELEM_I1:
        case CEE_LDELEM_I2:
        case CEE_LDELEM_I4:
        case CEE_LDELEM_I8:
        case CEE_LDELEM_U1:
        case CEE_LDELEM_U2:
        case CEE_LDELEM_U4:
        case CEE_LDELEM_R4:
        case CEE_LDELEM_R8:
        case CEE_LDELEM_I:
        case CEE_LDELEM_REF:
        case CEE_LDLEN:
        case CEE_STELEM_I1:
        case CEE_STELEM_I2:
        case CEE_STELEM_I4:
        case CEE_STELEM_I8:
        case CEE_STELEM_R4:
        case CEE_STELEM_R8:
        case CEE_STELEM_I:
        case CEE_STELEM_REF:
        case CEE_THROW:
            NotifyOperation(opCode, false, CilOperandNone, _currentOperationOffset);
            _ip++;
	        break;

        case CEE_BEQ_S:
        case CEE_BGE_S:
        case CEE_BGE_UN_S:
        case CEE_BGT_S:
        case CEE_BGT_UN_S:
        case CEE_BLE_S:
        case CEE_BLE_UN_S:
        case CEE_BLT_S:
        case CEE_BLT_UN_S:
        case CEE_BNE_UN_S:
        case CEE_BR_S:
        case CEE_BRFALSE_S:
        case CEE_BRTRUE_S:
        case CEE_LDARG_S:
        case CEE_LDARGA_S:
        case CEE_LDC_I4_S:
        case CEE_LDLOC_S:
        case CEE_LDLOCA_S:
        case CEE_LEAVE_S:
        case CEE_STARG_S:
        case CEE_STLOC_S:
            _ip++;
            ParseInt8();
	        break;

        case CEE_BEQ:
        case CEE_BGE:
        case CEE_BGE_UN:
        case CEE_BGT:
        case CEE_BGT_UN:
        case CEE_BLE:
        case CEE_BLE_UN:
        case CEE_BLT:
        case CEE_BLT_UN:
        case CEE_BNE_UN:
        case CEE_BR:
        case CEE_BRFALSE:
        case CEE_BRTRUE:
        case CEE_LDC_I4:
        case CEE_LEAVE:
            _ip++;
            ParseInt32();
            break;

        case CEE_LDC_I8:
            _ip++;
            ParseInt64();
            break;

        case CEE_LDC_R4:
            _ip++;
            ParseFloat32();
            break;

        case CEE_LDC_R8:
            _ip++;
            ParseFloat64();
            break;

        case CEE_CALL:
        case CEE_CALLI:
        case CEE_JMP:
        case CEE_BOX:
        case CEE_CALLVIRT:
        case CEE_CASTCLASS:
        case CEE_CPOBJ:
        case CEE_ISINST:
        case CEE_LDELEM:
        case CEE_LDELEMA:
        case CEE_LDFLD:
        case CEE_LDFLDA:
        case CEE_LDOBJ:
        case CEE_LDSFLD:
        case CEE_LDSFLDA:
        case CEE_LDSTR:
        case CEE_LDTOKEN:
        case CEE_MKREFANY:
        case CEE_NEWARR:
        case CEE_NEWOBJ:
        case CEE_STELEM:
        case CEE_STFLD:
        case CEE_STOBJ:
        case CEE_STSFLD:
        case CEE_UNBOX:
        case CEE_UNBOX_ANY:
            _ip++;
            ParseToken();
            break;

        case CEE_SWITCH:
        {
            _isParsingSwitchOperation = true;

            _ip++;
            const auto numberOfTargets = ParseInt32();

            _switchTargets = new DWORD[numberOfTargets];
            _currentSwitchTarget = 0;

            for (DWORD target = 0; target < numberOfTargets; target++)
            {
                ParseInt32();
            }

            const auto switchOperation = new SwitchOperation(numberOfTargets, _switchTargets);
            const CilOperand cilOperand(switchOperation);
            delete[] _switchTargets;
            _isParsingSwitchOperation = false;

            NotifyOperation(CEE_SWITCH, false, cilOperand, _currentOperationOffset);
        }
        break;

        case 0xFE:
            NotifyTwoByteOpCodePrefix(_ip);

            _ip++;
            opCode = *(_code + _ip);

            _currentOpCode = opCode | 0x100;
            _isCurrentTwoByteOpCode = true;

            NotifyTwoByteOpCode(opCode, _ip);

            switch (opCode | 0x100)
            {
            case CEE_READONLY:
            case CEE_TAILCALL:
            case CEE_VOLATILE:
            case CEE_ARGLIST:
            case CEE_CEQ:
            case CEE_CGT:
            case CEE_CGT_UN:
            case CEE_CKFINITE:
            case CEE_CLT:
            case CEE_CLT_UN:
            case CEE_CPBLK:
            case CEE_DIV:
            case CEE_DIV_UN:
            case CEE_ENDFILTER:
            case CEE_INITBLK:
            case CEE_LOCALLOC:
            case CEE_REFANYTYPE:
            case CEE_RETHROW:
                NotifyOperation(_currentOpCode, true, CilOperandNone, _currentOperationOffset);
                _ip++;
                break;

            case CEE_UNALIGNED:
                _ip++;
                ParseInt8();
                break;

            case CEE_LDARG:
            case CEE_LDARGA:
            case CEE_LDLOC:
            case CEE_LDLOCA:
            case CEE_STARG:
            case CEE_STLOC:
                _ip++;
                ParseInt16();
                break;

            case CEE_CONSTRAINED:
                _ip++;
                ParseInt32();
                break;

            case CEE_LDFTN:
            case CEE_INITOBJ:
            case CEE_LDVIRTFTN:
            case CEE_REFANYVAL:
            case CEE_SIZEOF:
                _ip++;
                ParseToken();
                break;

            default:
                NotifyUnrecognizedTwoByteOpCode(opCode, _ip);
                _ip++;
            }
            break;
        
        default:
            NotifyUnrecognizedOpCode(opCode, _ip);
            _ip++;
        }

    }

    NotifyEndParse();
}

mdToken CilParser::ParseToken()
{
    const DWORD retVal = ConsumeInt32();
    NotifyToken(retVal, _ip - 4);

    const CilOperand cilOperand(retVal, true);
    NotifyOperation(_currentOpCode, _isCurrentTwoByteOpCode, cilOperand, _currentOperationOffset);

	
    return retVal;
}

BYTE CilParser::ParseInt8()
{
    const BYTE retVal = *(_code + _ip++);
    NotifyInt8(retVal, _ip - 1);

    const CilOperand cilOperand(retVal);
    NotifyOperation(_currentOpCode, _isCurrentTwoByteOpCode, cilOperand, _currentOperationOffset);

    return retVal;
}

WORD CilParser::ParseInt16()
{
    const BYTE byte0 = *(_code + _ip++);
    const BYTE byte1 = *(_code + _ip++);
    const WORD retVal = byte0 | byte1 << 8;
    NotifyInt16(retVal, _ip - 2);

    const CilOperand cilOperand(retVal);
    NotifyOperation(_currentOpCode, _isCurrentTwoByteOpCode, cilOperand, _currentOperationOffset);
	
    return retVal;
}

DWORD CilParser::ParseInt32()
{
    const DWORD retVal = ConsumeInt32();
    NotifyInt32(retVal, _ip - 4);

    const CilOperand cilOperand(retVal);
	 
	if (_isParsingSwitchOperation)
    {
        _switchTargets[_currentSwitchTarget++] = retVal;
    }
    else
    {
        // If we're parsing a switch operation, we raise the NotifyOperation event in the main parsing loop, after all the targets are parsed
        NotifyOperation(_currentOpCode, _isCurrentTwoByteOpCode, cilOperand, _currentOperationOffset);
    }
    return retVal;
}

UINT64 CilParser::ParseInt64()
{
    const UINT64 retVal = ConsumeInt64();
    NotifyInt64(retVal, _ip - 8);

    const CilOperand cilOperand(retVal);
    NotifyOperation(_currentOpCode, _isCurrentTwoByteOpCode, cilOperand, _currentOperationOffset);

	return retVal;
}

FLOAT CilParser::ParseFloat32()
{
    const FLOAT retVal = ConsumeFloat32();
    NotifyFloat32(retVal, _ip - 4);

    const CilOperand cilOperand(retVal);
    NotifyOperation(_currentOpCode, _isCurrentTwoByteOpCode, cilOperand, _currentOperationOffset);

	
    return retVal;
}

DOUBLE CilParser::ParseFloat64()
{
    const DOUBLE retVal = ConsumeFloat64();
    NotifyFloat64(retVal, _ip - 8);

    const CilOperand cilOperand(retVal);
    NotifyOperation(_currentOpCode, _isCurrentTwoByteOpCode, cilOperand, _currentOperationOffset);

	
    return retVal;
}

DWORD CilParser::ConsumeInt32()
{
    const BYTE byte0 = *(_code + _ip++);
    const BYTE byte1 = *(_code + _ip++);
    const BYTE byte2 = *(_code + _ip++);
    const BYTE byte3 = *(_code + _ip++);
    const DWORD retVal = byte0 | byte1 << 8 | byte2 << 16 | byte3 << 24;
    return retVal;
}

UINT64 CilParser::ConsumeInt64()
{
    const BYTE byte0 = *(_code + _ip++);
    const BYTE byte1 = *(_code + _ip++);
    const BYTE byte2 = *(_code + _ip++);
    const BYTE byte3 = *(_code + _ip++);
    const BYTE byte4 = *(_code + _ip++);
    const BYTE byte5 = *(_code + _ip++);
    const BYTE byte6 = *(_code + _ip++);
    const BYTE byte7 = *(_code + _ip++);

    const UINT64 retVal = byte0 | byte1 << 8 | byte2 << 16 | byte3 << 24 | byte4 << 32 | byte5 << 40 | byte6 << 48 | byte7 << 56;
    return retVal;
}

FLOAT CilParser::ConsumeFloat32()
{
    const BYTE byte0 = *(_code + _ip++);
    const BYTE byte1 = *(_code + _ip++);
    const BYTE byte2 = *(_code + _ip++);
    const BYTE byte3 = *(_code + _ip++);
    const FLOAT retVal = byte0 | byte1 << 8 | byte2 << 16 | byte3 << 24;
    return retVal;
}

DOUBLE CilParser::ConsumeFloat64()
{
    const BYTE byte0 = *(_code + _ip++);
    const BYTE byte1 = *(_code + _ip++);
    const BYTE byte2 = *(_code + _ip++);
    const BYTE byte3 = *(_code + _ip++);
    const BYTE byte4 = *(_code + _ip++);
    const BYTE byte5 = *(_code + _ip++);
    const BYTE byte6 = *(_code + _ip++);
    const BYTE byte7 = *(_code + _ip++);

    const DOUBLE retVal = byte0 | byte1 << 8 | byte2 << 16 | byte3 << 24 | byte4 << 32 | byte5 << 40 | byte6 << 48 | byte7 << 56;
    return retVal;
}
