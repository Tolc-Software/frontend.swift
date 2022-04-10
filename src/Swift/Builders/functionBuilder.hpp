#pragma once

#include "Swift/Proxy/function.hpp"
#include <IR/ir.hpp>
#include <optional>

namespace Swift::Builders {

/**
* Transforms a IR::Function to a Swift::Proxy::Function
* Fails if any of the arguments are std::unique_ptr (https://pybind11.readthedocs.io/en/stable/advanced/smart_ptrs.html)
*/
std::optional<Swift::Proxy::Function> buildFunction(IR::Function const& fun);
}    // namespace Swift::Builders
