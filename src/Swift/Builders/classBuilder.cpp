#include "Swift/Builders/classBuilder.hpp"
#include "ObjcSwift/Helpers/combine.hpp"
#include "ObjcSwift/Helpers/operatorNames.hpp"
#include "ObjcSwift/Helpers/types.hpp"
#include "ObjcSwift/getOverloadedFunctions.hpp"
#include "Swift/Builders/enumBuilder.hpp"
#include "Swift/Builders/functionBuilder.hpp"
#include "Swift/Builders/typeToStringBuilder.hpp"
#include "Swift/Proxy/function.hpp"
#include <IR/ir.hpp>
#include <numeric>
#include <optional>
#include <set>
#include <string>

namespace Swift::Builders {

namespace {

std::string
getTemplateParameterString(std::vector<IR::Type> const& parameters) {
	return std::accumulate(parameters.begin(),
	                       parameters.end(),
	                       std::string() /* Start with empty string */,
	                       [](std::string soFar, IR::Type const& current) {
		                       return std::move(soFar) + "_" +
		                              buildTypeString(current);
	                       });
}

void buildMemberFunction(Swift::Proxy::Function& pyFunction,
                         IR::Function const& cppFunction,
                         std::set<std::string> const& overloadedFunctions) {
	if (cppFunction.m_isStatic) {
		pyFunction.setAsStatic();
	}

	if (overloadedFunctions.find(cppFunction.m_representation) !=
	    overloadedFunctions.end()) {
		pyFunction.setAsOverloaded();
	}
}

struct TrampolineFunctions {
	std::vector<Swift::Proxy::Function> virtualFunctions;
	std::vector<Swift::Proxy::Function> pureVirtualFunctions;
};

}    // namespace

std::optional<Swift::Proxy::Class> buildClass(IR::Struct const& cppClass) {
	Swift::Proxy::Class pyClass(
	    ObjcSwift::Helpers::removeCppTemplate(cppClass.m_name) +
	        getTemplateParameterString(cppClass.m_templateArguments),
	    cppClass.m_representation);

	pyClass.setDocumentation(cppClass.m_documentation);

	pyClass.setInherited(cppClass.m_public.m_inherited);

	// Ignore private functions
	auto overloadedFunctions =
	    ObjcSwift::getOverloadedFunctions(cppClass.m_public.m_functions);
	for (auto const& function : cppClass.m_public.m_functions) {
		if (auto maybePyFunction = buildFunction(function)) {
			auto& pyFunction = maybePyFunction.value();

			buildMemberFunction(pyFunction, function, overloadedFunctions);

			pyClass.addFunction(pyFunction);
		} else {
			return std::nullopt;
		}
	}

	auto overloadedOperators =
	    ObjcSwift::getOverloadedFunctions(cppClass.m_public.m_operators);
	for (auto const& [op, function] : cppClass.m_public.m_operators) {
		if (auto maybePyFunction = buildFunction(function)) {
			if (auto maybeName = ObjcSwift::Helpers::getOperatorName(op)) {
				auto& pyFunction = maybePyFunction.value();

				buildMemberFunction(pyFunction, function, overloadedOperators);

				pyClass.addFunction(pyFunction);
			}
		} else {
			return std::nullopt;
		}
	}

	for (auto const& constructor : cppClass.m_public.m_constructors) {
		if (auto maybePyFunction = buildFunction(constructor)) {
			auto& pyFunction = maybePyFunction.value();

			if (constructor.m_isStatic) {
				pyFunction.setAsStatic();
			}

			if (cppClass.m_public.m_constructors.size() > 1) {
				pyFunction.setAsOverloaded();
			}

			pyFunction.setAsConstructor();

			pyClass.addConstructor(pyFunction);
		}
	}

	for (auto const& variable : cppClass.m_public.m_memberVariables) {
		pyClass.addMemberVariable(variable.m_name,
		                          variable.m_documentation,
		                          variable.m_type.m_isConst,
		                          variable.m_type.m_isStatic);
	}

	// Add default constructor
	if (cppClass.m_hasImplicitDefaultConstructor) {
		auto constructor =
		    Swift::Proxy::Function(pyClass.getName(), pyClass.getName());
		constructor.setAsConstructor();
		pyClass.addConstructor(constructor);
	}

	for (auto const& e : cppClass.m_public.m_enums) {
		pyClass.addEnum(buildEnum(e));
	}

	return pyClass;
}
}    // namespace Swift::Builders
