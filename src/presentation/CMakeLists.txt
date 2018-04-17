file(GLOB headers
        input_executor_runner.hpp
        user_input.hpp
        executor_runner.hpp
        utils.hpp)
set(headers_presentation ${headers} PARENT_SCOPE)

file(GLOB impl
        user_input.cpp
        utils.cpp)
add_library(presentation STATIC ${impl} ${headers})