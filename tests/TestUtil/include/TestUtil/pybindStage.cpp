#include "TestUtil/pybindStage.hpp"
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

ObjcSwiftStage::ObjcSwiftStage(std::filesystem::path const& baseStage,
                               std::string const& moduleName)
    : m_stage(baseStage, {"cmake", "CMakeLists.txt", "configureAndBuild.bat"}),
      m_moduleName(moduleName) {
	m_stage.setTargetName(m_moduleName);
	m_stage.setWindowsCMakeBuildAndConfigureScript("configureAndBuild.bat");
}

int runObjcSwiftTest(std::string const& cppCode,
                     std::string const& objCTestCode,
                     std::string const& swiftCode) {
	// Save as what has been used
	m_exports = {Code {"cpp", cppCode},
	             Code {"objc", objCTestCode},
	             Code {"swift", swiftCode}};

	auto globalNS = parseModuleFile(cppCode);

	if (auto m = Frontend::ObjcSwift::createModule(globalNS, m_moduleName)) {
		for (auto const& [file, content] : m.value()) {
			addModuleFile(file, content);
		}
		return runPythonUnittest(pythonUnittestCode);
	}

	return 1;
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

int ObjcSwiftStage::runPythonUnittest(std::string const& testBody) {
	// Make sure the module is built
	if (auto setupError = m_stage.configureAndBuild(); setupError != 0) {
		return setupError;
	}

	std::vector<std::string> includes = {m_moduleName};
	std::string testName = "default";

	// Store all the lines in the test
	// Necessary since python cares about indentation
	std::vector<std::string> body;
	std::string::size_type pos = 0;
	std::string::size_type prev = 0;
	while ((pos = testBody.find('\n', prev)) != std::string::npos) {
		body.push_back(testBody.substr(prev, pos - prev));
		prev = pos + 1;
	}

	// To get the last substring (or only, if delimiter is not found)
	body.push_back(testBody.substr(prev));

	TestStage::addPythonUnittest(
	    m_stage.m_stage, m_moduleName, testName, body, includes);

	return TestStage::runPythonUnittest(m_stage.m_stage, m_moduleName);
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
