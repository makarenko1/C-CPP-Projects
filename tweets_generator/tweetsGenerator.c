/* AUTHOR: MARIIA MAKARENKO
 * ID NUMBER: 342849676
 * EX NUMBER: 3
 * COURSE: 67315 PROGRAMMING WORKSHOP IN C & C++ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORDS_IN_SENTENCE_GENERATION 20
#define MIN_WORDS_IN_SENTENCE_GENERATION 2
#define MAX_WORD_LENGTH 100
#define MAX_SENTENCE_LENGTH 1000
#define SEED_IDX 1
#define SENTENCES_IDX 2
#define FILE_IDX 3
#define NUM_WORDS_IDX 4
#define MIN_ARGS_NUM 4
#define MAX_ARGS_NUM 5
#define USAGE_ERROR ("Usage: tweetsGenerator <seed> <sentences> <file> <words \
to read>\n")
#define FILE_ERROR "Error: File won't open.\n"
#define CONVERT_ERROR "Error: Strtol won't work.\n"
#define ALLOC_ERROR "Allocation failure: need more memory.\n"
#define TWEET_MESSAGE "Tweet %d: "
#define DELIMITERS " \n"
#define BASE 10
#define DOT_CHAR '.'
#define READ_ALL (-1)

typedef struct WordStruct {
  char *word;
  struct WordProbability *prob_list;
  int appears_total;
  int size_prob_list;
} WordStruct;

typedef struct WordProbability {
  struct WordStruct *word_struct_ptr;
  int appears_after;
} WordProbability;

/************ LINKED LIST ************/
typedef struct Node {
  WordStruct *data;
  struct Node *next;
} Node;

typedef struct LinkList {
  Node *first;
  Node *last;
  int size;
} LinkList;

/**
 * Add data to new node at the end of the given link list.
 * @param link_list Link list to add data to
 * @param data pointer to dynamically allocated data
 * @return 0 on success, 1 otherwise
 */
int add (LinkList *link_list, WordStruct *data)
{
  Node *new_node = malloc (sizeof (Node));
  if (new_node == NULL)
    {
      return 1;
    }
  *new_node = (Node) {data, NULL};

  if (link_list->first == NULL)
    {
      link_list->first = new_node;
      link_list->last = new_node;
    }
  else
    {
      link_list->last->next = new_node;
      link_list->last = new_node;
    }

  link_list->size++;
  return 0;
}
/*************************************/

/**
 * In case of an allocation error prints an error message and exits the
 * program without freeing the resources used.
 */
void treat_alloc_error ()
{
  printf (ALLOC_ERROR);
  exit (EXIT_FAILURE);
}

/**
 * Get random number between 0 and max_number [0, max_number).
 * @param max_number
 * @return Random number
 */
int get_random_number (int max_number)
{
  if (max_number == 0)
    return max_number;
  return rand () % max_number;
}

/**
 * Choose randomly the next word from the given dictionary, drawn uniformly.
 * The function won't return a word that end's in full stop '.' (Nekuda).
 * @param dictionary Dictionary to choose a word from
 * @return WordStruct of the chosen word
 */
WordStruct *get_first_random_word (LinkList *dictionary)
{
  Node *current;
  do
    {
      int random_num;
      random_num = get_random_number (dictionary->size);
      current = dictionary->first;
      for (int i = 0; i < random_num; i++)
        {
          current = current->next;
        }
    } while (current->data->prob_list == NULL); // check for a dot.
  return current->data;
}

/**
 * Choose randomly the next word. Depend on it's occurrence frequency
 * in word_struct_ptr->WordProbability.
 * @param word_struct_ptr WordStruct to choose from
 * @return WordStruct of the chosen word
 */
WordStruct *get_next_random_word (WordStruct *word_struct_ptr)
{
  int random_number = get_random_number (word_struct_ptr->appears_total);
  int max_cur_word_occurs = 0;
  for (int i = 0;; i++)
    {
      max_cur_word_occurs += word_struct_ptr->prob_list[i].appears_after;
      if (random_number < max_cur_word_occurs)
        return word_struct_ptr->prob_list[i].word_struct_ptr;
    }
}

/**
 * Receive dictionary, generate and print to stdout random sentence out of it.
 * The sentence most have at least 2 words in it.
 * @param dictionary Dictionary to use
 * @return Amount of words in printed sentence
 */
int generate_sentence (LinkList *dictionary)
{
  WordStruct *word_for_printing = get_first_random_word (dictionary);
  printf ("%s", word_for_printing->word);
  int word_counter = 1;
  while (word_counter < MAX_WORDS_IN_SENTENCE_GENERATION)
    {
      word_for_printing = get_next_random_word (word_for_printing);
      if (word_for_printing->prob_list == NULL) // check for a dot.
        {
          if (word_counter + 1 < MIN_WORDS_IN_SENTENCE_GENERATION)
            continue;
          word_counter++;
          printf (" %s", word_for_printing->word);
          break;
        }
      printf (" %s", word_for_printing->word);
      word_counter++;
    }
  return word_counter;
}

/**
 * Expands the prob_list of the first word by one WordProbability
 * struct object. Creates a WordProbability object for the second word and
 * adds it to the end of the probability list of the first word.
 * @param first_word
 * @param second_word
 */
void treat_not_in_prob_list (WordStruct *first_word, WordStruct *second_word)
{
  first_word->size_prob_list += 1;
  first_word->prob_list = realloc (
      first_word->prob_list, first_word->size_prob_list * sizeof
          (WordProbability));
  if (first_word->prob_list == NULL)
    treat_alloc_error ();
  WordProbability *current = first_word->prob_list +
                             first_word->size_prob_list - 1;
  current->appears_after = 1;
  current->word_struct_ptr = second_word;
}

/**
 * Gets 2 WordStructs. If second_word in first_word's prob_list,
 * update the existing probability value.
 * Otherwise, add the second word to the prob_list of the first word.
 * @param first_word
 * @param second_word
 * @return 0 if already in list, 1 otherwise.
 */
int add_word_to_probability_list (WordStruct *first_word,
                                  WordStruct *second_word)
{
  if (first_word->prob_list == NULL) // check if contains a dot.
    return 1;
  for (int i = 0; i < first_word->size_prob_list; i++) // check if in the list.
    {
      if (!strcmp (first_word->prob_list[i].word_struct_ptr->word,
                   second_word->word))
        {
          first_word->prob_list[i].appears_after += 1; // update the counter.
          return 0;
        }
    }
  treat_not_in_prob_list (first_word, second_word);
  return 1;
}

/**
 * Creates a probability list for the given WordStruct object word. If
 * the word has a full stop in the end, the probability list is initialized
 * to NULL.
 * @param word
 */
void create_prob_list (WordStruct *word)
{
  if (word->word[strlen (word->word) - 1] == DOT_CHAR)
    {
      word->prob_list = NULL;
      return;
    }
  WordProbability *prob_list = malloc (sizeof (WordProbability));
  if (prob_list == NULL)
    treat_alloc_error ();
  word->prob_list = prob_list;
  word->size_prob_list = 0;
}

/**
 * Creates, initializes and returns a WordStruct object for the given string
 * in a memory block. The string is copied to a new block of memory and
 * added into the WordStruct. The counter of the word in text is initialized
 * to 1, the size of the probability list is initialized to 0. In the end we
 * add the created object into the dictionary.
 * @param dictionary
 * @param str_memory_block A memory block with a string from the file
 * @return The new WordStruct object
 */
WordStruct *create_word_struct (LinkList *dictionary, char *str_memory_block)
{
  char *word = malloc (MAX_WORD_LENGTH);
  WordStruct *word_struct = malloc (sizeof (WordStruct));
  if (word_struct == NULL || word == NULL)
    treat_alloc_error ();
  word = strcpy (word, str_memory_block);
  word_struct->word = word;
  word_struct->appears_total = 1;
  word_struct->size_prob_list = 0;
  create_prob_list (word_struct);
  if (add (dictionary, word_struct))
    treat_alloc_error ();
  return word_struct;
}

/**
 * Checks if the word string is already in the dictionary. If it is, updates
 * the counter of the word in text. If it isn't creates a new WordStruct
 * object for the word and adds it to the dictionary.
 * @param dictionary
 * @param word A word string from the file
 * @return If the word is in list, return its WordStruct object. If it
 * isn't, return the WordStruct object added
 */
WordStruct *treat_one_word (LinkList *dictionary, char *word)
{
  Node *current = dictionary->first;
  while (current != NULL) // check if the word is in the list.
    {
      if (!strcmp (current->data->word, word)) // if it is, return it.
        {
          current->data->appears_total += 1; // update the word counter.
          return current->data;
        }
      current = current->next;
    }
  return create_word_struct (dictionary, word); // if it isn't, create struct.
}

/**
 * Read word from the given file. Add every unique word to the dictionary.
 * Also, at every iteration, update the prob_list of the previous word with
 * the value of the current word.
 * @param fp File pointer
 * @param words_to_read Number of words to read from file.
 *                      If value is bigger than the file's word count,
 *                      or if words_to_read == -1 than read entire file.
 * @param dictionary Empty dictionary to fill
 */
void fill_dictionary (FILE *fp, int words_to_read, LinkList *dictionary)
{
  char buffer[MAX_SENTENCE_LENGTH];
  int word_counter = 0;
  WordStruct *previous = NULL;
  while (fgets (buffer, MAX_SENTENCE_LENGTH, fp) != NULL)
    {
      char *word = strtok (buffer, DELIMITERS);
      while (word != NULL)
        {
          if (words_to_read != READ_ALL && word_counter == words_to_read)
            return;
          WordStruct *new_word = treat_one_word (dictionary, word);
          if (previous != NULL)
            add_word_to_probability_list (previous, new_word);
          word_counter++;
          previous = new_word;
          word = strtok (NULL, DELIMITERS);
        }
    }
}

/**
 * Free the given dictionary and all of it's content from memory.
 * @param dictionary Dictionary to free
 */
void free_dictionary (LinkList *dictionary)
{
  Node *current = dictionary->first;
  while (current != NULL)
    {
      free (current->data->prob_list);
      free (current->data->word);
      free (current->data);
      Node *temp = current->next;
      free (current);
      current = temp;
    }
  free (dictionary);
  dictionary = NULL;
}

/**
 * Creates a dictionary and fills it with words from the given file. Then
 * generates num_sentences sentences. In the end calls to the special free
 * function to free the dictionary.
 * @param num_words Number of words to read from the file
 * @param file File with words
 * @param num_sentences Number of sentences to create and print
 */
void run (int num_words, FILE *file, int num_sentences)
{
  LinkList *dictionary = malloc (sizeof (LinkList));
  if (dictionary == NULL)
    treat_alloc_error ();
  dictionary->size = 0;
  dictionary->first = NULL;
  dictionary->last = NULL;
  fill_dictionary (file, num_words, dictionary);
  for (int i = 1; i <= num_sentences; i++)
    {
      printf (TWEET_MESSAGE, i);
      generate_sentence (dictionary);
      printf ("\n");
    }
  free_dictionary (dictionary);
  dictionary = NULL;
}

/**
 * Checks if the fifth parameter has been specified prior to the run of the
 * program.
 * @param argc
 * @param argv
 * @return If the parameter was specified, return it. Otherwise, return the
 * default value
 */
int determine_num_words (int argc, char *argv[])
{
  int num_words = READ_ALL;
  if (argc == MAX_ARGS_NUM)
    {
      num_words = (int) strtol (argv[NUM_WORDS_IDX], NULL, BASE);
    }
  return num_words;
}

/**
 * Checks if the user's command arguments are valid (they are of the right
 * quantity, and the specified file opens).
 * @param argc
 * @param argv
 * @return If the arguments are valid, return 0. Otherwise, return 1.
 */
int get_input_and_run (int argc, char *argv[])
{
  srand ((unsigned int) strtol (argv[SEED_IDX], NULL, BASE));
  int sentences = (int) strtol (argv[SENTENCES_IDX], NULL, BASE);
  FILE *file = fopen (argv[FILE_IDX], "r");
  int num_words = determine_num_words (argc, argv);
  if (!num_words)
    {
      printf (CONVERT_ERROR); // strtol failed.
      return 1;
    }
  if (file == NULL)
    {
      printf (FILE_ERROR); // file didn't open.
      return 1;
    }
  run (num_words, file, sentences);
  fclose (file);
  return 0;
}

/**
 * Checks if the number of command line arguments is valid.
 * @param argc
 * @return If it is valid, return 0. Otherwise, return 1
 */
int check_num_args (int argc)
{
  if (argc != MAX_ARGS_NUM && argc != MIN_ARGS_NUM)
    {
      printf (USAGE_ERROR);
      return 1;
    }
  return 0;
}

/**
 * @param argc
 * @param argv 1) Seed
 *             2) Number of sentences to generate
 *             3) Path to file
 *             4) Optional - Number of words to read
 */
int main (int argc, char *argv[])
{
  if (check_num_args (argc) || get_input_and_run (argc, argv))
    {
      return EXIT_FAILURE;
    }
  return EXIT_SUCCESS;
}