#pragma once

#include "Objc/Proxy/type.hpp"
#include <IR/ir.hpp>
#include <string>

namespace Objc::Builders {
Objc::Proxy::Type buildType(IR::Type const& type,
                            std::string const& rootModuleName);
}
