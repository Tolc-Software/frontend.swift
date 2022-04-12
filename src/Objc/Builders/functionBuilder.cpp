#include "Objc/Builders/functionBuilder.hpp"
#include "Objc/Proxy/function.hpp"
#include "Objc/getName.hpp"
#include "Objc/types.hpp"
#include "ObjcSwift/Helpers/types.hpp"
#include "Swift/types.hpp"
#include <optional>
#include <spdlog/spdlog.h>

namespace Objc::Builders {

std::optional<Objc::Proxy::Function>
buildFunction(IR::Function const& cppFunction, bool isConstructor) {
	Objc::Proxy::Function objcFunction(
	    Objc::getFunctionName(cppFunction, isConstructor),
	    cppFunction.m_representation);

	for (auto const& arg : cppFunction.m_arguments) {
		if (!ObjcSwift::Helpers::isContainerType(
		        arg.m_type, IR::ContainerType::UniquePtr)) {

			Objc::Proxy::Function::Argument proxyArg;
			proxyArg.name = arg.m_name;
			proxyArg.type = Objc::toObjcType(arg.m_type);
			objcFunction.addArgument(proxyArg);
		} else {
			spdlog::error(
			    R"(The function {} takes a std::unique_ptr as one of its argument. Python cannot give up ownership of an object to a function. See https://pybind11.readthedocs.io/en/stable/advanced/smart_ptrs.html for more info.)",
			    cppFunction.m_representation);
			return std::nullopt;
		}
	}

	objcFunction.setReturnType(isConstructor ?
                                   "instancetype" :
                                   Objc::toObjcType(cppFunction.m_returnType));
	objcFunction.setDocumentation(cppFunction.m_documentation);

	return objcFunction;
}
}    // namespace Objc::Builders
