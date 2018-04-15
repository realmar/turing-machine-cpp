#include "multiply_machine_provider.hpp"
#include "../utils/misc.hpp"

realmar::turing::multiply_machine_provider::multiply_machine_provider(const int& a, const int& b)
        : int_machine_provider<3, int>(a, b) {}

realmar::turing::turing_machine<3, int> realmar::turing::multiply_machine_provider::get() {
    turing_machine<3, int> tm;

    tm.add_node(node<3>("q0"));
    tm.add_node(node<3>("q1"));
    tm.add_node(node<3>("q2"));
    tm.add_node(node<3>("q3"));
    tm.add_node(node<3>("q4"));

    {
        edge_args<3, int> args = {
                {symbol<int>(0),        symbol<int>(),         symbol<int>()},
                {symbol<int>(),         symbol<int>(0),        symbol<int>()},
                {move_direction::right, move_direction::right, move_direction::stale}
        };
        tm.add_edge("q0", "q0", args);
    }

    {
        edge_args<3, int> args = {
                {symbol<int>(1),        symbol<int>(),        symbol<int>()},
                {symbol<int>(),         symbol<int>(),        symbol<int>()},
                {move_direction::right, move_direction::left, move_direction::stale}
        };
        tm.add_edge("q0", "q1", args);
    }

    {
        edge_args<3, int> args = {
                {symbol<int>(0),        symbol<int>(),         symbol<int>()},
                {symbol<int>(0),        symbol<int>(),         symbol<int>()},
                {move_direction::stale, move_direction::stale, move_direction::stale}
        };
        tm.add_edge("q1", "q4", args);
    }

    {
        edge_args<3, int> args = {
                {symbol<int>(),         symbol<int>(),         symbol<int>()},
                {symbol<int>(),         symbol<int>(),         symbol<int>()},
                {move_direction::stale, move_direction::stale, move_direction::stale}
        };
        tm.add_edge("q1", "q4", args);
    }

    {
        edge_args<3, int> args = {
                {symbol<int>(),         symbol<int>(0),        symbol<int>()},
                {symbol<int>(),         symbol<int>(0),        symbol<int>()},
                {move_direction::stale, move_direction::stale, move_direction::stale}
        };
        tm.add_edge("q1", "q4", args);
    }

    {
        edge_args<3, int> args = {
                {symbol<int>(0),        symbol<int>(0),       symbol<int>()},
                {symbol<int>(0),        symbol<int>(0),       symbol<int>()},
                {move_direction::stale, move_direction::left, move_direction::stale}
        };
        tm.add_edge("q1", "q2", args);
    }

    {
        edge_args<3, int> args = {
                {symbol<int>(0),        symbol<int>(0),       symbol<int>()},
                {symbol<int>(0),        symbol<int>(0),       symbol<int>()},
                {move_direction::stale, move_direction::left, move_direction::stale}
        };
        tm.add_edge("q2", "q2", args);
    }

    {
        edge_args<3, int> args = {
                {symbol<int>(),         symbol<int>(0),        symbol<int>()},
                {symbol<int>(),         symbol<int>(0),        symbol<int>()},
                {move_direction::stale, move_direction::stale, move_direction::stale}
        };
        tm.add_edge("q2", "q4", args);
    }

    {
        edge_args<3, int> args = {
                {symbol<int>(),         symbol<int>(),         symbol<int>()},
                {symbol<int>(),         symbol<int>(),         symbol<int>()},
                {move_direction::stale, move_direction::stale, move_direction::stale}
        };
        tm.add_edge("q2", "q4", args);
    }

    {
        edge_args<3, int> args = {
                {symbol<int>(0),        symbol<int>(),         symbol<int>()},
                {symbol<int>(0),        symbol<int>(),         symbol<int>()},
                {move_direction::stale, move_direction::right, move_direction::stale}
        };
        tm.add_edge("q2", "q3", args);
    }

    {
        edge_args<3, int> args = {
                {symbol<int>(0),        symbol<int>(0),        symbol<int>()},
                {symbol<int>(0),        symbol<int>(0),        symbol<int>(0)},
                {move_direction::stale, move_direction::right, move_direction::right}
        };
        tm.add_edge("q3", "q3", args);
    }

    {
        edge_args<3, int> args = {
                {symbol<int>(0),        symbol<int>(),        symbol<int>()},
                {symbol<int>(),         symbol<int>(),        symbol<int>()},
                {move_direction::right, move_direction::left, move_direction::stale}
        };
        tm.add_edge("q3", "q2", args);
    }

    tm.set_start_node("q0");
    tm.set_final_node("q4");
    tm.set_word(create_unary_expression(number1, number2));

    return tm;
}
