
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CONTEXTS_DETAIL_FCONTEXT_PPC_H
#define BOOST_CONTEXTS_DETAIL_FCONTEXT_PPC_H

#include <boost/config.hpp>

#include <boost/cstdint.hpp>
#include <boost/context/detail/config.hpp>

#ifdef BOOST_HAS_ABI_HEADERS
# include BOOST_ABI_PREFIX
#endif

namespace boost {
namespace contexts {
namespace detail {

typedef struct stack    stack_t;
struct stack
{
    void    *   ss_base;
    void    *   ss_limit;
};

#if defined(__GNUC__) && defined(__powerpc__)

#define BOOST_CONTEXT_CALLDECL

# if defined(__powerpc64__)
typedef uint64_t greg_t;
# else
typedef uint32_t greg_t;
#endif

typedef uint64_t freg_t;

typedef greg_t gregset_t[24];
typedef freg_t fregset_t[19];

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

#endif // BOOST_CONTEXTS_DETAIL_FCONTEXT_PPC_H
