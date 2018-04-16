#pragma once

#include <string>

namespace realmar::turing {
    class node {
    private:
        const std::string _name;
    public:
        virtual ~node() = default;

        explicit node(const std::string& name) : _name(name) {}

        explicit node(const char* name) : _name(std::string(name)) {}

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
