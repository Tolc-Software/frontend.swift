#pragma once

#include "ObjcSwift/Proxy/class.hpp"
#include "ObjcSwift/Proxy/typeInfo.hpp"
#include <IR/ir.hpp>
#include <optional>

namespace ObjcSwift::Builders {

/**
* Transforms a IR::Struct to a ObjcSwift::Proxy::Class
* NOTE: If the struct does not have an explicit constructor,
*       the default constructor is added
* Fails if any member functions takes unique_ptr as an argument
*/
std::optional<ObjcSwift::Proxy::Class>
buildClass(IR::Struct const& s, ObjcSwift::Proxy::TypeInfo& typeInfo);
}    // namespace ObjcSwift::Builders
