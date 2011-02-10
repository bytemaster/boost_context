
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CONTEXTS_DETAIL_FCONTEXT_H
#define BOOST_CONTEXTS_DETAIL_FCONTEXT_H

#include <boost/config.hpp>

#include <boost/context/detail/config.hpp>

#ifdef BOOST_HAS_ABI_HEADERS
# include BOOST_ABI_PREFIX
#endif

// i386
# if defined(_M_IX86) || defined(__i386__)
#include <boost/context/detail/fcontext_i386.hpp>
// x86_64
# elif defined(_M_X64) || defined(__x86_64__)
#include <boost/context/detail/fcontext_x86_64.hpp>
// arm
# elif defined(__arm__)
#include <boost/context/detail/fcontext_arm.hpp>
// mips
# elif defined(__mips__)
#include <boost/context/detail/fcontext_mips.hpp>
// powerpc
# elif defined(__powerpc__)
#include <boost/context/detail/fcontext_ppc.hpp>
# endif

namespace boost {
namespace contexts {
namespace detail {

extern "C" int BOOST_CONTEXT_CALLDECL get_fcontext( fcontext_t * fc);
extern "C" int BOOST_CONTEXT_CALLDECL set_fcontext( fcontext_t const* fc);
extern "C" int BOOST_CONTEXT_CALLDECL swap_fcontext( fcontext_t * ofc, fcontext_t const* nfc);
extern "C" int BOOST_CONTEXT_CALLDECL make_fcontext( fcontext_t * fc, void (* fn)( void *), void * p);

}}}

#ifdef BOOST_HAS_ABI_HEADERS
# include BOOST_ABI_SUFFIX
#endif

#endif // BOOST_CONTEXTS_DETAIL_FCONTEXT_H

