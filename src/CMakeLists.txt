
cmake_minimum_required(VERSION 3.8)

add_subdirectory("executor")
add_subdirectory("utils")
add_subdirectory("graph")
add_subdirectory("tape")
add_subdirectory("presentation")
add_subdirectory("machines")

file(GLOB main
        "*.hpp"
        "*.cpp")

add_executable(TuringMachine
        ${main}

        ${impl_utils}
        ${impl_machines}
        ${impl_presentation}

        ${headers_executor}
        ${headers_utils}
        ${headers_graph}
        ${headers_tape}
        ${headers_presentation}
        ${headers_machines})
