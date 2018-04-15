#pragma once

#include "int_machine_provider.hpp"

namespace realmar::turing {
    class multiply_machine_provider : public int_machine_provider<3, int> {
    public:
        multiply_machine_provider(const int& a, const int& b);

        turing_machine<3, int> get() override;
    };
}
