#include "instructions.h"
#include "isa.h"

int instr_j_j(int32_t addr);
int instr_j_jal(int32_t addr);

/*Array containing all J type instructions*/
int (*j_instr[J_TYPE_COUNT])(int32_t addr) = {
   instr_j_j,
   instr_j_jal
};

int instr_j_j(int32_t addr)
{
   PC+=addr*4;
   return NO_ERROR;
}
int instr_j_jal(int32_t addr)
{
   registers[31] = PC;
   PC+=addr*4;
   return NO_ERROR;
}
