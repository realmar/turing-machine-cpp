file(GLOB headers
        abstract_tm_provider.hpp
        tm_provider.hpp
        sum_machine_provider.hpp
        subtract_machine_provider.hpp
        multiply_machine_provider.hpp
        tm_provider_collection.hpp)
set(headers_machines ${headers} PARENT_SCOPE)

file(GLOB impl
        sum_machine_provider.cpp
        subtract_machine_provider.cpp
        multiply_machine_provider.cpp
        tm_provider_collection.cpp)

add_library(machines STATIC ${impl} ${headers})
