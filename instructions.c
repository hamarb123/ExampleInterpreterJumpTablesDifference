#include "instructions.h"

int get_immediates_byte_size(instruction_t inst)
{
	if (inst >= Instr_COUNT) return -1;
	if (inst == Instr_LoadConstantByte || inst == Instr_LoadConstantSByte || inst == Instr_JumpByte || inst == Instr_JumpByteConditional || inst == Instr_PopMulti || inst == Instr_PushZeroConstantMulti) return 1;
	if (inst == Instr_LoadConstantShort || inst == Instr_LoadConstantUShort) return 2;
	if (inst == Instr_LoadConstantInt || inst == Instr_LoadConstantInt || inst == Instr_Jump || inst == Instr_JumpConditional) return 4;
	if (inst == Instr_LoadConstantLong) return 8;
	return 0;
}

int get_inputs_count(instruction_t inst)
{
	if (inst >= Instr_COUNT) return -1;
	if (inst >= Instr_Add && inst <= Instr_ShiftRightLogical) return 2;
	if (inst == Instr_Negate || inst == Instr_Not) return 1;
	if (inst >= Instr_LoadByte && inst <= Instr_LoadLong) return 1;
	if (inst >= Instr_StoreByte && inst <= Instr_StoreLong) return 2;
	if (inst >= Instr_Pop && inst <= Instr_PrintChar) return 1;
	if (inst >= Instr_ConvertFromByte && inst <= Instr_ConvertFromUInt) return 1;
	if (inst == Instr_PopMulti) return -1;
	if (inst == Instr_PushZeroConstantMulti) return 0;
	if (inst == Instr_JumpByteConditional || inst == Instr_JumpConditional) return 1;
	return 0;
}

int get_results_count(instruction_t inst)
{
	if (inst >= Instr_COUNT) return -1;
	if (inst >= Instr_Add && inst <= Instr_LoadConstantLong) return 1;
	if (inst >= Instr_LoadByte && inst <= Instr_LoadLong) return 1;
	if (inst >= Instr_ConvertFromByte && inst <= Instr_ConvertFromUInt) return 1;
	if (inst == Instr_PopMulti) return 0;
	if (inst == Instr_PushZeroConstantMulti) return -1;
	return 0;
}
