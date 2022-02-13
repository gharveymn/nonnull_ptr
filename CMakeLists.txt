cmake_minimum_required (VERSION 3.15)

project (
  nonnull_ptr
  VERSION
    0.0.5
  LANGUAGES
    CXX
)

# Options
string (COMPARE EQUAL "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_SOURCE_DIR}" _ENABLE_TESTS_DEFAULT)

option (
  GCH_NONNULL_PTR_ENABLE_TESTS
  "Set to ON to build tests for gch::nonnull_ptr."
  ${_ENABLE_TESTS_DEFAULT}
)

if (GCH_NONNULL_PTR_ENABLE_TESTS)
  include (CMakeDependentOption)

  cmake_dependent_option (
    GCH_USE_LIBCXX_WITH_CLANG
    "Set to ON to use libc++ when compiling with Clang."
    OFF
    "CMAKE_CXX_COMPILER_ID MATCHES Clang"
    OFF
  )

  enable_testing ()
endif ()

set (
  GCH_NONNULL_PTR_INSTALL_CMAKE_DIR
  "lib/cmake/nonnull_ptr"
  CACHE STRING
  "Specify where to install the CMake config files."
)

set (
  GCH_NONNULL_PTR_INSTALL_INCLUDE_DIR
  "include"
  CACHE STRING
  "Specify where to install gch/nonnull_ptr.hpp."
)

set (
  GCH_NONNULL_PTR_INSTALL_LICENSE_DIR
  "share/licenses/gch/nonnull_ptr"
  CACHE STRING
  "Specify where to install the license."
)

set (
  GCH_NONNULL_PTR_INSTALL_SUPPORT_DIR
  "share/gch"
  CACHE STRING
  "Specify where to install support files like GDB pretty printers."
)

add_subdirectory (source)

export (
  EXPORT
    nonnull_ptr-targets
  NAMESPACE
    gch::
)

install (
  FILES
    docs/LICENSE
  DESTINATION
    ${GCH_NONNULL_PTR_INSTALL_LICENSE_DIR}
)
