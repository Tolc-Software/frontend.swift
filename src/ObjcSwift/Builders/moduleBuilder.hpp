#pragma once

#include "ObjcSwift/Builders/functionBuilder.hpp"
#include "ObjcSwift/Proxy/module.hpp"
#include "ObjcSwift/Proxy/typeInfo.hpp"
#include <IR/ir.hpp>
#include <optional>

namespace ObjcSwift::Builders {

/**
* Build proxy module from a namespace
* NOTE: Does not traverse the structure,
*       but adds the children of ns as submodules
* Fails if any functions takes unique_ptr as an argument
*/
std::optional<ObjcSwift::Proxy::Module>
buildModule(IR::Namespace const& ns,
            std::string const& rootModuleName,
            ObjcSwift::Proxy::TypeInfo& typeInfo);
}    // namespace ObjcSwift::Builders
