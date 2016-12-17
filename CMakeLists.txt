cmake_minimum_required(VERSION 3.6)
project(searchfolder)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99")

set(SOURCE_FILES main.c)
add_executable(searchfolder ${SOURCE_FILES})