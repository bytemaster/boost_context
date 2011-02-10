#include <cstdlib>
#include <iostream>
#include <string>

#include <boost/bind.hpp>

#include <boost/context/all.hpp>


void fn( void * vp)
{
    int i = * ( int *) vp;
    std::cout << "i == " << i << std::endl;
}

int main()
{
    try
    {
        int x = 7;

        boost::protected_stack stack( 65536);
        boost::context<> ctx1;
        boost::context<> ctx2( fn, ctx1, & x, boost::move( stack) );

        std::cout << "start" << std::endl;

        ctx1.jump_to( ctx2);

        std::cout << "finish" << std::endl;

        return EXIT_SUCCESS;
    }
    catch ( std::exception const& e)
    { std::cerr << "exception: " << e.what() << std::endl; }
    catch (...)
    { std::cerr << "unhandled exception" << std::endl; }
    return EXIT_FAILURE;
}
