#pragma once

#include <vector>
#include <algorithm>
#include <memory>

#include "graph/node.hpp"
#include "graph/edge.hpp"
#include "tape/tape.hpp"
#include "utils/typedefs.hpp"
#include "abstract_turing_machine.hpp"

namespace realmar::turing {
    // forward declaration
    template<int N, typename T>
    class tm_auto_executor;

    template<int N, typename T>
    class turing_machine : public abstract_turing_machine {
    private:
        std::vector<std::shared_ptr<node>> _nodes;
        std::vector<std::shared_ptr<edge<N, T>>> _edges;

        std::shared_ptr<node> _start_node = nullptr;
        std::shared_ptr<node> _final_node = nullptr;
        std::array<tape<T>, N> _tapes;

        std::shared_ptr<node> get_node_by_name(const std::string& name) {
            auto result = std::find_if(_nodes.begin(), _nodes.end(),
                                       [&name](const auto& n) { return n->get_name() == name; });

            return result != _nodes.end() ? *result : nullptr;
        }

        std::shared_ptr<node> get_node_by_name_or_throw(const std::string& name) {
            auto node = get_node_by_name(name);
            if (node == nullptr) throw std::invalid_argument("Node not found.");

            return node;
        }

        bool has_node(const node& node) {
            return std::find_if(_nodes.begin(), _nodes.end(),
                                [&node](const auto& item) { return item != nullptr && *item == node; }) != _nodes.end();
        }

    public:
        virtual ~turing_machine() = default;

        turing_machine() = default;

        void add_node(const node& new_node) {
            if (get_node_by_name(new_node.get_name()) != nullptr) {
                throw std::invalid_argument("There is already a node with this name present.");
            }

            _nodes.emplace_back(std::make_shared<node>(new_node));
        }

        void add_edge(const node& node1, const node& node2, const edge_args<N, T>& edge_args) {
            if (has_node(node1) && has_node(node2)) {
                _edges.emplace_back(std::make_shared<edge<N, T>>(node1, node2, edge_args));
            } else {
                throw std::invalid_argument("Nodes not found in turing machine. Both nodes need to be present.");
            }
        }

        void add_edge(const std::string& node1_name, const std::string& node2_name, const edge_args <N, T>& edge_args) {
            auto node1 = get_node_by_name(node1_name);
            auto node2 = get_node_by_name(node2_name);

            if (node1 == nullptr)
                throw std::invalid_argument("Node " + node1_name + " not found.");

            if (node2 == nullptr)
                throw std::invalid_argument("Node " + node2_name + " not found.");

            add_edge(*node1, *node2, edge_args);
        }

        const std::vector<std::shared_ptr<node>>& get_nodes() const override {
            return _nodes;
        }

        std::vector<std::shared_ptr<abstract_edge>> get_edges() const override {
            std::vector<std::shared_ptr<abstract_edge>> edges;
            for (auto&& e : _edges) {
                edges.emplace_back(e);
            }

            return edges;
        }

        void set_start_node(const node& node) {
            set_start_node(node.get_name());
        }

        void set_start_node(const std::string& node_name) {
            auto n = get_node_by_name_or_throw(node_name);
            _start_node = n;
        }

        const node& get_start_node() const override {
            return *_start_node;
        }

        void set_final_node(const node& node) {
            set_final_node(node.get_name());
        }

        void set_final_node(const std::string& node_name) {
            auto n = get_node_by_name_or_throw(node_name);
            _final_node = n;
        }

        const node& get_final_node() const override {
            return *_final_node;
        }

        void set_word(const word<T>& word) {
            _tapes.at(0).overwrite_with_word(word);
        }

        std::array<tape<T>, N>& get_tapes() {
            return _tapes;
        }

        std::shared_ptr<tm_auto_executor<N, T>> get_concrete_executor() {
            if (_start_node == nullptr) throw std::logic_error("Start node is null.");
            if (_final_node == nullptr) throw std::logic_error("Final node is null.");

            return std::make_shared<tm_auto_executor<N, T>>(*this);
        };

        std::shared_ptr<abstract_tm_executor> get_executor() override {
            return get_concrete_executor();
        }

        std::vector<std::shared_ptr<edge<N, T>>> get_connected_edges(const node& node) {
            std::vector<std::shared_ptr<edge<N, T>>> edges;
            for (auto edge : _edges) {
                if (edge->get_from_node() == node)
                    edges.emplace_back(edge);
            }

            return edges;
        }
    };
}

#include "executor/tm_auto_executor.hpp"