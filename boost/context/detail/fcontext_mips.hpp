
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CONTEXTS_DETAIL_FCONTEXT_MIPS_H
#define BOOST_CONTEXTS_DETAIL_FCONTEXT_MIPS_H

#include <boost/config.hpp>

#include <boost/cstdint.hpp>
#include <boost/context/detail/config.hpp>

#ifdef BOOST_HAS_ABI_HEADERS
# include BOOST_ABI_PREFIX
#endif

namespace boost {
namespace contexts {
namespace detail {

// on MIPS we assume 64bit regsiters - even for 32bit ABIs
typedef uint64_t greg_t;
typedef uint64_t freg_t;

typedef struct stack    stack_t;
struct stack
{
    void    *   ss_base;
    void    *   ss_limit;
};

#if defined(__GNUC__) && defined(__mips__)

#define BOOST_CONTEXT_CALLDECL

typedef greg_t gregset_t[14];

# if _MIPS_SIM == _ABI64
typedef freg_t fregset_t[8];
#else
typedef freg_t fregset_t[6];
#endif

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

#endif // BOOST_CONTEXTS_DETAIL_FCONTEXT_MIPS_H
