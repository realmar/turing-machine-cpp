#pragma once

#include "symbol.hpp"

namespace realmar::turing {
    template<typename T>
    class tape_cell : public std::enable_shared_from_this<tape_cell<T>> {
    private:
        symbol <T> _symbol;
        std::shared_ptr<tape_cell<T>> _next = nullptr;
        std::shared_ptr<tape_cell<T>> _previous = nullptr;

    public:
        // virtual ~tape_cell() = default;

        tape_cell() = default;

        tape_cell(symbol <T>& symbol) : _symbol(symbol) {
        }

        symbol <T>& get_symbol() {
            return _symbol;
        }

        void set_symbol(symbol <T>& symbol) {
            _symbol = symbol;
        }

        std::shared_ptr<tape_cell<T>>& get_next() {
            return _next;
        }

        void set_next(std::shared_ptr<tape_cell<T>> value) {
            _next = value;
        }

        std::shared_ptr<tape_cell<T>>& get_previous() {
            return _previous;
        }

        void set_previous(std::shared_ptr<tape_cell<T>> value) {
            _previous = value;
        }

        void create_next() {
            _next = std::make_shared<tape_cell<T>>();
            _next->set_previous(this->shared_from_this());
        }

        void create_previous() {
            _previous = std::make_shared<tape_cell<T>>();
            _previous->set_next(this->shared_from_this());
        }
    };
}
