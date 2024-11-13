#include "darray.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void* _darray_create(unsigned long long length, unsigned long long stride) {
    unsigned long long header_size = DARRAY_FIELD_LENGTH * sizeof(unsigned long long);
    unsigned long long array_size = length * stride;
    unsigned long long* new_array = malloc(header_size + array_size);
    memset(new_array, 0, header_size + array_size);
    new_array[DARRAY_CAPACITY] = length;
    new_array[DARRAY_LENGTH] = 0;
    new_array[DARRAY_STRIDE] = stride;
    return(void*)(new_array + DARRAY_FIELD_LENGTH);
}

void _darray_destroy(void* array) {
    unsigned long long* header = (unsigned long long*)array - DARRAY_FIELD_LENGTH;
    free(header);
}

unsigned long long _darray_field_get(void* array, unsigned long long field) {
    unsigned long long* header = (unsigned long long*)array - DARRAY_FIELD_LENGTH;
    return header[field];
}

void _darray_field_set(void* array, unsigned long long field, unsigned long long value) {
    unsigned long long* header = (unsigned long long*)array - DARRAY_FIELD_LENGTH;
    header[field] = value;
}

void* _darray_resize(void* array) {
    unsigned long long length = darray_length(array);
    unsigned long long stride = darray_stride(array);
    void* temp = _darray_create(
        (DARRAY_RESIZE_FACTOR * darray_capacity(array)),
        stride);
    memcpy(temp, array, length * stride);    

    _darray_field_set(temp, DARRAY_LENGTH, length);
    _darray_destroy(array);
    return temp;
}

void* _darray_push(void* array, const void* value_ptr) {
    unsigned long long length = darray_length(array);
    unsigned long long stride = darray_stride(array);
    if(length >= darray_capacity(array)) {
        array = _darray_resize(array);
    }

    unsigned long long addr = (unsigned long long)array;
    addr += (length * stride);
    memcpy((void*)addr, value_ptr, stride);
    _darray_field_set(array, DARRAY_LENGTH, length + 1);
    return array;
}

void _darray_pop(void* array, void* dest) {
    unsigned long long length = darray_length(array);
    unsigned long long stride = darray_stride(array);
    unsigned long long addr = (unsigned long long)array;
    addr += ((length - 1) * stride);
    memcpy(dest, (void*)addr, stride);
    _darray_field_set(array, DARRAY_LENGTH, length - 1);
}

void* _darray_pop_at(void* array, unsigned long long index, void* dest) {
    unsigned long long length = darray_length(array);
    unsigned long long stride = darray_stride(array);
    if(index >= length) {
        printf("Index outside the bounds of this array!");
        return array;
    }

    unsigned long long addr = (unsigned long long)array;
    memcpy(dest, (void*)(addr + (index * stride)), stride);

    //If not on the last element, snip out the entry and copy the rest inward.
    if(index != length - 1) {
        memcpy(
            (void*)(addr + (index * stride)),
            (void*)(addr + ((index + 1) * stride)),
            stride * (length - index));
    }

    _darray_field_set(array, DARRAY_LENGTH, length - 1);
    return array;
};

void* _darray_insert_at(void* array, unsigned long long index, void* value_ptr) {
    unsigned long long length = darray_length(array);
    unsigned long long stride = darray_stride(array);
    if(index >= length) {
        printf("Index outside the bounds of this array!");
        return array;
    }
    if(length >= darray_capacity(array)) {
        array = _darray_resize(array);
    }

    unsigned long long addr = (unsigned long long)array;

    //If not on the last element, copy the rest outward.
    if(index != length - 1) {
        memcpy(
            (void*)(addr+((index+1)*stride)),
            (void*)(addr+(index*stride)),
            stride*(length-index));
    }

    // Set the value at the index
    memcpy((void*)(addr+(index*stride)), value_ptr, stride);

    _darray_field_set(array, DARRAY_LENGTH, length + 1);
    return array;
}
