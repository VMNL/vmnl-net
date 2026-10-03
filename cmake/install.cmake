include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set_target_properties(vmnl_net PROPERTIES EXPORT_NAME net)

install(TARGETS vmnl_net
  EXPORT vmnl_net
  FILE_SET HEADERS
)

install(EXPORT vmnl_net
  NAMESPACE vmnl::
  FILE vmnl_netConfig.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/vmnl_net
)

write_basic_package_version_file(vmnl_netConfigVersion.cmake
  COMPATIBILITY SameMinorVersion
)

install(FILES ${PROJECT_BINARY_DIR}/vmnl_netConfigVersion.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/vmnl_net
)

cmake_path(RELATIVE_PATH CMAKE_INSTALL_PREFIX
  BASE_DIRECTORY ${CMAKE_INSTALL_FULL_LIBDIR}/pkgconfig
  OUTPUT_VARIABLE VMNL_NET_PC_PREFIX
)

configure_file(cmake/vmnl-net.pc.in vmnl-net.pc @ONLY)

install(FILES ${PROJECT_BINARY_DIR}/vmnl-net.pc
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
)
