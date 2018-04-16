#pragma once

#include "execution_result.hpp"

namespace realmar::turing {
    class abstract_tm_executor {
    public:
        virtual int get_step_count() const = 0;

        virtual execution_result next() = 0;
    };
}