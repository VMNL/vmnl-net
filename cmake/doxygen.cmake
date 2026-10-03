find_package(Doxygen REQUIRED)

configure_file(docs/Doxyfile.in Doxyfile @ONLY)

add_custom_target(docs ALL
  COMMAND Doxygen::doxygen ${PROJECT_BINARY_DIR}/Doxyfile
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
  COMMENT "Generating the documentation"
  VERBATIM
)
