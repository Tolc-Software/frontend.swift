#pragma once

#include "Objc/Conversions/conversion.hpp"
#include <optional>
#include <string>
#include <vector>

namespace Objc::Proxy {
struct Type {
	std::string m_name;
	Objc::Conversions::Conversion m_conversions;
};
}    // namespace Objc::Proxy
