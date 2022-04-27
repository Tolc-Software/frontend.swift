#include "Objc/Builders/moduleFileBuilder.hpp"
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
}    // namespace

namespace Objc::Builders {

std::optional<Objc::Proxy::ModuleFile>
buildModuleFile(IR::Namespace const& rootNamespace,
                std::string const& rootModuleName) {
	Objc::Cache cache;
	cache.m_moduleName = rootModuleName;
	if (auto maybeRootModule =
	        Objc::Builders::buildModule(rootNamespace, cache)) {
		auto rootModule = maybeRootModule.value();
		Objc::Proxy::ModuleFile moduleFile(rootModule, rootModuleName);

		std::queue<ModulePair> namespaces;
		for (auto const& subNamespace : rootNamespace.m_namespaces) {
			if (auto m = Objc::Builders::buildModule(subNamespace, cache)) {
				namespaces.push({subNamespace, m.value()});
			} else {
				return std::nullopt;
			}
		}

		while (!namespaces.empty()) {
			auto const& [currentNamespace, currentModule] = namespaces.front();

			moduleFile.addModule(currentModule);

			// Go deeper into the nested namespaces
			for (auto const& subNamespace : currentNamespace.m_namespaces) {
				if (auto m = Objc::Builders::buildModule(subNamespace, cache)) {
					namespaces.push({subNamespace, m.value()});
				} else {
					return std::nullopt;
				}
			}

			// Need currentNamespace and currentModule to live this far
			namespaces.pop();
		}

		moduleFile.setCache(cache);
		return moduleFile;
	}

	return std::nullopt;
}
}    // namespace Objc::Builders
