#include "Swift/Builders/classBuilder.hpp"
#include "Swift/Builders/functionBuilder.hpp"
#include "Swift/getName.hpp"
#include <string>

namespace Swift::Builders {

Swift::Proxy::Class buildClass(Objc::Proxy::Class const& objcClass,
                               std::string const& moduleName) {
	auto splitted =
	    Swift::splitIntoNames(objcClass.getCppClassName(), moduleName);

	Swift::Proxy::Class swiftClass(
	    splitted.m_name, objcClass.getName(), splitted.m_swiftPrefix);

	for (auto const& constructor : objcClass.getConstructors()) {
		swiftClass.addConstructor(buildFunction(constructor, moduleName));
	}

	for (auto const& function : objcClass.getFunctions()) {
		swiftClass.addFunction(buildFunction(function, moduleName));
	}

	return swiftClass;
}
}    // namespace Swift::Builders
