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
	        ::ObjcSwift::Builders::buildModuleFile(rootNamespace, moduleName)) {
		auto& moduleFile = maybeModuleFile.value();

		return std::vector {
		    std::make_pair(moduleFile.getObjcHeaderFile(),
		                   moduleFile.getObjcHeader()),
		    std::make_pair(moduleFile.getObjcSourceFile(),
		                   moduleFile.getObjcSource()),
		    std::make_pair(moduleFile.getBridgingHeaderFile(),
		                   moduleFile.getBridgingHeader()),
		    std::make_pair(moduleFile.getSwiftFile(), moduleFile.getSwift())};
	}
	return std::nullopt;
}

}    // namespace Frontend::ObjcSwift
