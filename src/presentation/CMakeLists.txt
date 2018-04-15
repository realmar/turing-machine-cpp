file(GLOB headers
        input_executor.hpp
        user_input.hpp
        generic_input_executor.hpp)
set(headers_presentation ${headers} PARENT_SCOPE)

file(GLOB impl
        user_input.cpp)
set(impl_presentation ${impl} PARENT_SCOPE)