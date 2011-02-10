#include <cstdlib>
#include <iostream>
#include <string>

#include <boost/bind.hpp>

#include <boost/context/all.hpp>

int x = 7;
void fn( void * vp);

boost::protected_stack stack( 65536);
boost::context<> ctx1;
boost::context<> ctx2( fn, & x, boost::move( stack) );

void fn( void * vp)
{
    int i = * ( int *) vp;
    for(;;)
    {
        std::cout << ++i << std::endl;
        ctx2.jump_to( ctx1);
    }
}

int main()
{
    try
    {

        std::cout << "start" << std::endl;

        for ( int i = 0; i < 5; ++i)
        {
            ctx1.jump_to( ctx2);
        }

        std::cout << "finish" << std::endl;

        return EXIT_SUCCESS;
    }
    catch ( std::exception const& e)
    { std::cerr << "exception: " << e.what() << std::endl; }
    catch (...)
    { std::cerr << "unhandled exception" << std::endl; }
    return EXIT_FAILURE;
}
