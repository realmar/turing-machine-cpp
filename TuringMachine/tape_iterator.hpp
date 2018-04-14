#pragma once

#include "tape_cell.hpp"

namespace realmar::turing {
    template<typename T>
    class tape_iterator {
    private:
        std::shared_ptr<tape_cell<T>> _current_cell;

    public:
        virtual ~tape_iterator() = default;

        explicit tape_iterator(std::shared_ptr<tape_cell<T>>& first_cell) : _current_cell(first_cell) {}

        void move_next() {
            if (_current_cell->get_next() == nullptr)
                _current_cell->create_next();

            _current_cell = _current_cell->get_next();
        }

        void move_back() {
            if (_current_cell->get_previous() == nullptr)
                _current_cell->create_previous();

            _current_cell = _current_cell->get_previous();
        }

        symbol<T>& get_current_symbol() {
            return _current_cell->get_symbol();
        }

        tape_cell<T>& get_current_cell() const {
            return *_current_cell;
        }
    };
}
