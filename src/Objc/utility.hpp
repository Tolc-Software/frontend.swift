#pragma once

#include <string>
#include <string_view>

namespace Objc {

// A category is an extension of a class in Objective-C
std::string
getCategoryName(char prefix, std::string_view objcClass, std::string_view name);

std::string wrapInInterface(std::string_view objcClass,
                            std::string_view category,
                            std::string_view code);

std::string wrapInImplementation(std::string_view objcClass,
                                 std::string_view category,
                                 std::string_view code);
}
