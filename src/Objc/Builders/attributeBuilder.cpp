#include "Objc/Builders/attributeBuilder.hpp"
#include "Objc/Builders/typeBuilder.hpp"
#include "Objc/Proxy/attribute.hpp"
#include "Objc/cache.hpp"
#include <string>

namespace Objc::Builders {

Objc::Proxy::Attribute buildAttribute(IR::Variable const& v,
                                      Objc::Cache const& cache) {
	Objc::Proxy::Attribute attr;
	attr.m_name = v.m_name;
	attr.m_documentation = v.m_documentation;
	attr.m_isConst = v.m_type.m_isConst;
	attr.m_isStatic = true;
	attr.m_type = Objc::Builders::buildType(v.m_type, cache);

	return attr;
}
}    // namespace Objc::Builders
