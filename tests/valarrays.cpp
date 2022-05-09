#include "TestStage/paths.hpp"
#include "TestUtil/objcSwiftStage.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>

TEST_CASE("Simple valarrays", "[valarrays]") {
	std::string moduleName = "m";
	auto stage =
	    TestUtil::ObjcSwiftStage(TestStage::getRootStagePath(), moduleName);

	auto cppCode = R"(
#include <valarray>

std::valarray<int> getIt() {
	return {0, 1, 2};
}
)";

	auto objcTestCode = R"(
// std::valarray corresponds to NSArray
NSArray* v = [m getIt];
assert([v count] == 3);

// The vector contains {0, 1, 2}
assert([[v objectAtIndex:0] intValue] == 0);
assert([[v objectAtIndex:1] intValue] == 1);
assert([[v objectAtIndex:2] intValue] == 2);
)";

	auto swiftTestCode = R"()";

	auto errorCode =
	    stage.runObjcSwiftTest(cppCode, objcTestCode, swiftTestCode);
	REQUIRE(errorCode == 0);

	stage.exportAsExample("std::valarray");
}
