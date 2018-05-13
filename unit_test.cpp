#define BOOST_TEST_MODULE MapTest
#define TEST_BOOST_MAIN

#include <boost/test/unit_test.hpp>
#include <iostream>
#include <string>

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

BOOST_AUTO_TEST_CASE(MapSimpleIterator)
{


}
