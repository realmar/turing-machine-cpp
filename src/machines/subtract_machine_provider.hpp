#pragma once

#include "tm_provider.hpp"

namespace realmar::turing {
    class subtract_machine_provider : public tm_provider<2, int> {
    public:
        subtract_machine_provider() {
            tm_provider_factory::register_provider(std::make_shared<subtract_machine_provider>(*this));
        }

        std::string get_tm_name() override;

        turing_machine<2, int> get_tm() override;
    };
}