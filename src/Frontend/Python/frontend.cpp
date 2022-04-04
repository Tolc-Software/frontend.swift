#include "Frontend/ObjcSwift/frontend.hpp"
#include "ObjcSwift/Builders/moduleFileBuilder.hpp"
#include "ObjcSwift/Proxy/moduleFile.hpp"
#include <IR/ir.hpp>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

namespace Frontend::ObjcSwift {

std::optional<std::vector<std::pair<std::filesystem::path, std::string>>>
createModule(IR::Namespace const& rootNamespace,
             std::string const& moduleName) {
	if (auto maybeModuleFile =
	        ObjcSwift::Builders::buildModuleFile(rootNamespace, moduleName)) {
		auto& moduleFile = maybeModuleFile.value();

		return std::vector {std::make_pair(moduleFile.getFilepath(),
		                                   moduleFile.getObjcSwift())};
	}
	return std::nullopt;
}

}    // namespace Frontend::ObjcSwift
