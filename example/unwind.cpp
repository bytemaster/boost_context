
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cstdlib>
#include <iostream>
#include <string>

#include <boost/context/all.hpp>

boost::contexts::context<> ctx;

struct X
{
    X()
    { std::cout << "X()" << std::endl; }

    ~X()
    { std::cout << "~X()" << std::endl; }
};

void fn()
{
    X x;

    for( int i = 0;; ++i)
    {
        std::cout << "fn(): " << i << std::endl;
        ctx.suspend();
    }
}

int main( int argc, char * argv[])
{
    ctx = boost::contexts::context<>(
            fn,
            boost::contexts::protected_stack( boost::contexts::stack_helper::default_stacksize()),
            false, true);
    for ( int i = 0; i < 5; ++i)
    {
        ctx.resume();
    }

    std::cout << "ctx is complete: " << std::boolalpha << ctx.is_complete() << "\n";
    std::cout << "call ctx.unwind_stack()" << std::endl;
    ctx.unwind_stack();
    std::cout << "ctx is complete: " << std::boolalpha << ctx.is_complete() << "\n";

    std::cout << "Done" << std::endl;

    return EXIT_SUCCESS;
}
