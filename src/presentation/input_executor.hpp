#pragma once

#include <iostream>
#include <string>
#include <map>
#include "../turing_machine.hpp"
#include "user_input.hpp"
#include "generic_input_executor.hpp"

namespace realmar::turing {
    template<int N, typename T>
    class input_executor : public generic_input_executor {
    private:
        turing_machine<N, T> _turing_machine;

        void print_executor_state(const tm_executor<N, T>& executor) {
            auto steps = executor.get_steps();
            auto step_count = executor.get_step_count();

            std::cout << "operations: " << std::endl;
            int step_i = 0;
            for (auto&& step : steps) {
                for (auto i = 0; i < step.tape_states.size(); ++i) {
                    std::cout << "tape " << i << ": ";
                    for (auto j = 0; j < step.tape_states.at(i).size(); ++j) {
                        bool is_head_pos = j == step.head_positions.at(i);
                        std::string node_name = "q?";
                        if (step.transition_edge != nullptr) {
                            node_name = step.transition_edge->get_from_node().get_name();
                        } else {
                            // if we are in the first op then we will just take the next
                            if (step_i == 0 && steps.size() > 1 && steps.at(1).transition_edge != nullptr) {
                                node_name = steps.at(1).transition_edge->get_from_node().get_name();
                            }
                        }
                        if (is_head_pos)
                            std::cout << "|" << node_name << "|";
                        auto s = step.tape_states.at(i).at(j);
                        std::string s_str = "⌊⌋";
                        if (s != nullptr)
                            s_str = std::to_string(*s);
                        std::cout << s_str;
                    }
                    std::cout << std::endl;
                }
                std::cout << "execution result: " << step.get_execution_result_string() << std::endl;
                std::cout << "--" << std::endl;
                step_i++;
            }

            std::cout << "transitions: ";
            for (auto i = 0; i < steps.size(); ++i) {
                auto edge = steps.at(i).transition_edge;
                std::string from_node = "<?>", to_node = "<?>";
                if (edge != nullptr) {
                    from_node = steps.at(i).transition_edge->get_from_node().get_name();
                    to_node = steps.at(i).transition_edge->get_to_node().get_name();
                }

                if (edge != nullptr && i != 0) {
                    std::cout << from_node << " -> ";
                    if (i + 1 == steps.size()) {
                        std::cout << to_node;
                    }
                }
            }

            std::cout << std::endl;
            std::cout << "performed steps: " << step_count << std::endl;
            std::cout << "tape contents:" << std::endl;
            int t_i = 0;
            for (auto&& tape : _turing_machine.get_tapes()) {
                auto w = tape.get_non_empty_contents();
                std::map<T, int> w_counter;

                std::cout << " tape " << t_i++ << ": ";
                for (auto&& item : w) {
                    std::cout << item;
                    auto item_in_counter = w_counter.find(item);
                    if (item_in_counter != w_counter.end()) {
                        item_in_counter->second++;
                    } else {
                        w_counter.emplace(item, 1);
                    }
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
        explicit input_executor(turing_machine<N, T>& turing_machine)
                : _turing_machine(turing_machine) {}

        void run() override {
            auto executor = _turing_machine.get_executor();

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