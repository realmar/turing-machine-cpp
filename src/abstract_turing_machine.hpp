#pragma once

#include <memory>
#include <vector>
#include "executor/abstract_tm_executor.hpp"
#include "graph/node.hpp"
#include "graph/abstract_edge.hpp"

namespace realmar::turing {
	class abstract_turing_machine {
	public:
		virtual ~abstract_turing_machine() = default;
		virtual const std::vector<std::shared_ptr<node>>& get_nodes() const = 0;
		virtual std::vector<std::shared_ptr<abstract_edge>> get_edges() const = 0;
		virtual const std::shared_ptr<node>& get_start_node() const = 0;
		virtual const std::shared_ptr<node>& get_final_node() const = 0;
		virtual std::shared_ptr<abstract_tm_executor> get_executor() = 0;
	};
}