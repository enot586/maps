
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
  test_insert test_multithreading_insert(100);

  std::cout << "std::map(without synchronization)" << std::endl;
  std::map<std::string, size_t> std_m;
  run_test(test_multithreading_insert, std_m, 1000);
  std::cout << std::endl;

  std::cout << "t1::map" << std::endl;
  t1::map<std::string, size_t> t1_m;
  run_test(test_multithreading_insert, t1_m, 1000);
  std::cout << std::endl;

  std::cout << "t3::map" << std::endl;
  t3::map<std::string, size_t> t3_m;
  run_test(test_multithreading_insert, t3_m, 1000);
  std::cout << "****************************************" << std::endl;

  test_access test_multithreading_access(100);

  std::cout << "std::map(without synchronization)" << std::endl;
  run_test(test_multithreading_access, std_m, 0xFF);
  std::cout << std::endl;

  std::cout << "t1::map" << std::endl;
  run_test(test_multithreading_access, t1_m, 0xFF);
  std::cout << std::endl;

  std::cout << "t3::map" << std::endl;
  run_test(test_multithreading_access, t3_m, 0xFF);
  std::cout << "****************************************" << std::endl;


  return  0;
}


