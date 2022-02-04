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
    $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>
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
    DESTINATION include/gch
)

file (WRITE ${CMAKE_CURRENT_BINARY_DIR}/nonnull_ptr-config.cmake [=[
get_filename_component (PACKAGE_PREFIX_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../" ABSOLUTE)
include ("${CMAKE_CURRENT_LIST_DIR}/nonnull_ptr-targets.cmake")
]=])

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
    lib/cmake/nonnull_ptr
)

install (
  EXPORT
    nonnull_ptr-targets
  DESTINATION
    lib/cmake/nonnull_ptr
  NAMESPACE
    gch::
)

if (GCH_NONNULL_PTR_BUILD_DOCS)
  add_subdirectory (docs)
endif ()

if (GCH_NONNULL_PTR_ENABLE_TESTS)
  add_subdirectory (test)
endif ()
