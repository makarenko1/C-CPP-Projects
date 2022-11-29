#ifndef _VL_STRING_H_
#define _VL_STRING_H_

#include "vl_vector.h"
#include <cstring>
#define DEFAULT_CAPACITY 16UL
#define TERMINATOR '\0'
#define ERROR_INVALID_DATA "Error: Invalid data!\n"

template <size_t StaticCapacity = DEFAULT_CAPACITY>
class vl_string : public vl_vector<char, StaticCapacity>
{
 public:
  /**
   * Default Constructor. Initializes an empty vl_string.
   */
  vl_string () : vl_vector<char, StaticCapacity> (1, TERMINATOR) {}

  /**
   * Copy Constructor. Initializes a vl_string from another vl_string.
   * @param other another vl_string to initialize from.
   */
  vl_string (const vl_string &other) : vl_vector<char, StaticCapacity> (other)
  {}

  /**
   * Implicit Constructor. Initializes a vl_string from the given string
   * (const char *).
   * @param string the given string to initialize with.
   */
  vl_string (const char *string) : vl_string ()
  {
    if (string == nullptr)
      {throw std::invalid_argument (ERROR_INVALID_DATA);}
    this->insert (this->cend () - 1, string, string + strlen (string));
  }

  /**
   * Returns the size of the vl_string (not including the terminator
   * character).
   * @return the value of the size field of the vl_vector minus one.
   */
  size_t size () const override
  {return (this->_size - 1);}

  /**
   * Adds the given character to the end of the vl_string.
   * @param value the given character to add at the end of the vl_string.
   */
  void push_back (const char value) override
  {
    this->check_expand (1);
    *(this->end () - 1) = value; // overwrite the terminator character.
    this->_size++;
    *(this->end () - 1) = TERMINATOR;
  }

  /**
   * Erases one character from the back of the vl_string. If the vl-string is
   * empty (it has only the terminator character), then the function
   * terminates.
   */
  void pop_back () override
  {
    if (this->empty ())
      {return;}
    this->check_shrink (1);
    this->_size--;
    *(this->end () - 1) = TERMINATOR;
  }

  /**
   * Erases all the data from the vl_string. Puts the terminator character
   * at the end of the cleared vl_string.
   */
  void clear () override
  {
    this->_size = 1;
    this->_capacity = StaticCapacity;
    delete[] this->_dynamic_vector;
    this->_dynamic_vector = nullptr;
    *(this->end () - 1) = TERMINATOR;
  }

  /**
   * Checks if a vl_string contains the given string (const char *).
   * @param other the given string to check if it is a substring in the
   * vl_string.
   * @return true if it is a substring, false otherwise.
   */
  bool contains (const char *other)
  {
    if (other == nullptr)
      {throw std::invalid_argument (ERROR_INVALID_DATA);}
    if (strlen (other) == 0)
      {return true;}
    else if (strlen (other) > this->size ())
      {return false;}
    size_t counter_equal = 0;
    for (size_t i = 0; i < this->size (); i++)
      {
        if (counter_equal == strlen (other))
          {return true;}
        if ((*this)[i] == other[counter_equal])
          {counter_equal++;}
        else
          {counter_equal = 0;}
      }
    return (counter_equal == strlen (other));
  }

  /**
   * Expands the vl_string with the given vl_string. Supports chaining.
   * @param other the given vl_string to expand with.
   * @return the reference to the current vl_string.
   */
  vl_string<StaticCapacity> &operator+= (const vl_string<StaticCapacity>
      &other)
  {
    this->insert (this->cend () - 1, other.begin (), other.begin () +
    other.size ());
    return *this;
  }

  /**
   * Expands the vl_string with the given string (const char *). Supports
   * chaining.
   * @param other the given string to expand with.
   * @return the reference to the current vl_string.
   */
  vl_string<StaticCapacity> &operator+= (const char *other)
  {
    if (other == nullptr)
      {throw std::invalid_argument (ERROR_INVALID_DATA);}
    this->insert (this->cend () - 1, other, other + strlen (other));
    return *this;
  }

  /**
   * Expands the vl_string with the given character. Supports chaining.
   * @param value the given character to expand with.
   * @return the reference to the current vl_string.
   */
  vl_string<StaticCapacity> &operator += (const char value)
  {
    this->push_back (value);
    return *this;
  }

  /**
   * Adds the vl_string lhs and the vl_string rhs. Supports chaining.
   * @param lhs the first vl_string to add.
   * @param rhs the second vl_string to add.
   * @return the obtained vl_string (the result of the sum).
   */
  friend vl_string<StaticCapacity> operator+ (const vl_string<StaticCapacity>
      &lhs, const vl_string<StaticCapacity> &rhs)
  {
    vl_string string (lhs);
    string += rhs;
    return string;
  }

  /**
   * Adds the vl_string lhs and the string (const char *) rhs. Supports
   * chaining.
   * @param lhs the vl_string to add.
   * @param rhs the string (const char *) to add.
   * @return the obtained vl_string (the result of the sum).
   */
  friend vl_string<StaticCapacity> operator+ (const vl_string<StaticCapacity>
      &lhs, const char *rhs)
  {
    if (rhs == nullptr)
      {throw std::invalid_argument (ERROR_INVALID_DATA);}
    vl_string string (lhs);
    string += rhs;
    return string;
  }

  /**
   * Adds the vl_string lhs and the character rhs. Supports chaining.
   * @param lhs the vl_string to add.
   * @param rhs the character to add.
   * @return the obtained vl_string (the result of the sum).
   */
  friend vl_string<StaticCapacity> operator+ (const vl_string<StaticCapacity>
  &lhs, const char rhs)
  {
    vl_string string (lhs);
    string += rhs;
    return string;
  }

  /**
   * Implicit Casting Operator to const char *.
   * @return the constant pointer to the beginning of the data of the
   * vl_string.
   */
  operator const char *() const
  {return this->data ();}
};

#endif //_VL_STRING_H_
