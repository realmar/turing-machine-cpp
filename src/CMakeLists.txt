
cmake_minimum_required(VERSION 3.8)

add_subdirectory("executor")
add_subdirectory("utils")
add_subdirectory("graph")
add_subdirectory("tape")
add_subdirectory("presentation")
add_subdirectory("machines")

file(GLOB headers_tm
        turing_machine.hpp
        abstract_turing_machine.hpp)

file(GLOB cli_tm tm.cpp)
file(GLOB cli_tm_dots tm_dots.cpp)

set(headers_all
        ${headers_tm}
        ${headers_executor}
        ${headers_utils}
        ${headers_graph}
        ${headers_tape}
        ${headers_presentation}
        ${headers_machines})

add_executable(tm
        ${cli_tm}
        ${headers_all})

add_executable(tm_dots
        ${cli_tm_dots}
        ${headers_all})

target_link_libraries(tm machines presentation utils)
target_link_libraries(tm_dots machines presentation utils)