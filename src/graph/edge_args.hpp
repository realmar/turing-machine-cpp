#pragma once

#include <array>
#include "symbol.hpp"
#include "move_direction.hpp"

namespace realmar::turing {
	template<int N, typename T>
	struct edge_args final {
		const std::array<symbol<T>, N>& read_symbols;
		const std::array<symbol<T>, N>& write_symbols;
		const std::array<move_direction, N>& move_directions;
	};
}
