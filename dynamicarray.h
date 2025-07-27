#pragma once
#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int* data;
	size_t capacity;
	size_t size;
}DynamicArray;

DynamicArray* dynamic_array_create(size_t initial_capacity);

int dynamic_array_append(DynamicArray* array, int value);

int dynamic_array_get(const DynamicArray* array, size_t index);

int dynamic_array_set(DynamicArray* array, size_t index, int value);

void dynamic_array_destroy(DynamicArray* array);

size_t dynamic_array_size(const DynamicArray* array);

size_t dynamic_array_capacity(const DynamicArray* array);

