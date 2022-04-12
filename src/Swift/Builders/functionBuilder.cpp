#include "Swift/Builders/functionBuilder.hpp"
#include "Objc/getName.hpp"
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
		if (!ObjcSwift::Helpers::isContainerType(
		        arg.m_type, IR::ContainerType::UniquePtr)) {
			Swift::Proxy::Function::Argument proxyArg;
			proxyArg.name = arg.m_name;
			proxyArg.type = toSwiftType(arg.m_type);
			swiftFunction.addArgument(proxyArg);
		} else {
			spdlog::error(
			    R"(The function {} takes a std::unique_ptr as one of its argument. Python cannot give up ownership of an object to a function. See https://pybind11.readthedocs.io/en/stable/advanced/smart_ptrs.html for more info.)",
			    cppFunction.m_representation);
			return std::nullopt;
		}
	}

	if (isConstructor && !cppFunction.m_arguments.empty()) {
		auto params = Objc::getParameterString(cppFunction.m_arguments);
		if (!params.empty()) {
			params[0] = static_cast<char>(tolower(params[0]));
			swiftFunction.setObjcFirstConstructorParameter(params);
		}
	}

	swiftFunction.setReturnType(toSwiftType(cppFunction.m_returnType));
	swiftFunction.setDocumentation(cppFunction.m_documentation);

	return swiftFunction;
}
}    // namespace Swift::Builders
