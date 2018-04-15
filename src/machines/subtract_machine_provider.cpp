#include "subtract_machine_provider.hpp"
#include "../utils/misc.hpp"

realmar::turing::subtract_machine_provider::subtract_machine_provider(const int& a, const int& b)
        : int_machine_provider<2, int>(a, b) {
    if (a <= b) throw std::invalid_argument("a need to be bigger than b.");
}

realmar::turing::turing_machine<2, int> realmar::turing::subtract_machine_provider::get() {
    turing_machine<2, int> tm;

    tm.add_node(node<2>("q0"));
    tm.add_node(node<2>("q1"));
    tm.add_node(node<2>("q2"));

    {
        edge_args<2, int> args = {
                {symbol<int>(0),        symbol<int>()},
                {symbol<int>(),         symbol<int>(0)},
                {move_direction::right, move_direction::right}
        };
        tm.add_edge("q0", "q0", args);
    }

    {
        edge_args<2, int> args = {
                {symbol<int>(1),        symbol<int>()},
                {symbol<int>(),         symbol<int>()},
                {move_direction::right, move_direction::left}
        };
        tm.add_edge("q0", "q1", args);
    }

    {
        edge_args<2, int> args = {
                {symbol<int>(0),        symbol<int>(0)},
                {symbol<int>(),         symbol<int>()},
                {move_direction::right, move_direction::left}
        };
        tm.add_edge("q1", "q1", args);
    }

    {
        edge_args<2, int> args = {
                {symbol<int>(),         symbol<int>(0)},
                {symbol<int>(),         symbol<int>(0)},
                {move_direction::right, move_direction::right}
        };
        tm.add_edge("q1", "q2", args);
    }

    tm.set_start_node("q0");
    tm.set_final_node("q2");
    tm.set_word(create_unary_expression(number1, number2));

    return tm;
}