#pragma once

#include <vector>
#include "tm_operation.hpp"
#include "abstract_tm_executor.hpp"

namespace realmar::turing {
    template<int N, typename T>
    class tm_executor : public abstract_tm_executor {
    public:
        virtual ~tm_executor() = default;

        virtual const tm_operation<N, T>& get_initial_state() const = 0;

        virtual const std::vector<tm_operation<N, T>>& get_steps() const = 0;
    };
}