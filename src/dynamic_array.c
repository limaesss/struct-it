#ifndef DYNAMIC_ARRAYS_C
#define DYNAMIC_ARRAYS_C

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../include/dynamic_array.h"

void* da_init(dynamic_array* array, size_t capacity, size_t element_size) 
{
    if (element_size<=0) { element_size = 4;}
    if (capacity<=0) { capacity = 4; }

    void* data = (void*)malloc(capacity * element_size);

    if (data == NULL) {
        printf("[DA] init failed \n its only up from here, buy more RAM. \n You dont have enough to initialize an array with the size of %lu bytes. \n If youre running this on an embedded system, why are you using THIS data-struct lib? use literally any other one. \n Never try to make dynamic arrays, EVER AGAIN. Until you buy more RAM. \n", (capacity * element_size));
        exit(0);
    } else {
        printf("[DA] init successful \n you have enough RAM! Wowza!");
    }

    array->element_size = element_size;
    array->count = 0;
    return data;
}

void da_rollback(dynamic_array* array) 
{
    free(array->data);
    array->data = da_init(array, array->capacity, array->element_size);
}

static bool checks(dynamic_array* array) 
{
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

static bool index_check(dynamic_array* array, int index) 
{
    if (index < 0 || index >= array->count) {
        printf("[DA] index %d out of bounds (count: %lu)\n", index, array->count);
        return false;
    }
    return true;
}

static void grow(dynamic_array* array) 
{
    size_t new_capacity = array->capacity * 2;
    void* new_data = realloc(array->data, new_capacity * array->element_size);
    
    if (new_data == NULL) {
        printf("[DA] realloc failed \n");
        return;
    }
    
    array->data = new_data;
    array->capacity = new_capacity;
}

static void shrink(dynamic_array* array) 
{
    size_t new_capacity = array->capacity / 2;
    void* new_data = realloc(array->data, new_capacity * array->element_size);
    
    if (new_data == NULL) {
        printf("[DA] realloc failed \n");
        da_rollback(array);
        return;
    } else { 
        array->capacity = new_capacity;
        array->data = new_data;
    }
}

static inline bool should_shrink(const dynamic_array* array, bool grew) { return grew == false && array->count > 0 && array->count <= array->capacity / 4; }
static inline bool should_grow(const dynamic_array* array) { return array->count >= array->capacity; }

bool grew = false;

void da_append(dynamic_array *array, void* element) 
{
    if (!checks(array)) { return; }
    array->data[array->count] = element;
    array->count++;

    if (should_grow(array)) { grow(array); grew = true; }
}

void da_remove(dynamic_array *array, int index) 
{
    if (!checks(array)) { return; } ;
    
    if (!index_check(array, index)) { return; }
    
    // shift it left!!
    for (int i = index; i < array->count - 1; i++) {
        array->data[i] = array->data[i + 1];
    }

    if (should_shrink(array, grew)) { shrink(array); grew = false; }
    
    array->count--;
}

#endif