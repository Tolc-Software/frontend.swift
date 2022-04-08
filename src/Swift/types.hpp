#pragma once

#include <IR/ir.hpp>
#include <string>

namespace Swift {
std::string toSwiftType(IR::Type const& type);
}
