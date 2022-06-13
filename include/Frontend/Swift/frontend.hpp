#pragma once

#include <IR/ir.hpp>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

namespace Frontend::Swift {
/**
  * Takes a representation of some C++ code.
  * Returns a file and the corresponding file content.
  * The file content is the converted IR into Swift bindings.
  */
std::optional<std::vector<std::pair<std::filesystem::path, std::string>>>
createModule(IR::Namespace const& rootNamespace, std::string const& moduleName);
}    // namespace Frontend::Swift
