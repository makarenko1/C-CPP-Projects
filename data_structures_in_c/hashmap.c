#include "hashmap.h"

/**
 * Allocates dynamically new hash map element.
 * @param func a function which "hashes" keys.
 * @return pointer to dynamically allocated hashmap.
 * @if_fail return NULL.
 */
hashmap *hashmap_alloc (hash_func func)
{
  hashmap *new_hashmap = malloc (sizeof (hashmap));
  vector **buckets = calloc (HASH_MAP_INITIAL_CAP, sizeof (vector));
  if (new_hashmap == NULL || func == NULL || buckets == NULL) {return NULL;}
  new_hashmap->buckets = buckets;
  new_hashmap->size = 0;
  new_hashmap->capacity = HASH_MAP_INITIAL_CAP;
  new_hashmap->hash_func = func;
  return new_hashmap;
}

/**
 * Frees all the buckets in the bucket list.
 * @param buckets a dynamically allocated array of vectors (a bucket list).
 * @param capacity a number of buckets in the bucket list.
 */
void buckets_free (vector **buckets, size_t capacity)
{
  if (buckets == NULL) {return;}
  for (size_t i = 0; i < capacity; i++)
    {
      if (buckets[i] != NULL)
        {vector_free (buckets + i);}
    }
  free (buckets);
}

/**
 * Frees a hash map and the elements the hash map itself allocated.
 * @param p_hash_map pointer to dynamically allocated pointer to hash_map.
 */
void hashmap_free (hashmap **p_hash_map)
{
  if (p_hash_map == NULL || (*p_hash_map) == NULL) {return;}
  buckets_free ((*p_hash_map)->buckets, (*p_hash_map)->capacity);
  free (*p_hash_map);
  (*p_hash_map) = NULL;
}

/**
 * Creates a new bucket at the index given and increases the size of the map.
 * @param buckets a bucket list of a hash map.
 * @param bucket_index an index to insert the new bucket to.
 * @return returns 1 for successful creation, 0 otherwise.
 */
int create_new_bucket (vector **buckets, size_t bucket_index)
{
  if (buckets == NULL) {return 0;}
  vector *new_vector = vector_alloc (pair_copy, pair_cmp, pair_free);
  if (new_vector == NULL) {return 0;}
  buckets[bucket_index] = new_vector;
  return 1;
}

/**
 * Moves all the pairs from one bucket to the new hash map.
 * @param bucket a bucket to move from.
 * @param func a hash function of the hash table.
 * @param new_capacity a new capacity of the hash table after the resize.
 * @param new_buckets a new array of buckets to move to.
 * @return returns 1 for a successful move, 0 otherwise.
 */
int move_one_bucket (const vector *bucket, hash_func func, size_t
new_capacity, vector **new_buckets)
{
  if (bucket == NULL || func == NULL || new_buckets == NULL) {return 0;}
  for (size_t i = 0; i < bucket->size; i++)
    {
      pair *current_pair = (pair *) vector_at (bucket, i);
      if (current_pair == NULL || current_pair->key == NULL) {return 0;}
      size_t bucket_index = func (current_pair->key) & (new_capacity - 1);
      if (new_buckets[bucket_index] == NULL && !create_new_bucket (
          new_buckets, bucket_index))
        {return 0;} // create a new vector if cell at the index is empty.
      if (!vector_push_back (new_buckets[bucket_index], current_pair))
        {return 0;}
    }
  return 1;
}

/**
 * Resizes a hash map and creates a new bigger or smaller copy of it. Moves
 * all the pairs from the old hash map into the new hash map.
 * @param hash_map an old hash map.
 * @param new_capacity how many vectors can be stored in the future.
 * @return returns 1 for successful resizing, 0 otherwise.
 */
int hashmap_resize (hashmap *hash_map, size_t new_capacity)
{
  if (hash_map == NULL) {return 0;}
  if (hash_map->capacity == new_capacity) // nothing to change.
    {return 1;}
  vector **new_buckets = calloc (new_capacity, sizeof (vector));
  if (new_buckets == NULL) {return 0;}
  for (size_t i = 0; i < hash_map->capacity; i++)
    {
      if (hash_map->buckets[i] == NULL) {continue;}
      if (!move_one_bucket (hash_map->buckets[i], hash_map->hash_func,
                            new_capacity, new_buckets))
      {
        buckets_free (new_buckets, new_capacity);
        return 0;
      }
    }
  buckets_free (hash_map->buckets, hash_map->capacity);
  hash_map->buckets = new_buckets;
  hash_map->capacity = new_capacity;
  return 1;
}

/**
 * Checks if a hash map's load factor is bigger that the maximal load
 * factor and does resizing in that case.
 * @param hash_map a hash map.
 * @return returns 1 if the hash map's load factor is smaller or equal than
 * the maximal or the resizing was successful.
 */
int check_bigger_do_resizing (hashmap *hash_map)
{
  if (hashmap_get_load_factor (hash_map) > HASH_MAP_MAX_LOAD_FACTOR)
    {
      size_t new_capacity = hash_map->capacity * HASH_MAP_GROWTH_FACTOR;
      if (!hashmap_resize (hash_map, new_capacity)) {return 0;}
    }
  return 1;
}

/**
 * Checks if a hash map's load factor is smaller that the minimal load
 * factor and does resizing in that case.
 * @param hash_map a hash map.
 * @return returns 1 if the hash map's load factor is bigger or equal than the
 * minimal or the resizing was successful.
 */
int check_smaller_do_resizing (hashmap *hash_map)
{
  if (hashmap_get_load_factor (hash_map) < HASH_MAP_MIN_LOAD_FACTOR)
    {
      size_t new_capacity = hash_map->capacity / HASH_MAP_GROWTH_FACTOR;
      if (!hashmap_resize (hash_map, new_capacity)) {return 0;}
    }
  return 1;
}

/**
 * Inserts a new in_pair to the hash map.
 * The function inserts *new*, *copied*, *dynamically allocated* in_pair,
 * NOT the in_pair it receives as a parameter.
 * @param hash_map the hash map to be inserted with new element.
 * @param in_pair a in_pair the hash map would contain.
 * @return returns 1 for successful insertion, 0 otherwise.
 */
int hashmap_insert (hashmap *hash_map, const pair *in_pair)
{
  if (hash_map == NULL || in_pair == NULL || in_pair->key == NULL ||
  in_pair->value == NULL || hashmap_at (hash_map, in_pair->key) != NULL)
    {return 0;}
  size_t bucket_index = hash_map->hash_func (in_pair->key) &
                        (hash_map->capacity - 1); // calculate the bucket ind.
  if (hash_map->buckets[bucket_index] == NULL && !create_new_bucket (
      hash_map->buckets, bucket_index))
    {return 0;} // try to create a new vector if cell at the index is empty.
  if (!vector_push_back (hash_map->buckets[bucket_index], in_pair))
    {return 0;} // try to push the copied pair in the vector in the cell.
  hash_map->size++;
  if (!check_bigger_do_resizing (hash_map))
    { // return everything as it was previously.
      size_t index = vector_find (hash_map->buckets[bucket_index], in_pair);
      vector_erase (hash_map->buckets[bucket_index], index);
      hash_map->size--;
      return 0;
    }
  return 1;
}

/**
 * The function returns the value associated with the given key.
 * @param hash_map a hash map.
 * @param key the key to be checked.
 * @return the value associated with key if exists, NULL otherwise (the value
 * itself, not a copy of it).
 */
valueT hashmap_at (const hashmap *hash_map, const_keyT key)
{
  if (hash_map == NULL || key == NULL) {return NULL;}
  size_t bucket_index = hash_map->hash_func (key) & (hash_map->capacity - 1);
  if (hash_map->buckets[bucket_index] == NULL) {return NULL;}
  for (size_t i = 0; i < hash_map->buckets[bucket_index]->size; i++)
    {
      pair *current_pair = (pair *) vector_at
          (hash_map->buckets[bucket_index], i);
      if (current_pair != NULL && current_pair->key != NULL &&
      current_pair->key_cmp (current_pair->key, key) == 1)
        {
          if (current_pair->value == NULL) {return NULL;}
          else {return current_pair->value;}
        }
    }
  return NULL; // the key was not found in the bucket at the index.
}

/**
 * Removes a pair_to_remove pair from the bucket at the index bucket_index
 * from the index index_in_bucket in it.
 * @param hash_map a hashmap.
 * @param pair_to_remove a pair to remove.
 * @param bucket_index an index of the bucket to remove the pair from.
 * @param index_in_bucket an index in the bucket to remove the pair from.
 * @return returns 1 for a successful remove, 0 otherwise.
 */
int erase_pair (hashmap *hash_map, pair *pair_to_remove, size_t
bucket_index, size_t index_in_bucket)
{
  pair *copied_pair = pair_copy (pair_to_remove);
  if (copied_pair == NULL) {return 0;}
  if (!vector_erase (hash_map->buckets[bucket_index], index_in_bucket))
    { // try to erase a pair from the bucket at the given index.
      pair_free ((void **) &copied_pair);
      return 0;
    }
  hash_map->size--;
  if (!check_smaller_do_resizing (hash_map))
    { // return everything as it was before.
      vector_push_back (hash_map->buckets[bucket_index], copied_pair);
      pair_free ((void **) &copied_pair);
      hash_map->size++;
      return 0;
    }
  pair_free ((void **) &copied_pair);
  return 1;
}

/**
 * The function erases the pair associated with key.
 * @param hash_map a hash map.
 * @param key a key of the pair to be erased.
 * @return 1 if the erasing was done successfully, 0 otherwise. (if key not in
 * map, considered fail).
 */
int hashmap_erase (hashmap *hash_map, const_keyT key)
{
  if (hash_map == NULL || key == NULL || hashmap_at (hash_map, key) == NULL
  || !check_smaller_do_resizing (hash_map)) {return 0;}
  size_t bucket_index = hash_map->hash_func (key) & (hash_map->capacity - 1);
  if (hash_map->buckets[bucket_index] == NULL) {return 0;}
  for (size_t i = 0; i < hash_map->buckets[bucket_index]->size; i++)
    {
      pair *current_pair = (pair *) vector_at
          (hash_map->buckets[bucket_index], i);
      if (current_pair != NULL && current_pair->key != NULL &&
      current_pair->key_cmp (current_pair->key, key) == 1)
        {return erase_pair (hash_map, current_pair, bucket_index, i);}
    }
  return 0; // the key was not found in the bucket at the index.
}

/**
 * This function returns the load factor of the hash map.
 * @param hash_map a hash map.
 * @return the hash map's load factor, -1 if the function failed.
 */
double hashmap_get_load_factor (const hashmap *hash_map)
{
  if (hash_map == NULL || hash_map->capacity == 0) {return -1;}
  return (double) hash_map->size / (double) hash_map->capacity;
}

/**
 * Checks for each key in each pair in one bucket if the condition holds (by
 * the first function), and then applies the second function.
 * @param bucket a bucket that is changed.
 * @param keyT_func a function that checks a condition on keyT and returns 1 if
 * true, 0 else.
 * @param valT_func a function that modifies valueT, in-place.
 * @param counter_changed number of changed values.
 * @return counter_changed.
 */
int apply_one_bucket (vector *bucket, keyT_func keyT_func,
                       valueT_func valT_func, int counter_changed)
{
  for (size_t j = 0; j < bucket->size; j++)
    {
      pair *current_pair = (pair *) vector_at (bucket, j);
      if (current_pair != NULL && current_pair->key != NULL && keyT_func
      (current_pair->key) == 1 && current_pair->value != NULL)
        {
          valT_func (current_pair->value);
          counter_changed++;
        }
    }
  return counter_changed;
}

/**
 * This function receives a hashmap and 2 functions, the first checks a
 * condition on the keys, and the seconds apply some modification on the
 * values. The function should apply the modification
 * only on the values that are associated with keys that meet the condition.
 * Example: if the hashmap maps char->int, keyT_func checks if the char is a
 * capital letter (A-Z), and val_t_func multiples the number by 2,
 * hashmap_apply_if will change the map:
 * {('C',2),('#',3),('X',5)}, to: {('C',4),('#',3),('X',10)}, and the return
 * value will be 2.
 * @param hash_map a hashmap.
 * @param keyT_func a function that checks a condition on keyT and returns 1 if
 * true, 0 else.
 * @param valT_func a function that modifies valueT, in-place.
 * @return number of changed values.
 */
int hashmap_apply_if (const hashmap *hash_map, keyT_func keyT_func,
                      valueT_func valT_func)
{
  if (hash_map == NULL || keyT_func == NULL || valT_func == NULL) {return -1;}
  int counter_changed = 0;
  for (size_t i = 0; i < hash_map->capacity; i++)
    {
      if (hash_map->buckets[i] == NULL) {continue;}
      counter_changed = apply_one_bucket (hash_map->buckets[i], keyT_func,
                                          valT_func, counter_changed);
    }
  return counter_changed;
}