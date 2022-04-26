#pragma once

#include "Objc/Builders/functionBuilder.hpp"
#include "Objc/Proxy/module.hpp"
#include "Objc/cache.hpp"
#include <IR/ir.hpp>
#include <optional>

namespace Objc::Builders {

/**
* Build proxy module from a namespace
* NOTE: Does not traverse the structure,
*       but adds the children of ns as submodules
* Fails if any functions takes unique_ptr as an argument
*/
std::optional<Objc::Proxy::Module>
buildModule(IR::Namespace const& ns,
            std::string const& rootModuleName,
            Objc::Cache& cache);
}    // namespace Objc::Builders
