file(GLOB headers
        misc.hpp
        typedefs.hpp
        defines.hpp)
set(headers_utils ${headers} PARENT_SCOPE)

file(GLOB impl misc.cpp)
add_library(utils STATIC ${impl} ${headers})