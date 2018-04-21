#pragma once

#include <string>
#include <utility>
#include <memory>

namespace realmar::turing {
	class node {
	private:
		const std::string _name;
	public:
		virtual ~node() = default;
		explicit node(std::string name) : _name(std::move(name)) {}
		explicit node(const char* name) : _name(std::string(name)) {}
		explicit node(const std::shared_ptr<node>& node) : _name(node->get_name()) {}

		const std::string& get_name() const {
			return _name;
		}

		bool operator==(const node& other) const {
			return _name == other.get_name();
		}

		bool operator!=(const node& other) const {
			return !(*this == other);
		}
	};
}
