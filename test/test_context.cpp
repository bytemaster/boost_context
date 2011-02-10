
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <sstream>
#include <stdexcept>
#include <string>

#include <boost/assert.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/utility.hpp>

#include <boost/context/all.hpp>

int value1 = 0;
std::string value2;

void fn1( void *) {}

void fn2( void * vp)
{ value1 = * ( ( int *) vp); }

void fn3( void * vp)
{
    try
    { throw std::runtime_error("abc"); }
    catch ( std::runtime_error const& e)
    { value2 = e.what(); }
}

void test_case_1()
{
    boost::protected_stack stack( 65536);
    boost::context<> c1;
    int i = 1;
    BOOST_CHECK_EQUAL( 0, value1);
    boost::context<> c2( fn2, c1, & i, boost::move( stack) );
    c1.jump_to( c2);
    BOOST_CHECK_EQUAL( 1, value1);
}

void test_case_2()
{
    boost::protected_stack stack( 65536);
    boost::context<> c1;
    BOOST_CHECK_EQUAL( std::string(""), value2);
    boost::context<> c2( fn3, c1, 0, boost::move( stack) );
    c1.jump_to( c2);
    BOOST_CHECK_EQUAL( std::string("abc"), value2);
}

boost::unit_test::test_suite * init_unit_test_suite( int, char* [])
{
    boost::unit_test::test_suite * test =
        BOOST_TEST_SUITE("Boost.Context: context test suite");

    test->add( BOOST_TEST_CASE( & test_case_1) );
    test->add( BOOST_TEST_CASE( & test_case_2) );

    return test;
}
