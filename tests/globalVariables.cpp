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
#include <string_view>

static int i = 0;
namespace Nested {
	int life = 42;
	std::string s = "Hello World";
	constexpr std::string_view constant = "A constant";
}
)";

	auto objcTestCode = R"(
// Starts at 0 and can be changed
assert(m.i == 0);
m.i = 5;
assert(m.i == 5);

// Nested with the same name
assert(mNested.life == 42);

// Strings also work
assert([mNested.s isEqualToString:@"Hello World"]);

// And string_view
assert([mNested.constant isEqualToString:@"A constant"]);
)";

	[[maybe_unused]] auto swiftTestCode = R"()";

	auto errorCode = stage.runTest(cppCode, objcTestCode, "objc");
	REQUIRE(errorCode == 0);

	stage.exportAsExample("Global Variables");
}
