#include "Swift/Builders/classBuilder.hpp"
#include "Objc/getName.hpp"
#include "ObjcSwift/Helpers/combine.hpp"
#include "ObjcSwift/Helpers/operatorNames.hpp"
#include "ObjcSwift/Helpers/types.hpp"
#include "ObjcSwift/getOverloadedFunctions.hpp"
#include "Swift/Builders/enumBuilder.hpp"
#include "Swift/Builders/functionBuilder.hpp"
#include "Swift/Proxy/function.hpp"
#include "Swift/getName.hpp"
#include <IR/ir.hpp>
#include <numeric>
#include <optional>
#include <set>
#include <string>

namespace Swift::Builders {

namespace {

void buildMemberFunction(std::string const& objcClassName,
                         Swift::Proxy::Function& swiftFunction,
                         IR::Function const& cppFunction,
                         std::set<std::string> const& overloadedFunctions) {
	swiftFunction.setAsClassFunction(objcClassName);

	if (cppFunction.m_isStatic) {
		swiftFunction.setAsStatic();
	}

	if (overloadedFunctions.find(cppFunction.m_representation) !=
	    overloadedFunctions.end()) {
		swiftFunction.setAsOverloaded();
	}
}

}    // namespace

std::optional<Swift::Proxy::Class> buildClass(IR::Struct const& cppClass,
                                              std::string const& moduleName) {
	auto objcClassName = Objc::getClassName(cppClass, moduleName);

	Swift::Proxy::Class swiftClass(
	    Swift::getClassName(cppClass.m_name, cppClass.m_templateArguments),
	    objcClassName);

	swiftClass.setDocumentation(cppClass.m_documentation);

	swiftClass.setInherited(cppClass.m_public.m_inherited);

	// Ignore private functions
	auto overloadedFunctions =
	    ObjcSwift::getOverloadedFunctions(cppClass.m_public.m_functions);
	for (auto const& function : cppClass.m_public.m_functions) {
		if (auto maybeswiftFunction = buildFunction(function)) {
			auto& swiftFunction = maybeswiftFunction.value();

			buildMemberFunction(
			    objcClassName, swiftFunction, function, overloadedFunctions);

			swiftClass.addFunction(swiftFunction);
		} else {
			return std::nullopt;
		}
	}

	auto overloadedOperators =
	    ObjcSwift::getOverloadedFunctions(cppClass.m_public.m_operators);
	for (auto const& [op, function] : cppClass.m_public.m_operators) {
		if (auto maybeswiftFunction = buildFunction(function)) {
			if (auto maybeName = ObjcSwift::Helpers::getOperatorName(op)) {
				auto& swiftFunction = maybeswiftFunction.value();

				buildMemberFunction(objcClassName,
				                    swiftFunction,
				                    function,
				                    overloadedOperators);

				swiftClass.addFunction(swiftFunction);
			}
		} else {
			return std::nullopt;
		}
	}

	for (auto const& constructor : cppClass.m_public.m_constructors) {
		if (auto maybeswiftFunction = buildFunction(constructor, true)) {
			auto& swiftFunction = maybeswiftFunction.value();

			if (constructor.m_isStatic) {
				swiftFunction.setAsStatic();
			}

			if (cppClass.m_public.m_constructors.size() > 1) {
				swiftFunction.setAsOverloaded();
			}

			swiftFunction.setAsClassFunction(objcClassName);

			swiftFunction.setAsConstructor();

			swiftClass.addConstructor(swiftFunction);
		}
	}

	for (auto const& variable : cppClass.m_public.m_memberVariables) {
		swiftClass.addMemberVariable({variable.m_name,
		                              variable.m_documentation,
		                              variable.m_type.m_isConst,
		                              variable.m_type.m_isStatic});
	}

	// Add default constructor
	if (cppClass.m_hasImplicitDefaultConstructor) {
		auto constructor = Swift::Proxy::Function("init", swiftClass.getName());
		constructor.setAsClassFunction(objcClassName);
		constructor.setAsConstructor();
		swiftClass.addConstructor(constructor);
	}

	for (auto const& e : cppClass.m_public.m_enums) {
		swiftClass.addEnum(buildEnum(e));
	}

	return swiftClass;
}
}    // namespace Swift::Builders
