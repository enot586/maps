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

  typename _T::iterator end() const noexcept
  {
    std::lock_guard<_Mutex_type> lock(total_mutex);
    return data.end();
  }

  typename _T::const_iterator cbegin() const noexcept
  {
    std::lock_guard<_Mutex_type> lock(total_mutex);
    return data.cbegin();
  }

  typename _T::const_iterator cbegin ( typename _T::size_type n )
  {
    std::lock_guard<_Mutex_type> lock(total_mutex);
    return data.cbegin(n);
  }

  typename _T::const_iterator cend() const noexcept
  {
    std::lock_guard<_Mutex_type> lock(total_mutex);
    return data.cend();
  }

  typename _T::const_iterator cend ( typename _T::size_type n )
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

  typename _T::size_type erase(const typename _T::key_type& val)
  {
    std::lock_guard<_Mutex_type> lock(total_mutex);
    return data.erase(val);
  }

  typename _T::iterator erase(typename _T::const_iterator position)
  {
    std::lock_guard<_Mutex_type> lock(total_mutex);
    return data.erase(position);
  }

  typename _T::iterator erase(typename _T::iterator first, typename _T::iterator last)
  {
    auto it = first;
    for (; it != last; ++it) {
      erase(it);
    }
    return it;
  }

  //Element access:
  typename _T::mapped_type& operator[](const typename _T::key_type& k)
  {
    std::lock_guard<_Mutex_type> lock(total_mutex);
    return data[k];
  }

  typename _T::mapped_type& operator[](typename _T::key_type&& k)
  {
    std::lock_guard<_Mutex_type> lock(total_mutex);
    return data[k];
  }

  typename _T::mapped_type& at ( const typename _T::value_type& k )
  {
    std::lock_guard<_Mutex_type> lock(total_mutex);
    return data.at(k);
  }

  const typename _T::mapped_type& at ( const typename _T::value_type& k ) const
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

  typename _T::size_type size() const noexcept
  {
    std::lock_guard<_Mutex_type> lock(total_mutex);
    return data.size();
  }

  typename _T::size_type max_size() const noexcept
  {
    std::lock_guard<_Mutex_type> lock(total_mutex);
    return data.max_size();
  }

  //Buckets:
  typename _T::size_type bucket_count() const noexcept
  {
    std::lock_guard<_Mutex_type> lock(total_mutex);
    return data.bucket_count();
  }

  typename _T::size_type max_bucket_count() const noexcept
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

private:
  std::unordered_map<__Key,__Value> m;
};

}

#endif // MAP3_HPP
