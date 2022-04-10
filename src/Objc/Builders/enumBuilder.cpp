#include "Objc/Builders/enumBuilder.hpp"
#include <string>

namespace Objc::Builders {

Objc::Proxy::Enum buildEnum(IR::Enum const& e) {
	Objc::Proxy::Enum proxyEnum(e.m_name, e.m_representation);

	proxyEnum.setScoped(e.m_isScoped);
	proxyEnum.setDocumentation(e.m_documentation);

	for (auto const& value : e.m_values) {
		proxyEnum.addValue(value);
	}

	return proxyEnum;
}
}    // namespace Objc::Builders
