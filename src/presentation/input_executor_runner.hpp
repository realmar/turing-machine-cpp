#pragma once

#include <algorithm>
#include <iostream>
#include <string>
#include <map>
#include "../turing_machine.hpp"
#include "user_input.hpp"
#include "executor_runner.hpp"
#include "../utils/misc.hpp"

namespace realmar::turing {
    template<int N, typename T>
    class input_executor_runner : public executor_runner {
    private:
        turing_machine<N, T> _turing_machine;

        const char* empty_symbol = "_";

        inline void print_word(const tm_operation<N, T>& operation,
                               const std::shared_ptr<node>& node,
                               const word<T>& word,
                               const int& head_position) {
            for (auto j = 0; j < word.size(); ++j) {
                bool is_head_pos = j == head_position;
                std::string node_name = "q?";
                if (node != nullptr) node_name = node->get_name();
                if (is_head_pos)
                    std::cout << "|" << node_name << "|";
                auto s = word.at(j);
                std::string s_str = empty_symbol;
                if (!s.is_empty())
                    s_str = std::to_string(s.get_symbol());

                std::cout << s_str;
            }
        }

        void print_operation(const tm_operation<N, T>& operation) {
            std::shared_ptr<node> from_node = nullptr, to_node = nullptr;

            if (operation.transition_edge != nullptr) {
                from_node = operation.transition_edge->get_from_node();
                to_node = operation.transition_edge->get_to_node();
            } else {
                from_node = operation.from_node;
                to_node = operation.to_node;
            }

            for (auto i = 0; i < operation.tape_states_before.size(); ++i) {
                std::cout << "tape " << i << ": ";
                print_word(operation, from_node, operation.tape_states_before.at(i),
                           operation.head_positions_before.at(i));
                std::cout << " -> ";
                print_word(operation, to_node, operation.tape_states_after.at(i), operation.head_positions_after.at(i));
                std::cout << std::endl;
            }

            std::cout << "transition: <"
                      << (from_node == nullptr ? "?" : from_node->get_name().c_str()) << "> -> <"
                      << (to_node == nullptr ? "?" : to_node->get_name().c_str()) << ">" << std::endl;
            std::cout << "execution result: " << operation.get_execution_result_string() << std::endl;
        }

        void print_executor_state(const tm_executor<N, T>& executor) {
            auto steps = executor.get_steps();
            auto step_count = executor.get_step_count();

            std::cout << "initial state:" << std::endl;
            print_operation(executor.get_initial_state());
            std::cout << std::endl;

            int last_shown_steps = 4;
            std::cout << "last " << last_shown_steps << " operations:" << std::endl;

            int i_start = 0;
            if (last_shown_steps < steps.size()) i_start = steps.size() - last_shown_steps;

            for (auto i = i_start; i < steps.size(); ++i) {
                print_operation(steps.at(i));
                std::cout << "--" << std::endl;
            }
            std::cout << std::endl;

            std::cout << "transitions: ";
            std::shared_ptr<edge<N, T>> last_edge = nullptr;
            for (auto i = 0; i < steps.size(); ++i) {
                auto edge = steps.at(i).transition_edge;
                std::string from_node = "<?>", to_node = "<?>";
                if (edge != nullptr) {
                    from_node = steps.at(i).transition_edge->get_from_node()->get_name();
                    to_node = steps.at(i).transition_edge->get_to_node()->get_name();
                } else {
                    if (last_edge != nullptr) {
                        from_node = last_edge->get_to_node()->get_name();
                        to_node = last_edge->get_to_node()->get_name();
                    }
                }

                std::cout << from_node << " -> ";
                if (i + 1 == steps.size()) {
                    std::cout << to_node;
                }

                if (edge != nullptr)
                    last_edge = edge;
            }
            std::cout << std::endl << std::endl;

            std::cout << "performed steps: " << step_count << std::endl;
            std::cout << std::endl;
            std::cout << "tape contents:" << std::endl;
            int t_i = 0;
            for (auto&& tape : _turing_machine.get_tapes()) {
                auto w = tape.get_non_empty_contents();
                std::map<T, int> w_counter;

                std::cout << " tape " << t_i++ << ": ";
                for (auto&& item : w) {
                    auto symbol = item.get_symbol();

                    std::string s_str = empty_symbol;
                    if (!item.is_empty()) {
                        s_str = std::to_string(symbol);

                        auto item_in_counter = w_counter.find(symbol);
                        if (item_in_counter != w_counter.end()) {
                            item_in_counter->second++;
                        } else {
                            w_counter.emplace(symbol, 1);
                        }
                    }
                    std::cout << s_str;
                }

                if (w_counter.size() > 0) {
                    std::cout << " | ";

                    for (auto&& counter : w_counter)
                        std::cout << counter.first << " : " << counter.second << " | ";
                }

                std::cout << std::endl;
            }
        }

    public:
        explicit input_executor_runner(turing_machine<N, T>& turing_machine)
                : _turing_machine(turing_machine) {}

        void run() override {
            std::cout << "Enter the two numbers on which the operation should be performed" << std::endl;
            std::cout << "Enter first number: " << std::endl;
            auto a = get_number();

            std::cout << "Enter second number: " << std::endl;
            auto b = get_number();

            _turing_machine.set_word(create_unary_expression(a, b));

            auto executor = _turing_machine.get_concrete_executor();

            // REPL loop
            while (true) {
                int choice = get_user_choice({
                                                     "step",
                                                     "execute to finish",
                                                     "print current execution state",
                                                     "halt"
                                             });

                bool halt = false;
                bool p_exe_state = false;
                switch (choice) {
                    case 0:
                        executor->next();
                        p_exe_state = true;
                        break;
                    case 1:
                        executor->execute_to_finish();
                        p_exe_state = true;
                        halt = true;
                        break;
                    case 2:
                        p_exe_state = true;
                        break;
                    case 3:
                        halt = true;
                        break;
                    default:
                        print_invalid_input();
                        break;
                }

                if (p_exe_state) print_executor_state(*executor);

                std::cout << std::endl;
                if (halt) break;
            }
        }
    };
}