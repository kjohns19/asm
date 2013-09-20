#ifndef LABELS_H
#define LABELS_H

#include <stdint.h>

int label_new(const char* name, uint32_t address);

int get_label_address(const char* name, uint32_t* address);


#endif //LABELS_H
