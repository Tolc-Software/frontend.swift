#include "Swift/Builders/moduleBuilder.hpp"
#include "ObjcSwift/Helpers/combine.hpp"
#include "ObjcSwift/Helpers/split.hpp"
#include "ObjcSwift/getOverloadedFunctions.hpp"
#include "Swift/Builders/attributeBuilder.hpp"
#include "Swift/Builders/classBuilder.hpp"
#include "Swift/Builders/enumBuilder.hpp"
#include "Swift/Builders/functionBuilder.hpp"
#include "Swift/Proxy/module.hpp"
#include <IR/ir.hpp>
#include <algorithm>
#include <fmt/format.h>
#include <set>
#include <string>

namespace Swift::Builders {

// Return a unique variable name that can be used in the generated code for this module
std::string getVariableName(std::string qualifiedName,
                            std::string const& rootModuleName) {
	// MyNS::Math, rootModule -> rootModule_MyNs_Math
	// This is to avoid naming conflicts when defining namespaces with the
	// same name as the root module
	// This happens if you call your module tensorflow and have a namespace with tensorflow
	auto splitted = ObjcSwift::Helpers::split(qualifiedName, "::");

	// If we only have the global namespace (empty name), drop it
	// This will result in module variable name of "MyModule" instead of "MyModule_"
	if (splitted.size() == 1 && splitted[0] == "") {
		splitted.pop_back();
	}

	splitted.push_front(rootModuleName);
	// If qualifiedName is the root name (global namespace has no name)
	// This will return rootModuleName
	return fmt::format("{}", fmt::join(splitted, "_"));
}

std::optional<Swift::Proxy::Module>
buildModule(IR::Namespace const& ns, std::string const& rootModuleName) {
	Swift::Proxy::Module builtModule(
	    getVariableName(ns.m_representation, rootModuleName));

	auto overloadedFunctions =
	    ObjcSwift::getOverloadedFunctions(ns.m_functions);
	for (auto const& function : ns.m_functions) {
		if (auto maybeF = Swift::Builders::buildFunction(function)) {
			auto f = maybeF.value();
			if (overloadedFunctions.find(function.m_representation) !=
			    overloadedFunctions.end()) {
				f.setAsOverloaded();
			}
			builtModule.addFunction(f);
		} else {
			return std::nullopt;
		}
	}

	for (auto const& variable : ns.m_variables) {
		auto v = Swift::Builders::buildAttribute(ns.m_representation, variable);
		builtModule.addAttribute(v);
	}

	for (auto const& cls : ns.m_structs) {
		if (auto maybeC = Swift::Builders::buildClass(cls, rootModuleName)) {
			auto c = maybeC.value();
			builtModule.addClass(c);
		} else {
			return std::nullopt;
		}
	}

	for (auto const& e : ns.m_enums) {
		builtModule.addEnum(Swift::Builders::buildEnum(e));
	}

	for (auto const& subNamespace : ns.m_namespaces) {
		builtModule.addSubmodule(
		    subNamespace.m_name,
		    getVariableName(subNamespace.m_representation, rootModuleName),
		    subNamespace.m_documentation);
	}

	return builtModule;
}
}    // namespace Swift::Builders
