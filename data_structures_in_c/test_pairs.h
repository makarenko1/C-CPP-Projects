#ifndef _TEST_PAIRS_H_
#define _TEST_PAIRS_H_

#include <stdlib.h>
#include <ctype.h>
#include "pair.h"
#define DOUBLE 2

/**
 * Represents one item in a shop. Each element has a code number (an int) and
 * a price (a double).
 */
typedef struct item
{
  int code_num;
  int price;
} item;

/**
 * Copies the char key of the pair.
 * @param key a char key.
 * @return returns a new allocated char key.
 */
void *char_key_cpy (const_keyT key)
{
  char *new_char = malloc (sizeof (char));
  *new_char = *((char *) key);
  return new_char;
}

/**
 * Copies the int value of the pair.
 * @param value an int value.
 * @return returns a new allocated int value.
 */
void *int_value_cpy (const_valueT value)
{
  int *new_int = malloc (sizeof (int));
  *new_int = *((int *) value);
  return new_int;
}

/**
 * Compares the char key of the pair.
 * @param key_1 the first constant value.
 * @param key_2 the second constant value.
 * @return returns 1 if the values are equal, 0 otherwise.
 */
int char_key_cmp (const_keyT key_1, const_keyT key_2)
{
  return *(char *) key_1 == *(char *) key_2;
}

/**
 * Compares the int value of the pair.
 * @param val_1 the first constant value.
 * @param val_2 the second constant value.
 * @return returns 1 if the values are equal, 0 otherwise.
 */
int int_value_cmp (const_valueT val_1, const_valueT val_2)
{
  return *(int *) val_1 == *(int *) val_2;
}

/**
 * Frees the char key of the pair.
 */
void char_key_free (keyT* key)
{
  if (key && *key)
    {
      free (*key);
      *key = NULL;
    }
}

/**
 * Frees the int value of the pair.
 */
void int_value_free (valueT *val)
{
  if (val && *val)
    {
      free (*val);
      *val = NULL;
    }
}

/**
 * Creates a pair of a char key and an int value.
 * @param char_key a char key.
 * @param int_value an int value.
 * @return returns an allocated pair.
 */
pair *create_char_int_pair (char char_key, int int_value)
{
  return pair_alloc ((const keyT) &char_key, (const valueT) &int_value,
                     char_key_cpy, int_value_cpy,
                     char_key_cmp, int_value_cmp,
                     char_key_free, int_value_free);
}

/**
 * @param elem pointer to a char (keyT).
 * @return returns 1 if the char is a number, else - 0.
 */
int is_num (const_keyT elem)
{
  char symbol = *((char *) elem);
  if (isdigit (symbol)) {return 1;}
  else {return 0;}
}

/**
 * Makes the int value pointed to by the given pointer zero.
 * @param elem pointer to an int (valT).
 */
void zero_value (valueT elem)
{
  *((int *) elem) = 0;
}

/**
 * Copies the int key of the pair.
 * @param key an int key.
 * @return returns a new allocated int key.
 */
void *int_key_cpy (const_keyT key)
{
  int *new_int = malloc (sizeof (int));
  *new_int = *((int *) key);
  return new_int;
}

/**
 * Copies the item value of the pair.
 * @param value an item value.
 * @return returns a new allocated item value.
 */
void *item_value_cpy (const_valueT value)
{
  item *new_item = malloc (sizeof (item));
  if (new_item == NULL) {return NULL;}
  item given_item = *((item *) value);
  new_item->code_num = given_item.code_num;
  new_item->price = given_item.price;
  return new_item;
}

/**
 * Compares the int key of the pair.
 * @param key_1 the first constant key.
 * @param key_2 the second constant key.
 * @return returns 1 if the keys are equal, 0 otherwise.
 */
int int_key_cmp (const_keyT key_1, const_keyT key_2)
{
  return *(int *) key_1 == *(int *) key_2;
}

/**
 * Compares the item value of the pair.
 * @param val_1 the first constant value.
 * @param val_2 the second constant value.
 * @return returns 1 if the values are equal, 0 otherwise.
 */
int item_value_cmp (const_valueT val_1, const_valueT val_2)
{
  item *item_1 = (item *) val_1;
  item *item_2 = (item *) val_2;
  return ((item_1->code_num == item_2->code_num) && (item_1->price ==
  item_2->price));
}

/**
 * Frees the int key of the pair.
 */
void int_key_free (keyT *key)
{
  if (key && *key)
    {
      free (*key);
      *key = NULL;
    }
}

/**
 * Frees the item value of the pair.
 */
void item_value_free (valueT* val)
{
  if (val && *val)
    {
      free (*val);
      *val = NULL;
    }
}

/**
 * Creates a pair of an int key and an item value.
 * @param i a value for a key and a value.
 * @return returns an allocated pair.
 */
pair *create_int_item_pair (size_t i)
{
  item *new_item = malloc (sizeof (item));
  if (new_item == NULL) {return NULL;}
  new_item->code_num = (int) i;
  new_item->price = (int) i;
  pair *new_pair = pair_alloc ((const keyT) &i, (const valueT) new_item,
                               int_key_cpy, item_value_cpy,
                               int_key_cmp, item_value_cmp,
                               int_key_free, item_value_free);
  free (new_item);
  return new_pair;
}

/**
 * Checks if a key is even.
 * @param Elem pointer to an int (keyT)
 * @return returns 1 if the int is even, 0 otherwise.
 */
int is_even (const_keyT elem)
{
  int int_elem = *((int *) elem);
  return int_elem % DOUBLE == 0;
}

/**
 * Doubles the price of the item value pointed to by the given pointer.
 * @param elem pointer to an item (valT).
 */
void double_price (valueT elem)
{
  item *item_elem = (item *) elem;
  item_elem->price *= DOUBLE;
}

#endif //_TEST_PAIRS_H_