#include "Swift/Builders/attributeBuilder.hpp"
#include <string>

namespace Swift::Builders {

Swift::Proxy::Attribute buildAttribute(std::string const& parentNamespace,
                                       IR::Variable const& v) {
	Swift::Proxy::Attribute attr(v.m_name, parentNamespace + "::" + v.m_name);

	return attr;
}
}    // namespace Swift::Builders
