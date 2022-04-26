#pragma once

#include <optional>
#include <string>
#include <vector>

namespace Objc::Proxy {
struct Type {
	std::string m_name;
	std::string m_toObjc;
	std::string m_toCpp;
};
}    // namespace Objc::Proxy
