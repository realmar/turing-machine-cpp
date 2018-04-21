#pragma once

#include <functional>
#include <vector>
#include <tuple>
#include <array>

#include "tm_operation.hpp"
#include "../graph/node.hpp"
#include "../graph/move_direction.hpp"
#include "execution_result.hpp"
#include "tm_executor.hpp"

namespace realmar::turing {
#define tape_states_head_pos_tuple std::tuple<std::array<word<T>, N>, std::array<int, N>>

	template<int N, typename T>
	class tm_step_executor : public tm_executor<N, T> {
	private:
		turing_machine<N, T>& _turing_machine;
		std::vector<tape_iterator<T>> _iterators;
		std::shared_ptr<node> _current_node;

		std::shared_ptr<tm_operation<N, T>> _initial_state;
		int _step_count = -1;
		std::vector<tm_operation<N, T>> _steps;

		std::shared_ptr<node> to_node_temp = nullptr, from_node_temp = nullptr;

		bool _only_record_terminal_operations = false;
		bool _only_record_step_count = false;

		std::vector<std::reference_wrapper<symbol<T>>> get_symbols() {
			std::vector<std::reference_wrapper<symbol<T>>> symbols;
			symbols.reserve(_iterators.size());

			for (unsigned int i = 0; i < _iterators.size(); ++i)
				symbols.emplace_back(std::ref(_iterators.at(i).get_current_symbol()));

			return symbols;
		}

		std::shared_ptr<edge<N, T>> get_match(
			const std::vector<std::shared_ptr<edge<N, T>>>& edges,
			const std::vector<std::reference_wrapper<symbol<T>>>& symbols) {

			std::shared_ptr<edge<N, T>> matched = nullptr;

			for (auto&& edge : edges) {
				const std::array<symbol<T>, N>& s = edge->get_read_symbols();
				for (unsigned int i = 0; i < s.size(); ++i) {
					if (s.at(i) != symbols.at(i)) {
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
			std::array<word<T>, N> tape_states;
			std::array<int, N> head_positions;

			auto i = 0;
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

			return std::make_tuple(tape_states, head_positions);
		};

		tm_operation<N, T>
			create_tm_operation_from_current_state(
				const std::shared_ptr<node> from_node,
				const std::shared_ptr<node> to_node,
				const std::shared_ptr<edge<N, T>> matched,
				const execution_result& result,
				const tape_states_head_pos_tuple& before_states) {

			auto states = record_tape_states_and_head_pos_from_iterators();
			return tm_operation<N, T>(
				_step_count,
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
		explicit tm_step_executor(turing_machine<N, T>& tm) : _turing_machine(tm),
			_current_node(_turing_machine.get_start_node()) {
			for (auto&& tape : tm.get_tapes()) {
				_iterators.emplace_back(tape.get_iterator());
			}

			auto states = record_tape_states_and_head_pos_from_iterators();
			_initial_state = std::make_shared<tm_operation<N, T>>(create_tm_operation_from_current_state(
				std::make_shared<node>(_turing_machine.get_start_node()),
				nullptr,
				nullptr,
				not_finished,
				states));
		}

		int get_step_count() const override {
			return _step_count;
		}

		const tm_operation<N, T>& get_initial_state() const override {
			return *_initial_state;
		}

		const std::vector<tm_operation<N, T>>& get_steps() const override {
			return _steps;
		}

		void set_only_record_terminal_operations(bool value) {
			_only_record_terminal_operations = value;
		}

		void set_only_record_step_count(bool value) {
			_only_record_step_count = value;
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

			auto result = not_finished;
			if (matched == nullptr) {
				if (*_current_node == *_turing_machine.get_final_node()) {
					from_node_temp = _current_node;
					to_node_temp = _current_node;

					result = finished;
				} else {
					result = cannot_finish;
				}
			} else {
				from_node_temp = matched->get_from_node();
				to_node_temp = matched->get_to_node();

				const std::array<symbol<T>, N>& write_symbols = matched->get_write_symbols();
				auto move_directions = matched->get_move_directions();

				for (unsigned int i = 0; i < write_symbols.size(); ++i) {
					auto& symbol = symbols.at(i);
					auto& replace_symbol = write_symbols.at(i);

					symbol.get().set_symbol(replace_symbol);
					symbol.get().set_empty(replace_symbol.is_empty());

					auto md = move_directions.at(i);
					switch (md) {
					case right:
						_iterators.at(i).move_next();
						break;
					case left:
						_iterators.at(i).move_back();
						break;
					}
				}

				_current_node = matched->get_to_node();
			}

			//
			// store metadata about performed operation
			//

			_step_count++;
			if (!_only_record_step_count && !_only_record_terminal_operations || result != not_finished) {
				_steps.emplace_back(create_tm_operation_from_current_state(from_node_temp,
					to_node_temp,
					matched,
					result,
					before_state));
			}

			return result;
		}
	};
}
