#pragma once

#include "Frontend/ObjcSwift/Config.hpp"
#include <IR/ir.hpp>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

namespace Frontend::ObjcSwift {
/**
  * Takes a representation of some C++ code.
  * Returns a file and the corresponding file content.
  * The file content is the converted IR into pybind11 bindings.
  */
std::optional<std::vector<std::pair<std::filesystem::path, std::string>>>
createModule(
    IR::Namespace const& rootNamespace,
    std::string const& moduleName,
    Frontend::ObjcSwift::Config config = Frontend::ObjcSwift::Config());
}    // namespace Frontend::ObjcSwift
