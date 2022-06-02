cmake_minimum_required(VERSION 3.21)
project(coneproxy C)

include_directories(include)
set(CMAKE_C_STANDARD 11)

add_executable(coneproxy src/main.c src/net/tcp.c)
