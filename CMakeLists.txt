cmake_minimum_required(VERSION 3.5.2)
project(mpasswdsort)

set(CMAKE_C_FLAGS "-std=c11")

set(SOURCE_FILES mpasswdsort.c list.c)
add_executable(mpasswdsort ${SOURCE_FILES} mpasswdsort.c)