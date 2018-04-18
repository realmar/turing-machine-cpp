#pragma once

#include "tm_provider.hpp"

namespace realmar::turing {
    class multiply_machine_provider : public tm_provider<3, int> {
    public:
        std::string get_tm_name() override;

        turing_machine<3, int> get_tm() override;
    };
}
