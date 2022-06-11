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
		bool isConstructor = false;
		bool isOverloaded =
		    overloadedFunctions.find(function.m_representation) !=
		    overloadedFunctions.end();
		if (auto maybeF = Objc::Builders::buildFunction(objcName,
		                                                ns.m_representation,
		                                                function,
		                                                cache,
		                                                isConstructor,
		                                                isOverloaded)) {
			auto f = maybeF.value();
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
	std::unique_ptr<Objc::Cache> cache = std::make_unique<Objc::Cache>();
	cache->m_moduleName = rootModuleName;
	Objc::Proxy::ModuleFile moduleFile;

	std::queue<IR::Namespace const*> namespaces;
	namespaces.push(&rootNamespace);

	while (!namespaces.empty()) {
		auto currentNamespace = namespaces.front();
		if (auto m = Objc::Builders::buildModule(*currentNamespace, *cache)) {
			if (!addNamespaceObjects(moduleFile,
			                         *currentNamespace,
			                         m.value().getName(),
			                         *cache)) {
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

	moduleFile.setCache(std::move(cache));
	return moduleFile;
}
}    // namespace Objc::Builders
