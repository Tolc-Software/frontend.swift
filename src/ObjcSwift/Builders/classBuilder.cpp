#include "ObjcSwift/Builders/classBuilder.hpp"
#include "ObjcSwift/Builders/enumBuilder.hpp"
#include "ObjcSwift/Builders/functionBuilder.hpp"
#include "ObjcSwift/Builders/typeToStringBuilder.hpp"
#include "ObjcSwift/Helpers/combine.hpp"
#include "ObjcSwift/Helpers/operatorNames.hpp"
#include "ObjcSwift/Helpers/trampolineClass.hpp"
#include "ObjcSwift/Helpers/types.hpp"
#include "ObjcSwift/Proxy/function.hpp"
#include "ObjcSwift/Proxy/typeInfo.hpp"
#include "ObjcSwift/checkType.hpp"
#include "ObjcSwift/getOverloadedFunctions.hpp"
#include <IR/ir.hpp>
#include <numeric>
#include <optional>
#include <set>
#include <string>

namespace ObjcSwift::Builders {

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

void buildMemberFunction(ObjcSwift::Proxy::Function& pyFunction,
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
	std::vector<ObjcSwift::Proxy::Function> virtualFunctions;
	std::vector<ObjcSwift::Proxy::Function> pureVirtualFunctions;
};

void addIfVirtual(IR::Polymorphic polymorphic,
                  ObjcSwift::Proxy::Function const& pyFunction,
                  TrampolineFunctions& trampoline) {
	using IR::Polymorphic;
	switch (polymorphic) {
		case Polymorphic::PureVirtual:
			trampoline.pureVirtualFunctions.push_back(pyFunction);
			break;
		case Polymorphic::Virtual:
			trampoline.virtualFunctions.push_back(pyFunction);
			break;
		case Polymorphic::NA: break;
	}
}

}    // namespace

std::optional<ObjcSwift::Proxy::Class>
buildClass(IR::Struct const& cppClass, ObjcSwift::Proxy::TypeInfo& typeInfo) {
	ObjcSwift::Proxy::Class pyClass(
	    ObjcSwift::Helpers::removeCppTemplate(cppClass.m_name) +
	        getTemplateParameterString(cppClass.m_templateArguments),
	    cppClass.m_representation);

	pyClass.setDocumentation(cppClass.m_documentation);

	pyClass.setInherited(cppClass.m_public.m_inherited);
	TrampolineFunctions trampoline;

	// Ignore private functions
	auto overloadedFunctions =
	    ObjcSwift::getOverloadedFunctions(cppClass.m_public.m_functions);
	for (auto const& function : cppClass.m_public.m_functions) {
		if (auto maybePyFunction = buildFunction(function, typeInfo)) {
			auto& pyFunction = maybePyFunction.value();

			buildMemberFunction(pyFunction, function, overloadedFunctions);

			addIfVirtual(function.m_polymorphic, pyFunction, trampoline);

			pyClass.addFunction(pyFunction);
		} else {
			return std::nullopt;
		}
	}

	auto overloadedOperators =
	    ObjcSwift::getOverloadedFunctions(cppClass.m_public.m_operators);
	for (auto const& [op, function] : cppClass.m_public.m_operators) {
		if (auto maybePyFunction = buildFunction(function, typeInfo)) {
			if (auto maybeName = ObjcSwift::Helpers::getOperatorName(op)) {
				auto& pyFunction = maybePyFunction.value();

				// The python operators have special names
				pyFunction.setPythonName(maybeName.value());

				buildMemberFunction(pyFunction, function, overloadedOperators);

				addIfVirtual(function.m_polymorphic, pyFunction, trampoline);

				pyClass.addFunction(pyFunction);
			}
		} else {
			return std::nullopt;
		}
	}

	for (auto const& constructor : cppClass.m_public.m_constructors) {
		if (auto maybePyFunction = buildFunction(constructor, typeInfo)) {
			auto& pyFunction = maybePyFunction.value();

			if (constructor.m_isStatic) {
				pyFunction.setAsStatic();
			}

			if (cppClass.m_public.m_constructors.size() > 1) {
				pyFunction.setAsOverloaded();
			}

			pyFunction.setAsConstructor();

			addIfVirtual(constructor.m_polymorphic, pyFunction, trampoline);

			pyClass.addConstructor(pyFunction);
		}
	}

	for (auto const& variable : cppClass.m_public.m_memberVariables) {
		ObjcSwift::checkType(variable.m_type, typeInfo);
		pyClass.addMemberVariable(variable.m_name,
		                          variable.m_documentation,
		                          variable.m_type.m_isConst,
		                          variable.m_type.m_isStatic);
	}

	// Add default constructor
	if (cppClass.m_hasImplicitDefaultConstructor) {
		auto constructor =
		    ObjcSwift::Proxy::Function(pyClass.getName(), pyClass.getName());
		constructor.setAsConstructor();
		pyClass.addConstructor(constructor);
	}

	for (auto const& e : cppClass.m_public.m_enums) {
		pyClass.addEnum(buildEnum(e));
	}

	if (!trampoline.virtualFunctions.empty() ||
	    !trampoline.pureVirtualFunctions.empty()) {
		// There are virtual functions
		auto [name, cls] = ObjcSwift::Helpers::getTrampolineClass(
		    cppClass.m_name,
		    cppClass.m_representation,
		    trampoline.virtualFunctions,
		    trampoline.pureVirtualFunctions);

		typeInfo.m_trampolineClasses.insert(cls);
		pyClass.addTrampolineClass(typeInfo.m_extraFunctionsNamespace +
		                           "::" + name);
	}

	if (typeInfo.m_classesMarkedShared.contains(cppClass.m_representation)) {
		pyClass.setAsManagedByShared();
	}

	return pyClass;
}
}    // namespace ObjcSwift::Builders
