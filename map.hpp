#ifndef MAP_HPP
#define MAP_HPP

namespace t
{

template < typename _Key, typename _Tp >
  class map
  {

  public:

    map() : _M_t() { }

    /**
     *  @brief  %Map copy constructor.
     *  @param  __x  A %map of identical element and allocator types.
     *
     *  The newly-created %map uses a copy of the allocation object
     *  used by @a __x.
     */
    map(const map& __x) : _M_t(__x._M_t) { }

    /**
     *  @brief  %Map move constructor.
     *  @param  __x  A %map of identical element and allocator types.
     *
     *  The newly-created %map contains the exact contents of @a __x.
     *  The contents of @a __x are a valid, but unspecified %map.
     */
    map(map&& __x)
    noexcept(is_nothrow_copy_constructible<_Compare>::value)
    : _M_t(std::move(__x._M_t)) { }

    /**
     *  @brief  Builds a %map from a range.
     *  @param  __first  An input iterator.
     *  @param  __last  An input iterator.
     *
     *  Create a %map consisting of copies of the elements from
     *  [__first,__last).  This is linear in N if the range is
     *  already sorted, and NlogN otherwise (where N is
     *  distance(__first,__last)).
     */
    template<typename _InputIterator>
    map(_InputIterator __first, _InputIterator __last) : _M_t()
      {  }

    /**
     *  @brief  Builds a %map from a range.
     *  @param  __first  An input iterator.
     *  @param  __last  An input iterator.
     *  @param  __comp  A comparison functor.
     *  @param  __a  An allocator object.
     *
     *  Create a %map consisting of copies of the elements from
     *  [__first,__last).  This is linear in N if the range is
     *  already sorted, and NlogN otherwise (where N is
     *  distance(__first,__last)).
     */
    template<typename _InputIterator>
      map(_InputIterator __first, _InputIterator __last,
    const _Compare& __comp,
    const allocator_type& __a = allocator_type()) : _M_t(__comp, _Pair_alloc_type(__a))
      { }

    /**
     *  @brief  %Map assignment operator.
     *  @param  __x  A %map of identical element and allocator types.
     *
     *  All the elements of @a __x are copied, but unlike the copy
     *  constructor, the allocator object is not copied.
     */
    map& operator=(const map& __x)
    {
      _M_t = __x._M_t;
      return *this;
    }

    /// Move assignment operator.
    map& operator=(map&&) = default;

    /**
     *  @brief  %Map list assignment operator.
     *  @param  __l  An initializer_list.
     *
     *  This function fills a %map with copies of the elements in the
     *  initializer list @a __l.
     *
     *  Note that the assignment completely changes the %map and
     *  that the resulting %map's size is the same as the number
     *  of elements assigned.  Old data may be lost.
     */
    map& operator=(initializer_list<value_type> __l)
    {
      _M_t._M_assign_unique(__l.begin(), __l.end());
      return *this;
    }

    // iterators
    /**
     *  Returns a read/write iterator that points to the first pair in the
     *  %map.
     *  Iteration is done in ascending order according to the keys.
     */
    iterator begin()
    { return _M_t.begin(); }

    /**
     *  Returns a read-only (constant) iterator that points to the first pair
     *  in the %map.  Iteration is done in ascending order according to the
     *  keys.
     */
    const_iterator  begin() const
    { return _M_t.begin(); }

    /**
     *  Returns a read/write iterator that points one past the last
     *  pair in the %map.  Iteration is done in ascending order
     *  according to the keys.
     */
    iterator end()
    { return _M_t.end(); }

    /**
     *  Returns a read-only (constant) iterator that points one past the last
     *  pair in the %map.  Iteration is done in ascending order according to
     *  the keys.
     */
    const_iterator end() const
    { return _M_t.end(); }

    /**
     *  Returns a read/write reverse iterator that points to the last pair in
     *  the %map.  Iteration is done in descending order according to the
     *  keys.
     */
    reverse_iterator rbegin()
    { return _M_t.rbegin(); }

    /**
     *  Returns a read-only (constant) reverse iterator that points to the
     *  last pair in the %map.  Iteration is done in descending order
     *  according to the keys.
     */
    const_reverse_iterator rbegin() const
    { return _M_t.rbegin(); }

    /**
     *  Returns a read/write reverse iterator that points to one before the
     *  first pair in the %map.  Iteration is done in descending order
     *  according to the keys.
     */
    reverse_iterator rend() { return _M_t.rend(); }

    /**
     *  Returns a read-only (constant) reverse iterator that points to one
     *  before the first pair in the %map.  Iteration is done in descending
     *  order according to the keys.
     */
    const_reverse_iterator rend() const
    { return _M_t.rend(); }

    /**
     *  Returns a read-only (constant) iterator that points to the first pair
     *  in the %map.  Iteration is done in ascending order according to the
     *  keys.
     */
    const_iterator
    cbegin() const noexcept
    { return _M_t.begin(); }

    /**
     *  Returns a read-only (constant) iterator that points one past the last
     *  pair in the %map.  Iteration is done in ascending order according to
     *  the keys.
     */
    const_iterator
    cend() const noexcept
    { return _M_t.end(); }

    /**
     *  Returns a read-only (constant) reverse iterator that points to the
     *  last pair in the %map.  Iteration is done in descending order
     *  according to the keys.
     */
    const_reverse_iterator
    crbegin() const noexcept
    { return _M_t.rbegin(); }

    /**
     *  Returns a read-only (constant) reverse iterator that points to one
     *  before the first pair in the %map.  Iteration is done in descending
     *  order according to the keys.
     */
    const_reverse_iterator
    crend() const noexcept
    { return _M_t.rend(); }


    // capacity
    /** Returns true if the %map is empty.  (Thus begin() would equal
     *  end().)
    */
    bool empty() const { return _M_t.empty(); }

    /** Returns the size of the %map.  */
    size_type size() const { return _M_t.size(); }

    /** Returns the maximum size of the %map.  */
    size_type max_size() const { return _M_t.max_size(); }

    // [23.3.1.2] element access
    /**
     *  @brief  Subscript ( @c [] ) access to %map data.
     *  @param  __k  The key for which data should be retrieved.
     *  @return  A reference to the data of the (key,data) %pair.
     *
     *  Allows for easy lookup with the subscript ( @c [] )
     *  operator.  Returns data associated with the key specified in
     *  subscript.  If the key does not exist, a pair with that key
     *  is created using default values, which is then returned.
     *
     *  Lookup requires logarithmic time.
     */
    mapped_type& operator[](const key_type& __k)
    {

    }

    mapped_type& operator[](key_type&& __k)
    {

    }

    /**
     *  @brief  Access to %map data.
     *  @param  __k  The key for which data should be retrieved.
     *  @return  A reference to the data whose key is equivalent to @a __k, if
     *           such a data is present in the %map.
     *  @throw  std::out_of_range  If no such data is present.
     */
    mapped_type& at(const key_type& __k)
    {

    }

    const mapped_type& at(const key_type& __k) const
    {

    }

    // modifiers
    /**
     *  @brief Attempts to build and insert a std::pair into the %map.
     *
     *  @param __args  Arguments used to generate a new pair instance (see
     *	        std::piecewise_contruct for passing arguments to each
     *	        part of the pair constructor).
     *
     *  @return  A pair, of which the first element is an iterator that points
     *           to the possibly inserted pair, and the second is a bool that
     *           is true if the pair was actually inserted.
     *
     *  This function attempts to build and insert a (key, value) %pair into
     *  the %map.
     *  A %map relies on unique keys and thus a %pair is only inserted if its
     *  first element (the key) is not already present in the %map.
     *
     *  Insertion requires logarithmic time.
     */
    template<typename... _Args>
    std::pair<iterator, bool> emplace(_Args&&... __args)
    {  }

    /**
     *  @brief Attempts to build and insert a std::pair into the %map.
     *
     *  @param  __pos  An iterator that serves as a hint as to where the pair
     *                should be inserted.
     *  @param  __args  Arguments used to generate a new pair instance (see
     *	         std::piecewise_contruct for passing arguments to each
     *	         part of the pair constructor).
     *  @return An iterator that points to the element with key of the
     *          std::pair built from @a __args (may or may not be that
     *          std::pair).
     *
     *  This function is not concerned about whether the insertion took place,
     *  and thus does not return a boolean like the single-argument emplace()
     *  does.
     *  Note that the first parameter is only a hint and can potentially
     *  improve the performance of the insertion process. A bad hint would
     *  cause no gains in efficiency.
     *
     *  See
     *  https://gcc.gnu.org/onlinedocs/libstdc++/manual/associative.html#containers.associative.insert_hints
     *  for more on @a hinting.
     *
     *  Insertion requires logarithmic time (if the hint is not taken).
     */
    template<typename... _Args>
    iterator emplace_hint(const_iterator __pos, _Args&&... __args)
    {

    }

    /**
     *  @brief Attempts to insert a std::pair into the %map.

     *  @param __x Pair to be inserted (see std::make_pair for easy
     *	     creation of pairs).
     *
     *  @return  A pair, of which the first element is an iterator that
     *           points to the possibly inserted pair, and the second is
     *           a bool that is true if the pair was actually inserted.
     *
     *  This function attempts to insert a (key, value) %pair into the %map.
     *  A %map relies on unique keys and thus a %pair is only inserted if its
     *  first element (the key) is not already present in the %map.
     *
     *  Insertion requires logarithmic time.
     */
    std::pair<iterator, bool> insert(const value_type& __x)
    {  }

    template<typename _Pair, typename = typename
    std::enable_if<std::is_constructible<value_type, _Pair&&>::value>::type>
    std::pair<iterator, bool> insert(_Pair&& __x)
    { }

    /**
     *  @brief Attempts to insert a list of std::pairs into the %map.
     *  @param  __list  A std::initializer_list<value_type> of pairs to be
     *                  inserted.
     *
     *  Complexity similar to that of the range constructor.
     */
    void insert(std::initializer_list<value_type> __list)
    {  }

    /**
     *  @brief Attempts to insert a std::pair into the %map.
     *  @param  __position  An iterator that serves as a hint as to where the
     *                    pair should be inserted.
     *  @param  __x  Pair to be inserted (see std::make_pair for easy creation
     *               of pairs).
     *  @return An iterator that points to the element with key of
     *           @a __x (may or may not be the %pair passed in).
     *

     *  This function is not concerned about whether the insertion
     *  took place, and thus does not return a boolean like the
     *  single-argument insert() does.  Note that the first
     *  parameter is only a hint and can potentially improve the
     *  performance of the insertion process.  A bad hint would
     *  cause no gains in efficiency.
     *
     *  See
     *  https://gcc.gnu.org/onlinedocs/libstdc++/manual/associative.html#containers.associative.insert_hints
     *  for more on @a hinting.
     *
     *  Insertion requires logarithmic time (if the hint is not taken).
     */
    iterator insert(const_iterator __position, const value_type& __x)
    { return _M_t._M_insert_unique_(__position, __x); }


    /**
     *  @brief Template function that attempts to insert a range of elements.
     *  @param  __first  Iterator pointing to the start of the range to be
     *                   inserted.
     *  @param  __last  Iterator pointing to the end of the range.
     *
     *  Complexity similar to that of the range constructor.
     */
    template<typename _InputIterator>
    void insert(_InputIterator __first, _InputIterator __last)
    { }

    /**
     *  @brief Erases an element from a %map.
     *  @param  __position  An iterator pointing to the element to be erased.
     *  @return An iterator pointing to the element immediately following
     *          @a position prior to the element being erased. If no such
     *          element exists, end() is returned.
     *
     *  This function erases an element, pointed to by the given
     *  iterator, from a %map.  Note that this function only erases
     *  the element, and that if the element is itself a pointer,
     *  the pointed-to memory is not touched in any way.  Managing
     *  the pointer is the user's responsibility.
     */
    iterator erase(const_iterator __position)
    { return _M_t.erase(__position); }

    iterator erase(iterator __position)
    { return _M_t.erase(__position); }

    /**
     *  @brief Erases elements according to the provided key.
     *  @param  __x  Key of element to be erased.
     *  @return  The number of elements erased.
     *
     *  This function erases all the elements located by the given key from
     *  a %map.
     *  Note that this function only erases the element, and that if
     *  the element is itself a pointer, the pointed-to memory is not touched
     *  in any way.  Managing the pointer is the user's responsibility.
     */
    size_type erase(const key_type& __x)
    { return _M_t.erase(__x); }


    // _GLIBCXX_RESOLVE_LIB_DEFECTS
    // DR 130. Associative erase should return an iterator.
    /**
     *  @brief Erases a [first,last) range of elements from a %map.
     *  @param  __first  Iterator pointing to the start of the range to be
     *                   erased.
     *  @param __last Iterator pointing to the end of the range to
     *                be erased.
     *  @return The iterator @a __last.
     *
     *  This function erases a sequence of elements from a %map.
     *  Note that this function only erases the element, and that if
     *  the element is itself a pointer, the pointed-to memory is not touched
     *  in any way.  Managing the pointer is the user's responsibility.
     */
    iterator erase(const_iterator __first, const_iterator __last)
    { return _M_t.erase(__first, __last); }

    /**
     *  Erases all elements in a %map.  Note that this function only
     *  erases the elements, and that if the elements themselves are
     *  pointers, the pointed-to memory is not touched in any way.
     *  Managing the pointer is the user's responsibility.
     */
    void clear()
    { _M_t.clear(); }

    // observers
    /**
     *  Returns the key comparison object out of which the %map was
     *  constructed.
     */
    key_compare key_comp() const
    { return _M_t.key_comp(); }

    /**
     *  Returns a value comparison object, built from the key comparison
     *  object out of which the %map was constructed.
     */
    value_compare  value_comp() const
    { return value_compare(_M_t.key_comp()); }

    /**
     *  @brief  Finds the number of elements with given key.
     *  @param  __x  Key of (key, value) pairs to be located.
     *  @return  Number of elements with specified key.
     *
     *  This function only makes sense for multimaps; for map the result will
     *  either be 0 (not present) or 1 (present).
     */
    size_type  count(const key_type& __x) const
    {  }

    /**
     *  @brief Finds the beginning of a subsequence matching given key.
     *  @param  __x  Key of (key, value) pair to be located.
     *  @return  Iterator pointing to first element equal to or greater
     *           than key, or end().
     *
     *  This function returns the first element of a subsequence of elements
     *  that matches the given key.  If unsuccessful it returns an iterator
     *  pointing to the first element that has a greater value than given key
     *  or end() if no such element exists.
     */
    iterator lower_bound(const key_type& __x)
    { }

    /**
     *  @brief Finds the beginning of a subsequence matching given key.
     *  @param  __x  Key of (key, value) pair to be located.
     *  @return  Read-only (constant) iterator pointing to first element
     *           equal to or greater than key, or end().
     *
     *  This function returns the first element of a subsequence of elements
     *  that matches the given key.  If unsuccessful it returns an iterator
     *  pointing to the first element that has a greater value than given key
     *  or end() if no such element exists.
     */
    const_iterator lower_bound(const key_type& __x) const
    { return _M_t.lower_bound(__x); }

    /**
     *  @brief Finds the end of a subsequence matching given key.
     *  @param  __x  Key of (key, value) pair to be located.
     *  @return Iterator pointing to the first element
     *          greater than key, or end().
     */
    iterator upper_bound(const key_type& __x)
    {  }

    /**
     *  @brief Finds the end of a subsequence matching given key.
     *  @param  __x  Key of (key, value) pair to be located.
     *  @return  Read-only (constant) iterator pointing to first iterator
     *           greater than key, or end().
     */
    const_iterator upper_bound(const key_type& __x) const
    { return _M_t.upper_bound(__x); }

    /**
     *  @brief Finds a subsequence matching given key.
     *  @param  __x  Key of (key, value) pairs to be located.
     *  @return  Pair of iterators that possibly points to the subsequence
     *           matching given key.
     *
     *  This function is equivalent to
     *  @code
     *    std::make_pair(c.lower_bound(val),
     *                   c.upper_bound(val))
     *  @endcode
     *  (but is faster than making the calls separately).
     *
     *  This function probably only makes sense for multimaps.
     */
    std::pair<iterator, iterator> equal_range(const key_type& __x)
    { return _M_t.equal_range(__x); }

    template<typename _K1, typename _T1, typename _C1, typename _A1>
      friend bool
      operator==(const map<_K1, _T1, _C1, _A1>&,
     const map<_K1, _T1, _C1, _A1>&);

    template<typename _K1, typename _T1, typename _C1, typename _A1>
      friend bool
      operator<(const map<_K1, _T1, _C1, _A1>&,
    const map<_K1, _T1, _C1, _A1>&);
  };

/**
 *  @brief  Map equality comparison.
 *  @param  __x  A %map.
 *  @param  __y  A %map of the same type as @a x.
 *  @return  True iff the size and elements of the maps are equal.
 *
 *  This is an equivalence relation.  It is linear in the size of the
 *  maps.  Maps are considered equivalent if their sizes are equal,
 *  and if corresponding elements compare equal.
*/
template<typename _Key, typename _Tp, typename _Compare, typename _Alloc>
  inline bool
  operator==(const map<_Key, _Tp, _Compare, _Alloc>& __x,
             const map<_Key, _Tp, _Compare, _Alloc>& __y)
  { return __x._M_t == __y._M_t; }

/**
 *  @brief  Map ordering relation.
 *  @param  __x  A %map.
 *  @param  __y  A %map of the same type as @a x.
 *  @return  True iff @a x is lexicographically less than @a y.
 *
 *  This is a total ordering relation.  It is linear in the size of the
 *  maps.  The elements must be comparable with @c <.
 *
 *  See std::lexicographical_compare() for how the determination is made.
*/
template<typename _Key, typename _Tp, typename _Compare, typename _Alloc>
  inline bool
  operator<(const map<_Key, _Tp, _Compare, _Alloc>& __x,
            const map<_Key, _Tp, _Compare, _Alloc>& __y)
  { return __x._M_t < __y._M_t; }

/// Based on operator==
template<typename _Key, typename _Tp, typename _Compare, typename _Alloc>
  inline bool
  operator!=(const map<_Key, _Tp, _Compare, _Alloc>& __x,
             const map<_Key, _Tp, _Compare, _Alloc>& __y)
  { return !(__x == __y); }

/// Based on operator<
template<typename _Key, typename _Tp, typename _Compare, typename _Alloc>
  inline bool
  operator>(const map<_Key, _Tp, _Compare, _Alloc>& __x,
            const map<_Key, _Tp, _Compare, _Alloc>& __y)
  { return __y < __x; }

/// Based on operator<
template<typename _Key, typename _Tp, typename _Compare, typename _Alloc>
  inline bool
  operator<=(const map<_Key, _Tp, _Compare, _Alloc>& __x,
             const map<_Key, _Tp, _Compare, _Alloc>& __y)
  { return !(__y < __x); }

/// Based on operator<
template<typename _Key, typename _Tp, typename _Compare, typename _Alloc>
  inline bool
  operator>=(const map<_Key, _Tp, _Compare, _Alloc>& __x,
             const map<_Key, _Tp, _Compare, _Alloc>& __y)
  { return !(__x < __y); }

/// See std::map::swap().
template<typename _Key, typename _Tp, typename _Compare, typename _Alloc>
  inline void
  swap(map<_Key, _Tp, _Compare, _Alloc>& __x,
 map<_Key, _Tp, _Compare, _Alloc>& __y)
  { __x.swap(__y); }

} // namespace std

#endif // MAP_HPP

