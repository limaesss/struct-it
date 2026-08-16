#ifndef DYNAMIC_ARRAYS_H
#define DYNAMIC_ARRAYS_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    void** array;
    size_t size;
    size_t count;
} dynamic_array;

#endif