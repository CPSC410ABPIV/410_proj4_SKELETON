cmake_minimum_required(VERSION 3.16)
project(410_proj4_SKELETON)

set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS -pthread)

include_directories(includes)

add_executable(410_proj4_SKELETON
        baker/Baker.cpp
        baker/box.cpp
        includes/baker.h
        includes/box.h
        includes/constants.h
        includes/datastructs.h
        includes/externs.h
        includes/file_IO.h
        includes/logger.h
        includes/PRINT.h
        includes/waiter.h
        logger/logger.cpp
        src/410_proj4.cpp
        waiter/file_IO.cpp
        waiter/waiter.cpp)
