#include <stdexcept>
#include "typedefs.hpp"
#include "misc.hpp"

void realmar::turing::throw_if_not_positive(const int& i) {
    if (i < 0) throw std::invalid_argument("Cannot perform operation with negative number.");
}

realmar::turing::word<int> realmar::turing::create_unary_expression(const int& a, const int& b) {
    word<int> w;

    for (auto i = 0; i < a; ++i) w.emplace_back(0);
    w.emplace_back(1);
    for (auto i = 0; i < b; ++i) w.emplace_back(0);

    return w;
}