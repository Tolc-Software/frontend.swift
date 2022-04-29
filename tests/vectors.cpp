#include "TestStage/paths.hpp"
#include "TestUtil/objcSwiftStage.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>

TEST_CASE("Using std::vectors", "[vectors]") {
	std::string moduleName = "m";
	auto stage =
	    TestUtil::ObjcSwiftStage(TestStage::getRootStagePath(), moduleName);
	stage.keepAliveAfterTest();

	auto cppCode = R"(
#include <vector>

std::vector<int> f() {
	return {1, 2, 3};
}

)";

	auto objcTestCode = R"(
// Global functions gets added to
// a purely static class with
// the name of the library
// assert([m meaningOfLife] == 42);

// Strings can be used
// assert([[m sayHello:@"Tolc"] isEqualToString:@"Hello Tolc"]);

// Aswell as filesystem paths
// assert([[m getPath] isEqualToString:@"/path/to/stuff.hpp"]);

// Functions within namespaces
// are available with the
// namespaces names merged
// assert([mInner pi] == 3.14);
)";

	auto swiftTestCode = R"()";

	auto errorCode =
	    stage.runObjcSwiftTest(cppCode, objcTestCode, swiftTestCode);
	REQUIRE(errorCode == 0);

	stage.exportAsExample("std::vector");
}
