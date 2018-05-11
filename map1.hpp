#ifndef TMAP1_H
#define TMAP1_H

#include <mutex>
#include <unordered_map>
#include <vector>

namespace t1
{

/**
 *  Первый вариант трактовки условия:
 *  Необходимо реализовать контейнер, который превосходил бы на многопоточке
 *  выбранный стандартный(map или unordered_map).
 *  Реализация не закончена потому что не уверен, что понял задание правильно.
 *  Попытка получить выйгрыш в производительности за счет распараллеливания доступа
 */
template<typename _Key, typename _Value, size_t _Super_bucket_number=10>
class map
{

public:
  using iterator = std::iterator<std::forward_iterator_tag, _Value>;
  using const_iterator = std::iterator<std::forward_iterator_tag, const _Value>;

  using key_type    = _Key;
  using size_type   = size_t;
  using value_type  = std::pair< _Key, _Value >;

  template<typename __Key, typename __Value>
  struct super_bucket
  {
    std::mutex  m;
    std::unordered_map<__Key, __Value> v;
  };

  using super_bucket_t = super_bucket<size_t, _Value>;

  map() : intervals(super_bucket_count), total_elements_count(0)
  {  }

  ~map()
  {  }

  //Iterators:
  iterator begin() noexcept
  { return intervals[0].v.begin(); }

  iterator end() noexcept
  { return intervals.end(); }

  //Modifiers:
  void insert( const value_type& val )
  {
    size_t hash_level1 = std::hash<_Key>{}(val.first);
    size_t n_interval = hash_level1 % super_bucket_count;
    auto& super_bucket = intervals[n_interval];
    {
      std::lock_guard<std::mutex> lock(super_bucket.m);
      super_bucket.v[hash_level1] = val.second;
      ++total_elements_count;
    }
  }

  size_type erase(const key_type& val)
  {
    size_t hash_level1 = std::hash<_Key>{}(val);
    size_t n_interval = hash_level1 % super_bucket_count;
    auto& super_bucket = intervals[n_interval];

    std::lock_guard<std::mutex> lock(super_bucket.m);
    size_t n_el = super_bucket.v.erase(hash_level1);

    if ( n_el )
      total_elements_count-= n_el;

    return n_el;
  }

  //Element access:
  _Value& operator[](const key_type& k)
  {
    size_t hash_level1 = std::hash<_Key>{}(k);
    size_t n_interval = hash_level1 % super_bucket_count;
    return intervals[n_interval].v[hash_level1];
  }

  _Value& operator[](key_type&& k)
  { return operator[]( static_cast<const key_type&>(k) ); }

  //Capacity:
  bool empty() const noexcept
  { return size() == 0; }

  size_t size() noexcept
  {
    std::lock_guard<std::mutex> lock(total_mutex);
    size_t s = 0;
    for (auto it = intervals.begin(); it!= intervals.end(); ++it) {
      s+= it->v.size();
    }

    return s;
  }

  //Buckets:
  size_t bucket_count() const noexcept
  {
    std::lock_guard<std::mutex> lock(total_mutex);
    return intervals.size();
  }

  //Hash policy
  void reserve ( size_t n )
  { }

  float load_factor() const noexcept
  { return static_cast<float>( size() ) / static_cast<float>( bucket_count() ); }

  void rehash( size_t n )
  { }

private:
  std::vector< super_bucket_t > intervals;

  static const size_t super_bucket_count = _Super_bucket_number;
  std::mutex total_mutex;
  size_t total_elements_count;
};

}


#endif // TMAP1_H
