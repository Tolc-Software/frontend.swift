#include "Swift/Builders/moduleFileBuilder.hpp"
#include "Objc/Proxy/class.hpp"
#include "Objc/Proxy/structure.hpp"
#include "Swift/Builders/functionBuilder.hpp"
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
buildModuleFile(std::vector<Objc::Proxy::Structure const*> const& structures,
                std::string const& moduleName) {
	Swift::Proxy::ModuleFile rootFile(moduleName);
	for (auto const* structure : structures) {
		using Kind = Objc::Proxy::Structure::Kind;
		switch (structure->m_kind) {
			case Kind::Class: {
				break;
			}
			case Kind::Namespace: {
				break;
			}
			case Kind::Function: {
				rootFile.addFunction(buildFunction(
				    static_cast<Objc::Proxy::Function const&>(*structure),
				    moduleName));
				break;
			}
			case Kind::Enum: {
				break;
			}
			case Kind::Attribute: {
				break;
			}
			break;
		}
	}

	return rootFile;
}
}    // namespace Swift::Builders
