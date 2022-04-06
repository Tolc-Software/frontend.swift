#include "TestStage/paths.hpp"
#include "TestUtil/files.hpp"
#include "TestUtil/objcSwiftStage.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>

TEST_CASE("Classes", "[classes]") {
	std::string moduleName = "m";
	auto stage =
	    TestUtil::ObjcSwiftStage(TestStage::getRootStagePath(), moduleName);
	stage.keepAliveAfterTest();

	auto cppCode = R"(
class WithFunction {
public:
	int add(int i, int j) {
		return i + j;
	}
};
)";

	auto objCTestCode = R"(
// Member functions are available after construction
mWitFunction* withFunction = [[mWitFunction alloc] init];
assert([withFunction add: 2 j: 5] == 7);
)";

	auto swiftTestCode = R"(
// Member functions are available after construction
with_function = m.WithFunction()
assert(with_function.add(2, 5) == 7)
)";

	auto errorCode =
	    stage.runObjcSwiftTest(cppCode, objCTestCode, swiftTestCode);
	REQUIRE(errorCode == 0);

	stage.exportAsExample("Classes");
}
