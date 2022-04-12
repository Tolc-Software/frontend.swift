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
	explicit WithConstructor() : m_v(10) {}
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

class WithStatic {
public:
	static double getPi() {
		return 3.14;
	}
};
)";

	auto objCTestCode = R"(
// Constructors are overloaded with their argument types
mWithConstructor* defaultToTen = [[mWithConstructor alloc] init];
assert([defaultToTen getV] == 10);

mWithConstructor* withConstructor = [[mWithConstructor alloc] initWithInt:5];
assert([mWithConstructor getV] == 5);

// Member functions are available after construction
mWithFunction* withFunction = [[mWithFunction alloc] init];
assert([withFunction add: 2 j: 5] == 7);

// Static functions can be called without instantiating the class
assert([mWithFunction getPi] == 3.14);
)";

	auto swiftTestCode = R"(
// Constructors in swift does not need different names
var default_ten: m.WithConstructor = m.WithConstructor()
assert(default_ten.getV() == 10)

var five: m.WithConstructor = m.WithConstructor(v: 5)
assert(five.getV() == 5)

// Member functions are available after construction
var with_function: m.WithFunction = m.WithFunction()
assert(with_function.add(i: 2, j: 5) == 7)
)";

	auto errorCode =
	    stage.runObjcSwiftTest(cppCode, objCTestCode, swiftTestCode);
	REQUIRE(errorCode == 0);

	stage.exportAsExample("Classes");
}
