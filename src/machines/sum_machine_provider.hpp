#pragma once

#include "tm_provider.hpp"

namespace realmar::turing {
    class sum_machine_provider : public tm_provider<1, int> {
    public:
        sum_machine_provider() {
            tm_provider_factory::register_provider(std::make_shared<sum_machine_provider>(*this));
        }

        std::string get_tm_name() override;

        turing_machine<1, int> get_tm() override;
    };
}