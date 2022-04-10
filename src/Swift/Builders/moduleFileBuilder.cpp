#include "Swift/Builders/moduleFileBuilder.hpp"
#include "ObjcSwift/Helpers/combine.hpp"
#include "Swift/Builders/moduleBuilder.hpp"
#include "Swift/Proxy/moduleFile.hpp"
#include <IR/ir.hpp>
#include <optional>
#include <queue>
#include <set>
#include <string>

namespace {
struct ModulePair {
	IR::Namespace const& m_namespace;
	Swift::Proxy::Module m_module;
};
}    // namespace

namespace Swift::Builders {

std::optional<Swift::Proxy::ModuleFile>
buildModuleFile(IR::Namespace const& rootNamespace,
                std::string const& rootModuleName) {
	if (auto maybeRootModule =
	        Swift::Builders::buildModule(rootNamespace, rootModuleName)) {
		auto rootModule = maybeRootModule.value();
		Swift::Proxy::ModuleFile moduleFile(rootModule, rootModuleName);

		std::queue<ModulePair> namespaces;
		for (auto const& subNamespace : rootNamespace.m_namespaces) {
			if (auto m = Swift::Builders::buildModule(subNamespace,
			                                          rootModuleName)) {
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
				if (auto m = Swift::Builders::buildModule(subNamespace,
				                                          rootModuleName)) {
					namespaces.push({subNamespace, m.value()});
				} else {
					return std::nullopt;
				}
			}

			// Need currentNamespace and currentModule to live this far
			namespaces.pop();
		}

		return moduleFile;
	}

	return std::nullopt;
}
}    // namespace Swift::Builders
