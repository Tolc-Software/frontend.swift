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

std::vector<int> const f() {
	return {0, 1, 2};
}

double sum(std::vector<double> const& numbers) {
	double sum = 0;
	for (double number : numbers) {
		sum += number;
	}
	return sum;
}

)";

	auto objcTestCode = R"(
// std::vector corresponds to NSArray
NSArray* v = [m f];
assert([v count] == 3);

// The vector contains {0, 1, 2}
assert([[v objectAtIndex:0] intValue] == 0);
assert([[v objectAtIndex:1] intValue] == 1);
assert([[v objectAtIndex:2] intValue] == 2);

// Sending NSArray into function works as well
NSArray<NSNumber*>* toSum = @[@(1.1), @(2.2), @(3.3)];
assert([m sum:toSum] == 6.6);
)";

	auto swiftTestCode = R"()";

	auto errorCode =
	    stage.runObjcSwiftTest(cppCode, objcTestCode, swiftTestCode);
	REQUIRE(errorCode == 0);

	stage.exportAsExample("std::vector");
}
