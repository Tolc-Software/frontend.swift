#include "ObjcSwift/Builders/moduleFileBuilder.hpp"
#include "ObjcSwift/Builders/moduleBuilder.hpp"
#include "ObjcSwift/Helpers/combine.hpp"
#include "ObjcSwift/Proxy/moduleFile.hpp"
#include "ObjcSwift/Proxy/typeInfo.hpp"
#include <IR/ir.hpp>
#include <optional>
#include <queue>
#include <set>
#include <string>

namespace {
struct ModulePair {
	IR::Namespace const& m_namespace;
	ObjcSwift::Proxy::Module m_module;
};
}    // namespace

namespace ObjcSwift::Builders {

std::optional<ObjcSwift::Proxy::ModuleFile>
buildModuleFile(IR::Namespace const& rootNamespace,
                std::string const& rootModuleName) {
	ObjcSwift::Proxy::TypeInfo typeInfo;
	if (auto maybeRootModule = ObjcSwift::Builders::buildModule(
	        rootNamespace, rootModuleName, typeInfo)) {
		auto rootModule = maybeRootModule.value();
		ObjcSwift::Proxy::ModuleFile moduleFile(rootModule, rootModuleName);

		std::queue<ModulePair> namespaces;
		for (auto const& subNamespace : rootNamespace.m_namespaces) {
			if (auto m = ObjcSwift::Builders::buildModule(
			        subNamespace, rootModuleName, typeInfo)) {
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
				if (auto m = ObjcSwift::Builders::buildModule(
				        subNamespace, rootModuleName, typeInfo)) {
					namespaces.push({subNamespace, m.value()});
				} else {
					return std::nullopt;
				}
			}

			// Need currentNamespace and currentModule to live this far
			namespaces.pop();
		}

		moduleFile.setTypeInfo(typeInfo);
		return moduleFile;
	}

	return std::nullopt;
}
}    // namespace ObjcSwift::Builders
