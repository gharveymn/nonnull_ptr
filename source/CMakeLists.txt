SET(REQUIRED_CREATE_PACKAGE_VERSION 0.0.1)
IF(NOT HAS_CREATE_PACKAGE OR
   NOT CREATE_PACKAGE_VERSION VERSION_GREATER_EQUAL REQUIRED_CREATE_PACKAGE_VERSION)
  MESSAGE(STATUS "Initializing create-package-${REQUIRED_CREATE_PACKAGE_VERSION} in git submodule.")
  EXECUTE_PROCESS(COMMAND git submodule --quiet update --init -- "${PROJECT_SOURCE_DIR}/external/create-package"
                  WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")
  INCLUDE("${PROJECT_SOURCE_DIR}/external/create-package/cmake/create-package.cmake")
ENDIF()

CREATE_PACKAGE(AUTHOR        gharveymn
               NAMESPACE     gch
               NAME          nonnull_ptr
               HEADERS       nonnull_ptr.hpp
               COMPATIBILITY AnyNewerVersion)

# Only include tests if we are at the top level
IF(NONNULL_PTR_BUILD_TESTS)
  ADD_SUBDIRECTORY(test)
ENDIF()
