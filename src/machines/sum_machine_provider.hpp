#pragma once

#include "int_machine_provider.hpp"

namespace realmar::turing {
    class sum_machine_provider : public int_machine_provider<1, int> {
    public:
        sum_machine_provider(const int& a, const int& b);

        turing_machine<1, int> get() override;
    };
}