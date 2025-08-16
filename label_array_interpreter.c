#include "interpreter.h"
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>

uint64_t run_validated(unsigned char *instructions, uint64_t* evaluationStack, unsigned char *memory, size_t memorySize)
{
	static const int ImplArray[] =
	{
		&&Instr_Add_Impl - &&Offset,
		&&Instr_Subtract_Impl - &&Offset,
		&&Instr_Multiply_Impl - &&Offset,
		&&Instr_Divide_Impl - &&Offset,
		&&Instr_DivideSigned_Impl - &&Offset,
		&&Instr_Modulo_Impl - &&Offset,
		&&Instr_ModuloSigned_Impl - &&Offset,
		&&Instr_And_Impl - &&Offset,
		&&Instr_Or_Impl - &&Offset,
		&&Instr_Xor_Impl - &&Offset,
		&&Instr_ShiftLeft_Impl - &&Offset,
		&&Instr_ShiftRightArithmetic_Impl - &&Offset,
		&&Instr_ShiftRightLogical_Impl - &&Offset,
		&&Instr_Negate_Impl - &&Offset,
		&&Instr_Not_Impl - &&Offset,
		&&Instr_LoadConstantByte_Impl - &&Offset,
		&&Instr_LoadConstantSByte_Impl - &&Offset,
		&&Instr_LoadConstantShort_Impl - &&Offset,
		&&Instr_LoadConstantUShort_Impl - &&Offset,
		&&Instr_LoadConstantInt_Impl - &&Offset,
		&&Instr_LoadConstantUInt_Impl - &&Offset,
		&&Instr_LoadConstantLong_Impl - &&Offset,
		&&Instr_JumpByte_Impl - &&Offset,
		&&Instr_JumpByteConditional_Impl - &&Offset,
		&&Instr_Jump_Impl - &&Offset,
		&&Instr_JumpConditional_Impl - &&Offset,
		&&Instr_LoadByte_Impl - &&Offset,
		&&Instr_LoadSByte_Impl - &&Offset,
		&&Instr_LoadShort_Impl - &&Offset,
		&&Instr_LoadUShort_Impl - &&Offset,
		&&Instr_LoadInt_Impl - &&Offset,
		&&Instr_LoadUInt_Impl - &&Offset,
		&&Instr_LoadLong_Impl - &&Offset,
		&&Instr_StoreByte_Impl - &&Offset,
		&&Instr_StoreShort_Impl - &&Offset,
		&&Instr_StoreInt_Impl - &&Offset,
		&&Instr_StoreLong_Impl - &&Offset,
		&&Instr_Pop_Impl - &&Offset,
		&&Instr_Exit_Impl - &&Offset,
		&&Instr_PrintChar_Impl - &&Offset,
		&&Instr_ConvertFromByte_Impl - &&Offset,
		&&Instr_ConvertFromSByte_Impl - &&Offset,
		&&Instr_ConvertFromShort_Impl - &&Offset,
		&&Instr_ConvertFromUShort_Impl - &&Offset,
		&&Instr_ConvertFromInt_Impl - &&Offset,
		&&Instr_ConvertFromUInt_Impl - &&Offset,
		&&Instr_PopMulti_Impl - &&Offset,
		&&Instr_PushZeroConstantMulti_Impl - &&Offset,
	};
	goto *(&&Offset + ImplArray[*instructions]);
	Instr_Add_Impl:
	{
		evaluationStack[-2] += evaluationStack[-1];
		evaluationStack--;
		instructions++;
		goto *(&&Offset + ImplArray[*instructions]);
	}
	Instr_Subtract_Impl:
	{
		evaluationStack[-2] -= evaluationStack[-1];
		evaluationStack--;
		instructions++;
		goto *(&&Offset + ImplArray[*instructions]);
	}
	Instr_Multiply_Impl:
	{
		evaluationStack[-2] *= evaluationStack[-1];
		evaluationStack--;
		instructions++;
		goto *(&&Offset + ImplArray[*instructions]);
	}
	Instr_Divide_Impl:
	{
		if (evaluationStack[-1] == 0) evaluationStack[-2] = 0;
		else evaluationStack[-2] /= evaluationStack[-1];
		evaluationStack--;
		instructions++;
		goto *(&&Offset + ImplArray[*instructions]);
	}
	Instr_DivideSigned_Impl:
	{
		if (evaluationStack[-1] == 0) evaluationStack[-2] = 0;
		else if ((int64_t)evaluationStack[-2] == LONG_LONG_MIN && evaluationStack[-1] == -1) ;
		else evaluationStack[-2] = (uint64_t)((int64_t)evaluationStack[-2] / (int64_t)evaluationStack[-1]);
		evaluationStack--;
		instructions++;
		goto *(&&Offset + ImplArray[*instructions]);
	}
	Instr_Modulo_Impl:
	{
		if (evaluationStack[-1] == 0) ;
		else evaluationStack[-2] %= evaluationStack[-1];
		evaluationStack--;
		instructions++;
		goto *(&&Offset + ImplArray[*instructions]);
	}
	Instr_ModuloSigned_Impl:
	{
		if (evaluationStack[-1] == 0) ;
		else if ((int64_t)evaluationStack[-2] == LONG_LONG_MIN && evaluationStack[-1] == -1) evaluationStack[-2] = 0;
		else evaluationStack[-2] = (uint64_t)((int64_t)evaluationStack[-2] % (int64_t)evaluationStack[-1]);
		evaluationStack--;
		instructions++;
		goto *(&&Offset + ImplArray[*instructions]);
	}
	Instr_And_Impl:
	{
		evaluationStack[-2] &= evaluationStack[-1];
		evaluationStack--;
		instructions++;
		goto *(&&Offset + ImplArray[*instructions]);
	}
	Instr_Or_Impl:
	{
		evaluationStack[-2] |= evaluationStack[-1];
		evaluationStack--;
		instructions++;
		goto *(&&Offset + ImplArray[*instructions]);
	}
	Instr_Xor_Impl:
	{
		evaluationStack[-2] ^= evaluationStack[-1];
		evaluationStack--;
		instructions++;
		goto *(&&Offset + ImplArray[*instructions]);
	}
	Instr_ShiftLeft_Impl:
	{
		evaluationStack[-2] <<= evaluationStack[-1] & 63;
		evaluationStack--;
		instructions++;
		goto *(&&Offset + ImplArray[*instructions]);
	}
	Instr_ShiftRightArithmetic_Impl:
	{
		evaluationStack[-2] = (uint64_t)(evaluationStack[-2] >> (int64_t)(evaluationStack[-1] & 63));
		evaluationStack--;
		instructions++;
		goto *(&&Offset + ImplArray[*instructions]);
	}
	Instr_ShiftRightLogical_Impl:
	{
		evaluationStack[-2] >>= evaluationStack[-1] & 63;
		evaluationStack--;
		instructions++;
		goto *(&&Offset + ImplArray[*instructions]);
	}
	Instr_Negate_Impl:
	{
		evaluationStack[-1] = -evaluationStack[-1];
		instructions++;
		goto *(&&Offset + ImplArray[*instructions]);
	}
	Instr_Not_Impl:
	{
		evaluationStack[-1] = (evaluationStack[-1] == 0) ? 1 : 0;
		instructions++;
		goto *(&&Offset + ImplArray[*instructions]);
	}
	Instr_LoadConstantByte_Impl:
	{
		evaluationStack[0] = (unsigned char)instructions[1];
		evaluationStack++;
		instructions += 2;
		goto *(&&Offset + ImplArray[*instructions]);
	}
	Instr_LoadConstantSByte_Impl:
	{
		evaluationStack[0] = (uint64_t)(int64_t)(signed char)instructions[1];
		evaluationStack++;
		instructions += 2;
		goto *(&&Offset + ImplArray[*instructions]);
	}
	Instr_LoadConstantShort_Impl:
	{
		//evaluationStack[0] = (uint64_t)(int64_t)(int16_t)(instructions[1] | ((uint16_t)instructions[2] << 8));
		memcpy(evaluationStack, &instructions[1], 2);
		evaluationStack[0] = (uint64_t)(int64_t)(int16_t)*evaluationStack;
		evaluationStack++;
		instructions += 3;
		goto *(&&Offset + ImplArray[*instructions]);
	}
	Instr_LoadConstantUShort_Impl:
	{
		//evaluationStack[0] = (uint16_t)(instructions[1] | ((uint16_t)instructions[2] << 8));
		memcpy(evaluationStack, &instructions[1], 2);
		evaluationStack[0] = (uint64_t)(uint16_t)*evaluationStack;
		evaluationStack++;
		instructions += 3;
		goto *(&&Offset + ImplArray[*instructions]);
	}
	Instr_LoadConstantInt_Impl:
	{
		//evaluationStack[0] = (uint64_t)(int64_t)(int32_t)(instructions[1] | ((uint16_t)instructions[2] << 8) | ((uint32_t)instructions[3] << 16) | ((uint32_t)instructions[4] << 24));
		memcpy(evaluationStack, &instructions[1], 4);
		evaluationStack[0] = (uint64_t)(int64_t)(int32_t)*evaluationStack;
		evaluationStack++;
		instructions += 5;
		goto *(&&Offset + ImplArray[*instructions]);
	}
	Instr_LoadConstantUInt_Impl:
	{
		//evaluationStack[0] = (uint32_t)(instructions[1] | ((uint16_t)instructions[2] << 8) | ((uint32_t)instructions[3] << 16) | ((uint32_t)instructions[4] << 24));
		memcpy(evaluationStack, &instructions[1], 4);
		evaluationStack[0] = (uint64_t)(uint32_t)*evaluationStack;
		evaluationStack++;
		instructions += 5;
		goto *(&&Offset + ImplArray[*instructions]);
	}
	Instr_LoadConstantLong_Impl:
	{
		//evaluationStack[0] = (uint64_t)(instructions[1] | ((uint16_t)instructions[2] << 8) | ((uint32_t)instructions[3] << 16) | ((uint32_t)instructions[4] << 24) | ((uint64_t)instructions[5] << 32) | ((uint64_t)instructions[6] << 40) | ((uint64_t)instructions[7] << 48) | ((uint64_t)instructions[8] << 56));
		memcpy(evaluationStack, &instructions[1], 8);
		evaluationStack++;
		instructions += 9;
		goto *(&&Offset + ImplArray[*instructions]);
	}
	Instr_JumpByte_Impl:
	{
		int offset = (signed char)instructions[1];
		instructions += 2 + offset;
		goto *(&&Offset + ImplArray[*instructions]);
	}
	Instr_JumpByteConditional_Impl:
	{
		int offset = (signed char)instructions[1];
		evaluationStack--;
		instructions += 2 + (*evaluationStack != 0 ? offset : 0);
		goto *(&&Offset + ImplArray[*instructions]);
	}
	Instr_Jump_Impl:
	{
		//int offset = (ssize_t)(int32_t)(instructions[1] | ((uint16_t)instructions[2] << 8) | ((uint32_t)instructions[3] << 16) | ((uint32_t)instructions[4] << 24));
		int offset;
		memcpy(&offset, &instructions[1], 4);
		instructions += 5 + offset;
		goto *(&&Offset + ImplArray[*instructions]);
	}
	Instr_JumpConditional_Impl:
	{
		//int offset = (ssize_t)(int32_t)(instructions[1] | ((uint16_t)instructions[2] << 8) | ((uint32_t)instructions[3] << 16) | ((uint32_t)instructions[4] << 24));
		int offset;
		memcpy(&offset, &instructions[1], 4);
		evaluationStack--;
		instructions += 5 + (*evaluationStack != 0 ? offset : 0);
		goto *(&&Offset + ImplArray[*instructions]);
	}
	Instr_LoadByte_Impl:
	{
		uint64_t offset = evaluationStack[-1];
		if (offset >= memorySize)
		{
			printf("invalid memory access.\n");
			return 1;
		}
		evaluationStack[-1] = memory[offset];
		instructions++;
		goto *(&&Offset + ImplArray[*instructions]);
	}
	Instr_LoadSByte_Impl:
	{
		uint64_t offset = evaluationStack[-1];
		if (offset >= memorySize)
		{
			printf("invalid memory access.\n");
			return 1;
		}
		evaluationStack[-1] = (uint64_t)(int64_t)(signed char)memory[offset];
		instructions++;
		goto *(&&Offset + ImplArray[*instructions]);
	}
	Instr_LoadShort_Impl:
	{
		uint64_t offset = evaluationStack[-1];
		if (offset >= memorySize || offset + 1 >= memorySize)
		{
			printf("invalid memory access.\n");
			return 1;
		}
		//evaluationStack[-1] = (uint64_t)(int64_t)(int16_t)(memory[offset] | ((uint16_t)memory[offset + 1] << 8));
		memcpy(&evaluationStack[-1], &memory[offset], 2);
		evaluationStack[-1] = (uint64_t)(int64_t)(int16_t)evaluationStack[-1];
		instructions++;
		goto *(&&Offset + ImplArray[*instructions]);
	}
	Instr_LoadUShort_Impl:
	{
		uint64_t offset = evaluationStack[-1];
		if (offset >= memorySize || offset + 1 >= memorySize)
		{
			printf("invalid memory access.\n");
			return 1;
		}
		//evaluationStack[-1] = (uint16_t)(memory[offset] | ((uint16_t)memory[offset + 1] << 8));
		memcpy(&evaluationStack[-1], &memory[offset], 2);
		evaluationStack[-1] = (uint64_t)(uint16_t)evaluationStack[-1];
		instructions++;
		goto *(&&Offset + ImplArray[*instructions]);
	}
	Instr_LoadInt_Impl:
	{
		uint64_t offset = evaluationStack[-1];
		if (offset >= memorySize || offset + 3 >= memorySize)
		{
			printf("invalid memory access.\n");
			return 1;
		}
		//evaluationStack[-1] = (uint64_t)(int64_t)(int32_t)(memory[offset] | ((uint16_t)memory[offset + 1] << 8) | ((uint32_t)memory[offset + 2] << 16) | ((uint32_t)memory[offset + 3] << 24));
		memcpy(&evaluationStack[-1], &memory[offset], 4);
		evaluationStack[-1] = (uint64_t)(int64_t)(int32_t)evaluationStack[-1];
		instructions++;
		goto *(&&Offset + ImplArray[*instructions]);
	}
	Instr_LoadUInt_Impl:
	{
		uint64_t offset = evaluationStack[-1];
		if (offset >= memorySize || offset + 3 >= memorySize)
		{
			printf("invalid memory access.\n");
			return 1;
		}
		//evaluationStack[-1] = (uint32_t)(memory[offset] | ((uint16_t)memory[offset + 1] << 8) | ((uint32_t)memory[offset + 2] << 16) | ((uint32_t)memory[offset + 3] << 24));
		memcpy(&evaluationStack[-1], &memory[offset], 4);
		evaluationStack[-1] = (uint64_t)(uint32_t)evaluationStack[-1];
		instructions++;
		goto *(&&Offset + ImplArray[*instructions]);
	}
	Instr_LoadLong_Impl:
	{
		uint64_t offset = evaluationStack[-1];
		if (offset >= memorySize || offset + 7 >= memorySize)
		{
			printf("invalid memory access.\n");
			return 1;
		}
		//evaluationStack[-1] = (uint64_t)(memory[offset] | ((uint16_t)memory[offset + 1] << 8) | ((uint32_t)memory[offset + 2] << 16) | ((uint32_t)memory[offset + 3] << 24) | ((uint64_t)memory[offset + 4] << 32) | ((uint64_t)memory[offset + 5] << 40) | ((uint64_t)memory[offset + 6] << 48) | ((uint64_t)memory[offset + 7] << 56));
		memcpy(&evaluationStack[-1], &memory[offset], 8);
		instructions++;
		goto *(&&Offset + ImplArray[*instructions]);
	}
	Instr_StoreByte_Impl:
	{
		uint64_t offset = evaluationStack[-1];
		if (offset >= memorySize)
		{
			printf("invalid memory access.\n");
			return 1;
		}
		evaluationStack -= 2;
		memory[offset] = (unsigned char)*evaluationStack;
		instructions++;
		goto *(&&Offset + ImplArray[*instructions]);
	}
	Instr_StoreShort_Impl:
	{
		uint64_t offset = evaluationStack[-1];
		if (offset >= memorySize || offset + 1 >= memorySize)
		{
			printf("invalid memory access.\n");
			return 1;
		}
		evaluationStack -= 2;
		memcpy(&memory[offset], evaluationStack, 2);
		//memory[offset] = (unsigned char)*evaluationStack;
		//memory[offset + 1] = (unsigned char)(*evaluationStack >> 8);
		instructions++;
		goto *(&&Offset + ImplArray[*instructions]);
	}
	Instr_StoreInt_Impl:
	{
		uint64_t offset = evaluationStack[-1];
		if (offset >= memorySize || offset + 3 >= memorySize)
		{
			printf("invalid memory access.\n");
			return 1;
		}
		evaluationStack -= 2;
		memcpy(&memory[offset], evaluationStack, 4);
		//memory[offset] = (unsigned char)*evaluationStack;
		//memory[offset + 1] = (unsigned char)(*evaluationStack >> 8);
		//memory[offset + 2] = (unsigned char)(*evaluationStack >> 16);
		//memory[offset + 3] = (unsigned char)(*evaluationStack >> 24);
		instructions++;
		goto *(&&Offset + ImplArray[*instructions]);
	}
	Instr_StoreLong_Impl:
	{
		uint64_t offset = evaluationStack[-1];
		if (offset >= memorySize || offset + 7 >= memorySize)
		{
			printf("invalid memory access.\n");
			return 1;
		}
		evaluationStack -= 2;
		memcpy(&memory[offset], evaluationStack, 8);
		//memory[offset] = (unsigned char)*evaluationStack;
		//memory[offset + 1] = (unsigned char)(*evaluationStack >> 8);
		//memory[offset + 2] = (unsigned char)(*evaluationStack >> 16);
		//memory[offset + 3] = (unsigned char)(*evaluationStack >> 24);
		//memory[offset + 4] = (unsigned char)(*evaluationStack >> 32);
		//memory[offset + 5] = (unsigned char)(*evaluationStack >> 40);
		//memory[offset + 6] = (unsigned char)(*evaluationStack >> 48);
		//memory[offset + 7] = (unsigned char)(*evaluationStack >> 56);
		instructions++;
		goto *(&&Offset + ImplArray[*instructions]);
	}
	Instr_Pop_Impl:
	{
		evaluationStack--;
		instructions++;
		goto *(&&Offset + ImplArray[*instructions]);
	}
	Instr_Exit_Impl:
	{
		return evaluationStack[-1];
	}
	Instr_PrintChar_Impl:
	{
		printf("%c", (char)evaluationStack[-1]);
		evaluationStack--;
		instructions++;
		goto *(&&Offset + ImplArray[*instructions]);
	}
	Instr_ConvertFromByte_Impl:
	{
		evaluationStack[-1] = (unsigned char)evaluationStack[-1];
		instructions++;
		goto *(&&Offset + ImplArray[*instructions]);
	}
	Instr_ConvertFromSByte_Impl:
	{
		evaluationStack[-1] = (uint64_t)(int64_t)(signed char)evaluationStack[-1];
		instructions++;
		goto *(&&Offset + ImplArray[*instructions]);
	}
	Instr_ConvertFromShort_Impl:
	{
		evaluationStack[-1] = (uint64_t)(int64_t)(int16_t)evaluationStack[-1];
		instructions++;
		goto *(&&Offset + ImplArray[*instructions]);
	}
	Instr_ConvertFromUShort_Impl:
	{
		evaluationStack[-1] = (uint16_t)evaluationStack[-1];
		instructions++;
		goto *(&&Offset + ImplArray[*instructions]);
	}
	Instr_ConvertFromInt_Impl:
	{
		evaluationStack[-1] = (uint64_t)(int64_t)(int32_t)evaluationStack[-1];
		instructions++;
		goto *(&&Offset + ImplArray[*instructions]);
	}
	Instr_ConvertFromUInt_Impl:
	{
		evaluationStack[-1] = (int32_t)evaluationStack[-1];
		instructions++;
		goto *(&&Offset + ImplArray[*instructions]);
	}
	Instr_PopMulti_Impl:
	{
		evaluationStack -= instructions[1];
		instructions += 2;
		goto *(&&Offset + ImplArray[*instructions]);
	}
	Instr_PushZeroConstantMulti_Impl:
	{
		memset(evaluationStack, 0, instructions[1] * sizeof(uint64_t));
		evaluationStack += instructions[1];
		instructions += 2;
		goto *(&&Offset + ImplArray[*instructions]);
	}
	Offset:
}
