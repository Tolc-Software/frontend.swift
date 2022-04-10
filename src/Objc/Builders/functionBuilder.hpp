#pragma once

#include "Objc/Proxy/function.hpp"
#include <IR/ir.hpp>
#include <optional>

namespace Objc::Builders {

/**
* Transforms a IR::Function to a Objc::Proxy::Function
* Fails if any of the arguments are std::unique_ptr (https://pybind11.readthedocs.io/en/stable/advanced/smart_ptrs.html)
*/
std::optional<Objc::Proxy::Function> buildFunction(IR::Function const& fun);
}    // namespace Objc::Builders
