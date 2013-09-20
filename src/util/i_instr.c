#include "instructions.h"
#include "isa.h"
#include <stdio.h>

int instr_i_add(int8_t r1, int8_t r2, int16_t imm);
int instr_i_sub(int8_t r1, int8_t r2, int16_t imm);
int instr_i_mul(int8_t r1, int8_t r2, int16_t imm);
int instr_i_div(int8_t r1, int8_t r2, int16_t imm);

int instr_i_and(int8_t r1, int8_t r2, int16_t imm);
int instr_i_or(int8_t r1, int8_t r2, int16_t imm);
int instr_i_xor(int8_t r1, int8_t r2, int16_t imm);
int instr_i_nor(int8_t r1, int8_t r2, int16_t imm);

int instr_i_slt(int8_t r1, int8_t r2, int16_t imm);

int instr_i_lw(int8_t r1, int8_t r2, int16_t imm);
int instr_i_lh(int8_t r1, int8_t r2, int16_t imm);
int instr_i_lb(int8_t r1, int8_t r2, int16_t imm);

int instr_i_sw(int8_t r1, int8_t r2, int16_t imm);
int instr_i_sh(int8_t r1, int8_t r2, int16_t imm);
int instr_i_sb(int8_t r1, int8_t r2, int16_t imm);

int instr_i_beq(int8_t r1, int8_t r2, int16_t imm);
int instr_i_bne(int8_t r1, int8_t r2, int16_t imm);
int instr_i_bgt(int8_t r1, int8_t r2, int16_t imm);
int instr_i_bge(int8_t r1, int8_t r2, int16_t imm);
int instr_i_blt(int8_t r1, int8_t r2, int16_t imm);
int instr_i_ble(int8_t r1, int8_t r2, int16_t imm);

int instr_i_put(int8_t r1, int8_t r2, int16_t imm);

/*Array containing all I type instructions*/
int (*i_instr[I_TYPE_COUNT])(int8_t r1, int8_t r2, int16_t imm) = {
   instr_i_add, instr_i_sub, instr_i_mul, instr_i_div,
   instr_i_and, instr_i_or, instr_i_xor, instr_i_nor,
   instr_i_slt, instr_i_lw, instr_i_lh, instr_i_lb,
   instr_i_sw, instr_i_sh, instr_i_sb, instr_i_beq,
   instr_i_bne, instr_i_bgt, instr_i_bge, instr_i_blt,
   instr_i_ble, instr_i_put
};

int instr_i_add(int8_t r1, int8_t r2, int16_t imm)
{
   registers[r1] = registers[r2] + imm;
   return NO_ERROR;
}
int instr_i_sub(int8_t r1, int8_t r2, int16_t imm)
{
   registers[r1] = registers[r2] - imm;
   return NO_ERROR;
}
int instr_i_mul(int8_t r1, int8_t r2, int16_t imm)
{
   int64_t big = registers[r1] * imm;
   HI = big >> 32;
   LO = big & 0xFFFFFFFF;
   return NO_ERROR;
}
int instr_i_div(int8_t r1, int8_t r2, int16_t imm)
{
   if (imm == 0)
      return ERR_DIV_ZERO;

   LO = registers[r1] / imm;
   HI = registers[r1] % imm;

   return NO_ERROR;
}

int instr_i_and(int8_t r1, int8_t r2, int16_t imm)
{
   registers[r1] = registers[r2] & imm;
   return NO_ERROR;
}
int instr_i_or(int8_t r1, int8_t r2, int16_t imm)
{
   registers[r1] = registers[r2] | imm;
   return NO_ERROR;
}
int instr_i_xor(int8_t r1, int8_t r2, int16_t imm)
{
   registers[r1] = registers[r2] ^ imm;
   return NO_ERROR;
}
int instr_i_nor(int8_t r1, int8_t r2, int16_t imm)
{
   registers[r1] = ~(registers[r2] | imm);
   return NO_ERROR;
}

int instr_i_slt(int8_t r1, int8_t r2, int16_t imm)
{
   registers[r1] = registers[r2] < imm;
   return NO_ERROR;
}

int instr_i_lw(int8_t r1, int8_t r2, int16_t imm)
{
   int32_t value;
   int val = load_word(registers[r2]+imm, &value);
   if (val != NO_ERROR)
      return val;
   registers[r1] = value;
   return NO_ERROR;
}
int instr_i_lh(int8_t r1, int8_t r2, int16_t imm)
{
   int16_t value;
   int val = load_half(registers[r2]+imm, &value);
   if (val != NO_ERROR)
      return val;
   registers[r1] = value;
   return NO_ERROR;
}
int instr_i_lb(int8_t r1, int8_t r2, int16_t imm)
{
   int8_t value;
   int val = load_byte(registers[r2]+imm, &value);
   if (val != NO_ERROR)
      return val;
   registers[r1] = value;
   return NO_ERROR;
}

int instr_i_sw(int8_t r1, int8_t r2, int16_t imm)
{
   return store_word(registers[r2]+imm, (int32_t)registers[r1]);
}
int instr_i_sh(int8_t r1, int8_t r2, int16_t imm)
{
   return store_half(registers[r2]+imm, (int16_t)registers[r1]);
}
int instr_i_sb(int8_t r1, int8_t r2, int16_t imm)
{
   return store_byte(registers[r2]+imm, (int8_t)registers[r1]);
}

int instr_i_beq(int8_t r1, int8_t r2, int16_t imm)
{
   if (registers[r1] == registers[r2])
      PC+=imm*4;
   return NO_ERROR;
}
int instr_i_bne(int8_t r1, int8_t r2, int16_t imm)
{
   if (registers[r1] != registers[r2])
      PC+=imm*4;
   return NO_ERROR;
}
int instr_i_bgt(int8_t r1, int8_t r2, int16_t imm)
{
   if (registers[r1] > registers[r2])
      PC+=imm*4;
   return NO_ERROR;
}
int instr_i_bge(int8_t r1, int8_t r2, int16_t imm)
{
   if (registers[r1] >= registers[r2])
      PC+=imm*4;
   return NO_ERROR;
}
int instr_i_blt(int8_t r1, int8_t r2, int16_t imm)
{
   if (registers[r1] < registers[r2])
      PC+=imm*4;
   return NO_ERROR;
}
int instr_i_ble(int8_t r1, int8_t r2, int16_t imm)
{
   if (registers[r1] <= registers[r2])
      PC+=imm*4;
   return NO_ERROR;
}

int instr_i_put(int8_t r1, int8_t r2, int16_t imm)
{
   putchar((char)imm);
   return NO_ERROR;
}
