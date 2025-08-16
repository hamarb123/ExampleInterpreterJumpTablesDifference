#include "instructions.h"
#include "interpreter.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>

int parse_hex_char(unsigned char c)
{
	if (c >= '0' && c <= '9') return c - '0';
	if (c >= 'A' && c <= 'F') return c - 'A' + 10;
	if (c >= 'a' && c <= 'f') return c - 'a' + 10;
	return -1;
}

int32_t read_int_le(unsigned char * memory)
{
	return memory[0] | ((int32_t)memory[1] << 8) | ((int32_t)memory[2] << 16) | ((int32_t)memory[3] << 24);
}

bool skip_comment(int *c, FILE *fptr)
{
	// Comments start with ';' and continue until the end of the line.
	// Returns non-zero when should exit loop.
	while (*c == ';')
	{
		while ((*c = fgetc(fptr)) != EOF)
		{
			if (*c == '\n') break;
		}
		if (*c == EOF) return true;
		*c = fgetc(fptr);
		if (*c == EOF) return true;
	}
	return false;
}

int main(int argc, char *argv[])
{
	// File format is hex characters with spacing & newlines allowed, and line comments supported that start with ';'
	FILE *fptr = fopen("instructions.txt", "rt");
	int c;
	size_t fileAllocation = 16;
	unsigned char *fileBytes = malloc(fileAllocation);
	size_t fileSize = 0;
	while ((c = fgetc(fptr)) != EOF)
	{
		if (c == ' ' || c == '\n') continue;
		if (skip_comment(&c, fptr)) break;
		if (c == ' ' || c == '\n') continue;
		int hex0 = parse_hex_char((unsigned char)c);
		if (hex0 < 0)
		{
			printf("Invalid character.\n");
			return -1;
		}
		c = fgetc(fptr);
		while (c == ' ' || c == '\n' || c == ';')
		{
			while ((c == ' ' || c == '\n') && (c = fgetc(fptr)) != EOF) ;
			if (c != EOF && skip_comment(&c, fptr)) c = EOF;
		}
		if (c == EOF)
		{
			printf("Invalid hex pair.\n");
			return -1;
		}
		int hex1 = parse_hex_char((unsigned char)c);
		if (hex1 < 0)
		{
			printf("Invalid character.\n");
			return -1;
		}

		unsigned char value = (hex0 << 4) | hex1;
		if (fileSize >= fileAllocation)
		{
			fileAllocation *= 2;
			if (fileAllocation == 1u << 31 || fileAllocation == 0)
			{
				printf("Out of memory.\n");
				return -1;
			}
			fileBytes = realloc(fileBytes, fileAllocation);
		}
		fileBytes[fileSize++] = value;
	}

	/*
		File structure:
		- 4 bytes (signed, little endian) for number of arguments expected
		- 4 bytes (signed, little endian) for evaluation stack size required
		- 4 bytes (signed, little endian) for amount of memory required
		- Program instructions
	*/

	int32_t numArgs, evaluationStackSize, memoryRequired;
	if (fileSize < 12 || (numArgs = read_int_le(fileBytes)) < 0 || (uint32_t)(evaluationStackSize = read_int_le(&fileBytes[4])) > 1024 || evaluationStackSize < numArgs || (memoryRequired = read_int_le(&fileBytes[8])) < 0)
	{
		printf("Invalid file header.\n");
		return -1;
	}
	fileBytes += 12;
	fileSize -= 12;

	// Simple implementation of validation that every instruction has a valid & consistent number of values on the evaluation stack; and that we never jump to something that isn't a valid instruction.
	// Value of -1 means not yet visited or not instruction, any other value means that's how big the evaluation stack is at that point, which we will check for consistency at any jump points.
	int16_t* numEvaluationStackValues = malloc(fileSize * sizeof(int16_t));
	memset(numEvaluationStackValues, -1, fileSize * sizeof(int16_t));
	int32_t stackCount = numArgs;
	instruction_t prevInst = Instr_COUNT;
	for (size_t offset = 0; offset < fileSize; )
	{
		instruction_t inst = (instruction_t)fileBytes[offset];
		int immediatesByteSize = get_immediates_byte_size(inst);
		int inputsCount = get_inputs_count(inst);
		int resultsCount = get_results_count(inst);
		if (immediatesByteSize >= 0)
		{
			if (inputsCount < 0) inputsCount = fileBytes[offset + 1];
			if (resultsCount < 0) resultsCount = fileBytes[offset + 1];
		}
		if (immediatesByteSize < 0 || inputsCount > stackCount || stackCount - inputsCount + resultsCount > evaluationStackSize || offset + 1 + immediatesByteSize > fileSize)
		{
			printf("Invalid instruction stream.\n");
			return -1;
		}
		int32_t nextStackCount = stackCount - inputsCount + resultsCount;
		if (inst == Instr_JumpByte || inst == Instr_JumpByteConditional || inst == Instr_Jump || inst == Instr_JumpConditional)
		{
			int32_t localOffset;
			if (inst == Instr_JumpByte || inst == Instr_JumpByteConditional)
			{
				localOffset = (signed char)fileBytes[offset + 1];
			}
			else
			{
				localOffset = read_int_le(&fileBytes[offset + 1]);
			}
			if ((int64_t)offset + localOffset < 0 || (int64_t)offset + localOffset >= fileSize || ((int64_t)offset + localOffset < (int64_t)offset != localOffset < 0))
			{
				printf("Invalid instruction stream.\n");
				return -1;
			}
			if (localOffset >= 0)
			{
				if (numEvaluationStackValues[offset + 1 + immediatesByteSize + localOffset] < 0)
				{
					numEvaluationStackValues[offset + 1 + immediatesByteSize + localOffset] = nextStackCount;
				}
			}
			if (numEvaluationStackValues[offset + 1 + immediatesByteSize + localOffset] != nextStackCount)
			{
				printf("Invalid instruction stream.\n");
				return -1;
			}
		}
		if (numEvaluationStackValues[offset] >= 0 && numEvaluationStackValues[offset] != stackCount)
		{
			printf("Invalid instruction stream.\n");
			return -1;
		}
		for (int i = 1; i <= immediatesByteSize; i++)
		{
			if (numEvaluationStackValues[offset + i] >= 0)
			{
				printf("Invalid instruction stream.\n");
				return -1;
			}
		}
		numEvaluationStackValues[offset] = stackCount;
		stackCount = nextStackCount;
		offset += 1 + immediatesByteSize;
		if (inst == Instr_Exit && stackCount != 0)
		{
			printf("Invalid instruction stream.\n");
			return -1;
		}
		prevInst = inst;
	}
	if (prevInst != Instr_Exit)
	{
		printf("Invalid instruction stream.\n");
		return -1;
	}

	// Parse the arguments
	uint64_t* evaluationStack = malloc(sizeof(uint64_t) * evaluationStackSize);
	if (argc != 1 + numArgs)
	{
		printf("Expected %i arguments to the program.\n", numArgs);
		return -1;
	}
	for (int i = 0; i < numArgs; i++)
	{
		char* endptr;
		evaluationStack[i] = strtoull(argv[i + 1], &endptr, 10);
		if (*endptr != '\0')
		{
			printf("Argument %i was not a valid uint64_t.\n", numArgs);
			return -1;
		}
	}

	// Run the program
	unsigned char* memory = malloc(memoryRequired);
	memset(memory, 0, memoryRequired);
	run_validated(fileBytes, evaluationStack + numArgs, memory, memoryRequired);
}
