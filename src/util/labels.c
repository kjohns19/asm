#include "labels.h"

typedef struct label_t {
   char* name;
   struct label_list_t* left;
   struct label_list_t* right;
   uint32_t address;
} label_t;

label_t* list_root = NULL;

int label_new(const char* name, uint32_t address)
{
   label_t** node = &root;
   int cmp;

   if (!label)
      return NULL;

   while(*node)
   {
      cmp = strcmp(name, (*node)->name);
      if (cmp < 0)
         node = &((*node)->left);
      else if (cmp > 0)
         node = &((*node)->right);
      else
         return 0;
   }

   *node = (label_t*) malloc(sizeof(label_list_t));
   if (!*node)
      return 0;
   (*node)->name = (char*) malloc((strlen(name)+1)*sizeof(char));
   (*node)->left = NULL;
   (*node)->right = NULL;
   (*node)->address = address;

   if (!(*node)->name)
   {
      free(*node);
      *node = NULL;
      return 0;
   }

   strcpy((*node)->name, name);
   return 1;
}

int get_label_address(const char* name, uint32_t* address)
{
   label_t* node = root;
   int cmp;

   while(node)
   {
      cmp = strcmp(name, node->name);
      if (cmp < 0)
         node = node->left;
      else if (cmp > 0)
         node == node->right;
      else
      {
         *address = node->address;
         return 1;
      }
   }
   return 0;
}
