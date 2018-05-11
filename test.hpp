#ifndef TEST_HPP
#define TEST_HPP

#include <future>
#include <chrono>

template <typename T>
struct test
{
  std::vector< std::future<bool> > tasks;

  test(T& m, size_t thn, size_t s) : tasks(thn)
  {
    size_t i = 0;
    for (auto& it: tasks) {
      it = std::async(&test::insertion, this, std::ref(m), std::string("task"), (i++)*s, s);
    }
  }

  bool insertion(T& m, const std::string& name, size_t offset, size_t n)
  {
    for (size_t i = offset; i < (n+offset); ++i) {
      std::string k = name;
      k+= std::to_string(i);
      m[k] = i;
    }

    return true;
  }

  ~test()
  {
    for (auto& it: tasks)
      it.get();
  }
};

template<typename T>
void TestInsert(T& m)
{
  using namespace std::chrono;

  system_clock::time_point tp1 = system_clock::now();
  {
    test<T>(m, 10, 100000);
  }
  system_clock::time_point tp2 = system_clock::now();
  std::cout << "insertion test" << std::endl;
  std::cout << "elements : " << m.size() << std::endl;
  std::cout << "Duration: " << duration_cast<milliseconds>(tp2-tp1).count() << " milliseconds" << std::endl;
}

#endif // TEST_HPP
