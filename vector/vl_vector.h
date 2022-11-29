#ifndef _VL_VECTOR_H_
#define _VL_VECTOR_H_

#include <iostream>
#include <iterator>
#include <algorithm>
#define EMPTY_SIZE 0
#define DEFAULT_CAPACITY 16UL
#define ERROR_OUT_OF_RANGE "Error: The given value is out of range!\n"

/**
 * Returns a new capacity of the vl_vector.
 * @param size the current size of the vl_vector.
 * @param num_add the number of elements to add to the vl_vector.
 * @param static_capacity the static capacity of the vl_vector.
 * @return a new capacity calculated by the formula.
 */
size_t cap_c (size_t size, size_t num_add, size_t static_capacity)
{
  if (size + num_add <= static_capacity)
    {return static_capacity;}
  return ((size_t) ((float) (3 * (size + num_add))) /
  ((float) 2));
}

template <class T, size_t StaticCapacity = DEFAULT_CAPACITY>
class vl_vector {
  // Help functions.
 private:
  /**
   * Initializes the dynamic part of the vl_vector.
   */
  void create_dynamic_vector ()
  {
    _dynamic_vector = nullptr;
    if (_capacity > StaticCapacity)
      {_dynamic_vector = new T[_capacity] ();}
  }

  // Functions that are used also for vl_string.
 protected:
  /**
   * Expands the dynamically-allocated part of the vl_vector if needed.
   * @param num_add the number of elements to add to the vl_vector.
   */
  void check_expand (size_t num_add)
  {
    if (_capacity >= (_size + num_add))
      {return;}
    size_t new_capacity = cap_c (_size, num_add, StaticCapacity);
    auto *new_dynamic_vector = new T [new_capacity];
    std::copy (begin (), end (), new_dynamic_vector);
    _capacity = new_capacity;
    delete[] _dynamic_vector;
    _dynamic_vector = new_dynamic_vector;
  }

  /**
   * Shrinks from the dynamically-allocated part to the static part of the
   * vl_vector if needed.
   * @param num_erase the number of elements to erase from the vl_vector.
   */
  void check_shrink (size_t num_erase)
  {
    if (_capacity == StaticCapacity || (_size - num_erase) > StaticCapacity)
      {return;}
    std::copy (_dynamic_vector, _dynamic_vector + _size - num_erase,
               _static_vector);
    _capacity = StaticCapacity;
    delete[] _dynamic_vector;
    _dynamic_vector = nullptr;
  }

 public:
  /**
   * Typedefs of iterator and const_iterator (pointer iterators).
   */
  typedef T* iterator;
  typedef const T* const_iterator;

  /**
   * Typedefs of reverse_iterator and const_reverse_iterator.
   */
  typedef std::reverse_iterator<iterator> reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

  /**
   * Default Constructor. Initializes an empty vl_vector.
   */
  vl_vector () : _size (EMPTY_SIZE), _capacity (StaticCapacity),
  _dynamic_vector (nullptr) {}

  /**
   * Copy Constructor. Initializes a vl_vector from another vl_vector.
   * @param other another vl_vector to initialize from.
   */
  vl_vector (const vl_vector<T, StaticCapacity> &other) :
  _size (other._size), _capacity (other._capacity)
  {
    create_dynamic_vector ();
    std::copy (other.begin (), other.end (), begin ());
  }

  /**
   * Sequence based Constructor.
   * @tparam InputIterator an InputIterator to vl_vector of type T.
   * @param first an InputIterator to the first element in the other vl_vector.
   * @param last an InputIterator to the after-last element in the other
   * vl_vector.
   */
  template <class InputIterator>
  vl_vector (InputIterator first, InputIterator last)
  {
    _size = (size_t) std::distance (first, last);
    _capacity = cap_c (EMPTY_SIZE, _size, StaticCapacity);
    create_dynamic_vector ();
    std::copy (first, last, begin ());
  }

  /**
   * Single-value initialized Constructor. Initializes the vl_vector with
   * count elements of v.
   * @param count the number of elements v.
   * @param v an element of type T.
   */
  vl_vector (size_t count, const T v)
  {
    _size = count;
    _capacity = cap_c (EMPTY_SIZE, _size, StaticCapacity);
    create_dynamic_vector ();
    std::fill (begin (), end (), v);
  }

  /**
   * Destructor.
   */
  virtual ~vl_vector ()
  {
    if (_capacity > StaticCapacity)
      {
        delete[] _dynamic_vector;
        _dynamic_vector = nullptr;
      }
  }

  /**
   * Returns the current number of elements in the vl_vector.
   * @return the value of the size field of the vl_vector.
   */
  virtual size_t size () const
  {return _size;}

  /**
   * Returns the maximal possible number of elements in the vl_vector (its
   * capacity).
   * @return the value of the capacity field of the vl_vector.
   */
  size_t capacity () const
  {return _capacity;}

  /**
   * Checks if the vl_vector is empty.
   * @return true if the value of the field size of the vl_vector is equal
   * to zero, false otherwise.
   */
  bool empty () const
  {return size () == EMPTY_SIZE;}

  /**
   * Returns the element at the given index in the vl_vector. If the given
   * index is out of range, the function throws an exception.
   * @param index the given index.
   * @return a reference to the element of the type T at the given index in
   * the vl_vector.
   */
  T &at (size_t index)
  {
    if (index >= _size)
      {throw std::out_of_range (ERROR_OUT_OF_RANGE);}
    return *(data () + index);
  }

  /**
   * A constant version of the at function.
   * @param index the given index.
   * @return the element of the type T at the given index in the vl_vector.
   */
  T at (size_t index) const
  {
    if (index >= _size)
      {throw std::out_of_range (ERROR_OUT_OF_RANGE);}
    return *(data () + index);
  }

  /**
   * Adds the given value to the end of the vl_vector.
   * @param value the value of the type T to add at the end of the vl_vector.
   */
  virtual void push_back (const T value)
  {
    check_expand (1);
    *(end ()) = value;
    _size++;
  }

  /**
   * Inserts the given value before the given position (on the left).
   * @param position the given position to insert before.
   * @param value the value of the type T to insert before the position.
   * @return an iterator that points on the inserted value.
   */
  iterator insert (const_iterator position, const T value)
  {
    auto saved_distance = (size_t) std::distance (cbegin (), position);
    check_expand (1);
    iterator updated_position = (begin () + saved_distance);
    for (auto it = (end () - 1); it != (updated_position - 1); it--)
      {*(it + 1) = *it;}
    *updated_position = value;
    _size++;
    return updated_position;
  }

  /**
   * Inserts the elements [first, last) before the given position (on the
   * left).
   * @tparam InputIterator an InputIterator to vl_vector of type T.
   * @param position the given position to insert before.
   * @param first an iterator to the first element to insert.
   * @param last an iterator to the last element to insert.
   * @return an iterator that points on the first inserted value.
   */
  template <class InputIterator>
  iterator insert (const_iterator position, InputIterator first,
                          InputIterator last)
  {
    auto num_add = (size_t) std::distance (first, last);
    auto saved_distance = (size_t) std::distance (cbegin (), position);
    check_expand (num_add);
    iterator updated_position = (begin () + saved_distance);
    for (auto it = (end () - 1); it != (updated_position - 1); it--)
      {*(it + num_add) = *it;}
    std::copy (first, last, updated_position);
    _size+=num_add;
    return updated_position;
  }

  /**
   * Erases one element from the back of the vl_vector. If the vl-vector is
   * empty, then the function terminates.
   */
  virtual void pop_back ()
  {
    if (empty ())
      {return;}
    check_shrink (1);
    _size--;
  }

  /**
   * Erases the element at the given position.
   * @param position the given position to erase from.
   * @return an iterator that points to the element on the right from the
   * erased element.
   */
  iterator erase (const_iterator position)
  {
    auto saved_distance = (size_t) std::distance (cbegin (), position);
    for (auto it = (begin () + saved_distance); it != (end () - 1); it++)
      {*it = *(it + 1);}
    check_shrink (1);
    _size--;
    return (begin () + saved_distance);
  }

  /**
   * Erases the elements [first, last).
   * @param first an iterator to the first element to erase.
   * @param last an iterator to the last element to erase.
   * @return an iterator that points to the element on the right from the
   * erased elements.
   */
  iterator erase (const_iterator first, const_iterator last)
  {
    auto num_erase = (size_t) std::distance (first, last);
    auto saved_distance = (size_t) std::distance (cbegin (), first);
    for (auto it = (begin () + saved_distance); it != (end () - num_erase);
    it++)
      {*it = *(it + num_erase);}
    check_shrink (num_erase);
    _size-=num_erase;
    return (begin () + saved_distance);
  }

  /**
   * Erases all the data from the vl_vector.
   */
  virtual void clear ()
  {
    _size = EMPTY_SIZE;
    _capacity = StaticCapacity;
    delete[] _dynamic_vector;
    _dynamic_vector = nullptr;
  }

  /**
   * Returns a pointer to the variable that holds the current data of the
   * vl_vector.
   * @return a pointer to the static data or to the dynamic data depending
   * on the current state of the vl_vector.
   */
  T* data ()
  {
    if (_capacity > StaticCapacity)
      {return _dynamic_vector;}
    return _static_vector;
  }

  /**
   * A constant version of the data function.
   * @return a constant pointer to the static data or to the dynamic data
   * depending on the current state of the vl_vector.
   */
  const T* data () const
  {
    if (_capacity > StaticCapacity)
      {return _dynamic_vector;}
    return _static_vector;
  }

  /**
   * Checks if the given value is in the vl_vector.
   * @param value the given value to check if it is in the vl_vector.
   * @return true if the given value is in the vl_vector, false otherwise.
   */
  bool contains (const T value) const
  {
    return (std::find (begin (), end (), value) != end ());
  }

  /**
   * Returns an iterator to the beginning of the vl_vector.
   * @return a pointer to the beginning of the current data array.
   */
  iterator begin () {return data ();}

  /**
   * Returns an iterator to the past-end of the vl_vector.
   * @return a pointer to the past-end of the current data array.
   */
  iterator end () {return data () + _size;}

  /**
   * Returns a const_iterator to the beginning of the vl_vector.
   * @return a constant pointer to the beginning of the current data array.
   */
  const_iterator begin () const {return data ();}

  /**
   * Returns a const_iterator to the past-end of the vl_vector.
   * @return a constant pointer to the past-end of the current data array.
   */
  const_iterator end ()  const {return data () + _size;}

  /**
   * Returns a const_iterator to the beginning of the vl_vector.
   * @return same as the const version of begin.
   */
  const_iterator cbegin () const {return data ();}

  /**
   * Returns a const_iterator to the past-end of the vl_vector.
   * @return same as the const version of end.
   */
  const_iterator cend () const {return data () + _size;}

  /**
   * Returns a reverse_iterator to the past-end of the vl_vector.
   * @return a reverse iterator made from the end iterator.
   */
  reverse_iterator rbegin () {return reverse_iterator (end ());}

  /**
   * Returns a reverse_iterator to the beginning of the vl_vector.
   * @return a reverse iterator made from the begin iterator.
   */
  reverse_iterator rend () {return reverse_iterator (begin ());}

  /**
   * Returns a const_reverse_iterator to the past-end of the vl_vector.
   * @return a constant reverse iterator made from the end iterator.
   */
  const_reverse_iterator rbegin () const {return const_reverse_iterator (
      end ());}

  /**
   * Returns a const_reverse_iterator to the beginning of the vl_vector.
   * @return a constant reverse iterator made from the begin iterator.
   */
  const_reverse_iterator rend ()  const {return const_reverse_iterator (
      begin ());}

  /**
   * Returns a const_reverse_iterator to the past-end of the vl_vector.
   * @return a constant reverse iterator made from the cend iterator.
   */
  const_reverse_iterator crbegin () const {return const_reverse_iterator (
        cend ());}

  /**
   * Returns a const_reverse_iterator to the beginning of the vl_vector.
   * @return a constant reverse iterator made from the cbegin iterator.
   */
  const_reverse_iterator crend () const {return const_reverse_iterator (
        cbegin ());}

 /**
 * Assigns to the vl_vector.
 * @param other another vl_vector to assign from.
 * @return a reference to the current vl_vector that was changed.
 */
  vl_vector &operator= (const vl_vector<T, StaticCapacity> &other)
  {
    if (this != &other)
      {
        _size = other._size;
        _capacity = other._capacity;
        delete[] _dynamic_vector;
        create_dynamic_vector ();
        std::copy (other.begin (), other.end (), begin ());
      }
    return *this;
  }

  /**
   * Returns the element at the given index in the vl_vector. Doesn't throw
   * an exception.
   * @param index the given index.
   * @return a reference to the element of the type T at the given index in
   * the vl_vector.
   */
  T &operator [] (size_t index)
  {return *(data () + index);}

  /**
   * A constant version of the operator [].
   * @param index the given index.
   * @return the element of the type T at the given index in the vl_vector.
   */
  T operator [] (size_t index) const
  {return *(data () + index);}

  /**
   * Checks if two vl_vectors are equal.
   * @param lhs the vl_vector on the left side of the comparison.
   * @param rhs the vl_vector on the right side of the comparison.
   * @return true if they are equal (their size is equal, all the values and
   * the order of the values), false otherwise.
   */
  friend bool operator == (const vl_vector<T, StaticCapacity> &lhs, const
  vl_vector<T, StaticCapacity> &rhs)
  {
    if (lhs._size != rhs._size)
      {return false;}
    for (size_t i = 0; i < lhs._size; i++)
      {
        if (*(lhs.data () + i) != *(rhs.data () + i))
          {return false;}
      }
    return true;
  }

  /**
   * Checks if two vl_vectors are unequal.
   * @param lhs the vl_vector on the left side of the comparison.
   * @param rhs the vl_vector on the right side of the comparison.
   * @return true if they are unequal, false otherwise.
   */
  friend bool operator != (const vl_vector<T, StaticCapacity> &lhs, const
  vl_vector<T, StaticCapacity> &rhs)
  {return !(lhs == rhs);}

 protected:
  size_t _size; // real number of elements in the vector.
  size_t _capacity; // maximal number of elements in the vector.
  T _static_vector[StaticCapacity]; // static data of the vector.
  T *_dynamic_vector; // dynamic data of the vector.
};

#endif //_VL_VECTOR_H_
