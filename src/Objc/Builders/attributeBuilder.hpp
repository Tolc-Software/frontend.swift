#pragma once

#include "Objc/Proxy/attribute.hpp"
#include "Objc/cache.hpp"
#include <IR/ir.hpp>

namespace Objc::Builders {

/**
* Transforms a IR::Variable to a Objc::Proxy::Attribute
*/
Objc::Proxy::Attribute buildAttribute(IR::Variable const& v,
                                      Objc::Cache& cache);
}    // namespace Objc::Builders
