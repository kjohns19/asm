#ifndef ISA_H
#define ISA_H

#include <stdint.h>
#include "errors.h"
#include "memory.h"

/*Instruction struct
  The opcode determines the type
     R - 3 registers, a shift amount, and a function type
     I - 2 registers and a 16-bit immediate signed value
     J - A 26-bit address*/
typedef struct {
   union {
      struct {
         int8_t r3;
         int8_t sh;
         int8_t fn;
      };
      int16_t imm;
      int32_t addr;
   };
   int8_t op;
   int8_t r1;
   int8_t r2;
} instr_t;

/*Initializes the registers*/
void init_registers();

/*Gets a register index from its name.
  Returns -1 if the register does not exist.*/
int get_reg_index(const char* name);

/*Gets a register name from its index.
  Returns NULL if the register does not exist.*/
const char* get_register_name(int index);


/*Encodes an instr_t into a 32-bit unsigned value*/
uint32_t encode_instr(const instr_t* instr);
/*Decodes a 32-bit unsigned value into an instr_t*/
void decode_instr(instr_t* instr, uint32_t value);

/*Executes the next instruction*/
int execute_next();

/*Executes an instruction*/
int execute(uint32_t instr);

/*Executes an instruction*/
int execute_instr(const instr_t* instr);

extern int32_t registers[];
extern int32_t HI, LO;
extern uint32_t PC;

extern int debugMode;

#endif //ISA_H
