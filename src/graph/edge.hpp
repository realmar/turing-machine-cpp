#pragma once

#include <array>
#include "move_direction.hpp"
#include "edge_args.hpp"
#include "symbol.hpp"
#include "node.hpp"

namespace realmar::turing {
    template<int N, typename T>
    class edge {
    private:
        const std::array<symbol<T>, N> _read_symbols;
        const std::array<symbol<T>, N> _write_symbols;
        const std::array<move_direction, N> _move_directions;

        const node from_node;
        const node to_node;
    public:
        virtual ~edge() = default;

        edge(const node& from_node, const node& to_node, const edge_args<N, T>& args) :
                from_node(from_node),
                to_node(to_node),
                _read_symbols(args.read_symbols),
                _write_symbols(args.write_symbols),
                _move_directions(args.move_directions) {}

        const node& get_from_node() const {
            return from_node;
        }

        const node& get_to_node() const {
            return to_node;
        }

        const std::array<symbol<T>, N>& get_read_symbols() const {
            return _read_symbols;
        }

        const std::array<symbol<T>, N>& get_write_symbols() const {
            return _write_symbols;
        }

        const std::array<move_direction, N>& get_move_directions() const {
            return _move_directions;
        }
    };
}
