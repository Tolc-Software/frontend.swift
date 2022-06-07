#include "Objc/Builders/functionBuilder.hpp"
#include "Objc/Builders/typeBuilder.hpp"
#include "Objc/Proxy/function.hpp"
#include "Objc/cache.hpp"
#include "Objc/getName.hpp"
#include "ObjcSwift/Helpers/types.hpp"
#include <IR/ir.hpp>
#include <optional>

namespace Objc::Builders {

std::optional<Objc::Proxy::Function>
buildFunction(std::string const& objcClass,
              std::string const& cppClass,
              IR::Function const& cppFunction,
              Objc::Cache& cache,
              bool isConstructor,
              bool isOverloaded) {
	auto objcName = Objc::getFunctionName(cppFunction, isConstructor);
	if (isOverloaded) {
		objcName += Objc::getParameterString(cppFunction.m_arguments);
	}
	Objc::Proxy::Function objcFunction(objcName,
	                                   cppFunction.m_name,
	                                   cppFunction.m_representation,
	                                   objcClass,
	                                   cppClass);

	objcFunction.m_id = cppFunction.m_id;

	// For if there is no argument name
	size_t argNumber = 0;
	for (auto const& argument : cppFunction.m_arguments) {
		Objc::Proxy::Function::Argument arg;
		arg.m_name = argument.m_name.empty() ?
                         "arg" + std::to_string(argNumber) :
                         argument.m_name;
		arg.m_type = Objc::Builders::buildType(argument.m_type, cache);
		objcFunction.addArgument(arg);
		argNumber++;
	}

	if (isConstructor) {
		Objc::Proxy::Type returnType;
		returnType.m_name = "instancetype";
		returnType.m_conversions.m_toObjc = "";
		returnType.m_conversions.m_toCpp = "";
		objcFunction.setReturnType(returnType);
	} else {
		objcFunction.setReturnType(
		    Objc::Builders::buildType(cppFunction.m_returnType, cache));
	}
	objcFunction.setDocumentation(cppFunction.m_documentation);

	return objcFunction;
}
}    // namespace Objc::Builders
