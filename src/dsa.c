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
List *list_create(size_t elem_size, size_t initial_capacity) {
  if (elem_size == 0)
    return NULL;

  List *list = malloc(sizeof(List));
  if (!list)
    return NULL;

  list->elem_size = elem_size;
  list->capacity = (initial_capacity > 0) ? initial_capacity : 16;
  list->size = 0;
  list->data = malloc(list->capacity * elem_size);

  if (!list->data) {
    free(list);
    return NULL;
  }
  return list;
}

// Internal helper to grow capacity
static int grow_capacity(List *list) {
  size_t new_capacity = list->capacity * 2;
  void *new_data = realloc(list->data, new_capacity * list->elem_size);
  if (!new_data)
    return -1;

  list->data = new_data;
  list->capacity = new_capacity;
  return 0;
}

// Append an element (copies data from ptr)
int list_append(List *list, const void *element) {
  if (!list || !element)
    return -1;

  if (list->size >= list->capacity) {
    if (grow_capacity(list) != 0)
      return -1;
  }

  // Copy the element into the buffer
  char *base = (char *)list->data;
  memcpy(base + list->size * list->elem_size, element, list->elem_size);

  list->size++;
  return 0;
}

// Get a pointer to the element at index (DO NOT free this pointer)
// Returns NULL if index out of bounds
void *list_get_ptr(List *list, size_t index) {
  if (!list || index >= list->size)
    return NULL;
  return (char *)list->data + index * list->elem_size;
}

// Get a copy of the element at index into 'out'
int list_get(List *list, size_t index, void *out) {
  if (!list || !out || index >= list->size)
    return -1;

  char *base = (char *)list->data;
  memcpy(out, base + index * list->elem_size, list->elem_size);
  return 0;
}

// Set element at index (copies data from element)
int list_set(List *list, size_t index, const void *element) {
  if (!list || !element || index >= list->size)
    return -1;

  char *base = (char *)list->data;
  memcpy(base + index * list->elem_size, element, list->elem_size);
  return 0;
}

// Insert element at index (shifts existing elements right)
int list_insert(List *list, size_t index, const void *element) {
  if (!list || !element || index > list->size)
    return -1;

  if (list->size >= list->capacity) {
    if (grow_capacity(list) != 0)
      return -1;
  }

  char *base = (char *)list->data;
  size_t bytes_to_move = (list->size - index) * list->elem_size;

  // Shift elements to the right
  memmove(base + (index + 1) * list->elem_size, base + index * list->elem_size,
          bytes_to_move);

  // Insert new element
  memcpy(base + index * list->elem_size, element, list->elem_size);

  list->size++;
  return 0;
}

// Remove element at index, shifting left
// If 'out' is provided, copies the removed element into it
int list_remove(List *list, size_t index, void *out) {
  if (!list || index >= list->size)
    return -1;

  char *base = (char *)list->data;
  size_t offset = index * list->elem_size;

  if (out) {
    memcpy(out, base + offset, list->elem_size);
  }

  size_t bytes_to_move = (list->size - index - 1) * list->elem_size;

  // Shift elements left
  memmove(base + offset, base + offset + list->elem_size, bytes_to_move);

  list->size--;
  return 0;
}

// Remove last element
int list_pop(List *list, void *out) {
  if (!list || list->size == 0)
    return -1;
  return list_remove(list, list->size - 1, out);
}

// Clear the list (keeps capacity, resets size)
void list_clear(List *list) {
  if (list)
    list->size = 0;
}

// Free the list and its internal buffer
void list_destroy(List *list) {
  if (!list)
    return;
  free(list->data);
  free(list);
}

// Helper to print integer list (for demo)
void print_int_list(List *list) {
  printf("[");
  for (size_t i = 0; i < list->size; i++) {
    int val;
    list_get(list, i, &val);
    if (i > 0)
      printf(", ");
    printf("%d", val);
  }
  printf("] (size=%zu, cap=%zu)\n", list->size, list->capacity);
}

// Example usage with integers and structs
typedef struct {
  char *name;
  int age;
} Person;

int list_example(void) {
  // 1. List of Integers
  printf("--- Integers ---\n");
  List *int_list = list_create(sizeof(int), 4);

  int vals[] = {10, 20, 30};
  for (size_t i = 0; i < 3; i++) {
    list_append(int_list, &vals[i]);
  }
  print_int_list(int_list);

  // Insert 100 at index 1
  int insert_val = 100;
  list_insert(int_list, 1, &insert_val);
  print_int_list(int_list);

  // 2. List of Structs (Person)
  printf("\n--- Structs (Person) ---\n");
  List *person_list = list_create(sizeof(Person), 2);

  Person p1 = {"Alice", 30};
  Person p2 = {"Bob", 25};

  list_append(person_list, &p1);
  list_append(person_list, &p2);

  // Access and print
  for (size_t i = 0; i < person_list->size; i++) {
    Person *p = (Person *)list_get_ptr(person_list, i);
    printf("Person %zu: %s, age %d\n", i, p->name, p->age);
  }

  // Modify a struct in place
  Person *p = (Person *)list_get_ptr(person_list, 0);
  if (p) {
    p->age = 31; // Direct modification
    printf("Updated Alice to age %d\n", p->age);
  }

  list_destroy(int_list);
  list_destroy(person_list);

  return 0;
}