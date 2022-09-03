#include "Swift/Builders/classBuilder.hpp"
#include "Objc/getName.hpp"
#include "ObjcSwift/Helpers/combine.hpp"
#include "ObjcSwift/Helpers/operatorNames.hpp"
#include "ObjcSwift/Helpers/types.hpp"
#include "ObjcSwift/getOverloadedFunctions.hpp"
#include "Swift/Builders/enumBuilder.hpp"
#include "Swift/Builders/functionBuilder.hpp"
#include "Swift/Proxy/function.hpp"
#include "Swift/getName.hpp"
#include "Swift/types.hpp"
#include <IR/ir.hpp>
#include <numeric>
#include <optional>
#include <set>
#include <string>

namespace Swift::Builders {

std::optional<Swift::Proxy::Class> buildClass(IR::Struct const& cppClass,
                                              std::string const& moduleName) {
	auto objcClassName = Objc::getClassName(cppClass, moduleName);

	Swift::Proxy::Class swiftClass(
	    Swift::getClassName(cppClass.m_name, cppClass.m_templateArguments),
	    objcClassName);

	for (auto const& e : cppClass.m_public.m_enums) {
		swiftClass.addEnum(buildEnum(e));
	}

	swiftClass.setDocumentation(cppClass.m_documentation);

	swiftClass.setInherited(cppClass.m_public.m_inherited);

	return swiftClass;
}
}    // namespace Swift::Builders
