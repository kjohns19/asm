#include "memory.h"
#include <string.h>
#include <stdio.h>

int8_t memory[MEM_SIZE];

/*Initializes memory*/
void init_memory()
{
   memset(memory, 0, MEM_SIZE);
}

/*Loads a word (32-bit value) from memory*/
int load_word(uint32_t address, uint32_t* value)
{
   if (address >= MEM_SIZE)
      return ERR_MEM_INVALID;
   if (address % sizeof(uint32_t) != 0)
      return ERR_MEM_ALIGN;
   *value = *(uint32_t*)(memory+address);
   return NO_ERROR;
}

/*Loads a half-word (16-bit value) from memory*/
int load_half(uint32_t address, uint16_t* value)
{
   if (address >= MEM_SIZE)
      return ERR_MEM_INVALID;
   if (address % sizeof(uint16_t) != 0)
      return ERR_MEM_ALIGN;
   *value = *(uint16_t*)(memory+address);
   return NO_ERROR;
}

/*Loads a byte (8-bit value) from memory*/
int load_byte(uint32_t address, uint8_t* value)
{
   if (address >= MEM_SIZE)
      return ERR_MEM_INVALID;
   if (address % sizeof(uint8_t) != 0)
      return ERR_MEM_ALIGN;
   *value = *(uint8_t*)(memory+address);
   return NO_ERROR;
}


/*Stores a word (32-bit value) to memory*/
int store_word(uint32_t address, uint32_t value)
{
   printf("Storing %d to %x\n", value, address);
   if (address >= MEM_SIZE)
      return ERR_MEM_INVALID;
   if (address % sizeof(uint32_t) != 0)
      return ERR_MEM_ALIGN;
   *(uint32_t*)(memory+address) = value;
   return NO_ERROR;
}

/*Stores a half-word (16-bit value) to memory*/
int store_half(uint32_t address, uint16_t value)
{
   if (address >= MEM_SIZE)
      return ERR_MEM_INVALID;
   if (address % sizeof(uint16_t) != 0)
      return ERR_MEM_ALIGN;
   *(uint16_t*)(memory+address) = value;
   return NO_ERROR;
}

/*Stores a byte (8-bit value) to memory*/
int store_byte(uint32_t address, uint8_t value)
{
   if (address >= MEM_SIZE)
      return ERR_MEM_INVALID;
   if (address % sizeof(uint8_t) != 0)
      return ERR_MEM_ALIGN;
   *(uint8_t*)(memory+address) = value;
   return NO_ERROR;
}

