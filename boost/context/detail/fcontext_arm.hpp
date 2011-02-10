
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CONTEXTS_DETAIL_FCONTEXT_ARM_H
#define BOOST_CONTEXTS_DETAIL_FCONTEXT_ARM_H

#include <boost/config.hpp>

#include <boost/cstdint.hpp>
#include <boost/context/detail/config.hpp>

#ifdef BOOST_HAS_ABI_HEADERS
# include BOOST_ABI_PREFIX
#endif

namespace boost {
namespace contexts {
namespace detail {

typedef uint32_t greg_t;
typedef uint32_t freg_t;

typedef struct stack    stack_t;
struct stack
{
    void    *   ss_base;
    void    *   ss_limit;
};

#if defined(__GNUC__) && defined(__arm__)

#define BOOST_CONTEXT_CALLDECL

typedef greg_t gregset_t[12];
typedef freg_t fregset_t[16];

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

#endif // BOOST_CONTEXTS_DETAIL_FCONTEXT_ARM_H
