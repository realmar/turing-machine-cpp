#include "sum_machine_provider.hpp"
#include "../utils/misc.hpp"

std::string realmar::turing::sum_machine_provider::get_tm_name() {
    return "Sum Machine";
}

realmar::turing::turing_machine<1, int> realmar::turing::sum_machine_provider::get_tm() {
    turing_machine<1, int> tm;

    tm.add_node(node("q0"));
    tm.add_node(node("q1"));
    tm.add_node(node("q2"));
    tm.add_node(node("q3"));

    {
        edge_args<1, int> args = {
                {symbol<int>(0)},
                {symbol<int>()},
                {move_direction::right}
        };
        tm.add_edge("q0", "q1", args);
    }
    {
        edge_args<1, int> args = {
                {symbol<int>(1)},
                {symbol<int>()},
                {move_direction::right}
        };
        tm.add_edge("q0", "q2", args);
    }
    {
        edge_args<1, int> args = {
                {symbol<int>(0)},
                {symbol<int>(0)},
                {move_direction::right}
        };
        tm.add_edge("q1", "q1", args);
    }
    {
        edge_args<1, int> args = {
                {symbol<int>(1)},
                {symbol<int>(0)},
                {move_direction::right}
        };
        tm.add_edge("q1", "q2", args);
    }
    {
        edge_args<1, int> args = {
                {symbol<int>(0)},
                {symbol<int>(0)},
                {move_direction::right}
        };
        tm.add_edge("q2", "q2", args);
    }
    {
        edge_args<1, int> args = {
                {symbol<int>()},
                {symbol<int>()},
                {move_direction::right}
        };
        tm.add_edge("q2", "q3", args);
    }

    tm.set_start_node("q0");
    tm.set_final_node("q3");

    return tm;
}