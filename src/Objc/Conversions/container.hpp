#pragma once

#include "Objc/Conversions/conversion.hpp"
#include "Objc/cache.hpp"
#include <IR/ir.hpp>

namespace Objc::Conversions {

Conversion getContainerTypeConversions(IR::Type const& type,
                                       Objc::Cache& cache);
}    // namespace Objc::Conversions
