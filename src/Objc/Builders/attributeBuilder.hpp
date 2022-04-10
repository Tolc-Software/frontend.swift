#pragma once

#include "Objc/Proxy/attribute.hpp"
#include <IR/ir.hpp>

namespace Objc::Builders {

/**
* Transforms a IR::Variable to a Objc::Proxy::Attribute
*/
Objc::Proxy::Attribute buildAttribute(std::string const& parentNamespace,
                                      IR::Variable const& v);
}    // namespace Objc::Builders
