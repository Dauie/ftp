cmake_minimum_required(VERSION 3.6)
project(ftp)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        incl/ftp.h
        src/client.c
        src/server.c
        twochar.cpp)

add_executable(ftp ${SOURCE_FILES})