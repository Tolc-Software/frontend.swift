#include "Objc/Builders/attributeBuilder.hpp"
#include "Objc/Builders/typeBuilder.hpp"
#include "Objc/Proxy/attribute.hpp"
#include "Objc/cache.hpp"
#include <string>
#include <variant>

namespace Objc::Builders {

bool isObject(IR::Type const& type) {
	using IR::Type;
	if (auto baseType = std::get_if<Type::Value>(&type.m_type)) {
		switch (baseType->m_base) {
			case IR::BaseType::FilesystemPath:
			case IR::BaseType::String:
			case IR::BaseType::StringView: return true;
			default: return false;
		}
	}
	return std::holds_alternative<Type::Container>(type.m_type) ||
	       std::holds_alternative<Type::UserDefined>(type.m_type) ||
	       std::holds_alternative<Type::Function>(type.m_type);
}

Objc::Proxy::Attribute buildAttribute(IR::Variable const& v,
                                      Objc::Cache& cache) {
	Objc::Proxy::Attribute attr;
	attr.m_name = v.m_name;
	attr.m_documentation = v.m_documentation;
	attr.m_isConst = v.m_type.m_isConst;
	attr.m_isStatic = true;
	attr.m_type = Objc::Builders::buildType(v.m_type, cache);
	attr.m_isObject = isObject(v.m_type);

	return attr;
}
}    // namespace Objc::Builders
