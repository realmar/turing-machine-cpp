#pragma once

#include <memory>
#include "../graph/edge.hpp"
#include "execution_result.hpp"
#include "../utils/typedefs.hpp"

namespace realmar::turing {
    template<int N, typename T>
    struct tm_operation {
        const int step_index;

        const std::shared_ptr<node> from_node;
        const std::shared_ptr<node> to_node;

        const std::shared_ptr<edge<N, T>> transition_edge;
        const execution_result exe_result;

        const std::array<word<std::shared_ptr<T>>, N> tape_states_before;
        const std::array<word<std::shared_ptr<T>>, N> tape_states_after;

        const std::array<int, N> head_positions_before;
        const std::array<int, N> head_positions_after;

        tm_operation(const int& step_index,
                     const std::shared_ptr<node>& from_node,
                     const std::shared_ptr<node>& to_node,
                     const std::shared_ptr<edge<N, T>> edge,
                     const execution_result exe_result,
                     const std::array<word<std::shared_ptr<T>>, N> tape_states_before,
                     const std::array<word<std::shared_ptr<T>>, N> tape_states_after,
                     const std::array<int, N> head_positions_before,
                     const std::array<int, N> head_positions_after) : from_node(from_node),
                                                                      to_node(to_node),
                                                                      step_index(step_index),
                                                                      transition_edge(edge),
                                                                      exe_result(exe_result),
                                                                      tape_states_before(tape_states_before),
                                                                      tape_states_after(tape_states_after),
                                                                      head_positions_before(head_positions_before),
                                                                      head_positions_after(head_positions_after) {}

        std::string get_execution_result_string() const {
            switch (exe_result) {
                case execution_result::not_finished:
                    return "not finished";
                case execution_result::finished:
                    return "finished";
                case execution_result::cannot_finish:
                    return "cannot finished";
            }
        }
    };
}
