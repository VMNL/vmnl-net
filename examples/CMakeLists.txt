foreach(example version_string error_as_string allocator_set context_create clock_elapsed timer_start buffer_basics address_basics socket_datagram_basics)
    add_executable(vmnl_net_example_${example} example_${example}.c)

    target_link_libraries(vmnl_net_example_${example}
      PRIVATE
        vmnl::net
        vmnl_net_warnings
    )

    add_test(NAME vmnl_net_example_${example} COMMAND vmnl_net_example_${example})
endforeach()
