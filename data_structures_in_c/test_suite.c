#include "test_suite.h"
#include "hash_funcs.h"
#include "test_pairs.h"
#define KEY 1
#define VALUE 1
#define NUM_PAIRS 100
#define NUM_DIGITS 10
#define FIRST_DIGIT 48
#define BAD_LOAD_FACTOR (-1)
#define BAD_APPLY_IF (-1)

/**
 * This function fills a passed hash map with the {char, int} pairs.
 * @param hash_map a hash map.
 * @return returns a hash map with the pairs added.
 */
hashmap *fill_char_int (hashmap *hash_map)
{
  for (size_t i = 1; i <= NUM_PAIRS; i++)
    {
      pair *new_pair = create_char_int_pair ((char) i, (int) i);
      assert (new_pair != NULL);
      int result = hashmap_insert (hash_map, new_pair);
      assert (result == 1);
      pair_free ((void **) &new_pair);
    }
  return hash_map;
}

/**
 * This function fills a passed hash map with the {int, item} pairs.
 * @param hash_map a hash map.
 * @return returns a hash map with the pairs added.
 */
hashmap *fill_int_item (hashmap *hash_map)
{
  for (size_t i = 1; i <= NUM_PAIRS; i++)
    {
      pair *new_pair = create_int_item_pair (i);
      assert (new_pair != NULL);
      int result = hashmap_insert (hash_map, new_pair);
      assert (result == 1);
      pair_free ((void **) &new_pair);
    }
  return hash_map;
}

/**
 * This test checks the case when incorrect input is sent to the insert
 * function.
 */
void test_input_insert ()
{
  int result;
  // insert with all NULL pointers.
  result = hashmap_insert (NULL, NULL);
  assert (result == 0);
  pair *char_int_pair = create_char_int_pair ((char) KEY, (char) VALUE);
  hashmap *hash_map = hashmap_alloc (hash_char);
  assert (hash_map != NULL && char_int_pair != NULL);
  int result1, result2;
  // insert with one correct field and one NULL field.
  result1 = hashmap_insert (NULL, char_int_pair);
  result2 = hashmap_insert (hash_map, NULL);
  assert (result1 == 0 && result2 == 0);
  assert (hash_map != NULL && char_int_pair != NULL);
  assert (hash_map->capacity == HASH_MAP_INITIAL_CAP && hash_map->size == 0
  && hashmap_at (hash_map, char_int_pair->key) == NULL);
  pair_free ((void **) &char_int_pair);
  hashmap_free (&hash_map);
}

/**
 * This test checks the case when a key that already exists in a hash table
 * is inserted.
 */
void test_same_key_insert ()
{
  hashmap *hash_map = hashmap_alloc (hash_char);
  assert (hash_map != NULL);
  pair *pair1 = create_char_int_pair ((char) KEY, (int) VALUE);
  pair *pair2 = create_char_int_pair ((char) KEY, (int) (VALUE + 1));
  assert (pair1 != NULL && pair2 != NULL);
  int result1 = hashmap_insert (hash_map, pair1);
  assert (result1 == 1);
  int result2 = hashmap_insert (hash_map, pair1);
  assert (result2 == 0 && hash_map != NULL && hash_map->size == 1);
  result2 = hashmap_insert (hash_map, pair2);
  assert (result2 == 0 && hash_map != NULL && hash_map->size == 1 &&
  pair2 != NULL && hashmap_at (hash_map, pair2->key) != NULL);
  pair_free ((void **) &pair1);
  pair_free ((void **) &pair2);
  hashmap_free (&hash_map);
}

/**
 * This test checks that different keys in the hash map can have the same
 * values.
 */
void test_diff_keys_same_values ()
{
  hashmap *hash_map = hashmap_alloc (hash_char);
  assert (hash_map != NULL);
  pair *pair1 = create_char_int_pair ((char) KEY, (int) VALUE);
  pair *pair2 = create_char_int_pair ((char) (KEY + 1), (int) VALUE);
  assert (pair1 != NULL && pair2 != NULL);
  int result1 = hashmap_insert (hash_map, pair1);
  int result2 = hashmap_insert (hash_map, pair2);
  assert (result1 == 1 && result2 == 1);
  pair_free ((void **) &pair1);
  pair_free ((void **) &pair2);
  hashmap_free (&hash_map);
}

/**
 * This test checks that after the insert of one pair the rest of the pairs
 * in the hash map remain unchanged.
 */
void test_unchanged_insert ()
{
  hashmap *hash_map = hashmap_alloc (hash_char);
  assert (hash_map != NULL);
  fill_char_int (hash_map);
  pair *char_int_pair = create_char_int_pair ((char) (NUM_PAIRS + 1), (int)
      (NUM_PAIRS + 1));
  assert (char_int_pair != NULL);
  int result = hashmap_insert (hash_map, char_int_pair);
  assert (result == 1);
  pair_free ((void **) &char_int_pair);
  for (size_t i = 1; i <= NUM_PAIRS; i++)
    {
      char_int_pair = create_char_int_pair ((char) i, (int) i);
      assert (char_int_pair != NULL);
      valueT value = hashmap_at (hash_map, char_int_pair->key);
      assert (value != NULL);
      assert (char_int_pair != NULL && char_int_pair->value_cmp (
          value, char_int_pair->value) == 1);
      pair_free ((void **) &char_int_pair);
    }
  hashmap_free (&hash_map);
}

/**
 * This test checks that all the elements are at their places.
 * @param hash_map a hash map.
 */
void test_change_places_resize (hashmap *hash_map)
{
  for (size_t i = 0; i < hash_map->capacity; i++)
    {
      if (hash_map->buckets[i] == NULL) {continue;}
      for (size_t j = 0; j < hash_map->buckets[i]->size; j++)
        {
          pair *current_pair = vector_at (hash_map->buckets[i], j);
          assert (current_pair != NULL);
          assert (i == (hash_map->hash_func (current_pair->key) &
          (hash_map->capacity - 1)));
        }
    }
}

/**
 * Checks if the size and the capacity of the hash map are exactly what they
 * need to be.
 * @param hash_map a hash map.
 * @param current_size a current size.
 * @param previous_capacity a capacity from the previous iteration.
 */
void check_parameters_insert (hashmap *hash_map, size_t current_size, size_t
previous_capacity)
{
  double cur_load_factor = hashmap_get_load_factor (hash_map);
  // check the size is the number of pairs, load factor is smaller than max.
  assert (hash_map != NULL && cur_load_factor != BAD_LOAD_FACTOR &&
  hash_map->size == current_size && cur_load_factor <=
  HASH_MAP_MAX_LOAD_FACTOR);
  // check that we only make the capacity bigger.
  assert (hash_map->capacity >= previous_capacity);
  // check the capacity before the first resize.
  if (current_size <= HASH_MAP_MAX_LOAD_FACTOR * HASH_MAP_INITIAL_CAP)
    {assert (hash_map->capacity == HASH_MAP_INITIAL_CAP);}
  // check the capacity if there was a resize.
  if (current_size > HASH_MAP_MAX_LOAD_FACTOR * previous_capacity)
    {
      assert (hash_map->capacity == HASH_MAP_GROWTH_FACTOR *
                                    previous_capacity);
    }
  // check the capacity if there wasn't a resize.
  else
    {assert (hash_map->capacity == previous_capacity);}
  test_change_places_resize (hash_map);
}

/**
 * The same part in the tests of two different types.
 * @param hash_map a hash map.
 * @param pair a pair to be tested.
 * @param current_size a current sie of the hash map.
 * @param previous_capacity a capacity of the hash map from the previous
 * iteration.
 */
void helper_insert (hashmap *hash_map, pair *pair, size_t current_size, size_t
previous_capacity)
{
  int result = hashmap_insert (hash_map, pair);
  assert (result == 1);
  // check that the pair's key and value are in the hash map.
  valueT value = hashmap_at (hash_map, pair->key);
  assert (value != NULL);
  assert (pair != NULL && pair->value_cmp (value, pair->value) == 1);
  // check if a copy is inserted.
  assert (hashmap_at (hash_map, pair->key) != pair->value);
  check_parameters_insert (hash_map, current_size, previous_capacity);
  pair_free ((void **) &pair);
}

/**
 * This test checks the case when {char, int} pairs are inserted.
 */
void test_char_int_insert ()
{
  hashmap *hash_map = hashmap_alloc (hash_char);
  assert (hash_map != NULL);
  pair *char_int_pair;
  for (size_t i = 1; i <= NUM_PAIRS; i++)
    {
      char_int_pair = create_char_int_pair ((char) i, (int) i);
      assert (char_int_pair != NULL);
      helper_insert (hash_map, char_int_pair, i, hash_map->capacity);
    }
  hashmap_free (&hash_map);
}

/**
 * This test checks the case when {int, item (struct)} pairs are inserted.
 */
void test_int_item_insert ()
{
  hashmap *hash_map = hashmap_alloc (hash_int);
  assert (hash_map != NULL);
  pair *int_item_pair;
  for (size_t i = 1; i <= NUM_PAIRS; i++)
    {
      int_item_pair = create_int_item_pair (i);
      assert (int_item_pair != NULL);
      helper_insert (hash_map, int_item_pair, i, hash_map->capacity);
    }
  hashmap_free (&hash_map);
}

/**
 * This function checks the hashmap_insert function of the hashmap library.
 * If hashmap_insert fails at some points, the functions exits with exit code
 * 1.
 */
void test_hash_map_insert (void)
{
  test_input_insert ();
  test_same_key_insert ();
  test_diff_keys_same_values ();
  test_unchanged_insert ();
  test_char_int_insert ();
  test_int_item_insert ();
}

/**
 * This test checks the case when the input is incorrect.
 */
void test_input_at ()
{
  valueT result = hashmap_at (NULL, NULL);
  assert (result == NULL);
  hashmap *hash_map = hashmap_alloc (hash_char);
  assert (hash_map != NULL);
  char key = (char) KEY;
  result = hashmap_at (NULL, (keyT) &key);
  assert (result == NULL);
  result = hashmap_at (hash_map, NULL);
  assert (result == NULL);
  result = hashmap_at (hash_map, (keyT) &key);
  assert (result == NULL);
  hashmap_free (&hash_map);
}

/**
 * This test checks the case when we want to find a pair in an empty hash map.
 */
void test_empty_at ()
{
  hashmap *hash_map = hashmap_alloc (hash_char);
  assert (hash_map != NULL);
  pair *pair1 = create_char_int_pair ((char) KEY, (int) VALUE);
  pair *pair2 = create_char_int_pair ((char) (KEY + 1), (int) VALUE);
  assert (pair1 != NULL && pair2 != NULL);
  // check that the pairs are not found when the hash table is empty.
  assert (hashmap_at (hash_map, pair1) == NULL && hashmap_at (hash_map,
                                                              pair2) == NULL);
  assert (hashmap_insert (hash_map, pair1) == 1);
  // check that after the insert the first pair is found and the second is not.
  valueT value = hashmap_at (hash_map, pair1->key);
  assert (value != NULL && pair1 != NULL && pair1->value_cmp (value,
                                                         pair1->value) == 1);
  assert (hashmap_at (hash_map, pair2) == NULL);
  pair_free ((void **) &pair1);
  pair_free ((void **) &pair2);
  hashmap_free (&hash_map);
}

/**
 * The same part in the tests of two different types.
 * @param hash_map a hash map.
 * @param pair a pair to be tested.
 * @param key a key of the pair at the moment of creation.
 */
void helper_at (struct hashmap *hash_map, pair *pair, keyT key)
{
  // assume that hashmap_insert works well.
  assert (hashmap_insert (hash_map, pair) == 1);
  valueT value = hashmap_at (hash_map, pair->key);
  // check that the key isn't changed.
  assert (pair != NULL && pair->key_cmp (key, pair->key) == 1);
  // check that the returned value is the value of the pair.
  assert (value != NULL && pair->value_cmp (value, pair->value) == 1);
  // check that the value itself is returned and not a copy of it.
  valueT value_n = hashmap_at (hash_map, pair->key);
  assert (value_n != NULL && pair != NULL && pair->value_cmp (
      value_n, pair->value) == 1 && value == value_n);
  pair_free ((void **) &pair);
}

/**
 * This test checks the case when the key and the value of the searched pair
 * are chars.
 */
void test_char_int_at ()
{
  hashmap *hash_map = hashmap_alloc (hash_char);
  assert (hash_map != NULL);
  pair *char_int_pair;
  for (size_t i = 1; i <= NUM_PAIRS; i++)
    {
      char key = (char) i;
      char_int_pair = create_char_int_pair ((char) i, (int) i);
      assert (char_int_pair != NULL);
      helper_at (hash_map, char_int_pair, (keyT) &key);
      key = (char) (i + 1); // check that a non-existent key is not found.
      assert (hashmap_at (hash_map, (keyT) &key) == NULL);
    }
  hashmap_free (&hash_map);
}

/**
 * This test checks the case when the key and the value of the searched pair
 * are chars.
 */
void test_int_item_at ()
{
  hashmap *hash_map = hashmap_alloc (hash_int);
  assert (hash_map != NULL);
  pair *int_item_pair;
  for (size_t i = 1; i <= NUM_PAIRS; i++)
    {
      int key = (int) i;
      int_item_pair = create_int_item_pair (i);
      assert (int_item_pair != NULL);
      helper_at (hash_map, int_item_pair, (keyT) &key);
      key = (int) (i + 1); // check that a non-existent key is not found.
      assert (hashmap_at (hash_map, (keyT) &key) == NULL);
    }
  hashmap_free (&hash_map);
}

/**
 * This function checks the hashmap_at function of the hashmap library.
 * If hashmap_at fails at some points, the functions exits with exit code 1.
 */
void test_hash_map_at (void)
{
  test_input_at ();
  test_empty_at ();
  test_char_int_at ();
  test_int_item_at ();
}

/**
 * This test checks the case when the input is incorrect or we try to erase
 * from an empty hash table.
 */
void test_input_and_empty_erase ()
{
  int result = hashmap_erase (NULL, NULL);
  assert (result == 0);
  hashmap *hash_map = hashmap_alloc (hash_char);
  assert (hash_map != NULL);
  char key1 = (char) KEY;
  char key2 = (char) (KEY + 1);
  int result1 = hashmap_erase (NULL, (keyT) &key1);
  int result2 = hashmap_erase (NULL, (keyT) &key2);
  int result3 = hashmap_erase (hash_map, NULL);
  int result4 = hashmap_erase (hash_map, (keyT) &key1);
  int result5 = hashmap_erase (hash_map, (keyT) &key2);
  assert (result1 == 0 && result2 == 0 && result3 == 0 && result4 == 0 &&
  result5 == 0);
  assert (hash_map != NULL && hashmap_at (hash_map, (keyT) &key1) == NULL &&
  hashmap_at (hash_map, (keyT) &key2) == NULL && hash_map->size == 0 &&
  hash_map->capacity == HASH_MAP_INITIAL_CAP);
  hashmap_free (&hash_map);
}

/**
 * This test checks the case when we try to erase the keys that don't exist
 * in the hash map.
 */
void test_not_found_erase ()
{
  hashmap *hash_map = hashmap_alloc (hash_int);
  assert (hash_map != NULL);
  for (size_t i = 1; i <= NUM_PAIRS; i++)
    {
      int key = i;
      int result = hashmap_erase (hash_map, (keyT) &key);
      assert (result == 0);
      assert (hash_map != NULL && hashmap_at (hash_map, (keyT) &key) == NULL
      && hash_map->size == 0 && hash_map->capacity == HASH_MAP_INITIAL_CAP);
    }
  hashmap_free (&hash_map);
}

/**
 * This test checks the case when we try to delete the same pair twice.
 */
void test_double_delete ()
{
  hashmap *hash_map = hashmap_alloc (hash_char);
  assert (hash_map != NULL);
  pair *pair = create_char_int_pair ((char) KEY, (int) VALUE);
  assert (pair != NULL);
  int result = hashmap_erase (hash_map, pair->key);
  assert (result == 0);
  assert (hashmap_insert (hash_map, pair) == 1);
  result = hashmap_erase (hash_map, pair->key);
  assert (result == 1);
  result = hashmap_erase (hash_map, pair->key);
  assert (result == 0);
  pair_free ((void **) &pair);
  hashmap_free (&hash_map);
}

/**
 * This test checks that after erasing one pair the rest are unchanged.
 */
void test_unchanged_erase ()
{
  hashmap *hash_map = hashmap_alloc (hash_char);
  assert (hash_map != NULL);
  fill_char_int (hash_map);
  pair *char_int_pair = create_char_int_pair ((char) (NUM_PAIRS), (int)
      (NUM_PAIRS));
  assert (char_int_pair != NULL);
  int result = hashmap_erase (hash_map, char_int_pair->key);
  assert (result == 1);
  pair_free ((void **) &char_int_pair);
  for (size_t i = 1; i < NUM_PAIRS; i++)
    {
      char_int_pair = create_char_int_pair ((char) i, (int) i);
      assert (char_int_pair != NULL);
      valueT value = hashmap_at (hash_map, char_int_pair->key);
      assert (value != NULL);
      assert (char_int_pair != NULL && char_int_pair->value_cmp (
          value, char_int_pair->value) == 1);
      pair_free ((void **) &char_int_pair);
    }
  hashmap_free (&hash_map);
}

/**
 * Checks if the size and the capacity of the hash map are exactly what they
 * need to be.
 * @param hash_map a hash map.
 * @param current_size a current size.
 * @param starting_capacity a capacity after all insert was finished.
 * @param previous_capacity a capacity from the previous iteration.
 */
void check_parameters_erase (hashmap *hash_map, size_t current_size, size_t
starting_capacity, size_t previous_capacity)
{
  double cur_load_factor = hashmap_get_load_factor (hash_map);
  // check the size is the number of pairs, load factor is bigger than min.
  assert (cur_load_factor != BAD_LOAD_FACTOR && hash_map->size ==
  current_size);
  if (hash_map->size != 0)
    {assert (cur_load_factor >= HASH_MAP_MIN_LOAD_FACTOR);}
  // check that we only make the capacity smaller.
  assert (hash_map->capacity <= previous_capacity);
  // check the capacity before the first resize.
  if (current_size >= HASH_MAP_MIN_LOAD_FACTOR * starting_capacity)
    {assert (hash_map->capacity == starting_capacity);}
  // check the capacity if there was a resize.
  if (current_size < HASH_MAP_MIN_LOAD_FACTOR * previous_capacity)
    {
      assert (hash_map->capacity == previous_capacity /
      HASH_MAP_GROWTH_FACTOR);
    }
    // check the capacity if there wasn't a resize.
  else
    {assert (hash_map->capacity == previous_capacity);}
  test_change_places_resize (hash_map);
}

/**
 * The same part in the tests of two different types.
 * @param hash_map a hash map.
 * @param pair a pair to be tested.
 * @param current_size a current sie of the hash map.
 * @param previous_capacity a capacity of the hash map from the previous
 * iteration.
 */
void helper_erase (hashmap *hash_map, pair *pair, size_t current_size,
                   size_t starting_capacity, size_t previous_capacity)
{
  assert (pair != NULL);
  int result = hashmap_erase (hash_map, pair->key);
  assert (result == 1);
  // check that the pair's key and value are not in the hash map.
  assert (pair != NULL && hashmap_at (hash_map, pair->key) == NULL);
  check_parameters_erase (hash_map, current_size,
                           starting_capacity, previous_capacity);
  pair_free ((void **) &pair);
}

/**
 * This test checks the case when {char, int} pairs are erased.
 */
void test_char_int_erase ()
{
  hashmap *hash_map = hashmap_alloc (hash_char);
  assert (hash_map != NULL);
  fill_char_int (hash_map);
  size_t starting_capacity = hash_map->capacity;
  pair *char_int_pair;
  for (size_t i = 1; i <= NUM_PAIRS; i++)
    {
      assert (hash_map != NULL);
      size_t previous_capacity = hash_map->capacity;
      char_int_pair = create_char_int_pair ((char) i, (int) i);
      helper_erase (hash_map, char_int_pair, NUM_PAIRS - i,
                    starting_capacity, previous_capacity);
      // check that a non-existent key is not erased.
      assert (hash_map != NULL);
      previous_capacity = hash_map->capacity; // after the erase.
      char key = (char) (NUM_PAIRS + i);
      int result = hashmap_erase (hash_map, (keyT) &key);
      assert (hash_map != NULL && result == 0 && hash_map->size == NUM_PAIRS
      - i && hash_map->capacity == previous_capacity);
    }
  hashmap_free (&hash_map);
}

/**
 * This test checks the case when {int, item (struct)} pairs are erased.
 */
void test_int_item_erase ()
{
  hashmap *hash_map = hashmap_alloc (hash_int);
  assert (hash_map != NULL);
  fill_int_item (hash_map);
  size_t starting_capacity = hash_map->capacity;
  pair *int_item_pair;
  for (size_t i = 1; i <= NUM_PAIRS; i++)
    {
      assert (hash_map != NULL);
      size_t previous_capacity = hash_map->capacity;
      int_item_pair = create_int_item_pair (i);
      helper_erase (hash_map, int_item_pair, NUM_PAIRS - i,
                    starting_capacity, previous_capacity);
      // check that a non-existent key is not erased.
      assert (hash_map != NULL);
      previous_capacity = hash_map->capacity; // after the erase.
      int key = (int) (NUM_PAIRS + i);
      int result = hashmap_erase (hash_map, (keyT) &key);
      assert (hash_map != NULL && result == 0 && hash_map->size == NUM_PAIRS
      - i && hash_map->capacity == previous_capacity);
    }
  hashmap_free (&hash_map);
}

/**
 * This function checks the hashmap_erase function of the hashmap library.
 * If hashmap_erase fails at some points, the functions exits with exit code 1.
 */
void test_hash_map_erase (void)
{
  test_input_and_empty_erase ();
  test_not_found_erase ();
  test_double_delete ();
  test_unchanged_erase ();
  test_char_int_erase ();
  test_int_item_erase ();
}

/**
 * This test checks the case when the input is incorrect.
 */
void test_input_get_load_factor ()
{
  double result = hashmap_get_load_factor (NULL);
  assert (result == BAD_LOAD_FACTOR);
  hashmap *hash_map = hashmap_alloc (hash_char);
  assert (hash_map != NULL);
  result = hashmap_get_load_factor (hash_map);
  assert (result == 0);
  hashmap_free (&hash_map);
}

/**
 * This test checks the correctness of the load factor in the erase.
 * @param my_hash_map a hash map of ints.
 * @param cur_capacity a capacity after the inserts.
 */
void test_erase_get_load_factor (hashmap *my_hash_map, int cur_capacity)
{
  hashmap *hash_map = my_hash_map;
  assert (hash_map != NULL);
  int key;
  int capacity = cur_capacity;
  for (size_t i = 1; i <= NUM_PAIRS; i++)
    {
      key = i;
      assert (hashmap_erase (hash_map, (keyT) &key) == 1);
      if ((double) (NUM_PAIRS - i) / (double) capacity <
          HASH_MAP_MIN_LOAD_FACTOR) {capacity /= HASH_MAP_GROWTH_FACTOR;}
      double cur_load_factor = hashmap_get_load_factor (hash_map);
      assert (cur_load_factor != BAD_LOAD_FACTOR && cur_load_factor ==
      (double) (NUM_PAIRS - i) / (double) capacity);
      if (i != NUM_PAIRS)
        {assert (cur_load_factor >= HASH_MAP_MIN_LOAD_FACTOR);}
    }
}

/**
 * This test checks the correctness of the load factor in the insert.
 */
void test_insert_get_load_factor ()
{
  hashmap *hash_map = hashmap_alloc (hash_char);
  assert (hash_map != NULL);
  pair *char_int_pair;
  int capacity = HASH_MAP_INITIAL_CAP;
  for (size_t i = 1; i <= NUM_PAIRS; i++)
    {
      char_int_pair = create_char_int_pair ((char) i, (int) i);
      assert (char_int_pair != NULL);
      assert (hashmap_insert (hash_map, char_int_pair) == 1);
      if ((double) i / (double) capacity > HASH_MAP_MAX_LOAD_FACTOR)
        {capacity *= HASH_MAP_GROWTH_FACTOR;}
      double cur_load_factor = hashmap_get_load_factor (hash_map);
      assert (cur_load_factor != BAD_LOAD_FACTOR && cur_load_factor ==
      (double) i / (double) capacity && cur_load_factor <=
      HASH_MAP_MAX_LOAD_FACTOR);
      pair_free ((void **) &char_int_pair);
    }
  test_erase_get_load_factor (hash_map, capacity);
  hashmap_free (&hash_map);
}

/**
 * This function checks the hashmap_get_load_factor function of the hashmap
 * library. If hashmap_get_load_factor fails at some points, the functions
 * exits with exit code 1.
 */
void test_hash_map_get_load_factor (void)
{
  test_input_get_load_factor ();
  test_insert_get_load_factor ();
}

/**
 * This test checks the case when the input is incorrect.
 */
void test_input_apply_if ()
{
  int result = hashmap_apply_if (NULL, NULL, NULL);
  assert (result == BAD_APPLY_IF);
  hashmap *hash_map = hashmap_alloc (hash_char);
  assert (hash_map != NULL);
  int result1 = hashmap_apply_if (NULL, is_num, zero_value);
  int result2 = hashmap_apply_if (NULL, is_num, NULL);
  int result3 = hashmap_apply_if (NULL, NULL, zero_value);
  int result4 = hashmap_apply_if (hash_map, NULL, zero_value);
  int result5 = hashmap_apply_if (hash_map, is_even, NULL);
  int result6 = hashmap_apply_if (hash_map, NULL, NULL);
  assert (result1 == BAD_APPLY_IF && result2 == BAD_APPLY_IF && result3 ==
  BAD_APPLY_IF && result4 == BAD_APPLY_IF && result5 == BAD_APPLY_IF &&
  result6 == BAD_APPLY_IF);
  result = hashmap_apply_if (hash_map, is_even, zero_value);
  assert (result == 0);
  hashmap_free (&hash_map);
}

/**
 * This test checks that the application of the hashmap_apply_if doesn't
 * influence other pairs in the hash map.
 */
void test_unchanged_apply_if ()
{
  hashmap *hash_map = hashmap_alloc (hash_char);
  assert (hash_map != NULL);
  pair *pair1 = create_char_int_pair ((char) KEY, (int) VALUE);
  pair *pair2 = create_char_int_pair ((char) (KEY + 1), (int) (VALUE + 1));
  assert (pair1 != NULL && pair2 != NULL);
  assert (hashmap_insert (hash_map, pair1) == 1);
  assert (hashmap_insert (hash_map, pair2) == 1);
  int result = hashmap_apply_if (hash_map, is_num, zero_value);
  assert (result == 0);
  valueT value1 = hashmap_at (hash_map, pair1->key);
  valueT value2 = hashmap_at (hash_map, pair2->key);
  assert (value1 != NULL && value2 != NULL);
  assert (pair1 != NULL && pair1->value_cmp (value1, pair1->value) == 1);
  assert (pair2 != NULL && pair2->value_cmp (value2, pair2->value) == 1);
  pair_free ((void **) &pair1);
  pair_free ((void **) &pair2);
  hashmap_free (&hash_map);
}

/**
 * This test checks the case when hashmap_apply_if does nothing because the
 * table is empty or there aren't any keys that apply to the keys function.
 */
void no_key_or_empty_apply_if ()
{
  hashmap *hash_map = hashmap_alloc (hash_char);
  assert (hash_map != NULL);
  pair *pair;
  int result = hashmap_apply_if (hash_map, is_num, zero_value);
  assert (result == 0);
  for (size_t i = 0; i < FIRST_DIGIT; i++)
    {
      pair = create_char_int_pair ((char) i, (int) i);
      assert (pair != NULL);
      assert (hashmap_insert (hash_map, pair) == 1);
      pair_free ((void **) &pair);
    }
  result = hashmap_apply_if (hash_map, is_num, zero_value);
  assert (result == 0);
  hashmap_free (&hash_map);
}

/**
 * This test checks all the {char, int)} pairs in a hash map after the
 * hashmap_apply_if function.
 */
void test_char_int_apply_if ()
{
  hashmap *hash_map = hashmap_alloc (hash_char);
  assert (hash_map != NULL);
  fill_char_int (hash_map);
  int result = hashmap_apply_if (hash_map, is_num, zero_value);
  assert (result == NUM_DIGITS);
  for (size_t i = 1; i <= NUM_PAIRS; i++)
    {
      char key = (char) i;
      valueT value = hashmap_at (hash_map, (keyT) &key);
      assert (value != NULL);
      if (isdigit ((int) key))
        {assert (*(char *) value == (char) 0);}
      else
        {assert (*(char *) value == (char) key);}
    }
  hashmap_free (&hash_map);
}

/**
 * This test checks all the {int, item (struct)} pairs in a hash map after the
 * hashmap_apply_if function.
 */
void test_int_item_apply_if ()
{
  hashmap *hash_map = hashmap_alloc (hash_int);
  assert (hash_map != NULL);
  fill_int_item (hash_map);
  int result = hashmap_apply_if (hash_map, is_even, double_price);
  assert (result == (NUM_PAIRS / DOUBLE));
  for (size_t i = 1; i <= NUM_PAIRS; i++)
    {
      int key = (int) i;
      valueT value = hashmap_at (hash_map, (keyT) &key);
      assert (value != NULL);
      item *item_value = (item *) value;
      if (key % DOUBLE == 0)
        {assert (item_value != NULL && item_value->price == DOUBLE * key);}
      else
        {assert (item_value != NULL && item_value->price == key);}
    }
  hashmap_free (&hash_map);
}

/**
 * This function checks the HashMapGetApplyIf function of the hashmap library.
 * If HashMapGetApplyIf fails at some points, the functions exits with exit
 * code 1.
 */
void test_hash_map_apply_if ()
{
  test_input_apply_if ();
  test_unchanged_apply_if ();
  no_key_or_empty_apply_if ();
  test_char_int_apply_if ();
  test_int_item_apply_if ();
}
