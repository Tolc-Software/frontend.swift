#include "Swift/Builders/functionBuilder.hpp"
#include "ObjcSwift/Helpers/split.hpp"
#include "ObjcSwift/Helpers/types.hpp"
#include "Swift/Proxy/function.hpp"
#include "Swift/getName.hpp"
#include "Swift/types.hpp"
#include <fmt/format.h>
#include <optional>
#include <string>

namespace Swift::Builders {

std::pair<std::string, std::string>
getExtensionAndName(std::string const& fullyQualifiedName,
                    std::string const& libraryName) {
	auto splitted = ObjcSwift::Helpers::split(fullyQualifiedName, "::");
	if (splitted.size() == 1) {
		return {libraryName, splitted[0]};
	}

	auto name = splitted.back();
	// Remove the name
	splitted.pop_back();
	splitted.push_front(libraryName);
	return {fmt::format("{}", fmt::join(splitted, ".")), name};
}

Swift::Proxy::Function buildFunction(Objc::Proxy::Function const& objcFunction,
                                     std::string const& libraryName) {
	fmt::print("{}\n", "Building function");
	auto [extension, name] =
	    getExtensionAndName(objcFunction.getCppName(), libraryName);
	fmt::print("{}\n", extension);
	fmt::print("{}\n", name);

	Swift::Proxy::Function swiftFunction(
	    name, objcFunction.getName(), extension);

	if (objcFunction.isStatic()) {
		swiftFunction.setAsStatic();
	}

	if (objcFunction.isConstructor()) {
		swiftFunction.setAsConstructor();
	} else {
	}

	return swiftFunction;
}
}    // namespace Swift::Builders
