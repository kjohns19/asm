#ifndef MEMORY_H
#define MEMORY_H

#include "errors.h"
#include <stdint.h>

#define MEM_SIZE 1<<16

/*Initializes memory*/
void init_memory();

/*Loads a word (32-bit value) from memory*/
int load_word(uint32_t address, uint32_t* value);
/*Loads a half-word (16-bit value) from memory*/
int load_half(uint32_t address, uint16_t* value);
/*Loads a byte (8-bit value) from memory*/
int load_byte(uint32_t address, uint8_t* value);

/*Stores a word (32-bit value) to memory*/
int store_word(uint32_t address, uint32_t value);
/*Stores a half-word (16-bit value) to memory*/
int store_half(uint32_t address, uint16_t value);
/*Stores a byte (8-bit value) to memory*/
int store_byte(uint32_t address, uint8_t value);

#endif //MEMORY_H
