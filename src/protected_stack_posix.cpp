
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_CONTEXT_SOURCE

#include <boost/context/protected_stack.hpp>

extern "C" {
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
}

#include <cmath>
#include <stdexcept>

#include <boost/config.hpp>
#include <boost/assert.hpp>

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_PREFIX
#endif

namespace {

struct helper
{
    static std::size_t get_pagesize()
    {
        static std::size_t pagesize( ::getpagesize() );
        return pagesize;
    }

    static std::size_t calc_pages( std::size_t stacksize)
    {
        double x( stacksize / get_pagesize() );
        if ( 0 != stacksize % get_pagesize() )
            x += 0.5;
        std::size_t tmp( ( x - std::floor( x) >= 0.5 )
            ? static_cast< std::size_t >( std::ceil( x) )
            : static_cast< std::size_t >( std::floor( x) ) );
        return tmp;
    }
};

}

namespace boost {
namespace contexts {

protected_stack::protected_stack() :
    size_( 0), size__( 0), address_( 0)
{}

protected_stack::protected_stack( std::size_t size) :
    size_( size), size__( 0), address_( 0)
{
    if ( 16 >= size_)
        throw std::invalid_argument("invalid stack size");

    std::size_t pages( helper::calc_pages( size_) );
    ++pages; // add guard page
    size__ = pages * helper::get_pagesize();

    //int fd( ::open("/dev/zero", O_RDONLY) );
    int fd( ::open("bigfile", O_RDONLY) );
    BOOST_ASSERT( -1 != fd);
    void * limit =
        ::mmap( 0, size__, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if( limit == MAP_FAILED )
        perror( "mmap fail" );

    ::close( fd);
    if ( ! limit) throw std::bad_alloc();

    fprintf( stderr, "limit: %p  pagesize %p  ", limit, helper::get_pagesize() );
    int result( ::mprotect( limit, helper::get_pagesize(), PROT_NONE) );
    if( result != 0 ) perror( "mprotect" );
    BOOST_ASSERT( 0 == result);
    address_ = static_cast< char * >( limit) + size__;
}

protected_stack::~protected_stack()
{
    if ( address_)
    {
        BOOST_ASSERT( 0 < size_ && 0 < size__);
        void * limit = static_cast< char * >( address_) - size__;
        ::munmap( limit, size__);
    }
}

protected_stack::protected_stack( BOOST_RV_REF( protected_stack) other) :
    size_( 0), size__( 0), address_( 0)
{ swap( other); }

protected_stack &
protected_stack::operator=( BOOST_RV_REF( protected_stack) other)
{
    protected_stack tmp( boost::move( other) );
    swap( tmp);
    return * this;
}

void *
protected_stack::address() const
{ return address_; }

std::size_t
protected_stack::size() const
{ return size_; }

void
protected_stack::swap( protected_stack & other)
{
    std::swap( size_, other.size_);
    std::swap( size__, other.size__);
    std::swap( address_, other.address_);
}

bool
protected_stack::operator!() const
{ return 0 == address_; }

protected_stack::operator unspecified_bool_type() const
{ return 0 == address_ ? 0 : unspecified_bool; }

}}

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif
