#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  void *data;
  size_t size;      // Number of elements currently stored
  size_t capacity;  // Total allocated slots
  size_t elem_size; // Size of a single element in bytes
} List;

// Create a new list with a given element size and initial capacity
List *list_create(size_t elem_size, size_t initial_capacity);

// Append an element (copies data from ptr)
int list_append(List *list, const void *element);

// Get a pointer to the element at index (DO NOT free this pointer)
// Returns NULL if index out of bounds
void *list_get_ptr(List *list, size_t index);

// Get a copy of the element at index into 'out'
int list_get(List *list, size_t index, void *out);

// Set element at index (copies data from element)
int list_set(List *list, size_t index, const void *element);

// Insert element at index (shifts existing elements right)
int list_insert(List *list, size_t index, const void *element);

// Remove element at index, shifting left
// If 'out' is provided, copies the removed element into it
int list_remove(List *list, size_t index, void *out);

// Remove last element
int list_pop(List *list, void *out);

// Clear the list (keeps capacity, resets size)
void list_clear(List *list);

// Free the list and its internal buffer
void list_destroy(List *list);

// Helper to print integer list (for demo)
void print_int_list(List *list);