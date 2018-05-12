#ifndef TMAP1_H
#define TMAP1_H

#include <mutex>
#include <unordered_map>
#include <vector>

namespace t1
{

/**
 *  Первый вариант трактовки условия:
 *  Необходимо реализовать контейнер, который бы превосходил своего
 *  одноименника в std на многопоточке.
 *  Попытка получить выйгрыш над std::unordered_map в производительности
 *  за счет распараллеливания доступа
 */
template<typename _Key, typename _Value,
         typename _Mutex_type=std::mutex,
         size_t _NUMBER_SUPER_BUCKETS=10>
class map
{
public:
  typedef std::unordered_map<size_t, _Value> bucket_data_model;
  typedef typename bucket_data_model::iterator::iterator_category  bucket_iterator_category;

  struct super_bucket
  {
    mutable _Mutex_type  m;
    bucket_data_model v;
  };

  class iterator
  {
    public:
      typedef  typename bucket_data_model::iterator        self_type;
      typedef  typename bucket_data_model::value_type      value_type;
      typedef  typename bucket_data_model::value_type&     reference;
      typedef  typename bucket_data_model::iterator        pointer;
      typedef  bucket_iterator_category                    iterator_category;
      typedef  typename bucket_data_model::difference_type difference_type;

      iterator(map& base, size_t interval, pointer ptr) :
        base_(base), interval_(interval), ptr_(ptr)  { }

      iterator operator++()
      {
        auto& sb = base_.get_interval(interval_);
        std::lock_guard<_Mutex_type> lock(sb.m);

        if ( ptr_ != sb.v.end() ) {
          ++ptr_;
        } else {
          auto& sb = base_.get_interval(++interval_);
          std::lock_guard<_Mutex_type> lock(sb.m);
          ptr_= sb.v.begin();
        }
        return *this;
      }

      iterator operator++(int)
      {
        iterator i = *this;
        auto& sb = base_.get_interval(interval_);
        std::lock_guard<_Mutex_type> lock(sb.m);

        if ( ptr_ != sb.v.end() ) {
          ++ptr_;
        } else {
          auto& sb = base_.get_interval(++interval_);
          std::lock_guard<_Mutex_type> lock(sb.m);
          ptr_= sb.v.begin();
        }
        return i;
      }

      reference operator*() { return *ptr_; }
      pointer operator->() { return ptr_; }
      bool operator==(const iterator& rhs) { return ptr_ == rhs.ptr_; }
      bool operator!=(const iterator& rhs) { return ptr_ != rhs.ptr_; }

    protected:
      map& base_;
      size_t interval_;
      pointer ptr_;
  };

  class const_iterator: public iterator
  {
    typedef  typename bucket_data_model::iterator        self_type;
    typedef  typename bucket_data_model::value_type      value_type;
    typedef  typename bucket_data_model::value_type&     reference;
    typedef  typename bucket_data_model::iterator        pointer;
    typedef  bucket_iterator_category                    iterator_category;
    typedef  typename bucket_data_model::difference_type difference_type;

    const_iterator(map& base, size_t interval, pointer ptr) :
      iterator(base, interval, ptr)
    { }
  };

  typedef _Key key_type;
  typedef size_t size_type;
  typedef std::pair< _Key, _Value > value_type;

  map() : intervals_(super_bucket_count_)
  {  }

  map(const map&) = delete;
  map& operator=(const map&) = delete;

  map(map&&) = default;
  map& operator=(map&&) = default;

  virtual ~map()
  {  }

  //Iterators:
  iterator begin() noexcept
  {
    return iterator( *this, 0, intervals_.begin()->v.begin() );
  }

  iterator end() noexcept
  {
    return iterator( *this,
                     std::distance( intervals_.begin(), intervals_.end() )-1,
                     intervals_.end()->v.end() );
  }

  const_iterator cbegin() const noexcept
  {
    return const_iterator( *this,
                           std::distance( intervals_.begin(), intervals_.end() )-1,
                           intervals_.end()->v.end() );
  }

  const_iterator cend() const noexcept
  {
    return const_iterator( *this,
                           std::distance( intervals_.begin(), intervals_.end() )-1,
                           intervals_.end()->v.end() );

  }

  //Modifiers:
  void insert( const value_type& val )
  {
    size_t hash_level1 = std::hash<_Key>{}(val.first);
    size_t n_interval = hash_level1 % super_bucket_count_;
    auto& super_bucket = intervals_[n_interval];

    std::lock_guard<_Mutex_type> lock(super_bucket.m);
    super_bucket.v[hash_level1] = val.second;
  }

  size_type erase(const key_type& val)
  {
    size_t hash_level1 = std::hash<_Key>{}(val);
    size_t n_interval = hash_level1 % super_bucket_count_;
    auto& super_bucket = intervals_[n_interval];

    std::lock_guard<_Mutex_type> lock(super_bucket.m);
    size_t n_el = super_bucket.v.erase(hash_level1);
    return n_el;
  }

  //Element access:
  _Value& operator[](const key_type& k)
  {
    size_t hash_level1 = std::hash<_Key>{}(k);
    size_t n_interval = hash_level1 % super_bucket_count_;

    std::lock_guard<_Mutex_type> lock(intervals_[n_interval].m);
    std::pair<size_t, _Value> p;
    p.first = hash_level1;
    auto it = intervals_[n_interval].v.insert(p);
    return (*it.first).second;
  }

  super_bucket& get_interval(size_t n)
  { return intervals_[n]; }

  _Value& operator[](key_type&& k)
  { return operator[]( static_cast<const key_type&>(k) ); }

  //Capacity:
  bool empty() const noexcept
  { return size() == 0; }

  size_t size() noexcept
  {
    std::lock_guard<_Mutex_type> lock(total_mutex_);
    size_t s = 0;
    for (auto& it : intervals_) {
      s+= it.v.size();
    }

    return s;
  }

  //Buckets:
  size_t bucket_count() const noexcept
  {
    std::lock_guard<_Mutex_type> lock(total_mutex_);
    return intervals_.size();
  }

  //Hash policy
  void reserve ( size_t n )
  {
    for (auto& it : intervals_) {
      {
        std::lock_guard<_Mutex_type> lock(it.m);
        it.v.reserve(n);
      }
    }
  }

  float load_factor() const noexcept
  { return static_cast<float>( size() ) / static_cast<float>( bucket_count() ); }

  void rehash( size_t n )
  {
    for (auto& it : intervals_) {
      {
        std::lock_guard<_Mutex_type> lock(it.m);
        it.v.rehash(n);
      }
    }
  }

private:
  static const size_t super_bucket_count_ = _NUMBER_SUPER_BUCKETS;
  std::vector< super_bucket > intervals_;
  mutable _Mutex_type total_mutex_;
};

}


#endif // TMAP1_H
