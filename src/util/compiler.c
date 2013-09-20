#include "compiler.h"
#include "debug.h"
#include <string.h>
#include <stdio.h>

instr_type_t instructions[] = {
   {"add", 'r', 0x00, {ARG_REG, ARG_REG, ARG_REG, ARG_NUL, ARG_NUL}},
   {"sub", 'r', 0x01, {ARG_REG, ARG_REG, ARG_REG, ARG_NUL, ARG_NUL}},
   {"mul", 'r', 0x02, {ARG_REG, ARG_REG, ARG_NUL, ARG_NUL, ARG_NUL}},
   {"div", 'r', 0x03, {ARG_REG, ARG_REG, ARG_NUL, ARG_NUL, ARG_NUL}},
   {"and", 'r', 0x04, {ARG_REG, ARG_REG, ARG_REG, ARG_NUL, ARG_NUL}},
   {"or",  'r', 0x05, {ARG_REG, ARG_REG, ARG_REG, ARG_NUL, ARG_NUL}},
   {"xor", 'r', 0x06, {ARG_REG, ARG_REG, ARG_REG, ARG_NUL, ARG_NUL}},
   {"nor", 'r', 0x07, {ARG_REG, ARG_REG, ARG_REG, ARG_NUL, ARG_NUL}},
   {"slt", 'r', 0x08, {ARG_REG, ARG_REG, ARG_IMM, ARG_NUL, ARG_NUL}},
   {"sll", 'r', 0x09, {ARG_REG, ARG_REG, ARG_IMM, ARG_NUL, ARG_NUL}},
   {"srl", 'r', 0x0A, {ARG_REG, ARG_REG, ARG_IMM, ARG_NUL, ARG_NUL}},
   {"sra", 'r', 0x0B, {ARG_REG, ARG_REG, ARG_REG, ARG_NUL, ARG_NUL}},
   {"sllv",'r', 0x0C, {ARG_REG, ARG_REG, ARG_REG, ARG_NUL, ARG_NUL}},
   {"srlv",'r', 0x0D, {ARG_REG, ARG_REG, ARG_REG, ARG_NUL, ARG_NUL}},
   {"srav",'r', 0x0E, {ARG_REG, ARG_REG, ARG_REG, ARG_NUL, ARG_NUL}},
   {"mfh", 'r', 0x0F, {ARG_REG, ARG_NUL, ARG_NUL, ARG_NUL, ARG_NUL}},
   {"mfl", 'r', 0x10, {ARG_REG, ARG_NUL, ARG_NUL, ARG_NUL, ARG_NUL}},
   {"jr",  'r', 0x11, {ARG_REG, ARG_NUL, ARG_NUL, ARG_NUL, ARG_NUL}},
   {"jral",'r', 0x12, {ARG_REG, ARG_NUL, ARG_NUL, ARG_NUL, ARG_NUL}},
   {"put", 'r', 0x13, {ARG_REG, ARG_NUL, ARG_NUL, ARG_NUL, ARG_NUL}},
   {"get", 'r', 0x14, {ARG_REG, ARG_NUL, ARG_NUL, ARG_NUL, ARG_NUL}},
   {"exit",'r', 0x15, {ARG_NUL, ARG_NUL, ARG_NUL, ARG_NUL, ARG_NUL}},

   {"addi",'i', 0x01, {ARG_REG, ARG_REG, ARG_IMM, ARG_NUL, ARG_NUL}},
   {"subi",'i', 0x02, {ARG_REG, ARG_REG, ARG_IMM, ARG_NUL, ARG_NUL}},
   {"muli",'i', 0x03, {ARG_REG, ARG_IMM, ARG_NUL, ARG_NUL, ARG_NUL}},
   {"divi",'i', 0x04, {ARG_REG, ARG_IMM, ARG_NUL, ARG_NUL, ARG_NUL}},
   {"andi",'i', 0x05, {ARG_REG, ARG_REG, ARG_IMM, ARG_NUL, ARG_NUL}},
   {"ori", 'i', 0x06, {ARG_REG, ARG_REG, ARG_IMM, ARG_NUL, ARG_NUL}},
   {"xori",'i', 0x07, {ARG_REG, ARG_REG, ARG_IMM, ARG_NUL, ARG_NUL}},
   {"nori",'i', 0x08, {ARG_REG, ARG_REG, ARG_IMM, ARG_NUL, ARG_NUL}},
   {"slti",'i', 0x09, {ARG_REG, ARG_REG, ARG_IMM, ARG_NUL, ARG_NUL}},
   {"lw",  'i', 0x0A, {ARG_REG, ARG_REG, ARG_IMM, ARG_NUL, ARG_NUL}},
   {"lh",  'i', 0x0B, {ARG_REG, ARG_REG, ARG_IMM, ARG_NUL, ARG_NUL}},
   {"lb",  'i', 0x0C, {ARG_REG, ARG_REG, ARG_IMM, ARG_NUL, ARG_NUL}},
   {"sw",  'i', 0x0D, {ARG_REG, ARG_REG, ARG_IMM, ARG_NUL, ARG_NUL}},
   {"sh",  'i', 0x0E, {ARG_REG, ARG_REG, ARG_IMM, ARG_NUL, ARG_NUL}},
   {"sb",  'i', 0x0F, {ARG_REG, ARG_REG, ARG_IMM, ARG_NUL, ARG_NUL}},
   {"beq", 'i', 0x10, {ARG_REG, ARG_REG, ARG_IMM, ARG_NUL, ARG_NUL}},
   {"bne", 'i', 0x11, {ARG_REG, ARG_REG, ARG_IMM, ARG_NUL, ARG_NUL}},
   {"bgt", 'i', 0x12, {ARG_REG, ARG_REG, ARG_IMM, ARG_NUL, ARG_NUL}},
   {"bge", 'i', 0x13, {ARG_REG, ARG_REG, ARG_IMM, ARG_NUL, ARG_NUL}},
   {"blt", 'i', 0x14, {ARG_REG, ARG_REG, ARG_IMM, ARG_NUL, ARG_NUL}},
   {"ble", 'i', 0x15, {ARG_REG, ARG_REG, ARG_IMM, ARG_NUL, ARG_NUL}},
   {"puti",'i', 0x16, {ARG_IMM, ARG_NUL, ARG_NUL, ARG_NUL, ARG_NUL}},

   {"j",   'j', 0x38, {ARG_ADR, ARG_NUL, ARG_IMM, ARG_NUL, ARG_NUL}},
   {"jal", 'j', 0x39, {ARG_ADR, ARG_NUL, ARG_IMM, ARG_NUL, ARG_NUL}},
};

static int compare(void* p1, void* p2)
{
   instr_type_t* instr1 = (instr_type_t*)p1;
   instr_type_t* instr2 = (instr_type_t*)p2;

   return strcmp(instr1->name, instr2->name);
}

/*Initializes the instructions*/
void init_instructions()
{
   int num = sizeof(instructions)/sizeof(instr_type_t);
   qsort(instructions, num, sizeof(instr_type_t), compare);
}

/*Gets the instruction type from its name.
  Returns NULL if the instruction does not exist.*/
const instr_type_t* get_instr_type(char* name)
{
   int left = 0;
   int right = sizeof(instructions)/sizeof(instr_type_t)-1;
   int mid, cmp;

   while(left <= right)
   {
      mid = (left+right)/2;
      cmp = strcmp(name, instructions[mid].name);
      if (cmp < 0)
         right = mid-1;
      else if (cmp > 0)
         left = mid+1;
      else
         return &instructions[mid];
   }
   return NULL;
}

/*Reads the arguments from a string*/
static int get_args(const char* str, int lineNum, const instr_type_t* type, instr_t* instr);

/*Gets an instruction from a string. Returns a non-zero value
  if an error occurs.*/
int parse_instr(const char* str, int lineNum, instr_t* instr)
{
   char instrName[256];
   const instr_type_t* type;
   int count;

   memset(instr, 0, sizeof(instr));

   if (sscanf(str, "%s%n", instrName, &count) != 1 || count == 0)
      return INSTR_BLANK;

   type = get_instr_type(instrName);
   if (type == NULL)
   {
      ERROR(lineNum, "Unknown instruction \"%s\"\n", instrName);
      return INSTR_UNKNOWN;
   }

   return get_args(str+count, lineNum, type, instr);
}

/*Parses data from a string. Returns a non-zero value
  if an error occurs.*/
int parse_data(const char* line, int lineNum, uint8_t* data, int* count)
{
   char type[256];
   char numStr[256];
   int offset;
   int num;
   int size;
   int numCount;
   int spaceCount = 0;
   int array = 0;
   uint32_t value;

   if (!sscanf(line, "%s%n", type, &num) || num == 0)
      return DATA_BLANK;
   offset+=num;

   if (!strcmp(type, "array"))
   {
      array = 1;
      if (!sscanf(line+offset, "%s%n", type, &num) || num == 0)
      {
         ERROR(lineNum, "No data\n");
         return DATA_NO_VALUE;
      }
      offset+=num;
   }

   if (!strcmp(type, "word"))
      size = 4;
   else if (!strcmp(type, "half"))
      size = 2;
   else if (!strcmp(type, "byte"))
      size = 1;
   else
   {
      ERROR(lineNum, "Invalid data type \"%s\"\n", type);
      return DATA_UNKNOWN;
   }

   numCount = 0;
   while(1)
   {
      if (!sscanf(line+offset, "%s%n", numStr, &num) || num == 0)
      {
         if (numCount > 0)
            break;
         ERROR(lineNum, "No data\n");
         return DATA_NO_VALUE;
      }
      offset+=num;

      if (!getNumber(numStr, &value))
      {
         ERROR(lineNum, "Invalid value \"%s\"\n", numStr);
         return DATA_INVALID;
      }

      if (size == 4)
         *((uint32_t*)(data+spaceCount)) = (uint32_t)value;
      else if (size == 2)
         *((uint16_t*)(data+spaceCount)) = (uint16_t)value;
      else if (size == 1)
         *((uint8_t*)(data+spaceCount)) = (uint8_t)value;

      spaceCount+=size;

      numCount++;
      if (!array)
         break;
   }

   (*count) = spaceCount;

   return 0;
}

/*Reads the arguments from a string*/
static int get_args(const char* str, int lineNum, const instr_type_t* type, instr_t* instr)
{
   char argStr[256];
   int currentArg = 0;
   int count = 1;
   int pos = 0;
   int reg;
   int regCount = 0;
   int arg;
   int number;

   if (type->type == 'r')
   {
      instr->op = 0;
      instr->fn = type->code;
   }
   else
      instr->op = type->code;

   while(1)
   {
      arg = type->args[currentArg];
      //Get the next argument
      if (sscanf(str+pos, "%s%n", argStr, &count) != 1)
      {
         //No argument provided but more are needed - error
         if (arg != ARG_NUL)
         {
            ERROR(lineNum, "Too many arguments for \"%s\"\n", type->name);
            return INSTR_TOO_FEW_ARGS;
         }
         break;
      }
      //Argument provided but no more are needed - error
      if (arg == ARG_NUL)
      {
         ERROR(lineNum, "Too few arguments for \"%s\"\n", type->name);
         return INSTR_TOO_MANY_ARGS;
      }

      //Register argument needed
      if (arg == ARG_REG)
      {
         reg = get_reg_index(argStr);
         if (reg == -1)
         {
            ERROR(lineNum, "Register expected. Got \"%s\"\n", argStr);
            return INSTR_WRONG_ARG;
         }
         if (regCount == 0)
            instr->r1 = reg;
         else if (regCount == 1)
            instr->r2 = reg;
         else
            instr->r3 = reg;
         regCount++;
      }
      //Immediate or address needed
      else if (arg == ARG_IMM || arg == ARG_ADR)
      {
         if (!getNumber(argStr, &number))
         {
            ERROR(lineNum, "Number expected. Got \"%s\"\n", argStr);
            return INSTR_WRONG_ARG;
         }
         if (type->type == 'r')
            instr->sh = number;
         else if (type->type == 'i')
            instr->imm = number;
         else
            instr->addr = number;
      }

      currentArg++;
      pos+=count;
   }

   return INSTR_VALID;
}

/*Gets a number from a string.
  Puts the number in the pointer and returns 1 on success.
  If there is an error, 0 is returned and the number is unchanged.*/
int getNumber(const char* str, int* number)
{
   int base = 10;
   int pos = 0;
   int num = 0;
   int val;
   char c;

   if (str[0] == 0)
      return 0;

   if (str[0] == '0')
   {
      if (str[1] == 0)
      {
         *number = 0;
         return 1;
      }
      else if (str[1] == 'x' || str[1] == 'X')
      {
         base = 16;
         pos = 2;
      }
      else
      {
         base = 8;
         pos = 1;
      }
   }

   while(c = str[pos])
   {
      num*=base;

      if (c >= '0' && c <= '9')
         val = c-'0';
      else if (c >= 'A' && c <= 'F')
         val = 10 + c - 'A';
      else if (c >= 'a' && c <= 'f')
         val = 10 + c - 'a';
      else
         return 0;

      if (val >= base)
         return 0;

      num+=val;

      pos++;
   }

   *number = num;
   return 1;
}
