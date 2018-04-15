file(GLOB headers
        int_machine_provider.hpp
        turing_machine_provider.hpp
        sum_machine_provider.hpp
        subtract_machine_provider.hpp
        multiply_machine_provider.hpp)
set(headers_machines ${headers} PARENT_SCOPE)

file(GLOB impl
        sum_machine_provider.cpp
        subtract_machine_provider.cpp
        multiply_machine_provider.cpp)
set(impl_machines ${impl} PARENT_SCOPE)