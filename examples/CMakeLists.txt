foreach(example buffer_read_write clock_elapsed)
    add_executable(vmnl_net_example_${example} example_${example}.c)

    target_link_libraries(vmnl_net_example_${example}
      PRIVATE
        vmnl::net
        vmnl_net_warnings
    )

    add_test(NAME vmnl_net_example_${example} COMMAND vmnl_net_example_${example})
endforeach()
