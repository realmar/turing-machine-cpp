#pragma once

namespace realmar::turing {
    void throw_if_not_positive(const int& i);

    word<int> create_unary_expression(const int& a, const int& b);
}