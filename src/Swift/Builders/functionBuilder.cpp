#include "Swift/Builders/functionBuilder.hpp"
#include "ObjcSwift/Helpers/types.hpp"
#include "Swift/Proxy/function.hpp"
#include "Swift/getName.hpp"
#include "Swift/types.hpp"
#include <cctype>
#include <optional>
#include <spdlog/spdlog.h>
#include <string>

namespace Swift::Builders {

std::optional<Swift::Proxy::Function>
buildFunction(IR::Function const& cppFunction, bool isConstructor) {
	Swift::Proxy::Function swiftFunction(
	    Swift::getFunctionName(cppFunction, isConstructor),
	    cppFunction.m_representation);

	for (auto const& arg : cppFunction.m_arguments) {
		Swift::Proxy::Function::Argument proxyArg;
		proxyArg.name = arg.m_name;
		proxyArg.type = toSwiftType(arg.m_type);
		swiftFunction.addArgument(proxyArg);
	}

	swiftFunction.setReturnType(toSwiftType(cppFunction.m_returnType));
	swiftFunction.setDocumentation(cppFunction.m_documentation);

	return swiftFunction;
}
}    // namespace Swift::Builders
