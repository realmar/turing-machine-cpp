#pragma once

#include "../turing_machine.hpp"

namespace realmar::turing {
    template<int N, typename T>
    class turing_machine_provider {
    public:
        virtual ~turing_machine_provider() = default;

        virtual turing_machine<N, T> get() = 0;
    };
}