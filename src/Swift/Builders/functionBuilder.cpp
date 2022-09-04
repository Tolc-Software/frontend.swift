#include "Swift/Builders/functionBuilder.hpp"
#include "ObjcSwift/Helpers/split.hpp"
#include "Swift/Builders/typeBuilder.hpp"
#include "Swift/Proxy/function.hpp"
#include <fmt/format.h>
#include <optional>
#include <string>

namespace Swift::Builders {

struct SplitData {
	std::string m_name;
	std::string m_objcPrefix;
	std::string m_swiftPrefix;
};

SplitData getExtensionAndName(std::string const& fullyQualifiedName,
                              std::string const& libraryName) {
	SplitData data;

	auto splitted = ObjcSwift::Helpers::split(fullyQualifiedName, "::");
	if (splitted.size() == 1) {
		data.m_name = splitted[0];
		data.m_objcPrefix = libraryName;
		data.m_swiftPrefix = libraryName;
	} else {
		data.m_name = splitted.back();
		// Remove the name
		splitted.pop_back();
		splitted.push_front(libraryName);
		data.m_objcPrefix = fmt::format("{}", fmt::join(splitted, ""));
		data.m_swiftPrefix = fmt::format("{}", fmt::join(splitted, "."));
	}

	return data;
}

Swift::Proxy::Function buildFunction(Objc::Proxy::Function const& objcFunction,
                                     std::string const& libraryName) {
	fmt::print("{}\n", "Building function");
	auto splitted = getExtensionAndName(objcFunction.getCppName(), libraryName);

	Swift::Proxy::Function swiftFunction(
	    splitted.m_name, objcFunction.getName(), splitted.m_swiftPrefix);

	if (objcFunction.isStatic()) {
		swiftFunction.setAsStatic();
	}

	if (objcFunction.isStandalone()) {
		swiftFunction.setCallFrom("_" + splitted.m_objcPrefix);
	} else {
		swiftFunction.setCallFrom(
		    objcFunction.isStatic() ? "_" + splitted.m_objcPrefix : "m_object");
	}

	if (objcFunction.isConstructor()) {
		swiftFunction.setAsConstructor();
	} else {
		swiftFunction.setReturnType(buildType(objcFunction.getReturnType()));
	}

	for (auto const& argument : objcFunction.getArguments()) {
		swiftFunction.addArgument(
		    {argument.m_name, buildType(argument.m_type)});
	}

	return swiftFunction;
}
}    // namespace Swift::Builders
