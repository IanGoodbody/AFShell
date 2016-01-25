cmake_minimum_required(VERSION 3.3)
project(PEX1)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")

set(SOURCE_FILES
    CMakeLists.txt
    list.c
    list.h
    listtest.c)

add_executable(PEX1 ${SOURCE_FILES})