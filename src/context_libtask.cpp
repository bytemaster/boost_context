
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_CONTEXT_SOURCE

#include <boost/context/context.hpp>

#include <cstring>
#include <new>

#include <boost/config.hpp>
#include <boost/assert.hpp>
#include <iostream>

#include <boost/context/exceptions.hpp>

extern "C" {
#include "libtask/context.h"
}


namespace boost {
namespace contexts {

std::size_t
context::default_stacksize = 65536;


struct context::impl_t
{
	ucontext_t				ctx;
	allocator_base::ptr_t	alloc;
    void (*m_fn)( void *);
    void* m_arg;

    static void taskstart(uint32_t y, uint32_t x);

	impl_t() :
		ctx(), alloc( 0)
	{
		std::memset( & ctx, 0, sizeof( ctx) );
        if(::getcontext(&ctx) < 0){
            fprintf(stderr, "getcontext: \n");
            abort();
        }
	}

	impl_t( void( fn)( void *), void * vp, std::size_t stacksize,
			impl_t * nxt, allocator_base::ptr_t alloc_) :
		ctx(), alloc( alloc_)
	{
        m_fn  = fn;
        m_arg = vp;

        stacksize *= 16;


		std::memset( & ctx, 0, sizeof( ctx) );

	    sigset_t zero;
        sigemptyset(&zero);
        sigprocmask(SIG_BLOCK, &zero, &ctx.uc_sigmask);

        /* must initialize with current context */
        if(::getcontext(&ctx) < 0){
            fprintf(stderr, "getcontext: \n");
            abort();
        }

        /** Mac OS X requires the stack size to be 16 byte alligned */
        stacksize = stacksize - stacksize %16;
        char* stk = (char*)malloc( stacksize );

/**
 *  The default protected_stack_allocator returns values that do not
 *  work with Mac OS X or are otherwise different than simply calling
 *  malloc.  
 */
/*
		char * stk = (char*)alloc->allocate( stacksize);
		if ( ! stk) throw std::bad_alloc();
        stk -= stacksize;
*/
        ctx.uc_stack.ss_sp = stk;
		ctx.uc_stack.ss_size = stacksize;
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
	
	~impl_t()
	{
		if ( ctx.uc_stack.ss_sp)
		{
            free( static_cast<char*>(ctx.uc_stack.ss_sp)  );
            /*
			void * base = static_cast< char * >( ctx.uc_stack.ss_sp) - 8 + ctx.uc_stack.ss_size;
			alloc->deallocate( base, ctx.uc_stack.ss_size);
            */
		}
	}

};

void context::impl_t::taskstart(uint32_t y, uint32_t x)
{
	uint64_t z;

	z = x<<16;	/* hide undefined 32-bit shift from 32-bit compilers */
	z <<= 16;
	z |= y;
    context::impl_t* self = (context::impl_t*)(z);

    self->m_fn(self->m_arg);

    if( !self->ctx.uc_link )
        exit(1);
	::swapcontext( & self->ctx, self->ctx.uc_link );
}

context::impl_t *
context::init_( void( fn)( void *), context const* nxt,
		void * vp, std::size_t stacksize, allocator_base::ptr_t alloc)
{
	if (  nxt && ! * nxt) throw context_moved(); 
	return new impl_t( fn, vp, stacksize, nxt ? nxt->impl_ : 0, alloc);
}

context
context::current()
{ return context( new impl_t() ); }

context::context() :
	impl_( 0)
{}

context::context( impl_t * impl) :
	impl_( impl)
{}

context::~context()
{ if ( impl_) delete impl_; }

context::context( BOOST_RV_REF( context) other) :
	impl_( 0)
{ std::swap( impl_, other.impl_); }

context &
context::operator=( BOOST_RV_REF( context) other)
{
	context tmp( boost::move( other) );
	swap( tmp);
	return * this;
}

void
context::jump_to( context & other)
{
	if ( ! impl_ || ! other) throw context_moved(); 
	::swapcontext( & impl_->ctx, & other.impl_->ctx);
}

void
context::swap( context & other)
{ std::swap( impl_, other.impl_); }

bool
context::operator!() const
{ return 0 == impl_; }

context::operator unspecified_bool_type() const
{ return 0 == impl_ ? 0 : unspecified_bool; }

}}
