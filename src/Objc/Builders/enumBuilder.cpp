#include "Objc/Builders/enumBuilder.hpp"
#include "Objc/Conversions/enum.hpp"
#include "Objc/cache.hpp"
#include "Objc/getName.hpp"
#include <string>

namespace Objc::Builders {

Objc::Proxy::Enum buildEnum(IR::Enum const& e,
                            Objc::Cache& cache) {
	auto enumName = Objc::getEnumName(e.m_representation, cache.m_moduleName);
	Objc::Proxy::Enum proxyEnum(enumName);

	proxyEnum.setScoped(e.m_isScoped);
	proxyEnum.setDocumentation(e.m_documentation);

	for (auto const& value : e.m_values) {
		proxyEnum.addValue(enumName + value);
	}

	auto [toCpp, toObjc] = Objc::Conversions::getEnumConversions(e, proxyEnum);
	cache.m_enumConversions.m_toCpp[e.m_representation] =
	    cache.m_extraFunctionsNamespace + "::" + toCpp.first;
	cache.m_extraFunctions.push_back(toCpp.second);

	cache.m_enumConversions.m_toObjc[enumName] =
	    cache.m_extraFunctionsNamespace + "::" + toObjc.first;
	cache.m_extraFunctions.push_back(toObjc.second);

	return proxyEnum;
}
}    // namespace Objc::Builders
