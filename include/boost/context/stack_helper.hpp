
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CONTEXTS_STACK_HELPER_H
#define BOOST_CONTEXTS_STACK_HELPER_H

#include <cstddef>

#include <boost/config.hpp>

#include <boost/context/detail/config.hpp>

#ifdef BOOST_HAS_ABI_HEADERS
# include BOOST_ABI_PREFIX
#endif

namespace boost {
namespace contexts {

struct stack_helper
{
    static std::size_t default_stacksize();

    static BOOST_CONTEXT_DECL std::size_t pagesize();

    static BOOST_CONTEXT_DECL std::size_t page_count( std::size_t stacksize);

    static BOOST_CONTEXT_DECL bool unlimited_stacksize();

    static BOOST_CONTEXT_DECL std::size_t minimal_stacksize();

    static BOOST_CONTEXT_DECL std::size_t maximal_stacksize();
};

}}

#ifdef BOOST_HAS_ABI_HEADERS
# include BOOST_ABI_SUFFIX
#endif

#endif // BOOST_CONTEXTS_STACK_HELPER_H
