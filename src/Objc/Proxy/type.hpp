#pragma once

#include "Objc/Conversions/conversion.hpp"
#include <functional>
#include <optional>
#include <string>
#include <vector>

namespace Objc::Proxy {
struct Type {
	// Function that returns the name of the type.
	// The Objective-C type might not be known
	// When creating this type, therefore a function
	std::function<std::string()> m_name;
	// Might be '*' if the type needs
	// to be dereferenced to get a standard const&
	std::string m_dereference;
	Objc::Conversions::Conversion m_conversions;
};
}    // namespace Objc::Proxy
