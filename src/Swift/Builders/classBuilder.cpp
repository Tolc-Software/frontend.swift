#include "Swift/Builders/classBuilder.hpp"
#include <IR/ir.hpp>
#include <numeric>
#include <optional>
#include <set>
#include <string>

namespace Swift::Builders {

std::optional<Swift::Proxy::Class> buildClass(IR::Struct const& cppClass,
                                              std::string const& moduleName) {
	Swift::Proxy::Class swiftClass("", moduleName);

	swiftClass.setDocumentation(cppClass.m_documentation);

	swiftClass.setInherited(cppClass.m_public.m_inherited);

	return swiftClass;
}
}    // namespace Swift::Builders
