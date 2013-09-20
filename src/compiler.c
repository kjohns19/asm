#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "util/isa.h"
#include "util/compiler.h"
#include "util/debug.h"

uint32_t data[1<<14];
uint32_t current = 0;

char* inputFile;
char* outputFile;

int getArgs(int argc, char* argv[]);

int read_input();
int write_output();

//int read_instr(const char* line, int lineNum, instr_t* instr);
void write_instr(const instr_t* instr);

int main(int argc, char* argv[])
{
   int i;
   debugMode = 0;        //d - Debug mode (prints messages)
   inputFile = NULL; //  - Source file
   outputFile = NULL;//o - Output file (default = "a.out")

   if (!getArgs(argc, argv))
      return 1;

   printf("Arguments:\n");
   for(i=0;i<argc;i++)
      printf("%s\n", argv[i]);

   if (!outputFile)
      outputFile = "a.out";

   printf("Debug: %d\nInput: %s\nOutput: %s\n", debugMode, inputFile, outputFile);

   init_instructions();

   if (!read_input())
   {
      printf("Errors occurred. Cannot compile.\n");
      return 1;
   }
   if (!write_output())
      return 1;
   
   return 0;
}

char* first_character(char* str);
void remove_comment(char* str);

int read_input()
{
   FILE* file;
   char buffer[1024];
   char* str;
   int offset;
   int fail = 0;
   int lineNum = 0;
   int val;
   int mode = 0;
   instr_t instr;

   if (!(file = fopen(inputFile, "r")))
   {
      fprintf(stderr, "Could not open input file\n");
      return 0;
   }
   
   while(fgets(buffer, 1024, file))
   {
      lineNum++;

      //Start at the first non-whitespace character
      str = first_character(buffer);
      if (!str)
         continue;

      //Check for comment
      remove_comment(str);

      val = parse_data(str, lineNum, data+curCount, 
      if (val == INSTR_VALID)
         write_instr(&instr);
      else if (val != INSTR_BLANK)
         fail = 1;

      /*val = get_instr(str, lineNum, &instr);
      if (val == INSTR_VALID)
         write_instr(&instr);
      else if (val != INSTR_BLANK)
         fail = 1;*/
   }

   fclose(file);

   return !fail;
}

char* first_character(char* str)
{
   char* ptr = str;
   while(*ptr != 0 && (*ptr == ' ' || *ptr == '\t' || *ptr == '\n'))
      ptr++;
   return (*ptr == 0) ? NULL : ptr;
}

void remove_comment(char* str)
{
   int quote = 0;
   char* pos = str;
   while(*pos)
   {
      if (*pos == '"')
      {
         if (!quote)
            quote = 1;
         else if (*(pos-1) != '\\')
            quote = 0;
      }
      if (*pos == '#' && !quote)
      {
         *pos = 0;
         return;
      }
      pos++;
   }
}

void write_instr(const instr_t* instr)
{
   uint32_t val = encode_instr(instr);
   data[current] = val;
   current++;
}

int write_output()
{
   FILE* file;
   if (!(file = fopen(outputFile, "wb")))
   {
      fprintf(stderr, "Could not open output file\n");
      return 0;
   }
   fwrite(data, sizeof(uint32_t), current, file);
   return 1;
}

int getArgs(int argc, char* argv[])
{
   int invalidArg = 0;
   char c;
   while((c = getopt(argc, argv, "do:")) != -1)
      switch(c)
      {
         case 'd':
            debugMode = 1;
            break;
         case 'o':
            outputFile = optarg;
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
