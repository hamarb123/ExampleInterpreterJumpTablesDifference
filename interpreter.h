#ifndef __INTERPRETER_H
#define __INTERPRETER_H

#include "instructions.h"

#include <stddef.h>
#include <stdint.h>

uint64_t run_validated(unsigned char *instructions, uint64_t* evaluationStack, unsigned char *memory, size_t memorySize);

#endif
