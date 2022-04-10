#include "Frontend/ObjcSwift/frontend.hpp"
#include "Objc/Builders/moduleFileBuilder.hpp"
#include "Objc/Proxy/moduleFile.hpp"
#include "Swift/Builders/moduleFileBuilder.hpp"
#include "Swift/Proxy/moduleFile.hpp"
#include <IR/ir.hpp>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

namespace Frontend::ObjcSwift {

std::optional<std::vector<std::pair<std::filesystem::path, std::string>>>
createModule(IR::Namespace const& rootNamespace,
             std::string const& moduleName) {
	std::vector<std::pair<std::filesystem::path, std::string>> out;
	if (auto maybeObjcFile =
	        Objc::Builders::buildModuleFile(rootNamespace, moduleName)) {
		auto& objcFile = maybeObjcFile.value();

		out.push_back(std::make_pair(objcFile.getObjcHeaderFile(),
		                             objcFile.getObjcHeader()));
		out.push_back(std::make_pair(objcFile.getObjcSourceFile(),
		                             objcFile.getObjcSource()));
		out.push_back(std::make_pair(objcFile.getBridgingHeaderFile(),
		                             objcFile.getBridgingHeader()));
		if (auto maybeSwiftFile =
		        Swift::Builders::buildModuleFile(rootNamespace, moduleName)) {
			auto& swiftFile = maybeSwiftFile.value();
			out.push_back(
			    std::make_pair(swiftFile.getSwiftFile(), swiftFile.getSwift()));

			return out;
		}
	}
	return std::nullopt;
}

}    // namespace Frontend::ObjcSwift
