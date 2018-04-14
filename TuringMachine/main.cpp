#include <string>
#include <iostream>
#include "turing_machine.hpp"
#include "tm_auto_executor.hpp"
#include "typedefs.hpp"

using namespace realmar::turing;

template<int N>
struct result final {
    int step_count;
    std::vector<tm_operation<N, int>> operations;
    std::string unary_notation;
    int result;
};

inline void validate_number(const int& i) {
    if (i < 0) throw std::invalid_argument("Cannot perform operation with negative number.");
}

inline word<int> create_unary_expression(const int& a, const int& b) {
    word<int> w;

    for (auto i = 0; i < a; ++i) w.emplace_back(0);
    w.emplace_back(1);
    for (auto i = 0; i < b; ++i) w.emplace_back(0);

    return w;
}

template<int N>
inline result<N>
process_result(const execution_result& res,
               turing_machine<N, int>& tm,
               const tm_step_executor<N, int> executor,
               const int& tape_number) {

    auto word = tm.get_tapes().at(tape_number).get_non_empty_contents();
    int result_number = 0;
    std::string s;
    for (auto&& item : word) {
        s += std::to_string(item);
        result_number++;
    }

    result<N> r = {
            executor.get_step_count(),
            executor.get_steps(),
            s,
            result_number
    };

    return r;
}

result<1> sum(const int& a, const int& b) {
    validate_number(a);
    validate_number(b);

    turing_machine<1, int> tm;

    tm.add_node(node<1>("q0"));
    tm.add_node(node<1>("q1"));
    tm.add_node(node<1>("q2"));
    tm.add_node(node<1>("q3"));

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
    tm.set_final_node("q2");
    tm.set_word(create_unary_expression(a, b));

    auto executor = tm.get_executor();

    auto result = execution_result::not_finished;
    while (result == execution_result::not_finished) {
        result = executor->next();
    }

    return process_result(result, tm, *executor, 0);
}

result<2> subtract(const int& a, const int& b) {
    if (a <= b) throw std::invalid_argument("a need to be bigger than b.");

    validate_number(a);
    validate_number(b);

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
    tm.set_word(create_unary_expression(a, b));

    auto executor = tm.get_executor();

    auto result = execution_result::not_finished;
    while (result == execution_result::not_finished) {
        result = executor->next();
    }

    return process_result(result, tm, *executor, 1);
}

int main() {
    auto sum_result = sum(4, 2);
    std::cout << sum_result.unary_notation << " : " << sum_result.result << std::endl;

    auto subtract_result = subtract(10, 8);
    std::cout << subtract_result.unary_notation << " : " << subtract_result.result << std::endl;

    return 0;
}
