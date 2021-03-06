#include "TestUtil/objcSwiftStage.hpp"
#include "Frontend/Objc/frontend.hpp"
#include "Stage/cmakeStage.hpp"
#include "TestStage/paths.hpp"
#include "TestUtil/parserConfig.hpp"
#include <IR/ir.hpp>
#include <Parser/Parse.hpp>
#include <catch2/catch.hpp>
#include <filesystem>
#include <fmt/format.h>
#include <fstream>
#include <string>
#include <vector>

namespace TestUtil {

namespace {
/**
* Adds the source file moduleName.hpp to the stage with provided content
* Uses Tolc::Parser to parse the file and returns the parsed data
*/
IR::Namespace parseModuleFile(Stage::CMakeStage& stage,
                              std::string const& moduleName,
                              std::string const& content) {
	using path = std::filesystem::path;

	auto testFile = stage.addFile(path("include") / (moduleName + ".hpp"),
	                              "#pragma once\n\n" + content);

	auto parsed = Parser::parseFile(testFile, TestUtil::getParserConfig());
	REQUIRE(parsed.has_value());
	return parsed.value();
}

void addObjcSwiftTestBodies(Stage::CMakeStage& stage,
                            std::string const& moduleName,
                            std::string const& objc,
                            std::string const& swift) {
	using path = std::filesystem::path;
	stage.addFile(path("tests") / "main.mm",
	              fmt::format(R"(
#include <{moduleName}.h>

int main() {{
  @autoreleasepool {{
{code}
  }}
}}
)",
	                          fmt::arg("moduleName", moduleName),
	                          fmt::arg("code", objc)));
	stage.addFile(path("tests") / "main.swift",
	              fmt::format(R"(
import {moduleName}

{code}
)",
	                          fmt::arg("moduleName", moduleName),
	                          fmt::arg("code", swift)));
}

std::string makeValidFileName(std::string s) {
	std::replace(s.begin(), s.end(), ' ', '_');
	std::replace(s.begin(), s.end(), ':', '_');
	return s;
}
}    // namespace

ObjcSwiftStage::ObjcSwiftStage(std::filesystem::path const& baseStage,
                               std::string const& moduleName)
    : m_stage(baseStage,
              {"cmake", "tests", "CMakeLists.txt", "configureAndBuild.bat"}),
      m_moduleName(moduleName) {
	using path = std::filesystem::path;
	using std::filesystem::create_directories;
	create_directories(m_stage.m_stage / path("src"));
	create_directories(m_stage.m_stage / path("include"));
	create_directories(m_stage.m_stage / path("tests"));

	m_stage.setTargetName(m_moduleName);
	m_stage.setWindowsCMakeBuildAndConfigureScript("configureAndBuild.bat");
}

int ObjcSwiftStage::runObjcSwiftTest(std::string const& cppCode,
                                     std::string const& objCTestCode,
                                     std::string const& swiftCode) {
	using path = std::filesystem::path;
	// Save as what has been used
	m_exports = {Code {"cpp", cppCode}, Code {"objc", objCTestCode}};
	// Code {"swift", swiftCode}};

	addObjcSwiftTestBodies(m_stage, m_moduleName, objCTestCode, swiftCode);

	auto globalNS = parseModuleFile(m_stage, m_moduleName, cppCode);
	if (auto m = Frontend::Objc::createModule(globalNS, m_moduleName)) {
		for (auto const& [file, content] : m.value()) {
			auto ext = file.extension().string();
			if (ext == ".mm") {
				addModuleFile(file, content);
			} else {
				// Add an Objective-C++/Swift/whatever file
				// Assumes to be referred to in CMakeLists.txt properly
				m_stage.addFile(path("src") / file, content);
			}
		}
		if (m_stage.configureAndBuild() == 0) {
			return runCtest();
		}
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
