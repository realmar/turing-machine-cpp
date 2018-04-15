#pragma once

#include "turing_machine_provider.hpp"
#include "../utils/misc.hpp"

namespace realmar::turing {
    template<int N, typename T>
    class int_machine_provider : public turing_machine_provider<N, T> {
    protected:
        int number1, number2;
    public:
        int_machine_provider(const int& a, const int& b) {
            throw_if_not_positive(a);
            throw_if_not_positive(b);

            number1 = a;
            number2 = b;
        }

        turing_machine<N, T> get() override = 0;
    };
}