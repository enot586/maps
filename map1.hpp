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
 *  Попытка получить выйгрыш в производительности за счет распараллеливания доступа
 */
template<typename _Key, typename _Value, size_t _Super_bucket_number=10>
class map
{

public:
  typedef std::unordered_map<size_t, _Value> bucket_data_model;
  typedef typename bucket_data_model::iterator::iterator_category  bucket_iterator_category;

  struct super_bucket
  {
    std::mutex  m;
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
        iterator i = *this;

        if ( ptr_ != base_.get_interval(interval_).v.end() ) {
          ++ptr_;
        } else {
          ++interval_;
          ptr_= base_.get_interval(interval_).v.begin();
        }
        return i;
      }

      iterator operator++(int junk)
      {
        if (ptr_ != base_.get_interval(interval_).end() ) {
          ++ptr_;
        } else {
          ++interval_;
          ptr_= base_.get_interval(interval_).begin();
        }
        return *this;
      }

      reference operator*() { return *ptr_; }
      pointer operator->() { return ptr_; }
      bool operator==(const iterator& rhs) { return ptr_ == rhs.ptr_; }
      bool operator!=(const iterator& rhs) { return ptr_ != rhs.ptr_; }

    private:
      map& base_;
      size_t interval_;
      pointer ptr_;

  };

  using key_type    = _Key;
  using size_type   = size_t;
  using value_type  = std::pair< _Key, _Value >;

  map() :
    intervals_(super_bucket_count_),
    begin_( *this, 0, intervals_[0].v.begin() ),
    end_( *this, (intervals_.size()-1), intervals_[9].v.begin() )
  {  }

  ~map()
  {  }

  //Iterators:

  iterator begin() noexcept
  { return begin_; }

  iterator end() noexcept
  { return end_; }

  //Modifiers:
  void insert( const value_type& val )
  {
    size_t hash_level1 = std::hash<_Key>{}(val.first);
    size_t n_interval = hash_level1 % super_bucket_count_;
    auto& super_bucket = intervals_[n_interval];
    {
      std::lock_guard<std::mutex> lock(super_bucket.m);
      super_bucket.v[hash_level1] = val.second;
      ++total_elements_count_;
    }
  }

  size_type erase(const key_type& val)
  {
    size_t hash_level1 = std::hash<_Key>{}(val);
    size_t n_interval = hash_level1 % super_bucket_count_;
    auto& super_bucket = intervals_[n_interval];

    std::lock_guard<std::mutex> lock(super_bucket.m);
    size_t n_el = super_bucket.v.erase(hash_level1);

    if ( n_el )
      total_elements_count_-= n_el;

    return n_el;
  }

  //Element access:
  _Value& operator[](const key_type& k)
  {
    size_t hash_level1 = std::hash<_Key>{}(k);
    size_t n_interval = hash_level1 % super_bucket_count_;
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
    std::lock_guard<std::mutex> lock(total_mutex_);
    size_t s = 0;
    for (auto it = intervals_.begin(); it!= intervals_.end(); ++it) {
      s+= it->v.size();
    }

    return s;
  }

  //Buckets:
  size_t bucket_count() const noexcept
  {
    std::lock_guard<std::mutex> lock(total_mutex_);
    return intervals_.size();
  }

  //Hash policy
  void reserve ( size_t n )
  {
    for (auto& it : intervals_) {
      {
        std::lock_guard<std::mutex> lock(it.m);
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
        std::lock_guard<std::mutex> lock(it.m);
        it.v.rehash(n);
      }
    }
  }

private:
  std::vector< super_bucket > intervals_;
  iterator begin_;
  iterator end_;
  static const size_t super_bucket_count_ = _Super_bucket_number;
  std::mutex total_mutex_;
  size_t total_elements_count_;
};

}


#endif // TMAP1_H
