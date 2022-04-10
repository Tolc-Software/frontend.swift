#include "Swift/Proxy/moduleFile.hpp"
#include <fmt/format.h>
#include <string>

namespace Swift::Proxy {

ModuleFile::ModuleFile(Module const& rootModule, std::string const& libraryName)
    : m_rootModuleName(rootModule.getVariableName()),
      m_libraryName(libraryName), m_modules({rootModule}) {}

void ModuleFile::addModule(Module const& m) {
	m_modules.push_back(m);
}

std::filesystem::path ModuleFile::getFilepath() const {
	return m_libraryName + "_swift.cpp";
}

std::string ModuleFile::getSwift() const {
	std::string out = fmt::format(R"(
public class {libraryName} {{
)",
	                              fmt::arg("libraryName", m_libraryName));

	for (auto const& m : m_modules) {
		out += m.getSwift();
	}

	return out + '}';
}

std::filesystem::path ModuleFile::getSwiftFile() const {
	return m_libraryName + "_swift.swift";
}

}    // namespace Swift::Proxy
