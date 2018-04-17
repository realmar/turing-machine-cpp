#pragma once

#include <array>
#include "move_direction.hpp"
#include "edge_args.hpp"
#include "symbol.hpp"
#include "node.hpp"
#include "abstract_edge.hpp"

namespace realmar::turing {
    template<int N, typename T>
    class edge : public abstract_edge {
    private:
        const std::array<symbol<T>, N> _read_symbols;
        const std::array<symbol<T>, N> _write_symbols;
        const std::array<move_direction, N> _move_directions;

        const std::shared_ptr<node> from_node;
        const std::shared_ptr<node> to_node;

        std::vector<symbol<std::string>> symbols_to_string(const std::array<symbol<T>, N>& symbols) {
            std::vector<symbol<std::string>> syms;
            for (auto&& rs : symbols) {
                if (rs.get_symbol() != nullptr) {
                    syms.emplace_back(symbol(std::to_string(*rs.get_symbol())));
                } else {
                    syms.emplace_back(symbol<std::string>());
                }
            }

            return syms;
        }

    public:
        virtual ~edge() = default;

        edge(const std::shared_ptr<node>& from_node,
             const std::shared_ptr<node>& to_node,
             const edge_args<N, T>& args) :
                from_node(from_node),
                to_node(to_node),
                _read_symbols(args.read_symbols),
                _write_symbols(args.write_symbols),
                _move_directions(args.move_directions) {}

        const std::shared_ptr<node>& get_from_node() const override {
            return from_node;
        }

        const std::shared_ptr<node>& get_to_node() const override {
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

        std::vector<symbol<std::string>> get_read_symbols_as_string() override {
            return symbols_to_string(_read_symbols);
        }

        std::vector<symbol<std::string>> get_write_symbols_as_string() override {
            return symbols_to_string(_write_symbols);
        }

        std::vector<move_direction> get_move_directions_as_vector() override {
            return std::vector<move_direction>(_move_directions.begin(), _move_directions.end());
        }
    };
}
