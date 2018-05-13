#ifndef MAP3_HPP
#define MAP3_HPP

#include <unordered_map>
#include <mutex>

namespace t3
{

/**
 *  Трейти вариант трактовки условия, формальный:
 *  Реализация потокобезопасного контейнера, превосходящего по производительности только std::map.
 *  Сделать наиболее просто и не замороченно.
 */
template<typename _T, typename _Mutex_type>
class threadsafe_adapter
{
public:
  threadsafe_adapter(_T& __map) : data(__map)
  {  }

  ~threadsafe_adapter()
  {  }

  //Iterators:
  typename _T::iterator begin() const noexcept
  {
    std::lock_guard<_Mutex_type> lock(total_mutex);
    return data.begin();
  }

  auto end() const noexcept
  {
    std::lock_guard<_Mutex_type> lock(total_mutex);
    return data.end();
  }

  auto cbegin() const noexcept
  {
    std::lock_guard<_Mutex_type> lock(total_mutex);
    return data.cbegin();
  }

  auto cbegin ( typename _T::size_type n )
  {
    std::lock_guard<_Mutex_type> lock(total_mutex);
    return data.cbegin(n);
  }

  auto cend() const noexcept
  {
    std::lock_guard<_Mutex_type> lock(total_mutex);
    return data.cend();
  }

  auto cend ( typename _T::size_type n )
  {
    std::lock_guard<_Mutex_type> lock(total_mutex);
    return cend(n);
  }

  template <class... Args>
  std::pair<typename _T::iterator, bool> emplace ( Args&&... args )
  {
    std::lock_guard<_Mutex_type> lock(total_mutex);
    return data.emplace( args ...  );
  }

  //Modifiers:
  void insert( const typename _T::value_type& val )
  {
    std::lock_guard<_Mutex_type> lock(total_mutex);
    return data.insert(val);
  }

  auto erase(const typename _T::key_type& val)
  {
    std::lock_guard<_Mutex_type> lock(total_mutex);
    return data.erase(val);
  }

  auto erase(typename _T::const_iterator position)
  {
    std::lock_guard<_Mutex_type> lock(total_mutex);
    return data.erase(position);
  }

  auto erase(typename _T::iterator first, typename _T::iterator last)
  {
    auto it = first;
    for (; it != last; ++it) {
      erase(it);
    }
    return it;
  }

  //Element access:
  auto& operator[](const typename _T::key_type& k)
  {
    std::lock_guard<_Mutex_type> lock(total_mutex);
    return data[k];
  }

  auto& operator[](typename _T::key_type&& k)
  {
    std::lock_guard<_Mutex_type> lock(total_mutex);
    return data[k];
  }

  auto& at ( const typename _T::value_type& k )
  {
    std::lock_guard<_Mutex_type> lock(total_mutex);
    return data.at(k);
  }

  const auto& at ( const typename _T::value_type& k ) const
  {
    std::lock_guard<_Mutex_type> lock(total_mutex);
    return data.at(k);
  }

  //Capacity:
  bool empty() const noexcept
  {
    std::lock_guard<_Mutex_type> lock(total_mutex);
    return data.empty();
  }

  auto size() const noexcept
  {
    std::lock_guard<_Mutex_type> lock(total_mutex);
    return data.size();
  }

  auto max_size() const noexcept
  {
    std::lock_guard<_Mutex_type> lock(total_mutex);
    return data.max_size();
  }

  //Buckets:
  auto bucket_count() const noexcept
  {
    std::lock_guard<_Mutex_type> lock(total_mutex);
    return data.bucket_count();
  }

  auto max_bucket_count() const noexcept
  {
    std::lock_guard<_Mutex_type> lock(total_mutex);
    return data.max_bucket_count();
  }

  //Hash policy
  void reserve ( size_t n )
  {
    std::lock_guard<_Mutex_type> lock(total_mutex);
    data.reserve(n);
  }

  float load_factor() const noexcept
  {
    std::lock_guard<_Mutex_type> lock(total_mutex);
    return data.load_factor();
  }

  void rehash( typename _T::size_type n )
  {
    std::lock_guard<_Mutex_type> lock(total_mutex);
    return data.rehash(n);
  }

private:
  _T& data;
  mutable _Mutex_type total_mutex;
};

template <typename __Key, typename __Value, typename mutex_type=std::mutex>
class map : public threadsafe_adapter< std::unordered_map<__Key,__Value>, mutex_type >
{
public:
  std::unordered_map<__Key,__Value> m;

  map() :
    threadsafe_adapter<std::unordered_map<__Key,__Value>, mutex_type>::threadsafe_adapter(m)
  {  }

  virtual ~map()
  {

  }

  map(const map& v) = delete;
  map& operator=(const map&) = delete;

  map(map&& v) = default;
  map& operator=(map&& v) = default;
};

}

#endif // MAP3_HPP
