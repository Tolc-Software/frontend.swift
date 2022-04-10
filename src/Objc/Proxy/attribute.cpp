#include "Objc/Proxy/attribute.hpp"
#include <fmt/format.h>
#include <string>

namespace Objc::Proxy {

Attribute::Attribute(std::string const& name,
                     std::string const& fullyQualifiedName)
    : m_name(name), m_fullyQualifiedName(fullyQualifiedName) {}

std::string Attribute::getObjc() const {
	return fmt::format("\tattr(\"{name}\") = &{fullyQualifiedName};\n",
	                   fmt::arg("name", m_name),
	                   fmt::arg("fullyQualifiedName", m_fullyQualifiedName));
}
}    // namespace Objc::Proxy
