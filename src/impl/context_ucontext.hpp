
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CONTEXTS_CONTEXT_H
#define BOOST_CONTEXTS_CONTEXT_H

extern "C" {
#include <ucontext.h>
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

template< typename StackT = protected_stack >
class context : private noncopyable
{
public:
    typedef StackT      stack_type;

    context() :
        ctx_(), st_()
    {}

    context( void( fn)( void *), void * vp, BOOST_RV_REF( stack_type) st) :
        ctx_(), st_( boost::move( st) )
    {
        BOOST_ASSERT( st_);

        std::memset( & ctx_, 0, sizeof( ctx_) );
        ::getcontext( & ctx_);

        ctx_.uc_stack.ss_size = st_.size();
        void * base = st_.address();
        if ( ! base) throw std::bad_alloc();
        ctx_.uc_stack.ss_sp = static_cast< char * >( base) - st_.size();

        typedef void fn_type( void *);
        typedef void ( * st_fn)();
        fn_type * fn_ptr( fn);
        ::makecontext( & ctx_, ( st_fn)( fn_ptr), 1, vp);
    }

    context( void( fn)( void *), context & nxt, void * vp,
            BOOST_RV_REF( stack_type) st) :
        ctx_(), st_( boost::move( st) )
    {
        BOOST_ASSERT( st_);

        std::memset( & ctx_, 0, sizeof( ctx_) );
        ::getcontext( & ctx_);

        ctx_.uc_stack.ss_size = st_.size();
        void * base = st_.address();
        if ( ! base) throw std::bad_alloc();
        ctx_.uc_stack.ss_sp = static_cast< char * >( base) - st_.size();

        typedef void fn_type( void *);
        typedef void ( * st_fn)();
        fn_type * fn_ptr( fn);
        ctx_.uc_link = & nxt.ctx_;
        ::makecontext( & ctx_, ( st_fn)( fn_ptr), 1, vp);
    }

    void jump_to( context & other)
    { ::swapcontext( & ctx_, & other.ctx_); }

private:
    ucontext_t      ctx_;
    stack_type      st_;
};

}

using contexts::context;

}

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif

#endif // BOOST_CONTEXTS_CONTEXT_H
