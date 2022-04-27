#include "Objc/Builders/moduleFileBuilder.hpp"
#include "Objc/Builders/classBuilder.hpp"
#include "Objc/Builders/enumBuilder.hpp"
#include "Objc/Builders/moduleBuilder.hpp"
#include "Objc/Proxy/moduleFile.hpp"
#include "Objc/cache.hpp"
#include "ObjcSwift/Helpers/combine.hpp"
#include <IR/ir.hpp>
#include <optional>
#include <queue>
#include <set>
#include <string>

namespace {
struct ModulePair {
	IR::Namespace const& m_namespace;
	Objc::Proxy::Module m_module;
};

bool addNamespaceObjects(Objc::Proxy::ModuleFile& moduleFile,
                         IR::Namespace const& ns,
                         Objc::Cache& cache) {
	for (auto const& e : ns.m_enums) {
		moduleFile.addEnum(Objc::Builders::buildEnum(e, cache));
	}

	for (auto const& cls : ns.m_structs) {
		if (auto maybeC =
		        Objc::Builders::buildClass(cls, cache.m_moduleName, cache)) {
			moduleFile.addClass(maybeC.value());
		} else {
			return false;
		}
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
			if (!addNamespaceObjects(moduleFile, *currentNamespace, cache)) {
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
