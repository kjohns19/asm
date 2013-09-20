cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(ASM)

add_executable(Compiler src/compiler.c src/util/compiler.c src/util/isa.c src/util/memory.c src/util/r_instr.c src/util/i_instr.c src/util/j_instr.c)
add_executable(Processor src/processor.c src/util/isa.c src/util/memory.c src/util/r_instr.c src/util/i_instr.c src/util/j_instr.c)
