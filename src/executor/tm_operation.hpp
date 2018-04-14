#pragma once

#include "../graph/edge.hpp"

namespace realmar::turing {
    template<int N, typename T>
    struct tm_operation {
        const int step_index;
        const edge <N, T>* transition_edge;

        tm_operation(const int& step_index, const edge <N, T>* edge_) : step_index(step_index),
                                                                        transition_edge(edge_) {}
    };
}
