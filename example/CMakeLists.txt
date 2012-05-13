IF( ENABLE_EXAMPLES )
    add_executable( jump jump.cpp )
    target_link_libraries( jump boost_context )

#    add_executable( fcontext_test fcontext.cpp )
#    target_link_libraries( fcontext_test boost_context )

#    add_executable( link_test link.cpp )
#    target_link_libraries( link_test boost_context )

#    add_executable( exit_test exit.cpp )
#    target_link_libraries( exit_test boost_context )

#    add_executable( do_return_test do_return.cpp )
#    target_link_libraries( do_return_test boost_context )

#    add_executable( unwind_test unwind.cpp )
#    target_link_libraries( unwind_test boost_context )

#    add_executable( swap_test swap.cpp )
#    target_link_libraries( swap_test boost_context )
ENDIF(ENABLE_EXAMPLES)
