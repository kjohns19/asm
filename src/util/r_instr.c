#include "instructions.h"
#include "isa.h"
#include <stdio.h>

int instr_r_add(int8_t r1, int8_t r2, int8_t r3, int8_t sh);
int instr_r_sub(int8_t r1, int8_t r2, int8_t r3, int8_t sh);
int instr_r_mul(int8_t r1, int8_t r2, int8_t r3, int8_t sh);
int instr_r_div(int8_t r1, int8_t r2, int8_t r3, int8_t sh);

int instr_r_and(int8_t r1, int8_t r2, int8_t r3, int8_t sh);
int instr_r_or(int8_t r1, int8_t r2, int8_t r3, int8_t sh);
int instr_r_xor(int8_t r1, int8_t r2, int8_t r3, int8_t sh);
int instr_r_nor(int8_t r1, int8_t r2, int8_t r3, int8_t sh);

int instr_r_slt(int8_t r1, int8_t r2, int8_t r3, int8_t sh);

int instr_r_sll(int8_t r1, int8_t r2, int8_t r3, int8_t sh);
int instr_r_srl(int8_t r1, int8_t r2, int8_t r3, int8_t sh);
int instr_r_sra(int8_t r1, int8_t r2, int8_t r3, int8_t sh);

int instr_r_sllv(int8_t r1, int8_t r2, int8_t r3, int8_t sh);
int instr_r_srlv(int8_t r1, int8_t r2, int8_t r3, int8_t sh);
int instr_r_srav(int8_t r1, int8_t r2, int8_t r3, int8_t sh);

int instr_r_mfh(int8_t r1, int8_t r2, int8_t r3, int8_t sh);
int instr_r_mfl(int8_t r1, int8_t r2, int8_t r3, int8_t sh);

int instr_r_jr(int8_t r1, int8_t r2, int8_t r3, int8_t sh);
int instr_r_jral(int8_t r1, int8_t r2, int8_t r3, int8_t sh);

int instr_r_put(int8_t r1, int8_t r2, int8_t r3, int8_t sh);
int instr_r_get(int8_t r1, int8_t r2, int8_t r3, int8_t sh);

int instr_r_exit(int8_t r1, int8_t r2, int8_t r3, int8_t sh);

/*Array containing all R type instructions*/
int (*r_instr[R_TYPE_COUNT])(int8_t r1, int8_t r2, int8_t r3, int8_t sh) = {
   instr_r_add, instr_r_sub, instr_r_mul, instr_r_div,
   instr_r_and, instr_r_or, instr_r_xor, instr_r_nor,
   instr_r_slt, instr_r_sll, instr_r_srl, instr_r_sra,
   instr_r_sllv, instr_r_srlv, instr_r_srav, instr_r_mfh,
   instr_r_mfl, instr_r_jr, instr_r_jral, instr_r_put, instr_r_get,
   instr_r_exit
};

int instr_r_add(int8_t r1, int8_t r2, int8_t r3, int8_t sh)
{
   registers[r1] = registers[r2] + registers[r3];
   return NO_ERROR;
}
int instr_r_sub(int8_t r1, int8_t r2, int8_t r3, int8_t sh)
{
   registers[r1] = registers[r2] - registers[r3];
   return NO_ERROR;
}
int instr_r_mul(int8_t r1, int8_t r2, int8_t r3, int8_t sh)
{
   int64_t big = registers[r1] * registers[r2];
   HI = big >> 32;
   LO = big & 0xFFFFFFFF;
   return NO_ERROR;
}
int instr_r_div(int8_t r1, int8_t r2, int8_t r3, int8_t sh)
{
   if (registers[r2] == 0)
      return ERR_DIV_ZERO;

   LO = registers[r1] / registers[r2];
   HI = registers[r1] % registers[r2];

   return NO_ERROR;
}

int instr_r_and(int8_t r1, int8_t r2, int8_t r3, int8_t sh)
{
   registers[r1] = registers[r2] & registers[r3];
   return NO_ERROR;
}
int instr_r_or(int8_t r1, int8_t r2, int8_t r3, int8_t sh)
{
   registers[r1] = registers[r2] | registers[r3];
   return NO_ERROR;
}
int instr_r_xor(int8_t r1, int8_t r2, int8_t r3, int8_t sh)
{
   registers[r1] = registers[r2] ^ registers[r3];
   return NO_ERROR;
}
int instr_r_nor(int8_t r1, int8_t r2, int8_t r3, int8_t sh)
{
   registers[r1] = ~(registers[r2] | registers[r3]);
   return NO_ERROR;
}

int instr_r_slt(int8_t r1, int8_t r2, int8_t r3, int8_t sh)
{
   registers[r1] = registers[r2] < registers[r3];
   return NO_ERROR;
}

int instr_r_sll(int8_t r1, int8_t r2, int8_t r3, int8_t sh)
{
   registers[r1] = registers[r2] << sh;
   return NO_ERROR;
}
int instr_r_srl(int8_t r1, int8_t r2, int8_t r3, int8_t sh)
{
   registers[r1] = (int32_t)(((uint32_t)registers[r2]) >> sh);
   return NO_ERROR;
}
int instr_r_sra(int8_t r1, int8_t r2, int8_t r3, int8_t sh)
{
   registers[r1] = registers[r2] >> sh;
   return NO_ERROR;
}

int instr_r_sllv(int8_t r1, int8_t r2, int8_t r3, int8_t sh)
{
   registers[r1] = registers[r2] << registers[r3];
   return NO_ERROR;
}
int instr_r_srlv(int8_t r1, int8_t r2, int8_t r3, int8_t sh)
{
   registers[r1] = (int32_t)(((uint32_t)registers[r2]) >> registers[r3]);
   return NO_ERROR;
}
int instr_r_srav(int8_t r1, int8_t r2, int8_t r3, int8_t sh)
{
   registers[r1] = registers[r2] >> sh;
   return NO_ERROR;
}

int instr_r_mfh(int8_t r1, int8_t r2, int8_t r3, int8_t sh)
{
   registers[r1] = HI;
   return NO_ERROR;
}
int instr_r_mfl(int8_t r1, int8_t r2, int8_t r3, int8_t sh)
{
   registers[r1] = LO;
   return NO_ERROR;
}

int instr_r_jr(int8_t r1, int8_t r2, int8_t r3, int8_t sh)
{
   PC = registers[r1];
   return NO_ERROR;
}

int instr_r_jral(int8_t r1, int8_t r2, int8_t r3, int8_t sh)
{
   registers[31] = PC;
   PC = registers[r1];
   return NO_ERROR;
}

int instr_r_put(int8_t r1, int8_t r2, int8_t r3, int8_t sh)
{
   putchar((char)registers[r1]);
   return NO_ERROR;
}
int instr_r_get(int8_t r1, int8_t r2, int8_t r3, int8_t sh)
{
   registers[r1] = getchar();
   return NO_ERROR;
}

int instr_r_exit(int8_t r1, int8_t r2, int8_t r3, int8_t sh)
{
   return EXIT;
}
