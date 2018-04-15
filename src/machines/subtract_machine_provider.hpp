#pragma once

#include "int_machine_provider.hpp"

namespace realmar::turing {
    class subtract_machine_provider : public int_machine_provider<2, int> {
    public:
        subtract_machine_provider(const int& a, const int& b);

        turing_machine<2, int> get() override;
    };
}