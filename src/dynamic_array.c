#ifndef DYNAMIC_ARRAYS_C
#define DYNAMIC_ARRAYS_C

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../include/dynamic_array.h"

void** da_init(dynamic_array* array, size_t capacity, size_t element_size)
{
    // god is probably in deep hatred because of void**
    // humanity is ruined, void* was bad enough

    if (element_size<=0) { element_size = 4;}
    if (capacity<=0) { capacity = 4; }
    // stupid fucking hack

    // though, i dont care if you want
    // an element which size is less than 0
    // these are not real, if they are
    // your heap is not real
    // 4 bytes

    void** data = (void*)malloc(capacity * element_size);

    if (data == NULL) {
        printf("[DA] init failed \n its only up from here, buy more RAM. \n You dont have enough to initialize an array with the size of %lu bytes. \n If youre running this on an embedded system, why are you using THIS data-struct lib? use literally any other one. \n Never try to make dynamic arrays, EVER AGAIN. Until you buy more RAM. \n", (capacity * element_size));
        printf("[DA] now, if you excuse me, im leaving. Sucker");
        exit(69420);
        // what TED talk is this?
    } else {
        printf("[DA] init successful \n you have enough RAM! Wowza!");
    }

    array->element_size = element_size;
    array->capacity = capacity;
    array->count = 0;
    return data;

    /*
    Dear Caller,
        If you dont free this,
        Your ass is gonna get beat.
        If you do free this, 
        Your ass is gonna get beat. (Hello, da_rollback()!)

        The only way you dont get your ass beaten, 
        is if you jump off a cliff,
        or you dont use the lib.
        The first one is a better choice
    XOXO Your Favorite Library
    */
    // that was a valentine
}

void da_rollback(dynamic_array* array) 
{
    free(array->data); // double free! Hooray!
                           // please dont free it without a verified adult
                           // thanks
    array->data = da_init(array, array->capacity, array->element_size);
    // throw it all away, i dont fucking care
}

static bool checks(dynamic_array* array) 
{
    if (array == NULL) { // translated: "if you fucked up"
        printf("[DA] NULL array pointer (why would you do that?) \n");
        return false;
    }

    if (array->data == NULL) { // translated: "if i fucked up"
        printf("[DA] NULL data pointer \n");
        da_rollback(array);
        printf("[DA] data is valid; continuing \n");
    }

    if (array->capacity <= 0 || array->count < 0) { // translated: "go fuck yourself"
        printf("[DA] capacity/count is 0 \n");
        return false;
    }
    return true;
}

static bool index_check(dynamic_array* array, int index) 
{
    if (index < 0 || index >= array->count) { // i hate you
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
        // sucker; lolol
        return;
    }
    
    array->data = new_data;
    array->capacity = new_capacity;

}

static void shrink(dynamic_array* array) 
{
    size_t new_capacity = array->capacity / 2; 
    // if you have any elements in that half,
    // prepare to read a doc about how to let go
    void* new_data = realloc(array->data, new_capacity * array->element_size);
    
    if (new_data == NULL) {
        printf("[DA] realloc failed \n"); // or not!
        da_rollback(array); // oh
        return;
    } else { 
        array->capacity = new_capacity;
        array->data = new_data;
    }
}

static inline bool should_shrink(const dynamic_array* array, bool grew) { return grew == false && array->count > 0 && array->count <= array->capacity / 4; }
static inline bool should_grow(const dynamic_array* array) { return array->count >= array->capacity; }
// i need the cycles more than you (i) need readability
// though, it is readable, i admit; could be more readable, but why?

bool grew = false; 
// this is global because i am stupid,
// sorry

void da_append(dynamic_array *array, void* element) 
{
    if (!checks(array)) { return; }
    array->data[array->count] = element;  
    array->count++;

    // why does it have to be like this? 
    // please add "append(void* array, void* element)"" PLEASE

    if (should_grow(array)) { grow(array); grew = true; }
    // if should grow grow and grew
}

void da_remove(dynamic_array *array, int index) 
{
    if (!checks(array)) { return; };
    // loser
    
    if (!index_check(array, index)) { return; }
    // more a dumbass, than a loser..
    // if you dont manually remove elements,
    // how do you have an OOB index?
    
    // shift it left!!
    for (int i = index; i < array->count - 1; i++) {
        array->data[i] = array->data[i + 1];
    }

    if (should_shrink(array, grew)) { shrink(array); grew = false; }
    // if should shrink and didnt grow shrink and didnt grow 
    
    array->count--;
}

// oh god! i gotta implement push!!
// i gotta implement pop!! noooo!!
// give me python array functions..
// im sorry Guido van Rossum
// if only there was a benevolent dictator of life for C that is sane 

#endif