cmake_minimum_required(VERSION 3.6)
project(mpasswdsort)

set(CMAKE_C_FLAGS "-std=c99")

set(SOURCE_FILES mpasswdsort.c list.c)
add_executable(mpasswdsort ${SOURCE_FILES} mpasswdsort.c)