#include <string.h>
#include <stdio.h>
#include "isa.h"
#include "instructions.h"
#include "memory.h"
#include "debug.h"

#define J_OP_START 0x38

int32_t registers[32];
int32_t HI;
int32_t LO;
uint32_t PC;

int debugMode = 0;

/*Initializes the registers*/
void init_registers()
{
   memset(registers, 0, 32*sizeof(int32_t));
   registers[29] = MEM_SIZE;
   HI = 0;
   LO = 0;
   PC = 0;
}

const char* reg_names[] = {
   "$ze", "$at", "$v0", "$v1",
   "$a0", "$a1", "$a2", "$a3",
   "$a4", "$s0", "$s1", "$s2",
   "$s3", "$s4", "$s5", "$s6",
   "$s7", "$s8", "$s9", "$t0",
   "$t1", "$t2", "$t3", "$t4",
   "$t5", "$t6", "$t7", "$t8",
   "$t9", "$sp", "$fp", "$ra"
};

const char* reg_sorted[] = {
   "$a0", "$a1", "$a2", "$a3",
   "$a4", "$at", "$fp", "$ra",
   "$s0", "$s1", "$s2", "$s3",
   "$s4", "$s5", "$s6", "$s7",
   "$s8", "$s9", "$sp", "$t0",
   "$t1", "$t2", "$t3", "$t4",
   "$t5", "$t6", "$t7", "$t8",
   "$t9", "$v0", "$v1", "$ze",
};

const int reg_indices[] = {
   4, 5, 6, 7,
   8, 1, 30, 31,
   9, 10, 11, 12,
   13, 14, 15, 16,
   17, 18, 29, 19,
   20, 21, 22, 23,
   24, 25, 26, 27,
   28, 2, 3, 0
};

/*Gets a register index from its name.
  Returns -1 if the register does not exist.*/
int get_reg_index(const char* name)
{
   int left = 0;
   int right = 31;
   int mid, cmp;

   if (!name)
      return -1;

   while(left <= right)
   {
      mid = (left+right)/2;
      cmp = strcmp(name, reg_sorted[mid]);
      if (cmp < 0)
         right = mid-1;
      else if (cmp > 0)
         left = mid+1;
      else
         return reg_indices[mid];
   }
   return -1;
}

/*Gets a register name from its index.
  Returns NULL if the register does not exist.*/
const char* get_reg_name(int index)
{
   if (index < 0 || index > 31)
      return NULL;
   return reg_names[index];
}

/*Encodes an instr_t into a 32-bit unsigned value*/
uint32_t encode_instr(const instr_t* instr)
{
   uint32_t value = instr->op << 26;
   DEBUG("Encoding Instruction:\n");
   if (instr->op >= J_OP_START) //J type
   {
      value |= (instr->addr & 0x3FFFFFF);
      DEBUG("\tJ type: Op=%x Addr=%x\n",
         instr->op, instr->addr);
   }
   else
   {
      value |= (instr->r1 << 21) | (instr->r2 << 16);
      if (instr->op == 0) //R type
      {
         value |= (instr->r3 << 11) | (instr->sh << 6) | instr->fn;
         DEBUG("\tR type: Op=%x R1=%x R2=%x R3=%x Sh=%x Fn=%x\n",
            instr->op, instr->r1, instr->r2, instr->r3, instr->sh, instr->fn);
      }
      else //I type
      {
         value |= instr->imm;
         DEBUG("\tI type: Op=%x R1=%x R2=%x Imm=%x\n",
            instr->op, instr->r1, instr->r2, instr->imm);
      }
   }
   DEBUG("\t32-Bit Value: %08x\n", value);
   return value;
}

/*Decodes a 32-bit unsigned value into an instr_t*/
void decode_instr(instr_t* instr, uint32_t value)
{
   DEBUG("Decoding instruction:\n\t32-Bit Value: %x\n", value);

   instr->op = value >> 26;
   if (instr->op >= J_OP_START) //J type
   {
      instr->addr = ((int32_t)(value & 0x3FFFFFF) << 6) >> 6;
      DEBUG("\tJ type: Op=%x Addr=%x\n",
         instr->op, instr->addr);
   }
   else
   {
      instr->r1 = (value >> 21) & 0x1F;
      instr->r2 = (value >> 16) & 0x1F;
      if (instr->op == 0) //R type
      {
         instr->r3 = (value >> 11) & 0x1F;
         instr->sh = (value >> 6) & 0x1F;
         instr->fn = value & 0x3F;
         DEBUG("\tR type: Op=%x R1=%x R2=%x R3=%x Sh=%x Fn=%x\n",
            instr->op, instr->r1, instr->r2, instr->r3, instr->sh, instr->fn);
      }
      else //I type
      {
         instr->imm = value & 0xFFFF;
         DEBUG("\tI type: Op=%x R1=%x R2=%x Imm=%x\n",
            instr->op, instr->r1, instr->r2, instr->imm);
      }
   }
}

/*Executes the next instruction*/
int execute_next()
{
   DEBUG("Executing %x\n", PC);
   uint32_t instr;
   int val = load_word(PC, &instr);
   if (val != NO_ERROR)
   {
      PC+=sizeof(int32_t);
      return val;
   }
   return execute(instr);
}

/*Executes an instruction*/
int execute(uint32_t instr)
{
   instr_t i;
   decode_instr(&i, instr);
   return execute_instr(&i);
}

/*Executes an instruction*/
int execute_instr(const instr_t* instr)
{
   int val;
   PC+=sizeof(int32_t);
   if (instr->op == 0) //R type
   {
      if (instr->fn < R_TYPE_COUNT)
         val = (*r_instr[instr->fn])(instr->r1, instr->r2, instr->r3, instr->sh);
      else
         val = ERR_INVALID;
   }
   else if (instr->op < J_OP_START) //I type
   {
      if (instr->op <= I_TYPE_COUNT)
         val = (*i_instr[instr->op-1])(instr->r1, instr->r2, instr->imm);
      else
         val = ERR_INVALID;
   }
   else
   {
      if (instr->op < J_OP_START+J_TYPE_COUNT)
         val = (*j_instr[instr->op-J_OP_START])(instr->addr);
      else
         val = ERR_INVALID;
   }
   registers[0] = 0;
   return val;
}
