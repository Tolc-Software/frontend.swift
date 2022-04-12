#pragma once

#include "Swift/Proxy/class.hpp"
#include <IR/ir.hpp>
#include <optional>
#include <string>

namespace Swift::Builders {

/**
* Transforms a IR::Struct to a Swift::Proxy::Class
* NOTE: If the struct does not have an explicit constructor,
*       the default constructor is added
* Fails if any member functions takes unique_ptr as an argument
*/
std::optional<Swift::Proxy::Class> buildClass(IR::Struct const& s,
                                              std::string const& moduleName);
}    // namespace Swift::Builders
