#include "vector.h"

/**
 * Dynamically allocates a new vector.
 * @param elem_copy_func func which copies the element stored in the vector
 * (returns dynamically allocated copy).
 * @param elem_cmp_func func which is used to compare elements stored in the
 * vector.
 * @param elem_free_func func which frees elements stored in the vector.
 * @return pointer to dynamically allocated vector.
 * @if_fail return NULL.
 */
vector *vector_alloc (vector_elem_cpy elem_copy_func,
                      vector_elem_cmp elem_cmp_func,
                      vector_elem_free elem_free_func)
{
  vector *new_vector = malloc (sizeof (vector));
  void **data = malloc (VECTOR_INITIAL_CAP * sizeof (void *));
  if (new_vector == NULL || elem_copy_func == NULL || elem_cmp_func == NULL
      || elem_free_func == NULL || data == NULL) {return NULL;}
  new_vector->capacity = VECTOR_INITIAL_CAP;
  new_vector->size = 0;
  new_vector->data = data;
  new_vector->elem_copy_func = elem_copy_func;
  new_vector->elem_cmp_func = elem_cmp_func;
  new_vector->elem_free_func = elem_free_func;
  return new_vector;
}

/**
 * Frees a vector and the elements the vector itself allocated.
 * @param p_vector pointer to dynamically allocated pointer to vector.
 */
void vector_free (vector **p_vector)
{
  if (p_vector == NULL || (*p_vector) == NULL) {return;}
  for (size_t i = 0; i < (*p_vector)->size; i++)
    {(*p_vector)->elem_free_func ((*p_vector)->data + i);}
  free ((*p_vector)->data);
  free (*p_vector);
  (*p_vector) = NULL;
}

/**
 * Returns the element at the given index.
 * @param vector pointer to a vector.
 * @param ind the index of the element we want to get.
 * @return the element at the given index if exists (the element itself, not a
 * copy of it),
 * NULL otherwise.
 */
void *vector_at (const vector *vector, size_t ind)
{
  if (vector == NULL || ind >= vector->size) {return NULL;}
  return vector->data[ind];
}

/**
 * Gets a value and checks if the value is in the vector.
 * @param vector a pointer to vector.
 * @param value the value to look for.
 * @return the index of the given value if it is in the vector
 * ([0, vector_size - 1]).
 * Returns -1 if no such value in the vector.
 */
int vector_find (const vector *vector, const void *value)
{
  if (vector == NULL || value == NULL) {return -1;}
  for (size_t i = 0; i < vector->size; i++)
    {
      if (vector->data[i] != NULL && vector->elem_cmp_func (vector->data[i],
                                                             value) == 1)
        {return i;}
    }
  return -1;
}

/**
 * Expands or shrinks the vector (the amount of memory allocated for it)
 * according to the new capacity.
 * @param vector a pointer to vector.
 * @param new_capacity a new capacity of the vector.
 * @return 1 if the resizing has been done successfully, 0 otherwise.
 */
int vector_resize (vector *vector, size_t new_capacity)
{
  if (vector == NULL) {return 0;}
  void **temp = realloc (vector->data, new_capacity * sizeof (void *));
  if (temp == NULL) {return 0;}
  vector->capacity = new_capacity;
  vector->data = temp;
  return 1;
}

/**
 * Checks if a vector's load factor is bigger that the maximal load
 * factor and does resizing in that case.
 * @param vector a vector.
 * @return returns 1 if the vector's load factor is smaller or equal than
 * the maximal or the resizing was successful.
 */
int vector_check_bigger_do_resizing (vector *vector)
{
  vector->size++;
  if (vector_get_load_factor (vector) > VECTOR_MAX_LOAD_FACTOR)
    {
      size_t new_capacity = vector->capacity * VECTOR_GROWTH_FACTOR;
      if (!vector_resize (vector, new_capacity))
        {
          vector->size--;
          return 0;
        }
    }
  vector->size--;
  return 1;
}

/**
 * Checks if a vector's load factor is smaller that the minimal load
 * factor and does resizing in that case.
 * @param vector a vector.
 * @return returns 1 if the vector's load factor is bigger or equal than the
 * minimal or the resizing was successful.
 */
int vector_check_smaller_do_resizing (vector *vector)
{
  vector->size--;
  if (vector_get_load_factor (vector) < VECTOR_MIN_LOAD_FACTOR)
    {
      size_t new_capacity = vector->capacity / VECTOR_GROWTH_FACTOR;
      if (!vector_resize (vector, new_capacity))
        {
          vector->size++;
          return 0;
        }
    }
  vector->size++;
  return 1;
}


/**
 * Adds a new value to the back (index vector_size) of the vector.
 * @param vector a pointer to vector.
 * @param value the value to be added to the vector.
 * @return 1 if the adding has been done successfully, 0 otherwise.
 */
int vector_push_back (vector *vector, const void *value)
{
  if (vector == NULL || value == NULL || !vector_check_bigger_do_resizing
  (vector)) {return 0;}
  void *new_value = (void *) vector->elem_copy_func (value);
  if (new_value == NULL) {return 0;}
  vector->data[vector->size] = new_value;
  vector->size++;
  return 1;
}

/**
 * This function returns the load factor of the vector.
 * @param vector a vector.
 * @return the vector's load factor, -1 if the function failed.
 */
double vector_get_load_factor (const vector *vector)
{
  if (vector == NULL || vector->capacity == 0) {return -1;}
  return (double) vector->size / (double) vector->capacity;
}

/**
 * Removes the element at the given index from the vector. alters the indices
 * of the remaining elements so that there are no empty indices in the range
 * [0, size-1] (inclusive).
 * @param vector a pointer to vector.
 * @param ind the index of the element to be removed.
 * @return 1 if the removing has been done successfully, 0 otherwise.
 */
int vector_erase (vector *vector, size_t ind)
{
  if (vector == NULL || ind >= vector->size ||
  !vector_check_smaller_do_resizing (vector)) {return 0;}
  if (vector_get_load_factor (vector) < VECTOR_MIN_LOAD_FACTOR &&
      !vector_resize (vector, vector->capacity / VECTOR_GROWTH_FACTOR))
    {return 0;}
  vector->elem_free_func (vector->data + ind);
  for (size_t i = ind; i < vector->size - 1; i++)
    {
      vector->data[i] = vector->data[i + 1];
    }
  vector->size--;
  return 1;
}

/**
 * Deletes all the elements in the vector.
 * @param vector vector a pointer to vector.
 */
void vector_clear (vector *vector)
{
  if (vector == NULL) {return;}
  while (vector->size != 0)
    {
      if (!vector_erase (vector, vector->size - 1)) {return;}
    }
}
