#ifndef _HASHMAP_HPP_
#define _HASHMAP_HPP_

#include <vector>
#include <stdexcept>
#define INITIAL_CAPACITY 16
#define MINIMAL_CAPACITY 1
#define LOWER_LOAD_FACTOR 0.25
#define UPPER_LOAD_FACTOR 0.75
#define GROWTH_FACTOR 2
#define INVALID_KEYS_VALUES_ERROR "Error: Keys and Values don't match in size!"
#define KEY_ERROR "Error: Key not in hash map!"

template <typename KeyT, typename ValueT>
class HashMap
{
 public:
  typedef std::pair<KeyT, ValueT> PairT;
  typedef std::vector<PairT> Buckets;
  typedef typename Buckets::iterator IterT;

  HashMap();

  HashMap(const std::vector<KeyT> &keys, const std::vector<ValueT> &values);

  HashMap(const HashMap<KeyT, ValueT>  &other);

  virtual ~HashMap();

  HashMap<KeyT, ValueT>& operator=(const HashMap<KeyT, ValueT> &other);

  class ConstIterator;
  friend class ConstIterator;

  bool insert(const KeyT &key, const ValueT &value);

  virtual bool erase(const KeyT &key);

  void clear();

  bool contains_key(const KeyT &key) const
  { return get_iterator_position_on_bucket (key) != _null_iter; }

  const ValueT & at(const KeyT &key) const;

  ValueT & at(const KeyT &key);

  const ValueT operator[](const KeyT &key) const;

  ValueT& operator[](const KeyT &key);

  bool operator==(const HashMap<KeyT, ValueT>& other) const;

  bool operator!=(const HashMap<KeyT, ValueT>& other) const
  { return !operator==(other);}

  int size() const
  { return _size;}

  int capacity() const
  { return _capacity;}

  bool empty() const
  { return _size == 0;}

  int bucket_index(const KeyT &key) const;

  int bucket_size(const KeyT &key) const
  { return _table[bucket_index(key)].size();}

  double get_load_factor() const
  { return (double) _size / (double) _capacity;}

 protected:
  Buckets *_table;
  int _size, _capacity;
  Buckets * _empty_table = new Buckets();
  IterT _null_iter = _empty_table->end();
  // IterT can't be compared to regular null_ptr according to CLion

  int hash(const KeyT& key, int new_cap) const
  { return std::hash<KeyT>{}(key) & (new_cap - 1);}

  int hash(const KeyT &key) const
  {  return hash(key, _capacity); }

  IterT get_iterator_position_on_bucket(KeyT key) const // O(bucket size) time
  {
    Buckets& cur_bucket = _table[hash(key)];
    for (IterT it = cur_bucket.begin(); it != cur_bucket.end(); it++)
    {
      if (it->first == key)
      {
        return it;
      }
    }
    return _null_iter;
  }

  void rehash(int new_cap)
  {
    auto *new_hashtable = new Buckets[new_cap];
    for (const_iterator it = cbegin() ; it != cend() ; ++it)
    {
      new_hashtable[hash(it->first, new_cap)].push_back(PairT(it->first,
                                                      it->second));
    }
    delete [] _table;
    _table = new_hashtable;
    _capacity = new_cap;
  }

  void rebalance(bool insert)
  {
    int next_capacity = _capacity;
    bool changed = false;
    if (insert)
    {
      handle_insert (next_capacity, changed);
    }
    else
    {
      handle_erase (next_capacity, changed);
    }
    if (changed)
    {
      rehash(next_capacity);
    }
  }

  void handle_insert (int &next_capacity, bool &changed) const
  {
    double cur_load_factor = get_load_factor();
    while (cur_load_factor > UPPER_LOAD_FACTOR)
    {
      changed = true;
      next_capacity *= GROWTH_FACTOR;
      cur_load_factor /= GROWTH_FACTOR;
    }
  }

  void handle_erase (int &next_capacity, bool &changed) const
  {
    double cur_load_factor = get_load_factor();
    while (cur_load_factor < LOWER_LOAD_FACTOR && cur_load_factor != 0)
    {
      changed = true;
      next_capacity /= GROWTH_FACTOR;
      cur_load_factor *= GROWTH_FACTOR;
    }
    if (next_capacity < MINIMAL_CAPACITY || cur_load_factor == 0)
    {
      changed = true;
      next_capacity = MINIMAL_CAPACITY;
    }
  }

  void fill_table(const HashMap<KeyT, ValueT>& other)
  {
    for (const_iterator it = other.cbegin() ; it != other.cend() ; it++)
    {
      insert(it->first, it->second);
    }
  }

 public:
  class ConstIterator
  {
   public:
    // Iterator traits:
    using value_type = PairT;
    using reference = const PairT&;
    using pointer = const PairT*;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::forward_iterator_tag;

    ConstIterator(const HashMap<KeyT, ValueT>& hm, int bucket_i, int pair_i);

    ConstIterator& operator++();

    ConstIterator operator++(int);

    bool operator==(const ConstIterator &other) const;

    bool operator!= (const ConstIterator &other ) const
    { return !operator== (other); }

    reference operator*()
    { return _hash_map._table[_bucket_index].operator[](_pair_index); }

    pointer operator->() { return &(operator*()); }

   protected:
    friend class HashMap<KeyT, ValueT>;
    const HashMap<KeyT, ValueT>& _hash_map;
    int _bucket_index, _pair_index;

    void skip_empty_buckets (Buckets &cur_bucket)
    {
      while (_bucket_index != _hash_map._capacity)
      {
        cur_bucket = _hash_map._table[_bucket_index];
        if (!cur_bucket.empty())
        {
          break;
        }
        _bucket_index++;
        _pair_index = 0;
      }
    }
  };

  using const_iterator = ConstIterator;

  const_iterator begin() const
  { return ConstIterator(*this, 0, 0); }

  const_iterator cbegin() const
  { return ConstIterator(*this, 0, 0); }

  const_iterator end() const
  { return ConstIterator(*this, _capacity, 0); }

  const_iterator cend() const
  { return ConstIterator(*this, _capacity, 0); }
};

template <typename KeyT, typename ValueT>
HashMap<KeyT, ValueT>::HashMap() :_size(0), _capacity(INITIAL_CAPACITY)
{
  _table = new Buckets[_capacity];
}

template <typename KeyT, typename ValueT>
HashMap<KeyT, ValueT>::HashMap(const std::vector<KeyT> &keys,
                               const std::vector<ValueT> &values)
{
  if (keys.size() != values.size())
  {
    throw std::invalid_argument(INVALID_KEYS_VALUES_ERROR);
  }
  _size = 0;
  _capacity = INITIAL_CAPACITY;
  _table = new Buckets[_capacity];
  for (int i = 0 ; i < (int) keys.size() ; i++)
  {
    if(!insert(keys[i], values[i]))
    {
      operator[] (keys[i]) = values[i];
    }
  }
}

template <typename KeyT, typename ValueT>
HashMap<KeyT, ValueT>::HashMap(const HashMap<KeyT, ValueT> &other)
{
  _capacity = other._capacity;
  _size = 0;
  _table = new Buckets[_capacity];
  fill_table(other);
}
template <typename KeyT, typename ValueT>
HashMap<KeyT, ValueT>::~HashMap()
{
  delete[] _table;
  delete _empty_table;
}

template <typename KeyT, typename ValueT>
HashMap<KeyT, ValueT>& HashMap<KeyT, ValueT>::operator=
                                            (const HashMap<KeyT,ValueT> &other)
{
  if (this != &other)
  {
    clear();
    _capacity = other._capacity;
    _table = new Buckets[_capacity];
    fill_table (other);
  }
  return *this;
}

template <typename KeyT, typename ValueT>
bool HashMap<KeyT, ValueT>::insert(const KeyT &key, const ValueT &value)
{
  IterT iter_pos = get_iterator_position_on_bucket(key);
  if (iter_pos != _null_iter)
  {
    return false;
  }
  _table[hash(key)].push_back(PairT (key, value));
  _size++;
  rebalance (true);
  return true;
}


template <typename KeyT, typename ValueT>
bool HashMap<KeyT, ValueT>::erase(const KeyT &key)
{
  IterT iter_pos = get_iterator_position_on_bucket(key);
  if (iter_pos == _null_iter)
  {
    return false;
  }
  _table[hash(key)].erase(iter_pos);
  _size--;
  rebalance(false);
  return true;
}

template <typename KeyT, typename ValueT>
const ValueT& HashMap<KeyT, ValueT>::at(const KeyT &key) const
{
  IterT iter_pos = get_iterator_position_on_bucket(key);
  if (iter_pos == _null_iter)
  {
    throw std::out_of_range(KEY_ERROR);
  }
  return iter_pos->second;
}

template <typename KeyT, typename ValueT>
ValueT& HashMap<KeyT, ValueT>::at(const KeyT &key)
{
  IterT iter_pos = get_iterator_position_on_bucket(key);
  if (iter_pos == _null_iter)
  {
    throw std::out_of_range(KEY_ERROR);
  }
  return iter_pos->second;
}

template <typename KeyT, typename ValueT>
const ValueT HashMap<KeyT, ValueT>::operator[](const KeyT &key) const
{
  IterT iter_pos = get_iterator_position_on_bucket(key);
  if (iter_pos != _null_iter)
  {
    return iter_pos->second;
  }
  return ValueT();
}

template <typename KeyT, typename ValueT>
ValueT& HashMap<KeyT, ValueT>::operator[](const KeyT &key)
{
  IterT iter_pos = get_iterator_position_on_bucket(key);
  if (iter_pos != _null_iter)
  {
    return iter_pos->second;
  }
  else
  {
    insert(key, ValueT());
    iter_pos = get_iterator_position_on_bucket (key);
    return iter_pos->second; //return ValueT()
  }
}

template <typename KeyT, typename ValueT>
bool HashMap<KeyT, ValueT>::operator==(const HashMap<KeyT, ValueT>& other)const
{
  if (_size != other._size)
  {
    return false;
  }
  for (const_iterator it =  other.cbegin(); it != other.cend(); it++)
  {
    if ( !contains_key( it->first ) )
    {
      return false;
    }
    if ( at( it->first ) != it->second )
    {
      return false;
    }
  }
  for (const_iterator it = cbegin(); it != cend(); it++ )
  {
    if ( !other.contains_key( it->first ) )
    {
      return false;
    }
    if (other.at(it->first) != at (it->first))
    {
      return false;
    }
  }
  return true;
}

template <typename KeyT, typename ValueT>
int HashMap<KeyT, ValueT>::bucket_index(const KeyT &key) const
{
  if (!contains_key(key))
  {
    throw std::invalid_argument(KEY_ERROR);
  }
  return hash(key);
}

template <typename KeyT, typename ValueT>
void HashMap<KeyT, ValueT>::clear()
{
  delete[] _table;
  _table = new Buckets[_capacity];
  _size = 0;
}

template <typename KeyT, typename ValueT>
HashMap<KeyT, ValueT>::ConstIterator::ConstIterator
(const HashMap<KeyT, ValueT> &hm, int bucket_i, int pair_i)
: _hash_map(hm), _bucket_index(bucket_i), _pair_index(pair_i)
{
  if (_bucket_index != _hash_map._capacity)
  {
    Buckets cur_bucket = _hash_map._table[_bucket_index];
    skip_empty_buckets (cur_bucket);
  }
}

template <typename KeyT, typename ValueT>
typename HashMap<KeyT, ValueT>::ConstIterator&
HashMap<KeyT,ValueT>::ConstIterator::operator++ ()
{
  Buckets cur_bucket = _hash_map._table[_bucket_index];
  if (++_pair_index >= (int) cur_bucket.size())
  {
    _pair_index = 0;
    _bucket_index++;
  }
  if(_bucket_index == _hash_map._capacity)
  {
    return *this;
  }
  skip_empty_buckets (cur_bucket);
  return *this;
}

template <typename KeyT, typename ValueT>
typename HashMap<KeyT, ValueT>::ConstIterator
HashMap<KeyT,ValueT>::ConstIterator::operator++ (int)
{
ConstIterator cur_it = *this;
operator++();
return cur_it;
}

template <typename KeyT, typename ValueT>
bool HashMap<KeyT, ValueT>::ConstIterator::operator==
                                          (const ConstIterator &other) const
{
  return ((&_hash_map == &other._hash_map) &&
  (_bucket_index == other._bucket_index) &&
  (_pair_index == other._pair_index));
}

#endif //_HASHMAP_HPP_
