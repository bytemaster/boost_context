
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CONTEXTS_CONTEXT_H
#define BOOST_CONTEXTS_CONTEXT_H

extern "C" {
#include <windows.h>
}

#include <cstddef>
#include <cstring>
#include <new>

#include <boost/config.hpp>
#include <boost/assert.hpp>
#include <boost/move/move.hpp>
#include <boost/utility.hpp>

#include <boost/context/detail/config.hpp>
#include <boost/context/protected_stack.hpp>

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_PREFIX
#endif

namespace boost {
namespace contexts {
namespace detail {

//#if (_WIN32_WINNT >= _WIN32_WINNT_VISTA)
//  inline
//  bool is_a_fiber()
//  { return TRUE == ::IsThreadAFiber(); }
//#else
inline
bool is_a_fiber()
{
    LPVOID current( ::GetCurrentFiber() );
    return 0 != current && reinterpret_cast< LPVOID >( 0x1E00) != current;
}
//#endif

template< typename CTX >
void CALLBACK trampoline( LPVOID vp)
{
    BOOST_ASSERT( vp);

    CTX * ctx( static_cast< CTX * >( vp) );
    ctx->fn_( ctx->vp_);
    if ( ctx->nxt_) ::SwitchToFiber( ctx->nxt_->hndl_);
}

}

template< typename StackT = protected_stack >
class context : private noncopyable
{
public:
    typedef StackT      stack_type;

    context() :
        hndl_( 0), fn_( 0), vp_( 0), nxt_( 0), st_()
    {}

    ~context()
    { if ( hndl_) ::DeleteFiber( hndl_); }

    context( void( fn)( void *), void * vp, BOOST_RV_REF( stack_type) st) :
        hndl_( 0), fn_( fn), vp_( vp), nxt_( 0), st_( boost::move( st) )
    {
        BOOST_ASSERT( st_);

        hndl_ = ::CreateFiber(
            st_.size(),
            detail::trampoline< context >,
            static_cast< LPVOID >( this) );
        BOOST_ASSERT( hndl_);
    }

    context( void( fn)( void *), context & nxt, void * vp,
            BOOST_RV_REF( stack_type) st) :
        hndl_( 0), fn_( fn), vp_( vp), nxt_( & nxt), st_( boost::move( st) )
    {
        BOOST_ASSERT( st_);

        hndl_ = ::CreateFiber(
            st_.size(),
            detail::trampoline< context >,
            static_cast< LPVOID >( this) );
        BOOST_ASSERT( hndl_);
    }

    void jump_to( context & other)
    {
        if ( ! detail::is_a_fiber() )
        {
            hndl_ = ::ConvertThreadToFiber( 0);
            BOOST_ASSERT( hndl_);

            ::SwitchToFiber( other.hndl_);

            BOOL result = ::ConvertFiberToThread();
            BOOST_ASSERT( result);
            hndl_ = 0;
        }
        else
        {
            bool is_main = 0 == hndl_;
            if ( is_main) hndl_ = ::GetCurrentFiber();

            ::SwitchToFiber( other.hndl_);

            if ( is_main) hndl_ = 0;
        }
    }

private:
    template< typename CTX >
    friend void CALLBACK detail::trampoline( LPVOID vp);

    typedef void fn_t( void *);

    LPVOID              hndl_;
    fn_t            *   fn_;
    void            *   vp_;
    context         *   nxt_;
    stack_type          st_;
};

}

using contexts::context;

}

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif

#endif // BOOST_CONTEXTS_CONTEXT_H
