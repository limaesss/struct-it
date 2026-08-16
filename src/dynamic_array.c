#ifndef DYNAMIC_ARRAYS_C
#define DYNAMIC_ARRAYS_C

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../include/dynamic_array.h"

void* da_init(dynamic_array* array, size_t capacity, size_t element_size) {
    if (element_size<=0) { element_size = 4;}
    if (capacity<=0) { capacity = 4; }

    void* data = (void*)malloc(capacity * element_size);

    if (data == NULL) {
        printf("[DA] init failed \n its only up from here \n");
        exit(0);
    } else {
        printf("[DA] init successful");
    }

    array->element_size = element_size;
    array->count = 0;
    return data;
}

void da_rollback(dynamic_array *array) {
    free(array->data);
    array->data = da_init(array, array->capacity, array->element_size);
}

#endif