
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CONTEXTS_DETAIL_FCONTEXT_X86_64_H
#define BOOST_CONTEXTS_DETAIL_FCONTEXT_X86_64_H

#include <boost/config.hpp>
#include <boost/assert.hpp>

#include <boost/cstdint.hpp>
#include <boost/context/detail/config.hpp>

#ifdef BOOST_HAS_ABI_HEADERS
# include BOOST_ABI_PREFIX
#endif

namespace boost {
namespace contexts {
namespace detail {

typedef uint64_t greg_t;

typedef struct stack    stack_t;
struct stack
{
    void    *   ss_base;
    void    *   ss_limit;
};

#if defined(_MSC_VER) && defined(_M_X64)

#define BOOST_CONTEXT_CALLDECL

typedef greg_t gregset_t[12];

typedef struct fcontext fcontext_t;
struct fcontext
{
    gregset_t       fc_greg;
    void        *   fc_fp;
    stack_t         fc_stack;
    fcontext_t  *   fc_link;
    void        *   fc_local_storage;
};

#elif defined(__GNUC__) && defined(__x86_64__)

#define BOOST_CONTEXT_CALLDECL

typedef uint32_t freg_t;

typedef greg_t gregset_t[9];
typedef freg_t fregset_t[2];

typedef struct fcontext fcontext_t;
struct fcontext
{
    gregset_t       fc_greg;
    fregset_t       fc_freg;
    stack_t         fc_stack;
    fcontext_t  *   fc_link;
};

#else
#error "no fcontext_t for this platform"

#endif

}}}

#ifdef BOOST_HAS_ABI_HEADERS
# include BOOST_ABI_SUFFIX
#endif

#endif // BOOST_CONTEXTS_DETAIL_FCONTEXT_X86_64_H
