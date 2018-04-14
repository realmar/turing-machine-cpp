#pragma once

#include <memory>

namespace realmar::turing {
    template<typename T>
    struct symbol final {
    private:
        std::shared_ptr<T> _symbol = nullptr;

    public:
        symbol() = default;

        explicit symbol(const T& symbol) : _symbol(std::make_shared<T>(symbol)) {}

        const std::shared_ptr<T>& get_symbol() const {
            return _symbol;
        }

        void set_symbol(std::shared_ptr<T> symbol) {
            _symbol = symbol;
        }

        bool operator==(const symbol<T>& other) const {
            if (get_symbol() == nullptr && other.get_symbol() == nullptr) {
                return true;
            } else if (get_symbol() == nullptr || other.get_symbol() == nullptr) {
                return false;
            } else {
                return *get_symbol() == *other.get_symbol();
            }
        }

        bool operator!=(const symbol<T>& other) const {
            return !(*this == other);
        }
    };
}
