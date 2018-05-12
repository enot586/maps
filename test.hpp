#ifndef TEST_HPP
#define TEST_HPP

#include <future>
#include <chrono>

struct test_insert
{
  std::vector< std::future<bool> > tasks;

  test_insert( size_t thn ) : tasks(thn)
  {  }

  ~test_insert() = default;

  std::string caption()
  { return "Test insertion"; }

  template <typename T>
  void run(T& m, size_t s)
  {
    size_t i = 0;
    for (auto& it: tasks) {
      it = std::async(&test_insert::insertion<T>, this, std::ref(m), std::string("task"), (i++)*s, s);
    }

    for (auto& it: tasks)
      it.get();
  }

  template <typename T>
  bool insertion(T& m, const std::string& name, size_t offset, size_t n)
  {
    for (size_t i = offset; i < (n+offset); ++i) {
      std::string k = name;
      k+= std::to_string(i);
      m[k] = i;
    }
    return true;
  }
};

struct test_access
{
  std::vector< std::future<bool> > tasks;

  test_access( size_t thn ) : tasks(thn)
  {  }

  ~test_access() = default;

  std::string caption()
  { return "Test access"; }

  template <typename T>
  void run(T& m, size_t v)
  {
    for (auto& it: tasks) {
      it = std::async( &test_access::access<T>, this, std::ref(m), v );
    }

    for (auto& it: tasks)
      it.get();
  }

  template <typename T>
  bool access(T& m, size_t v)
  {
    for (auto& it : m) {
      it.second = v;
    }
    return true;
  }
};

template< typename test_type,
          typename container_type,
          typename... Args>
void run_test(test_type& test, container_type& m, Args... args)
{
  using namespace std::chrono;

  system_clock::time_point tp1 = system_clock::now();

  {
    test.run(m, args...);
  }

  system_clock::time_point tp2 = system_clock::now();
  std::cout << "members : " << m.size() << std::endl;
  std::cout << test.caption() << " duration: " << duration_cast<milliseconds>(tp2-tp1).count() << " milliseconds" << std::endl;
}

#endif // TEST_HPP
