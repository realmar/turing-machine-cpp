#pragma once

#include <string>
#include <vector>

namespace realmar::turing {
    void print_invalid_input();

    int get_user_choice(std::vector<std::string> choices);

    int get_number();

    int get_positive_number();
}