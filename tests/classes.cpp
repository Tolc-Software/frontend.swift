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
class WithConstructor {
public:
	explicit WithConstructor(int v) : m_v(v) {}

	int getV() { return m_v; }
private:
	int m_v;
};

class WithFunction {
public:
	int add(int i, int j) {
		return i + j;
	}
};
)";

	auto objCTestCode = R"(
// Member functions are available after construction
mWithFunction* withFunction = [[mWithFunction alloc] init];
assert([withFunction add: 2 j: 5] == 7);
)";

	auto swiftTestCode = R"(
// Member functions are available after construction
var with_function: m.WithFunction = m.WithFunction()
assert(with_function.add(i: 2, j: 5) == 7)
)";

	auto errorCode =
	    stage.runObjcSwiftTest(cppCode, objCTestCode, swiftTestCode);
	REQUIRE(errorCode == 0);

	stage.exportAsExample("Classes");
}
