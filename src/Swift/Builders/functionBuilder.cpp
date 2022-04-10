#include "Swift/Builders/functionBuilder.hpp"
#include "ObjcSwift/Helpers/types.hpp"
#include "Swift/Proxy/function.hpp"
#include "Swift/types.hpp"
#include <optional>
#include <spdlog/spdlog.h>

namespace Swift::Builders {

std::optional<Swift::Proxy::Function>
buildFunction(IR::Function const& cppFunction) {
	Swift::Proxy::Function middleFunction(
	    ObjcSwift::Helpers::removeCppTemplate(cppFunction.m_name),
	    cppFunction.m_representation);

	for (auto const& arg : cppFunction.m_arguments) {
		if (!ObjcSwift::Helpers::isContainerType(
		        arg.m_type, IR::ContainerType::UniquePtr)) {
			Swift::Proxy::Function::Argument proxyArg;
			proxyArg.name = arg.m_name;
			proxyArg.type = toSwiftType(arg.m_type);
			middleFunction.addArgument(proxyArg);
		} else {
			spdlog::error(
			    R"(The function {} takes a std::unique_ptr as one of its argument. Python cannot give up ownership of an object to a function. See https://pybind11.readthedocs.io/en/stable/advanced/smart_ptrs.html for more info.)",
			    cppFunction.m_representation);
			return std::nullopt;
		}
	}

	middleFunction.setReturnType(toSwiftType(cppFunction.m_returnType));
	middleFunction.setDocumentation(cppFunction.m_documentation);

	return middleFunction;
}
}    // namespace Swift::Builders
