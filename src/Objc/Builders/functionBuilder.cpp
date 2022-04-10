#include "Objc/Builders/functionBuilder.hpp"
#include "Objc/Proxy/function.hpp"
#include "ObjcSwift/Helpers/types.hpp"
#include "Swift/types.hpp"
#include <optional>
#include <spdlog/spdlog.h>

namespace Objc::Builders {

std::optional<Objc::Proxy::Function>
buildFunction(IR::Function const& cppFunction) {
	Objc::Proxy::Function middleFunction(
	    ObjcSwift::Helpers::removeCppTemplate(cppFunction.m_name),
	    cppFunction.m_representation);

	for (auto const& arg : cppFunction.m_arguments) {
		if (!ObjcSwift::Helpers::isContainerType(
		        arg.m_type, IR::ContainerType::UniquePtr)) {

			Objc::Proxy::Function::Argument proxyArg;
			proxyArg.name = arg.m_name;
			proxyArg.type = arg.m_type.m_representation;
			middleFunction.addArgument(proxyArg);
		} else {
			spdlog::error(
			    R"(The function {} takes a std::unique_ptr as one of its argument. Python cannot give up ownership of an object to a function. See https://pybind11.readthedocs.io/en/stable/advanced/smart_ptrs.html for more info.)",
			    cppFunction.m_representation);
			return std::nullopt;
		}
	}

	middleFunction.setReturnType(cppFunction.m_returnType.m_representation);
	middleFunction.setDocumentation(cppFunction.m_documentation);

	return middleFunction;
}
}    // namespace Objc::Builders
