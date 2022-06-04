project(tools_test)

set(CMAKE_CXX_STANDARD 17)

add_executable(tools_test main.cpp ../../src/details/strings.c)