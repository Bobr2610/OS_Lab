cmake_minimum_required(VERSION 3.16)
project(lab_04 C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

include_directories(${CMAKE_SOURCE_DIR}/include)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_library(functions1 SHARED src/lib1.c)
set_target_properties(functions1 PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    PREFIX "lib"
)

add_library(functions2 SHARED src/lib2.c)
set_target_properties(functions2 PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    PREFIX "lib"
)

add_executable(program1 src/program1.c)
target_link_libraries(program1 functions1)
set_target_properties(program1 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_executable(program2 src/program2.c)
target_link_libraries(program2 dl)
set_target_properties(program2 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
