#pragma once

#include <vector>
#include "tm_operation.hpp"
#include "../graph/node.hpp"
#include "../turing_machine.hpp"
#include "../graph/move_direction.hpp"
#include "execution_result.hpp"
#include "tm_executor.hpp"

namespace realmar::turing {
    template<int N, typename T>
    class tm_step_executor : public tm_executor<N, T> {
    private:
        turing_machine<N, T>& _turing_machine;
        std::vector<tape_iterator<T>> _iterators;
        std::shared_ptr<node> _current_node;

        std::shared_ptr<tm_operation<N, T>> _initial_state;
        int step_count = -1;
        std::vector<tm_operation<N, T>> _steps;

        std::shared_ptr<edge<N, T>> last_matched_edge = nullptr;

        inline std::array<symbol<T>*, N> get_symbols() {
            std::array<symbol<T>*, N> symbols;
            for (auto i = 0; i < _iterators.size(); ++i)
                symbols.at(i) = &_iterators.at(i).get_current_symbol();
            return symbols;
        }

        inline const std::shared_ptr<edge<N, T>>
        get_match(const std::vector<std::shared_ptr<edge<N, T>>>& edges,
                  const std::array<symbol<T>*, N>& symbols) {
            std::shared_ptr<edge<N, T>> matched = nullptr;

            for (auto&& edge : edges) {
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

            return matched;
        };

        inline tm_operation<N, T>
        create_tm_operation_from_current_state(const std::shared_ptr<node> from_node,
                                               const std::shared_ptr<node> to_node,
                                               const std::shared_ptr<edge<N, T>> matched,
                                               const execution_result& result) {
            std::array<word<std::shared_ptr<T>>, N> tape_states;
            std::array<int, N> head_positions;

            int i = 0;
            for (auto&& iterator : _iterators) {
                int head_pos = 0;

                tape_cell<T>* cell = &iterator.get_current_cell();
                while (true) {
                    if (cell->get_previous() == nullptr) break;
                    cell = cell->get_previous().get();
                    ++head_pos;
                }

                tape_states.at(i) = _turing_machine.get_tapes().at(i).get_tape_contents();
                head_positions.at(i) = head_pos;

                ++i;
            }

            return tm_operation<N, T>(step_count++, from_node, to_node, matched, result, tape_states, head_positions);
        }

    public:
        virtual ~tm_step_executor() = default;

        tm_step_executor(turing_machine<N, T>& tm) : _turing_machine(tm),
                                                     _current_node(_turing_machine.get_start_node()) {
            for (auto&& tape : tm.get_tapes()) {
                _iterators.emplace_back(tape.get_iterator());
            }

            _initial_state = std::make_shared<tm_operation<N, T>>(create_tm_operation_from_current_state(
                    std::make_shared<node>(_turing_machine.get_start_node()),
                    nullptr,
                    nullptr,
                    execution_result::not_finished));
        }

        int get_step_count() const override {
            return step_count;
        }

        const tm_operation<N, T>& get_initial_state() const override {
            return *_initial_state;
        }

        const std::vector<tm_operation<N, T>>& get_steps() const override {
            return _steps;
        }

        execution_result next() override {
            //
            // setup
            //

            auto edges = _turing_machine.get_connected_edges(*_current_node);
            auto symbols = get_symbols();

            //
            // get matching edge
            //

            auto matched = get_match(edges, symbols);

            //
            // perform operation
            //

            execution_result result;
            if (matched == nullptr) {
                if (*_current_node == *_turing_machine.get_final_node()) {
                    result = execution_result::finished;
                } else {
                    result = execution_result::cannot_finish;
                }
            } else {
                last_matched_edge = matched;

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

                _current_node = matched->get_to_node();

                // check if we can do another action
                auto next_edges = _turing_machine.get_connected_edges(*_current_node);
                auto next_symbols = get_symbols();
                auto next_match = get_match(next_edges, next_symbols);

                if (*_current_node == *_turing_machine.get_final_node() && next_match == nullptr) {
                    result = execution_result::finished;
                } else {
                    result = execution_result::not_finished;
                }
            }

            //
            // store metadata about performed operation
            //

            std::shared_ptr<node> from_node = nullptr, to_node = nullptr;
            if (last_matched_edge != nullptr) {
                from_node = std::make_shared<node>(last_matched_edge->get_from_node());
                to_node = std::make_shared<node>(last_matched_edge->get_to_node());
            }

            _steps.emplace_back(create_tm_operation_from_current_state(from_node, to_node, matched, result));

            return result;
        }
    };
}
