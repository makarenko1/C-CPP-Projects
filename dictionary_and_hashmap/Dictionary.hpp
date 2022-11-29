#ifndef _DICTIONARY_HPP_
#define _DICTIONARY_HPP_

#include "HashMap.hpp"

#define KEY_DICT_ERROR "Error: Key not in dictionary!"

class InvalidKey : public std::invalid_argument
{
 public:
  InvalidKey() : std::invalid_argument(KEY_DICT_ERROR) {}
  explicit InvalidKey(const std::string &msg) : std::invalid_argument(msg) {}
//  std::string what ()
//  {
//    return KEY_DICT_ERROR;
//  }
};


class Dictionary : public HashMap<std::string, std::string>
{
 public:
   Dictionary() = default;
   Dictionary(const std::vector<std::string> &keys,
              const std::vector<std::string> &values) :
              HashMap<std::string, std::string>(keys, values) {}

   bool erase(const std::string &key) override;
   template<class ForwardIterator>
   void update(ForwardIterator start, ForwardIterator end);
};

bool Dictionary::erase (const std::string &key)
 {
    if (!HashMap<std::string, std::string>::erase(key))
   {
     throw InvalidKey();
   }
   return true;
 }

template<class ForwardIterator>
void Dictionary::update(const ForwardIterator start, const
ForwardIterator end)
{
  for (auto it = start ; it != end ; it++)
    {
      if (!HashMap<std::string, std::string>::insert(it->first, it->second))
        {
          HashMap<std::string, std::string>::at(it->first) = it->second;
        }
    }
}

#endif //_DICTIONARY_HPP_
