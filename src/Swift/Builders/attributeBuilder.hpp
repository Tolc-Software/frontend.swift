#pragma once

#include "Swift/Proxy/attribute.hpp"
#include <IR/ir.hpp>

namespace Swift::Builders {

/**
* Transforms a IR::Variable to a Swift::Proxy::Attribute
*/
Swift::Proxy::Attribute buildAttribute(std::string const& parentNamespace,
                                       IR::Variable const& v);
}    // namespace Swift::Builders
