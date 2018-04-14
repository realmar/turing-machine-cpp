
cmake_minimum_required(VERSION 3.8)

file(GLOB everything
        "*.hpp"
        "*.cpp"
        )

add_executable(TuringMachine ${everything})
