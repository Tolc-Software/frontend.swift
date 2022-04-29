#pragma once

#include <IR/ir.hpp>
#include <string>
#include <vector>

namespace Objc {
std::string getClassName(IR::Struct const& cppClass,
                         std::string const& moduleName);

std::string getFunctionName(IR::Function const& cppFunction,
                            bool isConstructor);

// Joins a set of arguments as a string
std::string getParameterString(std::vector<IR::Argument> const& parameters);

std::string getEnumName(std::string const& qualifiedEnumName,
                        std::string const& moduleName);

std::string getContainerName(IR::ContainerType const& containerType);
}    // namespace Objc
