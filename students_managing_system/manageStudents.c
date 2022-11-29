/* AUTHOR: MARIIA MAKARENKO
 * ID NUMBER: 342849676
 * EX NUMBER: 2
 * COURSE: 67315 PROGRAMMING WORKSHOP IN C & C++ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define NUM_OF_ARGS 2
#define ARG_COMMAND 1
#define TRUE 1
#define FALSE 0
#define BASE 10
#define BUFFER_SIZE 61
#define FIELD_SIZE 21
#define ERROR_USAGE "USAGE: manageStudents <best|bubble|quick>\n"
#define COMMAND_BEST_STUDENT "best"
#define MESSAGE_ENTER_NUM "Enter number of students. Then enter\n"
#define MESSAGE_ENTER_INFO "Enter student info. Then enter\n"
#define MESSAGE_BEST_STUDENT "best student info is: %ld,%d,%d\n"
#define SMALLEST_NUM_OF_STUDENTS 1
#define ID_NUM_DIGITS 10
#define ID_NOT_START_WITH '0'
#define WORST_GRADE 0
#define BEST_GRADE 100
#define SMALLEST_AGE 18
#define BIGGEST_AGE 120
#define ERROR_NUM_STUDENTS ("ERROR: Number of students must be a positive "\
"integer.\n")
#define ERROR_ID ("ERROR: Id must be an integer of 10 digits and can not "\
"start with 0.\n")
#define ERROR_GRADE "ERROR: Grade should be an integer between 0 and 100 "\
"(includes).\n"
#define ERROR_AGE ("ERROR: Age should be an integer between 18 and 120 "\
"(includes).\n")
#define COMMAND_BUBBLE "bubble"
#define COMMAND_QUICK "quick"

typedef struct Student {
/* This struct represents a student. Each student has an id that must be of
 * 10 digits and the first digit is not 0, a grade (from 0 to 100), age
 * (from 18 to 120).*/
  long int id;
  int grade, age;
} Student;

Student initialize_student (long int id, int grade, int age)
/* This function returns an object student of struct Student with the
 * specified id, grade, age. */
{
  Student student;
  student.id = id;
  student.grade = grade;
  student.age = age;
  return student;
}

int check_command_line_input (int num_of_args, char **args)
/* This function checks the command line input and if it is correct returns
 * TRUE, FALSE otherwise. */
{
  if ((num_of_args == NUM_OF_ARGS) && ((!strcmp (
      COMMAND_BEST_STUDENT, args[ARG_COMMAND])) || (!strcmp (
          COMMAND_BUBBLE, args[ARG_COMMAND])) || (!strcmp (
              COMMAND_QUICK, args[ARG_COMMAND]))))
    {
      return TRUE;
    }
  printf (ERROR_USAGE);
  return FALSE;
}

int check_if_num (const char str[])
/* This function receives a string and checks if a string is a number. If
 * yes, the function returns TRUE, FALSE otherwise. */
{
  for (int i = 0; str[i] != '\0'; i++)
    {
      if (!isdigit (str[i]))
        {
          return FALSE;
        }
    }
  return TRUE;
}

int check_num_students (char num_temp[], int *num_of_students)
/* This function checks if the user provided us with a correct number of
 * students. It also initializes the number of students directly through the
 * memory address provided. */
{
  if (!check_if_num (num_temp))
    {
      printf (ERROR_NUM_STUDENTS);
      return FALSE;
    }
  *num_of_students = (int) strtol (num_temp, NULL, BASE);
  if (*num_of_students < SMALLEST_NUM_OF_STUDENTS)
    {
      printf (ERROR_NUM_STUDENTS);
      return FALSE;
    }
  return TRUE;
}

int get_num_students ()
/* This function returns a number of students got from the user. */
{
  char buffer[BUFFER_SIZE], num_temp[BUFFER_SIZE];
  int num_of_students;
  do // while input is incorrect, ask for input again.
    {
      printf (MESSAGE_ENTER_NUM);
      if (fgets (buffer, BUFFER_SIZE, stdin) == NULL || sscanf (
          buffer, "%s\n", num_temp) == EOF)
        {
          return FALSE; // if either of the stdio functions crashes, FALSE.
        }
    }
  while (!check_num_students (num_temp, &num_of_students));
  return num_of_students;
}

int check_id (char id_temp[], long int *id)
/* This function returns TRUE if the id is valid (is an integer of 10 digits
 * and the first digit is not 0), FALSE otherwise. */
{
  if (id_temp[0] == ID_NOT_START_WITH || !check_if_num (id_temp) ||
      strlen (id_temp) != ID_NUM_DIGITS)
    { // checking if a string is a number and does not start with 0.
      printf (ERROR_ID);
      return FALSE;
    }
  *id = strtol (id_temp, NULL, BASE);
  return TRUE;
}

int check_grade (char grade_temp[], int *grade)
/* This function returns TRUE if a grade is valid (is an integer from 0 to
 * 100), FALSE otherwise. */
{
  if (!check_if_num (grade_temp))
    {
      printf (ERROR_GRADE);
      return FALSE;
    }
  *grade = (int) strtol (grade_temp, NULL, BASE);
  if (*grade > BEST_GRADE || *grade < WORST_GRADE)
    {
      printf (ERROR_GRADE);
      return FALSE;
    }
  return TRUE;
}

int check_age (char age_temp[], int *age)
/* This function returns TRUE if age is valid (is an integer from 18 to 120),
 * FALSE otherwise. */
{
  if (!check_if_num (age_temp))
    {
      printf (ERROR_AGE);
      return FALSE;
    }
  *age = (int) strtol (age_temp, NULL, BASE);
  if (*age > BIGGEST_AGE || *age < SMALLEST_AGE)
    {
      printf (ERROR_AGE);
      return FALSE;
    }
  return TRUE;
}

int check_input_data (char id_temp[], char grade_temp[], char age_temp[],
                      long int *id, int *grade, int *age)
/* This function returns TRUE if all the input (id, grade, age) is valid,
 * FALSE otherwise. It also initializes id, grade and age through the memory
 * address provided. */
{
  return (check_id (id_temp, id) && check_grade (grade_temp, grade) &&
  check_age (age_temp, age));
}

int get_one_student (Student *student)
/* This function returns one object of struct Student with data from the
 * user. It returns FALSE if there were errors in sscanf or fgets. */
{
  char buff[BUFFER_SIZE], id_temp[FIELD_SIZE], grade_temp[FIELD_SIZE],
  age_temp[FIELD_SIZE];
  long int id;
  int grade, age;
  do
    { // while input is incorrect, ask for input again.
      printf (MESSAGE_ENTER_INFO);
      if (fgets (buff, BUFFER_SIZE, stdin) == NULL || sscanf (
          buff, "%[^,],%[^,],%s\n", id_temp, grade_temp, age_temp) == EOF)
        {
          return FALSE; // if either of the stdio functions crashes, FALSE.
        }
    }
  while (!check_input_data (id_temp, grade_temp, age_temp, &id, &grade, &age));
  *student = initialize_student (id, grade, age);
  return TRUE;
}

Student *fill_students (Student *students, int num_of_students)
/* This function fills an array of structs students with num_of_students
 * elements. */
{
  for (int i = 1; i <= num_of_students; i++)
    {
      if (!get_one_student (students++))
        {
          return NULL;
        }
    }
  return students;
}

void print_students_sorted (Student *start, Student *end)
/* This function prints the sorted array of students of the way <id>,
 * <grade>,<age>. */
{
  for (int i = 0; (start + i) != end; i++)
    {
      printf ("%ld,%d,%d\n", (start + i)->id, (start + i)->grade, (start + i)->
          age);
    }
}

void best_student (Student *start, Student *end)
/* This function takes a pointer to the start and a pointer to the end of
 * the array of students (objects of struct Student) and prints the data of
 * the best student (with the biggest grade/age relation). */
{
  Student *best_student = start;
  for (int i = 0; (start + i) <= (end - 1); i++)
    {
      if ((float) (start + i)->grade / (float) (start + i)->age >
          (float) best_student->grade / (float) best_student->age)
        {
          best_student = (start + i);
        }
    }
  printf (MESSAGE_BEST_STUDENT, best_student->id, best_student->grade,
          best_student->age);
}

void swap (Student *ptr1, Student *ptr2)
/* This function swaps two elements in the array. */
{
  Student temp = *ptr1;
  *ptr1 = *ptr2;
  *ptr2 = temp;
}

int check_swap_bubble (Student *student, int swapped)
/* This function checks if the grade of the current student is bigger than
 * the one of the next. If yes, they are swapped and the function returns
 * TRUE. FALSE otherwise. */
{
  if (student->grade > (student + 1)->grade)
    {
      swap (student, student + 1);
      return TRUE;
    }
  return swapped;
}

void bubble_sort (Student *start, Student *end)
/* This function sorts an array of students by grade using the bubble sort
 * algorithm and prints it afterwards. */
{
  for (int i = 0; (start + i) <= (end - 2); i++)
    {
      int swapped = FALSE;
      for (int j = 0; (start + j) <= (end - i - 2); j++)
        {
          swapped = check_swap_bubble (start + j, swapped);
        }
      if (!swapped)
        {
          break;
        }
    }
  print_students_sorted (start, end);
}

Student *partition (Student *start, Student *end)
/* This function takes a pivot as an end-1 index element and puts it into
 * the correct position in the sorted array by putting all the smaller
 * elements to the left, all the bigger to the right. */
{
  int i = -1;
  for (int j = 0; (start + j) <= (end - 2); j++)
    {
      if ((start + j)->age <= (end - 1)->age)
        {
          i++;
          swap (start + i, start + j);
        }
    }
  swap (start + i + 1, end - 1);
  return (start + i + 1);
}

void quick_sort (Student *start, Student *end)
/* This function sorts an array of students by age using the quick sort
 * algorithm. */
{
  if (start < end - 1)
    {
      Student *pivot = partition (start, end);
      quick_sort (start, pivot);
      quick_sort (pivot + 1, end);
    }
}

int allocate_and_fill (int num_of_students, Student **start, Student **end)
/* This function creates an array of students (allocates memory for it) and
 * fills it with objects of struct Student. The function modifies the start
 * pointer to point at the start of the array and the end to point after the
 * last element in the array. If there are any errors, the function returns
 * FALSE. TRUE otherwise. */
{
  *start = (Student *) malloc (num_of_students * sizeof (Student));
  if (*start == NULL)
    {
      return FALSE;
    }
  *end = fill_students (*start, num_of_students);
  if (*end == NULL)
    {
      free (*start);
      *start = NULL;
      return FALSE;
    }
  return TRUE;
}

void run_commands_and_free (char command[], Student *start, Student *end)
/* This function runs the selected by user command and after that frees the
 * allocated memory. */
{
  if (!strcmp (command, COMMAND_BEST_STUDENT)) // manageStudents best.
    {
      best_student (start, end); // print the data of the best student.
    }
  else if (!strcmp (command, COMMAND_BUBBLE)) // sort students by grade.
    {
      bubble_sort (start, end); // the results are printed in the function.
    }
  else // sort students by age.
    {
      quick_sort (start, end);
      print_students_sorted (start, end); // print the results after the sort.
    }
  free (start); // free the allocated memory.
  start = NULL;
  end = NULL;
}

int main (int argc, char *argv[])
{
  if (!check_command_line_input (argc, argv)) // CHECKING COMMAND LINE ARGS
    {
      return EXIT_FAILURE; // if cla are not correct, exit with code 1.
    }
  int num_of_students = get_num_students (); // GETTING MEMORY SIZE
  Student *start = NULL; // CREATING A POINTER TO THE START OF THE ARRAY
  Student *end = NULL; // CREATING A POINTER TO THE END OF THE ARRAY
  if (!num_of_students || !allocate_and_fill (num_of_students, &start, &end))
    { // ALLOCATING THE MEMORY AND FILLING THE ARRAY
      return EXIT_FAILURE; // if input error or alloc error, exit with code 1.
    }
  run_commands_and_free (argv[ARG_COMMAND], start, end);
  return EXIT_SUCCESS;
}
