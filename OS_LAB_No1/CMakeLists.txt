cmake_minimum_required(VERSION 3.16)
project(lab_02 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_executable(lab_01_parent src/parent.c)
add_executable(lab_01_child  src/child.c)