
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CONTEXTS_CONTEXT_H
#define BOOST_CONTEXTS_CONTEXT_H

#include <cstddef>
#include <cstring>
#include <new>

#include <boost/config.hpp>
#include <boost/assert.hpp>
#include <boost/move/move.hpp>
#include <boost/utility.hpp>

#include <boost/context/detail/config.hpp>
#include <boost/context/protected_stack.hpp>
extern "C" {
#include <boost/context/detail/libtask/context.h>
}

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
        ctx(), st_()
    {
        std::memset( & ctx, 0, sizeof( ctx) );
        if(::getcontext(&ctx) < 0){
            fprintf(stderr, "getcontext: \n");
            abort();
        }
    }

    context( void( fn)( void *), void * vp, BOOST_RV_REF( stack_type) st) :
        ctx(), st_( boost::move( st) )
    {
        BOOST_ASSERT( st_);
        init( fn, NULL, vp );
    }

    context( void( fn)( void *), context & nxt, void * vp,
            BOOST_RV_REF( stack_type) st) :
        ctx(), st_( boost::move( st) )
    {
        init( fn, &nxt, vp );
    }

    void jump_to( context & other)
    { ::swapcontext( & ctx, & other.ctx); }

private:

    static void taskstart(uint32_t y, uint32_t x)
    {
        uint64_t z;

        z = x<<16;	/* hide undefined 32-bit shift from 32-bit compilers */
        z <<= 16;
        z |= y;
        context* self = (context*)(z);

        self->m_fn(self->m_arg);

        if( !self->ctx.uc_link )
            exit(1);
        ::swapcontext( & self->ctx, self->ctx.uc_link );
    }


    void init( void( fn)( void *), context* nxt, void * vp )
    {
        BOOST_ASSERT( st_);
        
        m_fn  = fn;
        m_arg = vp;

		std::memset( & ctx, 0, sizeof( ctx) );

	    sigset_t zero;
        sigemptyset(&zero);
        sigprocmask(SIG_BLOCK, &zero, &ctx.uc_sigmask);

        /* must initialize with current context */
        if(::getcontext(&ctx) < 0){
            fprintf(stderr, "getcontext: \n");
            abort();
        }

        ctx.uc_stack.ss_size = st_.size();
        void * base = st_.address();
        if ( ! base) throw std::bad_alloc();
        ctx.uc_stack.ss_sp = static_cast< char * >( base) - st_.size();

		ctx.uc_link = nxt ? &nxt->ctx : 0;

        /*
         * All this magic is because you have to pass makecontext a
         * function that takes some number of word-sized variables,
         * and on 64-bit machines pointers are bigger than words.
         */
        uint64_t z = (uint64_t)this;
        uint32_t y = z;
        z >>= 16;	/* hide undefined 32-bit shift from 32-bit compilers */
        uint32_t x = z>>16;


        ::makecontext(&ctx, (void(*)())taskstart, 2, y, x);
    }
    void (*m_fn)( void *);
    void* m_arg;
    ucontext_t      ctx;
    stack_type      st_;
};


}

using contexts::context;

}

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif

#endif // BOOST_CONTEXTS_CONTEXT_H
