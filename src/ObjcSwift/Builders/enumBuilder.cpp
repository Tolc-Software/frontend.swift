#include "ObjcSwift/Builders/enumBuilder.hpp"
#include <string>

namespace ObjcSwift::Builders {

ObjcSwift::Proxy::Enum buildEnum(IR::Enum const& e) {
	ObjcSwift::Proxy::Enum proxyEnum(e.m_name, e.m_representation);

	proxyEnum.setScoped(e.m_isScoped);
	proxyEnum.setDocumentation(e.m_documentation);

	for (auto const& value : e.m_values) {
		proxyEnum.addValue(value);
	}

	return proxyEnum;
}
}    // namespace ObjcSwift::Builders
