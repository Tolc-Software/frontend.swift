#include "TestStage/paths.hpp"
#include "TestUtil/objcSwiftStage.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>

TEST_CASE("User defined classes", "[userDefined]") {
	std::string moduleName = "m";
	auto stage =
	    TestUtil::ObjcSwiftStage(TestStage::getRootStagePath(), moduleName);
	stage.keepAliveAfterTest();

	auto cppCode = R"(
#include <string>

class MyClass {
public:
	explicit MyClass(std::string s) : m_s(s) {}

	std::string* getS() { return &m_s; }

private:
	std::string m_s;
};

MyClass buildMyClass(std::string const& s) {
	return MyClass(s);
}

class Owner {
public:
	explicit Owner(MyClass m) : m_myClass(m) {};

	MyClass getMyClass() const { return m_myClass; }

private:
	MyClass m_myClass;
};

struct Point {
	int x;
	int y;
};

Point getMiddle(std::pair<Point, Point> p) {
	return {(p.first.x + p.second.x) / 2, (p.first.y + p.second.y) / 2};
}
)";

	auto objcTestCode = R"(
NSString* phrase = @"Hello from Objective-C";
mMyClass* myClass = [m buildMyClass:phrase];
assert([[myClass getS] isEqualToString:phrase]);

// Passing Objective-C classes to C++ classes
mOwner* owner = [[mOwner alloc] initWithMyClass:myClass];
assert([[[owner getMyClass] getS] isEqualToString:phrase]);

// Container of user defined classes
mPoint* a = [[mPoint alloc] init];
a.x = 1;
a.y = 0;
mPoint* b = [[mPoint alloc] init];
b.x = 2;
b.y = 0;
// NSArray* points
)";

	auto swiftTestCode = R"()";

	auto errorCode =
	    stage.runObjcSwiftTest(cppCode, objcTestCode, swiftTestCode);
	REQUIRE(errorCode == 0);

	stage.exportAsExample("Passing classes between languages");
}
