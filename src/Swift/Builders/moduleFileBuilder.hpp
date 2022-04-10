#pragma once

#include "Swift/Proxy/moduleFile.hpp"
#include <IR/ir.hpp>
#include <optional>

namespace Swift::Builders {

/**
* Goes through the given namespace downwards and creates a moduleFile from it
*/
std::optional<Swift::Proxy::ModuleFile>
buildModuleFile(IR::Namespace const& rootNamespace,
                std::string const& moduleName);
}    // namespace Swift::Builders
