#pragma once
#define OPDEF( id, s, pop, push, args, type, l, OpCode1, OpCode2, ctrl ) id,
typedef enum enumOpcode
{
#include "opcode.def"
    CEE_COUNT,/* number of instructions and macros pre-defined */
} OPCODE;
#undef OPDEF
