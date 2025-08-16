#include "interpreter.h"
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>

uint64_t run_validated(unsigned char *instructions, uint64_t* evaluationStack, unsigned char *memory, size_t memorySize)
{
	while (true)
	{
		switch (*instructions)
		{
			case Instr_Add:
			{
				evaluationStack[-2] += evaluationStack[-1];
				evaluationStack--;
				instructions++;
				break;
			}
			case Instr_Subtract:
			{
				evaluationStack[-2] -= evaluationStack[-1];
				evaluationStack--;
				instructions++;
				break;
			}
			case Instr_Multiply:
			{
				evaluationStack[-2] *= evaluationStack[-1];
				evaluationStack--;
				instructions++;
				break;
			}
			case Instr_Divide:
			{
				if (evaluationStack[-1] == 0) evaluationStack[-2] = 0;
				else evaluationStack[-2] /= evaluationStack[-1];
				evaluationStack--;
				instructions++;
				break;
			}
			case Instr_DivideSigned:
			{
				if (evaluationStack[-1] == 0) evaluationStack[-2] = 0;
				else if ((int64_t)evaluationStack[-2] == LONG_LONG_MIN && evaluationStack[-1] == -1) ;
				else evaluationStack[-2] = (uint64_t)((int64_t)evaluationStack[-2] / (int64_t)evaluationStack[-1]);
				evaluationStack--;
				instructions++;
				break;
			}
			case Instr_Modulo:
			{
				if (evaluationStack[-1] == 0) ;
				else evaluationStack[-2] %= evaluationStack[-1];
				evaluationStack--;
				instructions++;
				break;
			}
			case Instr_ModuloSigned:
			{
				if (evaluationStack[-1] == 0) ;
				else if ((int64_t)evaluationStack[-2] == LONG_LONG_MIN && evaluationStack[-1] == -1) evaluationStack[-2] = 0;
				else evaluationStack[-2] = (uint64_t)((int64_t)evaluationStack[-2] % (int64_t)evaluationStack[-1]);
				evaluationStack--;
				instructions++;
				break;
			}
			case Instr_And:
			{
				evaluationStack[-2] &= evaluationStack[-1];
				evaluationStack--;
				instructions++;
				break;
			}
			case Instr_Or:
			{
				evaluationStack[-2] |= evaluationStack[-1];
				evaluationStack--;
				instructions++;
				break;
			}
			case Instr_Xor:
			{
				evaluationStack[-2] ^= evaluationStack[-1];
				evaluationStack--;
				instructions++;
				break;
			}
			case Instr_ShiftLeft:
			{
				evaluationStack[-2] <<= evaluationStack[-1] & 63;
				evaluationStack--;
				instructions++;
				break;
			}
			case Instr_ShiftRightArithmetic:
			{
				evaluationStack[-2] = (uint64_t)(evaluationStack[-2] >> (int64_t)(evaluationStack[-1] & 63));
				evaluationStack--;
				instructions++;
				break;
			}
			case Instr_ShiftRightLogical:
			{
				evaluationStack[-2] >>= evaluationStack[-1] & 63;
				evaluationStack--;
				instructions++;
				break;
			}
			case Instr_Negate:
			{
				evaluationStack[-1] = -evaluationStack[-1];
				instructions++;
				break;
			}
			case Instr_Not:
			{
				evaluationStack[-1] = (evaluationStack[-1] == 0) ? 1 : 0;
				instructions++;
				break;
			}
			case Instr_LoadConstantByte:
			{
				evaluationStack[0] = (unsigned char)instructions[1];
				evaluationStack++;
				instructions += 2;
				break;
			}
			case Instr_LoadConstantSByte:
			{
				evaluationStack[0] = (uint64_t)(int64_t)(signed char)instructions[1];
				evaluationStack++;
				instructions += 2;
				break;
			}
			case Instr_LoadConstantShort:
			{
				//evaluationStack[0] = (uint64_t)(int64_t)(int16_t)(instructions[1] | ((uint16_t)instructions[2] << 8));
				memcpy(evaluationStack, &instructions[1], 2);
				evaluationStack[0] = (uint64_t)(int64_t)(int16_t)*evaluationStack;
				evaluationStack++;
				instructions += 3;
				break;
			}
			case Instr_LoadConstantUShort:
			{
				//evaluationStack[0] = (uint16_t)(instructions[1] | ((uint16_t)instructions[2] << 8));
				memcpy(evaluationStack, &instructions[1], 2);
				evaluationStack[0] = (uint64_t)(uint16_t)*evaluationStack;
				evaluationStack++;
				instructions += 3;
				break;
			}
			case Instr_LoadConstantInt:
			{
				//evaluationStack[0] = (uint64_t)(int64_t)(int32_t)(instructions[1] | ((uint16_t)instructions[2] << 8) | ((uint32_t)instructions[3] << 16) | ((uint32_t)instructions[4] << 24));
				memcpy(evaluationStack, &instructions[1], 4);
				evaluationStack[0] = (uint64_t)(int64_t)(int32_t)*evaluationStack;
				evaluationStack++;
				instructions += 5;
				break;
			}
			case Instr_LoadConstantUInt:
			{
				//evaluationStack[0] = (uint32_t)(instructions[1] | ((uint16_t)instructions[2] << 8) | ((uint32_t)instructions[3] << 16) | ((uint32_t)instructions[4] << 24));
				memcpy(evaluationStack, &instructions[1], 4);
				evaluationStack[0] = (uint64_t)(uint32_t)*evaluationStack;
				evaluationStack++;
				instructions += 5;
				break;
			}
			case Instr_LoadConstantLong:
			{
				//evaluationStack[0] = (uint64_t)(instructions[1] | ((uint16_t)instructions[2] << 8) | ((uint32_t)instructions[3] << 16) | ((uint32_t)instructions[4] << 24) | ((uint64_t)instructions[5] << 32) | ((uint64_t)instructions[6] << 40) | ((uint64_t)instructions[7] << 48) | ((uint64_t)instructions[8] << 56));
				memcpy(evaluationStack, &instructions[1], 8);
				evaluationStack++;
				instructions += 9;
				break;
			}
			case Instr_JumpByte:
			{
				int offset = (signed char)instructions[1];
				instructions += 2 + offset;
				break;
			}
			case Instr_JumpByteConditional:
			{
				int offset = (signed char)instructions[1];
				evaluationStack--;
				instructions += 2 + (*evaluationStack != 0 ? offset : 0);
				break;
			}
			case Instr_Jump:
			{
				//int offset = (ssize_t)(int32_t)(instructions[1] | ((uint16_t)instructions[2] << 8) | ((uint32_t)instructions[3] << 16) | ((uint32_t)instructions[4] << 24));
				int offset;
				memcpy(&offset, &instructions[1], 4);
				instructions += 5 + offset;
				break;
			}
			case Instr_JumpConditional:
			{
				//int offset = (ssize_t)(int32_t)(instructions[1] | ((uint16_t)instructions[2] << 8) | ((uint32_t)instructions[3] << 16) | ((uint32_t)instructions[4] << 24));
				int offset;
				memcpy(&offset, &instructions[1], 4);
				evaluationStack--;
				instructions += 5 + (*evaluationStack != 0 ? offset : 0);
				break;
			}
			case Instr_LoadByte:
			{
				uint64_t offset = evaluationStack[-1];
				if (offset >= memorySize)
				{
					printf("invalid memory access.\n");
					return 1;
				}
				evaluationStack[-1] = memory[offset];
				instructions++;
				break;
			}
			case Instr_LoadSByte:
			{
				uint64_t offset = evaluationStack[-1];
				if (offset >= memorySize)
				{
					printf("invalid memory access.\n");
					return 1;
				}
				evaluationStack[-1] = (uint64_t)(int64_t)(signed char)memory[offset];
				instructions++;
				break;
			}
			case Instr_LoadShort:
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
				break;
			}
			case Instr_LoadUShort:
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
				break;
			}
			case Instr_LoadInt:
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
				break;
			}
			case Instr_LoadUInt:
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
				break;
			}
			case Instr_LoadLong:
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
				break;
			}
			case Instr_StoreByte:
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
				break;
			}
			case Instr_StoreShort:
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
				break;
			}
			case Instr_StoreInt:
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
				break;
			}
			case Instr_StoreLong:
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
				break;
			}
			case Instr_Pop:
			{
				evaluationStack--;
				instructions++;
				break;
			}
			case Instr_Exit:
			{
				return evaluationStack[-1];
			}
			case Instr_PrintChar:
			{
				printf("%c", (char)evaluationStack[-1]);
				evaluationStack--;
				instructions++;
				break;
			}
			case Instr_ConvertFromByte:
			{
				evaluationStack[-1] = (unsigned char)evaluationStack[-1];
				instructions++;
				break;
			}
			case Instr_ConvertFromSByte:
			{
				evaluationStack[-1] = (uint64_t)(int64_t)(signed char)evaluationStack[-1];
				instructions++;
				break;
			}
			case Instr_ConvertFromShort:
			{
				evaluationStack[-1] = (uint64_t)(int64_t)(int16_t)evaluationStack[-1];
				instructions++;
				break;
			}
			case Instr_ConvertFromUShort:
			{
				evaluationStack[-1] = (uint16_t)evaluationStack[-1];
				instructions++;
				break;
			}
			case Instr_ConvertFromInt:
			{
				evaluationStack[-1] = (uint64_t)(int64_t)(int32_t)evaluationStack[-1];
				instructions++;
				break;
			}
			case Instr_ConvertFromUInt:
			{
				evaluationStack[-1] = (int32_t)evaluationStack[-1];
				instructions++;
				break;
			}
			case Instr_PopMulti:
			{
				evaluationStack -= instructions[1];
				instructions += 2;
				break;
			}
			case Instr_PushZeroConstantMulti:
			{
				memset(evaluationStack, 0, instructions[1] * sizeof(uint64_t));
				evaluationStack += instructions[1];
				instructions += 2;
				break;
			}
		}
	}
}
