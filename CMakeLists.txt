cmake_minimum_required(VERSION 3.21)
project(coneproxy C)

include_directories(include)
set(CMAKE_C_STANDARD 11)
add_subdirectory(unit_test)
add_subdirectory(test/tool_test)
add_executable(coneproxy src/main.c src/net/tcp.c src/net/http.c src/net/urlencoding.c src/details/strings.c src/details/strings_hash_table.c)
