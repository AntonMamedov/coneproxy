project(Google_tests)
set(CMAKE_CXX_STANDARD 17)
add_subdirectory(lib/googletest)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(Google_Tests_run strings_test.cpp ../src/details/strings.c)
target_link_libraries(Google_Tests_run gtest gtest_main)