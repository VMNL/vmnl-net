foreach(example buffer_dynamic buffer_ring buffer_view clock_elapsed context_allocator error_handling timer_loop)
    add_executable(vmnl_net_example_${example} example_${example}.c)

    target_link_libraries(vmnl_net_example_${example}
      PRIVATE
        vmnl::net
        vmnl_net_warnings
    )

    add_test(NAME vmnl_net_example_${example} COMMAND vmnl_net_example_${example})
endforeach()
