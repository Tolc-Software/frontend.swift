#include "TestUtil/objcSwiftStage.hpp"
#include "Frontend/ObjcSwift/frontend.hpp"
#include "Stage/cmakeStage.hpp"
#include "TestStage/paths.hpp"
#include "TestStage/stageFunctions.hpp"
#include "TestUtil/parse.hpp"
#include <IR/ir.hpp>
#include <Parser/Parse.hpp>
#include <filesystem>
#include <fmt/format.h>
#include <string>
#include <vector>

namespace TestUtil {

namespace {
void addObjcSwiftTestBodies(Stage::CMakeStage& stage,
                            std::string const& moduleName,
                            std::string const& objc,
                            std::string const& swift) {
	using path = std::filesystem::path;
	stage.addFile(path("tests") / "main.mm",
	              fmt::format(R"(
#include <{moduleName}_objc.h>

int main() {{
{code}
}}
)",
	                          fmt::arg("moduleName", moduleName),
	                          fmt::arg("code", objc)));
	stage.addFile(path("tests") / "main.swift",
	              fmt::format(R"(
import {moduleName}_swift

{code}
)",
	                          fmt::arg("moduleName", moduleName),
	                          fmt::arg("code", swift)));
}
}    // namespace

ObjcSwiftStage::ObjcSwiftStage(std::filesystem::path const& baseStage,
                               std::string const& moduleName)
    : m_stage(baseStage,
              {"cmake", "tests", "CMakeLists.txt", "configureAndBuild.bat"}),
      m_moduleName(moduleName) {
	m_stage.setTargetName(m_moduleName);
	m_stage.setWindowsCMakeBuildAndConfigureScript("configureAndBuild.bat");
}

int ObjcSwiftStage::runObjcSwiftTest(std::string const& cppCode,
                                     std::string const& objCTestCode,
                                     std::string const& swiftCode) {
	// Save as what has been used
	m_exports = {Code {"cpp", cppCode},
	             Code {"objc", objCTestCode},
	             Code {"swift", swiftCode}};

	addObjcSwiftTestBodies(m_stage, m_moduleName, objCTestCode, swiftCode);

	auto globalNS = parseModuleFile(cppCode);
	if (auto m = Frontend::ObjcSwift::createModule(globalNS, m_moduleName)) {
		for (auto const& [file, content] : m.value()) {

			auto ext = file.extension().string();
			if (ext == ".mm") {
				addModuleFile(file, content);
			} else {
				// Add an Ojective-C++/Swift/whatever file
				// Assumes to be referred to in CMakeLists.txt properly
				m_stage.addFile(std::filesystem::path("src") / file, content);
			}
		}
		return m_stage.configureAndBuild() && runCtest();
	}

	return 1;
}

int ObjcSwiftStage::runCtest() {
	return m_stage.runCommand("ctest --output-on-failure -j1", "build");
}

void ObjcSwiftStage::addModuleFile(std::filesystem::path const& file,
                                   std::string const& content) {
	m_stage.addSourceFile(file,
	                      "#include \"" + m_moduleName + ".hpp\"\n" + content);
}

IR::Namespace ObjcSwiftStage::parseModuleFile(std::string const& content) {
	auto testFile = m_stage.addSourceFile(m_moduleName + ".hpp", content);
	return TestUtil::parseFile(testFile.string());
}

namespace {
std::string makeValidFileName(std::string s) {
	std::replace(s.begin(), s.end(), ' ', '_');
	std::replace(s.begin(), s.end(), ':', '_');
	return s;
}
}    // namespace

void ObjcSwiftStage::exportAsExample(std::string const& name) {
	std::filesystem::path fileName =
	    TestStage::getExamplesPath() /
	    fmt::format("{}.md", makeValidFileName(name));
	std::string content = fmt::format(R"(
## {} ##

)",
	                                  name);
	for (auto const& [language, code] : m_exports) {
		content += fmt::format(R"(
```{language}
{code}
```

)",
		                       fmt::arg("language", language),
		                       fmt::arg("code", code));
	}

	std::ofstream example(fileName);
	example << content;
	example.close();
}

void ObjcSwiftStage::keepAliveAfterTest() {
	m_stage.m_removeOnDestruction = false;
}
}    // namespace TestUtil
