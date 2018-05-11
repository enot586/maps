#ifndef STRING_HASH_H
#define STRING_HASH_H

#include <string>
#include <functional>
/*
namespace std
{
template<>
  struct hash<string> : public __hash_base<size_t, _Tp*>
  {
    unsigned constexpr const_hash(char const *input)
    {
        return *input ?
            static_cast<unsigned int>(*input) + 33 * const_hash(input + 1) :
            5381;
    }

    size_t operator()(const string&  v) const noexcept
    {
      return const_hash( v.c_str() );
    }
  };

}*/
#endif // STRING_HASH_H
