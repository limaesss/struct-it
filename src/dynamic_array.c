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

void da_rollback(dynamic_array* array) {
    free(array->data);
    array->data = da_init(array, array->capacity, array->element_size);
}

static bool checks(dynamic_array* array) {
    if (array == NULL) {
        printf("[DA] NULL array pointer (why?) \n");
        return false;
    }

    if (array->data == NULL) {
        printf("[DA] NULL data pointer \n");
        da_rollback(array);
        printf("[DA] data is valid; continuing \n");
    }

    if (array->capacity <= 0 || array->count <= 0) {
        printf("[DA] capacity/count is 0 \n");
        return false;
    }
    return true;
}

static bool index_check(dynamic_array* array, int index) {
    if (index < 0 || index >= array->count) {
        printf("[DA] index %d out of bounds (count: %lu)\n", index, array->count);
        return false;
    }
    return true;
}

static void grow(dynamic_array* array) {
    size_t new_capacity = array->capacity * 2;
    void* new_data = realloc(array->data, new_capacity * array->element_size);
    
    if (new_data == NULL) {
        printf("[DA] realloc failed \n");
        return;
    }
    
    array->data = new_data;
    array->capacity = new_capacity;
}

#endif