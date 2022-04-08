#include "ObjcSwift/Builders/functionBuilder.hpp"
#include "ObjcSwift/Builders/typeBuilder.hpp"
#include "ObjcSwift/Helpers/types.hpp"
#include "ObjcSwift/Proxy/function.hpp"
#include "ObjcSwift/Proxy/typeInfo.hpp"
#include "ObjcSwift/checkType.hpp"
#include "Swift/types.hpp"
#include <optional>
#include <spdlog/spdlog.h>

namespace ObjcSwift::Builders {

std::optional<ObjcSwift::Proxy::Function>
buildFunction(IR::Function const& cppFunction,
              ObjcSwift::Proxy::TypeInfo& typeInfo) {
	ObjcSwift::Proxy::Function middleFunction(
	    ObjcSwift::Helpers::removeCppTemplate(cppFunction.m_name),
	    cppFunction.m_representation);

	for (auto const& arg : cppFunction.m_arguments) {
		if (!ObjcSwift::Helpers::isContainerType(
		        arg.m_type, IR::ContainerType::UniquePtr)) {
			ObjcSwift::checkType(arg.m_type, typeInfo);

			ObjcSwift::Proxy::Function::Argument proxyArg;
			proxyArg.name = arg.m_name;
			proxyArg.type = toObjcSwiftType(arg.m_type);
			middleFunction.addArgument(proxyArg);
		} else {
			spdlog::error(
			    R"(The function {} takes a std::unique_ptr as one of its argument. Python cannot give up ownership of an object to a function. See https://pybind11.readthedocs.io/en/stable/advanced/smart_ptrs.html for more info.)",
			    cppFunction.m_representation);
			return std::nullopt;
		}
	}

	ObjcSwift::checkType(cppFunction.m_returnType, typeInfo);

	middleFunction.setReturnType(toObjcSwiftType(cppFunction.m_returnType));
	middleFunction.setDocumentation(cppFunction.m_documentation);

	return middleFunction;
}
}    // namespace ObjcSwift::Builders
