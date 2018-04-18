#include <iostream>
#include <dirent.h>
#include <algorithm>
#include <fstream>

#include "presentation/utils.hpp"
#include "machines/tm_provider_collection.hpp"

using namespace realmar::turing;

void print_usage() {
    std::cout << "Usage: tm_graph [DIRECTORY]" << std::endl;
    std::cout << "Generate dot files from turing machines. Those files are stored in [DIRECTORY]." << std::endl;
}

bool check_if_directory_exists(const char* dir) {
    DIR* handle = opendir(dir);
    bool exists = false;
    if (handle) exists = true;
    closedir(handle);

    return exists;
}

bool validate_sanity_of_args(const int& argc, char** argv) {
    if (argc != 2) {
        print_usage();
        return false;
    }

    if (!check_if_directory_exists(argv[1])) {
        std::cout << "Given path is not a directory" << std::endl;
    }

    return true;
}


const char* dot_file_template = R"(// Generated file
// Turing Machine name: {name}
digraph subtract_machine {
    rankdir=LR;
    size="20"

    node [shape = point ]; q_start
    node [shape = doublecircle]; {node_final};

    node [shape = circle];
    q_start -> {node_start};
{edges}}
)";

const char* dot_edge = "    {node1} -> {node2} [ label = \"{label}\" ];\n";

const char* dot_empty_symbol = "⌊⌋";

// https://stackoverflow.com/questions/3418231/replace-part-of-a-string-with-another-string
bool replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if (start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

std::string to_string(const symbol<std::string>& symbol) {
    if (symbol.get_symbol() == nullptr) {
        return dot_empty_symbol;
    } else {
        return *symbol.get_symbol();
    }
}

bool write_to_file(const std::string& path, const std::string& contents) {
    std::fstream stream;
    stream.open(path, std::ios::out | std::ios::trunc);
    stream << contents;
    stream.close();
}

const char*
get_platform_path_separator() {
// https://stackoverflow.com/questions/3213037/determine-if-linux-or-windows-in-c
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    return "\\";
#else
    return "/";
#endif
}

std::string generate_label(const std::shared_ptr<abstract_edge>& edge) {
    std::string read, write, move;

    for (auto&& item : edge->get_read_symbols_as_string()) read += to_string(item);
    for (auto&& item : edge->get_write_symbols_as_string()) write += to_string(item);
    for (auto&& item : edge->get_move_directions_as_vector()) {
        switch (item) {
            case right:
                move += "R";
                break;
            case left:
                move += "L";
                break;
            case stale:
                move += "S";
                break;
        }
    }

    std::string label = read;
    label += "/";
    label += write;
    label += ",";
    label += move;

    return label;
}

struct aggregated_edge final {
    std::shared_ptr<node> from_node;
    std::shared_ptr<node> to_node;
    std::vector<std::string> labels;
};

// generate dot files of all turing machines
int main(int argc, char** argv) {
    print_copyright();
    if (!validate_sanity_of_args(argc, argv)) return 1;

    std::string out_directory = argv[1];

    tm_provider_collection provider_collection;
    auto providers = provider_collection.get_providers();

    for (auto&& provider : providers) {
        auto tm = provider->get_abstract_tm();

        auto start_node = tm->get_start_node();
        auto final_node = tm->get_final_node();
        auto nodes = tm->get_nodes();
        auto edges = tm->get_edges();

        std::string contents = dot_file_template;
        std::string dot_edges;

        replace(contents, "{name}", provider->get_tm_name());
        replace(contents, "{node_start}", start_node->get_name());
        replace(contents, "{node_final}", final_node->get_name());

        std::vector<aggregated_edge> a_edges;
        for (auto&& edge : edges) {
            auto result = std::find_if(a_edges.begin(), a_edges.end(), [&edge](const auto& x) {
                return x.from_node == edge->get_from_node() && x.to_node == edge->get_to_node();
            });

            auto label = generate_label(edge);

            if (result != a_edges.end()) {
                result->labels.emplace_back(label);
            } else {
                a_edges.emplace_back(aggregated_edge{
                        edge->get_from_node(),
                        edge->get_to_node(),
                        {label}
                });
            }
        }

        for (auto&& edge : a_edges) {
            std::string de = dot_edge;

            replace(de, "{node1}", edge.from_node->get_name());
            replace(de, "{node2}", edge.to_node->get_name());

            std::string label;
            for (auto&& l : edge.labels) {
                label += l;
                label += "\\n";
            }
            replace(de, "{label}", label);
            dot_edges += de;
        }

        replace(contents, "{edges}", dot_edges);

        std::locale loc;
        std::string filename;
        for (auto&& elem : provider->get_tm_name()) {
            auto lower = std::tolower(elem, loc);
            filename.append(sizeof(lower), lower);
        }
        replace(filename, " ", "_");
        filename.append(".dot");

        std::string fullpath = out_directory;
        fullpath += get_platform_path_separator();
        fullpath += filename;

        write_to_file(fullpath, contents);
    }

    return 0;
}