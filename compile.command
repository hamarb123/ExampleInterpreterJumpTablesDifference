#!/usr/bin/env bash
cd "$(dirname "$0")"

source ~/.profile

gcc-15 -O3 main.c -g instructions.c standard_interpreter.c -o standard_interpreter
gcc-15 -O3 main.c -g instructions.c label_array_interpreter.c -o label_array_interpreter
