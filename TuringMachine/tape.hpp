#pragma once

#include <memory>
#include "tape_iterator.hpp"
#include "typedefs.hpp"

namespace realmar::turing {
    template<typename T>
    class tape {
    private:
        std::shared_ptr<tape_cell<T>> _first_cell;

    public:
        virtual ~tape() = default;

        tape() : _first_cell(std::make_shared<tape_cell<T>>()) {}

        void overwrite_with_word(const word<T>& word) {
            _first_cell = std::make_shared<tape_cell<T>>();

            tape_iterator<T> iterator(_first_cell);
            for (auto w : word) {
                iterator.get_current_symbol().set_symbol(std::make_shared<T>(w));
                iterator.move_next();
            }
        }

        tape_iterator<T> get_iterator() {
            return tape_iterator<T>(_first_cell);
        }

        word<std::shared_ptr<T>> get_tape_contents() {
            tape_iterator<T> iterator(_first_cell);

            // iterate back until we find a nullptr
            while (true) {
                if (iterator.get_current_cell().get_previous() == nullptr) break;
                iterator.move_back();
            }

            // move next and write all symbols into word until we find a nullptr
            word<std::shared_ptr<T>> w;
            while (true) {
                w.emplace_back(iterator.get_current_symbol().get_symbol());

                if (iterator.get_current_cell().get_next() == nullptr) break;
                iterator.move_next();
            }

            return w;
        }

        word<T> get_non_empty_contents() {
            auto contents = get_tape_contents();
            word<T> w;
            for (auto&& item : contents) {
                if (item != nullptr) w.emplace_back(*item);
            }

            return w;
        }
    };
}
