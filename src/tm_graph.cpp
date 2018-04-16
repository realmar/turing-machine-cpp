#include <iostream>
#include <dirent.h>

#include "presentation/utils.hpp"
#include "machines/tm_provider_factory.hpp"

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
digraph subtract_machine {
    rankdir=LR;
    size="20"

    node [shape = point ]; q_start
    node [shape = doublecircle]; {node_start};

    node [shape = circle];
    q_start -> {node_start};
    {edges}
}
)";

const char* dot_edge = "{node1} -> {node2} [ label = \"{label}\" ];";

const char* dot_empty_symbol = "⌊⌋";

// generate dot files of all turing machines
int main(int argc, char** argv) {
    std::cout << dot_file_template << std::endl;

    print_copyright();
    if (!validate_sanity_of_args(argc, argv)) return 1;

    auto providers = tm_provider_factory::get_providers();


    return 0;
}