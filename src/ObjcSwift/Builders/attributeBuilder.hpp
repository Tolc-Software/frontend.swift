#pragma once

#include "ObjcSwift/Proxy/attribute.hpp"
#include "ObjcSwift/Proxy/typeInfo.hpp"
#include <IR/ir.hpp>

namespace ObjcSwift::Builders {

/**
* Transforms a IR::Variable to a ObjcSwift::Proxy::Attribute
*/
ObjcSwift::Proxy::Attribute
buildAttribute(std::string const& parentNamespace,
               IR::Variable const& v,
               ObjcSwift::Proxy::TypeInfo& typeInfo);
}    // namespace ObjcSwift::Builders
