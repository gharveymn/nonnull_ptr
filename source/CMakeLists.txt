add_library (nonnull_ptr INTERFACE)

target_sources (
  nonnull_ptr
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include/gch/nonnull_ptr.hpp>
)

target_include_directories (
  nonnull_ptr
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
)

target_include_directories (
  nonnull_ptr
  SYSTEM INTERFACE
    $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${GCH_NONNULL_PTR_INSTALL_INCLUDE_DIR}>
)

set_target_properties (
  nonnull_ptr
  PROPERTIES
  PUBLIC_HEADER
    include/gch/nonnull_ptr.hpp
)

add_library (gch::nonnull_ptr ALIAS nonnull_ptr)

install (
  TARGETS
    nonnull_ptr
  EXPORT
    nonnull_ptr-targets
  PUBLIC_HEADER
    DESTINATION ${GCH_NONNULL_PTR_INSTALL_INCLUDE_DIR}/gch
)

file (
  RELATIVE_PATH
  _PACKAGE_PREFIX_DIR
  "${CMAKE_INSTALL_PREFIX}/${GCH_NONNULL_PTR_INSTALL_CMAKE_DIR}"
  "${CMAKE_INSTALL_PREFIX}"
)
string (PREPEND _PACKAGE_PREFIX_DIR \${CMAKE_CURRENT_LIST_DIR}/)

file (WRITE ${CMAKE_CURRENT_BINARY_DIR}/nonnull_ptr-config.cmake "\
get_filename_component (PACKAGE_PREFIX_DIR \"${_PACKAGE_PREFIX_DIR}\" ABSOLUTE)
include (\"\${CMAKE_CURRENT_LIST_DIR}/nonnull_ptr-targets.cmake\")
")

include (CMakePackageConfigHelpers)
write_basic_package_version_file (
  nonnull_ptr-version.cmake
  COMPATIBILITY
    SameMinorVersion
)

install (
  FILES
    ${CMAKE_CURRENT_BINARY_DIR}/nonnull_ptr-config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/nonnull_ptr-version.cmake
  DESTINATION
    ${GCH_NONNULL_PTR_INSTALL_CMAKE_DIR}
)

install (
  EXPORT
    nonnull_ptr-targets
  DESTINATION
    ${GCH_NONNULL_PTR_INSTALL_CMAKE_DIR}
  NAMESPACE
    gch::
)

install (
  DIRECTORY
    ${CMAKE_CURRENT_LIST_DIR}/support/
  DESTINATION
    ${GCH_NONNULL_PTR_INSTALL_SUPPORT_DIR}
  PATTERN "__pycache__" EXCLUDE
)

if (GCH_NONNULL_PTR_ENABLE_TESTS)
  add_subdirectory (test)
endif ()
