#include "Objc/Builders/attributeBuilder.hpp"
#include <string>

namespace Objc::Builders {

Objc::Proxy::Attribute buildAttribute(std::string const& parentNamespace,
                                      IR::Variable const& v) {
	Objc::Proxy::Attribute attr(v.m_name, parentNamespace + "::" + v.m_name);

	return attr;
}
}    // namespace Objc::Builders
