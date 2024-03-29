//
// Created by charname on 7/23/19.
//

#ifndef STRINGBUFFERTEST_STRINGBUFFER_H
#define STRINGBUFFERTEST_STRINGBUFFER_H

#define VECTOR_INITIAL_CAPACITY 10

typedef struct {
    int size;      // slots used so far
    int capacity;  // total available slots
    char *data;     // array of integers we're storing
} Vector;

void vector_init(Vector *vector);

void vector_append(Vector *vector, char* value, size_t nCount);

int vector_get(Vector *vector, int index);

void vector_short(Vector *vector, int value);
//void vector_set(Vector *vector, int index, int value);

//void vector_double_capacity_if_full(Vector *vector);

void vector_free(Vector *vector);

#endif //STRINGBUFFERTEST_STRINGBUFFER_H
