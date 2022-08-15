#pragma once

#include "Stage/cmakeStage.hpp"
#include <IR/ir.hpp>
#include <array>
#include <filesystem>

namespace TestUtil {
struct ObjcSwiftStage {
	ObjcSwiftStage(std::filesystem::path const& baseStage,
	               std::string const& moduleName);

	/**
	* Compile and run a test
	*/
	int runObjcTest(std::string const& cppCode,
	                std::string const& objCTestCode);

	/**
	* Compile and run a test
	*/
	int runSwiftTest(std::string const& cppCode, std::string const& swiftCode);

	/**
	* Adds a source file to the stage and imports the module file (moduleName.hpp)
	*/
	void addModuleFile(std::filesystem::path const& file,
	                   std::string const& content);

	/**
	 * Exports as example. Assumes that runObjcTest has been called before
	 * (to save the code)
	 */
	void exportAsExample(std::string const& name);

	int runCtest();

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
	Code m_cpp;
	Code m_objc;
	Code m_swift;
};
}    // namespace TestUtil
