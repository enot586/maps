#ifndef TMAP2_H
#define TMAP2_H

namespace t2
{

/**
 *  Второй вариант трактовки условия:
 *  Реализация потокобезопасного контейнера превосходящего по производительности только std::map,
 *  без использования сторонних инструментов(т.к. std::unordered_map из коробки делает почти все что нужно)
 */
template<typename _Key, typename _Value>
class map
{

public:
  using iterator = std::iterator<std::forward_iterator_tag, _Value>;
  using const_iterator = std::iterator<std::forward_iterator_tag, const _Value>;

  using key_type    = _Key;
  using size_type   = size_t;
  using value_type  = std::pair< _Key, _Value >;

  map()
  {  }

  ~map()
  {  }

  //Iterators:
  iterator begin() noexcept
  {  }

  iterator end() noexcept
  {  }

  //Modifiers:
  void insert( const value_type& val )
  {

  }

  size_type erase(const key_type& val)
  {
    return 0;
  }

  iterator erase(const_iterator position)
  {
    //TODO
  }

  //Element access:
  _Value& operator[](const key_type& k)
  {

  }

  _Value& operator[](key_type&& k)
  { }

  //Capacity:
  bool empty() const noexcept
  { return true; }

  size_t size() noexcept
  {
    return 0;
  }

  //Buckets:
  size_t bucket_count() const noexcept
  { return 0; }

  //Hash policy
  void reserve ( size_t n )
  { }

  float load_factor() const noexcept
  { return static_cast<float>( size() ) / static_cast<float>( bucket_count() ); }

  void rehash( size_t n )
  { }

private:

  std::mutex total_mutex;
  size_t total_elements_count;
};

}

#endif // TMAP2_H
