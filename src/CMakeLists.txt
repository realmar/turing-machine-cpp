
cmake_minimum_required(VERSION 3.8)

add_subdirectory("executor")
add_subdirectory("utils")
add_subdirectory("graph")
add_subdirectory("tape")

file(GLOB everything
        "*.hpp"
        "*.cpp")

add_executable(TuringMachine ${everything} ${headers_executor} ${headers_utils} ${headers_graph} ${headers_tape})
