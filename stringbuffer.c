/*
 ============================================================================
 Name        : Simple Remote Execution System Linux client
 Author      : Alex
 Version     : 0.9v
 Copyright   : No copyright
 Description : 2803ICT assignment 1, Ansi-style, CLion + Ubuntu
 ============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include "stringbuffer.h"

void buffer_init(Buffer *buf) {
    // initialize size and capacity
    buf->size = 0;
    buf->capacity = BUFFER_INITIAL_CAPACITY;

    // allocate memory for vector->data
    buf->data = malloc(sizeof(char) * buf->capacity);
}

void buffer_append(Buffer *buf, char* value, size_t nCount) {
    // make sure there's room to expand into
    while (nCount > (buf->capacity - buf->size)){
        buf->capacity *= 2;
        buf->data = realloc(buf->data, sizeof(char) * buf->capacity);
    }
    buf->size += snprintf(buf->data + buf->size, nCount + 1, "%s", value);
}

void buffer_append_short(Buffer *buf, int value){
    buf->size += snprintf(buf->data + buf->size, 2, "%d", value);
}

void buffer_append_timestamp(Buffer *buf, long long value){
    buf->size += snprintf(buf->data + buf->size, 14, "%lld", value);
}

int buffer_get(Buffer *buf, int index) {
    if (index >= buf->size || index < 0) {
        printf("Index %d out of bounds for vector of size %d\n", index, buf->size);
        exit(1);
    }
    return buf->data[index];
}

void buffer_free(Buffer *buf) {
    free(buf->data);
}
