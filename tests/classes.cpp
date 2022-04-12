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

class WithMember {
public:
	explicit WithMember() : i(10), phi(1.618) {}

	int i;
	double const phi;
};
)";

	auto objCTestCode = R"(
// Constructors are overloaded with their argument types
mWithConstructor* ten = [[mWithConstructor alloc] init];
assert([ten getV] == 10);

mWithConstructor* five = [[mWithConstructor alloc] initWithInt:5];
assert([five getV] == 5);

// Member functions are available after construction
mWithFunction* withFunction = [[mWithFunction alloc] init];
assert([withFunction add: 2 j: 5] == 7);

// Static functions can be called without instantiating the class
assert([mWithStatic getPi] == 3.14);

// Member variables
mWithMember* member = [[mWithMember alloc] init];
assert(member.i == 10);
// i is not marked const
member.i = 5;
assert(member.i == 5);

// phi is marked const
// Cannot be assigned
assert(member.phi == 1.618);
)";

	auto swiftTestCode = R"(
// Constructors in swift does not need different names
var ten: m.WithConstructor = m.WithConstructor()
assert(ten.getV() == 10)

var five: m.WithConstructor = m.WithConstructor(v: 5)
assert(five.getV() == 5)

// Member functions are available after construction
var withFunction: m.WithFunction = m.WithFunction()
assert(withFunction.add(i: 2, j: 5) == 7)

// Static functions can be called without instantiating the class
assert(m.WithStatic.getPi() == 3.14)
)";

	auto errorCode =
	    stage.runObjcSwiftTest(cppCode, objCTestCode, swiftTestCode);
	REQUIRE(errorCode == 0);

	stage.exportAsExample("Classes");
}
