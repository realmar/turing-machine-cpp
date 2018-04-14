#pragma once

#include <vector>
#include <algorithm>
#include <memory>

#include "node.hpp"
#include "edge.hpp"
#include "vector_helpers.hpp"
#include "tape.hpp"
#include "typedefs.hpp"

namespace realmar::turing {
    // forward declaration
    template<int N, typename T>
    class tm_step_executor;

    template<int N, typename T>
    class turing_machine {
    private:
        std::vector<node<N>> _nodes;
        std::vector<edge<N, T>> _edges;

        const node<N>* _start_node = nullptr;
        const node<N>* _final_node = nullptr;
        std::array<tape<T>, N> _tapes;

        node<N>* get_node_by_name(const std::string& name) {
            auto result = std::find_if(_nodes.begin(), _nodes.end(),
                                       [name](const auto& n) { return n.get_name() == name; });

            return result != _nodes.end() ? &*result : nullptr;
        }

        node<N>* get_node_by_name_or_throw(const std::string& name) {
            auto node = get_node_by_name(name);
            if (node == nullptr) throw std::invalid_argument("Node not found.");

            return node;
        }

    public:
        virtual ~turing_machine() = default;

        turing_machine() = default;

        void add_node(const node<N>& node) {
            if (get_node_by_name(node.get_name()) != nullptr) {
                throw std::invalid_argument("There is already a node with this name present.");
            }

            _nodes.emplace_back(node);
        }

        void add_edge(const node<N>& node1, const node<N>& node2, const edge_args<N, T>& edge_args) {
            if (IS_IN_VECTOR(_nodes, node1) && IS_IN_VECTOR(_nodes, node2)) {
                _edges.emplace_back(edge<N, T>(node1, node2, edge_args));
            } else {
                throw std::invalid_argument("Nodes not found in turing machine. Both nodes need to be present.");
            }
        }

        void add_edge(const std::string& node1_name, const std::string& node2_name, const edge_args <N, T>& edge_args) {
            auto node1 = get_node_by_name(node1_name);
            auto node2 = get_node_by_name(node2_name);

            if (node1 == nullptr)
                throw std::invalid_argument("Node1 not found.");

            if (node2 == nullptr)
                throw std::invalid_argument("Node2 not found.");

            add_edge(*node1, *node2, edge_args);
        }

        void set_start_node(const node<N>& node) {
            set_start_node(node.get_name());
        }

        void set_start_node(const std::string& node_name) {
            auto node = get_node_by_name_or_throw(node_name);
            _start_node = node;
        }

        const node<N>& get_start_node() const {
            return *_start_node;
        }

        void set_final_node(const node<N>& node) {
            set_final_node(node.get_name());
        }

        void set_final_node(const std::string& node_name) {
            auto node = get_node_by_name_or_throw(node_name);
            _final_node = node;
        }

        const node<N>& get_final_node() const {
            return *_final_node;
        }

        void set_word(const word<T>& word) {
            _tapes.at(0).overwrite_with_word(word);
        }

        std::array<tape<T>, N>& get_tapes() {
            return _tapes;
        }

        std::shared_ptr<tm_step_executor<N, T>> get_executor() {
            if (_start_node == nullptr) throw std::logic_error("Start node is null.");
            if (_final_node == nullptr) throw std::logic_error("Final node is null.");

            return std::make_shared<tm_step_executor<N, T>>
                    (*this);
        }

        std::vector<edge<N, T>*> get_connected_edges(const node<N>& node) {
            std::vector<edge<N, T>*> edges;
            for (edge<N, T>& edge : _edges) {
                if (edge.get_from_node() == node)
                    edges.emplace_back(&edge);
            }

            return edges;
        }
    };
}

#include "tm_step_executor.hpp"