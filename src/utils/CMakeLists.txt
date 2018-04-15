file(GLOB headers
        misc.hpp
        typedefs.hpp
        vector_helpers.hpp)
set(headers_utils ${headers} PARENT_SCOPE)

file(GLOB impl misc.cpp)
set(impl_utils ${impl} PARENT_SCOPE)