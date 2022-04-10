#pragma once

#include "Swift/Builders/functionBuilder.hpp"
#include "Swift/Proxy/module.hpp"
#include <IR/ir.hpp>
#include <optional>

namespace Swift::Builders {

/**
* Build proxy module from a namespace
* NOTE: Does not traverse the structure,
*       but adds the children of ns as submodules
* Fails if any functions takes unique_ptr as an argument
*/
std::optional<Swift::Proxy::Module>
buildModule(IR::Namespace const& ns, std::string const& rootModuleName);
}    // namespace Swift::Builders
