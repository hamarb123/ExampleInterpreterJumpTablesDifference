#ifndef __INSTRUCTIONS_H
#define __INSTRUCTIONS_H

enum instruction : unsigned char
{
	// Values on the evaluation stack are 64 bits - smaller values are implicitly upcasted when loaded.

	// 0 immediates, 2 operands, 1 result
	Instr_Add = 0x00,
	Instr_Subtract = 0x01,
	Instr_Multiply = 0x02,
	Instr_Divide = 0x03,
	Instr_DivideSigned = 0x04,
	Instr_Modulo = 0x05,
	Instr_ModuloSigned = 0x06,
	Instr_And = 0x07,
	Instr_Or = 0x08,
	Instr_Xor = 0x09,
	Instr_ShiftLeft = 0x0A,
	Instr_ShiftRightArithmetic = 0x0B,
	Instr_ShiftRightLogical = 0x0C,

	// 0 immediates, 1 operand, 1 result
	Instr_Negate = 0x0D,
	Instr_Not = 0x0E,

	// 1 immediate (variable size), 0 operands, 1 result
	Instr_LoadConstantByte = 0x0F,
	Instr_LoadConstantSByte = 0x10,
	Instr_LoadConstantShort = 0x11,
	Instr_LoadConstantUShort = 0x12,
	Instr_LoadConstantInt = 0x13,
	Instr_LoadConstantUInt = 0x14,
	Instr_LoadConstantLong = 0x15,

	// 1 immediate (sbyte / int offset from end of instruction), 0 operands, 0 results
	// NOTE: the evaluation stack size after the instruction and at the jump point have to be equal (regardless of whether conditional or unconditional jump)
	Instr_JumpByte = 0x16,
	Instr_JumpByteConditional = 0x17,
	Instr_Jump = 0x18,
	Instr_JumpConditional = 0x19,

	// 0 immediates, 1 operand, 1 result
	Instr_LoadByte = 0x1A,
	Instr_LoadSByte = 0x1B,
	Instr_LoadShort = 0x1C,
	Instr_LoadUShort = 0x1D,
	Instr_LoadInt = 0x1E,
	Instr_LoadUInt = 0x1F,
	Instr_LoadLong = 0x20,

	// 0 immediates, 2 operands, 0 results
	Instr_StoreByte = 0x21,
	Instr_StoreShort = 0x22,
	Instr_StoreInt = 0x23,
	Instr_StoreLong = 0x24,

	// 0 immediates, 1 operand, 0 results
	// NOTE: the last instruction must be Instr_Exit, which requires an empty evaluation stack other than the 1 value it operates on
	Instr_Pop = 0x25,
	Instr_Exit = 0x26,
	Instr_PrintChar = 0x27, // prints a byte as a character

	// 0 immediates, 1 operand, 1 result
	Instr_ConvertFromByte = 0x28,
	Instr_ConvertFromSByte = 0x29,
	Instr_ConvertFromShort = 0x2A,
	Instr_ConvertFromUShort = 0x2B,
	Instr_ConvertFromInt = 0x2C,
	Instr_ConvertFromUInt = 0x2D,

	// Misc. instructions: 1 immediate (byte), n/0 operands, 0/n results
	Instr_PopMulti = 0x2E,
	Instr_PushZeroConstantMulti = 0x2F,

	// Not an instruction
	Instr_COUNT = 0x30,
};

typedef enum instruction instruction_t;

int get_immediates_byte_size(instruction_t inst);
int get_inputs_count(instruction_t inst);
int get_results_count(instruction_t inst);

#endif
