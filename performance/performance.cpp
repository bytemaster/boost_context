
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include <boost/assert.hpp>
#include <boost/context/all.hpp>
#include <boost/program_options.hpp>

#include "bind_processor.hpp"
#include "performance.hpp"

namespace po = boost::program_options;

volatile int value = 0;

void fn( void * vp)
{ value = * ( ( int *) vp); }

void test_creation( unsigned int iterations)
{
    int value( 3);
    cycle_t total( 0);
    cycle_t overhead( get_overhead() );
    std::cout << "overhead for rdtsc == " << overhead << " cycles" << std::endl;

    // cache warm-up
    {
        boost::protected_stack stack( 65536);
        boost::context<> c1;
        boost::context<> c2( fn, c1, & value, boost::move( stack) );
    }

    for ( unsigned int i = 0; i < iterations; ++i)
    {
        boost::protected_stack stack( 65536);
        boost::context<> c1;
        cycle_t start( get_cycles() );
        boost::context<> c2( fn, c1, & value, boost::move( stack) );
        cycle_t diff( get_cycles() - start);
        diff -= overhead;
        BOOST_ASSERT( diff >= 0);
        total += diff;
    }
    std::cout << "average of " << total/iterations << " cycles per creation" << std::endl;
}

void test_switching( unsigned int iterations)
{
    int value( 3);
    long total( 0);
    cycle_t overhead( get_overhead() );
    std::cout << "overhead for rdtsc == " << overhead << " cycles" << std::endl;

    // cache warum-up
    {
        boost::protected_stack stack( 65536);
        boost::context<> c1;
        boost::context<> c2( fn, c1, & value, boost::move( stack) );
        c1.jump_to( c2);
    }

    for ( unsigned int i = 0; i < iterations; ++i)
    {
        boost::protected_stack stack( 65536);
        boost::context<> c1;
        boost::context<> c2( fn, c1, & value, boost::move( stack) );
        cycle_t start( get_cycles() );
        c1.jump_to( c2);
        cycle_t diff( get_cycles() - start);
        diff -= overhead;
        BOOST_ASSERT( diff >= 0);
        total += diff;
    }
    std::cout << "average of " << total/iterations << " cycles per switch" << std::endl;
}

int main( int argc, char * argv[])
{
    try
    {
        unsigned int iterations( 0);

        po::options_description desc("allowed options");
        desc.add_options()
            ("help,h", "help message")
            ("creating,c", "test creation")
            ("switching,s", "test switching")
            ("iterations,i", po::value< unsigned int >( & iterations), "iterations");

        po::variables_map vm;
        po::store(
            po::parse_command_line(
                argc,
                argv,
                desc),
            vm);
        po::notify( vm);

        if ( vm.count("help") )
        {
            std::cout << desc << std::endl;
            return EXIT_SUCCESS;
        }

        if ( 0 == iterations) throw std::invalid_argument("iterations must be greater than zero");

        bind_to_processor( 0);

        if ( vm.count("creating") )
            test_creation( iterations);

        if ( vm.count("switching") )
            test_switching( iterations);

        return EXIT_SUCCESS;
    }
    catch ( std::exception const& e)
    { std::cerr << "exception: " << e.what() << std::endl; }
    catch (...)
    { std::cerr << "unhandled exception" << std::endl; }
    return EXIT_FAILURE;
}
