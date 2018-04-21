#pragma once

#include <memory>

namespace realmar::turing {
	template<typename T>
	struct symbol final {
	private:
		T _symbol;
		bool _is_empty;

	public:
		symbol() :
			_symbol(T()),
			_is_empty(true) {};

		explicit symbol(const T& symbol) :
			_symbol(symbol),
			_is_empty(false) {}

		symbol<T>& operator=(const symbol<T>& other) {
			if (this != &other) {
				this->_is_empty = other.is_empty();
				this->_symbol = other.get_symbol();
			}

			return *this;
		}

		const T& get_symbol() const {
			return _symbol;
		}

		void set_symbol(const T& value) {
			_symbol = value;
		}

		void set_symbol(const symbol<T>& value) {
			_symbol = value.get_symbol();
			_is_empty = value.is_empty();
		}

		bool is_empty() const {
			return _is_empty;
		}

		void set_empty(bool value) {
			_is_empty = value;
		}

		bool operator==(const symbol<T>& other) const {
			if (other.is_empty() && is_empty()) return true;
			if (other.is_empty() || is_empty()) return false;
			return get_symbol() == other.get_symbol();
		}

		bool operator!=(const symbol<T>& other) const {
			return !(*this == other);
		}
	};
}
