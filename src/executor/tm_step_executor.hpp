#pragma once

#include <vector>
#include "tm_operation.hpp"
#include "../graph/node.hpp"
#include "../turing_machine.hpp"
#include "../graph/move_direction.hpp"
#include "execution_result.hpp"

namespace realmar::turing {
    template<int N, typename T>
    class tm_step_executor {
    private:
        turing_machine<N, T>& _turing_machine;
        std::vector<tape_iterator<T>> _iterators;
        const node<N>* _current_node;
        int step_count;
    protected:
        std::vector<tm_operation<N, T>> steps;
    public:
        virtual ~tm_step_executor() = default;

        explicit tm_step_executor(turing_machine<N, T>& tm) : _turing_machine(tm),
                                                              _current_node(&_turing_machine.get_start_node()) {
            for (auto&& tape : tm.get_tapes()) {
                _iterators.emplace_back(tape.get_iterator());
            }
        }

        int get_step_count() const {
            return step_count;
        }

        const std::vector<tm_operation<N, T>>& get_steps() const {
            return steps;
        }

        execution_result next() {
            // setup
            std::vector<edge<N, T>*> edges = _turing_machine.get_connected_edges(*_current_node);
            std::array<symbol<T>*, N> symbols;

            for (auto i = 0; i < _iterators.size(); ++i)
                symbols.at(i) = &_iterators.at(i).get_current_symbol();

            // get matching edge
            const edge<N, T>* matched = nullptr;

            for (edge<N, T>* edge : edges) {
                const std::array<symbol<T>, N>& s = edge->get_read_symbols();
                for (auto i = 0; i < s.size(); ++i) {
                    if (s.at(i) != *symbols.at(i)) {
                        matched = nullptr;
                        break;
                    }
                    matched = edge;
                }

                if (matched != nullptr)
                    break;
            }

            // perform operation

            execution_result result;
            if (matched == nullptr) {
                if (*_current_node == _turing_machine.get_final_node()) {
                    result = execution_result::finished;
                } else {
                    result = execution_result::cannot_finish;
                }
            } else {
                const std::array<symbol<T>, N>& write_symbols = matched->get_write_symbols();
                auto move_directions = matched->get_move_directions();

                for (auto i = 0; i < write_symbols.size(); ++i) {
                    auto symbol = symbols.at(i);
                    auto replace_symbol = write_symbols.at(i).get_symbol();
                    symbol->set_symbol(replace_symbol);

                    auto md = move_directions.at(i);
                    switch (md) {
                        case move_direction::right:
                            _iterators.at(i).move_next();
                            break;
                        case move_direction::left:
                            _iterators.at(i).move_back();
                            break;
                    }
                }

                _current_node = &matched->get_to_node();

                if (*_current_node == _turing_machine.get_final_node()) {
                    result = execution_result::finished;
                } else {
                    result = execution_result::not_finished;
                }
            }

            // store metadata about performed operation
            steps.emplace_back(tm_operation(step_count, matched));
            step_count++;

            return result;
        }
    };
}
