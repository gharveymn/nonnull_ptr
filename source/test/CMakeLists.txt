SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

ADD_EXECUTABLE(test_nonnull_ptr main.cpp)
TARGET_LINK_LIBRARIES(test_nonnull_ptr nonnull_ptr)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  TARGET_COMPILE_OPTIONS(test_nonnull_ptr PUBLIC -stdlib=libc++ -ftime-trace --coverage -Wall)
  TARGET_LINK_OPTIONS(test_nonnull_ptr PUBLIC -Wl --allow-multiple-definition --coverage)
  TARGET_LINK_LIBRARIES(test_nonnull_ptr c++ c++abi)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  SET(COMPILE_OPTS -Wall -Wextra)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  SET(COMPILE_OPTS /W4)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")

endif()

SET_TARGET_PROPERTIES(test_nonnull_ptr PROPERTIES
                      CXX_STANDARD          11
                      CXX_STANDARD_REQUIRED NO
                      CXX_EXTENSIONS        NO
                      COMPILE_OPTIONS       "${COMPILE_OPTS}"
                      LINK_OPTIONS          "${LINK_OPTS}")
