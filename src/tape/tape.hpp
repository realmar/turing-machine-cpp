#pragma once

#include <memory>
#include "tape_iterator.hpp"
#include "../utils/typedefs.hpp"

namespace realmar::turing {
    template<typename T>
    class tape {
    private:
        std::shared_ptr<tape_cell<T>> _first_cell = nullptr;

    public:
        virtual ~tape() = default;

        tape() : _first_cell(std::make_shared<tape_cell<T>>()) {}

        void overwrite_with_word(const word<T>& word) {
            _first_cell = std::make_shared<tape_cell<T>>();

            tape_iterator<T> iterator(_first_cell);
            for (auto w : word) {
                symbol<T>& symbol = iterator.get_current_symbol();
                symbol.set_symbol(w);
                symbol.set_empty(w.is_empty());

                iterator.move_next();
            }
        }

        tape_iterator<T> get_iterator() {
            return tape_iterator<T>(_first_cell);
        }

        word<T> get_tape_contents() {
            tape_iterator<T> iterator(_first_cell);

            // iterate back until we find a nullptr
            while (true) {
                if (iterator.get_current_cell().get_previous() == nullptr) break;
                iterator.move_back();
            }

            // move next and write all symbols into word until we find a nullptr
            word<T> w;
            while (true) {
                w.emplace_back(iterator.get_current_symbol());

                if (iterator.get_current_cell().get_next() == nullptr) break;
                iterator.move_next();
            }

            return w;
        }

        word<T> get_non_empty_contents() {
            auto contents = get_tape_contents();
            word<T> w;
            for (auto&& item : contents) {
                if (!item.is_empty()) w.emplace_back(item);
            }

            return w;
        }
    };
}
