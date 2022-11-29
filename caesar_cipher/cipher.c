/* MARIIA MAKARENKO - 342849676
 * Ex-1 - PROGRAMMING WORKSHOP IN C & C++ - 67315
 * A SIMPLE CIPHER PROGRAM THAT DOES THE CAESAR CIPHER ON FILES. */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define TRUE 1
#define FALSE 0
#define ARG1 1
#define ARG2 2
#define ARG3 3
#define ARG4 4
#define LOWER_DOWN_BORDER 97
#define LOWER_HIGH_BORDER 122
#define UPPER_DOWN_BORDER 65
#define UPPER_HIGH_BORDER 90
#define ALPHABET_LENGTH 26
#define DECIMAL 10
#define DECODE "decode"
#define ENCODE "encode"
#define CHECK "check"
#define NUM_OF_ARGS_ENC_DEC 5
#define NUM_OF_ARGS_CHECK 4
#define COMMAND_ERROR "The given command is invalid\n"
#define ARGS_ERROR_DEC_ENC ("Usage: cipher <encode|decode> <k> <source path "\
"file> <output path file>\n")
#define ARGS_ERROR_CHECK ("Usage: cipher <check> <source path file> <output "\
"path file>\n")
#define FILE_ERROR "The given file is invalid\n"
#define INVALID_K_MESSAGE "Invalid encrypting\n"
#define VALID_K_MESSAGE "Valid encrypting with k = %d\n"

int check_sign_of_k (char command[], int k)
/* This function changes the sign of k if the command is decode. Otherwise,
 * it returns k. */
{
  if (!strcmp (command, DECODE))
    {
      return -k;
    }
  return k;
}

int add_key_to_letter (int letter, int k)
/* This function adds k to the letter and so shifts it k places in the ascii
 * table. */
{
  return letter + k;
}

int treat_over_boards (int letter, int low_border, int high_border)
/* While the letter is out of borders after adding k to it, start from the
 * opposite border and add the difference between the previous border and
 * the letter. */
{
  while (letter > high_border)
    {
      letter = low_border + (letter - high_border) - 1;
    }
  while (letter < low_border)
    {
      letter = high_border - (low_border - letter) + 1;
    }
  return letter;
}

int check_over_borders (int letter, int flag_upper)
/* Check if the letter is out of borders for big letters (if it was big) or
 * small letters (if it was small). Change it with the treat_over_boards
 * function if yes. Return it either changes or unchanged. */
{
  if (flag_upper && (letter > UPPER_HIGH_BORDER || letter < UPPER_DOWN_BORDER))
    {
      return treat_over_boards (letter, UPPER_DOWN_BORDER, UPPER_HIGH_BORDER);
    }
  else if (!flag_upper && (letter > LOWER_HIGH_BORDER || letter <
  LOWER_DOWN_BORDER))
    {
      return treat_over_boards (letter, LOWER_DOWN_BORDER, LOWER_HIGH_BORDER);
    }
  return letter;
}

int cipher (int character, int k)
/* If the character is in alphabet, cipher it with k (encode or decode). Return
 * the character either changed or unchanged. */
{
  if (isalpha (character))
    {
      int flag = isupper (character); // remembering if it was upper or lower.
      character = add_key_to_letter (character, k); // adding the key.
      character = check_over_borders (character, flag); // cycling the cipher.
    }
  return character;
}

void decode_and_encode (char command[], int k, FILE *input_file, FILE
*output_file)
/* This function does encoding and decoding. It gets a character from the
 * input file and pushes it through the cipher function. Then it writes the
 * character into the output file. */
{
  int character = fgetc (input_file); // receiving one character at a time.
  k = check_sign_of_k (command, k); // change sign of k depending by command.
  while (character != EOF) // if out of file, exit the loop.
    {
      character = cipher (character, k); // cipher if in alphabet.
      fputc (character, output_file); // putting into the output.
      character = fgetc (input_file); // getting next character.
    }
}

int check_negative_difference (int difference)
/* This function checks if the difference is negative and reflects it by
 * the alphabet length. The function returns the difference. */
{
  if (difference < 0) // if there was an overflow in the input.
    {
      difference = ALPHABET_LENGTH + difference;
    }
  return difference;
}

int check_k_borders (int k)
/* If k is out of borders, return FALSE. TRUE otherwise. */
{
  if ((k < 0) || (k > ALPHABET_LENGTH - 1))
    {
      return FALSE;
    }
  return TRUE;
}

int check_k_difference (int character1, int character2, int flag, int
difference, int k)
/* If both characters are in the alphabet and flag is TRUE, if difference
 * between the characters is not k, return FALSE. If both characters are not
 * in the alphabet and difference is not 0, return FALSE. Return TRUE
 * otherwise. */
{
  if ((isalpha (character1) && isalpha (character2) && flag &&
       difference != k) || (!isalpha (character1) && !isalpha
      (character2) && difference != 0))
    {
      return FALSE;
    }
  return TRUE;
}

int check_opposites_alphabet (int character1, int character2)
/* If one character is in the alphabet, return FALSE. TRUE otherwise. */
{
  if ((!isalpha (character1) && isalpha (character2)) ||
      (isalpha (character1) && !isalpha (character2)))
    {
      return FALSE;
    }
  return TRUE;
}

int check_exit (int character1, int character2, int difference, int k, int
flag)
/* If the exit conditions are met, return FALSE. TRUE otherwise. */
{
  if (!check_k_borders (k) || !check_k_difference (character1, character2,
                                                   flag, difference, k) ||
      !check_opposites_alphabet (character1, character2))
    {
      printf (INVALID_K_MESSAGE);
      return FALSE;
    }
  return TRUE;
}

void check (FILE *input_file, FILE *output_file)
/* This function prints the k value given the input and the output files if
 * the encryption is valid. It prints that the encryption is not valid
 * otherwise. */
{
  int character1, character2, difference;
  int flag = 0; // flag to determine the first alphabetic letter.
  int k = 0; // the return value, cipher's key.
  do
    {
      character1 = fgetc (input_file);
      character2 = fgetc (output_file);
      difference = check_negative_difference (character2 - character1);
      if (!check_exit (character1, character2, difference, k, flag))
        {
          return; // exit if doesn't meet the conditions.
        }
      if (isalpha (character1) && isalpha (character2) && !flag)
        {
          flag = TRUE; // if the first alphabet letter, change k to non-zero.
          k = difference;
        }
    }
  while (character1 != EOF && character2 != EOF);
  printf (VALID_K_MESSAGE, k);
}

int check_command (char *argv[], int num_of_args)
/* This function checks if the arguments are not empty and the command is
 * either decode, or encode, or check. If yes, returns TRUE. If no, FALSE. */
{
  if (num_of_args == 1 || ((strcmp (argv[ARG1], DECODE) != 0 &&
                            strcmp (argv[ARG1], ENCODE) != 0
                            && strcmp (argv[ARG1], CHECK) != 0)))
    {
      fprintf (stderr, COMMAND_ERROR);
      return FALSE;
    }
  return TRUE;
}

int check_num_of_args (char command[], int num_of_args)
/* This function checks if the number of args is correct for the command. */
{
  if ((!strcmp (command, DECODE) || !strcmp (command, ENCODE)) &&
      num_of_args != NUM_OF_ARGS_ENC_DEC)
    {
      fprintf (stderr, ARGS_ERROR_DEC_ENC);
      return FALSE;
    }
  else if (!strcmp (command, CHECK) && num_of_args != NUM_OF_ARGS_CHECK)
    {
      fprintf (stderr, ARGS_ERROR_CHECK);
      return FALSE;
    }
  return TRUE;
}

void close_files (FILE *first_file, FILE *second_file)
/* This function closes the two files that were given to it. */
{
  fclose (first_file);
  fclose (second_file);
}

int check_file_validity (char *argv[])
/* This function opens the file in the suitable for the commands modes and
 * checks if they opened. If yes, it closes them and returns TRUE. FALSE
 * otherwise. */
{
  FILE *input_file;
  FILE *output_file;
  if (!strcmp (argv[ARG1], DECODE)
      || !strcmp (argv[ARG1], ENCODE)) // open if dec/enc.
    {
      input_file = fopen (argv[ARG3], "r");
      output_file = fopen (argv[ARG4], "w");
    }
  else // open if check.
    {
      input_file = fopen (argv[ARG2], "r");
      output_file = fopen (argv[ARG3], "r");
    }
  if (input_file == NULL || output_file == NULL) // if null, didn't open.
    {
      fprintf (stderr, FILE_ERROR); // do not close files 'cause they are null.
      return FALSE;
    }
  close_files (input_file, output_file); // close files.
  return TRUE;
}

int check_input (int argc, char *argv[])
/* Check the command is correct, check the number of args is right, check if
 * the files open. If all is right, returns TRUE, FALSE otherwise. */
{
  if (!check_command (argv, argc) || !check_num_of_args (argv[ARG1], argc) ||
      !check_file_validity (argv))
    {
      return FALSE;
    }
  return TRUE;
}

int main (int argc, char *argv[])
/* The main function of the program. Receives input from the command line.
 * There are three main cases: if the input is incorrect the program
 * finishes, if the command (first argument) is either encode or decode, if
 * the command is check. If the input is correct and the command is encode
 * or decode, there is a shift by k in ascii. If it is check, we find the k
 * by the data given if it is possible. EXIT_SUCCESS if the program finishes
 * successfully. */
{
  if (!check_input (argc, argv)) // check the input.
    {
      return EXIT_FAILURE; // if not correct, exit with error.
    }
  else if (!strcmp (argv[ARG1], DECODE) || !strcmp (argv[ARG1], ENCODE))
    {
      FILE *input_file = fopen (argv[ARG3], "r");
      FILE *output_file = fopen (argv[ARG4], "w");
      decode_and_encode (argv[ARG1], (int) strtol (argv[ARG2], NULL, DECIMAL),
                         input_file, output_file);
      close_files (input_file, output_file); // close all files.
    }
  else if (!strcmp (argv[ARG1], CHECK)) // check.
    {
      FILE *input_file = fopen (argv[ARG2], "r");
      FILE *output_file = fopen (argv[ARG3], "r");
      check (input_file, output_file);
      close_files (input_file, output_file); // close all files.
    }
  return EXIT_SUCCESS; // if all is right, exit without error.
}
