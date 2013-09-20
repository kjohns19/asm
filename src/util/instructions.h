#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <stdint.h>

#define R_TYPE_COUNT 22
#define I_TYPE_COUNT 22
#define J_TYPE_COUNT 2

/*Array containing all R type instructions*/
extern int (*r_instr[R_TYPE_COUNT])(int8_t r1, int8_t r2, int8_t r3, int8_t sh);
/*Array containing all I type instructions*/
extern int (*i_instr[I_TYPE_COUNT])(int8_t r1, int8_t r2, int16_t imm);
/*Array containing all J type instructions*/
extern int (*j_instr[J_TYPE_COUNT])(int32_t addr);

#endif //INSTRUCTIONS_H
