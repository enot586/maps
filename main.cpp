
#include <iostream>
#include <string>
#include <map>
#include <unordered_map>

#include "map1.hpp"
#include "map2.hpp"
#include "map3.hpp"
#include "test.hpp"


int main( int /*argc*/, char*[]/*argv[]*/ )
{
  test_insert test_multithread_insert(100);

  std::cout << "std::map(without synchronization)" << std::endl;
  std::map<std::string, size_t> m1;
  run_test(test_multithread_insert, m1, 10000);
  std::cout << std::endl;

  std::cout << "t1::map" << std::endl;
  t1::map<std::string, size_t> m2;
  run_test(test_multithread_insert, m2, 10000);
  std::cout << std::endl;

  std::cout << "t3::map" << std::endl;
  t3::map<std::string, size_t> m3;
  run_test(test_multithread_insert, m3, 10000);
  std::cout << "****************************************" << std::endl;

  test_access test_multithread_access(100);

  std::cout << "std::map(without synchronization)" << std::endl;
  run_test(test_multithread_access, m1, 0xFF);
  std::cout << std::endl;

//  std::cout << "t1::map" << std::endl;
//  run_test(test_multithread_access, m2, 0xFF);
//  std::cout << std::endl;

  std::cout << "t3::map" << std::endl;
  run_test(test_multithread_access, m3, 0xFF);
  std::cout << "****************************************" << std::endl;


  return  0;
}


