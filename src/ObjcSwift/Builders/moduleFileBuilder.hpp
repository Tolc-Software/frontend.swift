#pragma once

#include "ObjcSwift/Proxy/moduleFile.hpp"
#include <IR/ir.hpp>
#include <optional>

namespace ObjcSwift::Builders {

/**
* Goes through the given namespace downwards and creates a moduleFile from it
*/
std::optional<ObjcSwift::Proxy::ModuleFile>
buildModuleFile(IR::Namespace const& rootNamespace,
                std::string const& moduleName);
}    // namespace ObjcSwift::Builders
