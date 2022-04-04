#include "ObjcSwift/Builders/attributeBuilder.hpp"
#include "ObjcSwift/Proxy/typeInfo.hpp"
#include "ObjcSwift/checkType.hpp"
#include <string>

namespace ObjcSwift::Builders {

ObjcSwift::Proxy::Attribute
buildAttribute(std::string const& parentNamespace,
               IR::Variable const& v,
               ObjcSwift::Proxy::TypeInfo& typeInfo) {
	ObjcSwift::Proxy::Attribute attr(v.m_name,
	                                 parentNamespace + "::" + v.m_name);
	ObjcSwift::checkType(v.m_type, typeInfo);

	return attr;
}
}    // namespace ObjcSwift::Builders
