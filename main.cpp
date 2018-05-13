
#include <iostream>
#include <string>
#include <map>

#include "map1.hpp"
#include "map3.hpp"
#include "test.hpp"


int main( int /*argc*/, char*[]/*argv[]*/ )
{
  static const size_t NUMBER_OF_THREADS      = 100;
  static const size_t NUMBER_OF_MAP_ELEMENTS = 10000;

  test_insert test_multithreading_insert(NUMBER_OF_THREADS);

  std::cout << "std::map(without synchronization)" << std::endl;
  std::map<std::string, size_t> std_m;
  run_test(test_multithreading_insert, std_m, NUMBER_OF_MAP_ELEMENTS);
  std::cout << std::endl;

  std::cout << "t1::map" << std::endl;
  t1::map<std::string, size_t> t1_m;
  run_test(test_multithreading_insert, t1_m, NUMBER_OF_MAP_ELEMENTS);
  std::cout << std::endl;

  std::cout << "t3::map" << std::endl;
  t3::map<std::string, size_t> t3_m;
  run_test(test_multithreading_insert, t3_m, NUMBER_OF_MAP_ELEMENTS);
  std::cout << "****************************************" << std::endl;

  static const size_t VALUE_TO_SET = 0xFF;
  test_access test_multithreading_access(NUMBER_OF_THREADS);

  std::cout << "std::map(without synchronization)" << std::endl;
  run_test(test_multithreading_access, std_m, VALUE_TO_SET);
  std::cout << std::endl;

  std::cout << "t1::map" << std::endl;
  run_test(test_multithreading_access, t1_m, VALUE_TO_SET);
  std::cout << std::endl;

  std::cout << "t3::map" << std::endl;
  run_test(test_multithreading_access, t3_m, VALUE_TO_SET);
  std::cout << "****************************************" << std::endl;

  test_access_erase test_multithreading_access_erase(NUMBER_OF_THREADS);

  std::cout << "std::map(without synchronization)" << std::endl;
  run_test(test_multithreading_access_erase, std_m, 0);
  std::cout << std::endl;

  std::cout << "t1::map" << std::endl;
  run_test(test_multithreading_access_erase, t1_m, 0);
  std::cout << std::endl;

  std::cout << "t3::map" << std::endl;
  run_test(test_multithreading_access_erase, t3_m, 0);
  std::cout << "****************************************" << std::endl;

  return  0;
}


