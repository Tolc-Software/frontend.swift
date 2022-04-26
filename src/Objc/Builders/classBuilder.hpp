#pragma once

#include "Objc/Proxy/class.hpp"
#include "Objc/cache.hpp"
#include <IR/ir.hpp>
#include <optional>

namespace Objc::Builders {

/**
* Transforms a IR::Struct to a Objc::Proxy::Class
* NOTE: If the struct does not have an explicit constructor,
*       the default constructor is added
* Fails if any member functions takes unique_ptr as an argument
*/
std::optional<Objc::Proxy::Class> buildClass(IR::Struct const& cppClass,
                                             std::string const& moduleName,
                                             Objc::Cache& cache);
}    // namespace Objc::Builders
