#ifndef TMAP1_H
#define TMAP1_H

#include <mutex>
#include <unordered_map>
#include <vector>
#include <atomic>

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
  typedef std::pair<_Key, _Value> value_type;
  typedef std::unordered_map<size_t, value_type> bucket_data_model;
  typedef typename bucket_data_model::iterator::iterator_category  bucket_iterator_category;
  typedef typename bucket_data_model::const_iterator::iterator_category  bucket_const_iterator_category;

  struct super_bucket
  {
    std::atomic<size_t> reference_counter;
    mutable _Mutex_type  m;
    bucket_data_model    v;

    super_bucket() : reference_counter(0)
    {}

    inline bool is_busy()
    { return reference_counter.load(); }

    inline void inc_ref()
    { ++reference_counter; }

    inline void dec_ref()
    { --reference_counter; }
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

      iterator(map* base, size_t interval, pointer ptr) :
        base_(base), super_bucket_index_(interval), ptr_(ptr)
      { }

      iterator(const iterator& v) :
        base_( v.base() ),
        super_bucket_index_( v.interval() ),
        ptr_( v.get_internal_iterator() )
      { }

      iterator& operator=(const iterator& v)
      {
        base_ = v.base();
        ptr_ = v.get_internal_iterator();
        super_bucket_index_ = v.interval();
        return *this;
      }

      ~iterator()
      {   }

      iterator& operator=(iterator&& v)
      {
        this->base_ = std::move(v.base_);
        this->ptr_ = std::move(v.ptr_);
        this->super_bucket_index_ = std::move(v.super_bucket_index_);
        return *this;
      }

      iterator operator++()
      {
        bool next{false};
        size_t inervals_n;
        do
        {
          {//searching in current super_bucket
            auto& sb = base_->get_super_bucket(super_bucket_index_);
            inervals_n = base_->bucket_count();
            std::lock_guard<_Mutex_type> lock(sb.m);

            if (next)
              ptr_= sb.v.begin();

            if ( ptr_ != sb.v.end() ) {

              if (next)
                return *this;

              ++ptr_;
              if ( ptr_ != sb.v.end() )
                return *this;
            }
          }

          //goto next super_bucket
          if (super_bucket_index_ < inervals_n-1) {
            ++super_bucket_index_;
            next = true;
          } else {
            return *this;
          }
        } while (next);

        return *this;
      }

      iterator operator++(int)
      {
        iterator i = *this;
        operator++();
        return i;
      }

      map::value_type* operator->(){ return &ptr_->second; }
      map::value_type& operator*() { return ptr_->second; }
      bool operator==(const iterator& rhs) const { return ptr_ == rhs.ptr_; }
      bool operator!=(const iterator& rhs) const { return ptr_ != rhs.ptr_; }

      pointer get_internal_iterator() const { return ptr_; }
      size_t interval() const { return super_bucket_index_; }
      map* base() const { return base_; }

    private:
      map* base_;
      size_t super_bucket_index_;
      pointer ptr_;
  };

  typedef const iterator const_iterator;

  typedef _Key key_type;
  typedef size_t size_type;

  map() : super_buckets(super_bucket_count_)
  {  }

  map(const map&) = delete;
  map& operator=(const map&) = delete;

  map(map&&) = default;
  map& operator=(map&&) = default;

  virtual ~map()
  {  }

  //Element lookup
  iterator find ( const key_type& k ) {
    size_t hash_level1 = std::hash<key_type>{}(k);
    size_t n_interval = hash_level1 % super_bucket_count_;
    auto& super_bucket = super_buckets[n_interval];
    auto res = super_bucket.v.find(hash_level1);

    if ( res != super_bucket.v.end() ) {
      return iterator(this, n_interval, res);
    }

    return end();
  }

  iterator find_first() noexcept
  {
    for (auto it = super_buckets.begin(); it != super_buckets.end(); ++it) {
      if ( it->v.begin() != it->v.end() ) {
        return iterator(
                         this, std::distance(super_buckets.begin(), it),
                         it->v.begin()
                       );
      }
    }

    return end();
  }

  //Iterators:
  iterator begin() noexcept
  { return find_first(); }

  iterator end() noexcept
  {
    return iterator( this,
                     std::distance( super_buckets.begin(), super_buckets.end() )-1,
                     super_buckets.end()->v.end() );
  }

  const_iterator cbegin()  noexcept
  { return find_first(); }

  const_iterator cend()  noexcept
  {
    return const_iterator( this,
                           std::distance( super_buckets.begin(), super_buckets.end() )-1,
                           super_buckets.end()->v.end() );
  }

  //Modifiers:
  void insert(const value_type& val)
  {
    size_t hash_level1 = std::hash<_Key>{}(val.first);
    size_t n_interval = hash_level1 % super_bucket_count_;
    auto& super_bucket = super_buckets[n_interval];

    std::lock_guard<_Mutex_type> lock(super_bucket.m);
    super_bucket.v[hash_level1] = val;
  }

  iterator erase(const_iterator position)
  {
    auto& sb = get_super_bucket( position.interval() );
    auto it = position.get_internal_iterator();

    {
      std::lock_guard<_Mutex_type> lock(sb.m);
      it = sb.v.erase( position.get_internal_iterator() );
    }

    iterator result_it(this, position.interval(), it);

    if ( it == sb.v.end() )
      ++result_it;

    return result_it;
  }

  size_type erase(const key_type& val)
  {
    size_t hash_level1 = std::hash<_Key>{}(val);
    size_t n_interval = hash_level1 % super_bucket_count_;
    auto& super_bucket = super_buckets[n_interval];

    std::lock_guard<_Mutex_type> lock(super_bucket.m);
    size_t n_el = super_bucket.v.erase(hash_level1);
    return n_el;
  }

  iterator erase ( const_iterator first, const_iterator last )
  {
    auto it = first;
    for (; it != last; ++it) {
      it = erase(it);
    }
    return it;
  }

  //Element access:
  _Value& operator[](const key_type& k)
  {
    size_t hash_level1 = std::hash<_Key>{}(k);
    size_t n_interval = hash_level1 % super_bucket_count_;

    std::lock_guard<_Mutex_type> lock(super_buckets[n_interval].m);
    std::pair<size_t, value_type> p;
    p.first = hash_level1;
    p.second.first = k;
    auto it = super_buckets[n_interval].v.insert(p);
    return (it.first)->second.second;
  }

  super_bucket& get_super_bucket(size_t n)
  { return super_buckets[n]; }

  _Value& operator[](key_type&& k)
  { return operator[]( static_cast<const key_type&>(k) ); }

  //Capacity:
  bool empty() const noexcept
  { return size() == 0; }

  size_t size() const noexcept
  {
    std::lock_guard<_Mutex_type> lock(total_mutex_);
    size_t s = 0;
    for (auto& it : super_buckets) {
      s+= it.v.size();
    }

    return s;
  }

  //Buckets:
  size_t bucket_count() const noexcept
  {
    std::lock_guard<_Mutex_type> lock(total_mutex_);
    return super_buckets.size();
  }

  //Hash policy
  void reserve ( size_t n )
  {
    for (auto& it : super_buckets) {
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
    for (auto& it : super_buckets) {
      {
        std::lock_guard<_Mutex_type> lock(it.m);
        it.v.rehash(n);
      }
    }
  }

private:
  static const size_t super_bucket_count_ = _NUMBER_SUPER_BUCKETS;
  std::vector< super_bucket > super_buckets;
  mutable _Mutex_type total_mutex_;
};

}


#endif // TMAP1_H
