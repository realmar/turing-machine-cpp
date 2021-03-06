#include "multiply_machine_provider.hpp"
#include "../utils/misc.hpp"

std::string realmar::turing::multiply_machine_provider::get_tm_name() {
	return "Multiply Machine";
}

realmar::turing::turing_machine<3, int> realmar::turing::multiply_machine_provider::get_tm() {
	turing_machine<3, int> tm;

	tm.add_node(node("q0"));
	tm.add_node(node("q1"));
	tm.add_node(node("q2"));
	tm.add_node(node("q3"));
	tm.add_node(node("q4"));

	{
		const edge_args<3, int> args = {
				{symbol<int>(0),        symbol<int>(),         symbol<int>()},
				{symbol<int>(),         symbol<int>(0),        symbol<int>()},
				{right, right, stale}
		};
		tm.add_edge("q0", "q0", args);
	}

	{
		const edge_args<3, int> args = {
				{symbol<int>(1),        symbol<int>(),        symbol<int>()},
				{symbol<int>(),         symbol<int>(),        symbol<int>()},
				{right, left, stale}
		};
		tm.add_edge("q0", "q1", args);
	}

	{
		const edge_args<3, int> args = {
				{symbol<int>(0),        symbol<int>(),         symbol<int>()},
				{symbol<int>(0),        symbol<int>(),         symbol<int>()},
				{stale, stale, stale}
		};
		tm.add_edge("q1", "q4", args);
	}

	{
		const edge_args<3, int> args = {
				{symbol<int>(),         symbol<int>(),         symbol<int>()},
				{symbol<int>(),         symbol<int>(),         symbol<int>()},
				{stale, stale, stale}
		};
		tm.add_edge("q1", "q4", args);
	}

	{
		const edge_args<3, int> args = {
				{symbol<int>(),         symbol<int>(0),        symbol<int>()},
				{symbol<int>(),         symbol<int>(0),        symbol<int>()},
				{stale, stale, stale}
		};
		tm.add_edge("q1", "q4", args);
	}

	{
		const edge_args<3, int> args = {
				{symbol<int>(0),        symbol<int>(0),       symbol<int>()},
				{symbol<int>(0),        symbol<int>(0),       symbol<int>()},
				{stale, left, stale}
		};
		tm.add_edge("q1", "q2", args);
	}

	{
		const edge_args<3, int> args = {
				{symbol<int>(0),        symbol<int>(0),       symbol<int>()},
				{symbol<int>(0),        symbol<int>(0),       symbol<int>()},
				{stale, left, stale}
		};
		tm.add_edge("q2", "q2", args);
	}

	{
		const edge_args<3, int> args = {
				{symbol<int>(),         symbol<int>(0),        symbol<int>()},
				{symbol<int>(),         symbol<int>(0),        symbol<int>()},
				{stale, stale, stale}
		};
		tm.add_edge("q2", "q4", args);
	}

	{
		const edge_args<3, int> args = {
				{symbol<int>(),         symbol<int>(),         symbol<int>()},
				{symbol<int>(),         symbol<int>(),         symbol<int>()},
				{stale, stale, stale}
		};
		tm.add_edge("q2", "q4", args);
	}

	{
		const edge_args<3, int> args = {
				{symbol<int>(0),        symbol<int>(),         symbol<int>()},
				{symbol<int>(0),        symbol<int>(),         symbol<int>()},
				{stale, right, stale}
		};
		tm.add_edge("q2", "q3", args);
	}

	{
		const edge_args<3, int> args = {
				{symbol<int>(0),        symbol<int>(0),        symbol<int>()},
				{symbol<int>(0),        symbol<int>(0),        symbol<int>(0)},
				{stale, right, right}
		};
		tm.add_edge("q3", "q3", args);
	}

	{
		const edge_args<3, int> args = {
				{symbol<int>(0),        symbol<int>(),        symbol<int>()},
				{symbol<int>(),         symbol<int>(),        symbol<int>()},
				{right, left, stale}
		};
		tm.add_edge("q3", "q2", args);
	}

	tm.set_start_node("q0");
	tm.set_final_node("q4");

	return tm;
}