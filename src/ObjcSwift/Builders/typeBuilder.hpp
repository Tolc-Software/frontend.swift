#pragma once

#include "ObjcSwift/Type.hpp"
#include <IR/ir.hpp>

namespace ObjcSwift::Builders {
ObjcSwift::Type toObjcSwiftType(IR::Type const& type);
}
