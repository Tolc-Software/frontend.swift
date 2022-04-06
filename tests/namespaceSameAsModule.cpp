#include "TestStage/paths.hpp"
#include "TestUtil/objcSwiftStage.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>

TEST_CASE("Namespace name same as module", "[namespaceSameAsModule]") {
	std::string moduleName = "m";
	auto stage =
	    TestUtil::ObjcSwiftStage(TestStage::getRootStagePath(), moduleName);

	auto cppCode = R"(
namespace MyLib {

int complexFunction() {
	return 5;
}

})";

	auto pythonTestCode = fmt::format(R"(
result = {moduleName}.MyLib.complexFunction()
self.assertEqual(result, 5)
)",
	                                  fmt::arg("moduleName", moduleName));

	auto errorCode = stage.runObjcSwiftTest(cppCode, pythonTestCode);
	REQUIRE(errorCode == 0);
}
