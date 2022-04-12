#include "Objc/Builders/classBuilder.hpp"
#include "Objc/Builders/enumBuilder.hpp"
#include "Objc/Builders/functionBuilder.hpp"
#include "Objc/Builders/typeToStringBuilder.hpp"
#include "Objc/Proxy/function.hpp"
#include "Objc/getName.hpp"
#include "ObjcSwift/Helpers/combine.hpp"
#include "ObjcSwift/Helpers/operatorNames.hpp"
#include "ObjcSwift/Helpers/types.hpp"
#include "ObjcSwift/getOverloadedFunctions.hpp"
#include <IR/ir.hpp>
#include <numeric>
#include <optional>
#include <set>
#include <string>

namespace Objc::Builders {

namespace {

void buildMemberFunction(std::string const& fullyQualifiedClassName,
                         Objc::Proxy::Function& objcFunction,
                         IR::Function const& cppFunction,
                         std::set<std::string> const& overloadedFunctions) {
	objcFunction.setAsClassFunction(fullyQualifiedClassName);

	if (cppFunction.m_isStatic) {
		objcFunction.setAsStatic();
	}

	if (overloadedFunctions.find(cppFunction.m_representation) !=
	    overloadedFunctions.end()) {
		objcFunction.setAsOverloaded();
	}
}

}    // namespace

std::optional<Objc::Proxy::Class> buildClass(IR::Struct const& cppClass,
                                             std::string const& moduleName) {
	Objc::Proxy::Class objcClass(Objc::getClassName(cppClass, moduleName),
	                             cppClass.m_representation);

	objcClass.setDocumentation(cppClass.m_documentation);

	objcClass.setInherited(cppClass.m_public.m_inherited);

	// Ignore private functions
	auto overloadedFunctions =
	    ObjcSwift::getOverloadedFunctions(cppClass.m_public.m_functions);
	for (auto const& function : cppClass.m_public.m_functions) {
		if (auto maybeobjcFunction = buildFunction(function)) {
			auto& objcFunction = maybeobjcFunction.value();

			buildMemberFunction(cppClass.m_representation,
			                    objcFunction,
			                    function,
			                    overloadedFunctions);

			objcClass.addFunction(objcFunction);
		} else {
			return std::nullopt;
		}
	}

	auto overloadedOperators =
	    ObjcSwift::getOverloadedFunctions(cppClass.m_public.m_operators);
	for (auto const& [op, function] : cppClass.m_public.m_operators) {
		if (auto maybeobjcFunction = buildFunction(function)) {
			if (auto maybeName = ObjcSwift::Helpers::getOperatorName(op)) {
				auto& objcFunction = maybeobjcFunction.value();

				buildMemberFunction(cppClass.m_representation,
				                    objcFunction,
				                    function,
				                    overloadedOperators);

				objcClass.addFunction(objcFunction);
			}
		} else {
			return std::nullopt;
		}
	}

	for (auto const& constructor : cppClass.m_public.m_constructors) {
		if (auto maybeobjcFunction = buildFunction(constructor, true)) {
			auto& objcFunction = maybeobjcFunction.value();

			if (constructor.m_isStatic) {
				objcFunction.setAsStatic();
			}

			if (cppClass.m_public.m_constructors.size() > 1) {
				objcFunction.setAsOverloaded();
			}

			objcFunction.setAsClassFunction(cppClass.m_representation);

			objcFunction.setAsConstructor();

			objcClass.addConstructor(objcFunction);
		}
	}

	for (auto const& variable : cppClass.m_public.m_memberVariables) {
		objcClass.addMemberVariable(variable.m_name,
		                            variable.m_documentation,
		                            variable.m_type.m_isConst,
		                            variable.m_type.m_isStatic);
	}

	// Add default constructor
	if (cppClass.m_hasImplicitDefaultConstructor) {
		auto constructor = Objc::Proxy::Function("init", objcClass.getName());
		constructor.setAsClassFunction(cppClass.m_representation);
		constructor.setReturnType("instancetype");
		constructor.setAsConstructor();
		objcClass.addConstructor(constructor);
	}

	for (auto const& e : cppClass.m_public.m_enums) {
		objcClass.addEnum(buildEnum(e));
	}

	return objcClass;
}
}    // namespace Objc::Builders
