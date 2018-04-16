#pragma once

#include "tm_provider.hpp"

namespace realmar::turing {
    class multiply_machine_provider : public tm_provider<3, int> {
    public:
        multiply_machine_provider() {
            tm_provider_factory::register_provider(std::make_shared<multiply_machine_provider>(*this));
        }

        std::string get_tm_name() override;

        turing_machine<3, int> get_tm() override;
    };
}
