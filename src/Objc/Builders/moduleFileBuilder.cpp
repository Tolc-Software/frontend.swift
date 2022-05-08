#include "Objc/Builders/moduleFileBuilder.hpp"
#include "Objc/Builders/attributeBuilder.hpp"
#include "Objc/Builders/classBuilder.hpp"
#include "Objc/Builders/enumBuilder.hpp"
#include "Objc/Builders/moduleBuilder.hpp"
#include "Objc/Proxy/moduleFile.hpp"
#include "Objc/cache.hpp"
#include "ObjcSwift/Helpers/combine.hpp"
#include "ObjcSwift/getOverloadedFunctions.hpp"
#include <IR/ir.hpp>
#include <optional>
#include <queue>
#include <set>
#include <string>

namespace {

bool addNamespaceObjects(Objc::Proxy::ModuleFile& moduleFile,
                         IR::Namespace const& ns,
                         std::string const& objcName,
                         Objc::Cache& cache) {
	for (auto const& e : ns.m_enums) {
		moduleFile.addEnum(Objc::Builders::buildEnum(e, cache));
	}

	for (auto const& cls : ns.m_structs) {
		if (auto maybeC = Objc::Builders::buildClass(cls, cache)) {
			moduleFile.addClass(maybeC.value());
		} else {
			return false;
		}
	}

	auto overloadedFunctions =
	    ObjcSwift::getOverloadedFunctions(ns.m_functions);
	for (auto const& function : ns.m_functions) {
		if (auto maybeF = Objc::Builders::buildFunction(
		        objcName, ns.m_representation, function, cache)) {
			auto f = maybeF.value();
			if (overloadedFunctions.find(function.m_representation) !=
			    overloadedFunctions.end()) {
				f.setAsOverloaded();
			}
			// Global functions act as static functions
			f.setAsStatic();
			f.setAsStandalone();
			moduleFile.addFunction(f);
		} else {
			return false;
		}
	}

	for (auto const& variable : ns.m_variables) {
		auto attr = Objc::Builders::buildAttribute(
		    objcName, ns.m_representation, variable, cache);
		// All global variables act as static with this interface
		// (through a class)
		attr.setAsStatic();
		attr.setAsStandalone();
		moduleFile.addAttribute(attr);
	}

	return true;
}
}    // namespace

namespace Objc::Builders {

std::optional<Objc::Proxy::ModuleFile>
buildModuleFile(IR::Namespace const& rootNamespace,
                std::string const& rootModuleName) {
	Objc::Cache cache;
	cache.m_moduleName = rootModuleName;
	Objc::Proxy::ModuleFile moduleFile;

	std::queue<IR::Namespace const*> namespaces;
	namespaces.push(&rootNamespace);

	while (!namespaces.empty()) {
		auto currentNamespace = namespaces.front();
		if (auto m = Objc::Builders::buildModule(*currentNamespace, cache)) {
			if (!addNamespaceObjects(moduleFile,
			                         *currentNamespace,
			                         m.value().getName(),
			                         cache)) {
				return std::nullopt;
			}
			moduleFile.addModule(m.value());
		}

		// Go deeper into the nested namespaces
		for (auto const& subNamespace : currentNamespace->m_namespaces) {
			namespaces.push(&subNamespace);
		}

		// Need currentNamespace and currentModule to live this far
		namespaces.pop();
	}

	moduleFile.setCache(cache);
	return moduleFile;
}
}    // namespace Objc::Builders
