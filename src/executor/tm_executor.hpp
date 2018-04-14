#pragma once

#include <vector>
#include "execution_result.hpp"
#include "tm_operation.hpp"

namespace realmar::turing {
    template<int N, typename T>
    class tm_executor {
    public:
        virtual ~tm_executor() = default;

        virtual int get_step_count() const = 0;

        virtual const std::vector<tm_operation<N, T>>& get_steps() const = 0;

        virtual execution_result next() = 0;

    };
}