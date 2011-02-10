# Distributed under the Boost Software License, Version 1.0.
# See http://www.boost.org/LICENSE_1_0.txt

boost_project("Context"
  AUTHORS
    "Oliver Kowalke"
    "Daniel Larimer"
  DESCRIPTION
    "User space context switching."
  DEPENDS
  )

file(GLOB_RECURSE headers include/boost/*.?pp)
boost_add_headers(${headers})

boost_add_library( boost_context
    SOURCE
        src/libtask/asm.S
        src/libtask/context.c
)


