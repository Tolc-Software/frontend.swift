#include "ObjcSwift/Builders/functionBuilder.hpp"
#include "ObjcSwift/Helpers/types.hpp"
#include "ObjcSwift/Proxy/function.hpp"
#include "ObjcSwift/Proxy/typeInfo.hpp"
#include "ObjcSwift/checkType.hpp"
#include <optional>
#include <spdlog/spdlog.h>

namespace ObjcSwift::Builders {

std::optional<ObjcSwift::Proxy::Function>
buildFunction(IR::Function const& cppFunction,
              ObjcSwift::Proxy::TypeInfo& typeInfo) {
	ObjcSwift::Proxy::Function pyFunction(
	    ObjcSwift::Helpers::removeCppTemplate(cppFunction.m_name),
	    cppFunction.m_representation);

	for (auto const& arg : cppFunction.m_arguments) {
		if (!ObjcSwift::Helpers::isContainerType(
		        arg.m_type, IR::ContainerType::UniquePtr)) {
			ObjcSwift::checkType(arg.m_type, typeInfo);

			pyFunction.addArgument(arg.m_type.m_representation, arg.m_name);
		} else {
			spdlog::error(
			    R"(The function {} takes a std::unique_ptr as one of its argument. Python cannot give up ownership of an object to a function. See https://pybind11.readthedocs.io/en/stable/advanced/smart_ptrs.html for more info.)",
			    cppFunction.m_representation);
			return std::nullopt;
		}
	}

	ObjcSwift::checkType(cppFunction.m_returnType, typeInfo);
	pyFunction.setReturnType(cppFunction.m_returnType.m_representation);
	pyFunction.setDocumentation(cppFunction.m_documentation);

	if (cppFunction.m_returnType.m_numPointers > 0) {
		pyFunction.setReturnValuePolicy(
		    ObjcSwift::Proxy::Function::return_value_policy::reference);
	}

	return pyFunction;
}
}    // namespace ObjcSwift::Builders
