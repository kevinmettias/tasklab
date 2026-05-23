cmake_minimum_required(VERSION 4.2)
project(tasklab)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

add_executable(tasklab
        .idea/.gitignore
        .idea/workspace.xml
        Makefile
        src/main.c
        tests/test_main.c
        src/task.c
        tests/test_task.h
        include/tl/task.h)
