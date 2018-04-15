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
        const node<N>* _current_node;
        int step_count = 0;

        inline std::array<symbol<T>*, N> get_symbols() {
            std::array<symbol<T>*, N> symbols;
            for (auto i = 0; i < _iterators.size(); ++i)
                symbols.at(i) = &_iterators.at(i).get_current_symbol();
            return symbols;
        }

        inline const edge<N, T>*
        get_match(const std::vector<edge<N, T>*>& edges, const std::array<symbol<T>*, N>& symbols) {
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

            return matched;
        };

        inline void create_tm_operation_from_current_state(const edge<N, T>* matched, const execution_result& result) {
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

            steps.emplace_back(tm_operation<N, T>(step_count++, matched, result, tape_states, head_positions));
        }

    protected:
        std::vector<tm_operation<N, T>> steps;
    public:
        virtual ~tm_step_executor() = default;

        tm_step_executor(turing_machine<N, T>& tm) : _turing_machine(tm),
                                                     _current_node(&_turing_machine.get_start_node()) {
            for (auto&& tape : tm.get_tapes()) {
                _iterators.emplace_back(tape.get_iterator());
            }

            create_tm_operation_from_current_state(nullptr, execution_result::not_finished);
        }

        int get_step_count() const override {
            return step_count;
        }

        const std::vector<tm_operation<N, T>>& get_steps() const override {
            return steps;
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

                // check if we can do another action
                auto next_edges = _turing_machine.get_connected_edges(*_current_node);
                auto next_symbols = get_symbols();
                auto next_match = get_match(next_edges, next_symbols);

                if (*_current_node == _turing_machine.get_final_node() && next_match == nullptr) {
                    result = execution_result::finished;
                } else {
                    result = execution_result::not_finished;
                }
            }

            //
            // store metadata about performed operation
            //

            create_tm_operation_from_current_state(matched, result);

            return result;
        }
    };
}
