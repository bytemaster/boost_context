
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cstdlib>
#include <iostream>
#include <string>

#include <boost/context/all.hpp>

void fn();
void fn2();
boost::contexts::protected_stack stack( boost::contexts::stack_helper::default_stacksize());
boost::contexts::context<> ctx( fn, boost::interprocess::move( stack), false, true);
boost::contexts::protected_stack stack2( boost::contexts::stack_helper::default_stacksize());
boost::contexts::context<> ctx2( fn2, boost::interprocess::move( stack2), false, true);

void fn2() {
    std::cout << "inside function fn2(): fn2() returns return to fn()" << std::endl;
 //   ctx.resume();
    ctx2.suspend();
    std::cout << "finish fn2(), returning back to fn\n";
}

void fn()
{
    std::cout << "inside function fn(), calling fn2\n";
    ctx2.resume();
    std::cout << "back in fn(), let fn2() complete()\n";
    ctx2.resume();
    std::cout << "fn() returns return to main()" << std::endl;
}

int main( int argc, char * argv[])
{
    {
        ctx.resume();
    }

    std::cout << "Done" << std::endl;

    return EXIT_SUCCESS;
}
