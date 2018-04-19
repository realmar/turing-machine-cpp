#pragma once

#include <vector>
#include "../graph/symbol.hpp"

namespace realmar::turing {
    template<typename T>
    using word = std::vector<symbol<T>>;
}
