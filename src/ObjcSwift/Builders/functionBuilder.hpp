#pragma once

#include "ObjcSwift/Proxy/function.hpp"
#include "ObjcSwift/Proxy/typeInfo.hpp"
#include <IR/ir.hpp>
#include <optional>

namespace ObjcSwift::Builders {

/**
* Transforms a IR::Function to a ObjcSwift::Proxy::Function
* Fails if any of the arguments are std::unique_ptr (https://pybind11.readthedocs.io/en/stable/advanced/smart_ptrs.html)
*/
std::optional<ObjcSwift::Proxy::Function>
buildFunction(IR::Function const& fun, ObjcSwift::Proxy::TypeInfo& typeInfo);
}    // namespace ObjcSwift::Builders
