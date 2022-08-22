#include "Swift/Builders/moduleFileBuilder.hpp"
#include "Objc/Proxy/class.hpp"
#include "Objc/Proxy/structure.hpp"
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
buildModuleFile(std::vector<Objc::Proxy::Structure const*> const& structures) {
	for (auto const* structure : structures) {
		using Kind = Objc::Proxy::Structure::Kind;
		switch (structure->m_kind) {
			case Kind::Class: {
				auto cls = static_cast<Objc::Proxy::Class const&>(*structure);
				break;
			}
			case Kind::Namespace: {
				break;
			}
			case Kind::Function: {
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

	return std::nullopt;
}
}    // namespace Swift::Builders
