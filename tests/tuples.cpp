#include "TestStage/paths.hpp"
#include "TestUtil/objcSwiftStage.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>

TEST_CASE("Using std::tuples", "[tuples]") {
	std::string moduleName = "m";
	auto stage =
	    TestUtil::ObjcSwiftStage(TestStage::getRootStagePath(), moduleName);
	stage.keepAliveAfterTest();

	auto cppCode = R"(
#include <string>
#include <tuple>

std::tuple<int, std::string> sumInts(std::tuple<int, int, std::string> t) {
  // Sum the first two elements
  return {
    std::get<0>(t) + std::get<1>(t),
    std::get<2>(t)
  };
}
)";

	auto objcTestCode = R"(
// std::tuple corresponds to a NSArray
// with the same amount of values
NSArray* toSum = [NSArray
  arrayWithObjects:@(1), @(2), @"Hello", nil];
assert([toSum count] == 3);

// Sending a tuple to a function
NSArray* summed = [m sumInts:toSum];
assert([summed count] == 2);
assert([[summed objectAtIndex:0] intValue] == 3);
assert([[summed objectAtIndex:1] isEqualToString:@"Hello"]);

// Error handling
@try {
  // Sending an array with size != 3
  NSArray* tooManyArgs =
    [toSum arrayByAddingObject:@"Boom"];
  [m sumInts:tooManyArgs];
  // Should throw exception before
  assert(NO);
} @catch(NSException* error) {
  assert([[error name] isEqualToString:@"TypeException"]);
  NSString* reason =
    @"The array passed does not match the number of types expected in the tuple. Expected: 3, Got: 4.";
  assert([[error reason] isEqualToString:reason]);
}
)";

	auto swiftTestCode = R"()";

	auto errorCode =
	    stage.runObjcSwiftTest(cppCode, objcTestCode, swiftTestCode);
	REQUIRE(errorCode == 0);

	stage.exportAsExample("std::tuple");
}
