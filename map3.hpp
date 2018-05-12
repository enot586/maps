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
template<typename T>
class threadsafe_adapter
{
public:
  threadsafe_adapter(T& __map) : data(__map)
  {  }

  ~threadsafe_adapter()
  {  }

  //Iterators:
  typename T::iterator begin() const noexcept
  {
    std::lock_guard<std::mutex> lock(total_mutex);
    return data.begin();
  }

  auto end() const noexcept
  {
    std::lock_guard<std::mutex> lock(total_mutex);
    return data.begin();
  }

  auto cbegin() const noexcept
  {
    std::lock_guard<std::mutex> lock(total_mutex);
    return data.cbegin();
  }

  auto cbegin ( typename T::size_type n )
  {
    std::lock_guard<std::mutex> lock(total_mutex);
    return data.cbegin(n);
  }

  auto cend() const noexcept
  {
    std::lock_guard<std::mutex> lock(total_mutex);
    return data.cend();
  }

  auto cend ( typename T::size_type n )
  {
    std::lock_guard<std::mutex> lock(total_mutex);
    return cend(n);
  }

  template <class... Args>
  std::pair<typename T::iterator, bool> emplace ( Args&&... args )
  {
    std::lock_guard<std::mutex> lock(total_mutex);
    return data.emplace( args ...  );
  }

  //Modifiers:
  void insert( const typename T::value_type& val )
  {
    std::lock_guard<std::mutex> lock(total_mutex);
    return data.insert(val);
  }

  auto erase(const typename T::key_type& val)
  {
    std::lock_guard<std::mutex> lock(total_mutex);
    return data.erase(val);
  }

  auto erase(typename T::const_iterator position)
  {
    std::lock_guard<std::mutex> lock(total_mutex);
    return data.erase(position);
  }

  //Element access:
  auto& operator[](const typename T::key_type& k)
  {
    std::lock_guard<std::mutex> lock(total_mutex);
    return data[k];
  }

  auto& operator[](typename T::key_type&& k)
  {
    std::lock_guard<std::mutex> lock(total_mutex);
    return data[k];
  }

  auto& at ( const typename T::value_type& k )
  {
    std::lock_guard<std::mutex> lock(total_mutex);
    return data.at(k);
  }

  const auto& at ( const typename T::value_type& k ) const
  {
    std::lock_guard<std::mutex> lock(total_mutex);
    return data.at(k);
  }

  //Capacity:
  bool empty() const noexcept
  {
    std::lock_guard<std::mutex> lock(total_mutex);
    return data.empty();
  }

  auto size() const noexcept
  {
    std::lock_guard<std::mutex> lock(total_mutex);
    return data.size();
  }

  auto max_size() const noexcept
  {
    std::lock_guard<std::mutex> lock(total_mutex);
    return data.max_size();
  }

  //Buckets:
  auto bucket_count() const noexcept
  {
    std::lock_guard<std::mutex> lock(total_mutex);
    return data.bucket_count();
  }

  auto max_bucket_count() const noexcept
  {
    std::lock_guard<std::mutex> lock(total_mutex);
    return data.max_bucket_count();
  }

  //Hash policy
  void reserve ( size_t n )
  {
    std::lock_guard<std::mutex> lock(total_mutex);
    data.reserve(n);
  }

  float load_factor() const noexcept
  {
    std::lock_guard<std::mutex> lock(total_mutex);
    return data.load_factor();
  }

  void rehash( typename T::size_type n )
  {
    std::lock_guard<std::mutex> lock(total_mutex);
    return data.rehash(n);
  }

private:
  T& data;
  mutable std::mutex total_mutex;
};

template <typename __Key, typename __Value>
class map : public threadsafe_adapter< std::unordered_map<__Key,__Value> >
{
  std::unordered_map<__Key,__Value> m;

public:
  map() :
    threadsafe_adapter<std::unordered_map<__Key,__Value> >::threadsafe_adapter(m)
  {  }
};

}

#endif // MAP3_HPP
