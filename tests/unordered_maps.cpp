#include "TestStage/paths.hpp"
#include "TestUtil/objcSwiftStage.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>

TEST_CASE("Using std::unordered_maps", "[unordered_maps]") {
	std::string moduleName = "m";
	auto stage =
	    TestUtil::ObjcSwiftStage(TestStage::getRootStagePath(), moduleName);

	auto cppCode = R"(
#include <string>
#include <unordered_map>

std::unordered_map<std::string, int>
getUnordered() {
  return {{"Unordered", 1}};
}
)";

	auto objcTestCode = R"(
// std::unordered_map translates to a NSDictionary
NSDictionary* dict = [m getUnordered];
assert([dict count] == 1);
NSNumber* n = [dict objectForKey:@"Unordered"];
assert(n != nil);
assert([n intValue] == 1);
)";

	auto swiftTestCode = R"()";

	auto errorCode =
	    stage.runObjcSwiftTest(cppCode, objcTestCode, swiftTestCode);
	REQUIRE(errorCode == 0);

	stage.exportAsExample("std::unordered_map");
}
