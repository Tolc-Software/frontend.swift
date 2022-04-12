#pragma once

#include <IR/ir.hpp>
#include <string>
#include <vector>

namespace Swift {
std::string getClassName(std::string const& cppClassName,
                         std::vector<IR::Type> const& cppTemplateArgs);

std::string getFunctionName(IR::Function const& cppFunction,
                            bool isConstructor);
}    // namespace Swift
