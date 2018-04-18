#pragma once

#include <vector>
#include <tuple>
#include "tm_operation.hpp"
#include "../graph/node.hpp"
#include "../turing_machine.hpp"
#include "../graph/move_direction.hpp"
#include "execution_result.hpp"
#include "tm_executor.hpp"

namespace realmar::turing {
#define tape_states_head_pos_tuple std::tuple<std::array<word<std::shared_ptr<T>>, N>, std::array<int, N>>

    template<int N, typename T>
    class tm_step_executor : public tm_executor<N, T> {
    private:
        turing_machine<N, T>& _turing_machine;
        std::vector<tape_iterator<T>> _iterators;
        std::shared_ptr<node> _current_node;

        std::shared_ptr<tm_operation<N, T>> _initial_state;
        int step_count = -1;
        std::vector<tm_operation<N, T>> _steps;

        std::shared_ptr<node> to_node_temp = nullptr, from_node_temp = nullptr;

        inline std::array<symbol<T>*, N> get_symbols() {
            std
            ::array<symbol<T>*, N> symbols;
            for (auto i = 0; i < _iterators.size(); ++i)
                symbols.at(i) = &_iterators.at(i).get_current_symbol();
            return symbols;
        }

        inline const std::shared_ptr<edge<N, T>>
        get_match(const std::vector<std::shared_ptr<edge<N, T>>>& edges,
                  const std
                  ::array<symbol<T>*, N>& symbols
        ) {
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

        tape_states_head_pos_tuple record_tape_states_and_head_pos_from_iterators() {
            std::array<word<std::shared_ptr<T >>, N> tape_states;
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

            return std
            ::make_tuple(tape_states, head_positions);
        };

        inline tm_operation<N, T>
        create_tm_operation_from_current_state(const std::shared_ptr<node> from_node,
                                               const std::shared_ptr<node> to_node,
                                               const std::shared_ptr<edge<N, T>> matched,
                                               const execution_result& result,
                                               const tape_states_head_pos_tuple& before_states) {

            auto states = record_tape_states_and_head_pos_from_iterators();
            return tm_operation<N, T>(step_count++,
                                      from_node,
                                      to_node,
                                      matched,
                                      result,
                                      std::get<0>(before_states),
                                      std::get<0>(states),
                                      std::get<1>(before_states),
                                      std::get<1>(states));
        }

    public:
        virtual ~tm_step_executor() = default;

        tm_step_executor(turing_machine<N, T>& tm) : _turing_machine(tm),
                                                     _current_node(_turing_machine.get_start_node()) {
            for (auto&& tape : tm.get_tapes()) {
                _iterators.emplace_back(tape.get_iterator());
            }

            auto states = record_tape_states_and_head_pos_from_iterators();
            _initial_state = std::make_shared<tm_operation<N, T>>(create_tm_operation_from_current_state(
                    std::make_shared<node>(_turing_machine.get_start_node()),
                    nullptr,
                    nullptr,
                    execution_result::not_finished,
                    states));
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
            // record state before operation
            //

            auto before_state = record_tape_states_and_head_pos_from_iterators();

            //
            // perform operation
            //

            auto result = execution_result::not_finished;
            if (matched == nullptr) {
                if (*_current_node == *_turing_machine.get_final_node()) {
                    from_node_temp = _current_node;
                    to_node_temp = _current_node;

                    result = execution_result::finished;
                } else {
                    result = execution_result::cannot_finish;
                }
            } else {
                from_node_temp = matched->get_from_node();
                to_node_temp = matched->get_to_node();

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
            }

            //
            // store metadata about performed operation
            //

            _steps.emplace_back(create_tm_operation_from_current_state(from_node_temp,
                                                                       to_node_temp,
                                                                       matched,
                                                                       result,
                                                                       before_state));

            return result;
        }
    };
}
