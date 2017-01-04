cmake_minimum_required(VERSION 3.6)
project(searchfolder)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99")

set(SOURCE_FILES main.c logger.c logger.h crawler.c crawler.h stack.c stack.h parser.c parser.h filter.c filter.h wrappersyscall.c wrappersyscall.h)
add_executable(searchfolder ${SOURCE_FILES})