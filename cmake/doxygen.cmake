if(VMNL_NET_BUILD_DOCS_INTERNAL)
    find_package(Doxygen REQUIRED COMPONENTS dot)
    set(VMNL_NET_DOCS_EXTRA_INPUT src)
    set(VMNL_NET_DOCS_GRAPHS YES)
    set(VMNL_NET_DOCS_TITLE_SUFFIX " (internals)")
else()
    find_package(Doxygen REQUIRED)
    set(VMNL_NET_DOCS_GRAPHS NO)
endif()

configure_file(docs/Doxyfile.in Doxyfile @ONLY)

add_custom_target(docs ALL
  COMMAND Doxygen::doxygen ${PROJECT_BINARY_DIR}/Doxyfile
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
  COMMENT "Generating the documentation"
  VERBATIM
)
