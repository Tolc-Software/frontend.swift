#include "TestStage/paths.hpp"
#include "TestUtil/objcSwiftStage.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>

TEST_CASE("Write to file functions", "[functions]") {
	std::string moduleName = "m";
	auto stage =
	    TestUtil::ObjcSwiftStage(TestStage::getRootStagePath(), moduleName);

	auto cppCode = R"(
#include <filesystem>
#include <string>

int meaningOfLife() {
	return 42;
}

std::string sayHello(std::string const& name) {
	return "Hello " + name;
}

std::filesystem::path getPath() {
	return std::filesystem::path("/path/to/stuff.hpp");
}

namespace Inner {
	double pi() {
		return 3.14;
	}
}
)";

	auto objcTestCode = R"(
// Global functions gets added to
// a purely static class with
// the name of the library
assert([m meaningOfLife] == 42);

// Strings can be used
assert([[m sayHello:@"Tolc"] isEqualToString:@"Hello Tolc"]);

// Aswell as filesystem paths
assert([[m getPath] isEqualToString:@"/path/to/stuff.hpp"]);

// Functions within namespaces
// are available with the
// namespaces names merged
assert([mInner pi] == 3.14);
)";

	auto swiftTestCode = R"()";

	auto errorCode = stage.runObjcTest(cppCode, objcTestCode, swiftTestCode);
	REQUIRE(errorCode == 0);

	stage.exportAsExample("Functions");
}
