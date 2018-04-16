#include "subtract_machine_provider.hpp"
#include "../utils/misc.hpp"

std::string realmar::turing::subtract_machine_provider::get_tm_name() {
    return "Subtract Machine";
}

realmar::turing::turing_machine<2, int> realmar::turing::subtract_machine_provider::get_tm() {
    turing_machine<2, int> tm;

    tm.add_node(node("q0"));
    tm.add_node(node("q1"));
    tm.add_node(node("q2"));

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

    return tm;
}

// create instance and register with factory
static realmar::turing::subtract_machine_provider prov;