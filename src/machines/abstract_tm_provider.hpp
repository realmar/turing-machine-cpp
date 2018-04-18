#pragma once

#include <memory>
#include "../presentation/executor_runner.hpp"
#include "../abstract_turing_machine.hpp"
#include "tm_provider_collection.hpp"

namespace realmar::turing {
    class abstract_tm_provider {
    public:
        virtual ~abstract_tm_provider() = default;

        virtual std::string get_tm_name() = 0;

        virtual std::shared_ptr<abstract_turing_machine> get_abstract_tm() = 0;

        virtual std::shared_ptr<executor_runner> get_runner() = 0;
    };
}