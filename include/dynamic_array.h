#ifndef DYNAMIC_ARRAYS_H
#define DYNAMIC_ARRAYS_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    void* data;
    size_t element_size; // size of each element
    size_t count;
    size_t capacity;
} dynamic_array;

void** da_init(dynamic_array* array, size_t element_size);

void da_rollback(dynamic_array* array);

void da_append(dynamic_array* array, void* element);
void da_insert(dynamic_array* array, void* element, int index);

void da_remove(dynamic_array* array, int index);

void* da_get(dynamic_array* array, int index);

void da_free(dynamic_array* array);

#endif