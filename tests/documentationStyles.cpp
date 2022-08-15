#include "TestStage/paths.hpp"
#include "TestUtil/files.hpp"
#include "TestUtil/objcSwiftStage.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>

TEST_CASE("Classes", "[documentationStyles]") {
	std::string moduleName = "m";
	auto stage =
	    TestUtil::ObjcSwiftStage(TestStage::getRootStagePath(), moduleName);

	auto cppCode = R"(
// One line comment
class OneLiner {};

/** Single multi line comment */
void f() {}

struct Multi {
	/**
	* Multi
	* line
	* comment
	*/
	int multi;
};

/**
Bare multi
Another line
*/
enum class BareMulti {
	One,
	Two
};
)";

	auto objcTestCode = R"(
// These types of documentations are supported for:
//   Classes
//   Member variables
//   Enums
//   Functions
)";

	[[maybe_unused]] auto swiftTestCode = R"()";

	auto errorCode = stage.runObjcTest(cppCode, objcTestCode);
	REQUIRE(errorCode == 0);

	// stage.exportAsExample("Documentation Styles");
}
