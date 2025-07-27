#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dynamicarray.h"

/*
 Dynamic Array Project – University Exercise

 This program implements a basic dynamic array in C with functions like
 `dynamic_array_append()` to store sensor values read from a CSV file.

 The data comes from:
 Luftqualitaet_DEBE066_Berlin-Karlshorst_2025-01-01_00-2025-05-09_08.csv
 The file contains Nitrogen Dioxide (NO2) concentration values measured in Berlin Karlshorst.

 Purpose:
 - Practice pointer manipulation and dynamic memory allocation
 - Practice implementing a dynamic array in C
 - Parse and process real-world sensor data from a CSV file
*/


// Resize the dynamic array to a new capacity if it's full or needs more space
static int resize(DynamicArray* array, size_t newcap) { 

	int* newdata = (int*)realloc(array->data, newcap * sizeof(int));
	// realloc attempts to resize the memory block, copying old data if needed.
	// May move to a new location; original pointer becomes invalid if successful.
	// Returns NULL if failed
	
	if (newdata == NULL && newcap > 0) {
		perror("Realloc failed");
		return 0;
	}


	array->data = newdata;
	array->capacity = newcap;
	// Store the new memory address and update the array's capacity

	if (array->size > newcap) {// The size should not be bigger than the capacity of the array
		array->size = newcap;
	}
	return 1;
}

DynamicArray* dynamic_array_create(size_t initial_cap) { 
	//Create the array with memory allocation

	DynamicArray* array = (DynamicArray*)malloc(sizeof(DynamicArray));//Allocating memory for the array

	if (array == NULL) {
		perror("Malloc error");
		return NULL;
	}
	// If memory allocation failed I want to return NULL

	array->capacity = 0;
	array->data = NULL;
	array->size = 0;
	// Initializing the array

	if (initial_cap > 0) {
		if (resize(array, initial_cap) == NULL) {
			free(array);
			return NULL;
		}
	}
	/*If there is a given capacity to initialize the array, we want to resize the capacity from 0 to the given capacity
	If the resize function fails to resize the array, we want to free the array to avoid memory leaks
	*/

	return array;
}

int dynamic_array_append(DynamicArray* array, int value) {
	/*For this project appending to the array is enough, we do not need to insert data at the front or the middle of the array
	I could have implemented a stack, but i wanted to learn the dynamic array
	*/

	if (array == NULL) {
		perror("Appending failed");
		return 0;
	}
	// Always check for possible errors

	if (array->size == array->capacity) {
		size_t newcap = array->capacity == 0 ? 1 : array->capacity * 2;
		if (resize(array, newcap) == NULL) {
			return 0;
		}
	}
	/* Checking if the capacity is big enough for one more element.If the capacity is 0, we want to change it to 1,
	else we want to double the current capacity
	*/

	array->data[array->size] = value;
	array->size++;
	return 1;
}

int dynamic_array_get(const DynamicArray* array, size_t index) {
	// Getting a value stored in the array

	if (array == NULL) {
		perror("Getting value failed");
		return 0;
	}


	int value = array->data[index];
	return value;
}

int dynamic_array_set(DynamicArray* array, size_t index, int value) {
	// Set a current value in the array to the new given value

	if (array == NULL || array->size<=index) {
		perror("Setting value failed");
		return 0;
	}
	// The index must be smaller than the current size; otherwise, we're writing out of bounds.

	array->data[index] = value;
	return 1;
}

void dynamic_array_destroy(DynamicArray* array) {
	// A function to destroy the array, if we do not need it no more

	if (array == NULL) {
		perror("Destroying failed");
		return;
	}


	if (array->data != NULL) {
		free(array->data);
	}
	/* We need to free the internal data before freeing the array structure itself,
	otherwise we lose access to the data pointer and cause undefined behavior.
	*/

	free(array);
}

size_t dynamic_array_size(const DynamicArray* array) {
	// Function for us to know the current number of elements in the array

	if (array == NULL) {
		perror("Size cannot be printed");
		return 0;
	}

	return array->size;
}

size_t dynamic_array_capacity(const DynamicArray* array) {
	// Function for us to know the current capacity of the array

	if (array == NULL) {
		perror("Capacity cannot be printed");
		return 0;
	}

	return array->capacity;
}

int load_no2_data_from_csv(const char filename[], DynamicArray* array) {
	//Function to load the data from the .csv file

	FILE* file = fopen(filename, "r");

	if (file == NULL) {
		perror("File cant be opened");
		return 0;
	} 

	char line[1024];//buffer for the current line to safe the string
	int line_num = 0;

	while (fgets(line, sizeof(line), file)) {
		if (line_num++ < 4) continue;// Skip first 4 lines – actual sensor data starts at line 5

		char* token;
		int column = 0;
		char* no2_str = NULL;//needs to be NULL else something unpredicatble happens in if (no2_str && strcmp(no2_str, "-") != 0)
		token = strtok(line, ",");// Use ',' as the delimiter to split the CSV line into tokens

		while (token != NULL) {// token == NULL if the end of the line is reached

			if (column == 4) { // The sensor value is located in column 4 (5th column in CSV)
				no2_str = token;
				break;
			}

			token = strtok(NULL, ",");
			column++;
		}


		if (no2_str && strcmp(no2_str, "-") != 0) {//strcmp(no2_str, "-") checks if no2str == - and returns 0 if true

			int value = atoi(no2_str); // atoi turns no2_str into an integer

			if (!dynamic_array_append(array, value)) { // Append the value to the array
				fclose(file);
				return 0;
			}
		}
	}


	fclose(file);
	return 1;
}

float avg_no2(DynamicArray* array) {
	// Calculate the average of all values in the array

	if (array == NULL || array->size == 0) {
		return 0.0;
	}

	float gesamt = 0.0f;

	for (int i = 0; i < array->size; i++) {
		gesamt += array->data[i];
	}

	float avg = gesamt / array->size;
	
	return avg;
}

float max_no2(DynamicArray* array) {
	// Search for the highest value in the array

	if (array == NULL || array->size == 0) {
		return 0.0;
	}

	float max = 0;
	
	for (int i = 0; i < array->size; i++) {
		if (array->data[i] > max) {
			max = array->data[i];
		}
	}
	
	return max;
}

float min_no2(DynamicArray* array) {
	// Search for the lowest value in the array

	if (array == NULL || array->size == 0) {
		return 0.0;
	}

	float min = 30.0; // From reading the csv file I know that there are lower values than 30.0
	
	for (int i = 0; i < array->size; i++) {
		if (array->data[i] < min) {
			min = array->data[i];
		}
	}

	return min;
}

DynamicArray* detect_crit_no2(DynamicArray* array) {
	// Function to detect critical NO2 values and store them in an array

	if (array == NULL || array->size == 0) {
		return NULL;
	}

	int counter = 0;
	for (int i = 0; i < array->size; i++) {
		if (array->data[i] > 20) {
			counter++;
		}
	}
	// Count all critical values

	DynamicArray* crit_array = (DynamicArray*)malloc(sizeof(DynamicArray));//Create an array for the critical values
	if (crit_array == NULL) {
		perror("Malloc failed");
		return NULL;
	}


	crit_array->data = NULL;
	crit_array->size = 0;
	crit_array->capacity = 0;

	if (resize(crit_array, counter) == 0) {
		perror("Resizing criticial array failed");
		free(crit_array);
		return NULL;
	}

	int cnt = 0;
	for (int i = 0; i < array->size; i++) {
		if (array->data[i] > 20) {
			crit_array->data[cnt] = array->data[i];
			cnt++;
			crit_array->size++;
		}
	}
	// Fill critical array with critical values

	return crit_array;
}


int main() {

	DynamicArray* array = dynamic_array_create(0);

	if (!array) {
		printf("Error\n");
		return 1;
	}

	if (!load_no2_data_from_csv("C:\\Users\\Samsung\\Downloads\\Luftqualitaet_DEBE066_Berlin-Karlshorst_2025-01-01_00-2025-05-09_08.csv", array)) {
		perror("Could not load csv file\n");
		dynamic_array_destroy(array);
		return 1;
	}

	for (size_t i = 0; i < array->size; ++i) {
		printf("NO2-Value %zu: %d\n", i, array->data[i]);
	}

	float avg = avg_no2(array);
	printf("Average: %.2f\n", avg);

	float max = max_no2(array);
	printf("Max. Value: %.2f\n", max);

	float min = min_no2(array);
	printf("Min. Value: %.2f\n", min);

	DynamicArray* dynamic_crit = detect_crit_no2(array);
	for (int i = 0; i < dynamic_crit->size; i++) {
		printf("NO2-value %i: %i\n",i, dynamic_crit->data[i]);
	}

	dynamic_array_destroy(array);
	dynamic_array_destroy(dynamic_crit);
	return 0;
}

