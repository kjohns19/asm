#include <stdio.h>
#include <unistd.h>
#include "util/isa.h"

char* inputFile;

int getArgcs(int argc, char* argv[]);

int read_file(const char* file);

int main(int argc, char* argv[])
{
   inputFile = NULL;

   int val;

   if (!getArgs(argc, argv))
      return 1;

   init_memory();
   init_registers();

   if (!read_file(inputFile))
   {
      fprintf(stderr, "Error reading input file\n");
      return 1;
   }

   //printf("PC=%x\n", PC);
   
   while((val = execute_next()) == NO_ERROR)
   {
      //printf("PC=%x\n", PC);
   }

   PC-=sizeof(int32_t);

   switch(val)
   {
      case EXIT:
         printf("Program returned 0x%x (%d), 0x%x (%d)\n", registers[2], registers[2], registers[3], registers[3]);
         break;
      case ERR_DIV_ZERO:
         printf("Error <PC=0x%x (%u)>: Divide by zero\n", PC, PC);
         break;
      case ERR_INVALID:
         printf("Error <PC=0x%x (%u)>: Invalid instruction\n", PC, PC);
         break;
      case ERR_MEM_INVALID:
         printf("Error <PC=0x%x (%u)>: Invalid memory access\n", PC, PC);
         break;
      case ERR_MEM_ALIGN:
         printf("Error <PC=0x%x (%u)>: Invalid memory alignment\n", PC, PC);
         break;
      default:
         printf("Error <PC=0x%x (%u)>: Unknown error (code %d)\n", PC, PC, val);
         break;
   }

   return 0;
}

int read_file(const char* filename)
{
   FILE* file;
   uint32_t pos = 0;
   size_t i, num;
   int32_t buffer[256];

   if (!(file = fopen(filename, "rb")))
      return 0;

   while(!feof(file))
   {
      num = fread(buffer, sizeof(int32_t), 256, file);
      printf("Read %d values\n", (int)num);
      for(i=0;i<num;i++)
         store_word((pos+i)*sizeof(int32_t), buffer[i]);
      pos+=num;
   }

   return 1;
}

int getArgs(int argc, char* argv[])
{
   int invalidArg = 0;
   char c;
   while((c = getopt(argc, argv, "d")) != -1)
      switch(c)
      {
         case 'd':
            debugMode = 1;
            break;
         default:
            invalidArg = 1;
            break;
      }

   if (optind < argc)
      inputFile = argv[optind];
   else
   {
      fprintf(stderr, "No input file specified\n");
      invalidArg = 1;
   }

   return !invalidArg;
}
