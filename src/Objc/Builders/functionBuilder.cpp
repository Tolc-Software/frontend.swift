#include "Objc/Builders/functionBuilder.hpp"
#include "Objc/Builders/typeBuilder.hpp"
#include "Objc/Proxy/function.hpp"
#include "Objc/cache.hpp"
#include "Objc/getName.hpp"
#include "ObjcSwift/Helpers/types.hpp"
#include "Swift/types.hpp"
#include <IR/ir.hpp>
#include <iostream>
#include <optional>
#include <spdlog/spdlog.h>
#include <variant>

namespace Objc::Builders {

std::string getConversion(std::map<std::string, std::string> const& m,
                          std::string const& key) {
	if (auto it = m.find(key); it != m.end()) {
		return it->second;
	}
	return "";
}

std::string getFromObjcToCppConversion(IR::Type const& type,
                                       Objc::Cache const& cache) {
	using IR::Type;
	if (auto e = std::get_if<IR::Type::EnumValue>(&type.m_type)) {
		std::cout << "Is an enum" << '\n';
		auto conv =
		    getConversion(cache.m_enumConversions.m_toCpp, e->m_representation);
		std::cout << "Conversion is: " << conv << '\n';
		return conv;
	}

	return "";
}

std::string getCallArguments(std::vector<IR::Argument> const& arguments,
                             Objc::Cache& cache) {
	// Get the typenames of the arguments
	std::vector<std::string> names;
	std::transform(arguments.begin(),
	               arguments.end(),
	               std::back_inserter(names),
	               [&](auto const& argument) {
		auto conversion = getFromObjcToCppConversion(argument.m_type, cache);
		return fmt::format(
		    "{conversion}{leftBracket}{name}{rightBracket}",
		    fmt::arg("conversion", conversion),
		    fmt::arg("leftBracket", conversion.empty() ? "" : "("),
		    fmt::arg("name", argument.m_name),
		    fmt::arg("rightBracket", conversion.empty() ? "" : ")"));
                   });
	return fmt::format("{}", fmt::join(names, ", "));
}

std::string getDeclarationArguments(std::vector<IR::Argument> const& arguments,
                                    std::string const& moduleName) {
	// (int)x y:(int)y z:(int)z
	// Get the typenames of the arguments
	// The first one doesn't start with a name
	bool isFirst = true;
	std::string out;
	for (auto const& arg : arguments) {
		if (!isFirst) {
			out += arg.m_name + ':';
		}
		isFirst = false;
		out += fmt::format(
		    "({type}){name} ",
		    fmt::arg("type",
		             Objc::Builders::buildType(arg.m_type, moduleName).m_name),
		    fmt::arg("name", arg.m_name));
	}
	if (!arguments.empty()) {
		// Remove the last space
		out.pop_back();
	}
	return out;
}

std::optional<Objc::Proxy::Function>
buildFunction(IR::Function const& cppFunction,
              std::string const& moduleName,
              Objc::Cache& cache,
              bool isConstructor) {
	Objc::Proxy::Function objcFunction(
	    Objc::getFunctionName(cppFunction, isConstructor),
	    cppFunction.m_representation);

	objcFunction.setObjCDeclarationArguments(
	    getDeclarationArguments(cppFunction.m_arguments, moduleName));
	auto callargs = getCallArguments(cppFunction.m_arguments, cache);
	std::cout << callargs << '\n';
	objcFunction.setCppCallArguments(callargs);

	if (isConstructor) {
		Objc::Proxy::Type returnType;
		returnType.m_name = "instancetype";
		returnType.m_toObjc = "";
		returnType.m_toCpp = "";
		objcFunction.setReturnType(returnType);
	} else {
		objcFunction.setReturnType(
		    Objc::Builders::buildType(cppFunction.m_returnType, moduleName));
	}
	objcFunction.setDocumentation(cppFunction.m_documentation);

	return objcFunction;
}
}    // namespace Objc::Builders
