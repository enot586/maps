#define BOOST_TEST_MODULE MapTest
#define TEST_BOOST_MAIN

#include <boost/test/unit_test.hpp>
#include <iostream>
#include <string>
#include <map>
#include <algorithm>

#include "map1.hpp"
#include "map3.hpp"

using namespace std;

template<typename _Key, typename _Value, typename _Map>
void test_insert_erase(_Key k, _Value v, _Map& m)
{
  m[k] = v;
  BOOST_CHECK( m[k] == v );
  BOOST_CHECK( m.size() == 1 );

  m.erase(k);
  BOOST_CHECK( m.size() == 0 );
}

BOOST_AUTO_TEST_CASE(MapSimpleInsertErase)
{
  {
    t1::map<string, string> m;
    test_insert_erase( string("new_key"), string("new_value"), m);

    t1::map<int, int> m1;
    test_insert_erase(5, 6, m1);

    t1::map<double, double> m2;
    test_insert_erase(.5, .6, m2);
  }

  {
    t3::map<string, string> m;
    test_insert_erase( string("new_key"), string("new_value"), m);

    t3::map<int, int> m1;
    test_insert_erase(5, 6, m1);

    t3::map<double, double> m2;
    test_insert_erase(.5, .6, m2);
  }
}

template<typename _Map>
void test_iterators(_Map& m)
{
  std::vector<string> varr{ "v1", "v2", "v3" };
  std::vector<string> karr{ "value1", "value2", "value3" };

  m[ karr[0] ] = varr[0];
  m[ karr[1] ] = varr[1];
  m[ karr[2] ] = varr[2];

  for (auto& it : karr) {
    BOOST_CHECK( m.find(it) != m.end() );
  }
}

BOOST_AUTO_TEST_CASE(MapSimpleIterator)
{
  {
    t1::map<string, string> m;
    test_iterators(m);
  }

  {
    t3::map<string, string> m;
    test_iterators(m);
  }

}
