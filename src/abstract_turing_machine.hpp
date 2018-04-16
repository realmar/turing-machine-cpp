#pragma once

#include <memory>
#include "executor/abstract_tm_executor.hpp"

namespace realmar::turing {
    class abstract_turing_machine {
    public:
        virtual std::shared_ptr<abstract_tm_executor> get_executor() = 0;
    };
}