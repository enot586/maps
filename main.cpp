
#include <iostream>
#include <string>
#include <map>

#include "map1.hpp"
#include "map2.hpp"
#include "map3.hpp"
#include "test.hpp"


int main( int /*argc*/, char*[]/*argv[]*/ )
{
  std::cout << "t1::map" << std::endl;
  t1::map<std::string, size_t> m1;
  TestInsert(m1);

  std::cout << std::endl;

  std::cout << "std::map" << std::endl;
  std::map<std::string, size_t> m2;
  TestInsert(m2);

  std::cout << std::endl;

  std::cout << "std::map<threadsafe_adapter>" << std::endl;
  t3::map<std::string, size_t> m3;
  TestInsert(m3);


  std::cout << "****************************************" << std::endl;

  return  0;
}


