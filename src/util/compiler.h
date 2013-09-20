#ifndef COMPILER_H
#define COMPILER_H

#include <stdint.h>
#include "isa.h"

#define ARG_NUL 0
#define ARG_REG 1
#define ARG_IMM 2
#define ARG_ADR 3

#define INSTR_VALID           0
#define INSTR_BLANK           1
#define INSTR_UNKNOWN         2
#define INSTR_TOO_MANY_ARGS   3
#define INSTR_TOO_FEW_ARGS    4
#define INSTR_WRONG_ARG       5

#define DATA_VALID      0
#define DATA_BLANK      1
#define DATA_NO_VALUE   2
#define DATA_INVALID    3
#define DATA_UNKNOWN    4
#define DATA_LARGE      5

/*An instruction type.
  name - The name
  type - The type (can be 'r', 'i', or 'j')
  code - The op code (for 'i' or 'j') or fn (for 'r') value
  args - The argument types for the instruction.*/
typedef struct {
   char* name;
   char type;
   uint8_t code;
   uint8_t args[5];
} instr_type_t;

/*Initializes the instructions*/
void init_instructions();

/*Gets the instruction type from its name.
  Returns NULL if the instruction does not exist.*/
const instr_type_t* get_instr_type(char* name);

/*Gets a number from a string.
  Puts the number in the pointer and returns 1 on success.
  If there is an error, 0 is returned and the number is unchanged.*/
int getNumber(const char* str, int* number);

/*Parses an instruction from a string. Returns a non-zero value
  if an error occurs.*/
int parse_instr(const char* line, int lineNum, instr_t* instr);

/*Parses data from a string. Returns a non-zero value
  if an error occurs.*/
int parse_data(const char* line, int lineNum, uint8_t* data, int* count);

#endif //COMPILER_H
