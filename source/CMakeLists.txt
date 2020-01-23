EXECUTE_PROCESS(COMMAND "git submodule update --init -- \"external/create-package\""
                WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

INCLUDE("${PROJECT_SOURCE_DIR}/external/create-package/cmake/create-package.cmake" OPTIONAL)

CREATE_PACKAGE(AUTHOR        gharveymn
               NAME          nonnull_ptr
               HEADERS       nonnull_ptr.hpp
               COMPATIBILITY ExactVersion)

# Only include tests if we are at the top level
IF(NONNULL_PTR_BUILD_TESTS)
  ADD_SUBDIRECTORY(test)
ENDIF()
