#pragma once

#include "Objc/Conversions/conversion.hpp"
#include <optional>
#include <string>
#include <vector>

namespace Objc::Proxy {
struct Type {
	std::string m_name;
	// Might be '*' if the type needs
	// to be dereferenced to get a standard const&
	std::string m_dereference;
	Objc::Conversions::Conversion m_conversions;
};
}    // namespace Objc::Proxy
