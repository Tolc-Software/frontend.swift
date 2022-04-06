#pragma once

#include "Stage/cmakeStage.hpp"
#include <IR/ir.hpp>
#include <filesystem>

namespace TestUtil {
struct ObjcSwiftStage {
	ObjcSwiftStage(std::filesystem::path const& baseStage,
	               std::string const& moduleName);

	/**
	* Compile and run a test
	*/
	int runObjcSwiftTest(std::string const& cppCode,
	                     std::string const& objCTestCode,
	                     std::string const& swiftCode);

	/**
	* Adds the source file moduleName.hpp to the stage with provided content
	* Uses Tolc::Parser to parse the file and returns the global namespace
	*/
	IR::Namespace parseModuleFile(std::string const& content);

	/**
	* Adds a source file to the stage and imports the module file (moduleName.hpp)
	*/
	void addModuleFile(std::filesystem::path const& file,
	                   std::string const& content);

	/**
	 * Exports as example. Assumes that runObjcSwiftTest has been called before
	 * (to save the code)
	 */
	void exportAsExample(std::string const& name);

	/**
	* Injects input code testBody into a swift file and runs the code.
	*/
	int runSwiftUnittest(std::string const& testBody);

	/**
	* Injects input code testBody into a Objective-C file and runs the code.
	*/
	int runObjectiveCUnittest(std::string const& testBody);

	/**
	* Run this to keep the stage on the filesystem after the test has completed
	*/
	void keepAliveAfterTest();

	struct Code {
		std::string language;
		std::string code;
	};

	Stage::CMakeStage m_stage;
	std::string m_moduleName;
	std::vector<Code> m_exports;
};
}    // namespace TestUtil
