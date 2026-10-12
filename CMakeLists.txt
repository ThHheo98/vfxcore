cmake_minimum_required(VERSION 3.25)

project(vfxcore LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_BUILD_TYPE STREQUAL "Debug" AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
  add_link_options(-fsanitize=address,undefined)
endif()

add_library(vfxcore src/vec3.cpp)
target_include_directories(vfxcore PUBLIC include)

add_executable(vfxcore_demo src/main.cpp)
target_link_libraries(vfxcore_demo PRIVATE vfxcore)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(vfxcore_tests
  tests/vec3_test.cpp
  tests/semantics_test.cpp
  tests/ownership_test.cpp
)
target_link_libraries(vfxcore_tests PRIVATE vfxcore GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(vfxcore_tests)
