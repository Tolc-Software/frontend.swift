#pragma once

#include "ObjcSwift/Proxy/typeInfo.hpp"
#include <IR/ir.hpp>
#include <queue>
#include <set>
#include <string>

namespace ObjcSwift {

// Fill TypeInfo related to input type
// Should be called once per type in the whole interface
void checkType(IR::Type const& type, ObjcSwift::Proxy::TypeInfo& info);

}    // namespace ObjcSwift
