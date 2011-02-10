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

set_property(SOURCE src/libtask/asm.S PROPERTY LANGUAGE C )
boost_add_library( context
    SOURCES
        src/libtask/asm.S
        src/libtask/context.c
)


