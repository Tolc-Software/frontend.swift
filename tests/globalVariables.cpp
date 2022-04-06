#include "TestStage/paths.hpp"
#include "TestUtil/objcSwiftStage.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>

TEST_CASE("Global variables are converted",
          "[globalVariables]") {
	std::string moduleName = "m";
	auto stage =
	    TestUtil::ObjcSwiftStage(TestStage::getRootStagePath(), moduleName);

	auto cppCode = R"(
#include <string>

static int i = 0;
namespace Nested {
	int i = 0;
	std::string s = "Hello world";
}
)";

	auto pythonTestCode = fmt::format(R"(
# Starts at 0 and can be changed
self.assertEqual({moduleName}.i, 0)
{moduleName}.i = 5
self.assertEqual({moduleName}.i, 5)

# Nested with the same name
self.assertEqual({moduleName}.Nested.i, 0)

# More complex variables are available aswell
self.assertEqual({moduleName}.Nested.s, "Hello world")
)",
	                                  fmt::arg("moduleName", moduleName));

	auto errorCode = stage.runObjcSwiftTest(cppCode, pythonTestCode);
	REQUIRE(errorCode == 0);

	stage.exportAsExample("Global Variables");
}
