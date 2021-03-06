#pragma once

#include <iostream>
#include <string>
#include <map>
#include <chrono>

#include "user_input.hpp"
#include "executor_runner.hpp"
#include "../turing_machine.hpp"
#include "../utils/misc.hpp"

namespace realmar::turing {
	template<int N, typename T>
	class input_executor_runner : public executor_runner {
	private:
		enum output_mode {
			DEFAULT,
			ALL_OPS,
			ONLY_STEPS
		};

		turing_machine<N, T> _turing_machine;
		output_mode _output_mode = DEFAULT;
		const char* _empty_symbol = "_";

		void print_word(const tm_operation<N, T>& operation,
			const std::shared_ptr<node>& node,
			const word<T>& word,
			const int& head_position) {
			for (unsigned int j = 0; j < word.size(); ++j) {
				const auto is_head_pos = j == head_position;
				std::string node_name = "q?";
				if (node != nullptr) node_name = node->get_name();
				if (is_head_pos)
					std::cout << "|" << node_name << "|";
				auto s = word.at(j);
				std::string s_str = _empty_symbol;
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

			for (unsigned int i = 0; i < operation.tape_states_before.size(); ++i) {
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

			if (_output_mode != ONLY_STEPS) {
				std::cout << "initial state:" << std::endl;
				print_operation(executor.get_initial_state());
				std::cout << std::endl;

				const auto last_shown_steps = _output_mode == ALL_OPS ? steps.size() : 4;
				if (_output_mode == ALL_OPS) {
					std::cout << "all operations:" << std::endl;
				} else {
					std::cout << "last " << last_shown_steps << " operations:" << std::endl;
				}

				size_t i_start = 0;
				if (last_shown_steps < steps.size()) i_start = steps.size() - last_shown_steps;

				for (auto i = i_start; i < steps.size(); ++i) {
					print_operation(steps.at(i));
					std::cout << "--" << std::endl;
				}
				std::cout << std::endl;

				std::cout << "transitions: ";
				std::shared_ptr<edge<N, T>> last_edge = nullptr;
				for (unsigned int i = 0; i < steps.size(); ++i) {
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
			}

			std::cout << "performed steps: " << step_count << std::endl;

			if (_output_mode != ONLY_STEPS) {
				std::cout << std::endl;
				std::cout << "tape contents:" << std::endl;
				auto t_i = 0;
				for (auto&& tape : _turing_machine.get_tapes()) {
					auto w = tape.get_non_empty_contents();
					std::map<T, int> w_counter;

					std::cout << " tape " << t_i++ << ": ";
					for (auto&& item : w) {
						auto symbol = item.get_symbol();

						std::string s_str = _empty_symbol;
						if (!item.is_empty()) {
							s_str = std::to_string(symbol);

							auto item_in_counter = w_counter.find(symbol);
							if (item_in_counter != w_counter.end()) {
								++item_in_counter->second;
							} else {
								w_counter.emplace(symbol, 1);
							}
						}
						std::cout << s_str;
					}

					if (!w_counter.empty()) {
						std::cout << " | ";

						for (auto&& counter : w_counter)
							std::cout << counter.first << " : " << counter.second << " | ";
					}

					std::cout << std::endl;
				}
			} else {

			}
		}

	public:
		explicit input_executor_runner(turing_machine<N, T>& tm)
			: _turing_machine(tm) {
		}

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
				const auto choice = get_user_choice({
					/* 0 */ "step",
					/* 1 */ "execute to finish",
					/* 2 */ "print current execution state",
					/* 3 */ "only record terminal operations",
					/* 4 */ "only show step count",
					/* 5 */ "show all steps",
					/* 6 */ "measure performance",
					/* 7 */ "halt"
					});

				auto measure_perf = false;
				auto halt = false;
				auto p_exe_state = false;

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
					executor->set_only_record_terminal_operations(true);
					break;
				case 4:
					executor->set_only_record_step_count(true);
					_output_mode = ONLY_STEPS;
					break;
				case 5:
					executor->set_only_record_terminal_operations(false);
					executor->set_only_record_step_count(false);
					_output_mode = ALL_OPS;
					break;
				case 6:
					executor->set_only_record_step_count(true);
					measure_perf = true;
					halt = true;
					break;
				case 7:
					halt = true;
					break;
				default:
					print_invalid_input();
					break;
				}

				if (p_exe_state) print_executor_state(*executor);

				if (measure_perf) {
					std::cout << "Measuring performance ..." << std::endl;
					const auto start = std::chrono::steady_clock::now();
					executor->execute_to_finish();
					const auto end = std::chrono::steady_clock::now();

					const auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
					const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(milliseconds);

					std::cout << "Execution took: " << milliseconds.count() << "ms / " << seconds.count() << "s" << std::endl;
					std::cout << "Performed steps: " << executor->get_step_count() << std::endl;
				}

				std::cout << std::endl;
				if (halt) break;
			}
		}
	};
}
