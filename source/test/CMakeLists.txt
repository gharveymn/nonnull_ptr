SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

ADD_EXECUTABLE(test_nonnull_ptr main.cpp)
TARGET_LINK_LIBRARIES(test_nonnull_ptr nonnull_ptr)

IF(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  TARGET_COMPILE_OPTIONS(test_nonnull_ptr PUBLIC -stdlib=libc++ -ftime-trace --coverage -Wall -Wextra)
  TARGET_LINK_OPTIONS(test_nonnull_ptr PUBLIC "-Wl,--allow-multiple-definition" --coverage)
  TARGET_LINK_LIBRARIES(test_nonnull_ptr c++ c++abi)
ELSEIF(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  TARGET_COMPILE_OPTIONS(test_nonnull_ptr PUBLIC -Wall -Wextra)
ELSEIF(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  TARGET_COMPILE_OPTIONS(test_nonnull_ptr PUBLIC /W4)
ELSEIF(CMAKE_CXX_COMPILER_ID STREQUAL "Intel")

ENDIF()

SET_TARGET_PROPERTIES(test_nonnull_ptr PROPERTIES
                      CXX_STANDARD          11
                      CXX_STANDARD_REQUIRED NO
                      CXX_EXTENSIONS        NO)
