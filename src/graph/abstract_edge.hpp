#pragma once

#include <vector>
#include <string>
#include "node.hpp"
#include "symbol.hpp"
#include "move_direction.hpp"

namespace realmar::turing {
	class abstract_edge {
	public:
		virtual ~abstract_edge() = default;
		virtual const std::shared_ptr<node>& get_from_node() const = 0;
		virtual const std::shared_ptr<node>& get_to_node() const = 0;
		virtual std::vector<symbol<std::string>> get_read_symbols_as_string() = 0;
		virtual std::vector<symbol<std::string>> get_write_symbols_as_string() = 0;
		virtual std::vector<move_direction> get_move_directions_as_vector() = 0;
	};
}